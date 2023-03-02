#include "pch.h"
#include "OpenSystemMgr.h"
#include "define/itemdef.h"
#include "gamelogic/AchivementActivityManager.h"
#include "unit/rolemanager.h"
#include "util/XCommon.h"
#include "util/timespecificmgr.h"
#include "gamelogic/taskrecord.h"
#include "gamelogic/opensystemhandler.h"
#include "unit/systeminfo.h"
#include "role/ptcg2c_opensystemntf.h"
#include "config.h"
#include "role/ptcg2m_opensystemntfms.h"
#include "network/mslink.h"
#include "servermgr/servercrossdatamgr.h"

INSTANCE_SINGLETON(OpenSystemMgr)

OpenSystemMgr::OpenSystemMgr()
{
	m_loadCnt = 0;
	m_getServerOpenTimeCnt = 0;
}

OpenSystemMgr::~OpenSystemMgr()
{

}

bool OpenSystemMgr::Init()
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return true;
	}

	_LoadIdipOpSystem();

	return LoadFile();
}

void OpenSystemMgr::Uninit()
{
	ClearFile();
}

bool OpenSystemMgr::CheckFile()
{
	OpenSystemTable tmpOpenSystemTable;
	if (!tmpOpenSystemTable.LoadFile("table/SystemList.txt"))
	{
		LogWarn("Load file %s failed!", "table/SystemList.txt");
		return false;
	}

	return true;
}

bool OpenSystemMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_oSystemTable.LoadFile("table/SystemList.txt");

	for (auto i = m_handler.begin(); i != m_handler.end(); ++i)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*i);
	}
	m_handler.clear();
	
	for (UINT32 i = 0; i < m_oSystemTable.Table.size(); ++i)
	{
		auto Row = m_oSystemTable.Table[i];
		if (GetOpenDay(Row) == 0 && Row->TaskID == 0 && Row->PlayerLevel == 0 && Row->IsOpen)
		{
			m_initSystem.push_back(Row->SystemID);
		}
		if (!Row->Reward.empty())
		{
			std::vector<ItemDesc> items;
			for (auto j = Row->Reward.begin(); j != Row->Reward.end(); ++j)
			{
				items.push_back(ItemDesc(j->seq[0], j->seq[1]));
			}
			m_id2reward[Row->SystemID] = items;
		}
	}

	if (m_loadCnt != 0)
	{
		OnGetGameServerOpenTime(true); //默认reload的时候，开服时间已经获得了，如果没有获得，需要开服时间的系统就会无法开启
	}
	m_loadCnt++;
	return true;
}

void OpenSystemMgr::ClearFile()
{
	m_initSystem.clear();
	m_oSystemTable.Clear();
	m_startedSystem.clear();
}

bool OpenSystemMgr::OnTimer(std::vector<OpenSystemConf*>& systemID)
{
	for (UINT32 i = 0; i < systemID.size(); i++)
	{
		m_startedSystem.push_back(systemID[i]);
	}
	std::list<Role*>::iterator Begin = RoleManager::Instance()->Begin();
	std::list<Role*>::iterator End = RoleManager::Instance()->End();
	for (; Begin != End; Begin++)
	{
		std::vector<int> opensystem;
		Role* pRole = *Begin;
		for (UINT32 i = 0; i < systemID.size(); i++)
		{
			if (pRole->GetLevel() >= systemID[i]->PlayerLevel)
			{
				if (systemID[i]->TaskID == 0 || pRole->Get<TaskRecord>()->IsTaskFinish(systemID[i]->TaskID))
				{
					opensystem.push_back(systemID[i]->SystemID);
				}
			}
		}
		if (!opensystem.empty())
		{
			OpenSystemHandler::GlobaHandler.DoOpenSystem(pRole, opensystem);
		}
	}

	return true;
}

