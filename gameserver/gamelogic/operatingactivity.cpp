#include "pch.h"
#include "unit/role.h"
#include "operatingactivity.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "table/OpenSystemMgr.h"
#include "unit/systeminfo.h"
#include "define/itemdef.h"
#include "bagtransition.h"


INSTANCE_SINGLETON(OperatingActivityMgr);

OperatingActivityMgr::OperatingActivityMgr(){}

OperatingActivityMgr::~OperatingActivityMgr(){}

bool OperatingActivityMgr::Init()
{
	bool bRet = LoadFile();
	if (bRet == false)
		return false;
	return true;
}

void OperatingActivityMgr::Uninit()
{
	ClearFile();
}

void OperatingActivityMgr::ClearFile()
{
	for(auto iter = m_mapHandle.begin();iter!=m_mapHandle.end();iter++)
	{
		if (iter->second.nStartHandle != INVALID_TIMESPECIFIC)
			CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->second.nStartHandle);
		if (iter->second.nEndHandle != INVALID_TIMESPECIFIC)
			CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->second.nEndHandle);
	}
	m_mapHandle.clear();
	m_Table.Clear();
	m_nTime = 0;
}

bool OperatingActivityMgr::LoadFile()
{
	ClearFile();
	bool bRet = m_Table.LoadFile("table/OperatingActivity.txt");
	if (bRet == false)
		return false;
	if(GSConfig::Instance()->IsCrossGS()==true)
		return true;
	time_t curtime = GameTime::GetTime();
	for(int i = 0; i < m_Table.Table.size();i++)
	{
		OperatingActivity::RowData *pRowData = m_Table.Table[i];
		if (pRowData->OpenTime.empty()==false)
		{
			stOperatingBase  base;
			base.nStartTime	= XCommon::ParseTime(pRowData->OpenTime[0]);
			base.nEndTime   = XCommon::ParseTime(pRowData->OpenTime[1]);
			if (curtime < base.nEndTime)
			{
				if (curtime > base.nStartTime)
					base.nStartTime = curtime+30;
				TimeSpecific startts(base.nStartTime);
				base.nStartHandle = CTimeSpecificMgr::Instance()->RegisterEvent(startts, std::bind(&OperatingActivityMgr::OnStart, this, pRowData->SysID));
				TimeSpecific endtss(base.nEndTime);
				base.nEndHandle = CTimeSpecificMgr::Instance()->RegisterEvent(endtss, std::bind(&OperatingActivityMgr::OnClose, this, pRowData->SysID));
				m_mapHandle.insert(std::pair<UINT32,stOperatingBase>(pRowData->SysID,base));
			}
		}
	}
	return true;
}


bool OperatingActivityMgr::OnStart(UINT32 nID)
{
	OperatingActivity::RowData *pRowData =  m_Table.GetBySysID(nID);
	if(pRowData == NULL)
		return true;
	std::vector<UINT32> vecSys;
	vecSys.push_back(nID);
	for (auto iter = RoleManager::Instance()->Begin(); iter != RoleManager::Instance()->End(); iter++)
	{
		Role*pRole = *iter;
		if (pRole->GetLevel() >= pRowData->Level)
		{

			if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(nID) == false)
			{
				pRole->Get<CRoleSystem>()->SetSystemOpened(nID);
				OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &vecSys, NULL);
				pRole->HintNotify(nID, false);
			}			
		}
	}
	return true;
}


bool OperatingActivityMgr::OnClose(UINT32 nID)
{
	OperatingActivity::RowData *pRowData =  m_Table.GetBySysID(nID);
	if(pRowData == NULL)
		return true;
	std::vector<UINT32> vecSys;
	vecSys.push_back(nID);
	for (auto iter = RoleManager::Instance()->Begin(); iter != RoleManager::Instance()->End(); iter++)
	{
		Role*pRole = *iter;
		if (pRole->GetLevel() >= pRowData->Level)
		{
			if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(nID) == true)
			{
				pRole->Get<CRoleSystem>()->SetSystemOpened(nID,false);
				OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &vecSys);
			}
		}
	}
	return true;
}

void OperatingActivityMgr::Close()
{
	for(auto iter = m_mapHandle.begin(); iter!=m_mapHandle.end();iter++)
	{
		OperatingActivity::RowData *pRowData =  m_Table.GetBySysID(iter->first);
		if(pRowData!=NULL)
		{
			for (auto siter = RoleManager::Instance()->Begin(); siter != RoleManager::Instance()->End(); siter++)
			{
				Role*pRole = *siter;
				std::vector<UINT32> vecSys;
				vecSys.push_back(pRowData->SysID);
				if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(iter->first) == true)
				{
					pRole->Get<CRoleSystem>()->SetSystemOpened(iter->first,false);
					OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &vecSys);
				}
			}
		}
	}
}

void OperatingActivityMgr::OnTriger(Role *pRole)
{
	std::vector<UINT32> vecSys;
	UINT32 nTime = GameTime::GetTime();
	for(auto iter = m_mapHandle.begin(); iter!=m_mapHandle.end();iter++)
	{
		OperatingActivity::RowData *pRowData =  m_Table.GetBySysID(iter->first);
		if(pRowData!=NULL)
		{
			if(pRole->GetLevel()>= pRowData->Level)
			{
				if(nTime >= iter->second.nStartTime && nTime < iter->second.nEndTime )
				{
					if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(iter->first) == false)
					{
						vecSys.push_back(iter->first);
						pRole->Get<CRoleSystem>()->SetSystemOpened(iter->first);
					}
				}
			}
		}
	}
	if (vecSys.empty()==false)
	{
		OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &vecSys, NULL);
	}
}

