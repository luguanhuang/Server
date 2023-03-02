#include "pch.h" 
#include "scenebossrush.h"
#include "scene.h"
#include "gamelogic/bossrushmgr.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "unit/enemymanager.h"
#include "unit/enemy.h"
#include "gamelogic/stagemgr.h"
#include "scenefinder.h"
#include "component/XSkillComponent.h"
#include "component/XBuffComponent.h"
#include "entity/XRole.h"

CSceneBossRush::CSceneBossRush(Scene* scene)
	:m_scene(scene)
{
	ClearLastBoss();
	ClearBuffInfo();
	SetStopFlag(true);
}

CSceneBossRush::~CSceneBossRush()
{
	//SSDebug << __FUNCTION__ << END;
}

void CSceneBossRush::OnBossAppear(Enemy* pBoss)
{
	//SSDebug << __FUNCTION__ << END;
	SetLastBoss(pBoss);
	BossRushMgr::Instance()->FillBossData(pBoss, m_scene);

	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(pRole)
	{
		auto pState = pRole->Get<StageMgr>()->GetBRRoleState();
		if(pState)
		{
			auto pConf = BossRushMgr::Instance()->GetConf(pState->confid);
			if(pConf)
			{
				m_buffID = pConf->ragebuff[0];
				m_buffLevel = pConf->ragebuff[1];
				m_addBuffTime = time(NULL) + pConf->ragetime;
			}
		}
	}
}

void CSceneBossRush::OnBossDie(Enemy* pBoss)
{
	if(Stoped())
	{
		return;
	}

	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(NULL == pRole)
	{
		SSError << " find not owner roleid = " << m_scene->GetOwnerID() << END;
		return ;
	}
	pRole->ResetHpMp();//不要让Role也死
	AddBuff(pRole, 1, 1);//无敌buff

	BossRushMgr::Instance()->FinishOneCount(pRole, true);

	SetStopFlag(true);

	ClearBuffInfo();
}

void CSceneBossRush::Update()
{
	CheckAddBuff();
}

void CSceneBossRush::SetLastBoss(Enemy* pBoss)
{
	ClearLastBoss();
	if(pBoss)
	{
		m_lastBossAppearT = time(NULL);
		m_bossID = pBoss->GetID();
	}
}

void CSceneBossRush::ClearLastBoss()
{
	m_lastBossAppearT = time_t(0);
	m_bossID = 0;
}

void CSceneBossRush::ClearBuffInfo()
{
	m_buffID = 0;
	m_buffLevel = 0;
	m_addBuffTime = 0;
}

void CSceneBossRush::CheckAddBuff()
{
	if(m_addBuffTime && m_addBuffTime <= time(NULL))
	{
		Enemy* pBoss = EnemyManager::Instance()->FindEnemy(m_bossID);
		if(pBoss && m_buffID && m_buffLevel)
		{
			//SSDebug << __FUNCTION__ << " buffid = " << m_buffID << " bufflevel = " << m_buffLevel << END;
			XAIEventArgs args;
			args.Templateid = pBoss->GetTemplateID();	
			args.SkillId = m_buffID;
			args.DelayTime = 1.0f;
			args.EventArg = "addbuff";
			m_scene->MsgChannel().SendMsgTo(AISceneMsgChannel::GlobalAI, AISceneMsgChannel::Cmd, args);
		}

		ClearBuffInfo();
	}
}

bool CSceneBossRush::IsWin()
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(NULL == pRole)
	{
		return false;
	}
	BRRoleState* pState = pRole->Get<StageMgr>()->GetBRRoleState();
	if(pState && pState->rank >= BossRushMgr::Instance()->GetMaxRank())
	{
		SSInfo << " bossrush win role = " << m_scene->GetOwnerID() << END;
		return true;
	}
	return false;
}

void CSceneBossRush::OnLeaveScene(Scene *pScene, Role* role)
{
	if(Stoped())
	{
		return;
	}

	BossRushMgr::Instance()->FinishOneCount(role, false);

	SetStopFlag(true);
}

