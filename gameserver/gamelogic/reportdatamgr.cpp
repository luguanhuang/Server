#include "pch.h"
#include "reportdatamgr.h"
#include "httptask/reportdata.h"
#include "util/gametime.h"
#include "util.h"
#include "httptask/httptaskmgr.h"
#include "pb/project.pb.h"
#include "gamelogic/pkrecord.h"
#include "pvprecord.h"
#include "atlas.h"
#include "define/tlogenum.h"
#include "herobattlerecord.h"
#include "teamrecord.h"
#include "define/teamdef.h"
#include "rolefashion.h"

INSTANCE_SINGLETON(ReportDataMgr)

ReportDataMgr::ReportDataMgr()
{
	m_bIsQQ = false;
}

ReportDataMgr::~ReportDataMgr()
{

}

bool ReportDataMgr::Init()
{
	if (!m_oTable.LoadFile("table/ReportDataNest.txt"))
	{
		LogWarn("Load table %s failed!", "table/ReportDataNest.txt");
		return false;
	}

	if (!m_oRecommondTable.LoadFile("table/RecommendFightNum.txt"))
	{
		LogWarn("Load table %s failed!", "table/RecommendFightNum.txt");
		return false;
	}
	return true;
}

void ReportDataMgr::Uninit()
{

}

void ReportDataMgr::DailyOnline(Role* role, INT32 iOnlineTime)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(Tx_Reportdata_DailyOnlineTime, ToString(iOnlineTime));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::AtlasActive(Role* role, UINT32 groupId, UINT32 Num)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	if (groupId != 1 && groupId != 2)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	if (groupId == 1)//阿尔特里亚;
	{
		poTask->AddData(Tx_Reportdata_CardsGroup1Num, ToString(Num));
	}
	else //塞尼亚;
	{
		poTask->AddData(Tx_Reportdata_CardsGroup2Num, ToString(Num));
	}
	poTask->AddData(Tx_Reportdata_CardsGroupAddAttr, ToString(role->Get<CAtlasSys>()->GetAtlasPowerPoint()));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::PkChange(Role* role, KKSG::PkResultType result)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	if (result == KKSG::PkResult_Win || result == KKSG::PkResult_Lose)
	{
		poTask->AddData(Tx_Reportdata_PkPoint, ToString(role->Get<CPkRecord>()->GetPoint()));
	}
	UINT32 win = role->Get<CPkRecord>()->GetWin();
	UINT32 total = role->Get<CPkRecord>()->GetCont();
	poTask->AddData(Tx_Reportdata_PkTotalNum, ToString(total));
	poTask->AddData(Tx_Reportdata_PkWinRate, GetWinRate(win, total));
	poTask->AddData(Tx_Reportdata_PkResult, ToString(result-1));
	poTask->AddData(Tx_Reportdata_PKDayTimes, ToString(role->Get<CPkRecord>()->GetTodayPlayTimes()));
	poTask->AddData(Tx_Reportdata_PKWeekTimes, ToString(role->Get<CPkRecord>()->GetWeekPlayTimes()));

	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::PvpChange(Role* role,  KKSG::PvpOneRec& rec)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	KKSG::PkResultType iResult = KKSG::PkResult_Win;
	if (rec.wincount() > rec.losecount())
	{
		iResult = KKSG::PkResult_Win;
	}
	else if (rec.wincount() < rec.losecount())
	{
		iResult = KKSG::PkResult_Lose;
	}
	else 
	{
		iResult = KKSG::PkResult_Draw;
	}
	INT32 win = role->Get<MyPvpRecord>()->GetWinCountAll();
	UINT32 total = role->Get<MyPvpRecord>()->GetCountAll();
	poTask->AddData(Tx_Reportdata_PvpTotalNum, ToString(total));
	poTask->AddData(Tx_Reportdata_PvpWinRate, GetWinRate(win, total));
	poTask->AddData(Tx_Reportdata_PvpResult, ToString(iResult-1));

	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::TowerChange(Role* role, INT32 iFloor)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(Tx_Reportdata_TowerFloor, ToString(iFloor));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}


