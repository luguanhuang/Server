#include "pch.h"
#include "scene/scenereswarpve.h"
#include "unit/enemy.h"
#include "foreach.h"
#include "scene/scene.h"
#include "unit/role.h"
#include <vector>
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "unit/role.h"
#include "unit/enemymanager.h"
#include "utility/tlogger.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "gamelogic/XLevelWave.h"
#include "unit/rolemanager.h"
#include "stage/ptcg2c_bossrushonefinishntf.h"
#include "scene/scenefinder.h"
#include "entity/XRole.h"
#include "unit/attributewatcher.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "component/XSkillComponent.h"
#include "table/GuildMineralBattle.h"
#include "reswar/reswarpvemgr.h"
#include "util/gametime.h"


SceneResWarPVE::SceneResWarPVE(Scene* pScene)
{
	m_nTime          = 0;
	m_pScene		= pScene;
	m_addBuffTime	= 0;
	m_buffID		= 0;
	m_buffLevel		= 0;
	m_bossID        = 0;
	m_nRandBuffID   = 0;
}


SceneResWarPVE* SceneResWarPVE::CreateSceneResWarPVE(Scene* pScene)
{
	SceneResWarPVE *pRet = NULL;
	if (pScene->GetSceneType() == KKSG::SCENE_RESWAR_PVE)
	{
		pRet = new SceneResWarPVE(pScene);
		pRet->m_nTime = GameTime::GetTime();
	}
	return pRet;
}


void SceneResWarPVE::Spawn()
{
	GuildMineralBattle::RowData* pConf = ResWarAwardMgr::Instance()->GetPVEAward(m_nMineID);
	if(!pConf)  
		return;
	XLevelSpawner*	pSpawner = m_pScene->GetSpawner();
	if (!pSpawner) 
		return;
	LogDebug("SceneResWarPVE::Spawn bossid =%d",pConf->BossID);
	pSpawner->ReplaceRandmizedEnemyID(1, pConf->BossID);
}


void SceneResWarPVE::OnBossAppear(Enemy* pBoss)
{
	if (!pBoss) 
		return;
	GuildMineralBattle::RowData* pConf = ResWarAwardMgr::Instance()->GetPVEAward(m_nMineID);
	if(!pConf) 
		return;
	pBoss->ResetBaseAttr(pConf->lifepercent, pConf->atkpercent, pConf->defpercent);
	m_buffID		= pConf->ragebuff[0];
	m_buffLevel		= pConf->ragebuff[1];
	m_addBuffTime	= GameTime::GetTime() + pConf->ragetime;
	m_bossID		= pBoss->GetID();

}


void SceneResWarPVE::Update()
{
	time_t nTime    = GameTime::GetTime();
	if (nTime-m_nTime>=120)
		m_pScene->SetDontDestory(false);
	if(m_addBuffTime && m_addBuffTime <= time(NULL))
	{
		Enemy* pBoss = EnemyManager::Instance()->FindEnemy(m_bossID);
		if(pBoss && m_buffID && m_buffLevel)
		{
			XAIEventArgs args;
			args.Templateid = pBoss->GetTemplateID();	
			args.SkillId = m_buffID;
			args.DelayTime = 1.0f;
			args.EventArg = "addbuff";
			m_pScene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
		}
		m_buffID = 0;
		m_buffLevel = 0;
		m_addBuffTime = 0;
	}

}


void SceneResWarPVE::OnEnterScene(Role* role)
{
	m_pScene->SetDontDestory(false);
	GuildMineralBufflist::RowData*pRowData = ResWarAwardMgr::Instance()->GetGuildMineralBufflist(m_nRandBuffID);
	if (pRowData&&pRowData->type != 1)
	{
		AddBuff(role, pRowData->BuffID,pRowData->Quality);
	}
}

void SceneResWarPVE::AddBuff(Role* pRole, UINT32 nBuffID, UINT32 nBuffLevel)
{
	if(pRole && pRole->GetXObject())
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = nBuffID;
		args.singleBuff.BuffLevel = nBuffLevel;
		SSDebug << __FUNCTION__ << " nBuffID = " << nBuffID << " nBuffLevel = " << nBuffLevel << END;
		args.Caster = pRole->GetID();
		pRole->GetXObject()->DispatchAction(&args);
	}
}
