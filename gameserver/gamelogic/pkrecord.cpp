#include "pch.h"
#include "pkrecord.h"
#include "time.h"
#include "util/XCommon.h"
#include "pb/project.pb.h"
#include "globalconfig.h"
#include "globalconfig.h"
#include "table/ProfessionMgr.h"
#include "reportdatamgr.h"
#include "popwindows.h"
#include "util/gametime.h"

#define RECORD_MAX 20 

CPkRecord::CPkRecord(Role* role)
{
	m_day = 0;
	m_week = 0;
	m_oneWeekResetTime = 0;
	m_todayplaytime = 0;
	m_poRole = role;
	m_todayplaytimes = 0;
	m_weekplaytimes = 0;
	m_last7daytime = 0;
	m_point = (UINT32) GetGlobalConfig().PkBasePoint;
	WeekReset();
	DayReset();
	m_lastWeekPoint = 0;
	//OneWeekReset();
}

bool CPkRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_pkrecord())
	{
		const KKSG::PkRecord& record = poRoleAllInfo->pkrecord();
		m_point = record.point();
		m_week = record.week();
		m_day = record.day();
		m_oneWeekResetTime = record.oneweekresettime();
		m_lastWeekPoint = record.lastweekpoint();
		m_dayRewardCount = record.rewardcounttoday();
		m_honorpoint = record.honorpoint();
		m_pointlastlose = record.pointlastlose();
		m_todayplaytime = record.todayplaytime();
		m_todayplaytimes = record.pkdaytimes();
		m_weekplaytimes = record.weektimes();
		m_last7daytime = record.last7daystime();
		m_baseHistWeek.Load(record.histweek());
		m_baseHistAll.Load(record.histall());
		m_baseHistDay.Load(record.histday());
		for (int i = 0; i < record.boxtaken_size(); ++i)
		{
			m_boxtaken.insert(record.boxtaken(i));
		}
		if (record.prowin_size() == record.prolose_size() && record.prowin_size() == record.prodraw_size())
		{
			for (int i = 0; i < record.prowin_size() && i < PROFESSION_MAX; ++i)
			{
				m_results[i].win = record.prowin(i);	
				m_results[i].lose = record.prolose(i);	
				m_results[i].draw = record.prodraw(i);
			}
		}
		for (int i = 0; i < record.records_size(); ++i)
		{
			const KKSG::PkOneRecord& onerecord = record.records(i);
			SPkOneRecord temprecord;
			temprecord.opposer = onerecord.opposer();
			temprecord.profession = onerecord.profession();
			temprecord.name = onerecord.name();
			temprecord.result = onerecord.result();
			temprecord.point = onerecord.point();
			temprecord.honorpoint = onerecord.honorpoint();
			m_records.push_back(temprecord);
		}

		if(record.has_info2v2())//线上老号无需赋值 用初始化的就可以
		{
			m_info2v2 = record.info2v2();
		}
		else
		{
			SSInfo << " online first time role = " << m_poRole->GetID() << END;
			m_ismodify.Set();//changed
		}

		Update();
	}
	return true;
}

void CPkRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_ismodify.TestAndReset())
	{
		KKSG::PkRecord* record = poRoleAllInfo->mutable_pkrecord();
		roChanged.insert(record);
		record->Clear();
		ToKKSG(record);
	}
}

void CPkRecord::ToKKSG(KKSG::PkRecord* record)
{
	record->set_point(m_point);
	record->set_week(m_week);
	record->set_day(m_day);
	record->set_oneweekresettime(m_oneWeekResetTime);
	record->set_lastweekpoint(m_lastWeekPoint);
	record->set_rewardcounttoday(m_dayRewardCount);
	record->set_honorpoint(m_honorpoint);
	record->set_pointlastlose(m_pointlastlose);
	record->set_todayplaytime(m_todayplaytime);
	record->set_pkdaytimes(m_todayplaytimes);
	record->set_weektimes(m_weekplaytimes);
	record->set_last7daystime(m_last7daytime);
	m_baseHistWeek.Save(*record->mutable_histweek());
	m_baseHistAll.Save(*record->mutable_histall());
	m_baseHistDay.Save(*record->mutable_histday());
	for (auto i = m_boxtaken.begin(); i != m_boxtaken.end(); ++i)
	{
		record->add_boxtaken(*i);
	}
	for (int i = 0; i < PROFESSION_MAX; ++i)
	{
		record->add_prowin(m_results[i].win);
		record->add_prolose(m_results[i].lose);
		record->add_prodraw(m_results[i].draw);
	}
	for (auto i = m_records.begin(); i != m_records.end(); ++i)
	{
		KKSG::PkOneRecord* onerecord = record->add_records();
		onerecord->set_opposer(i->opposer);
		onerecord->set_profession(i->profession);
		onerecord->set_name(i->name);
		onerecord->set_point(i->point);
		onerecord->set_honorpoint(i->honorpoint);
		onerecord->set_result(i->result);
	}

	*record->mutable_info2v2() = m_info2v2;
}