void ReportDataMgr::NestFirstPass(Role* role, INT32 id)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	auto it = m_oTable.GetBySceneID(id);
	if (NULL == it || it->FirstPassReportID == 0)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(it->FirstPassReportID, ToString(it->HardLevel));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::NestResult(Role* role, INT32 id, INT32 result)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	auto it = m_oTable.GetBySceneID(id);
	if (NULL == it || (it->PlayReportID == 0 && it->WinReportID == 0))
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	if (it->PlayReportID)
	{
		poTask->AddData(it->PlayReportID, ToString(it->HardLevel));
	}
	if (result == TX_BATTLE_SUCCESS && it->WinReportID)
	{
		poTask->AddData(it->WinReportID, ToString(it->HardLevel));
	}
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::PvpTime(Role* role, UINT32 iTime)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(Tx_Reportdata_PvpDailyTime, ToString(iTime));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::PkTime(Role* role, UINT32 iTime)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(Tx_Reportdata_PkDailyTime, ToString(iTime));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::Within20Mins(Role* role)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(Tx_Reportdata_2DayWithin20Mins, "0");
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::OnChangePPT(Role* pRole)
{
	if (NULL == pRole || pRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(pRole);
	poTask->AddData(Tx_Reportdata_PowerPoint, ToString((UINT32)pRole->GetAttr(TOTAL_POWERPOINT)));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::HeroBattleResult(Role* pRole, UINT32 resoult, UINT32 killnum, UINT32 deadnum)
{
	if (NULL == pRole || pRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}

	ReportDataTask* poTask = new ReportDataTask(pRole);
	poTask->AddData(Tx_Reportdata_HeroResoult, ToString(resoult));
	poTask->AddData(Tx_Reportdata_HeroBattleTimes, ToString(pRole->Get<CHeroBattleRecord>()->GetDayTimes()));
	std::string strKD = "1";
	if (deadnum)
	{
		double kd = INT32(killnum*100/deadnum);
		kd /= 100;
		strKD = ToString(kd);
	}
	poTask->AddData(Tx_Reportdata_HeroBattleKD, strKD);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
void ReportDataMgr::WeekNest(Role* pRole, UINT32 iTime, UINT32 Count)
{
	if (NULL == pRole || pRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(pRole);
	if (iTime)
	{
		poTask->AddData(Tx_Reportdata_NestFastPassTime, ToString(iTime));
	}
	poTask->AddData(Tx_Reportdata_NestDayTimes, ToString(Count));
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
void ReportDataMgr::Common(Role* role, INT32 type, const string& data)
{
	if (NULL == role || role->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(role);
	poTask->AddData(type, data);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

std::string ReportDataMgr::GetWinRate(UINT32 win, UINT32 total)
{
	UINT32 winrate = total?UINT32((win*100)/total):0;
	std::ostringstream ss;
	ss << winrate << "%";
	return ss.str();
}

UINT32 ReportDataMgr::GetRecommendPoint(UINT32 level)
{
	for(auto i = m_oRecommondTable.Table.begin(); i != m_oRecommondTable.Table.end(); i ++)
	{
		if ((*i)->Level == level)
		{
			return (*i)->Total;
		}
	}
	return 0;
}

void ReportDataMgr::MobaBattleResult(Role* pRole, UINT32 result, UINT32 killnum, UINT32 deadnum)
{
	if (NULL == pRole || pRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}

	ReportDataTask* poTask = new ReportDataTask(pRole);
	poTask->AddData(Tx_Reportdata_MobaResult, ToString(result));
	std::string strKD = "1";
	if (deadnum)
	{
		double kd = INT32(killnum*100/deadnum);
		kd /= 100;
		strKD = ToString(kd);
	}
	poTask->AddData(Tx_Reportdata_MobaKD, strKD);
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}

void ReportDataMgr::FashionCompose(Role* pRole, bool isSuccess, UINT32 quality)
{
	if (NULL == pRole || pRole->GetClientConf().logintype() != KKSG::LOGIN_QQ_PF)
	{
		return;
	}
	ReportDataTask* poTask = new ReportDataTask(pRole);
	poTask->AddData(Tx_Reportdata_ComposeCount, ToString(pRole->Get<RoleFashion>()->GetComposeNum()));
	if (isSuccess)
	{
		poTask->AddData(Tx_Reportdata_ComposeSuccessCount, ToString(pRole->Get<RoleFashion>()->GetComposeSuccessNum()));
		if (quality == FashionAQuality || quality == FashionSQuality)
		{
			UINT32 num = pRole->Get<RoleFashion>()->GetCompseSucessNumByQuality(quality);
			UINT32 id = quality==FashionAQuality?Tx_Reportdata_ComposeSuccessACount:Tx_Reportdata_ComposeSuccessSCount;
			poTask->AddData(id, ToString(num));
		}
	}
	else
	{
		poTask->AddData(Tx_Reportdata_ComposeFailCount, ToString(pRole->Get<RoleFashion>()->GetComposeFailedNum()));
	}
	poTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(poTask);
}
