#include "pch.h"
#include "dragonguildmgr.h"
#include "commondef.h"
#include "db/mysqlmgr.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "notice/notice.h"
#include "util/gametime.h"
#include "util/timespecificmgr.h"
#include "config.h"
#include "timeutil.h"
#include "util/XCommon.h"
#include "util.h"
#include "global/GlobalConfigDBBuild.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "dragonguild/dragonguildtask.h"
#include "team/teamtranshandler.h"
#include "guild/guildmgr.h"
#include "loghelper/tlogr.h"
#include "event/eventmgr.h"

DragonGuildMgr::DragonGuildMgr():m_curId(0)
	,m_lastPassDayTime(0)
{

}

DragonGuildMgr::~DragonGuildMgr()
{

}

static DragonGuildPermission ToDragonGuildPermission(const std::string &guild)
{
#define ENUM2STR(X) if (guild == #X) return X
	ENUM2STR(DGEM_DISMISS);
	ENUM2STR(DGEM_SET_VICELEADER);
	ENUM2STR(DGEM_SETTINGS);
	ENUM2STR(DGEM_ANNOUNCEMENT);
	ENUM2STR(DGEM_APPROVAL);
	ENUM2STR(DGEM_FIREMEMBER);
#undef  ENUM2STR
	return DGPEM_MAX;
}

bool DragonGuildMgr::Init()
{
	std::stringstream sql;
	sql << "select _id, base, liveness, shop, applications, bindinfo, task from dragonguild;";

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	IMysqlRecordSet* poRes = NULL;
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	if(!poMySqlConn->QueryWithRecord(sql.str().c_str(), sql.str().size(), &poRes))
	{
		LogError("Read table dragonguild failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	m_dragonguilds.clear();

	while (poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 len = 0;
		std::string strData;

		UINT64 dgid;
		if (poRes->GetFieldValue(0, &pVal, len))
		{
			dgid = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed.");
			return false;
		}

		KKSG::DragonGuildSaveData data;
		// base
		if(poRes->GetFieldValue(1, &pVal, len))
		{
			data.mutable_base()->assign(pVal, len);
		}
		else
		{
			LogError("Read 'base' column failed");
			return false;
		}

		if(poRes->GetFieldValue(2, &pVal, len))
		{
			data.mutable_liveness()->assign(pVal, len);
		}
		else
		{
			LogError("Read 'liveness' column failed");
			return false;
		}

		if(poRes->GetFieldValue(3, &pVal, len))
		{
			data.mutable_shop()->assign(pVal, len);
		}
		else
		{
			LogError("Read 'shop' column failed");
			return false;
		}

		if(poRes->GetFieldValue(4, &pVal, len))
		{
			data.mutable_applications()->assign(pVal, len);
		}
		else
		{
			LogError("Read 'applications' column failed");
			return false;
		}

		if(poRes->GetFieldValue(5, &pVal, len))
		{
			data.mutable_bindinfo()->assign(pVal, len);
		}
		else
		{
			LogError("Read 'bindinfo' column failed");
			return false;
		}

		if(poRes->GetFieldValue(6, &pVal, len))
		{
			data.mutable_task()->assign(pVal, len);
		}
		else
		{
			LogError("Read 'task' column failed");
			return false;
		}

		DragonGuild* dguild = new DragonGuild;
		dguild->SetID(dgid);
		dguild->LoadFromDB(data);
		
		AddDragonGuild(dgid, dguild);
		m_sorter.AddDragonGuild(dguild);

		// 检查
		dguild->OnLoaded();
	}
	m_sorter.Sort();

	poRes->Release();
	SSInfo << "Load partner num: " << m_delList.size() << END;

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);

	LogInfo("Read all dragonguild data from db use time: %llu ms", qwTimeUsed);

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&DragonGuildMgr::OnTimer, this));

	auto times = DragonGuildTableMgr::Instance()->GetShopRefreshTime();
	for (auto it = times.begin(); it != times.end(); ++it)
	{
		UINT32 temp = *it;
		UINT32 hour =  temp / 100;
		UINT32 min = temp % 100;
		TimeSpecific time(hour, min, 0);
		CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&DragonGuildMgr::RefreshShop, this));
	}

	if (!InitPermissionConfig())
	{
		LogError("init DragonGuildPermission.txt failed.");
		return false;
	}

	return true;
}