void CPkRecord::DayReset()
{
	if (m_todayplaytime)
	{
		ReportDataMgr::Instance()->PkTime(m_poRole, m_todayplaytime);
		m_todayplaytime = 0;
	}
	m_dayRewardCount = 0;
	m_todayplaytimes = 0;
	if (XCommon::GetDiffDayCount(XCommon::GetDayBeginTime(time_t(m_last7daytime)), XCommon::GetTodayBeginTime()) >= 7)
	{
		m_weekplaytimes = 0;
		m_last7daytime = UINT32(GameTime::GetTime());
	}

	m_baseHistDay.Reset();

	ResetDayInfo(m_info2v2);
}

void CPkRecord::WeekReset()
{
	UINT32 newPoint = (UINT32) GetGlobalConfig().PkBasePoint;
	const auto& ResetPoint = GetGlobalConfig().PkResetPoint;
	for(size_t i = 0; i < ResetPoint.size(); ++i)
	{
		if(m_point < ResetPoint[i][0])
		{
			break;
		}
		newPoint = ResetPoint[i][1];
	}
	m_point = newPoint;

	m_honorpoint = 0;
	m_pointlastlose = 0;
	m_baseHistWeek.Reset();
	m_boxtaken.clear();
	m_records.clear();
	memset(m_results, 0, sizeof(m_results));

	ResetWeekInfo(m_info2v2);
}
void CPkRecord::OneWeekReset()
{
	m_lastWeekPoint = GetPoint();
}

void CPkRecord::Update()
{
	time_t now = GameTime::GetTime();
	if(0 == m_week)
	{
		m_week = UINT32(now);
		m_ismodify.Set();
	}
	if(0 == m_day)
	{
		m_day = UINT32(now);
		m_ismodify.Set();
	}
	if(0 == m_oneWeekResetTime)
	{
		m_oneWeekResetTime = UINT32(now);
		m_ismodify.Set();
	}

	if(!XCommon::IsWeekSame(m_oneWeekResetTime, now))
	{
		OneWeekReset();
		m_oneWeekResetTime = UINT32(now);
		m_ismodify.Set();
	}

	if(GetGlobalConfig().PkGetResetLeftTime(m_week, UINT32(now)) == 0)
	{
		WeekReset();	
		m_week = (UINT32)now;
		m_ismodify.Set();
	}

	if (!XCommon::IsDaySame(time_t(m_day), now))
	{
		DayReset();
		m_day = UINT32(now);
		m_ismodify.Set();
	}
}

void CPkRecord::AddPoint(UINT64 roleid, UINT32 point)
{	
	m_point += point;
	m_ismodify.Set();

	//CRankListMgr::Instance()->OnChangePoint(roleid, m_point, (UINT32)time(NULL));
}

void CPkRecord::SubPoint(UINT64 roleid, UINT32 point)
{
	m_point = m_point <= point ? 0 : m_point - point;
	m_ismodify.Set();
	//CRankListMgr::Instance()->OnChangePoint(roleid, m_point, (UINT32)time(NULL));
}

void CPkRecord::SetPoint(UINT32 point)
{
	m_point = point;
	m_ismodify.Set();
}

void CPkRecord::AddWinNum()
{
	m_pointlastlose = 0;

	m_baseHistWeek.AddWinNum();
	m_baseHistAll.AddWinNum();
	m_baseHistDay.AddWinNum();

	m_ismodify.Set();
	PopWindows::Instance()->CheckComment(m_poRole, KKSG::COMMENT_LADDER,m_baseHistWeek.m_continuewin);
}

void CPkRecord::AddLoseNum()
{
	if(GetPoint() < GetGlobalConfig().PkAddRobotPoint[0])
	{
		++m_pointlastlose;
	}

	m_baseHistWeek.AddLoseNum();
	m_baseHistAll.AddLoseNum();
	m_baseHistDay.AddLoseNum();

	m_ismodify.Set();
}

void CPkRecord::AddDrawNum()
{
	m_pointlastlose = 0;

	m_baseHistWeek.AddDrawNum();
	m_baseHistAll.AddDrawNum();
	m_baseHistDay.AddDrawNum();

	m_ismodify.Set();
}

