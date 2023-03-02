#include "pch.h"
#include "enterscenecondition.h"
#include "sceneconfig.h"
#include "unit/role.h"
#include "scene.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/stagemgr.h"
#include "table/globalconfig.h"
#include "util/XCommon.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "scenepk.h"
#include "guild/guildrecord.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/towerrecord.h"
#include "guild/guilddef.h"
#include "gamelogic/taskrecord.h"
//#include "gamelogic/stagemgr.h"

int EnterSceneFatigueCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg)
{
	// 扣体力
	Bag *pBag = pRole->Get<Bag>();
	for (unsigned i = 0; i < pSceneInfo->m_pConf->FatigueCost.size(); ++i)
	{
		Sequence<int,2> &seq = pSceneInfo->m_pConf->FatigueCost[i];
		if (seq[0] < VIRTUAL_ITEM_MAX)
		{
			if (pBag->CountVirtualItem(seq[0]) < seq[1])
			{
				LogInfo("Enter Scene not have %d/%d, sceneid:%d, roleid:%llu", seq[0], seq[1], pSceneInfo->m_pConf->id, pRole->GetID());
				return KKSG::ERR_SCENE_NOFATIGUE;
			}
		}
		else
		{
			if (pBag->CountItemInBag(seq[0]) < (UINT32)seq[1])
			{
				LogInfo("Enter Scene not have %d/%d, sceneid:%d, roleid:%llu", seq[0], seq[1], pSceneInfo->m_pConf->id, pRole->GetID());
				return KKSG::ERR_SCENE_NOFATIGUE;
			}
		}
	}

	return KKSG::ERR_SUCCESS;
}

void EnterSceneFatigueCondition::TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin)
{
	int scenetype = pSceneInfo->m_pConf->type;
	bool iscostcount = SceneConfig::Instance()->IsCostFatigue(scenetype, iswin);
	if (!iscostcount)
	{
		return;
	}
	BagTakeItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Enter);
	transition.SetParam(scenetype);
	for (unsigned i = 0; i < pSceneInfo->m_pConf->FatigueCost.size(); ++i)
	{
		Sequence<int,2> &seq = pSceneInfo->m_pConf->FatigueCost[i];
		if (!transition.TakeItem(seq[0], seq[1]))
		{
			transition.RollBack();
			return;
		} 
	}
	transition.NotifyClient();
}

int EnterSceneCountCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg)
{
	StageMgr* stage = pRole->Get<StageMgr>();
	stage->Update();
	if (!stage->CanPlayCount(pSceneInfo))
	{
		return KKSG::ERR_SCENE_TODYCOUNTLIMIT;		
	}
	return KKSG::ERR_SUCCESS;
}

void EnterSceneCountCondition::TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin)
{
	int scenetype = pSceneInfo->m_pConf->type;
	bool iscostcount = SceneConfig::Instance()->IsCostCount(scenetype, iswin);
	if (iscostcount)
	{
		//Scene* scene = pRole->GetCurrScene();
		//bool iscostexcept = (NULL == scene) ? false : scene->IsCostExcept(pRole->GetID());
		//if(!iscostexcept)
		//{
			StageMgr* stage = pRole->Get<StageMgr>();
			stage->Update();
			stage->AddScenePlayCount(pSceneInfo);
		//}
	}
}

int EnterSceneTimeLimitCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg)
{
	///> 进入场景CD限制
	UINT32 time = pRole->Get<StageMgr>()->CanPlayCoolDown(pSceneInfo);
	if (0 != time)
	{
		if (NULL != arg)
		{
			*arg = time;
		}
		return KKSG::ERR_SCENE_COOLDOWN;
	}	
	return KKSG::ERR_SUCCESS;
}

void EnterSceneTimeLimitCondition::TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin)
{
	int scenetype = pSceneInfo->m_pConf->type;
	bool iscooldown = SceneConfig::Instance()->IsCoolDown(scenetype, iswin);
	if (iscooldown)
	{
		pRole->Get<StageMgr>()->AddSceneCoolDown(pSceneInfo);
	}
}

int EnterSceneNormalCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg)
{
	/*	同场景观战非观战直接切换
	Scene *currScene = pRole->GetCurrScene();
	if (currScene && ((UINT32)pSceneInfo->m_pConf->id == currScene->GetSceneTemplateID()) && pSceneInfo->m_pConf->type != KKSG::SCENE_FAMILYGARDEN)
	{
		LogWarn("already in Scene %d, can't switch scene", currScene->GetSceneTemplateID());
		return KKSG::ERR_FAILED;
	}
	*/

	// not used temporally
	//if (currScene && ((UINT32)pSceneInfo->m_pConf->id == currScene->GetSceneTemplateID()))
	//{
	//	LogWarn("already in Scene %d, can't switch scene", currScene->GetSceneTemplateID());
	//	return KKSG::ERR_FAILED;
	//}

	if (!SceneConfig::Instance()->CheckPreScene(pRole, pSceneInfo->m_pConf->PreScene))
	{
		LogWarn("enter scene need pre scene");
		return KKSG::ERR_SCENE_NEED_PRESCENE;
	}

	if (pSceneInfo->m_pConf->PreTask != 0)
	{
		if (pRole->Get<TaskRecord>()->IsTaskFinish(pSceneInfo->m_pConf->PreTask) == false
			&& pRole->Get<TaskRecord>()->IsTaskDoing(pSceneInfo->m_pConf->PreTask) == false)
		{
			LogWarn("enter scene need pre task");
			return KKSG::ERR_SCENE_NEED_PRETASK;
		}
	}

	// 正在Loading场景
	if (pRole->GetLoadingScene() != NULL)
	{
		LogWarn("role is loading scene, can't switch scene");
		return KKSG::ERR_FAILED;
	}

	if (pRole->GetLevel() < pSceneInfo->m_pConf->RequiredLevel)
	{
		LogWarn("role level %d is less than scene requare level %d", pRole->GetLevel(), pSceneInfo->m_pConf->RequiredLevel);
		return KKSG::ERR_SCENE_LEVELREQ;
	}

	return KKSG::ERR_SUCCESS;
}

void EnterSceneNormalCondition::TakeEnterSceneCost(Role *pRole, SceneInfo *pSceneInfo, bool iswin)
{

}

int EnterGuildSceneCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg)
{
	if (pSceneInfo->m_pConf->type != KKSG::SCENE_GUILD_HALL)
	{
		return KKSG::ERR_SUCCESS;
	}

	if(!GSConfig::Instance()->IsHallGS())
	{
		return KKSG::ERR_SUCCESS;
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		return KKSG::ERR_SUCCESS;
	}

	if (GuildSimpleMgr::Instance()->GetGuildId(pRole->GetID()) == INVALID_GUILDID)
	{
		return KKSG::ERR_GUILD_NOT_IN_GUILD;
	}

	return KKSG::ERR_SUCCESS;
}

int EnterTowerSceneCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg /*= NULL*/)
{
	if (pSceneInfo->m_pConf->type != KKSG::SCENE_TOWER)
	{
		return KKSG::ERR_SUCCESS;
	}

	const TeamTowerRewardTable::RowData* data = TowerConfig::Instance()->GetTowerTableData(1, pRole->Get<CTowerRecord>()->GetStartFloor(1));
	if (!data)
	{
		return KKSG::ERR_FAILED;
	}
	if (pSceneInfo->m_pConf->id != data->SceneID)
	{
		return KKSG::ERR_FAILED;
	}

	return pRole->Get<CTowerRecord>()->CheckEnterScene(1, data->TowerFloor);
}

int EnterHolidaySceneCondition::CanEnterScene(Role *pRole, SceneInfo *pSceneInfo, UINT32* arg/* = NULL*/)
{
	if (pSceneInfo->m_pConf->syncMode != 0) // 只检查单人模式
	{
		return KKSG::ERR_SUCCESS;
	}

	return pRole->Get<StageMgr>()->HolidayCheck(pSceneInfo->m_pConf->id);
}