void DragonGuildMgr::Uninit()
{
	for (auto it = m_dragonguilds.begin(); it != m_dragonguilds.end(); ++it)
	{
		delete it->second;
	}
	m_dragonguilds.clear();
}

void DragonGuildMgr::SaveAll2DB()
{
	// 删除
	while (!m_delList.empty())
	{
		UINT64 id = m_delList.pop_front();
		DragonGuild* pDguild = GetDragonGuild(id);
		if (NULL == pDguild)
		{
			continue;
		}
		
		RemoveGuildName(pDguild->GetName());
		pDguild->DoMiss();
		pDguild->DelToDB();
		m_sorter.RemoveDragonGuild(pDguild);

		delete pDguild;
		m_dragonguilds.erase(id);
	}

	// 更新
	while(!m_dirtyList.empty())
	{
		UINT64 id = m_dirtyList.pop_front();
		DragonGuild* pDguild = GetDragonGuild(id);
		if (NULL == pDguild)
		{
			continue;
		}

		pDguild->UpdateToDB();
	}
}

bool DragonGuildMgr::OnTimer()
{
	ClearJoinCount();
	for (auto iter = m_dragonguilds.begin(); iter != m_dragonguilds.end(); ++ iter)
	{
		iter->second->OnDayPass();
	}

	if (XCommon::GetWeekNum() == 1)  // monday
	{
		for (auto iter = m_dragonguilds.begin(); iter != m_dragonguilds.end(); ++ iter)
		{
			iter->second->OnWeekPass();
		}
	}
	return true;
}

void DragonGuildMgr::Update()
{
	// 删除
	while (!m_delList.empty())
	{
		UINT64 id = m_delList.pop_front();
		DragonGuild* pDguild = GetDragonGuild(id);
		if (NULL == pDguild)
		{
			continue;
		}

		RemoveGuildName(pDguild->GetName());
		pDguild->DoMiss();
		pDguild->DelToDB();
		m_sorter.RemoveDragonGuild(pDguild);

		delete pDguild;
		m_dragonguilds.erase(id);
		
	}

	// 更新
	UINT32 i = 0;
	while(!m_dirtyList.empty() && i < 20)
	{
		UINT64 id = m_dirtyList.pop_front();
		DragonGuild* pDguild = GetDragonGuild(id);
		if (NULL == pDguild)
		{
			continue;
		}

		pDguild->UpdateToDB();
		i ++ ;
	}
}

void DragonGuildMgr::DoRoleLogin(CRole* pRole)
{
	if (pRole == NULL)
	{
		SSWarn << "pRole is NULL" << END;
		return ;
	}

	DragonGuild* pDGuild = GetDragonGuildByRole(pRole->GetID());
	if (pDGuild == NULL)
	{
		SSWarn << "pDGuild is NULL and roleId: " << pRole->GetID() << END;
		return ;
	}

	pDGuild->DoRoleLogin(pRole);
}

UINT32 DragonGuildMgr::CanCreateDragonGuild(CRole* pRole, std::string name)
{
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	if (GetDragonGuildIdByRole(pRole->GetID()) != INVAILD_DRAGONGUILD)
	{
		return KKSG::ERR_DG_ALREADY_IN_DG;
	}

	int ret = XCommon::ValidNameCheck(name);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	if (!IsNameNotUsed(name))
	{
		return KKSG::ERR_DG_DGNAME_EXIST;
	}

	return KKSG::ERR_SUCCESS;
}

