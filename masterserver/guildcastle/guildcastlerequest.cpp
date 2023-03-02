#include "pch.h"
#include "guildcastlerequest.h"
#include "table/globalconfig.h"
#include "guild/guildmgr.h"
#include "guildcastlemgr.h"
#include "role/role.h"
#include "guildcastlebase.h"
#include "scene/sceneswitch.h"
#include "ranklist/guildcastleranklist.h"
#include "notice/notice.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "guild/guildterritorybattle.h"
#include "table/TerritoryBattle.h"
#include "team/teamforceop.h"
#include "guildcastle/ptcm2c_guildcastfeatsntf.h"

GcfRequest::GcfRequest(CRole* pRole)
{
	m_pRole = pRole;
}

GcfRequest::~GcfRequest()
{

}

int GcfRequest::JoinReadyScene(UINT32 territoryID)
{
	UINT32 mapID = GetGCWaitMapID();

	auto pOneFight = GuildCastleMgr::Instance()->GetTerritoryFight(territoryID);
	if(NULL == pOneFight)
	{
		return KKSG::ERR_FAILED;
	}
	if(pOneFight->GetStage() == CGFS_END)
	{
		return KKSG::ERR_GCASTLE_FIGHT_END;
	}
	if(pOneFight->GetStage() != CGFS_READY && pOneFight->GetStage() != CGFS_FIGHT)
	{
		return KKSG::ERR_GCASTLE_NOT_IN_ACT;
	}

	int ret = ChangeBattleField(pOneFight, mapID);
	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	GuildCastleMgr::Instance()->AddGuildJoinRole(m_pRole->GetID());

	//fix bug 1001770
	PtcM2C_GuildCastFeatsNtf ntf;
	ntf.m_Data.set_roleid(m_pRole->GetID());
	ntf.m_Data.set_feats(0);
	if (pOneFight!=NULL)
	{
		GuildCastleRankList* pRankList =  pOneFight->GetRankList();
		if (pRankList!=NULL)
		{
			ntf.m_Data.set_feats(pRankList->GetFeats(m_pRole->GetID()));
		}
	}
	m_pRole->Send(ntf);


	//离队
	if(m_pRole->GetTeamID())
	{
		TeamForceOp teamOp;
		std::vector<CRole*> tmpVec;
		tmpVec.push_back(m_pRole);
		teamOp.LeaveTeam(tmpVec);
	}

	return KKSG::ERR_SUCCESS;
}

int GcfRequest::JoinFightScene(UINT32 mapID)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(m_pRole->GetSceneID());
	if(NULL == pOneFight)
	{
		return KKSG::ERR_FAILED;
	}
	if(pOneFight->GetStage() != CGFS_FIGHT)
	{
		SSDebug << " not fight stage " << END;
		return KKSG::ERR_GCASTLE_NOT_IN_FIGHT;
	}

	if(GetGCWaitMapID() == mapID)
	{
		return KKSG::ERR_FAILED;
	}

	if(GetGCWaitMapID() != m_pRole->GetMapID())
	{
		return KKSG::ERR_FAILED;
	}

	int ret = ChangeBattleField(pOneFight, mapID);

	if(KKSG::ERR_SUCCESS != ret)
	{
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

int GcfRequest::ChangeBattleField(GCOneFight* pOneFight, UINT32 mapID)
{
	if(NULL == pOneFight)
	{
		return KKSG::ERR_FAILED;
	}
	auto newField = pOneFight->GetBattleFieldByMapID(mapID);

	UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID());
	GCFGroup myGroup = pOneFight->GetGuildGroup(gid);
	if(GCF_GROUP_NONE == myGroup)
	{
		return KKSG::ERR_FAILED;
	}

	if(newField && newField->SceneID() && !newField->HaveRole(m_pRole->GetID()))
	{
		if(newField->SceneID() == m_pRole->GetSceneID())
		{
			return KKSG::ERR_FAILED;
		}

		if(!newField->IsWaitField() && newField->CurRoleCount(myGroup) >= GetGlobalConfig().GuildCastleBattleRoleMax)
		{
			SSDebug << " role full map = " << mapID << END;
			return KKSG::ERR_GCASTLE_ROLE_FULL;
		}

		const KKSG::SceneSwitchData roData;
		if(!CSceneSwitch::EnterScene(m_pRole, newField->SceneID(), roData))
		{
			return KKSG::ERR_FAILED;
		}

		newField->IncreaseRole(m_pRole->GetID(), myGroup);
		
	}
	else
	{
		return KKSG::ERR_FAILED;//not ready
	}

	return KKSG::ERR_SUCCESS;
}

