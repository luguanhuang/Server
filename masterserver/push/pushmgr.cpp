#include "pch.h"
#include "pushmgr.h"
#include <time.h>

#include "cmdline.h"
#include "rapidjson/prettywriter.h"
#include "pb/project.pb.h"

#include "httptask/multipushtask.h"
#include "httptask/singlepushtask.h"
#include "pushhandler.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "table/PushMessageTable.h"
#include "httptask/httptaskmgr.h"

INSTANCE_SINGLETON(PushMgr)

static bool CmdPushAll(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() <= 2)
	{
		SSError << "Usage: pushall title content" << END;
		return false;
	}
	PushMgr::Instance()->PushToAll(args[1], args[2]);
	return true;
}

static bool CmdPushRole(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() <= 3)
	{
		SSError << "Usage: pushrole account title message" << END;
		return false;
	}
	PushMgr::Instance()->PushToRole(args[1], args[2], args[3]);
	return true;
}

static bool CmdPushAllTable(const CmdLineArgs &args, std::string &outputMessage)
{
	if (args.size() <= 2)
	{
		SSError << "Usage: pushalltable type subtype" << END;
		return false;
	}
	UINT32 type = convert<UINT32>(args[1]);
	UINT32 subType = convert<UINT32>(args[2]);

	std::vector<PushMessageTableData*> data;
	PushMgr::Instance()->GetTableData(type, subType, data);
	for (auto it = data.begin(); it != data.end(); ++it)
	{
		PushMgr::Instance()->PushToAll(*it);
	}

	return true;
}

PushMgr::PushMgr()
:m_timerId(0)
{
}

PushMgr::~PushMgr()
{
}

bool PushMgr::Init()
{
	CmdLine::RegistCmd("pushall", CmdPushAll);
	CmdLine::RegistCmd("pushrole", CmdPushRole);
	CmdLine::RegistCmd("pushalltable", CmdPushAllTable);


	//m_handlers.insert(std::make_pair(PushHanlder_SpecificTime, new SpecificTimePushHandler));
	m_handlers.insert(std::make_pair(PushHandler_LevelSeal, new LevelSealPushHandler));

	if (!LoadFile())
	{
		return false;
	}
	m_timerId = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	
	return true;
}

void PushMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_timerId);
	ClearFile();
	for (auto it = m_time2PushItem.begin(); it != m_time2PushItem.end(); ++it)
	{
		delete it->second;
	}
	m_time2PushItem.clear();

	for (auto it = m_handlers.begin(); it != m_handlers.end(); it++)
	{
		delete it->second;
	}
	m_handlers.clear();
}

bool PushMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}
	ClearFile();

	if (!m_config.LoadFile("table/PushMessage.txt"))
	{
		SSError << "load PushMessage.txt error" << END;
		return false;
	}

	m_tableData.clear();
	for (auto it = m_config.Table.begin(); it != m_config.Table.end(); ++it)
	{
		PushMessageTableData* row = *it;
		m_tableData[row->Type].push_back(row);
	}

	return true;
}

bool PushMgr::CheckFile()
{
	PushMessageTable table;
	if (!table.LoadFile("table/PushMessage.txt"))
	{
		SSError << "load PushMessage.txt error" << END;
		return false;
	}

	return true;
}

void PushMgr::ClearFile()
{
	m_config.Clear();
}

const std::vector<PushMessageTableData*>& PushMgr::GetAllTableData()
{
	return m_config.Table;
}

void PushMgr::GetTableData(UINT32 type, std::vector<PushMessageTableData*>& data)
{
	auto it = m_tableData.find(type);
	if (it != m_tableData.end())
	{
		data = it->second;
	}
}

void PushMgr::GetTableData(UINT32 type, UINT32 subType, std::vector<PushMessageTableData*>& data)
{	auto it = m_tableData.find(type);
	if (it == m_tableData.end())
	{
		return;
	}
	for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
	{
		if ((*it2)->SubType == subType)
		{
			data.push_back(*it2);
		}
	}
}

void PushMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Update();
}

void PushMgr::Update() 
{
	RolePushUpdate();
	HandlerUpdate();
}

void PushMgr::HandlerUpdate()
{
	for (auto it = m_handlers.begin(); it != m_handlers.end(); it++)
	{
		it->second->Update();
	}
}

void PushMgr::RolePushUpdate()
{
	UINT32 now = (UINT32)::time(NULL);	
	for (auto it = m_time2PushItem.begin(); it != m_time2PushItem.end(); )
	{
		RolePushItem* item = it->second;
		if (item->time > now)
		{
			break;
		}
		// 推送
		std::vector<PushMessageTableData*> data;
		GetTableData(item->type, item->subType, data);
		for (auto it2 = data.begin(); it2 != data.end(); ++it2)
		{
			PushToRole(item->roleId, *it2);
		}

		// 删除
		RemoveRoleTime(item->roleId, item->time);
		delete item;
		m_time2PushItem.erase(it++);
	}
}

void PushMgr::PushToRole(UINT64 roleId, PushMessageTableData* data)
{
	if (data == NULL)
	{
		return;
	}
	PushToRole(roleId, data->Title, data->Content, data->Type);
}