UINT32 DragonGuildMgr::CreateDragonGuild(CRole* pRole, const std::string name)
{
	if (GetDragonGuildIdByRole(pRole->GetID()) != INVAILD_DRAGONGUILD)
	{
		SSWarn << "role already in dragonguild, roleid: " << pRole->GetID() << END;
		return KKSG::ERR_DG_ALREADY_IN_DG;
	}

	int ret = XCommon::ValidNameCheck(name);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}

	if (!IsNameNotUsed(name))
	{
		return KKSG::ERR_DG_DGNAME_EXIST;
	}

	DragonGuild* pDGuild = new DragonGuild();
	pDGuild->OnCreate(pRole, NewID(), name);

	AddDragonGuild(pDGuild->GetID(), pDGuild);
	pDGuild->AddToDB();

	m_sorter.AddDragonGuild(pDGuild);
	m_sorter.Sort();

	TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = pRole->GetID();
	oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_1;
	pDGuild->DoTxLog(oLog);
	oLog.Do();

	return KKSG::ERR_SUCCESS;
}

UINT32 DragonGuildMgr::JoinDragonGuild(CRole* pRole, UINT32 attr, UINT64 dragonguildId)
{
	if (GetDragonGuildIdByRole(pRole->GetID()) != INVAILD_DRAGONGUILD)
	{
		SSWarn << "role already in dragonguild, roleid: " << pRole->GetID() << END;
		return KKSG::ERR_DG_ALREADY_IN_DG;
	}

	DragonGuild *pDGuild = NULL;
	if (dragonguildId == 0)
	{
		pDGuild = FindQuickJoinDragonGuild(attr);
	}
	else
	{
		pDGuild = GetDragonGuild(dragonguildId);
	}

	if (pDGuild == NULL)
	{
		return KKSG::ERR_DG_DGUILD_NOT_EXIST;
	}

	if(pDGuild->GetDragonGuildApplication().IsApplyFull())
	{
		return KKSG::ERR_DG_APPLY_FULL;
	}

	if (pDGuild->IsFull())
	{
		return KKSG::ERR_DG_FULL;
	}

	if (!CheckJoin(pRole->GetID()))
	{
		return KKSG::ERR_DG_IN_CD;
	}

	return pDGuild->AddApplication(pRole->GetID(), attr);
}

DragonGuild* DragonGuildMgr::FindQuickJoinDragonGuild(UINT32 ppt)
{
	std::vector<DragonGuild *> DragonGuilds = m_sorter.GetSortResult(KKSG::DragonGuildSortByLevel);
	for (UINT32 i = 0; i < DragonGuilds.size(); ++ i)
	{
		if (DragonGuilds[i]->GetNeedApproval() == 0 && 
			DragonGuilds[i]->GetRecuitPPT() <= ppt &&
			!DragonGuilds[i]->IsFull())
		{
			return DragonGuilds[i];
		}
	}
	return NULL;
}

UINT64 DragonGuildMgr::NewID()
{
	//和guildID用一个自增id保证不冲突 for绑定渠道功能和工会用同一接口不冲突
	return CGuildMgr::Instance()->MakeGuildID();
	/*
	// 16位serverid 32位时间戳 16位自增id
	UINT64 temp = MSConfig::Instance()->GetServerID() & 0xFFFF;
	temp <<= 48;
	UINT64 t = TimeUtil::GetTime();
	temp |= ((t & 0xFFFFFFFF) << 16);
	temp |= ((++m_curId) & 0xFFFF);
	return temp;
	*/
}

void DragonGuildMgr::AddDragonGuild(UINT64 id, DragonGuild* dragonguild)
{
	auto it = m_dragonguilds.find(id);
	if (it != m_dragonguilds.end())
	{
		LogError("duplicate dragonguild id[%llu]", id);
		return;
	}
	m_dragonguilds.insert(std::make_pair(id, dragonguild));

	auto iter = m_mapGuildNames.find(dragonguild->GetName());
	if (iter != m_mapGuildNames.end())
	{
		LogError("duplicate dragonguild name[%s]", dragonguild->GetName().c_str());
		return ;
	}
	m_mapGuildNames.insert(std::make_pair(dragonguild->GetName(), dragonguild));
}

void DragonGuildMgr::AddRoleDragonGuild(UINT64 roleId, UINT64 id)
{
	m_sorter.SetChanged(KKSG::DragonGuildSortByMemberCount);
	m_roleId2dragonguildId.insert(std::make_pair(roleId, id));
}

