#include "pch.h"
#include "garden/garden_manager.h"
#include "foreach.h"
#include "db/mysqlmgr.h"
#include "util/timespecificmgr.h"
#include "role/rolemanager.h"
#include "global/GlobalConfigDBBuild.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "garden/rpcm2d_querygardeninfo.h"
#include "network/dblink.h"

#define GARDEN_SAVA_INTERVAL	30
#define GARDEN_TIMEUPDATE  GARDEN_SAVA_INTERVAL*1000
#define GARDEN_SAVA_CYCLE	2
#define GARDEN_COMMIT_COUNT	200

INSTANCE_SINGLETON(GardenManager);

GardenManager::GardenManager()
{
	all_garden_.clear();
	all_guild_garden_.clear();

	//TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	//CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&GardenManager::Refresh, this));
}

GardenManager::~GardenManager()
{
}

bool GardenManager::Init()
{
	// load DB
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::stringstream sql;	
	sql<< "select roleid,rolename,plant, cooking,fishing,banquet,eventlog from garden";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.str().c_str(), sql.str().size(), &poRes))
	{
		LogError("Read table garden failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	time_t now = GameTime::GetTime();
	while(poRes->FetchRow())
	{
		KKSG::GardenInfo garden_info;
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		std::string strdata;
		// roleid		
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			garden_info.set_role_id(convert<UINT64>(pVal));
		}
		else
		{
			LogError("Read 'roleid' column failed");
			return false;
		}

		// 失效用户		
		if (CRoleSummaryMgr::Instance()->IsExpire(garden_info.role_id(), GetGlobalConfig().RolesummaryExpireTime))		
		{
			LogDebug("garden id : [%llu]  not load",garden_info.role_id());
			unload_roles_.insert(garden_info.role_id());
			continue;
		}
		LogDebug("garden id : [%llu]   load",garden_info.role_id());
		// name
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			strdata.clear();
			strdata.assign(pVal, dwLen);
			garden_info.set_role_name(strdata);
		}
		else
		{
			LogError("Read 'rolename' column failed");
			return false;
		}

		// plant
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			strdata.clear();
			strdata.assign(pVal, dwLen);
			garden_info.set_plant(strdata);
		}
		else
		{
			LogError("Read 'plant' column failed");
			return false;
		}
		// cooking
		if(poRes->GetFieldValue(3, &pVal, dwLen))
		{
			strdata.clear();
			strdata.assign(pVal, dwLen);
			garden_info.set_cooking(strdata);
		}
		else
		{
			LogError("Read 'cooking' column failed");
			return false;
		}
		// fishing
		if(poRes->GetFieldValue(4, &pVal, dwLen))
		{
			strdata.clear();
			strdata.assign(pVal, dwLen);
			garden_info.set_fishing(strdata);
		}
		else
		{
			LogError("Read 'fishing' column failed");
			return false;
		}
		// banquet
		if(poRes->GetFieldValue(5, &pVal, dwLen))
		{
			strdata.clear();
			strdata.assign(pVal, dwLen);
			garden_info.set_banquet(strdata);
		}
		else
		{
			LogError("Read 'banquet' column failed");
			return false;
		}

		// eventlog
		if(poRes->GetFieldValue(6, &pVal, dwLen))
		{
			strdata.clear();
			strdata.assign(pVal, dwLen);
			garden_info.set_eventlog(strdata);
		}
		else
		{
			LogError("Read 'eventlog' column failed");
			return false;
		}

		// 加载到garden中
		LoadFromDB(garden_info);
	}

	poRes->Release();

	garden_timehandle_ = CTimerMgr::Instance()->SetTimer(this ,0,GARDEN_TIMEUPDATE, -1, __FILE__, __LINE__);
	garden_dbfreshtime_ = 0;

	return true;
}

void GardenManager::Uninit()
{
	if (INVALID_HTIMER != garden_timehandle_)
	{
		CTimerMgr::Instance()->KillTimer(garden_timehandle_);
		garden_timehandle_ = INVALID_HTIMER;
	}
	auto it = all_garden_.begin();
	auto it_end = all_garden_.end();
	for (; it != it_end; ++it)
	{
		it->second->SaveToDB();
		delete it->second;
	}
}

void GardenManager::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	if (++garden_dbfreshtime_ > GARDEN_SAVA_CYCLE - 1 )
	{
		garden_dbfreshtime_ = 0;

		auto it =  all_garden_.begin();
		auto it_end = all_garden_.end();
		for (; it != it_end; ++it)
		{
			it->second->OnTimer();		
		}

		auto guildit = all_guild_garden_.begin();
		auto guildit_end = all_guild_garden_.end();
		for (; guildit != guildit_end; ++guildit)
		{
			guildit->second->GetGuildPlant()->OnTimer();
		}
	}

	UINT32 count = 0;
	while (!queue_changes_.empty())
	{
		UINT64 roleid = queue_changes_.front();
		queue_changes_.pop();

		auto j = changes_.find(roleid);
		if (j != changes_.end())
		{
			changes_.erase(j);
		}

		auto it =  all_garden_.find(roleid);
		if (it == all_garden_.end())
		{
			continue;
		}

		it->second->SaveToDB();

		if (++count >= GARDEN_COMMIT_COUNT)
		{
			break;
		}
	}

	LogInfo("garden_dbfreshtime_:%d,and queue_changes_ size is %d",garden_dbfreshtime_,queue_changes_.size());
}

