#include "pch.h"
#include "briefranklist.h"
#include "pb/project.pb.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "util/timespecificmgr.h"
#include "util//XCommon.h"
#include "table/globalconfig.h"
#include "config.h"
#include "util/gametime.h"
#include "common/ptcm2g_worldlevelntf.h"
#include "global/GlobalConfigDBBuild.h"
#include "other/servercrossdatamgr.h"
#include "network/gslink.h"
//#include "gamelogic/tshowrecord.h"

#define BRIEFRANKCOUNT 10000

CBriefRankInfo::CBriefRankInfo(UINT64 qwRoleId)
:CRankInfo(qwRoleId)
{
	Reset();
}

CBriefRankInfo::~CBriefRankInfo()
{

}

void CBriefRankInfo::Reset()
{
	m_level = 0;
	m_powerpoint = 0;
	m_intervalTimeStamp = 0;
}

void CBriefRankInfo::RankInfoToKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData || 0 == GetUniqueId())
	{
		return;
	}
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(GetUniqueId());
	if (NULL == pRoleSummary)
	{
		pRankData->set_roleid(0);
		return;
	}
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_powerpoint(m_powerpoint);
	pRankData->set_rolelevel(m_level);
	pRankData->set_rolename(pRoleSummary->GetName());
	pRankData->set_titleid(pRoleSummary->GetTitleID());
	pRankData->set_is_vip(pRoleSummary->IsQQVip());
	pRankData->set_is_svip(pRoleSummary->IsQQSVip());
	pRankData->set_starttype(pRoleSummary->GetStartUpType());
	Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(GetUniqueId());
	if (NULL != guild)
	{
		pRankData->set_guildicon(guild->GetIcon());
		pRankData->set_guildname(guild->GetName());
	}
}

void CBriefRankInfo::RankInfoToDBKKSG(KKSG::RankData* pRankData) const
{
	if (NULL == pRankData)
	{
		return;
	}
	pRankData->set_roleid(GetUniqueId());
	pRankData->set_powerpoint(m_powerpoint);
	pRankData->set_rolelevel(m_level);
	pRankData->set_time(m_time);
	pRankData->set_intervaltimestamp(m_intervalTimeStamp);
}

void CBriefRankInfo::DBKKSGToRankInfo(const KKSG::RankData* pRankData)
{
	if (NULL == pRankData)
	{
		return;
	}
	SetUniqueId(pRankData->roleid());
	m_powerpoint = pRankData->powerpoint();
	m_level = pRankData->rolelevel();
	m_time = pRankData->time();
	m_intervalTimeStamp = pRankData->intervaltimestamp();
}

//////////////////////////////////////////////////////////////////////////
/*void CBriefRankDb::PackDBData(KKSG::RankList* ranklist)
{
	std::vector<CBriefRankInfo*> lastrankinfos;
	(dynamic_cast<CBriefRankList*>(m_ranklist))->GetLastRankInfo(lastrankinfos);

	for (auto i = lastrankinfos.begin(); i != lastrankinfos.end(); ++i)
	{
		CRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}

	std::vector<CBriefRankInfo*> currankinfos;
	(static_cast<CBriefRankList*>(m_ranklist))->GetRankInfo(currankinfos);
	
	for (auto i = currankinfos.begin(); i != currankinfos.end(); i++)
	{
		CRankInfo* rankinfo = *i;
		if (NULL == rankinfo)
		{
			continue;
		}
		KKSG::RankData* rankdata = ranklist->add_rankdata();
		rankinfo->RankInfoToDBKKSG(rankdata);
	}
}

void CBriefRankDb::UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank)
{
	CBriefRankList* ranklist = static_cast<CBriefRankList*>(m_ranklist);

	CBriefRankInfo* rankinfo = new CBriefRankInfo(rankdata.roleid());

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(rankdata.roleid());
	if (!summary)
	{
		delete rankinfo;
		return;
	}
	rankinfo->DBKKSGToRankInfo(&rankdata);
	if (rankinfo->m_intervalTimeStamp)
	{
		if(ranklist->GetLastRank(rankdata.roleid()) != INVALID_RANK)
		{
			ranklist->InsertLastKey(rankinfo);
		}
	}
	else
	{
		ranklist->InsertKey(rankinfo);
	}
}

void CBriefRankDb::OnReceiveFromDB()
{
	CBriefRankList* ranklist = static_cast<CBriefRankList*>(m_ranklist);

	UINT32 intervalTimeStamp = ranklist->GetFirstIntervalTimeStamp();
	if (intervalTimeStamp != 0)
	{
		if (!XCommon::IsWeekSame(intervalTimeStamp, time(0)))
		{
			ranklist->Update();
		}
		m_fillLastInfo = false;
	}
}

bool CBriefRankDb::BeforeSaveToDB()
{
	return true;
}*/