void OpenSystemMgr::OnGetGameServerOpenTime(bool force)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}

	if (force == false)
	{
		if (m_getServerOpenTimeCnt != 0)
		{
			return;
		}
		m_getServerOpenTimeCnt++;
	}

	time_t nowTime = time(NULL);
	time_t serverOpenTime = GSConfig::Instance()->GetGameServerOpenTimeStamp();
	time_t startTime = XCommon::GetDayBeginTime(serverOpenTime);

	std::vector<OpenSystemConf*> alreadyOpened;
	std::map<time_t, std::vector<OpenSystemConf*>> registerMap;
	for(UINT32 i = 0; i < m_oSystemTable.Table.size(); i++)
	{
		auto Row = m_oSystemTable.Table[i];
		if (Row->IsOpen == false)
		{
			continue;
		}
		UINT32 OpenDay = GetOpenDay(Row);
		if (0 == OpenDay)
		{
			alreadyOpened.push_back(Row);
			continue;
		}
		time_t openTime = startTime + OpenDay * 24 * 3600;
		if (openTime <= nowTime)
		{
			alreadyOpened.push_back(Row);
		}
		else
		{
			registerMap[openTime].push_back(Row);
		}
	}

	for (auto i = registerMap.begin(); i != registerMap.end(); i++)
	{
		TimeSpecific start(i->first);
		m_handler.insert(CTimeSpecificMgr::Instance()->RegisterEvent(start, std::bind(&OpenSystemMgr::OnTimer, this, i->second)));
	}

	OnTimer(alreadyOpened);
}

void OpenSystemMgr::OnRoleFirstEnterScene(Role* pRole)
{
	std::vector<int> opensystem;
	for(UINT32 i = 0; i < m_startedSystem.size(); i++)
	{
		auto Row = m_startedSystem[i];
		if(pRole->GetLevel() >= Row->PlayerLevel)
		{
			if(Row->TaskID == 0 || pRole->Get<TaskRecord>()->IsTaskFinish(Row->TaskID))
			{
				opensystem.push_back(Row->SystemID);
			}
		}
	}
	OpenSystemHandler::GlobaHandler.DoOpenSystem(pRole, opensystem);
}

void OpenSystemMgr::CheckOpenSystem(int level, int taskid, std::vector<int>& systemids)
{
	for (UINT32 i = 0; i < m_startedSystem.size(); ++i)
	{
		auto Row = m_startedSystem[i];
		if (level >= Row->PlayerLevel)
		{
			if (Row->TaskID == 0 ||
				Row->TaskID == taskid)
			{
				systemids.push_back(Row->SystemID);
			}
		}
	}
}

int OpenSystemMgr::GetPlayerLevelBySystemID(int systemID)
{
	OpenSystemTable::RowData* data = m_oSystemTable.GetBySystemID(systemID);
	if(NULL == data)
	{
		return 0;
	}
	return data->PlayerLevel;
}

const std::vector<ItemDesc>* OpenSystemMgr::GetReward(UINT32 id)
{
	auto i = m_id2reward.find(id);
	if (i == m_id2reward.end())
	{
		return NULL;
	}
	else
	{
		return &i->second;
	}
}

bool OpenSystemMgr::IsClosedSystem(UINT32 systemID, UINT64 roleID)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return ServerCrossDataMgr::Instance()->IsIdipClosed(roleID, systemID);
	}
	else
	{
		return IsClosedSystem(systemID);
	}
}

bool OpenSystemMgr::IsClosedSystem(UINT32 systemID)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		SSError << " cross gs should not run here " << END;
	}
	return _IsIdipCloseSystem(systemID);
}

void OpenSystemMgr::_LoadIdipOpSystem()
{
	std::string closeInfo = GSConfig::Instance()->ReadFileContent("serveronly/idipclosesystem.txt");
	std::vector<std::string> Lines = Split(closeInfo, "\r\n");
	for (UINT32 i = 0; i < Lines.size(); ++i)
	{
		std::string temp = Trim(Lines[i]);
		if (!temp.empty())
		{
			m_idipSystemClose.insert(convert<UINT32>(temp.c_str()));
			LogDebug("load idip close system:%u", *m_idipSystemClose.rbegin());
		}
	}
}

void OpenSystemMgr::_GetClosedSystem(std::vector<UINT32>& closeSys)
{
	for (auto iter = m_idipSystemClose.begin(); iter != m_idipSystemClose.end(); ++iter)
	{
		closeSys.push_back(*iter);
	}
}

void OpenSystemMgr::IdipOpSystem(UINT32 systemID, bool opOpen)
{
	std::vector<UINT32> openVec;
	std::vector<UINT32> closeVec;

	if (opOpen)
	{
		m_idipSystemClose.erase(systemID);
		openVec.push_back(systemID);
	}
	else
	{
		m_idipSystemClose.insert(systemID);
		closeVec.push_back(systemID);
	}

	for(auto iter = RoleManager::Instance()->Begin(); iter != RoleManager::Instance()->End(); ++iter)
	{
		Role* role= *iter;
		if (NULL != role)
		{
			NotifyClientSystemOpen(role, &openVec, &closeVec);
		}
	}

	LogInfo("idip %s system:%u", opOpen ? "open" : "close", systemID);
}

