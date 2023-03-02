#include "pch.h"
#include "dragonrecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include <time.h>
#include "gamelogic/bagtransition.h"
#include "foreach.h"
#include "table/DragonNestTable.h"
#include "gamelogic/dragonconfig.h"
#include <time.h>
#include "util/gametime.h"
#include "scene/sceneteam.h"
#include "scene/scene.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/stagemgr.h"

CDragonRecord::CDragonRecord(Role* role)
{
	m_pRole = role;
	m_updateDay = 0;
	m_updateHardDragonDay = 0;
	m_updateSmallDragonDay = 0;
}

CDragonRecord::~CDragonRecord()
{

}

bool CDragonRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_dragoninfo())
	{
		const KKSG::DragonRecord2DB* dbInfo = &poRoleAllInfo->dragoninfo();
		m_updateDay = dbInfo->updateday();
		m_updateHardDragonDay = dbInfo->updateharddragonday();
		m_updateSmallDragonDay = dbInfo->updatesmalldragonday();
		for (int i = 0; i < dbInfo->record_size(); ++i)
		{
			int dragonType = dbInfo->record(i).dragontype();

			DragonHardInfo hardInfo;
			hardInfo.hardLevel = dbInfo->record(i).hardlevel();
			hardInfo.reachTopFloor = dbInfo->record(i).curfloor();
			hardInfo.dragonDiamondBoxTimes = dbInfo->record(i).dragondiamondboxtimes();

			m_dragonInfo[dragonType].dragonType = dragonType;
			m_dragonInfo[dragonType].dragonHard[hardInfo.hardLevel] = hardInfo;
			if (m_dragonInfo[dragonType].openHardLevel < hardInfo.hardLevel)
			{
				m_dragonInfo[dragonType].openHardLevel = hardInfo.hardLevel;
			}
		}
	}

	Update();

	return true;
}

void CDragonRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::DragonRecord2DB* record = poRoleAllInfo->mutable_dragoninfo();
		record->Clear();
		record->set_updateday(m_updateDay);
		record->set_updateharddragonday(m_updateHardDragonDay);
		record->set_updatesmalldragonday(m_updateSmallDragonDay);
		foreach (i in m_dragonInfo)
		{
			foreach(j in i->second.dragonHard)
			{
				KKSG::DragonRecord* info = record->add_record();
				info->set_dragontype(i->first);
				info->set_hardlevel(j->first);
				info->set_curfloor(j->second.reachTopFloor);
				info->set_dragondiamondboxtimes(j->second.dragonDiamondBoxTimes);
			}
		}

		roChanged.insert(record);
	}
}

void CDragonRecord::Change()
{
	m_switch.Set();
}

int CDragonRecord::GetLastActivityTime(std::vector<UINT32>& weekDays)
{
	time_t day = GameTime::GetTime();
	day = XCommon::GetDayBeginTime(day);
	tm time ;
#ifdef WIN32
	localtime_s(&time, &day);
#else
	localtime_r(&day, &time);
#endif

	size_t i = 0;
	bool find = false;
	for (i = 0; i < 7; ++i)
	{
		foreach(j in weekDays)
		{
			if ((time.tm_wday - i)%7 == *j)
			{
				find = true;
				break;
			}
		}
		if (find)
		{
			break;
		}
	}

	if (i < 7)
	{
		return day - i * 3600 * 24;

	}

	return 0;
}

int CDragonRecord::GetNextActivityTime(std::vector<UINT32>& weekDays)
{
	time_t day = GameTime::GetTime();
	day = XCommon::GetDayBeginTime(day);
	tm time ;
#ifdef WIN32
	localtime_s(&time, &day);
#else
	localtime_r(&day, &time);
#endif

	size_t i = 1;
	bool find = false;
	for (i = 1; i < 8; ++i)
	{
		foreach(j in weekDays)
		{
			if ((time.tm_wday + i)%7 == *j)
			{
				find = true;
				break;
			}
		}
		if (find)
		{
			break;
		}
	}

	if (i < 8)
	{
		return day + i * 3600 * 24;

	}

	return 0;

}