//可以重复调用
void GcfRequest::OnLeave(UINT64 uid, UINT32 sceneID)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetOneFight(uid);
	if(NULL == pOneFight)
	{
		return ;
	}

	if(CGFS_END == pOneFight->GetStage())
	{
		return ;
	}

	auto pField = pOneFight->GetBattleFieldBySceneID(sceneID);
	if(NULL == pField)
	{
		return;
	}

	pField->ReduceRole(m_pRole->GetID());
}

int GcfRequest::BattleReport(KKSG::GCFCommonRes& roRes, UINT32 territoryID)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetTerritoryFight(territoryID);
	if(NULL == pOneFight)
	{
		return KKSG::ERR_FAILED;
	}
	pOneFight->FillBattleReport(roRes, m_pRole->GetID());
	
	return KKSG::ERR_SUCCESS;
}

int GcfRequest::BattleReport(KKSG::GCFCommonRes& roRes)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(m_pRole->GetSceneID());
	if(NULL == pOneFight)
	{
		return KKSG::ERR_FAILED;
	}
	pOneFight->FillBattleReport(roRes, m_pRole->GetID());
	
	return KKSG::ERR_SUCCESS;
}

void GcfRequest::ReadyInfos(KKSG::GCFReadyInfoRes& roRes)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(m_pRole->GetSceneID()); 
	if(pOneFight && (pOneFight->GetStage() == CGFS_READY || pOneFight->GetStage() == CGFS_FIGHT))
	{
		pOneFight->FillReadyInfo(roRes, m_pRole->GetID());
	}
}

void GcfRequest::FightInfos(KKSG::GCFFightInfoRes& roRes)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(m_pRole->GetSceneID()); 
	if(pOneFight && (pOneFight->GetStage() == CGFS_READY || pOneFight->GetStage() == CGFS_FIGHT))
	{
		pOneFight->FillFightInfo(roRes, m_pRole->GetID(), m_pRole->GetMapID());
	}
}

void GcfRequest::Kill(UINT64 uid, UINT32 continueKill)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetOneFight(uid); 
	if(NULL == pOneFight)
	{
		return ;
	}
	if(pOneFight->GetStage() != CGFS_FIGHT)
	{
		return ;
	}
	auto pRankList = pOneFight->GetRankList();
	if(NULL == pRankList)
	{
		return;
	}
	UINT32 killCount = pRankList->AddKill(m_pRole->GetID());
	pOneFight->KillCountSyn(m_pRole->GetID(), killCount);


	PtcM2C_GuildCastFeatsNtf ntf;
	ntf.m_Data.set_roleid(m_pRole->GetID());
	ntf.m_Data.set_feats(pRankList->GetFeats(m_pRole->GetID()));
	m_pRole->Send(ntf);


	KillNotice(m_pRole, continueKill, pOneFight);
}

void GcfRequest::Occupy(KKSG::GCFJvDianType type, UINT64 uid)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetOneFight(uid); 
	if(NULL == pOneFight)
	{
		return ;
	}
	if(pOneFight->GetStage() != CGFS_FIGHT)
	{
		return ;
	}
	auto pRankList = pOneFight->GetRankList();
	if(NULL == pRankList)
	{
		return;
	}
	pRankList->AddOccupy(m_pRole->GetID());

	PtcM2C_GuildCastFeatsNtf ntf;
	ntf.m_Data.set_roleid(m_pRole->GetID());
	ntf.m_Data.set_feats(pRankList->GetFeats(m_pRole->GetID()));
	m_pRole->Send(ntf);


	auto pField = pOneFight->GetBattleFieldBySceneID(m_pRole->GetSceneID());
	if(pField)
	{
		UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID());		
		if(INVALID_GUILDID != gid)
		{
			pField->ResetJvDian(type, gid);
		}
	}

	OccupyNotice(m_pRole, type, pOneFight);
}

