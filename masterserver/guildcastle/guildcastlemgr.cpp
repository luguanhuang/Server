#include "pch.h"
#include "guildcastlemgr.h"
#include "guild/guildmgr.h"
#include "guildcastlebase.h"
#include <ctime>
#include "table/globalconfig.h"
#include "role/rolemanager.h"
#include "role/rolemanager.h"
#include "guild/guildterritorybattle.h"
#include "loghelper/tlogr.h"
#include "ranklist/guildcastleranklist.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "levelseal/levelsealMgr.h"
#include "table/MultActivityMgr.h"

INSTANCE_SINGLETON(GuildCastleMgr)

GuildCastleMgr::GuildCastleMgr()
{

}

GuildCastleMgr::~GuildCastleMgr()
{

}

bool GuildCastleMgr::Init()
{
	if(!LoadFile())
	{
		return false;
	}

	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	return true;
}

void GuildCastleMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
	ClearFightData();
	ClearFile();
}

bool GuildCastleMgr::CheckFile()
{
	TerritoryBattleReward tmpTerritoryBattleReward;
	if(!tmpTerritoryBattleReward.LoadFile("table/TerritoryBattleReward.txt"))
	{
		return false;
	}
	TerritoryRewd tmpTerritoryRewd;
	if (!tmpTerritoryRewd.LoadFile("table/TerritoryRewd.txt"))
	{
		return false;
	}
	return true;
}


void GuildCastleMgr::ClearFile()
{
	m_joinRewardTable.Clear();
	m_territoryRewdTable.Clear();
}

bool GuildCastleMgr::LoadFile()
{
	if (CheckFile()==false)
	{
		return false;
	}

	ClearFile();

	if(!m_joinRewardTable.LoadFile("table/TerritoryBattleReward.txt"))
	{
		return false;
	}

	if (!m_territoryRewdTable.LoadFile("table/TerritoryRewd.txt"))
	{
		return false;
	}

	return true;
}

void GuildCastleMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 now = UINT32(GameTime::GetTime());

	switch(m_stateMgr.GetState())
	{
	case GCF_ACT_S_NONE:
		{
			if(!m_guildJoins.empty())
			{
				m_stateMgr.ChangeState(GCF_ACT_S_RUNNING, now);	
			}
		}
		break;
	case GCF_ACT_S_RUNNING:
		{
			if(CheckFightStage() && m_stateMgr.GetTime() + GetGlobalConfig().GuildCastleReadyTime + GetGlobalConfig().GuildCastleFightTime <= now)
			{
				m_stateMgr.ChangeState(GCF_ACT_S_ENE, now);
				GiveGulidReward();
				GiveGuildFeatsReward();
				//FightResultReport();	
				//GiveAuctReward();
			}
		}
		break;
	case GCF_ACT_S_ENE:
		{
			if(MultActivityMgr::Instance()->GetEndTime(MULT_ACTIVITY_16)+10 < now)
			{
				ClearFightData();
				m_stateMgr.ChangeState(GCF_ACT_S_NONE, now);
			}
		}
		break;
	default:
		break;
	}
}

bool GuildCastleMgr::CheckFightStage()
{
	bool AllEnd = true;
	UINT32 now = UINT32(time(NULL));
	for(auto it = m_allFights.begin(); it != m_allFights.end(); ++it)
	{
		UINT64 UID = (it->first);
		auto pOneFight = (it->second);
		if(NULL == pOneFight)
		{
			continue;
		}
		switch(pOneFight->GetStage())
		{
		case CGFS_READY:
			{
				AllEnd = false;
				if(0 == pOneFight->GetLeftTime())
				{
					pOneFight->ChangeStage(CGFS_FIGHT);	
				}
			}
			break;
		case CGFS_FIGHT:
			{
				AllEnd = false;
				if(0 == pOneFight->GetLeftTime() || pOneFight->GetMaxGroupPoint() >= GetGlobalConfig().GuildCastleEndPoint)
				{
					pOneFight->ChangeStage(CGFS_END);
				}
				else
				{
					pOneFight->Update();
				}
			}
			break;
		case CGFS_END:
			{
			}
			break;
		default:
			break;
		}
	}

	return AllEnd;
}

