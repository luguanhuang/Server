#include "pch.h"
#include "holidaystage.h"
#include "levelsealMgr.h"
#include "../share/util/XCommon.h"
#include "../share/util/gametime.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "stagemgr.h"
#include "role/ptcg2c_opensystemntf.h"
#include "util/timespecificmgr.h"
#include "foreach.h"
#include "table/OpenSystemMgr.h"

INSTANCE_SINGLETON(CHolidayStage);

CHolidayStage::CHolidayStage()
{
}

CHolidayStage::~CHolidayStage()
{

}

bool CHolidayStage::Init()
{
	return LoadFile();	
}


void CHolidayStage::Uninit()
{
	auto start_it = start_handler_.begin();
	auto start_it_end = start_handler_.end();
	for (; start_it != start_it_end; ++start_it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(start_it->second);
	}

	auto end_it = end_handler_.begin();
	auto end_it_end = end_handler_.end();
	for (; end_it != end_it_end; ++end_it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(end_it->second);
	}
}


void CHolidayStage::CheckOnline(Role *poRole)
{
	if (poRole == NULL)
	{
		return ;
	}

	if (GetHolidaySceneId() != 0)
	{
		PtcG2C_OpenSystemNtf msg;
		msg.m_Data.add_sysids(SYS_FESTIVAL_ACTIVE);
		poRole->Send(msg);
	}
	else
	{
		PtcG2C_OpenSystemNtf msg;
		msg.m_Data.add_closesysids(SYS_FESTIVAL_ACTIVE);
		poRole->Send(msg);
	}
	return ;
}


bool CHolidayStage::_IsHolidayOpen(UINT32 id)
{
	FestScene::RowData* pData = m_festSceneTable.GetByid(id);
	if (pData == NULL)
	{
		return false;
	}

	time_t beginTime = XCommon::MakeTimeNew(pData->openTime.seq[0], pData->openTime.seq[1], pData->openTime.seq[2],HOUR_ONE_DAY_BEGIN);
	time_t endTime = beginTime + pData->keepTime * 24 * 60 * 60;
	time_t nowTime = GameTime::GetTime();

	if (nowTime >= beginTime && nowTime < endTime)
	{
		return true;
	}

	return false;
}

void CHolidayStage::_CreateEndTime()
{
	time_t nowTime = GameTime::GetTime();
	foreach (i in m_festSceneTable.Table)
	{
		time_t beginTime = XCommon::MakeTimeNew((*i)->openTime.seq[0], (*i)->openTime.seq[1], (*i)->openTime.seq[2],HOUR_ONE_DAY_BEGIN);
		time_t endTime = beginTime + (*i)->keepTime * 24 * 60 * 60;
		if (beginTime > nowTime)
		{
			TimeSpecific spec((UINT32)beginTime);
			UINT32 hander = CTimeSpecificMgr::Instance()->RegisterEvent(spec,std::bind(&CHolidayStage::OnStart,this,(*i)->id));
			start_handler_.insert(std::make_pair((*i)->id,hander));
		}
		if (endTime > nowTime)
		{
			TimeSpecific spec((UINT32)endTime);
			UINT32 hander = CTimeSpecificMgr::Instance()->RegisterEvent(spec, std::bind(&CHolidayStage::OnEnd, this,(*i)->id));			
			end_handler_.insert(std::make_pair((*i)->id,hander));
		}
	}
}


bool CHolidayStage::OnStart(UINT32 active_id)
{
	PtcG2C_OpenSystemNtf msg;
	msg.m_Data.add_sysids(SYS_FESTIVAL_ACTIVE);

	std::vector<UINT32> openids;
	openids.push_back(SYS_FESTIVAL_ACTIVE);
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); i++)
	{
		Role *role = *i;
		if ( NULL != role)
		{
			if (!role->Get<CRoleSystem>()->IsSystemOpenedAtRole(SYS_FESTIVAL_ACTIVE))
			{
				role->Get<CRoleSystem>()->SetSystemOpened(SYS_FESTIVAL_ACTIVE);
				OpenSystemMgr::Instance()->NotifyClientSystemOpen(role, &openids, NULL);	
			}
			role->Send(msg); 
		}
	}
	auto it = start_handler_.find(active_id);
	if ( it != start_handler_.end())
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(it->second);
	}
	return true;
}