void DragonGuildMgr::RemoveRoleDragonGuild(UINT64 roleId)
{
	m_sorter.SetChanged(KKSG::DragonGuildSortByMemberCount);
	m_roleId2dragonguildId.erase(roleId);
}

DragonGuild* DragonGuildMgr::GetDragonGuild(UINT64 dragonguildId)
{
	auto it = m_dragonguilds.find(dragonguildId);
	if (it == m_dragonguilds.end())
	{
		return NULL;
	}
	return it->second;
}

UINT64 DragonGuildMgr::GetDragonGuildIdByRole(UINT64 roleID)
{
	auto it = m_roleId2dragonguildId.find(roleID);
	if (it == m_roleId2dragonguildId.end())
	{
		return INVAILD_DRAGONGUILD;
	}
	return it->second;
}

DragonGuild* DragonGuildMgr::GetDragonGuildByRole(UINT64 roleID)
{
	UINT64 dgID = GetDragonGuildIdByRole(roleID);
	if (dgID == 0)
	{
		return NULL;
	}
	return GetDragonGuild(dgID);
}

bool DragonGuildMgr::HasDragonGuild(UINT64 roleID)
{
	return GetDragonGuildIdByRole(roleID) != INVAILD_DRAGONGUILD;
}

bool DragonGuildMgr::IsInsameDragonGuild(UINT64 roleID1, UINT64 roleID2)
{
	if (roleID1 == roleID2)
	{
		return false;
	}
	UINT64 dragonguildId1 = GetDragonGuildIdByRole(roleID1);
	if (dragonguildId1 == INVAILD_DRAGONGUILD)
	{
		return false;
	}

	UINT64 dragonguildId2 = GetDragonGuildIdByRole(roleID2);
	if (dragonguildId2 == INVAILD_DRAGONGUILD)
	{
		return false;
	}

	return dragonguildId2 == dragonguildId1;
}

void DragonGuildMgr::OnChangePPT(UINT64 roleId, UINT32 ppt)
{
	DragonGuild* pDguild = GetDragonGuildByRole(roleId);
	if (pDguild == NULL)
	{
		return ;
	}
	pDguild->OnChangePPT(roleId, ppt);
	m_sortByPPT = true;
}

void DragonGuildMgr::SortByPPT()
{
	if (m_sortByPPT)
	{
		m_sorter.SetChanged(KKSG::DragongGuildSortByTotalPPT);
	}
	m_sortByPPT = false;
}

bool DragonGuildMgr::RefreshShop()
{
	for (auto it = m_dragonguilds.begin(); it != m_dragonguilds.end(); ++it)
	{
		it->second->GetDragonGuildShop().RefreshItem();
	}
	return true;
}

void DragonGuildMgr::Dismiss(UINT64 dragonguildId)
{
	SSInfo << "DragonGuild Dismiss, id: " << dragonguildId << END;
	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuild(dragonguildId);
	if (pDGuild == NULL)
	{
		SSInfo << "DragonGuild Not Exists , id: " << dragonguildId << END;
		return ;
	}
	std::vector<UINT64> vec;
	pDGuild->GetDragonGuildMemberIds(vec);
	for (UINT32 i = 0; i < vec.size(); ++ i)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(vec[i]);
		if (role)
		{
			TeamTransHandler trans;
			trans.SynTeamExtraInfoToTs(vec[i]);
		}
		EventMgr::Instance()->AddEvent(vec[i], DESIGNATION_COM_TYPE_DRAGONGUILD, 0, GlobalEvent::EVENT_OP_DEL, pDGuild->GetName());
	}
	m_delList.push_back(dragonguildId);
}

void DragonGuildMgr::RemoveGuildName(const std::string &name)
{
	auto iter = m_mapGuildNames.find(name);
	if (iter!=m_mapGuildNames.end())
	{
		m_mapGuildNames.erase(iter);
	}
}