void CPkRecord::PushRecord(UINT64 roleid, UINT32 profession, std::string name, int point, UINT32 honorpoint, KKSG::PkResultType result)
{
	if (m_records.size() >= RECORD_MAX)		
	{
		m_records.pop_front();
	}
	SPkOneRecord record;
	record.opposer = roleid;
	record.profession = profession;
	record.name = name;
	record.point = point;
	record.honorpoint = honorpoint;
	record.result = result;
	m_records.push_back(record);

	profession = ProfessionMgr::Instance()->GetBasicProfession(profession) - 1;

	if (profession >= 0 && profession < PROFESSION_MAX)
	{
		if (result == KKSG::PkResult_Win)
		{
			++m_results[profession].win;
		}
		else if (result == KKSG::PkResult_Lose)
		{
			++m_results[profession].lose;
		}
		else 
		{
			++m_results[profession].draw;
		}
	}
	else
	{
		SSWarn<<"invalid profession:"<<profession<<END;
	}

	m_ismodify.Set();
}

//UINT32 CPkRecord::AddHonorPoint(bool iswin)
//{
//	UINT32 honorpoint = 0;
//	UINT32 honorcount = 0;
//	if (m_win + m_lose + m_draw <=  GetGlobalConfig().PkHonorPoint[0])
//	{
//		honorpoint +=  GetGlobalConfig().PkHonorPoint[1];		
//		honorcount = m_win + m_lose + m_draw;
//	}
//	else
//	{
//		honorcount =  GetGlobalConfig().PkHonorPoint[0];
//	}
//	m_honorpoint += honorpoint;
//
//	UINT32 random = XRandom::randInt( GetGlobalConfig().PkHonorPoint[3],  GetGlobalConfig().PkHonorPoint[4] + 1);
//	if (!iswin)
//	{
//		random = random/2;
//	}
//	UINT32 pointlimit =  GetGlobalConfig().PkHonorPoint[2] + honorcount *  GetGlobalConfig().PkHonorPoint[1];
//	if (random + m_honorpoint > pointlimit)
//	{
//		random = (pointlimit > m_honorpoint) ? pointlimit - m_honorpoint : 0;
//	}
//	m_honorpoint += random;
//
//	m_ismodify.Set();
//
//	return random + honorpoint;
//}

//UINT32 CPkRecord::GetRewardCount()
//{
//	if (m_win + m_lose + m_draw >=  GetGlobalConfig().PkHonorPoint[0])
//	{
//		return 0;
//	}
//	else
//	{
//		return  GetGlobalConfig().PkHonorPoint[0] - m_win - m_lose - m_draw;
//	}
//}

bool CPkRecord::IsBoxTaken(UINT32 index)
{
	auto i = m_boxtaken.find(index);
	return i != m_boxtaken.end();
}

void CPkRecord::SetBoxTaken(UINT32 index)
{
	m_boxtaken.insert(index);
	m_ismodify.Set();
}

void CPkRecord::SetWin( UINT32 winTimes )
{
	m_baseHistWeek.m_win = winTimes;
	m_ismodify.Set();
}

bool CPkRecord::NeedAddRobot()
{
	return (m_pointlastlose >= GetGlobalConfig().PkAddRobotPoint[1]);
}

void CPkRecord::FillPkRec(KKSG::PkRoleRec& data, KKSG::PkNVNType type)
{
	if(KKSG::PK_1v1 == type)
	{
		data.set_point(m_point);
		data.set_win(m_baseHistWeek.m_win);
		data.set_lose(m_baseHistWeek.m_lose);
		data.set_draw(m_baseHistWeek.m_draw);
		for(auto it = m_records.begin(); it != m_records.end(); ++it)
		{
			data.add_records((*it).result);
		}
	}
	else
	{
		auto pSubInfo = GetSubInfo(type);
		if(pSubInfo)
		{
			data.set_point(pSubInfo->point());
			data.set_win(pSubInfo->seasondata().win());
			data.set_lose(pSubInfo->seasondata().lose());
			data.set_draw(pSubInfo->seasondata().draw());
			//record TODO
		}
	}
}

UINT32 CPkRecord::GetRewardCount()
{
	return m_dayRewardCount;
}

void CPkRecord::AddRewardCount()
{
	++ m_dayRewardCount;
	m_ismodify.Set();
}

UINT32 CPkRecord::GetRewradLeftCount()
{
	UINT32 maxCount = GetGlobalConfig().PkDayReward[0];	
	if(maxCount > m_dayRewardCount)
	{
		return maxCount - m_dayRewardCount;
	}
	return 0;
}

