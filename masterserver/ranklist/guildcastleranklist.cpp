#include "pch.h"
#include "guildcastleranklist.h"
#include "ranklist.h"
#include "role/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "loghelper/tlogr.h"

GuildCastleRankInfo::GuildCastleRankInfo()
{
	m_roleID	= 0;
	m_killCount = 0;
	m_zlCount	= 0;
	m_feats		= 0;
	SetData(0, 0, 0);
}

GuildCastleRankInfo::~GuildCastleRankInfo()
{

}


void GuildCastleRankInfo::DoTxLog()
{
	std::string szTransTag = TLogMgr::Instance()->GetTagString();
	TGuildCastleRoleFlow oLog(szTransTag);
	oLog.m_RoleID = m_roleID;
	oLog.m_KillCount = m_killCount;
	oLog.m_ZlCount =m_zlCount;
	oLog.m_Feats  = m_feats;
	oLog.Do();
}


void GuildCastleRankInfo::SetData(UINT64 roleID, UINT32 killCount, UINT32 zlCount)
{
	m_roleID = roleID;
	if (killCount > m_killCount)
	{
		m_feats += (killCount - m_killCount)*GetGlobalConfig().GuildCastleFeatsRules[0];
		LogDebug("killcount GuildCastleRankInfo::SetData m_killCount=%u killCount=%u roleid=%llu feats=%u", m_killCount, killCount,roleID,m_feats);
		m_killCount = killCount;
		DoTxLog();
	}

	if (zlCount > m_zlCount)
	{
		m_feats += (zlCount - m_zlCount)*GetGlobalConfig().GuildCastleFeatsRules[1];
		LogDebug("zlcount GuildCastleRankInfo::SetData m_killCount=%u killCount=%u roleid=%llu feats=%u", m_killCount, killCount,roleID,m_feats);
		m_zlCount = zlCount;
		DoTxLog();
	}
}

void GuildCastleRankInfo::FillData(KKSG::GCFRoleBrief& data, UINT32 rank)
{
	data.set_rank(rank);
	data.set_roleid(m_roleID);
	data.set_rolename(CRoleSummaryMgr::Instance()->GetRoleName(m_roleID));
	data.set_killcount(m_killCount);
	data.set_occupycount(m_zlCount);
	data.set_feats(m_feats);

}

void GuildCastleRankInfo::TestPrint()
{
	SSDebug << " roleid = " << m_roleID << " killcount = " << m_killCount << " zlcount = " << m_zlCount << END;
}

GuildCastleRankList::GuildCastleRankList()
{

}

GuildCastleRankList::~GuildCastleRankList()
{
	Clear();
}

void GuildCastleRankList::Clear()
{
	std::vector<GuildCastleRankInfo*> infos;
	GetAllInfo(infos);	

	for(size_t i = 0; i < infos.size(); ++i)
	{
		delete infos[i];
	}
	infos.clear();

	m_rankTree.Clear();
}

void GuildCastleRankList::OnChange(UINT64 roleID, UINT32 killCount, UINT32 zlCount)
{
	if(0 == roleID)
	{
		return;
	}

	GuildCastleRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roleID, info);
	if(INVALID_RANK == ret)
	{
		info = new GuildCastleRankInfo();	
	}
	else if(info)
	{
		m_rankTree.Erase(roleID);
	}

	if(info)
	{
		info->SetData(roleID, killCount, zlCount);
		m_rankTree.Insert(info, info->RoleID());
	}
}

void GuildCastleRankList::GetAllInfo(std::vector<GuildCastleRankInfo*>& vec)
{
	m_rankTree.GetAllKey(vec);
}

UINT32 GuildCastleRankList::GetRank(UINT64 roleID)
{
	return m_rankTree.GetRankByID(roleID);
}

UINT32 GuildCastleRankList::GetFeats(UINT64 roleID)
{
	GuildCastleRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roleID, info);
	if(INVALID_RANK != ret && info)
	{
		return info->GetFeats();
	}
	return 0;
}

void GuildCastleRankList::FillMyRank(KKSG::GCFRoleBrief& myInfo, UINT64 roleID)
{
	GuildCastleRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roleID, info);
	if(INVALID_RANK != ret && info)
	{
		UINT32 myRank = m_rankTree.GetRankByID(roleID);
		info->FillData(myInfo, myRank);
	}
	else
	{
		myInfo.set_rank(0);
		myInfo.set_roleid(roleID);
		myInfo.set_rolename(CRoleSummaryMgr::Instance()->GetRoleName(roleID));
		myInfo.set_killcount(0);
		myInfo.set_occupycount(0);
		myInfo.set_feats(0);
	}
}

void GuildCastleRankList::FillRankData(KKSG::GCFCommonRes& data, UINT64 roleID)
{
	UINT32 SendCountMax = 100;	
	
	std::vector<GuildCastleRankInfo*> infos;

	GetAllInfo(infos);

	for(size_t i = 0; i < infos.size() && i < SendCountMax; ++i)
	{
		if(infos[i])
		{
			infos[i]->FillData(*data.add_roles(), i + 1);			
		}
	}

	FillMyRank(*data.mutable_myinfo(), roleID);
}

void GuildCastleRankList::TestPrint()
{
	SSDebug << __FUNCTION__ << END;
	std::vector<GuildCastleRankInfo*> infos;
	GetAllInfo(infos);
	for(size_t i = 0; i < infos.size(); ++i)
	{
		if(infos[i])
		{
			infos[i]->TestPrint();
		}
	}
}

UINT32 GuildCastleRankList::AddKill(UINT64 roleID)
{
	UINT32 killCount = 0;
	UINT32 occupyCount = 0;
	GetOldCount(roleID, killCount, occupyCount);
	OnChange(roleID, killCount + 1, occupyCount);
	return (killCount + 1);
}

void GuildCastleRankList::AddOccupy(UINT64 roleID)
{
	UINT32 killCount = 0;
	UINT32 occupyCount = 0;
	GetOldCount(roleID, killCount, occupyCount);
	OnChange(roleID, killCount, occupyCount + 1);
}

void GuildCastleRankList::GetOldCount(UINT64 roleID, UINT32& killCount, UINT32& occupyCount)
{
	killCount = 0;
	occupyCount = 0;
	GuildCastleRankInfo* info = NULL;
	UINT32 ret = m_rankTree.GetKeyByID(roleID, info);
	if(INVALID_RANK != ret && info)
	{
		killCount = info->KillCount();
		occupyCount = info->ZLCount();
	}
}

