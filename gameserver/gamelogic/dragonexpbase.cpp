#include "pch.h"
#include "dragonexpbase.h"
#include "pb/project.pb.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "unit/enemy.h"
#include "stagemgr.h"
#include "scene/scenefinder.h"
#include "scene/DragonExpConfig.h"
#include "bagtransition.h"
#include "event/eventmgr.h"

void CDEStageProgress::SetBossHpPercent(UINT32 bossid, int percent)
{
	DEBossHpPercent BHP;
	BHP.bossID = bossid;
	BHP.bossHpPercent = percent;

	m_bossHpPercent[BHP.bossID] = BHP;
}

int CDEStageProgress::GetBossHpPercent(UINT32 bossid)
{
	auto it = m_bossHpPercent.find(bossid);
	if(it == m_bossHpPercent.end())
	{
		return DEDefaultHpPercent;
	}
	return (it->second).bossHpPercent;
}

int CDEStageProgress::GetAVGBossHpPercent()
{
	int count = 0;
	int allpercent = 0;

	for(auto it = m_bossHpPercent.begin(); it != m_bossHpPercent.end(); ++it)
	{
		allpercent += ((it->second).bossHpPercent);
		++count;
	}

	int avg = DEDefaultHpPercent; 
	if(count > 0)
	{
		avg = allpercent / count;
		if(allpercent % count)
		{
			avg ++;
		}
	}
	return avg;
}

void CDEStageProgress::Load(const KKSG::DEStageProgress& sp)
{
	//SSDebug << __FUNCTION__ << " sceneid = " << m_sceneID << END;
	m_bossHpPercent.clear();
	for(int i = 0; i < sp.bossids_size() && i < sp.bosshppercenet_size(); ++i)
	{
		DEBossHpPercent BHP;
		BHP.bossID = sp.bossids(i);
		BHP.bossHpPercent = sp.bosshppercenet(i);
		m_bossHpPercent[BHP.bossID] = BHP;
		//SSDebug << __FUNCTION__ << " bossid = " << (BHP).bossID << " hppercent = " << (BHP).bossHpPercent << END;
	}
}

void CDEStageProgress::Save(KKSG::DEStageProgress& sp)
{
	//SSDebug << __FUNCTION__ << " sceneid = " << m_sceneID << END;
	sp.set_sceneid(m_sceneID);
	for(auto it = m_bossHpPercent.begin(); it != m_bossHpPercent.end(); ++it)
	{
		sp.add_bossids((it->second).bossID);
		sp.add_bosshppercenet((it->second).bossHpPercent);
		//SSDebug << __FUNCTION__ << " bossid = " << (it->second).bossID << " hppercent = " << (it->second).bossHpPercent << END;
	}
}

void CDESceneHandler::OnRoleEnter(Role* pRole)
{
	//SSDebug << __FUNCTION__ << END;
	if(m_scene->GetSceneTemplateID() != DragonExpConfig::Instance()->GetFightSceneID(pRole))
	{
		m_errFlag = true;
		SSError << " enter error dragon exp roleid = " << pRole->GetID() << " sceneid = " << m_scene->GetSceneTemplateID() << END;
	}

	m_oldBossHpPercent = pRole->Get<StageMgr>()->GetBossAVGHpProgress(m_scene->GetSceneTemplateID());

	//for test
	//double fvalue = pRole->GetAttr(BASIC_MaxHP);
	//pRole->SetAttr(BASIC_MaxHP, fvalue*10000);
	//pRole->SetAttr(BASIC_CurrentHP, fvalue*10000);
}

void CDESceneHandler::OnRoleLeave(Role* pRole)
{
	SSDebug << __FUNCTION__ << END;
	SaveLeftBossHpPercent(pRole);
	if(!m_rewardFlag)
	{
		KKSG::DragonExpResult tmpr;
		FillResult(&tmpr, pRole, false);
	}
}

void CDESceneHandler::OnBossDie(Enemy* pBoss)
{
	SSDebug << __FUNCTION__ << END;
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(pBoss && pRole && !m_saveFlag)
	{
		pRole->Get<StageMgr>()->SetBossHpProgress(m_scene->GetSceneTemplateID(), pBoss->GetTemplateID(), 0);
	}
	if (pRole && pBoss)
	{
		EventMgr::Instance()->AddGuildBonusEvent(pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_KILLDRAGON, pBoss->GetTemplateID());
	}
}