void GcfRequest::BackToReady(UINT64 uid)
{
	auto pOneFight = GuildCastleMgr::Instance()->GetOneFight(uid);
	if(NULL == pOneFight)
	{
		return ;
	}
	if(pOneFight->GetStage() != CGFS_FIGHT)
	{
		return ;
	}
	JoinReadyScene(pOneFight->TerritoryID());
}

int GcfRequest::FightResult(KKSG::GCFCommonRes& roRes)
{
	SSDebug << __FUNCTION__ << " role = " << m_pRole->GetID() << END;
	auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(m_pRole->GetSceneID());
	if(NULL == pOneFight)
	{
		return KKSG::ERR_FAILED;
	}
	if(CGFS_END != pOneFight->GetStage())
	{
		return KKSG::ERR_FAILED;
	}
	pOneFight->FillResult(roRes, m_pRole->GetID());
	return KKSG::ERR_SUCCESS;
}

void GcfRequest::OnLogout()
{
	auto pOneFight = GuildCastleMgr::Instance()->GetSceneFight(m_pRole->GetSceneID());
	if(NULL == pOneFight)
	{
		return ;
	}

	if(CGFS_END == pOneFight->GetStage())
	{
		return ;
	}

	auto pField = pOneFight->GetBattleFieldBySceneID(m_pRole->GetSceneID());
	if(NULL == pField)
	{
		return;
	}

	pField->ReduceRole(m_pRole->GetID());

}

void GcfRequest::KillNotice(CRole* pRole, UINT32 continueKill, GCOneFight* pOneFight)
{
	if(NULL == pRole || NULL == pOneFight)
	{
		return;
	}
	UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	if(INVALID_GUILDID == guildID)
	{
		return;
	}
	UINT32 noticeID = 0;
	switch(continueKill)
	{
	case 4:
		{
			noticeID = GuildTerrContinueKill_4;
		}
		break;
	case 6:
		{
			noticeID = GuildTerrContinueKill_6;
		}
		break;
	case 8:
		{
			noticeID = GuildTerrContinueKill_8;
		}
		break;
	default:
		break;
	}

	if(0 == noticeID)
	{
		return;
	}
	Notice notice(noticeID);
	notice.Replace("$R", pRole->GetName());
	notice.SendGuild(guildID);
}

void GcfRequest::OccupyNotice(CRole* pRole, KKSG::GCFJvDianType type, GCOneFight* pOneFight)
{
	if(NULL == pRole || NULL == pOneFight)
	{
		return;
	}
	UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	if(INVALID_GUILDID == guildID)
	{
		return;
	}
	CScene* pScene = CSceneManager::Instance()->GetScene(pRole->GetSceneID());
	if(NULL == pScene)
	{
		return;
	}
	if(NULL == pScene->GetConf())
	{
		return;
	}
	TerritoryBattle::RowData* pConf = CGuildTerritoryBattle::Instance()->GetTerrConf(pOneFight->TerritoryID());
	if(NULL == pConf)
	{
		return ;
	}

	UINT32 noticeID = 0;
	switch(type)
	{
	case KKSG::GCF_JUDIAN_UP:
		{
			noticeID = GuildTerrOccupy_Up;
		}
		break;
	case KKSG::GCF_JUDIAN_MID:
		{
			noticeID = GuildTerrOccupy_Mid;
		}
		break;
	case KKSG::GCF_JUDIAN_DOWN:
		{
			noticeID = GuildTerrOccupy_Down;
		}
		break;
	default:
		break;
	}

	if(0 == noticeID)
	{
		return;
	}

	Notice notice(noticeID);
	notice.Replace("$R", pRole->GetName());
	notice.Replace("$S", pConf->territoryname);
	notice.Replace("$S", pScene->GetConf()->Comment);
	notice.SendGuild(guildID);	
}