void CPkRecord::AddHonorPoint(UINT32 hp)
{
	m_honorpoint += hp;
	m_ismodify.Set();
}

void CPkRecord::AddPlayTime(UINT32 iTime)
{
	Update();
	m_todayplaytime += iTime;
	m_todayplaytimes ++;
	m_weekplaytimes ++;
	m_ismodify.Set();
}

void CPkRecord::FillRankData(KKSG::PkRankData& data, KKSG::PkNVNType type)
{
	data.set_roleid(m_poRole->GetID());
	if(KKSG::PK_1v1 == type)
	{
		data.set_point(GetPoint());
		data.mutable_extradata()->set_joincount(m_baseHistWeek.GetAllNum());
		data.mutable_extradata()->set_winrate(m_baseHistWeek.GetWinRate());
		data.mutable_extradata()->set_continuewin(m_baseHistWeek.m_continuewin);
	}
	else
	{
		auto pSubInfo = GetSubInfo(type);
		if(pSubInfo)
		{
			data.set_point(pSubInfo->point());
			PkBaseHistory baseHist;
			baseHist.Load(pSubInfo->seasondata());
			data.mutable_extradata()->set_joincount(baseHist.GetAllNum());
			data.mutable_extradata()->set_winrate(baseHist.GetWinRate());
			data.mutable_extradata()->set_continuewin(baseHist.m_continuewin);
		}
	}
}

void CPkRecord::ResetDayInfo(KKSG::PkRecordSubInfo& info)
{
	info.set_rewardcount(0);
	m_ismodify.Set();
}

void CPkRecord::ResetWeekInfo(KKSG::PkRecordSubInfo& info)
{
	UINT32 point = (UINT32) GetGlobalConfig().PkBasePoint;
	info.set_point(point);
	info.clear_seasondata();
	info.mutable_seasondata();//不能为NULL

	m_ismodify.Set();
}

void CPkRecord::AddResult(KKSG::PkNVNType type, KKSG::PkOneRec& rec)
{
	auto pInfo = GetSubInfo(type);
	if(NULL == pInfo)
	{
		return;
	}

	auto& sdata = *pInfo->mutable_seasondata();
	PkBaseHistory pkhist;//临时
	pkhist.Load(sdata);
	switch(rec.ret())
	{
	case KKSG::PkResult_Win:
		{
			pkhist.AddWinNum();	
		}
		break;
	case KKSG::PkResult_Lose:
		{
			pkhist.AddLoseNum();
		}
		break;
	case KKSG::PkResult_Draw:
		{
			pkhist.AddDrawNum();
		}
		break;
	default:
		break;
	}
	pkhist.Save(sdata);

	*pInfo->add_recs() = rec;
	//移到最前
	for(int i = pInfo->recs_size() - 1; i > 0; --i)
	{
		pInfo->mutable_recs()->SwapElements(i, i - 1);
	}

	if(pInfo->recs_size() > RECORD_MAX)
	{
		pInfo->mutable_recs()->RemoveLast();	
	}

	m_ismodify.Set();
}

KKSG::PkRecordSubInfo* CPkRecord::GetSubInfo(KKSG::PkNVNType type)
{
	switch(type)
	{
	case KKSG::PK_2v2:
		{
			return &m_info2v2;
		}
		break;
	default:
		break;
	}

	return NULL;
}

void CPkRecord::AddPoint(KKSG::PkNVNType type, int point)
{
	auto pInfo = GetSubInfo(type);
	if(NULL == pInfo)
	{
		return;
	}
	int oldPoint = pInfo->point();
	int newPoint = oldPoint + point;
	if(newPoint <= 0)
	{
		newPoint = 0;
	}
	pInfo->set_point(newPoint);
	m_ismodify.Set();
}

//UINT32 CPkRecord::GetRewardLeftCount(KKSG::PkNVNType type)
//{
//	auto pInfo = GetSubInfo(type);	
//	if(NULL == pInfo)
//	{
//		return 0;
//	}
//	UINT32 maxCount = GetGlobalConfig().PkDayReward[0];	
//	if(maxCount > pInfo->rewardcount())
//	{
//		return maxCount - pInfo->rewardcount();
//	}
//	return 0;
//}
//
//void CPkRecord::AddRewardCount(KKSG::PkNVNType type)
//{
//	auto pInfo = GetSubInfo(type);	
//	if(NULL == pInfo)
//	{
//		return;
//	}
//	pInfo->set_rewardcount(pInfo->rewardcount() + 1);
//	m_ismodify.Set();
//}

