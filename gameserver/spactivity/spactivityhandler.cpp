#include "pch.h"
#include "spactivityhandler.h"
#include "spactivitymgr.h"
#include "spactivityconfig.h"
#include "scene/scene.h"
#include "spactivityrecord.h"
#include "unit/role.h"
#include "pb/project.pb.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/loginrecord.h"
#include "define/tlogenum.h"
#include "gamelogic/jademgr.h"
#include "role/ptcg2c_opensystemntf.h"
#include "define/qadef.h"
#include "table/OpenSystemMgr.h"
#include "spactivitybase.h"
#include "spactivitydef.h"
#include "gamelogic/activityrecord.h"
#include "pb/enum.pb.h"
#include "scene/sceneteam.h"
#include "guild/guildmgr.h"
#include "table/TaskMgr.h"
#include "gamelogic/dragonconfig.h"

SpActivityHandler SpActivityHandler::GlobalHandler;


/*void ForEachItem(XItem* pItem, Role* pRole, UINT32 actid)
{
	if (pItem == NULL) return;
	SpActivityHandler::GlobalHandler.CompleteGetItemTask(pRole, pItem->itemID, pItem->itemCount, ItemFlow_SpActivity, ItemFlow_SpActivity_Collect, actid);
}*/

void SpActivityHandler::CompleteTaskAfterStartSpActivity(Role* pRole, UINT32 actid)
{
	///> 由于原本完成任务的设计是触发式的
	///> 这里可以在活动开启的时候（在线开启活动，或者刚上线后触发开启活动）做一些没有触发就能直接完成的任务	
	//ItemCB callback = std::bind(ForEachItem, std::placeholders::_1, pRole, actid);
	//pRole->Get<Bag>()->ForeachItem(callback);

	CompleteLevelUpTask(pRole, actid);

	UpdateTaskWhenEnterHallGS(pRole);
}

void SpActivityHandler::UpdateTaskWhenEnterHallGS(Role* pRole)
{
	if (GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID()) != NULL)
	{
		OnInGuild(pRole);
	}
	OnFriendChange(pRole);
}

void SpActivityHandler::CompleteGetItemTask(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason, UINT32 actid)
{
	std::vector<SpActivityTaskConf*> task;
	if (XItem::GetType(itemID) == JADE)
	{
		if(nReason == ItemFlow_Auction && nSubReason == ItemFlow_Auction_GetBack)
		{
			return; ///> 上架后下架，不算增加
		}

		if(nReason == ItemFlow_Jade && nSubReason == ItemFlow_Jade_Compose)
		{
			return; ///> 龙玉合成在OnComposeJade里算
		}

		if(nReason == ItemFlow_Jade && nSubReason == ItemFlow_Jade_TakeOff)
		{
			return; ///> 脱龙玉不算增加
		}

		UINT32 jadeLevel = CJadeMgr::Instance()->GetLevel(itemID);
		SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_GetJade, jadeLevel), task, actid);
	}

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_Collect, itemID), task, actid);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task, count);
}

void SpActivityHandler::CompleteLevelUpTask(Role* pRole, UINT32 actid)
{
	std::vector<SpActivityTaskConf*> allTask;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_LevelUp), allTask, actid);

	UINT32 level = pRole->GetLevel();
	std::vector<SpActivityTaskConf*> task;
	for (auto i = allTask.begin(); i != allTask.end(); i++)
	{
		if (level >= (*i)->num[0])
		{
			task.push_back(*i);
		}
	}

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnHallGsEnterScene(Role* pRole, Scene* pScene)
{
	UpdateTaskWhenEnterHallGS(pRole);
}

void SpActivityHandler::OnCompleteScene(Role* pRole, Scene *pScene)
{
	std::vector<SpActGetTaskConfArg> vecArgs;
	if (pScene->IsHelper(pRole->GetID())) 
	{
		vecArgs.push_back(SpActGetTaskConfArg(SpActTask_HelperSceneID, pScene->GetSceneTemplateID()));
		vecArgs.push_back(SpActGetTaskConfArg(SpActTask_HelperSceneType, pScene->GetSceneType()));
	}
	else
	{
		vecArgs.push_back(SpActGetTaskConfArg(SpActTask_PassOneScene, pScene->GetSceneTemplateID()));
		vecArgs.push_back(SpActGetTaskConfArg(SpActTask_JoinSceneType, pScene->GetSceneType()));
		vecArgs.push_back(SpActGetTaskConfArg(SpActTask_CompleteSceneType, pScene->GetSceneType()));
		vecArgs.push_back(SpActGetTaskConfArg(SpActTask_CompleteSceneID, pScene->GetSceneTemplateID()));
	}

	SceneTeam* pTeam = pScene->GetSceneTeam();
	if (pTeam != NULL)
	{
		const DragonNestTable::RowData* data = DragonConfig::Instance()->GetDragonTableData(pTeam->GetExpID());
		if (data != NULL && data->DragonNestDifficulty == 0)
		{
			vecArgs.push_back(SpActGetTaskConfArg(SpActTask_CompleteSmallDragonNest));
		}
	}

	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, vecArgs, task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);

	if (pScene->GetSceneType() == KKSG::SCENE_NEST)
	{
		pRole->Get<SpActivityRecord>()->OnFinishNest();
	}
}

