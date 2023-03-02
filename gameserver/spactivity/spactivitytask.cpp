#include "pch.h"
#include "spactivitytask.h"
#include "spactivityrecord.h"
#include "spactivitydatabase.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "define/tlogenum.h"

std::map<UINT32, SpActTaskHandler*> SpActTaskHandler::m_handlers;

void SpActTaskHandler::InitHandlers()
{
#define INSERT(x, handler, ...) \
	if (m_handlers.find(x) != m_handlers.end()) \
	{\
		LogWarn("TaskHandler, duplicated key [%d]", x); \
		delete m_handlers[x]; \
	}\
	m_handlers[x] = new handler(x, ## __VA_ARGS__);

	INSERT(SpActTask_PassOneScene,		SpActTaskHandlerArg, 1, 0);
	INSERT(SpActTask_JoinSceneType,		SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_HitWorldBoss,		SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_HitGuildBoss,		SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_Login,				SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_VipLogin,			SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_GuildDare,			SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_GuildQA,			SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_EnhanceMaster,		SpActTaskHandlerArg, 1, 0);
	INSERT(SpActTask_GetJade,			SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_SendFriendGift,	SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_LevelUp,			SpActTaskHandlerArg, 1, 0);
	INSERT(SpActTask_Collect,			SpActTaskHandlerCollect);
	INSERT(SpActTask_CompleteSceneTypeWithPartner, SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_CompleteSceneType,	SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_GetActivityPoint,	SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_HelperSceneType,	SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_HelperSceneID,		SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_CompleteSceneID,	SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_JoinGuild,			SpActTaskHandlerArg, 0, 0);
	INSERT(SpActTask_FriendNum,			SpActTaskHandlerArg, 1, 0);
	INSERT(SpActTask_GuildDonate,		SpActTaskHandlerArg, 1, 1);
	INSERT(SpActTask_SubTaskComplete,	SpActTaskHandlerArg, 2, 2);
	INSERT(SpActTask_CompleteSmallDragonNest, SpActTaskHandlerArg, 1, 1);

#undef INSERT
}

SpActTaskHandler* SpActTaskHandler::GetTaskHandler(UINT32 baseTask)
{
	if (m_handlers.empty())
	{
		InitHandlers();
	}
	auto iter = m_handlers.find(baseTask);
	if (iter != m_handlers.end())
	{
		return iter->second;
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActTaskHandlerArg::SpActTaskHandlerArg(UINT32 baseTask, UINT32 argsSize, UINT32 judgePos):SpActTaskHandler(baseTask)
{
	m_argsSize = argsSize;
	m_judgePos = judgePos;
	assert (m_argsSize >= m_judgePos);
}

bool SpActTaskHandlerArg::CheckTaskConfValid(const SpActivityTaskConf* conf)
{
	if (conf == NULL)
	{
		return false;
	}
	return conf->basetask == m_baseTask && conf->num.size() == m_argsSize;
}

bool SpActTaskHandlerArg::IncreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count)
{
	if (pRole == NULL || conf == NULL)
	{
		return false;
	}
	if (!CheckTaskConfValid(conf))
	{
		LogWarn("What!!!Why SpActivityTaskConf [%u] not valid?", conf->taskid);
		return false;
	}

	switch (conf->tasktype)
	{
	case SpActivityTask_Normal:
		{
			if (spActTask.state == SpActTaskState_AlreadyGet)
			{
				return false;
			}
			break;
		}
	case SpActivityTask_Father:
		{
			LogError("taskid [%u], taskfather shouldn't in IncreaseProgress", conf->taskid);
			return false;
		}
	default:
		{
			break;
		}
	}

	if (m_judgePos == 0)
	{
		if (count != 0)
		{
			spActTask.state = SpActTaskState_Completed;
		}
	}
	else
	{
		spActTask.progress += count;
		if (spActTask.progress >= conf->num[m_judgePos - 1])
		{
			spActTask.state = SpActTaskState_Completed;
			if (conf->tasktype == SpActivityTask_Normal)
			{
				spActTask.progress = conf->num[m_judgePos - 1];
			}
		}
		else
		{
			spActTask.state = SpActTaskState_NotComplete;
		}
	}

	return true;
}

bool SpActTaskHandlerArg::DecreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count)
{
	return false;
}

bool SpActTaskHandlerArg::DecreaseSonTaskProgressOneTime(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf)
{
	if (pRole == NULL || conf == NULL)
	{
		return false;
	}
	if (!CheckTaskConfValid(conf))
	{
		LogWarn("What!!!Why SpActivityTaskConf [%u] not valid?", conf->taskid);
		return false;
	}
	if (conf->tasktype != SpActivityTask_Son)
	{
		LogWarn("taskid [%u], tasktype [%u] not taskson in function [%s]", conf->taskid, conf->tasktype, __FUNCTION__);
		return false;
	}
	if (spActTask.state != SpActTaskState_Completed)
	{
		LogError("the task:%u should be completed, but state is [%u]", conf->taskid, spActTask.state);
		return false;
	}

	if (m_judgePos == 0)
	{
		spActTask.state = SpActTaskState_NotComplete;
	}
	else
	{
		if (spActTask.progress >= conf->num[m_judgePos - 1]) spActTask.progress -= conf->num[m_judgePos - 1];
		else spActTask.progress = 0;

		if (spActTask.progress >= conf->num[m_judgePos - 1])
		{
			spActTask.state = SpActTaskState_Completed;
		}
		else
		{
			spActTask.state = SpActTaskState_NotComplete;
		}
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SpActTaskHandlerCollect::SpActTaskHandlerCollect(UINT32 baseTask):SpActTaskHandler(baseTask)
{
	assert(baseTask == SpActTask_Collect);
}

bool SpActTaskHandlerCollect::CheckTaskConfValid(const SpActivityTaskConf* conf)
{
	if (conf == NULL)
	{
		return false;
	}
	return conf->basetask == m_baseTask && conf->num.size() >= 2;
}

bool SpActTaskHandlerCollect::IncreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count)
{
	if (pRole == NULL || conf == NULL)
	{
		return false;
	}
	if (!CheckTaskConfValid(conf))
	{
		LogWarn("What!!!Why SpActivityTaskConf [%u] not valid?", conf->taskid);
		return false;
	}

	switch (conf->tasktype)
	{
	case SpActivityTask_Normal:
		{
			if (spActTask.state != SpActTaskState_NotComplete)
			{
				return false;
			}
			spActTask.progress += count;
			if (spActTask.progress >= conf->num[1])
			{
				spActTask.progress = conf->num[1];
				spActTask.state = SpActTaskState_Completed;
			}
			return true;
		}
	case SpActivityTask_Father:
		{
			LogError("taskid [%u], taskfather shouldn't in IncreaseProgress", conf->taskid);
			return false;
		}
	case SpActivityTask_Son:
		{
			SpActivityDataBase* pBase = pRole->Get<SpActivityRecord>()->GetSpActData(conf->actid);
			if (pBase == NULL)
			{
				LogError("SpActivityDataBase [%u] not find in role [%llu] SpActivityRecord in function [%s]", conf->actid, pRole->GetID(), __FUNCTION__);
				return false;
			}
			const SpActTask* pFatherTask = pBase->GetTask(conf->taskfather);
			if (pFatherTask == NULL)
			{
				LogError("taskid [%u], not has taskfather [%u] in task map, role [%llu]", conf->taskid, conf->taskfather, pRole->GetID());
				return false;
			}
			UINT32 pos = pFatherTask->progress + 1;
			if (pos >= conf->num.size())
			{
				pos = conf->num.size() - 1;
			}
			spActTask.progress = pRole->Get<Bag>()->CountItemInBag(conf->num[0]);
			if (spActTask.progress >= conf->num[pos])
			{
				spActTask.state = SpActTaskState_Completed;
			}
			else
			{
				spActTask.state = SpActTaskState_NotComplete;
			}
			return true;
		}
	default:
		{
			return false;
		}
	}
}

bool SpActTaskHandlerCollect::DecreaseProgress(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf, UINT32 count)
{
	if (pRole == NULL || conf == NULL)
	{
		return false;
	}
	if (!CheckTaskConfValid(conf))
	{
		LogWarn("What!!!Why SpActivityTaskConf [%u] not valid?", conf->taskid);
		return false;
	}
	///> 收集任务，只有子任务才会减，普通任务不减(普通收集任务领取时不会扣物品)
	if (conf->tasktype != SpActivityTask_Son)
	{
		return false;
	}
	SpActivityDataBase* pBase = pRole->Get<SpActivityRecord>()->GetSpActData(conf->actid);
	if (pBase == NULL)
	{
		LogError("SpActivityDataBase [%u] not find in role [%llu] SpActivityRecord in function [%s]", conf->actid, pRole->GetID(), __FUNCTION__);
		return false;
	}
	const SpActTask* pFatherTask = pBase->GetTask(conf->taskfather);
	if (pFatherTask == NULL)
	{
		LogError("taskid [%u], not has taskfather [%u] in task map, role [%llu]", conf->taskid, conf->taskfather, pRole->GetID());
		return false;
	}
	UINT32 pos = pFatherTask->progress + 1;
	if (pos >= conf->num.size())
	{
		pos = conf->num.size() - 1;
	}
	spActTask.progress = pRole->Get<Bag>()->CountItemInBag(conf->num[0]);
	if (spActTask.progress >= conf->num[pos])
	{
		spActTask.state = SpActTaskState_Completed;
	}
	else
	{
		spActTask.state = SpActTaskState_NotComplete;
	}
	return true;
}

bool SpActTaskHandlerCollect::DecreaseSonTaskProgressOneTime(Role* pRole, SpActTask& spActTask, const SpActivityTaskConf* conf)
{
	if (pRole == NULL || conf == NULL)
	{
		return false;
	}
	if (!CheckTaskConfValid(conf))
	{
		LogWarn("What!!!Why SpActivityTaskConf [%u] not valid?", conf->taskid);
		return false;
	}
	if (conf->tasktype != SpActivityTask_Son)
	{
		return false;
	}
	if (spActTask.state != SpActTaskState_Completed)
	{
		LogError("the task:%u should be completed, but state is [%u]", conf->taskid, spActTask.state);
		return false;
	}

	SpActivityDataBase* pBase = pRole->Get<SpActivityRecord>()->GetSpActData(conf->actid);
	if (pBase == NULL)
	{
		LogError("SpActivityDataBase [%u] not find in role [%llu] SpActivityRecord in function [%s]", conf->actid, pRole->GetID(), __FUNCTION__);
		return false;
	}
	const SpActTask* pFatherTask = pBase->GetTask(conf->taskfather);
	if (pFatherTask == NULL)
	{
		LogError("taskid [%u], not has taskfather [%u] in task map, role [%llu]", conf->taskid, conf->taskfather, pRole->GetID());
		return false;
	}
	UINT32 pos = pFatherTask->progress + 1;
	if (pos >= conf->num.size())
	{
		pos = conf->num.size() - 1;
	}

	UINT32 needTake = conf->num[pos];
	UINT32 count = pRole->Get<Bag>()->CountItemInBag(conf->num[0]);
	if (count < needTake)
	{
		needTake = count;
		LogError("it should not happen, table wrong or progarm wrong"); ///> 如果一个收集父任务里配了多个相同物品id的子任务，就会出现这种情况
	}
	BagTakeItemTransition transition(pRole);
	transition.SetReason(ItemFlow_SpActivity, ItemFlow_SpActivity_Collect);
	transition.TakeItem(conf->num[0], needTake);
	transition.NotifyClient();

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////