UINT32 CPkRecord::GetMaxPoint()
{
	UINT32 point = GetPoint();//1v1
	for(int i = KKSG::PK_2v2; i <= KKSG::PkNVNType_MAX; ++i)
	{
		UINT32 tmp = GetPoint(KKSG::PkNVNType(i));
		if(tmp > point)
		{
			point = tmp;
		}
	}
	return point;
}

UINT32 CPkRecord::GetPoint(KKSG::PkNVNType type)
{
	auto pInfo = GetSubInfo(type);
	if(pInfo)
	{
		return pInfo->point();
	}
	return 0;
}

void CPkRecord::NVNInfoPrint(KKSG::PkNVNType type)
{
	auto pInfo = GetSubInfo(type);
	if(NULL == pInfo)
	{
		return;
	}
	SSInfo << " role = " << m_poRole->GetID() << " point = " << pInfo->point() << END;
	PkBaseHistory hist;
	hist.Load(pInfo->seasondata());
	SSInfo << " win = " << hist.m_win << " lose = " << hist.m_lose << " draw = " << hist.m_draw << END;
}

void CPkRecord::TestReset()
{
	m_day = 0;
	m_week = 0;
	m_ismodify.Set();
}

//针对跨周登录玩家周重置时间有可能和全局不一致的处理
void CPkRecord::PkWeekTimeSyn(UINT32 ctime)
{
	SSInfo << " pktime syn role = " << m_poRole->GetID() << " ctime = " << ctime << " week = " << m_week << END;
	if(0 == ctime)
	{
		SSError << " ctime = 0 " << END;
		return;
	}
	if(0 == m_week)
	{
		SSError << " m_week = 0 " << END;
		return;
	}

	UINT32 offset_ctime = ctime + GetGlobalConfig().PkWeekOffset;
	UINT32 offset_week = m_week + GetGlobalConfig().PkWeekOffset;
	if(!XCommon::IsWeekSame(offset_ctime, offset_week, false))//偏差有一周 需要矫正
	{
		SSInfo << " week time syn to same week role = " << m_poRole->GetID() << " time = " << ctime << END;
		m_week = ctime;

		m_boxtaken.clear();//对线上已有问题账号进行评分奖励重置 后续也不影响正常流程

		m_ismodify.Set();
	}
}

UINT32 CPkRecord::GetLastWeekPoint()
{
	Update();
	return m_lastWeekPoint;
}

UINT32 CPkRecord::GetContWinDay()
{
	Update();
	return m_baseHistDay.GetContinueWin();
}

PkBaseHistory::PkBaseHistory()
{
	Reset();
}

void PkBaseHistory::Reset()
{
	m_win = 0;
	m_lose = 0;
	m_draw = 0;
	m_lastwin = 0;
	m_lastlose = 0;
	m_continuewin = 0;
	m_continuelose = 0;
}

void PkBaseHistory::Save(KKSG::PkBaseHist& data)
{
	data.set_win(m_win);
	data.set_lose(m_lose);
	data.set_draw(m_draw);
	data.set_lastwin(m_lastwin);
	data.set_lastlose(m_lastlose);
	data.set_continuewin(m_continuewin);
	data.set_continuelose(m_continuelose);
}

void PkBaseHistory::Load(const KKSG::PkBaseHist& data)
{
	m_win = data.win();
	m_lose = data.lose();
	m_draw = data.draw();
	m_lastwin = data.lastwin();
	m_lastlose = data.lastlose();
	m_continuewin = data.continuewin();
	m_continuelose = data.continuelose();
}

void PkBaseHistory::AddWinNum()
{
	++m_win;
	++m_lastwin;
	m_lastlose = 0;
	if (m_lastwin > m_continuewin)
	{
		m_continuewin = m_lastwin;
	}

}

void PkBaseHistory::AddLoseNum()
{
	++m_lose;
	++m_lastlose;
	m_lastwin = 0;
	if (m_lastlose > m_continuelose)
	{
		m_continuelose = m_lastlose;
	}
}

void PkBaseHistory::AddDrawNum()
{
	++m_draw;
	m_lastwin = 0;
	m_lastlose = 0;
}

UINT32 PkBaseHistory::GetAllNum()
{
	return m_win + m_lose + m_draw;
}

UINT32 PkBaseHistory::GetContinueWin()
{
	return m_continuewin;
}

float PkBaseHistory::GetWinRate()
{
	UINT32 allNum = GetAllNum();
	if(0 == allNum)
	{
		return 0.0;
	}
	return float((1.0*m_win)/allNum);
}