bool GuildCastleMgr::MakeOneFight(const std::vector<UINT64>& guilds, UINT32 territoryID)
{

	if(guilds.size() < 2 || guilds.size() > 3)
	{
		SSError << " guild size = " << guilds.size() << END;
		return false;
	}

	if(GetTerritoryFight(territoryID))
	{
		SSError << " repeat territoryID = " << territoryID << END;
		return false;
	}

	UINT64 UID = NewUID();

	GCOneFight* pOneFight = new GCOneFight(UID, territoryID);
	if(NULL == pOneFight)
	{
		return false;
	}

	if(!pOneFight->Init(guilds))
	{
		delete pOneFight;
		return false;
	}

	pOneFight->CreateSceneBegin();

	Add(pOneFight);

	return true;
}

UINT64 GuildCastleMgr::NewUID()
{
	static UINT64 OneFightUniqueID = 1;

	return (OneFightUniqueID ++);
}

void GuildCastleMgr::Add(GCOneFight* oneFight)
{
	if(NULL == oneFight)
	{
		return ;
	}
	m_allFights[oneFight->GetUID()] = oneFight;
}

void GuildCastleMgr::Del(UINT64 UID)
{
	auto it = m_allFights.find(UID);
	if(it == m_allFights.end())
	{
		return ;
	}
	delete (it->second);
	m_allFights.erase(it);
}

GCOneFight* GuildCastleMgr::GetOneFight(UINT64 uid)
{
	auto it = m_allFights.find(uid);
	if(it != m_allFights.end())
	{
		return (it->second);
	}
	return NULL;
}

GCOneFight* GuildCastleMgr::GetTerritoryFight(UINT32 territoryID)
{
	for(auto it = m_allFights.begin(); it != m_allFights.end(); ++it)
	{
		auto pOneFight = (it->second);
		if(pOneFight && pOneFight->TerritoryID() == territoryID)
		{
			return pOneFight;
		}
	}
	return NULL;
}

void GuildCastleMgr::TestMakeFight(UINT32 territoryID)
{
	std::unordered_set<UINT64> guildtmp;
	const auto& itBegin = CRoleManager::Instance()->Begin();
	const auto& itEnd = CRoleManager::Instance()->End();

	for(auto it = itBegin; it != itEnd; ++it)
	{
		UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId((*it)->GetID());	
		if(guildID)
		{
			guildtmp.insert(guildID);
		}
		if(guildtmp.size() >= 3)
		{
			break;
		}
	}

	if(guildtmp.size() < 2)
	{
		return ;
	}

	std::vector<UINT64> guilds;
	guilds.insert(guilds.begin(), guildtmp.begin(), guildtmp.end());

	if(guilds.empty())
	{
		return;
	}

	MakeOneFight(guilds, territoryID);
}

void GuildCastleMgr::TestPrint()
{
	for(auto it = m_allFights.begin(); it != m_allFights.end(); ++it)
	{
		auto pOneFight = (it->second);
		if(pOneFight)
		{
			pOneFight->TestPrint();
		}
	}
}

GCOneFight* GuildCastleMgr::GetSceneFight(UINT32 sceneID)
{
	for(auto it = m_allFights.begin(); it != m_allFights.end(); ++it)
	{
		GCOneFight* pOneFight = (it->second);
		if(pOneFight && pOneFight->GetBattleFieldBySceneID(sceneID))
		{
			return pOneFight;
		}
	}
	return NULL;
}

void GuildCastleMgr::ClearFightData()
{
	SSDebug << __FUNCTION__ << END;
	for(auto it = m_allFights.begin(); it != m_allFights.end(); ++it)
	{
		delete (it->second);
	}
	m_allFights.clear();
	m_guildJoins.clear();

}

