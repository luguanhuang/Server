#include "pch.h"
#include "spactivityconfig.h"
#include "spactivityhandler.h"
#include "spactivitymgr.h"
#include "spactivitydef.h"
#include "spactivityrecord.h"
#include "spactivitydatabase.h"
#include "gamelogic/bag.h"
#include "spactivitybase.h"
#include "unit/role.h"

INSTANCE_SINGLETON(SpActivityConfig)

bool SpActivityConfig::Init()
{
	return LoadFile(false);
}

void SpActivityConfig::Uninit()
{
	ClearFile();
}

bool SpActivityConfig::CheckFile(bool isReload)
{
	SuperActivityTime tmpSpActivityTime;
	if (!tmpSpActivityTime.LoadFile("table/SuperActivityTime.txt"))
	{
		LogWarn("Load SuperActivityTime.txt failed!");
		return false;
	}
	if (tmpSpActivityTime.Table.size() != tmpSpActivityTime.actid_Index.size())
	{
		LogWarn("SuperActivityTime.txt data wrong, has duplicated key");
		return false;
	}
	for (auto i = tmpSpActivityTime.Table.begin(); i != tmpSpActivityTime.Table.end(); ++i)
	{
		if (!SpActivityMgr::IsValidDateType((*i)->datetype))
		{
			LogWarn("unknown datetype [%u]", (*i)->datetype);
			return false;
		}
		if ((*i)->datetype == 2)
		{
			if ((*i)->starttime / 10000000 != 2)
			{
				LogWarn("SuperActivityTime.txt starttime data wrong, actid:%u", (*i)->actid);
				return false;
			}
		}
		if ((*i)->timestage.empty())
		{
			LogWarn("actid [%u], timestage is empty", (*i)->actid);
			return false;
		}
		if ((*i)->starthour < 0 || (*i)->starthour >= 24)
		{
			LogWarn("actid [%u], starthour is invalid, should [0, 24)", (*i)->actid);
			return false;
		}
		if ((*i)->datetype == SpActDateType_EveryWeek)
		{
			if ((*i)->starttime < 1 || (*i)->starttime > 7)
			{
				LogWarn("actid [%u], datetype is 3, starttime [%u] is invalid", (*i)->actid, (*i)->starttime);
				return false;
			}
		}
	}

	SuperActivityTask tmpSpActivityTask;
	if (!tmpSpActivityTask.LoadFile("table/SuperActivityTask.txt"))
	{
		LogWarn("Load SuperActivityTask.txt failed!");
		return false;
	}
	if (tmpSpActivityTask.Table.size() != tmpSpActivityTask.taskid_Index.size())
	{
		LogWarn("SuperActivityTask.txt has duplicated taskid");
		return false;
	}
	for (auto i = tmpSpActivityTask.Table.begin(); i != tmpSpActivityTask.Table.end(); ++i)
	{
		if (tmpSpActivityTime.GetByactid((*i)->actid) == NULL) ///> task表对应的actid在time表里找不到
		{
			LogWarn("taskid:%u 's actid:%u can't find in SuperActivityTime.txt", (*i)->taskid, (*i)->actid);
			return false;
		}
		if ((*i)->tasktype != SpActivityTask_Normal && (*i)->tasktype != SpActivityTask_Father && (*i)->tasktype != SpActivityTask_Son)
		{
			LogWarn("taskid:%u tasktype is invalid", (*i)->taskid);
			return false;
		}
		if ((*i)->tasktype == SpActivityTask_Father)
		{
			if ((*i)->basetask != 0)
			{
				LogWarn("taskid:%u is taskfather, basetask must be 0", (*i)->taskid);
				return false;
			}
		}
		else
		{
			SpActTaskHandler* handler = SpActTaskHandler::GetTaskHandler((*i)->basetask);
			if (handler == NULL)
			{
				LogError("basetask [%u] handler not find", (*i)->basetask);
				return false;
			}
			if (!handler->CheckTaskConfValid(*i))
			{
				LogWarn("taskid:%u num's format is wrong", (*i)->taskid);
				return false;
			}
		}
		if ((*i)->tasktype == SpActivityTask_Father)
		{
			if ((*i)->taskson.empty())
			{
				LogWarn("taskid:%u, father task's taskson size is zero", (*i)->taskid);
				return false;
			}
			for (auto j = (*i)->taskson.begin(); j != (*i)->taskson.end(); ++j)
			{
				const SpActivityTaskConf* conf = tmpSpActivityTask.GetBytaskid(*j);
				if (conf == NULL)
				{
					LogWarn("taskid:%u, it's taskson:%u not exist", (*i)->taskid, *j);
					return false;
				}
				if (conf->tasktype != SpActivityTask_Son)
				{
					LogWarn("taskid:%u, it's taskson:%u, type is not taskson", (*i)->taskid, *j);
					return false;
				}
				if (conf->taskfather != (*i)->taskid)
				{
					LogWarn("taskid:%u, it's taskson:%u, taskfather is wrong", (*i)->taskid, *j);
					return false;
				}
				if (conf->actid != (*i)->actid)
				{
					LogWarn("taskid:%u, it's taskson:%u, not the same actid", (*i)->taskid, *j);
					return false;
				}
			}
		}
		else if ((*i)->tasktype == SpActivityTask_Son)
		{
			const SpActivityTaskConf* conf = tmpSpActivityTask.GetBytaskid((*i)->taskfather);
			if (conf == NULL)
			{
				LogWarn("taskid:%u, it's taskfather:%u not exist", (*i)->taskid, (*i)->taskfather);
				return false;
			}
			if (conf->tasktype != SpActivityTask_Father)
			{
				LogWarn("taskid:%u, it's taskfather:%u, type is not taskfather", (*i)->taskid, (*i)->taskfather);
				return false;
			}
			if (conf->actid != (*i)->actid)
			{
				LogWarn("taskid:%u, it's taskfather:%u, not the same actid", (*i)->taskid, (*i)->taskfather);
				return false;	
			}
			bool isHave = false;
			for (auto j = conf->taskson.begin(); j != conf->taskson.end(); ++j)
			{
				if (*j == (*i)->taskid)
				{
					isHave = true;
					break;
				}
			}
			if (!isHave)
			{
				LogWarn("taskid:%u, it's taskfather:%u, not this taskson", (*i)->taskid, (*i)->taskfather);
				return false;
			}
		}
	}

	BackflowActivity tmpBackFlowAct;
	if (!tmpBackFlowAct.LoadFile("table/BackflowActivity.txt"))
	{
		LogWarn("Load BackflowActivity.txt failed");
		return false;
	}

	if (isReload)
	{
		if (!_CheckReloadTimeTable(tmpSpActivityTime))
		{
			LogWarn("new SuperActivityTime table not valid");
			return false;
		}
	}

	return true;
}