/////////////////////////////////////////////////////////////////////////////////////////////
CBriefRankList::CBriefRankList(int type, BriefRankCompare* cmp)
:CRankList(type)
,m_rankTree(BriefRankCmpWrap(cmp))
{
	if (KKSG::LevelRank == type)
	{
		TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 2);
		CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CBriefRankList::OnTimer, this));

		m_GMUpdateNextWorldLevel = true;
	}
}

CBriefRankList::~CBriefRankList()
{
	std::vector<CBriefRankInfo*> briefinfo;
	m_rankTree.GetAllKey(briefinfo);
	for(auto i = briefinfo.begin(); i != briefinfo.end(); i++)
	{
		delete *i;
	}
	m_rankTree.Clear();

	/*for(auto i = m_lastrankinfos.begin(); i != m_lastrankinfos.end(); i++)
	{
		delete *i;
	}
	m_lastrankinfos.clear();
	m_hasID_lastrank.clear();

	if (GetType() == KKSG::FashionPowerPointRank)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timehandle);
	}*/
}

UINT32 CBriefRankList::GetNeedRankCount() 
{ 
	return BRIEFRANKCOUNT; 
}

UINT32 CBriefRankList::GetRank(UINT64 roleid)
{
	return m_rankTree.GetRankByID(roleid);	
}

UINT32 CBriefRankList::GetSize()
{
	return m_rankTree.GetSize();
}

const CBriefRankInfo* CBriefRankList::GetInfoByRank(UINT32 rank)
{
	CBriefRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByRank(rank, rankinfo);
	if(ret != INVALID_RANK)
	{
		return rankinfo;
	}
	else 
	{
		return NULL;
	}
}

void CBriefRankList::RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession)
{
	std::vector<CBriefRankInfo*> vec;
	m_rankTree.GetSomeTopKey(count, vec);

	UINT32 vecSize = vec.size();
	for(UINT32 i = 0; i < vecSize; i++)
	{
		CBriefRankInfo* rankinfo = vec[i];
		KKSG::RankData* rankdata = rankdatas->add_rankdata();
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

void CBriefRankList::RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank)
{
	const CBriefRankInfo* rankinfo = GetInfoByRank(rank);
	if(rankinfo != NULL)
	{
		rankinfo->RankInfoToKKSG(rankdata);
	}
}

bool CBriefRankList::OnTimer()
{
	if (m_type != KKSG::LevelRank) return true;

	if (!m_GMUpdateNextWorldLevel)
	{
		m_GMUpdateNextWorldLevel = true;
		return true;
	}

	std::vector<CBriefRankInfo*> vec;
	m_rankTree.GetSomeTopKey(GetGlobalConfig().WorldLevelCount, vec);
	if (vec.empty())
	{
		return true;
	}
	UINT32 levelSum = 0;
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		levelSum += vec[i]->m_level;
	}
	UINT32 worldLevel = levelSum / vec.size();

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWorldLevel, ToString(worldLevel));
	CGlobalConfigDBBuild::Instance()->SetConfigValue(eWorldLevelDay, ToString(XCommon::GetTodayUnique()));
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWorldLevel);
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eWorldLevelDay);

	ServerCrossDataSyn::SynDataToWorld();

	PtcM2G_WorldLevelNtf oPtc;
	oPtc.m_Data.set_worldlevel(worldLevel);
	GSLink::Instance()->SendToAllLine(oPtc);
	LogInfo("Now worldLevel is %u", worldLevel);
	return true;
}

