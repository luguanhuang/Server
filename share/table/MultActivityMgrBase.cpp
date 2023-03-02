#include "pch.h"
#include "MultActivityMgrBase.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "pb/project.pb.h"
#include "foreach.h"

MultActivityMgrBase::MultActivityMgrBase()
:m_timer(0)
{
}

MultActivityMgrBase::~MultActivityMgrBase()
{
}

bool MultActivityMgrBase::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	m_nOpenTime = 0;
	return true;
}

void MultActivityMgrBase::Uninit()
{
	ClearFile();
	CTimerMgr::Instance()->KillTimer(m_timer);
}

void MultActivityMgrBase::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckActivityStateChange();
}

bool MultActivityMgrBase::CheckFile()
{
	MultiActivityList tmp;
	if (!tmp.LoadFile("table/MultiActivityList.txt"))
	{
		SSWarn << "load file table/MultiActivityList.txt failed" << END;
		return false;
	}
	return true;
}

void MultActivityMgrBase::ClearFile()
{
	m_tableList.Clear();
	m_activitys.clear();
}

bool MultActivityMgrBase::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();	

	if (!m_tableList.LoadFile("table/MultiActivityList.txt"))
	{
		SSWarn << "load file table/MultiActivityList.txt failed" << END;
		return false;
	}

	for (INT32 i = 0; i < m_tableList.Table.size(); ++i)
	{
		AddConfig(m_tableList.Table[i]);
	}

	return true;
}

void MultActivityMgrBase::AddConfig(MultiActivityList::RowData* data)
{
	MultActivityConf& config = m_activitys[data->ID];

	config.id = data->ID;

	config.openDays.clear();
	config.openTime.clear();
	config.openServerWeek = data->OpenServerWeek;
	config.icon = data->PushIcon;

	// 开放天
	std::vector<std::string> tmp;
	tmp = Split(data->OpenWeek, '|');
	for(size_t i = 0; i < tmp.size(); ++i)
	{
		config.openDays.push_back(convert<int>(tmp[i]));
	}

	if(tmp.size()==1&&convert<int>(tmp[0])==-1)
		return;

	// 开放时间
	for(size_t i = 0; i < data->OpenDayTime.size(); ++i)
	{
		const Sequence<UINT32,2>& seq = data->OpenDayTime[i];
		Sequence<UINT32,2> sec;	
		MultActivityTime opentime;
		opentime.startSec = (seq[0]/100) * 3600 + (seq[0]%100) * 60;
		opentime.endSec = (seq[1]/100) * 3600 + (seq[1]%100) * 60;
		config.openTime.push_back(opentime);
	}

	config.guildLevel = data->GuildLevel;	
	config.maxCount = data->DayCountMax;

	// 初始状态
	config.curState = GetOpenTimeState(config.id);
}

MultActivityConf* MultActivityMgrBase::GetActivityConfig(UINT32 id)
{
	auto it = m_activitys.find(id);
	if (it == m_activitys.end())
	{
		return NULL;
	}
	return &it->second;
}

MultActivityTime MultActivityMgrBase::GetNearTime(std::vector<MultActivityTime>& times, UINT32 todaySec)
{
	INT32 max = times.size();
	for (INT32 i = 0; i < max; i++)
	{
		MultActivityTime& cur = times[i];
		if (todaySec < cur.startSec)
		{
			return cur;
		}
		if (todaySec >= cur.startSec && todaySec <= cur.endSec)
		{
			return cur;
		}
		// 最后一个
		if (i == max -1)
		{
			return cur;
		}
	}
	return MultActivityTime();
}

UINT32 MultActivityMgrBase::GetOpenTime(UINT32 id)
{
	MultActivityConf* config = GetActivityConfig(id);
	if (!config)
	{
		return 0;
	}
	if (config->openTime.empty())
	{
		return 0;
	}
	UINT32 now = GameTime::GetTime();
	UINT32 todayZero = XCommon::GetDayBeginTime(now, false);
	UINT32 openDayZero = GetOpenDayZeroSecond(config);
	MultActivityTime nearTime = GetNearTime(config->openTime, now - todayZero);

	return openDayZero + nearTime.startSec;
}



UINT32 MultActivityMgrBase::GetEndTime(UINT32 id)
{
	MultActivityConf* config = GetActivityConfig(id);
	if (!config)
	{
		return 0;
	}
	if (config->openTime.empty())
	{
		return 0;
	}

	UINT32 now = GameTime::GetTime();
	UINT32 todayZero = XCommon::GetDayBeginTime(now, false);
	UINT32 openDayZero = GetOpenDayZeroSecond(config);
	MultActivityTime nearTime = GetNearTime(config->openTime, now - todayZero);

	return openDayZero + nearTime.endSec;
}