bool DragonGuildMgr::ModifyName(DragonGuild* pDGuild, const std::string& oldname, const std::string& newname)
{
	auto iter = m_mapGuildNames.find(oldname);
	if (iter != m_mapGuildNames.end())
	{
		m_mapGuildNames.erase(oldname);
		m_mapGuildNames.insert(std::pair<std::string,DragonGuild*>(newname, pDGuild));
		return true;
	}
	return false;
}

bool DragonGuildMgr::InitPermissionConfig()
{
	DragonGuildPermissionTable& permissTable = DragonGuildTableMgr::Instance()->GetDragonGuildPermission();

	for (UINT32 i = 0; i < permissTable.Table.size(); ++ i)
	{
		auto *pRow = permissTable.Table[i];
		DragonGuildPermission pem = ToDragonGuildPermission(pRow->DragonGuildID);
		if (pem == DGPEM_MAX)
		{
			LogWarn("Load DragonGuildPermission.txt With error in line: %d, %s", i, pRow->DragonGuildID.c_str());
			return false;
		}

		m_PERMMATRIX[pem][DGPOS_LEADER]      = !!pRow->DGPOS_LEADER;
		m_PERMMATRIX[pem][DGPOS_VICELEADER]  = !!pRow->DGPOS_VIVELEADER;
		m_PERMMATRIX[pem][DGPOS_MEMBER]      = !!pRow->DGPOS_MEMBER;
	}

	return true;
}

void DragonGuildMgr::PassSceneAndFinishTask(UINT64 dragonguildId, std::vector<UINT64>& roleid, UINT32 enterSceneRoleCnt, UINT32 sceneId, UINT32 noDieCnt, UINT32 timeSpan)
{
	if(roleid.size() < GetGlobalConfig().DragonGuildTaskNeedPartnerCount)
		return;
	auto it = m_dragonguilds.find(dragonguildId);
	if(it == m_dragonguilds.end())
		return;
	DragonGuild* guild = it->second;
	if(!guild)
		return;
	//4人以上通关才算排行榜通关次数
	guild->AddPassSceneCount(sceneId);
	DragonGuildTask& taskinfo = guild->GetDragonGuildTask();
	vector<DragonGuildTaskTable::RowData*> vec = DragonGuildTableMgr::Instance()->GetSceneTaskList(sceneId);
	for(size_t i = 0; i < vec.size(); ++i)
	{
		//正常通关
		if(vec[i]->taskType == KKSG::TASK_PASS_SCENE)
		{
			taskinfo.AddTaskFinishCount(KKSG::TASK_NORMAL, vec[i]->taskID);
		}
		//不死通关
		else if(vec[i]->taskType == KKSG::TASK_PASS_NODIE)
		{
			if(enterSceneRoleCnt == noDieCnt)
				taskinfo.AddTaskFinishCount(KKSG::TASK_NORMAL, vec[i]->taskID);			
		}
		//时间内通关
		else if(vec[i]->taskType == KKSG::TASK_TIME_SPAN)
		{
			if(timeSpan <= vec[i]->value)
				taskinfo.AddTaskFinishCount(KKSG::TASK_NORMAL, vec[i]->taskID);
		}
	}

	//成就类型1-3,6
	vector<DragonGuildAchieveTable::RowData*> vecAchieve = DragonGuildTableMgr::Instance()->GetSceneAcheveList(sceneId);	
	for(size_t i = 0; i < vecAchieve.size(); ++i)
	{
		//正常通关
		if(vecAchieve[i]->Type == KKSG::TASK_PASS_SCENE)
		{
			taskinfo.AddTaskFinishCount(KKSG::TASK_ACHIVEMENT, vecAchieve[i]->ID);
		}
		//不死通关
		else if(vecAchieve[i]->Type == KKSG::TASK_PASS_NODIE)
		{
			if( enterSceneRoleCnt == noDieCnt )
				taskinfo.AddTaskFinishCount(KKSG::TASK_ACHIVEMENT, vecAchieve[i]->ID);			
		}
		//时间内通关
		else if(vecAchieve[i]->Type == KKSG::TASK_TIME_SPAN)
		{
			if(timeSpan <= vecAchieve[i]->value)
				taskinfo.AddTaskFinishCount(KKSG::TASK_ACHIVEMENT, vecAchieve[i]->ID);			
		}
		//不同职业组队通关
		else if(vecAchieve[i]->Type == 6)
		{
			std::map<UINT32,bool>prof_map;
			prof_map.clear();
			bool have_same_profession = false;
			for(size_t j = 0; j< roleid.size(); ++j)
			{
				CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid[j]);
				if (!summary)
				{
					return;
				}
				UINT32 t_profession = summary->GetProfession();
				t_profession = t_profession % 10;
				if(prof_map.find(t_profession) != prof_map.end())
				{
					have_same_profession = true;
					break;
				}
				prof_map[t_profession] = true;
			}
			if(have_same_profession == false)
				taskinfo.AddTaskFinishCount(KKSG::TASK_ACHIVEMENT, vecAchieve[i]->ID);
		}
	}
}
//特殊成就
void DragonGuildMgr::UnLockSpeicalAchieve(UINT64 dragonguildId, std::vector<UINT64>& roleid, UINT32 enterSceneRoleCnt, UINT32 sceneId, UINT32 noDieCnt, UINT32 timeSpan)
{
	auto it = m_dragonguilds.find(dragonguildId);
	if(it == m_dragonguilds.end())
		return;
	DragonGuild* guild = it->second;
	if(!guild)
		return;
	DragonGuildTask& taskinfo = guild->GetDragonGuildTask();

	vector<DragonGuildAchieveTable::RowData*> vecAchieve = DragonGuildTableMgr::Instance()->GetSceneAcheveList(sceneId);
	if(vecAchieve.size() == 0)
		return;
	for(size_t i = 0; i < vecAchieve.size(); ++i)
	{
		//通关人数类型
		if(vecAchieve[i]->Type == KKSG::TASK_PASS_PARTNER_COUNT )
		{
			if( (roleid.size() ==  vecAchieve[i]->value) &&(enterSceneRoleCnt == roleid.size()) )
			{
				bool sameGuild = true;
				//二人通关，三人通关必须是同战队成员
				if(roleid.size() > 1)
				{
					UINT32 preId = -1;
					for(size_t j = 0; j < roleid.size(); ++j)
					{
						UINT32 guildId = GetDragonGuildIdByRole(roleid[j]);
						if( (preId != -1) && (guildId != preId ) )
						{
							sameGuild = false;
							break;
						}
						preId = guildId;
					}
				}
				if(sameGuild)
				{
					taskinfo.AddTaskFinishCount(KKSG::TASK_ACHIVEMENT, vecAchieve[i]->ID);
				}
			}
				
		}
	}
}


