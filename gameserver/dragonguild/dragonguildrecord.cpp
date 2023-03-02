#include "pch.h"
#include "dragonguildrecord.h"
#include "util/gametime.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "table/globalconfig.h"
#include "unit/role.h"
#include "define/systemiddef.h"
#include "gamelogic/bag.h"
#include "table/globalconfig.h"
#include "gamelogic/designationMgr.h"
#include "util/XCommon.h"
#include "timeutil.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/rpcg2m_checkdragonguildredpointg2m.h"

#include "unit/rolemanager.h"
DragonGuildRecord::DragonGuildRecord(Role *pRole)
	:m_role(pRole)
	,m_takedChest(0)
	,m_lastUpdateTime(0)
	,m_hasChest(false)
	,m_taskRefreshTime(0)
	,m_iReceiveCount(0)
{
	m_iReceiveCount = GetGlobalConfig().DragonGuildWeekReciveCount;


	m_tasksReceived.clear();
	m_achiveReceived.clear();
	
	CheckAndPushNewTasks();
	CheckAndPushNewAchivement();

	UpdateTaskRefreshTime();
}

DragonGuildRecord::~DragonGuildRecord()
{

}

bool DragonGuildRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::DragonGuildRecordData& data = poRoleAllInfo->dragonguild();
	m_takedChest = data.taked_chest();
	m_hasChest = data.chest_redpoint();
	m_lastUpdateTime = data.last_update_time();
	m_iReceiveCount = data.receivecount();

	for(UINT32 i =0; i<data.tasks_size(); ++i)
	{
		m_tasksReceived[data.tasks(i).taskid()] = data.tasks(i).received();
	}
	for(UINT32 i =0; i<data.achivements_size(); ++i)
	{
		m_achiveReceived[data.achivements(i).taskid()] = data.achivements(i).received();
	}
	m_taskRefreshTime = data.taskrefreshtime();
	OnDayPass();
	return true;

}

void DragonGuildRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_dirty.TestAndReset())
	{
		return;
	}
	KKSG::DragonGuildRecordData& data = *poRoleAllInfo->mutable_dragonguild();
	data.set_taked_chest(m_takedChest);
	data.set_chest_redpoint(m_hasChest);
	data.set_last_update_time(m_lastUpdateTime);
	data.set_receivecount(m_iReceiveCount);
	data.clear_tasks();
	for (auto it = m_tasksReceived.begin(); it != m_tasksReceived.end(); ++it)
	{
		KKSG::DragonGuildRoleTaskItem item;
		item.set_taskid(it->first);
		item.set_received(it->second);
		data.add_tasks()->CopyFrom(item);
	}
	data.clear_achivements();
	for (auto it = m_achiveReceived.begin(); it != m_achiveReceived.end(); ++it)
	{
		KKSG::DragonGuildRoleTaskItem item;
		item.set_taskid(it->first);
		item.set_received(it->second);
		data.add_achivements()->CopyFrom(item);
	}
	data.set_taskrefreshtime(m_taskRefreshTime);
	
	roChanged.insert(&data);
}

void DragonGuildRecord::OnDayPass()
{
	UINT32 now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_lastUpdateTime, now))
	{
		return;
	}

	m_lastUpdateTime = now;
	m_takedChest = 0;
	m_hasChest = false;

	CheckAndPushNewAchivement();

	UINT32 cycleTime = GetGlobalConfig().DragonGuildTaskRefreshCycleTime;
	//到达下一刷新周期
	if( now >= m_taskRefreshTime )
	{
		UpdateTaskRefreshTime();
		OnTaskRefresh();
	}
	//现在时间加一个周期还没达到刷新时间，说明改过时间，纠正回来...
	if( (now + (cycleTime * 3600 * 24)) < m_taskRefreshTime ) 
	{
		UpdateTaskRefreshTime();
		OnTaskRefresh();
	}

	m_dirty.Set();
}

void DragonGuildRecord::OnTaskRefresh()
{
	//每周重置领取次数
	m_iReceiveCount = m_iReceiveCount = GetGlobalConfig().DragonGuildWeekReciveCount;
	m_tasksReceived.clear();
	CheckAndPushNewTasks();
}

void DragonGuildRecord::UpdateTaskRefreshTime( )
{
	Sequence<UINT32, 3>date = GetGlobalConfig().DragonGuildTaskRefreshStartDate;
	UINT32 year = date[0], month = date[1], day = date[2];
	UINT32 openSec = XCommon::MakeTimeNew(year, month, day, 5);
	UINT32 timeNow = GameTime::GetTime();
	UINT32 cycleTime = GetGlobalConfig().DragonGuildTaskRefreshCycleTime;
	UINT32 d = (timeNow - openSec)/(cycleTime * 3600 * 24);
	m_taskRefreshTime = openSec + (d + 1) * cycleTime * 3600*24;
	m_dirty.Set();
}

void DragonGuildRecord::CheckAndPushNewTasks()
{
	std::vector<UINT32>taskList;
	taskList.clear();
	bool added = false;
	DragonGuildTableMgr::Instance()->GetTaskList(taskList);
	for(UINT32 i=0; i<taskList.size(); ++i)
	{
		UINT32 id = taskList[i];
		if(m_tasksReceived.find(id) == m_tasksReceived.end())
		{
			m_tasksReceived[id] = false;
			added = true;
		}
	}
	if(added) 
		m_dirty.Set();
}