bool CHolidayStage::OnEnd(UINT32 active_id)
{
	PtcG2C_OpenSystemNtf msg;
	msg.m_Data.add_closesysids(SYS_FESTIVAL_ACTIVE);

	std::vector<UINT32> closeids;
	closeids.push_back(SYS_FESTIVAL_ACTIVE);
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); i++)
	{
		Role *role = *i;
		if (NULL != role)
		{
			if (role->Get<CRoleSystem>()->IsSystemOpenedAtRole(SYS_FESTIVAL_ACTIVE))
			{
				role->Get<CRoleSystem>()->SetSystemOpened(SYS_FESTIVAL_ACTIVE, false);
				OpenSystemMgr::Instance()->NotifyClientSystemOpen(role, NULL, &closeids);	
			}
		}
	}

	auto it = end_handler_.find(active_id);
	if ( it != end_handler_.end())
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(it->second);
	}

	return true;
}


void CHolidayStage::_ReCheck()
{
	if (!m_sealMap.empty())
	{
		if (!_IsHolidayOpen(m_sealMap.begin()->first))
		{
			m_sealMap.clear();
		}
		else
		{
			return;
		}
	}

	foreach (i in m_festSceneTable.Table)
	{
		if (_IsHolidayOpen((*i)->id))
		{
			m_sealMap[(*i)->id] = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;

			return ;
		}
	}

	return ;
}

UINT32 CHolidayStage::GetHolidaySceneId()
{
	_ReCheck();

	auto iter = m_sealMap.begin();
	if (iter != m_sealMap.end())
	{
		FestScene::RowData* pData = m_festSceneTable.GetByid(iter->first);
		if (pData != NULL)
		{
			foreach (i in pData->SceneData)
			{
				if (i->seq[0] == iter->second)
				{
					return i->seq[1];
				}
			}
		}
	}

	return 0;
}

UINT32 CHolidayStage::GetHolidayId()
{
	_ReCheck();
	if (!m_sealMap.empty())
	{
		return m_sealMap.begin()->first;
	}
	return 0;
}

UINT32 CHolidayStage::GetLimitTimes()
{
	FestScene::RowData* pData = m_festSceneTable.GetByid(GetHolidayId());
	UINT32 sealId = m_sealMap.empty()? 0 : m_sealMap.begin()->second;

	if (pData != NULL)
	{
		foreach (i in pData->SceneData)
		{
			if (i->seq[0] == sealId)
			{
				return i->seq[2];
			}
		}
	}

	return 0;
}

bool CHolidayStage::IsHolidaySecneId(UINT32 sceneId)
{
	foreach (i in m_festSceneTable.Table)
	{
		foreach (j in (*i)->SceneData)
		{
			if (sceneId == j->seq[1])
			{
				return true;
			}
		}
	}
	return false;
}

UINT32 CHolidayStage::GetLostTimes(UINT64 roleId, UINT32 sceneId)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleId);
	if (pRole == NULL)
	{
		return 0;
	}

	if (sceneId != GetHolidaySceneId())
	{
		return 0;
	}

	UINT32 timeLimit = GetLimitTimes();
	UINT32 times = pRole->Get<StageMgr>()->GetHolidayTimes(GetHolidayId());

	return timeLimit > times ? (timeLimit - times) : 0; 
}

UINT32 CHolidayStage::GetHaveTime()
{
	FestScene::RowData* pData = m_festSceneTable.GetByid(GetHolidayId());
	if (pData == NULL)
	{
		return 0;
	}

	time_t nowTime = GameTime::GetTime();
	time_t endTime = XCommon::MakeTimeNew(pData->openTime.seq[0], pData->openTime.seq[1], pData->openTime.seq[2],HOUR_ONE_DAY_BEGIN) +  24 * 60 * 60 * pData->keepTime;
	return (UINT32)(endTime > nowTime ? (endTime - nowTime) : 0);
}

bool CHolidayStage::CheckFile()
{
	FestScene tempTable;

	if (!tempTable.LoadFile("table/festivalscenelist.txt"))
	{
		LogWarn("reload festivalscenelist.txt failed !");
		return false;
	}

	return true;
}

bool CHolidayStage::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	m_festSceneTable.Clear();

	if (!m_festSceneTable.LoadFile("table/festivalscenelist.txt"))
	{
		SSWarn << "reload file table/festivalscenelist.txt failed" << END;
		return false;
	}

	_CreateEndTime();
	return true;
}