int CDragonRecord::CalcDragonRefreshTimes(int beginTime, int hard)
{
	if (!beginTime)
	{
		return 0;
	}
	//if (!hard)
	//{
	//	return 0;
	//}
	time_t nowDay = XCommon::GetDayBeginTime(GameTime::GetTime());
	int nowWeekDay = GameTime::GetWeekBeginTime(nowDay);
	tm nowtime;
#ifdef WIN32
	localtime_s(&nowtime, &nowDay);
#else
	localtime_r(&nowDay, &nowtime);
#endif

	time_t beginDay = XCommon::GetDayBeginTime(beginTime);
	int beginWeekDay = GameTime::GetWeekBeginTime(beginDay);
	tm begintime;
#ifdef WIN32
	localtime_s(&begintime, &beginDay);
#else
	localtime_r(&beginDay, &begintime);
#endif


	if (beginDay >= nowDay)
	{
		return 0;
	}

	std::vector<UINT32> refreshDays;
	if (hard == 1)
	{
		refreshDays = GetGlobalConfig().DragonResetWeekDay;
	}
	else if (hard == 2)
	{
		refreshDays = GetGlobalConfig().HardDragonResetWeekDay;
	}
	else if (hard == 0)
	{
		refreshDays = GetGlobalConfig().SmallDragonResetWeekDay;
	}
	int refreshTimes = 0;
	for (auto iter = refreshDays.begin(); iter != refreshDays.end(); ++iter)
	{
		if ((*iter) == 0)
		{
			*iter = 7;
		}
	}
	int begintimeweekday = begintime.tm_wday == 0 ? 7 : begintime.tm_wday;
	int nowtimeweekday = nowtime.tm_wday == 0 ? 7 : nowtime.tm_wday;

	if (!XCommon::IsWeekSame(beginDay, nowDay))
	{
		int wholeWeek = (nowWeekDay - beginWeekDay - 86400 * 7) / (86400 * 7);
		int beginweekrefresh = 0;
		int nowweekrefresh = 0;

		refreshTimes = wholeWeek * refreshDays.size();

		for (auto iter = refreshDays.begin(); iter != refreshDays.end(); ++iter)
		{
			if (*iter > begintimeweekday)
			{
				++beginweekrefresh;
			}
			if (*iter <= nowtimeweekday)
			{
				++nowweekrefresh;
			}
		}

		refreshTimes += beginweekrefresh + nowweekrefresh;
	}
	else
	{
		for (auto iter = refreshDays.begin(); iter != refreshDays.end(); ++iter)
		{
			if (*iter > begintimeweekday && *iter <= nowtimeweekday)
			{
				++refreshTimes;
			}
		}
	}

	return refreshTimes;
}

void CDragonRecord::Update()
{
	int lastTime = GetLastActivityTime( GetGlobalConfig().DragonResetWeekDay);
	if (m_updateDay < lastTime)
	{
		Reset(lastTime, 1);
	}

	int lastSmallTime = GetLastActivityTime( GetGlobalConfig().SmallDragonResetWeekDay);
	if (m_updateSmallDragonDay < lastSmallTime)
	{
		Reset(lastSmallTime, 0);
	}
	
	int lastHardTime = GetLastActivityTime( GetGlobalConfig().HardDragonResetWeekDay);
	if (m_updateHardDragonDay < lastHardTime)
	{
		Reset(lastHardTime, 2);
	}
}

void CDragonRecord::Reset(int lastTime, int hard)
{
	if (hard == 1)
	{
		m_updateDay = lastTime;
	}
	else if (hard == 2)
	{
		m_updateHardDragonDay = lastTime;
	}
	else if (hard == 0)
	{
		m_updateSmallDragonDay = lastTime;
	}
	foreach (i in m_dragonInfo)
	{
		foreach(j in i->second.dragonHard)
		{
			if (j->first == hard)
			{
				j->second.reachTopFloor = 0;
				j->second.bestTime = 0;
			}
		}
	}
	//m_switch.Set();
	Change();
}


void CDragonRecord::DebugReset(int type, int hard, bool clear)
{
	//m_dragonInfo.clear();
	if (clear)
	{
		m_dragonInfo.erase(type);
		m_updateDay = 0;
		m_updateHardDragonDay = 0;
		m_updateSmallDragonDay = 0;
		Update();
	}
	else
	{
		auto data = DragonConfig::Instance()->GetDragonTableData(type, hard, 1);
		if (data)
		{
			AddFloor(data->DragonNestID);
		}
	}

	Change();
}