KKSG::MulActivityTimeState MultActivityMgrBase::GetOpenTimeState(UINT32 id)
{
	UINT32 leftTime;
	MultActivityTime openIime;	
	return GetOpenTimeState(id, leftTime, openIime);
}

KKSG::MulActivityTimeState MultActivityMgrBase::GetOpenTimeState(UINT32 id, UINT32& leftTime, MultActivityTime& openTime)
{
	MultActivityConf* config = GetActivityConfig(id);
	if (!config)
	{
		return KKSG::MULACTIVITY_UNOPEN_TODAY;
	}
	return GetOpenTimeState(config, leftTime, openTime);
}

KKSG::MulActivityTimeState MultActivityMgrBase::GetOpenTimeState(MultActivityConf* config, UINT32& leftTime, MultActivityTime& openTime)
{
	UINT32 now = GameTime::GetTime();
	if (config->openServerWeek)
	{
		UINT32 nDiff  =  (GameTime::GetWeekBeginTime(now) -  GameTime::GetWeekBeginTime(m_nOpenTime))/(7*24*3600) + 1;
		if (nDiff < config->openServerWeek)
		{
			return KKSG::MULACTIVITY_UNOPEN_TODAY;
		}
	}

	UINT32 weekNum = XCommon::GetWeekNum(now, false);
	bool isWeek = false;
	for (size_t i = 0; i < config->openDays.size(); ++i)
	{
		INT32 num = config->openDays[i];
		if (-1 == num)
		{
			break;
		}
		if (0 == num || weekNum == num)
		{
			isWeek = true;
			break;
		}
	}



	if (false == isWeek)
	{
		return KKSG::MULACTIVITY_UNOPEN_TODAY;
	}

	UINT32 dayBegin = XCommon::GetDayBeginTime(now,false);
	UINT32 todaySec = UINT32(now - dayBegin);

	for(size_t i = 0; i < config->openTime.size(); ++i)
	{
		MultActivityTime& tmpTime = config->openTime[i];
		if (tmpTime.startSec == 0 && tmpTime.endSec == 0)
		{
			leftTime = 24 * 3600 - todaySec;
			openTime = tmpTime;
			return KKSG::MULACTIVITY_RUNNING;
		}
		if (todaySec < tmpTime.startSec)
		{
			leftTime = tmpTime.startSec - todaySec;
			openTime = tmpTime;
			return  KKSG::MULACTIVITY_BEfOREOPEN;
		}
		if (todaySec >= tmpTime.startSec && todaySec <= tmpTime.endSec)
		{
			leftTime = tmpTime.endSec - todaySec;
			openTime = tmpTime;
			return KKSG::MULACTIVITY_RUNNING;
		}
	}

	return KKSG::MULACTIVITY_END;
}

UINT32 MultActivityMgrBase::GetOpenDayZeroSecond(MultActivityConf* config)
{
	UINT32 now = GameTime::GetTime();
	UINT32 weekNum = XCommon::GetWeekNum(now, false);

	UINT32 lowerBoundNum = 8;
	UINT32 firstNum = 8;
	bool isWeek = false;
	for (size_t i = 0; i < config->openDays.size(); ++i)
	{
		INT32 num = config->openDays[i];
		// 不开放
		if (-1 == num)
		{
			return 0;
		}
		if (0 == num || weekNum == num)
		{
			return XCommon::GetDayBeginTime(now, false);
		}
		if (num >= weekNum && num <= lowerBoundNum)
		{
			lowerBoundNum = num;
		}
		if (num <= firstNum)
		{
			firstNum = num;
		}
	}
	// 本周的开放日
	if (lowerBoundNum != 8)
	{
		return XCommon::GetDayBeginTime(now, false) + (lowerBoundNum - weekNum) * 24 * 3600;
	}
	// 下周的首个开发日
	if (firstNum != 8)
	{
		return XCommon::GetDayBeginTime(now, false) + (7 - weekNum + firstNum) * 24 * 3600;
	}
	return 0;
}

void MultActivityMgrBase::ResetConfig(UINT32 id)
{
	auto config = m_tableList.GetByID(id);
	if (!config)
	{
		return;
	}
	AddConfig(config);
}

