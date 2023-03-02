#include "pch.h"
#include "scene/ptcc2g_enterscenereq.h"
#include "unit/rolemanager.h"
#include "scene/scenemanager.h"
#include "unit/role.h"
#include "gamelogic/stagemgr.h"
#include "scene/scene.h"
#include "pb/project.pb.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildboss.h"
#include "scene/sceneswitch.h"
#include "idip/idiprecord.h"
#include "table/globalconfig.h"
#include "table/ProfessionMgr.h"
#include "common/ptcg2c_gserrornotify.h"
#include "util/gametime.h"
#include "gamelogic/teammgr.h"
#include "gamelogic/team.h"
#include "gamelogic/horsemgr.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"
#include "scene/scenemanager.h"

// generate by ProtoGen at date: 2014/10/16 11:10:28

void PtcC2G_EnterSceneReq::Process(UINT32 dwConnID)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSWarn<<"role is NULL, sessionid:"<<m_sessionID<<END;
		return;
	}

	SceneInfo *pInfo = SceneConfig::Instance()->FindSceneInfo(m_Data.sceneid());
	if (pInfo == NULL || pInfo->m_pConf == NULL)
	{
		SSWarn<<"scene info is NULL, sceneid:"<<m_Data.sceneid()<<END;
		return;
	}
	if (!SceneConfig::Instance()->IsSceneValid(pInfo->m_pConf->type))
	{
		SSWarn<<"scene id is invalid, sceneid:"<<m_Data.sceneid()<<END;
		return;
	}
	
	///> 限制场景类型
	auto i = GetGlobalConfig().CanEnterScene.find(pInfo->m_pConf->type);
	if (i == GetGlobalConfig().CanEnterScene.end())
	{
		SSWarn<<"scene type:"<<pInfo->m_pConf->type<<" can not enter scene"<<END;
		return;
	}

	UINT32 type = pInfo->m_pConf->type;
	if (KKSG::SCENE_BOSSRUSH == type || KKSG::SCENE_DRAGON_EXP == type || 
		KKSG::SCENE_TOWER == type || KKSG::SCENE_BATTLE == type)//主线
	{
		if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_DAILY_PLAY))
		{
			return ;
		}
	}
	else if (KKSG::SCENE_WORLDBOSS == type)
	{
		///> system open
		if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SysWorldBossId))
		{
			SSInfo<<"world boss close"<<END;
			return;
		}

		if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
		{
			return;
		}
	}else if (KKSG::SCENE_HORSE == type)
	{	
		if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_HORSE))
		{
			SSInfo<<"horse race close"<<END;
			return;
		}
		if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
		{
			return;
		}
		if(HorseRideMgr::Instance()->IsOpen() == false)
		{
			PtcG2C_GSErrorNotify ntf;
			ntf.m_sessionID = m_sessionID;
			ntf.m_Data.set_errorno(KKSG::ERR_HORSE_ACTIVITY);
			pRole->Send(ntf);
			HorseRideMgr::Instance()->HallIconNoticeCheck(pRole,SYS_HORSE,KKSG::HICONS_END);
			return;
		}
		if (!GSConfig::Instance()->IsHallGS())
		{
			LogInfo("invalid enter horse wait scene roleid=%llu",pRole->GetID());
			return;
		}
		StageMgr* pStage = pRole->Get<StageMgr>();
		if (pStage->CanPlayCount(GetGlobalConfig().HorseSceneID) == false)
		{
			LogInfo("role=%llu count not enouch", pRole->GetID());
			return;
		}
	}

	Scene* loadscene = pRole->GetLoadingScene();
	if (NULL != loadscene && m_Data.sceneid() == loadscene->GetSceneTemplateID())
	{
		SSWarn << pRole << " try to enter scene:"<<m_Data.sceneid()<<" when loading " << pRole->GetLoadingScene() << END;
		return;
	}

	// 二转转职场景特殊判断
	if (GetGlobalConfig().SecondPromoteSecneID == m_Data.sceneid() && ProfessionMgr::PromoteLevel(pRole->GetProfession()) < 2)
	{
		PtcG2C_GSErrorNotify ntf;
		ntf.m_sessionID = m_sessionID;
		ntf.m_Data.set_errorno(KKSG::ERR_NEED_FIRST_PROMOTE);
		pRole->Send(ntf);
		return;
	}

	UINT32 sceneID = m_Data.sceneid();

	UINT64 ownerID = m_Data.roleid();	
	if(0 == ownerID)
	{
		ownerID = pRole->GetID();		
	}
	SceneManager::Instance()->ChangeScene(pRole, sceneID, ownerID);

	SSInfo<<"role:"<<pRole->GetID()<<" enter scene:"<<sceneID<<" ownerid:"<<ownerID<<END;
}