void CBriefRankList::OnGSConnected(UINT32 dwLine)
{
	if(m_type != KKSG::LevelRank) return;

	UINT32 worldLevel = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	int worldLevelDay = convert<int>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevelDay));

	int dayUnique = XCommon::GetTodayUnique();
	if(dayUnique == worldLevelDay)
	{
		PtcM2G_WorldLevelNtf oPtc;
		oPtc.m_Data.set_worldlevel(worldLevel);
		GSLink::Instance()->SendToLine(dwLine, oPtc);
	}
	else
	{
		OnTimer();
	}
}

void CBriefRankList::OnChange(UINT64 roleid, UINT32 powerpoint, UINT32 level, UINT32 time)
{
	CBriefRankInfo* rankinfo;
	UINT32 ret = m_rankTree.GetKeyByID(roleid, rankinfo);
	if(ret == INVALID_RANK)
	{
		if(GetType() == KKSG::FashionPowerPointRank && 0 == powerpoint)
		{
			return;
		}
		rankinfo = new CBriefRankInfo(roleid);
		rankinfo->m_powerpoint = powerpoint;
		rankinfo->m_level = level;
		rankinfo->m_time = time;
		InsertKey(rankinfo);

		if(m_rankTree.GetSize() > GetNeedRankCount())
		{
			UINT32 ret = m_rankTree.GetKeyByRank(m_rankTree.GetSize(), rankinfo);
			if(ret != INVALID_RANK)
			{
				m_rankTree.Erase(rankinfo->GetUniqueId());
				delete rankinfo;
			}
		}
	}
	else
	{	
		m_rankTree.Erase(roleid); // just erase the pointer
		if(GetType() == KKSG::FashionPowerPointRank && 0 == powerpoint)
		{
			delete rankinfo;
			return;
		}
		rankinfo->m_powerpoint = powerpoint;
		rankinfo->m_level = level;
		rankinfo->m_time = time;
		InsertKey(rankinfo);
	}

	/*if (GetType() == KKSG::FashionPowerPointRank && powerpoint > (UINT32) GetGlobalConfig().TShowCatWalkFirstWeekFashionPower && (int)m_lastrankinfos.size() < FASHIONPOWERLASTRANK)
	{
		if (XCommon::IsWeekSame(MSConfig::Instance()->GetGameServerOpenTimeStamp(), time) || ((CBriefRankDb*)m_db)->m_fillLastInfo)
		{
			if(m_hasID_lastrank.find(roleid) != m_hasID_lastrank.end())
			{
				return;
			}
			CBriefRankInfo* rankinfo = new CBriefRankInfo(roleid);
			rankinfo->m_powerpoint = powerpoint;
			rankinfo->m_level = level;
			rankinfo->m_intervalTimeStamp = time;
			InsertLastKey(rankinfo);
		}
	}*/
}

void CBriefRankList::InsertKey(CBriefRankInfo* rankinfo)
{
	if(NULL == rankinfo)
	{
		LogWarn("Insert a NULL pointer in BriefRanklist\n");
		return;
	}
	UINT64 roleid = rankinfo->GetUniqueId();
	m_rankTree.Insert(rankinfo, roleid);
}

void CBriefRankList::GetRankInfo(std::vector<CBriefRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

void CBriefRankList::GMSetUpdateNextWorldLevelFalse()
{
	if (m_type != KKSG::LevelRank) return;
	m_GMUpdateNextWorldLevel = false;
}