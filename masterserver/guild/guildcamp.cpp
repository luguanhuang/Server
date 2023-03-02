#include "pch.h"
#include "guild/guildbonus.h"
#include "pb/project.pb.h"
#include "guildcamp.h"
#include "util/timespecificmgr.h"
#include "foreach.h"
#include "guildlistener.h"
#include "scene/sceneconfig.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "table/guildConfig.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guild/guildbonus.h"
#include "guild/guildcamprank.h"
#include "pal/ttime.h"

CGuildCamp::CGuildCamp(Guild* pGuild):m_rankList(this)
{
	m_pGuild = pGuild;

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CGuildCamp::OnTimer, this));

	Reset();
}

CGuildCamp::~CGuildCamp()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
}

bool CGuildCamp::OnTimer()
{
	Update();
	return true;
}

void CGuildCamp::Reset()
{
	m_updatetime = 0;

	m_curCampID = 0;
	m_nextCampID = 0;
	m_rankList.Clear();

}

void CGuildCamp::Update()
{
	m_rankList.OnDayPass();
	Reset();
	SetChanged();

	time_t now = time(0);
	m_updatetime = now;

	struct tm temp;
	localtime_r(&now, &temp);
	auto& CampDayIDs = GetGlobalConfig().GuildCampDayID;
	int weekday = temp.tm_wday;
	if (!weekday)
	{
		weekday = 7;
	}
	auto find = CampDayIDs.find(weekday);
	if (find != CampDayIDs.end())
	{
		m_curCampID = find->second;
		++find;
		if (find != CampDayIDs.end())
		{
			m_nextCampID = find->second;
		}
		else
		{
			m_nextCampID = CampDayIDs.begin()->second;
		}
	}
	m_rankList.SetRankType(m_curCampID);
}

void CGuildCamp::DebugReset(UINT32 day)
{
	Update();
	auto& CampDayIDs = GetGlobalConfig().GuildCampDayID;
	auto find = CampDayIDs.find(day);
	if (find != CampDayIDs.end())
	{
		m_curCampID = find->second;
		++find;
		if (find != CampDayIDs.end())
		{
			m_nextCampID = find->second;
		}
		else
		{
			m_nextCampID = CampDayIDs.begin()->second;
		}
	}
	m_rankList.SetRankType(m_curCampID);
}

bool CGuildCamp::IsChanged()
{
	return m_isModified.TestAndReset();
}

bool CGuildCamp::Load(const KKSG::GuildAllDBData & guilddata)
{
	KKSG::GuildCampSave guildcamp;
	if (!guilddata.has_guildcamp())
	{
		return false;
	}

	if (guilddata.guildcamp().empty())
	{
		return false;
	}

	if (!guildcamp.ParseFromString(guilddata.guildcamp()))
	{
		return false;
	}
	
	m_updatetime = guildcamp.updatetime();
	m_curCampID = guildcamp.curcampid();
	m_nextCampID = guildcamp.nextcampid();
	m_rankList.SetRankType(m_curCampID);
	m_rankList.LoadFromDb(guildcamp);

	if (!XCommon::IsDaySame(m_updatetime, time(0)))
	{
		Update();
	}
	//Sort();
	
	return true;
}

void CGuildCamp::Save(KKSG::GuildAllDBData & guilddata)
{
	guilddata.mutable_guildcamp()->assign(ToBinary());
}


const std::string CGuildCamp::ToBinary()
{
	KKSG::GuildCampSave save;
	if (!m_updatetime)
	{
		m_updatetime = time(0);
	}
	save.set_updatetime(m_updatetime);
	save.set_curcampid(m_curCampID);
	save.set_nextcampid(m_nextCampID);
	m_rankList.SaveDb(save);

	return save.SerializeAsString();

}


GuildCampRankList* CGuildCamp::GetGuildCampRankList()
{
	return &m_rankList;
}

void CGuildCamp::FillGuildCampInfo(KKSG::GuildCampInfoRes& roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	roRes.set_curcampid(m_curCampID);
	roRes.set_nextcampid(m_nextCampID);
	roRes.set_leftcount(0);
	m_rankList.FillRankInfo2Client(roRes);
}

void CGuildCamp::UpdateRankInfo(const KKSG::GuildCampResult& result)
{
	if(m_curCampID != result.curcampid())
		return;
	std::vector<UINT64> roleids;
	for (int i = 0; i < result.roles_size(); ++i)
	{
		roleids.push_back(result.roles(i).roleid());
	}

	m_rankList.OnChange(roleids, result.var(), time(0));
}

void CGuildCamp::SetChanged()
{
	m_isModified.Set();
}