void GardenManager::SaveAllToDB()
{
	auto it =  all_garden_.begin();
	auto it_end = all_garden_.end();
	for (; it != it_end; ++it)
	{
		it->second->SaveToDB();	
	}
}
void GardenManager::LoadFromDB(const KKSG::GardenInfo& garden_info)
{
	Garden* garden = new Garden(garden_info.role_id(),garden_info.role_name());
	garden->LoadFromDB(garden_info);

	all_garden_.insert(std::make_pair(garden_info.role_id(),garden));
}

//bool GardenManager::Refresh()
//{
//	//// 检查逻辑变化
//	//auto it =  all_garden_.begin();
//	//auto it_end = all_garden_.end();
//	//for (; it != it_end; ++it)
//	//{
//	//	it->second->Refresh();
//	//}
//	//// 更新刷新时间
//	//CGlobalConfigDBBuild::Instance()->SetConfigValue(eGadenUpdateTime,ToString(GameTime::GetTime()));
//	//CGlobalConfigDBBuild::Instance()->UpdateToDb(eGadenUpdateTime);
//
//	return true;
//}

Garden* GardenManager::GetGarden(
	const UINT64 role_id,
	const std::string& role_name)
{
	Garden* garden = NULL;
	auto it = all_garden_.find(role_id);
	if (it != all_garden_.end())
	{
		garden = it->second;
		garden->Refresh();
	}
	else
	{
		auto garden_it = unload_roles_.find(role_id);
		if (garden_it != unload_roles_.end())
		{
			// 异步加载
			RpcM2D_QueryGardenInfo *rpc = RpcM2D_QueryGardenInfo::CreateRpc();
			rpc->m_oArg.set_role_id(role_id);

			DBLink::Instance()->SendTo(*rpc);
			SSInfo<<"query garden from db, role:"<<role_id<<END;
			return NULL;
		}
		else
		{
			garden = new Garden(role_id,role_name);
			// 个人的家园需要插入记录
			garden->InsertTODB();
			all_garden_.insert(std::make_pair(role_id,garden));

			garden->Init();
		}
	}

	return garden;
}


GuildGarden* GardenManager::GuildGetGarden(const UINT64 garden_id)
{
	auto it= all_guild_garden_.find(garden_id);
	auto it_end = all_guild_garden_.end();
	if (it != it_end)
	{
		return it->second;
	}

	return NULL;
}

void GardenManager::AddGuildGarden(const UINT64 garden_id,GuildGarden* guild_garden)
{
	auto it= all_guild_garden_.find(garden_id);
	auto it_end = all_guild_garden_.end();
	if (it == it_end)
	{
		all_guild_garden_.insert(std::make_pair(garden_id,guild_garden));
	}
}

void GardenManager::RemoveGuildGarden(const UINT64 garden_id)
{
	all_guild_garden_.erase(garden_id);
}


// 检查
void GardenManager::CheckGardenNtf(const UINT64 garden_id)
{
	bool hint = false;
	auto it = all_garden_.find(garden_id);
	if (it != all_garden_.end())
	{
		// 获取种植记录		
		const PlantInfoStore& store =  it->second->GetPlantInfoStore();
		if (store.sprite_durations > 0 && store.sprite_id > 0)
		{
			hint = true;
		}
		auto it = store.plantinfo_record.begin();
		auto it_end = store.plantinfo_record.end();
		for (; it != it_end; ++it)
		{
			// 有植物成熟
			if ( KKSG::growMature == it->second->plant_grow_state)
			{
				hint = true;
				break;
			}
		}
	}

	CRole* role = CRoleManager::Instance()->GetByRoleID(garden_id);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, role id:"<<garden_id<<END;
		return;
	}	

	role->HintNotify(GARDEN_SYSTEM_ID,hint);
}


void GardenManager::SetChanged(UINT64 role_id)
{
	auto i = changes_.find(role_id);
	if (i == changes_.end())
	{
		changes_.insert(role_id);
		queue_changes_.push(role_id);
	}
}

void GardenManager::StartUpGardenEvent()
{
	/*std::string szEvent =  CGlobalConfigDBBuild::Instance()->GetConfigValue(eGadenUpdateTime);
	UINT32 nEventTime   =  0; 
	if(!szEvent.empty())
	{
	nEventTime = convert<UINT32>(szEvent);
	}
	UINT32 nTime      = GameTime::GetTime();
	if (!XCommon::IsDaySame(GameTime::GetTime(), nEventTime))
	{
	Refresh();
	}*/
}