void GuildCastleMgr::AddOneFight(UINT32 territoryID, UINT64 defentGuild, const std::set<UINT64>& attackGuild)
{
	SSInfo << " terrid = " << territoryID << " defentguild = " << defentGuild << " attacksize = " << attackGuild.size() << END;
	std::vector<UINT64> guilds;
	if(defentGuild)
	{
		guilds.push_back(defentGuild);
	}
	guilds.insert(guilds.end(), attackGuild.begin(), attackGuild.end());

	std::unordered_set<UINT64> tmpguilds;
	for(size_t i = 0; i < guilds.size(); ++i)
	{
		SSInfo << " guild = " << guilds[i] << END;
		auto pGuild = CGuildMgr::Instance()->GetGuild(guilds[i]);
		if(NULL == pGuild)
		{
			SSError << " find not guild = " << guilds[i] << END;
			return;
		}
		tmpguilds.insert(guilds[i]);
	}

	if(tmpguilds.size() != guilds.size())
	{
		SSError << " have repeat guild " << END;
		return;
	}

	bool haveFight = (1 != guilds.size());
	if(haveFight)
	{
		if(!MakeOneFight(guilds, territoryID))
		{
			SSError << " make fight failed territoryID = " << territoryID << END;
			return;
		}
	}

	for(size_t i = 0; i < guilds.size(); ++i)
	{
		UINT64 gid = guilds[i];
		if(gid == defentGuild)
		{
			m_guildJoins[gid].SetDefent(territoryID, haveFight);
		}
		else
		{
			m_guildJoins[gid].SetAttack(territoryID, haveFight);
		}
	}
}

void GuildCastleMgr::AddGuildJoinRole(UINT64 roleID)
{
	UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(roleID);
	auto it = m_guildJoins.find(guildID);
	if(it == m_guildJoins.end())
	{
		return ;
	}
	(it->second).AddRole(roleID);
}

void GuildCastleMgr::SetGuildResult(UINT64 guildID, UINT32 territoryID, bool win, UINT64 winGuildID)
{
	auto it = m_guildJoins.find(guildID);
	if(it == m_guildJoins.end())
	{
		return ;
	}
	(it->second).SetResult(territoryID, win);
	DoTxLog(guildID, territoryID, win, guildID==winGuildID);
}

void GuildCastleMgr::DoTxLog(UINT64 guildID, UINT32 territoryID, bool win, bool iscastle)
{
	TGuildCastleFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_allianceId = CGuildTerritoryBattle::Instance()->GetAllianceId(guildID);
	oLog.m_GuildId = guildID;
	oLog.m_TerritoryID = territoryID;
	if (iscastle)//是否为占领公会;
	{
		oLog.m_Result = Tx_CasteType2;
	}
	else
	{
		oLog.m_Result = win?Tx_CasteType1:Tx_CasteType0;
	}
	Guild* pGuild = NULL;
	auto it = CGuildTerritoryBattle::Instance()->GetCityMap();
	auto i = it.find(territoryID);
	if (i != it.end())
	{
		oLog.m_GuildId2 = i->second.m_guildId;
		pGuild = CGuildMgr::Instance()->GetGuild(i->second.m_guildId);
		oLog.m_strGuildName2 = pGuild?pGuild->GetName():"";
	}
	pGuild = CGuildMgr::Instance()->GetGuild(guildID);
	oLog.m_strGuildName = pGuild?pGuild->GetName():"";
	oLog.Do();
}

UINT32 GuildCastleMgr::GetLevelSeal()
{
	UINT32 nMaxLevelSeal = 0;
	UINT32 nCurLevelSeal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	for(size_t i = 0; i < m_territoryRewdTable.Table.size(); i++)
	{
		if (m_territoryRewdTable.Table[i]->ID == nCurLevelSeal)
		{
			return nCurLevelSeal;
		}
		if (m_territoryRewdTable.Table[i]->ID>nMaxLevelSeal)
		{
			nMaxLevelSeal = m_territoryRewdTable.Table[i]->ID;
		}
	}
	return nMaxLevelSeal;
}