void DragonGuildMgr::AddJoinInfo(UINT64 nRoleID)
{
	auto iter = m_mapGuildDoor.find(nRoleID);
	if (iter!=m_mapGuildDoor.end())
	{
		GuildDoor &ref = iter->second;
		ref.nCount++;
		if (ref.nCount>=GetGlobalConfig().DragonGuildEnterLimit[0])
		{
			ref.nNextTime = GameTime::GetTime() + (GetGlobalConfig().DragonGuildEnterLimit[1] * 3600 );
		}
		LogInfo("Role Join Guild nRoleID=%llu nCount=%u nNextTime=%u",nRoleID,ref.nCount,ref.nNextTime);
	}else
	{
		GuildDoor door;
		door.nCount = 1;
		door.nNextTime = 0;
		m_mapGuildDoor.insert(std::pair<UINT64,GuildDoor>(nRoleID,door));
	}
}

void DragonGuildMgr::ClearJoinCount()
{
	for(auto iter = m_mapGuildDoor.begin(); iter!=m_mapGuildDoor.end(); iter++)
	{
		GuildDoor &ref = iter->second;
		ref.nCount = 0;
	}
}

bool DragonGuildMgr::CheckJoin(UINT64 nRoleID)
{
	auto iter = m_mapGuildDoor.find(nRoleID);
	if (iter!=m_mapGuildDoor.end())
	{
		if (GameTime::GetTime()>iter->second.nNextTime)
		{
			return true;
		}else
		{
			return false;
		}
	}
	return true;
}