bool OpenSystemMgr::_IsIdipCloseSystem(UINT32 systemID)
{
	auto find = m_idipSystemClose.find(systemID);
	if (find != m_idipSystemClose.end())
	{
		return true;
	}

	return false;
}

void OpenSystemMgr::ExceptCloseSystem(Role* role, KKSG::RoleAllInfo* info)
{
	if (!role || !info || !info->has_system())
	{
		return;
	}

	if (m_idipSystemClose.empty() && GSConfig::Instance()->GetAppType() == KKSG::GAME_APP_QQ)
	{
		return;
	}

	auto* systemInfo = role->Get<CRoleSystem>();
	BitSet<MAX_SYSTEM_ID> newInfo;
	for (int i = 0; i < MAX_SYSTEM_ID; ++i)
	{
		if (systemInfo->IsSystemOpened(i))
		{
			newInfo.Set(i, true);
		}
	}
	info->mutable_system()->mutable_system()->assign(newInfo.ToString());
}

void OpenSystemMgr::NotifyClientSystemOpen(Role* role, std::vector<UINT32>* openids, std::vector<UINT32>* closeids)
{
	if (!role)
	{
		return;
	}
	if (!openids && !closeids)
	{
		return;
	}
	bool openEmpty = false;
	if (!openids || (openids && openids->empty()))
	{
		openEmpty = true;
	}
	bool closeEmpty = false;
	if (!closeids || (closeids && closeids->empty()))
	{
		closeEmpty = true;
	}
	if (openEmpty && closeEmpty)
	{
		return;
	}

	PtcG2C_OpenSystemNtf SysPtc;
	if (openids)
	{
		for (int i = 0; i < openids->size(); ++i)
		{
			if (!role->Get<CRoleSystem>()->IsSystemOpened((*openids)[i]))
			{
				continue;
			}
			//if (IsIdipCloseSystem((*openids)[i]))
			//{
			//	continue;
			//}
			SysPtc.m_Data.add_sysids((*openids)[i]);
		}
	}
	if (closeids)
	{
		for (int i = 0; i < closeids->size(); ++i)
		{
			SysPtc.m_Data.add_closesysids((*closeids)[i]);
		}
	}
	if (SysPtc.m_Data.sysids_size() || SysPtc.m_Data.closesysids_size())
	{
		role->Send(SysPtc);
	}
}

void OpenSystemMgr::NotifyMsSystemOpen(Role* role, std::vector<UINT32>* openids)
{
	if (!role || !openids)
	{
		return;
	}

	PtcG2M_OpenSystemNtfMs ntfms;
	ntfms.m_Data.set_roleid(role->GetID());
	for (int i = 0; i < openids->size(); ++i)
	{
		if (!role->Get<CRoleSystem>()->IsSystemOpened((*openids)[i]))
		{
			continue;
		}
		ntfms.m_Data.mutable_systems()->add_sysids((*openids)[i]);
	}
	if (ntfms.m_Data.mutable_systems()->sysids_size() > 0)
	{
		MSLink::Instance()->SendTo(ntfms);
	}
}

void OpenSystemMgr::OnHallEnterScene(Role* role)
{
	std::vector<UINT32> closedSys;
	_GetClosedSystem(closedSys);

	NotifyClientSystemOpen(role, NULL, &closedSys);
}

UINT32 OpenSystemMgr::GetOpenDay(OpenSystemTable::RowData* pConf)
{
	if (GSConfig::Instance()->IsBackFlowServer())
	{
		size_t n = pConf->BackServerOpenDay.size();
		if(0 == n)
		{
			return 0;
		}
		UINT32 level = GSConfig::Instance()->GetBackFlowLevel();
		for (size_t i = 0; i < n; ++i)
		{
			if (level <= pConf->BackServerOpenDay[i][0])	
			{
				return pConf->BackServerOpenDay[i][1];
			}
		}
		return pConf->BackServerOpenDay[n-1][1];
	}
	return pConf->OpenDay;
}





	