void GuildCastleMgr::GiveGuildFeatsReward()
{
	std::map<UINT64,UINT32> mapRoleFeats;
	for(auto it = m_allFights.begin(); it != m_allFights.end(); ++it)
	{
		GCOneFight* pOneFight = (it->second);
		GuildCastleRankList*pRankList =  pOneFight->GetRankList();
		if (pRankList!=NULL)
		{
			std::vector<GuildCastleRankInfo*> vecCastRank;
			pRankList->GetAllInfo(vecCastRank);
			for (auto iter = vecCastRank.begin(); iter!=vecCastRank.end(); iter++)
			{
				GuildCastleRankInfo* pRankInfo = *iter;
				if(pRankInfo->GetFeats() > mapRoleFeats[pRankInfo->RoleID()])
				{
					mapRoleFeats[pRankInfo->RoleID()] = pRankInfo->GetFeats();
				}
			}
		}
	}

	int sealType = GetLevelSeal();
	for (auto iter = mapRoleFeats.begin(); iter!=mapRoleFeats.end(); iter++)
	{
		std::vector<ItemDesc> itemList;
		TerritoryRewd::RowData *pRowData = NULL;
		for(size_t i = 0; i < m_territoryRewdTable.Table.size(); i++)
		{
			if(sealType == m_territoryRewdTable.Table[i]->ID && iter->second >= m_territoryRewdTable.Table[i]->Point)
			{
				pRowData = m_territoryRewdTable.Table[i];
			}
		}

		for (size_t j = 0; pRowData!=NULL&&j < pRowData->Reward.size(); j++)
		{
			ItemDesc item(pRowData->Reward[j][0], pRowData->Reward[j][1]);
			itemList.push_back(item);
		}

		if (itemList.empty()==false)
		{
			std::string mailtitle = "null";
			std::string mailcontent = "null";
			const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_GuildCastleFeats);
			if(pMailConf)
			{
				mailtitle   = pMailConf->m_title;
				mailcontent = pMailConf->m_content;
				mailcontent = MailConfig::Instance()->ReplaceString("%s",ToString(iter->second),mailcontent);
			}

			MailData maildata(mailtitle, mailcontent, itemList, MAIL_EXPIRE_TIME);
			maildata.SetReason(ItemFlow_GuildTerrFeats);
			CMailMgr::Instance()->SendMail(iter->first, maildata);
		}


	}

}

void GuildCastleMgr::GiveGulidReward()
{
	for(auto it = m_guildJoins.begin(); it != m_guildJoins.end(); ++it)
	{
		(it->second).GiveReward(it->first);
	}
}

void GuildCastleMgr::FightResultReport(UINT32 terrID, UINT64 winGuildID)
{
	SSInfo << __FUNCTION__ << " terrid = " << terrID << " winguild = " << winGuildID << END;
	if(0 == terrID || 0 == winGuildID)
	{
		SSError << __FUNCTION__ << " terrid = " << terrID << " winguild = " << winGuildID << END;
		return;
	}
	CGuildTerritoryBattle::Instance()->SetBattleResult(terrID, winGuildID);
}

void GuildCastleMgr::GetGuildJoinRole(UINT64 guildID, std::set<UINT64>& roles)
{
	roles.clear();

	auto it = m_guildJoins.find(guildID);
	if(it == m_guildJoins.end())
	{
		SSError << " find join guild failed " << guildID << " time = " << (time(NULL)) << END;
		return; 
	}

	const auto& allroles = (it->second.AllRoles());

	roles.insert(allroles.begin(), allroles.end());
}


TerritoryBattleReward::RowData* GuildCastleMgr::GetJoinRewardConf(UINT32 terrLevel, int type)
{
	for(size_t i = 0; i < m_joinRewardTable.Table.size(); ++i)
	{
		auto pConf = m_joinRewardTable.Table[i];
		if(terrLevel == pConf->Territorylevel && type == pConf->Rewardtype)
		{
			return pConf;
		}
	}
	return NULL;
}