void DragonGuildRecord::CheckAndPushNewAchivement()
{
	std::vector<UINT32>achiveList;
	achiveList.clear();
	bool added = false;
	DragonGuildTableMgr::Instance()->GetAchiveList(achiveList);
	for(UINT32 i=0; i<achiveList.size(); ++i)
	{
		UINT32 id = achiveList[i];
		if(m_achiveReceived.find(id) == m_achiveReceived.end())
		{
			m_achiveReceived[id] = false;
			added = true;
		}
	}
	if(added) 
		m_dirty.Set();
}

KKSG::ErrorCode DragonGuildRecord::GetTaskStatus(KKSG::DragonGuildTaskType taskType, UINT32 taskID)
{
	if(taskType == KKSG::TASK_NORMAL)
	{
		 //领取次数用完
		if(m_iReceiveCount == 0)                       
			return KKSG::ERR_TEAM_TOWER_DAYCOUNT;
		if(m_tasksReceived.find(taskID) == m_tasksReceived.end())
			return KKSG::ERR_FAILED;
		//领取过了
		if(m_tasksReceived[taskID] == true)
			return KKSG::ERR_REWARD_TAKEN;
		return KKSG::ERR_SUCCESS;
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		if(m_achiveReceived.find(taskID) == m_achiveReceived.end())
			return KKSG::ERR_FAILED;
		if(m_achiveReceived[taskID] == true)
			return KKSG::ERR_REWARD_TAKEN;
		return KKSG::ERR_SUCCESS;
	}
	return KKSG::ERR_FAILED;
}

void  DragonGuildRecord::ReceiveOneTaskChest(KKSG::DragonGuildTaskType taskType, UINT32 taskID)
{
	if(taskType == KKSG::TASK_NORMAL)
	{
		if(m_iReceiveCount > 0)
			m_iReceiveCount = m_iReceiveCount - 1;
		m_tasksReceived[taskID] = true;
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		m_achiveReceived[taskID] = true;
	} 	
	m_dirty.Set();
}

bool DragonGuildRecord::IsChestTaked(UINT32 index)
{
	if (index >= 32)
	{
		LogWarn("chest index[%u] invalid", index);
		return false;
	}
	return (m_takedChest & (1 << index)) > 0 ? true : false;
}

void DragonGuildRecord::SetChestTaked(UINT32 index)
{
	if (index >= 32)
	{
		LogWarn("chest index[%u] invalid", index);
		return;
	}
	m_takedChest |= (1 << index);
	m_dirty.Set();
}

bool DragonGuildRecord::HasCheskCanTake(UINT32 level, UINT32 liveness)
{
	if (DragonGuildSimpleMgr::Instance()->GetDragonGuildId(m_role->GetID()) == 0)
	{
		return false;
	}
	std::vector<UINT32> all;
	DragonGuildTableMgr::Instance()->GetCanTakeChestIndex(level, liveness, all);
	for (auto it = all.begin(); it != all.end(); ++it)
	{
		if (!IsChestTaked(*it))
		{
			return true;
		}
	}
	return false;
}

void DragonGuildRecord::CheckChest(UINT32 partLevel, UINT32 liveness)
{
	bool old = m_hasChest;
	m_hasChest = HasCheskCanTake(partLevel, liveness);
	if(m_hasChest)
	{
		m_role->HintNotify(SYS_DRAGONGUILD_LIVENESS, false);
		m_dirty.Set();
	}
	/*
	if (old == false && m_hasChest == true)
	{
		// 推送红点
		m_role->HintNotify(SYS_DRAGONGUILD_LIVENESS, false);
		m_dirty.Set();
	}
	if (old == true && m_hasChest == false)
	{
		// 推送红点消失
		m_role->HintNotify(SYS_DRAGONGUILD_LIVENESS, true);
		m_dirty.Set();
	}
	*/
}

void DragonGuildRecord::OnLogin()
{
	//登录检查一次任务和活跃宝箱红点
	CheckRedPoint();
} 

void DragonGuildRecord::CheckRedPoint()
{
	UINT64 id = DragonGuildSimpleMgr::Instance()->GetDragonGuildId(m_role->GetID());
	if (id)
	{
		RpcG2M_CheckDragonGuildRedPointG2M* rpc = RpcG2M_CheckDragonGuildRedPointG2M::CreateRpc();
		rpc->m_sessionID = m_role->GetSessionID();
		rpc->m_oArg.set_dragonguildid(id);
		m_role->SendToMS(*rpc);
	}
} 

bool DragonGuildRecord::CheckTaskChest(KKSG::DragonGuildTaskType taskType, UINT32 taskID)
{
	if(taskType == KKSG::TASK_NORMAL)
	{
		if(m_tasksReceived[taskID] == false  && m_iReceiveCount > 0)
		{
			m_role->HintNotify(SYS_DRAGONGUILD_TASK, false);
			return true;
		}
	}
	else if(taskType == KKSG::TASK_ACHIVEMENT)
	{
		if(m_achiveReceived[taskID] == false  )
		{
			m_role->HintNotify(SYS_DRAGONGUILD_TASK, false);
			return true;
		}
	}
	return false;
} 