bool SpActivityConfig::LoadFile(bool isReload)
{
	if (!CheckFile(isReload))
	{
		return false;
	}

	ClearFile();

	m_oSpActivityTime.LoadFile("table/SuperActivityTime.txt");
	m_oSpActivityTask.LoadFile("table/SuperActivityTask.txt");

	for (auto i = m_oSpActivityTask.Table.begin(); i != m_oSpActivityTask.Table.end(); ++i)
	{
		m_mapActTask[(*i)->actid].push_back(*i);
	}

	m_oBackFlowActivity.LoadFile("table/BackflowActivity.txt");

	if (isReload)
	{
		if (!SpActivityMgr::Instance()->OnReloadTimeAndTaskTable())
		{
			return false;
		}
	}

	return true;
}

void SpActivityConfig::ClearFile()
{
	m_oSpActivityTime.Clear();
	m_oSpActivityTask.Clear();

	m_mapActTask.clear();

	m_oBackFlowActivity.Clear();
}

void SpActivityConfig::GetRoleHaveTaskConf(Role* pRole, const SpActGetTaskConfArg& arg, std::vector<SpActivityTaskConf*>& outTask, UINT32 actid)
{
	if (pRole == NULL) return;
	const std::map<UINT32, SpActivityDataBase*>& allSpActData = pRole->Get<SpActivityRecord>()->GetAllSpActData();
	if (actid != 0)
	{
		auto iter = allSpActData.find(actid);
		if (iter == allSpActData.end())
		{
			return;
		}
		const std::map<UINT32, SpActTask>& mapTask = iter->second->GetTaskMap();
		for (auto j = mapTask.begin(); j != mapTask.end(); ++j)
		{
			SpActivityTaskConf* conf = m_oSpActivityTask.GetBytaskid(j->first);
			if (conf == NULL)
			{
				LogError("taskconf is NULL, code error, it should be gurantee, role [%llu], actid [%u], taskid [%u]", pRole->GetID(), actid, j->first);
				continue;
			}
			if (IsTaskConfSuitArg(conf, arg))
			{
				outTask.push_back(conf);
			}
		}
		return;
	}
	for (auto i = allSpActData.begin(); i != allSpActData.end(); ++i)
	{
		const std::map<UINT32, SpActTask>& mapTask = i->second->GetTaskMap();
		for (auto j = mapTask.begin(); j != mapTask.end(); ++j)
		{
			SpActivityTaskConf* conf = m_oSpActivityTask.GetBytaskid(j->first);
			if (conf == NULL)
			{
				LogError("taskconf is NULL, code error, it should be gurantee, role [%llu], actid [%u], taskid [%u]", pRole->GetID(), i->first, j->first);
				continue;
			}
			if (IsTaskConfSuitArg(conf, arg))
			{
				outTask.push_back(conf);
			}
		}
	}
}