void PushMgr::PushToRole(UINT64 roleId, const std::string& title, const std::string& content, UINT32 type)
{
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (role == NULL)
	{
		SSWarn << "No RoleId: " << roleId << END;	
		return;
	}
	if (!PushFilter(type, role))
	{
		return;
	}
	DoPushTask(role, title, content);
}

void PushMgr::PushToRole(const std::string& account, const std::string& title, const std::string& content, UINT32 type)
{
	CRoleSummary* role = CRoleSummaryMgr::Instance()->GetLastLoginRole(account);
	if (role == NULL)
	{
		SSWarn << "No Role: " << account << END;	
		return;
	}
	if (!PushFilter(type, role))
	{
		return;
	}
	DoPushTask(role, title, content);
}

void PushMgr::PushToAll(PushMessageTableData* data)
{
	PushToAll(data->Title, data->Content, data->Type);
}

void PushMgr::PushToAll(const std::string& title, const std::string& content, UINT32 filterType)
{
	FindRoleFilter filter = std::bind(&PushMgr::PushFilter, this, filterType, std::placeholders::_1);
	// IOS 和 Android 批量推送，对应两个任务
	for (INT32 i = KKSG::PlatType_MIN; i <= KKSG::PlatType_MAX; ++i)
	{
		XinGeMultiPushItem* item = new XinGeMultiPushItem;
		CRoleSummaryMgr::Instance()->GetPushRole(i, filter, item->roles);
		if (item->roles.empty())
		{
			delete item;
			continue;
		}
		item->pushType = PushTask::PushType_Account;
		item->platType = i;
		item->title = title;
		item->content = content;
		m_xinge.AddMultiPush(item);
	}
}

void PushMgr::DoPushTask(CRoleSummary* role, const std::string& title, const std::string& content)
{
	SinglePushTask* task = new SinglePushTask();
	task->SetPushType(PushTask::PushType_Account);
	task->SetPlatType(role->GetPlatType());
	PushMessage& message = task->GetMessage();
	message.token = role->GetAccount();
	message.content = content;	
	message.title = title;

	HttpTaskMgr::Instance()->AddTask(task);
}

void PushMgr::OnRoleLogin(UINT64 roleId)
{
	RemoveRoleAllPushItem(roleId);
}

void PushMgr::OnRoleLogout(UINT64 roleId)
{
	CRoleSummary* roleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (roleSummary == NULL)
	{
		return;
	}
	UpdateRolePushItem(roleSummary, roleSummary->GetData().push_info());
}

void PushMgr::UpdateRolePushItem(CRoleSummary* role, const KKSG::RolePushInfo& info)
{
	UINT64 roleId = role->GetID();
	RemoveRoleAllPushItem(roleId);
	for (INT32 i = 0; i < info.infos_size(); ++i)
	{
		const KKSG::PushInfo& item = info.infos(i);
		if (!role->IsPushTypeForbid(item.type()))
		{
			AddRolePushItem(roleId, item.type(), item.sub_type(), item.time());
		}
	}
}

void PushMgr::AddRolePushItem(UINT64 roleId, UINT32 type, UINT32 subType, UINT32 time)
{
	UINT32 now = (UINT32)::time(NULL);	
	if ((time + 10) < now)
	{
		return;
	}
	RolePushItem* item = new RolePushItem;
	item->roleId = roleId;
	item->type = type;
	item->subType = subType;
	item->time = time;

	m_time2PushItem.insert(std::make_pair(time, item));
	m_roleId2time.insert(std::make_pair(roleId, time));
}

void PushMgr::RemoveRoleAllPushItem(UINT64 roleId)
{
	auto itPairRole = m_roleId2time.equal_range(roleId);
	for (auto it = itPairRole.first; it != itPairRole.second; )
	{
		UINT32 time = it->second;
		auto itPairTime = m_time2PushItem.equal_range(time);
		for (auto itItem = itPairTime.first; itItem != itPairTime.second; )
		{
			RolePushItem* item = itItem->second;
			if (item->roleId == roleId)
			{
				delete item;
				m_time2PushItem.erase(itItem++);
			}
			else
			{
				itItem++;
			}
		}
		m_roleId2time.erase(it++);
	}
}

void PushMgr::RemoveRoleTime(UINT64 roleId, UINT32 time)
{
	auto itPairRole = m_roleId2time.equal_range(roleId);
	for (auto it = itPairRole.first; it != itPairRole.second; )
	{
		if (time == it->second)
		{
			m_roleId2time.erase(it++);
		}
		else 
		{
			it++;
		}
	}
}

void PushMgr::TriggerByLevelSeal()
{
	auto it = m_handlers.find(PushHandler_LevelSeal);
	if (it == m_handlers.end())
	{
		return;
	}
	it->second->Update(true);
}

bool PushMgr::PushFilter(UINT32 type, CRoleSummary* pRoleSummary)
{
	if (CRoleManager::Instance()->IsOnline(pRoleSummary->GetID()))
	{
		return false;
	}
	if (pRoleSummary->IsPushTypeForbid(type))
	{
		return false;
	}
	// TODO 系统开放判断等
	return true;
}