void CDESceneHandler::OnBossApper(Enemy* pBoss)
{//最好在加入场景之前调用
	SSDebug << __FUNCTION__ << END;
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(pBoss && pRole)
	{
		int hpPrecent = pRole->Get<StageMgr>()->GetBossHpProgress(m_scene->GetSceneTemplateID(), pBoss->GetTemplateID());
		if(0 == hpPrecent)
		{
			;//是否需要特殊处理?
		}
		else if(100 == hpPrecent)
		{
			;
		}
		SSDebug << __FUNCTION__ << " sceneid = " << m_scene->GetSceneTemplateID() << " bosstemplateid = " << pBoss->GetTemplateID() << " hpprecent = " << hpPrecent << END;
		double fvalue = pBoss->GetAttr(BASIC_MaxHP);
		fvalue = fvalue * hpPrecent / 100;
		pBoss->SetAttr(BASIC_CurrentHP, fvalue);

		UINT32 buffid = 0;
		UINT32 bufflevel = 0;
		DragonExpConfig::Instance()->GetSealBuff(buffid, bufflevel, m_scene->GetSceneTemplateID());
		if(0 != buffid)
		{
			XAddBuffActionArgs args;
			args.singleBuff.buffID = buffid;
			args.singleBuff.BuffLevel = bufflevel;
			args.Caster = pBoss->GetID();
			pBoss->GetXObject()->DispatchAction(&args);
		}
	}
}

CDESceneHandler::CDESceneHandler(Scene* pScene)
{
	SSDebug << __FUNCTION__ << END;
	m_oldBossHpPercent = 100;
	m_scene = pScene;
	m_saveFlag = false;
	m_errFlag = false;
	m_rewardFlag = false;
}

CDESceneHandler::~CDESceneHandler()
{
	SSDebug << __FUNCTION__ << END;
}

void CDESceneHandler::SaveLeftBossHpPercent(Role* pRole)
{
	if(m_saveFlag)
	{
		return ;
	}
	m_saveFlag = true;
	SSDebug << __FUNCTION__ << END;

	if(NULL == pRole)
	{
		return ;
	}
	StageMgr* stagemgr = pRole->Get<StageMgr>();

	std::vector<Unit*> bossVec;
	FindBossCondition cond;
	m_scene->GetUnitByCondition(cond, bossVec);
	for(size_t i = 0; i < bossVec.size(); ++i)
	{
		if(!bossVec[i]->IsDead())
		{
			int newleft = (int)ceil(bossVec[i]->GetPreciseHpPercent());
			int oldleft = pRole->Get<StageMgr>()->GetBossHpProgress(m_scene->GetSceneTemplateID(), bossVec[i]->GetTemplateID());
			if(newleft > oldleft)//取整问题
			{
				SSDebug << " old left = " << oldleft << " new left = " << newleft << END;
				newleft = oldleft;
			}
			if(newleft <= 0)
			{
				newleft = 1;
			}
			stagemgr->SetBossHpProgress(m_scene->GetSceneTemplateID(), bossVec[i]->GetTemplateID(), newleft);
		}
	}

	DragonExpConfig::Instance()->SynProgressToMS(pRole, true);
}
//要处理玩家和boss同时死亡的情况 *****
void CDESceneHandler::OnSceneUpdate(int sceneState)
{
	if(SCENE_RUNNING == sceneState)
	{
		return;
	}
		
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_scene->GetOwnerID());
	if(NULL == pRole)
	{
		return ;
	}
	SSDebug << __FUNCTION__ << " iswin = " << (int)(SCENE_WIN == sceneState) << END;
	if(SCENE_WIN == sceneState || SCENE_LOSE == sceneState)
	{
		SaveLeftBossHpPercent(pRole);
	}

}

void CDESceneHandler::FillResult(KKSG::DragonExpResult* pDEResult, Role* pRole, bool iswin)
{
	if(NULL == pDEResult || NULL == pRole || m_errFlag)
	{
		return ;
	}
	if(m_rewardFlag)
	{
		return ;
	}
	m_rewardFlag = true;

	int bossnowhppercent = pRole->Get<StageMgr>()->GetBossAVGHpProgress(m_scene->GetSceneTemplateID());	
	pDEResult->set_bosshurthp(m_oldBossHpPercent - bossnowhppercent);
	pDEResult->set_bosslefthp(bossnowhppercent);
	pDEResult->set_iswin(iswin);
	pDEResult->set_sceneid(m_scene->GetSceneTemplateID());

	std::vector<Sequence<UINT32,2>> joinreward;
	std::vector<Sequence<UINT32,2>> winreward;
	DragonExpConfig::Instance()->GetJoinReward(m_scene->GetSceneTemplateID(), joinreward);
	if(iswin)
	{
		DragonExpConfig::Instance()->GetWinReward(m_scene->GetSceneTemplateID(), winreward);	
	}

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Stage, ItemFlow_StageDragonExp);

	for(size_t i = 0; i < joinreward.size(); ++i)
	{
		give.GiveItem(joinreward[i][0], joinreward[i][1], false);

		KKSG::ItemBrief* pItemBrief = pDEResult->add_joinreward();
		pItemBrief->set_itemid(joinreward[i][0]);
		pItemBrief->set_itemcount(joinreward[i][1]);
	}
	for(size_t i = 0; i < winreward.size(); ++i)
	{
		give.GiveItem(winreward[i][0], winreward[i][1], false);

		KKSG::ItemBrief* pItemBrief = pDEResult->add_winreward();
		pItemBrief->set_itemid(winreward[i][0]);
		pItemBrief->set_itemcount(winreward[i][1]);
	}

	give.NotifyClient();
}