void CSceneBossRush::OnEnemyDie(Scene* pScene, Enemy* enemy)
{
	if(enemy->IsBoss())
	{
		OnBossDie(enemy);
	}
}

void CSceneBossRush::SetStopFlag(bool flag)
{
	m_stopFlag = flag;
}

void CSceneBossRush::MakeCountinue()
{
	ResetAll();
	SetStopFlag(false);
	m_scene->InitSceneStartTime();
}

void CSceneBossRush::TestDead(bool rolefirst)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	Enemy* pBoss = EnemyManager::Instance()->FindEnemy(m_bossID);
	if(NULL == pRole || NULL == pBoss)
	{
		return ;
	}
	if(rolefirst)
	{
		if(pRole->GetAttr(BASIC_CurrentHP) <= 1)
		{
			pRole->TriggerDeath();
		}
		if(pBoss->GetAttr(BASIC_CurrentHP) <= 1)
		{
			pBoss->TriggerDeath();
		}
	}
	else
	{
		if(pBoss->GetAttr(BASIC_CurrentHP) <= 1)
		{
			pBoss->TriggerDeath();
		}
		if(pRole->GetAttr(BASIC_CurrentHP) <= 1)
		{
			pRole->TriggerDeath();
		}
	}
}

void CSceneBossRush::OnRoleDie(Scene* pScene, Role* role)
{
	if(Stoped())
	{
		return;
	}

	BossRushMgr::Instance()->FinishOneCount(role, false);

	SetStopFlag(true);
}

void CSceneBossRush::StopAIHurt()
{
	FindEnemyCondition cond;
	vector<Unit *> enemys;
	m_scene->GetUnitByCondition(cond, enemys);
	//停掉隐形怪技能和AI
	for(size_t i = 0; i < enemys.size(); ++i)
	{
		if(enemys[i]->GetXObject()->SkillComponent())
		{
			enemys[i]->GetXObject()->SkillComponent()->EndSkill();
		}
		if(!enemys[i]->IsDead())
		{
			enemys[i]->TriggerDeath();
			//XAIDisableEventArgs oArgs;
			//enemys[i]->GetXObject()->DispatchAction(&oArgs);
		}
	}
}

void CSceneBossRush::ResetAll()
{
	StopAIHurt();

	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(NULL == pRole)
	{
		return ;
	}

	if(pRole->GetXObject()->BuffComponent())
	{
		pRole->GetXObject()->BuffComponent()->ClearBuff();
	}
	if(pRole->GetXObject()->SkillComponent())
	{
		pRole->GetXObject()->SkillComponent()->EndSkill();
	}
	pRole->GetXRole()->CoolDownAllSkills();

	///> 恢复角色身上的buff
	pRole->RecoverBuff();
	//addbuff
	auto pState = pRole->Get<StageMgr>()->GetBRRoleState();
	if(pState)
	{
		auto buffconf = BossRushMgr::Instance()->GetBuffConf(pState->brbid1);
		if(buffconf && buffconf->AttrBuff[0] && buffconf->AttrBuff[1])
		{
			AddBuff(pRole, buffconf->AttrBuff[0], buffconf->AttrBuff[1]);
		}
		buffconf = BossRushMgr::Instance()->GetBuffConf(pState->brbid2);
		if(buffconf && buffconf->AttrBuff[0] && buffconf->AttrBuff[1])
		{
			AddBuff(pRole, buffconf->AttrBuff[0], buffconf->AttrBuff[1]);
		}
	}

	pRole->ResetHpMp();
}

void CSceneBossRush::AddBuff(Role* pRole, UINT32 buffid, UINT32 bufflevel)
{
	if(pRole && pRole->GetXObject())
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = buffid;
		args.singleBuff.BuffLevel = bufflevel;
		//SSWarn << __FUNCTION__ << " " << buffid << " " << bufflevel << END;
		args.Caster = pRole->GetID();
		pRole->GetXObject()->DispatchAction(&args);
	}
}