void MultActivityMgrBase::GMAddOpenDayTime(UINT32 id, UINT32 startTime, UINT32 endTime)
{
	MultActivityConf* config = GetActivityConfig(id);
	if(NULL == config)
	{
		SSError << " can not find multi activity id = " << id << END;
		return;
	}
	MultiActivityList::RowData *pRowData = m_tableList.GetByID(config->id);
	if(NULL ==pRowData)
	{
		SSError << " can not find multi activity id = " << id << END;
		return;
	}
	pRowData->OpenServerWeek = 0;
	pRowData->OpenWeek.clear();
	ResetConfig(id);

	config->openServerWeek = 0;
	config->openDays.clear();
	config->openDays.push_back(XCommon::GetWeekNum(TimeUtil::GetTime(), false));
	config->openTime.clear();
	config->curState = KKSG::MULACTIVITY_BEfOREOPEN;

	MultActivityTime openTime;
	openTime.startSec = startTime;
	openTime.endSec = endTime;
	config->openTime.push_back(openTime);
	UINT32 beginTime = XCommon::GetTodayBeginTime(false);
	SSInfo << "Gm add multi act, id " << id << ", start Time: " << TimeUtil::GetDateTimeStr(beginTime + startTime)
		   << ", end Time: " << TimeUtil::GetDateTimeStr(beginTime + endTime) << END;
}

bool MultActivityMgrBase::IsActivityTimeOpen(UINT32 id)
{
	return GetOpenTimeState(id) == KKSG::MULACTIVITY_RUNNING;
}

bool MultActivityMgrBase::IsGuildLevelOpen(UINT32 id, UINT32 guildLvl)
{
	MultActivityConf* conf = GetActivityConfig(id);
	if (!conf)
	{
		return false;
	}
	return conf->guildLevel <= guildLvl;
}

bool MultActivityMgrBase::IsActivityOnDay(UINT32 id, UINT32 weekNum)
{
	MultActivityConf* conf = GetActivityConfig(id);
	if (!conf)
	{
		return false;
	}

	if (conf->openDays.empty())
	{
		return true;
	}
	
	if (conf->openDays.size() == 1)
	{
		if (conf->openDays[0] == 0)
		{
			return true;
		}

		if (conf->openDays[0] == -1)
		{
			return false;
		}
	}

	foreach (i in conf->openDays)
	{
		if (weekNum == *i)
		{
			return true;
		}
	}

	return false;
}

bool MultActivityMgrBase::IsGuildBossOpen(UINT32 guildLvl)
{
	return IsGuildLevelOpen(MULT_ACTIVITY_GUILD_BOSS, guildLvl);
}

UINT32 MultActivityMgrBase::GetGuildBossOpenLvl()
{
	MultActivityConf* conf = GetActivityConfig(MULT_ACTIVITY_GUILD_BOSS);
	if (!conf)
	{
		return false;
	}
	return conf->guildLevel;
}

bool MultActivityMgrBase::IsGuildBossOnTime()
{
	return GetOpenTimeState(MULT_ACTIVITY_GUILD_BOSS) == KKSG::MULACTIVITY_RUNNING;
}

time_t MultActivityMgrBase::GetGuilBossOpenTime()
{
	return GetOpenTime(MULT_ACTIVITY_GUILD_BOSS);
}

time_t MultActivityMgrBase::GetGuilBossEndTime()
{
	return GetEndTime(MULT_ACTIVITY_GUILD_BOSS);
}

void MultActivityMgrBase::SetGuildBossTime(UINT32 startTime, UINT32 endTime)
{
	GMAddOpenDayTime(MULT_ACTIVITY_GUILD_BOSS, startTime, endTime);
}

void MultActivityMgrBase::CheckActivityStateChange()
{
	std::vector<UINT32> changIDs;
	UINT32 opencount = 0;
	if (m_nOpenTime == 0)
		return;
	for (auto it = m_activitys.begin(); it != m_activitys.end(); ++it)
	{
		KKSG::MulActivityTimeState lastState = it->second.curState;
		KKSG::MulActivityTimeState curState = GetOpenTimeState(it->first);
		if (lastState != curState)
		{
			OnActivityStateChange(it->first, lastState, curState);
			changIDs.push_back(it->first);
		}
		it->second.curState = curState;
		if(KKSG::MULACTIVITY_RUNNING == curState)
		{
			++opencount;
		}
	}

	if(changIDs.empty())
	{
		return ;
	}
	OnActivitysChange(opencount, changIDs);
}

void MultActivityMgrBase::OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState)
{

}

void MultActivityMgrBase::OnActivitysChange(UINT32 opencount, std::vector<UINT32>& changeIds)
{

}

