#include "pch.h"
#include "spactivitybase.h"
#include "util/timespecificmgr.h"
#include "spactivityconfig.h"
#include "util/gametime.h"
#include "spactivity/ptcg2c_spactivityoffsetdayntf.h"
#include "util/XCommon.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivityrecord.h"
#include "unit/systeminfo.h"
#include "table/OpenSystemMgr.h"
#include "spactivitydef.h"
#include "spactivitymgr.h"
#include "spactivityhandler.h"
#include "define/tlogenum.h"
#include "spactivitydatabase.h"

SpActivityBase::SpActivityBase(UINT32 actid)
{
	m_stage = 0;
	m_handler = INVALID_TIMESPECIFIC;
	m_startTime = 0;
	m_actid = actid;
}

SpActivityBase::~SpActivityBase()
{
	if (m_handler != INVALID_TIMESPECIFIC)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(m_handler);
	}
}

void SpActivityBase::OnStart()
{
	std::vector<SpActivityTaskConf*> allTask;
	if (m_actid != SpActivity_LuckyTurntable)
	{
		SpActivityConfig::Instance()->GetAllTaskByActID(m_actid, allTask);
	}
	StartOnlinePlayer(allTask);
}

void SpActivityBase::OnNextStage()
{
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		pRole->Get<SpActivityRecord>()->AddSpActStage(m_actid);
	}
}

void SpActivityBase::OnEnd()
{
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		return;
	}

	std::vector<UINT32> closeids;
	closeids.push_back(conf->systemid);

	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); i++)
	{
		Role* pRole = *i;
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(conf->systemid))
		{
			pRole->Get<CRoleSystem>()->SetSystemOpened(conf->systemid, false);
			OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &closeids);	
		}
	}
}

void SpActivityBase::Reset()
{
	if (m_handler != INVALID_TIMESPECIFIC)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(m_handler);
		m_handler = INVALID_TIMESPECIFIC;
	}
	m_startTime = 0;
	m_stage = 0;
}

bool SpActivityBase::CheckOpenCondition(Role* pRole)
{
	if (m_stage == 0)
	{
		return false;
	}
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL || pRole == NULL)
	{
		return false;
	}
	return pRole->GetLevel() >= conf->minlevel;
}

void SpActivityBase::UpdateSpActivityData(Role* pRole, bool updateTask)
{
	if (pRole == NULL) return;
	///> 未开启
	if (m_stage == 0)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	if (!CheckOpenCondition(pRole))
	{
		pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		return;
	}
	bool isOpenNow = false;
	///> startTime or stage与当前不一样 重置任务
	SpActivityDataBase* pDataBase = pRole->Get<SpActivityRecord>()->GetSpActData(m_actid);
	if (pDataBase == NULL)
	{
		isOpenNow = true;
	}
	if (pDataBase != NULL)
	{
		if (pDataBase->GetStartTime() != m_startTime)
		{
			isOpenNow = true;
			pRole->Get<SpActivityRecord>()->DeleteOldSpAct(m_actid);
		}
		else if (pDataBase->GetActStage() != m_stage)
		{
			pDataBase->ResetTask();
		}
	}

	OpenSpActivitySystem(pRole, conf->systemid);

	///> 未开启活动 or 登录 才会更新任务
	if (isOpenNow || pRole->GetFlag(RSF_ISWaitLogin) || updateTask)
	{
		pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, NULL);
	}

	if (isOpenNow)
	{
		SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
	}
}

KKSG::ErrorCode SpActivityBase::GetReward(Role* pRole, UINT32 taskid)
{
	const SpActivityTaskConf* taskConf = SpActivityConfig::Instance()->GetTaskConf(taskid);
	if (taskConf == NULL)
	{
		LogWarn("can't find SpActivityTaskConf, taskid:%u", taskid);
		return KKSG::ERR_UNKNOWN;
	}
	return pRole->Get<SpActivityRecord>()->GetReward(m_actid, taskid, taskConf->items, ItemFlow_SpActivity_GetReward);
}

void SpActivityBase::OpenSpActivitySystem(Role* pRole, UINT32 systemid)
{
	if (!pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(systemid))
	{
		pRole->Get<CRoleSystem>()->SetSystemOpened(systemid);
		if (!pRole->GetFlag(RSF_ISWaitLogin))
		{
			std::vector<UINT32> openSystem;
			openSystem.push_back(systemid);
			OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &openSystem, NULL);
		}
	}
}

void SpActivityBase::CloseSpActivitySystem(Role* pRole, UINT32 systemid)
{
	if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(systemid))
	{
		pRole->Get<CRoleSystem>()->SetSystemOpened(systemid, false);
		if (!pRole->GetFlag(RSF_ISWaitLogin))
		{
			std::vector<UINT32> closeSystem;
			closeSystem.push_back(systemid);
			OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &closeSystem);
		}
	}
}

void SpActivityBase::StartOnlinePlayer(const std::vector<SpActivityTaskConf*>& task)
{
	const SpActivityTimeConf* conf = SpActivityConfig::Instance()->GetTimeConf(m_actid);
	if (conf == NULL)
	{
		return;
	}

	std::vector<UINT32> openids;
	openids.push_back(conf->systemid);
	PtcG2C_SpActivityOffsetDayNtf oOffsetPtc;
	oOffsetPtc.m_Data.add_actid(m_actid);
	oOffsetPtc.m_Data.add_offsetday(0);
	oOffsetPtc.m_Data.add_offsettime(0);
	
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* pRole = *i;
		if (CheckOpenCondition(pRole))
		{
			if (!pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(conf->systemid))
			{
				pRole->Get<CRoleSystem>()->SetSystemOpened(conf->systemid);
				OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &openids, NULL);
			}
			pRole->Get<SpActivityRecord>()->AddOrUpdateSpActivityTask(m_actid, m_startTime, m_stage, &task);
			pRole->Send(oOffsetPtc); //may happen different actid ---> same systemid

			SpActivityHandler::GlobalHandler.CompleteTaskAfterStartSpActivity(pRole, m_actid);
		}
	}
}