void CDragonRecord::FillTopInfo2Client(KKSG::GetDragonTopInfoRes& roRes)
{
	foreach(i in m_dragonInfo)
	{
		bool hasSmallDragon = false;
		bool hasNormalDragon = false;
		foreach(j in i->second.dragonHard)
		{
			if (j->first == 0)
			{
				hasSmallDragon = true;
			}
			else if (j->first == 1)
			{
				hasNormalDragon = true;
			}
			KKSG::DragonInfo2Client* info = roRes.add_dragoninfo();
			info->set_dragontype(i->first);
			info->set_hardlevel(j->first);
			info->set_curfloor(j->second.reachTopFloor + 1);
			if (info->curfloor() > DragonConfig::Instance()->GetTopestFloor(i->first, j->first))
			{
				info->set_curfloor(-1);
			}

			int tempFloor = info->curfloor();
			if (info->curfloor() == -1)
			{
				tempFloor = DragonConfig::Instance()->GetTopestFloor(i->first, j->first);
			}

			//FILLDRAGONREFRESHTIMES(i->first, j->first, tempFloor)
			_FillDragonInfo(i->first, j->first, tempFloor, info);
		}
		if(!hasNormalDragon)
		{
			KKSG::DragonInfo2Client* info = roRes.add_dragoninfo();
			info->set_dragontype(i->first);
			info->set_hardlevel(1);
			info->set_curfloor(1);

			_FillDragonInfo(i->first, 1, 1, info);
		}
		if (!hasSmallDragon)
		{
			KKSG::DragonInfo2Client* info = roRes.add_dragoninfo();
			info->set_dragontype(i->first);
			info->set_hardlevel(0);
			info->set_curfloor(1);

			_FillDragonInfo(i->first, 0, 1, info);
		}
	}
	
	const DragonNestType& nestType = DragonConfig::Instance()->GetDragonNestType();
	for (auto iter = nestType.Table.begin(); iter != nestType.Table.end(); ++iter)
	{
		if (m_dragonInfo.find((*iter)->DragonNestType) != m_dragonInfo.end())
		{
			continue;
		}
		KKSG::DragonInfo2Client* info = roRes.add_dragoninfo();
		info->set_dragontype((*iter)->DragonNestType);
		info->set_hardlevel(1);
		info->set_curfloor(1);

		_FillDragonInfo((*iter)->DragonNestType, 1, 1, info);

		info = roRes.add_dragoninfo();
		info->set_dragontype((*iter)->DragonNestType);
		info->set_hardlevel(0);
		info->set_curfloor(1);

		_FillDragonInfo((*iter)->DragonNestType, 0, 1, info);
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

bool CDragonRecord::CheckRoleCondition(int expID)
{
	const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(expID);
	if(!data)
	{
		return false;
	}

	auto find = m_dragonInfo.find(data->DragonNestType);
	if (find == m_dragonInfo.end())
	{
		if ((data->DragonNestDifficulty == 1 || data->DragonNestDifficulty == 0) && data->DragonNestWave == 1)
		{
			return true;
		}

		return false;
	}

	auto findHard = find->second.dragonHard.find(data->DragonNestDifficulty);
	if (findHard == find->second.dragonHard.end())
	{
		if (data->DragonNestDifficulty == 1 || data->DragonNestDifficulty == 0)
		{
			if (data->DragonNestWave == 1)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			if (data->DragonNestDifficulty > find->second.openHardLevel)
			{
				return false;
			}
			if (data->DragonNestWave == 1)
			{
				return true;
			}

			return false;
		}
	}

	if(data->DragonNestWave == (findHard->second.reachTopFloor + 1))
	{
		return true;
	}

	return false;
}

bool CDragonRecord::OnDayPass()
{
	time_t day = GameTime::GetTime();
	tm* time = localtime(&day);
	foreach(i in  GetGlobalConfig().DragonResetWeekDay)
	{
		if (time->tm_wday == *i)
		{
			Reset(XCommon::GetDayBeginTime(day), 1);
			break;
		}
	}

	foreach(i in  GetGlobalConfig().SmallDragonResetWeekDay)
	{
		if (time->tm_wday == *i)
		{
			Reset(XCommon::GetDayBeginTime(day), 0);
			break;
		}
	}

	foreach(i in  GetGlobalConfig().HardDragonResetWeekDay)
	{
		if (time->tm_wday == *i)
		{
			Reset(XCommon::GetDayBeginTime(day), 2);
			break;
		}
	}

	return true;
}

int CDragonRecord::AddFloor(int expID)
{
	const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(expID);
	if(!data)
	{
		return 0;
	}
	int type = data->DragonNestType;
	int hard = data->DragonNestDifficulty;
	int& floor = m_dragonInfo[type].dragonHard[hard].reachTopFloor;
	++floor;
	if (hard != 0)
	{
		m_dragonInfo[type].openHardLevel = hard;
		int max = DragonConfig::Instance()->GetTopestFloor(type, hard);
		int maxHard = DragonConfig::Instance()->GetHardest(type);
		if (floor >= max && m_dragonInfo[type].openHardLevel < maxHard)
		{
			++m_dragonInfo[type].openHardLevel;
			m_dragonInfo[type].dragonHard[m_dragonInfo[type].openHardLevel].reachTopFloor = 0;
		}
	}

	Change();

	const DragonNestTable::RowData* info = DragonConfig::Instance()->GetDragonTableData(type, hard, floor + 1);
	if (info)
	{
		return info->DragonNestID;
	}

	return 0;
}

int CDragonRecord::GetNextExpID(int expID)
{
	const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(expID);
	if(!data)
	{
		return 0;
	}
	auto typeInfo = m_dragonInfo.find(data->DragonNestType);
	if (typeInfo == m_dragonInfo.end())
	{
		return 0;
	}
	if (typeInfo->second.dragonHard.find(data->DragonNestDifficulty) == typeInfo->second.dragonHard.end())
	{
		return 0;
	}

	int floor = typeInfo->second.dragonHard[data->DragonNestDifficulty].reachTopFloor;

	const DragonNestTable::RowData* info = DragonConfig::Instance()->GetDragonTableData(data->DragonNestType, data->DragonNestDifficulty, floor + 1);
	if (info)
	{
		return info->DragonNestID;
	}

	return 0;
}

bool CDragonRecord::CheckDefiniteDrop(UINT32& qualityDrop, bool resetTimes)
{
	if (!m_pRole->GetCurrScene())
	{
		return false;
	}

	SceneTeam* pTeam = m_pRole->GetCurrScene()->GetSceneTeam();
	if (!pTeam)
	{
		return false;
	}

	UINT32 expID = pTeam->GetExpID();
    const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(expID);
    if(!data)
    {
        return false;
    }

	if (data->DragonNestDifficulty != 2)
	{
		return false;
	}

	UINT32 times = GetGlobalConfig().HardDragonDropDiamondBox;
	if (!times)
	{
		return false;
	}

	auto find = m_dragonInfo.find(data->DragonNestType);
	if (find == m_dragonInfo.end())
	{
		return false;
	}

	if (find->second.openHardLevel < data->DragonNestDifficulty)
	{
		return false;
	}

	if(resetTimes)
	{
		if (qualityDrop == 4)
		{
			find->second.dragonHard[data->DragonNestDifficulty].dragonDiamondBoxTimes = 0;
		}
		else
		{
			++find->second.dragonHard[data->DragonNestDifficulty].dragonDiamondBoxTimes;
		}
		Change();
	}
	else
	{
		if (find->second.dragonHard[data->DragonNestDifficulty].dragonDiamondBoxTimes < times)
		{
			return false;
		}

		qualityDrop = 4;
	}

	return true;
}


void CDragonRecord::OnLogin()
{
	for (auto iter = m_dragonInfo.begin(); iter != m_dragonInfo.end(); ++iter)
	{
		int maxHard = DragonConfig::Instance()->GetHardest(iter->first);
		if (iter->second.openHardLevel >= maxHard)
		{
			continue;
		}
		int top = DragonConfig::Instance()->GetTopestFloor(iter->first, iter->second.openHardLevel);
		auto data = DragonConfig::Instance()->GetDragonTableData(iter->first, iter->second.openHardLevel, top);
		if (!data)
		{
			continue;
		}

		UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(data->DragonNestID);
		if (m_pRole->Get<StageMgr>()->GetRank(sceneID) != INVALID_STAGERANK)
		{
			++iter->second.openHardLevel;
			iter->second.dragonHard[iter->second.openHardLevel].reachTopFloor = 0;

			Change();
		}
	}
}

void CDragonRecord::_FillDragonInfo(int type, int hard, int floor, KKSG::DragonInfo2Client* info)
{
	auto data = DragonConfig::Instance()->GetDragonTableData(type, hard, floor);
	if (data)
	{
		info->set_weaktype(KKSG::DragonWeakType_Null);

		KKSG::DragonWeakType temptype = KKSG::DragonWeakType_Null;
		UINT32 refreshTimes = 0;
		UINT32 buffID = 0;
		UINT32 buffLevel = 0;
		DragonConfig::Instance()->CalcDragonWeakBuffInfo(data->DragonNestID, buffID, buffLevel, temptype, refreshTimes);
		
		info->set_weaktype(temptype);
		info->set_refreshtimes(refreshTimes);
	}

}