void SpActivityHandler::OnCompleteSceneWithPartner(Role* pRole, Scene* pScene, std::vector<Role*>& partners, bool isWin)
{
	if (isWin == false) return;

	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_CompleteSceneTypeWithPartner, pScene->GetSceneType()), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnSceneFailed(Role* pRole, Scene *poScene, bool isEndScene, bool isdraw)
{
	if (false == isEndScene) return; //必须要打完才能算
	if (poScene->IsHelper(pRole->GetID())) return; //助战不算在里面

	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_JoinSceneType, poScene->GetSceneType()), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnSweepScene(Role *pRole, SceneInfo *pSceneInfo)
{
	if (pSceneInfo == NULL || pSceneInfo->m_pConf == NULL) return;

	std::vector<SpActGetTaskConfArg> vecArgs;
	vecArgs.push_back(SpActGetTaskConfArg(SpActTask_PassOneScene, pSceneInfo->m_pConf->id));
	vecArgs.push_back(SpActGetTaskConfArg(SpActTask_JoinSceneType, pSceneInfo->m_pConf->type));
	vecArgs.push_back(SpActGetTaskConfArg(SpActTask_CompleteSceneType, pSceneInfo->m_pConf->type));

	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, vecArgs, task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);

	if (pSceneInfo->m_pConf->type == KKSG::SCENE_NEST)
	{
		pRole->Get<SpActivityRecord>()->OnFinishNest();
	}
}

void SpActivityHandler::OnHitWorldBoss(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_HitWorldBoss), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnHitGuildBoss(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_HitGuildBoss), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnLoginOneDay(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	int vipLevel = pRole->Get<CPayV2Record>()->GetVipLevel();

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_Login), task);
	if(vipLevel > 0)
	{
		SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_VipLogin), task);
	}

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnGuildDare(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_GuildDare), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnQA(Role* pRole, UINT32 type)
{
	if(type != QATYPE_GUILD) return;

	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_GuildQA), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnEnhanceMaster(Role* pRole, UINT32 level)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_EnhanceMaster, level), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnGetItemCountAndReason(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason)
{
	CompleteGetItemTask(pRole, itemID, count, nReason, nSubReason, 0);	
}

void SpActivityHandler::OnTakeItemCountAndReason(Role* pRole, UINT32 itemID, UINT32 count, int nReason, int nSubReason)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_Collect, itemID), task);

	pRole->Get<SpActivityRecord>()->OnDecreaseProgress(task, count);
}

void SpActivityHandler::OnComposeJade(Role* pRole, UINT32 jadeLevel, UINT32 count)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_GetJade, jadeLevel), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task, count);
}

void SpActivityHandler::OnSendFriendGift(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_SendFriendGift), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnLevelup(Role* pRole)
{
	SpActivityMgr::Instance()->UpdateSpActivityData(pRole);

	CompleteLevelUpTask(pRole, 0);

	pRole->Get<SpActivityRecord>()->CheckSendTickets();
}

void SpActivityHandler::OnAddActivityPoint(Role* pRole, UINT32 point)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_GetActivityPoint), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task, point);

	pRole->Get<SpActivityRecord>()->OnAddActivityPoint(point);
}

void SpActivityHandler::OnInGuild(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_JoinGuild), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}

void SpActivityHandler::OnFriendChange(Role* pRole)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_FriendNum), task);

	UINT32 friendNum = pRole->GetFriendList()->size();

	std::vector<SpActivityTaskConf*> finishTask;
	for (auto i = task.begin(); i != task.end(); ++i)
	{
		if (friendNum >= (*i)->num[0])
		{
			finishTask.push_back(*i);
		}
	}

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(finishTask);
}

void SpActivityHandler::OnGuildDonate(Role* pRole, UINT32 count)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_GuildDonate), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task, count);
}

void SpActivityHandler::OnSubTaskComplete(Role* pRole, UINT32 type)
{
	std::vector<SpActivityTaskConf*> task;

	SpActivityConfig::Instance()->GetRoleHaveTaskConf(pRole, SpActGetTaskConfArg(SpActTask_SubTaskComplete, type), task);

	pRole->Get<SpActivityRecord>()->OnIncreaseProgress(task);
}
