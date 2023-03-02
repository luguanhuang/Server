#include "pch.h"
#include "rolepushinforecord.h"
#include "pb/project.pb.h"
#include "util/gametime.h"
#include "table/PushTableConfig.h"
#include "gamelogic/rolesummarymgr.h"

RolePushInfoRecord::RolePushInfoRecord(Role *pRole)
:m_role(pRole)
{
}

RolePushInfoRecord::~RolePushInfoRecord()
{

}

void RolePushInfoRecord::FirstInit(UINT32 roleNum)
{

}

bool RolePushInfoRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	m_infos.clear();
	m_configs.clear();

	const KKSG::RolePushInfo& info = poRoleAllInfo->pushinfo();
	for (INT32 i = 0; i < info.infos_size(); ++i) 
	{
		const KKSG::PushInfo& itemPb = info.infos(i);
		RolePushInfoItem item;
		item.type = itemPb.type();
		item.subType = itemPb.sub_type();
		item.time = itemPb.time();
		m_infos.push_back(item);
	}
	for (INT32 i = 0; i < info.configs_size(); ++i) 
	{
		RolePushConfigItem item;
		item.type = info.configs(i).type();
		item.forbid = info.configs(i).forbid();
		m_configs.push_back(item);
	}
	return true;
}

void RolePushInfoRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_dirty.TestAndReset())
	{
		return;
	}

	KKSG::RolePushInfo& info = *poRoleAllInfo->mutable_pushinfo();
	ToKKSG(info);

	roChanged.insert(&info);
}

void RolePushInfoRecord::ToKKSG(KKSG::RolePushInfo& info)
{
	info.clear_infos();
	info.clear_configs();
	for (auto it = m_infos.begin(); it != m_infos.end(); ++it)
	{
		KKSG::PushInfo* itemPb = info.add_infos();
		RolePushInfoItem& item = *it;
		itemPb->set_type(item.type);
		itemPb->set_sub_type(item.subType);
		itemPb->set_time(item.time);
	}
	for (auto it = m_configs.begin(); it != m_configs.end(); ++it)
	{
		KKSG::PushConfig* item = info.add_configs();
		item->set_type(it->type);
		item->set_forbid(it->forbid);
	}
}

void RolePushInfoRecord::UpdateConfig(const std::string& type, const std::string& value)
{
	if (!PushTableConfig::Instance()->IsConfigKey(type))
	{
		return;
	}
	UINT32 iType = PushTableConfig::Instance()->GetType(type);
	bool forbid = false;
	if (value == "0")
	{
		forbid = true;
	}
	else if (value == "1")
	{
		forbid = false;
	}
	SetForbid(iType, forbid);
}

void RolePushInfoRecord::AddInfo(UINT32 type, UINT32 subType, UINT32 time)
{
	m_dirty.Set();
	UINT32 now = GameTime::GetTime();
	bool found = false;
	for (auto it = m_infos.begin(); it != m_infos.end(); ++it) 
	{
		RolePushInfoItem& item = *it;
		if (item.type == type && item.subType == subType)
		{
			// 同类型推送，推送最近一个
			if (time < item.time || item.time < now)
			{
				item.time = time;
			}
			found = true;
			break;
		}
	}
	if (!found)
	{
		RolePushInfoItem item;
		item.type = type;
		item.subType = subType;
		item.time = time;
		m_infos.push_back(item);
	}
	RoleSummaryMgr::Instance()->UpdateRoleSummary(m_role, SUMMARY_PUSHINFO);
}

void RolePushInfoRecord::SetForbid(UINT32 type, bool forbid)
{
	m_dirty.Set();
	bool found = false;
	for (auto it = m_configs.begin(); it != m_configs.end(); ++it)
	{
		if (it->type == type)
		{
			it->forbid = forbid;
			found = true;
			break;
		}
	}
	if (!found)
	{
		RolePushConfigItem item;
		item.type = type;
		item.forbid = forbid;
		m_configs.push_back(item);
	}
	RoleSummaryMgr::Instance()->UpdateRoleSummary(m_role, SUMMARY_PUSHINFO);
}