void SpActivityConfig::GetRoleHaveTaskConf(Role* pRole, const std::vector<SpActGetTaskConfArg>& vecArgs, std::vector<SpActivityTaskConf*>& outTask, UINT32 actid)
{
	if (pRole == NULL) return;
	const std::map<UINT32, SpActivityDataBase*>& allSpActData = pRole->Get<SpActivityRecord>()->GetAllSpActData();
	if (actid != 0)
	{
		auto iter = allSpActData.find(actid);
		if (iter == allSpActData.end())
		{
			return;
		}
		const std::map<UINT32, SpActTask>& mapTask = iter->second->GetTaskMap();
		for (auto j = mapTask.begin(); j != mapTask.end(); ++j)
		{
			SpActivityTaskConf* conf = m_oSpActivityTask.GetBytaskid(j->first);
			if (conf == NULL)
			{
				LogError("taskconf is NULL, code error, it should be gurantee, role [%llu], actid [%u], taskid [%u]", pRole->GetID(), actid, j->first);
				continue;
			}
			for (auto k = vecArgs.begin(); k != vecArgs.end(); ++k)
			{
				if (IsTaskConfSuitArg(conf, *k))
				{
					outTask.push_back(conf);
					break;
				}
			}
		}
		return;
	}
	for (auto i = allSpActData.begin(); i != allSpActData.end(); ++i)
	{
		const std::map<UINT32, SpActTask>& mapTask = i->second->GetTaskMap();
		for (auto j = mapTask.begin(); j != mapTask.end(); ++j)
		{
			SpActivityTaskConf* conf = m_oSpActivityTask.GetBytaskid(j->first);
			if (conf == NULL)
			{
				LogError("taskconf is NULL, code error, it should be gurantee, role [%llu], actid [%u], taskid [%u]", pRole->GetID(), i->first, j->first);
				continue;
			}
			for (auto k = vecArgs.begin(); k != vecArgs.end(); ++k)
			{
				if (IsTaskConfSuitArg(conf, *k))
				{
					outTask.push_back(conf);
					break;
				}
			}
		}
	}
}

bool SpActivityConfig::IsTaskConfSuitArg(SpActivityTaskConf* conf, const SpActGetTaskConfArg& arg)
{
	if (conf == NULL)
	{
		return false;
	}
	if (conf->basetask != arg.baseTask)
	{
		return false;
	}
	if (arg.num1 == -1)
	{
		return true;
	}
	if (conf->num.size() < 1 || conf->num[0] != arg.num1)
	{
		return false;
	}
	return true;
}

void SpActivityConfig::GetAllTaskByActID(UINT32 actid, std::vector<SpActivityTaskConf*>& outTask)
{
	auto iter = m_mapActTask.find(actid);
	if (iter == m_mapActTask.end())
	{
		return;
	}
	outTask = iter->second;
}

void SpActivityConfig::GetTaskConfsByTaskIDs(const std::vector<UINT32>& taskids, std::vector<SpActivityTaskConf*>& confs)
{
	for (auto i = taskids.begin(); i != taskids.end(); ++i)
	{
		SpActivityTaskConf* conf = m_oSpActivityTask.GetBytaskid(*i);
		if (conf == NULL)
		{
			continue;
		}
		confs.push_back(conf);
	}
}

const BackflowActivityConf* SpActivityConfig::GetBackflowActivityConf(UINT32 taskid)
{
	return m_oBackFlowActivity.GetByTaskId(taskid);
}

bool SpActivityConfig::_CheckReloadTimeTable(SuperActivityTime& newTime)
{
	if (!GSConfig::Instance()->IsHallGS())
	{
		return true;
	}
	///> 对于正在进行中的活动，只能修改开启最低等级，不能修改时间
	for (auto i = m_oSpActivityTime.Table.begin(); i != m_oSpActivityTime.Table.end(); ++i)
	{
		const SpActivityTimeConf* conf = *i;
		const SpActivityTimeConf* newConf = newTime.GetByactid(conf->actid);
		if (newConf == NULL)
		{
			LogWarn("actid [%u] erase in new SuperActivityTime table", conf->actid);
			return false;
		}
		SpActivityBase* pBase = SpActivityMgr::Instance()->GetSpActivity(conf->actid);
		if (pBase == NULL)
		{
			LogError("actid [%u] in SuperActivityTime table, but no base in manager, previous reload Init failed??", conf->actid);
			continue;
		}
		if (pBase->GetStage() != 0)
		{
			if (conf->systemid != newConf->systemid
				|| conf->datetype != newConf->datetype
				|| conf->starttime != newConf->starttime
				|| conf->starthour != newConf->starthour
				|| conf->timestage != conf->timestage)
			{
				LogError("actid [%u], already open activity, conf changed", conf->actid);
				return false;
			}
		}
	}
	return true;
}