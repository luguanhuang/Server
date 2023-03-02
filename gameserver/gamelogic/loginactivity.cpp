#include "pch.h"
#include "unit/role.h"
#include "globalconfig.h"
#include "loginactivity.h"
#include "util/gametime.h"
#include "define/systemiddef.h"
#include "util/timespecificmgr.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "table/OpenSystemMgr.h"
#include "unit/systeminfo.h"
#include "gamelogic/worldlevelmgr.h"
#include "common/ptcg2m_updateglobalvalue.h"
#include "loginactivity/ptcg2c_loginactivitystatusntf.h"
#include "network/mslink.h"
#include "rolemisc.h"
#include "define/itemdef.h"
#include "bagtransition.h"


INSTANCE_SINGLETON(LoginActivityMgr);

LoginActivityMgr::LoginActivityMgr():m_nTime(0),m_nWorldLevel(0){}

LoginActivityMgr::~LoginActivityMgr(){}

bool LoginActivityMgr::Init()
{
	bool bRet = LoadFile();
	if (bRet == false)
		return false;
	return true;
}

void LoginActivityMgr::Uninit()
{
	ClearFile();
}

void LoginActivityMgr::ClearFile()
{
	ControlActivity(false);
	for(auto iter = m_mapHandle.begin();iter!=m_mapHandle.end();iter++)
	{
		if (iter->first != INVALID_TIMESPECIFIC)
			CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->first);
		if (iter->second != INVALID_TIMESPECIFIC)
			CTimeSpecificMgr::Instance()->UnRegisterEvent(iter->second);
	}
	m_mapHandle.clear();
	m_Table.Clear();
	m_nTime = 0;
	SetWolrdLevel(0);
	m_mmapActivity.clear();
}

bool LoginActivityMgr::LoadFile()
{
	ClearFile();
	bool bRet = m_Table.LoadFile("table/LoginActivity.txt");
	if (bRet == false)
		return false;
	if(GSConfig::Instance()->IsCrossGS()==true)
		return true;
	time_t curtime = GameTime::GetTime();
	std::set<UINT32> setMap;
	UINT32 daybein = (UINT32)GameTime::GetDayBeginTime(curtime);
	for(int i = 0; i < m_Table.Table.size();i++)
	{
		LoginActivity::RowData *pRowData = m_Table.Table[i];
		UINT32 starttime	= XCommon::ParseTime(pRowData->Time);
		UINT32 endtime		= starttime + 24*3600;
		UINT32 offsettime   = starttime;
		auto iter = setMap.find(starttime);
		if (iter == setMap.end())
		{
			setMap.insert(starttime);
			if (daybein < offsettime)
			{
				TimeSpecific startts(starttime);
				UINT32 dwStartHandle = CTimeSpecificMgr::Instance()->RegisterEvent(startts, std::bind(&LoginActivityMgr::OnStart, this, starttime));
				TimeSpecific endtss(endtime);
				UINT32 dwEndHandle = CTimeSpecificMgr::Instance()->RegisterEvent(endtss, std::bind(&LoginActivityMgr::OnClose, this, starttime));
				m_mapHandle.insert(std::pair<UINT32,UINT32>(dwStartHandle,dwEndHandle));
			}else if (daybein == offsettime)
			{
				TimeSpecific startts(curtime+30);
				UINT32 dwStartHandle = CTimeSpecificMgr::Instance()->RegisterEvent(startts, std::bind(&LoginActivityMgr::OnStart, this, starttime));
				TimeSpecific endtss(endtime);
				UINT32 dwEndHandle = CTimeSpecificMgr::Instance()->RegisterEvent(endtss, std::bind(&LoginActivityMgr::OnClose, this, starttime));
				m_mapHandle.insert(std::pair<UINT32,UINT32>(dwStartHandle,dwEndHandle));
			}
		}
		m_mmapActivity.insert(std::make_pair((UINT32)starttime,pRowData));
	}
	return true;
}


bool LoginActivityMgr::OnStart(UINT32 nTime)
{
	LogInfo("activity start time=%u",nTime);
	ControlActivity(true);
	UINT32 nLevel = WorldLevelMgr::Instance()->GetLevel();
	SetWolrdLevel(nLevel,true);
	m_nTime = (UINT32)GameTime::GetDayBeginTime(GameTime::GetTime());
	return true;
}

void LoginActivityMgr::ControlActivity(bool bOpen)
{
	//开服时间
	if(GSConfig::Instance()->GetGameServerOpenDays() < GetGlobalConfig().LoginActivityOpenDay)
		return;
	std::vector<UINT32> vecSys;
	vecSys.push_back(SYS_LOGINACTIVITY);
	for (auto iter = RoleManager::Instance()->Begin(); iter != RoleManager::Instance()->End(); iter++)
	{
		Role*pRole = *iter;
		if (pRole->GetLevel() >= GetGlobalConfig().LoginActivityLevel)
		{
			if (bOpen == true)
			{
				if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(SYS_LOGINACTIVITY) == false)
				{
					pRole->Get<CRoleSystem>()->SetSystemOpened(SYS_LOGINACTIVITY);
					OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &vecSys, NULL);
					PtcG2C_LoginActivityStatusNtf ntf;
					ntf.m_Data.set_state(true);
					pRole->Send(ntf);
				}
			}else
			{
				if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(SYS_LOGINACTIVITY) == true)
				{
					pRole->Get<CRoleSystem>()->SetSystemOpened(SYS_LOGINACTIVITY,false);
					OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &vecSys);	
				}
			}
		}

	}
}


bool LoginActivityMgr::OnClose(UINT32 nTime)
{
	LogInfo("activity close=%u",nTime);
	ControlActivity(false);
	return true;
}

void LoginActivityMgr::OnTriger(Role *pRole,bool bLogin)
{
	if (pRole->GetLevel() < GetGlobalConfig().LoginActivityLevel)
		return;
	std::vector<UINT32> vecSys;
	vecSys.push_back(SYS_LOGINACTIVITY);
	UINT32 nTime = GameTime::GetDayBeginTime(GameTime::GetTime());
	if (nTime == m_nTime)
	{
		bool bIcon = false;
		if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(SYS_LOGINACTIVITY) == false)
		{
			pRole->Get<CRoleSystem>()->SetSystemOpened(SYS_LOGINACTIVITY);
			OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, &vecSys, NULL);
			bIcon = true;
		}
		if (bLogin==true||bIcon==true)
		{
			RoleMisc* record = pRole->Get<RoleMisc>();
			PtcG2C_LoginActivityStatusNtf ntf;
			if (record->GetLoginActTime() != nTime)
				ntf.m_Data.set_state(true);
			else
				ntf.m_Data.set_state(false);
			pRole->Send(ntf);
		}
	}else
	{
		if (pRole->Get<CRoleSystem>()->IsSystemOpenedAtRole(SYS_LOGINACTIVITY) == true)
		{
			pRole->Get<CRoleSystem>()->SetSystemOpened(SYS_LOGINACTIVITY,false);
			OpenSystemMgr::Instance()->NotifyClientSystemOpen(pRole, NULL, &vecSys);	
		}
	}
}

void LoginActivityMgr::GiveAward(Role *pRole)
{
	if (pRole == NULL)
		return;
	UINT32 nTime = GameTime::GetDayBeginTime(GameTime::GetTime());
	if (m_nTime != nTime)
	{
		LogInfo("LoginActivity  UnOpen nRoleID=%llu name=%s acttime=%u curtime=%u",pRole->GetID(),pRole->GetName().c_str(),m_nTime,nTime);
		return;
	}
	RoleMisc* record = pRole->Get<RoleMisc>();
	if (record->GetLoginActTime() == nTime)
	{
		LogInfo("LoginActivity RoleID Have Got nRoleID=%llu name=%s acttime=%u curtime=%u",pRole->GetID(),pRole->GetName().c_str(),m_nTime,nTime);
		return;
	}
	else
		record->SetLoginActTime(nTime);

    typedef std::multimap<UINT32, LoginActivity::RowData *>::iterator int_multimap;  
	auto iter = m_mmapActivity.equal_range(nTime);
	for(auto siter =  iter.first; siter != iter.second; siter++)
	{	
		LoginActivity::RowData *pRowData = siter->second;
		if (m_nWorldLevel>=pRowData->WorldLevel[0] && m_nWorldLevel < pRowData->WorldLevel[1])
		{
			for(int i = 0; i < pRowData->Award.size(); i++)
			{
				BagGiveItemTransition stTransition(pRole);
				stTransition.SetReason(ItemFlow_Reward, ItemFlow_LoginActivity, false);
				stTransition.GiveItem(pRowData->Award[i][0], pRowData->Award[i][1], 1);
				stTransition.NotifyClient();
			}
			LogInfo("LoginActivity RoleID Got  Sucess nRoleID=%llu time=%u",pRole->GetID(),nTime);
			break;
		}
	}
	PtcG2C_LoginActivityStatusNtf ntf;
	ntf.m_Data.set_state(false);
	pRole->Send(ntf);
}

void LoginActivityMgr::SetWolrdLevel(UINT32 nWorldLevel,bool bSave)
{
	m_nWorldLevel = nWorldLevel;
	if (bSave == true)
	{
		PtcG2M_UpdateGlobalValue ptc;
		ptc.m_Data.set_id(28);
		KKSG::GlobalLoginActivityInfo * info = ptc.m_Data.mutable_loginactivity();
		info->set_worldlv(nWorldLevel);
		MSLink::Instance()->SendTo(ptc);
	}
}

void LoginActivityMgr::Pack(Role *pRole,KKSG::LoginActivityRes &res)
{
	if (pRole == NULL)
	{
		res.set_errcode(KKSG::ERR_FAILED);
		return;
	}
	if(pRole->GetLevel() < GetGlobalConfig().LoginActivityLevel)
	{
		res.set_errcode(KKSG::ERR_FAILED);
		return;
	}
	UINT32 nTime = GameTime::GetDayBeginTime(GameTime::GetTime());
	if (nTime!=m_nTime)
	{
		res.set_errcode(KKSG::ERR_FAILED);
		return;
	}
	res.set_errcode(KKSG::ERR_SUCCESS);
	typedef std::multimap<UINT32, LoginActivity::RowData *>::iterator int_multimap;  
	auto iter = m_mmapActivity.equal_range(nTime);
	for(auto siter =  iter.first; siter != iter.second; siter++)
	{	
		LoginActivity::RowData *pRowData = siter->second;
		if (m_nWorldLevel>=pRowData->WorldLevel[0] && m_nWorldLevel < pRowData->WorldLevel[1])
		{
			for(int i = 0; i < pRowData->Award.size(); i++)
			{
				KKSG::ItemBrief* pItemBrief =  res.add_items();
				pItemBrief->set_itemid(pRowData->Award[i][0]);
				pItemBrief->set_itemcount(pRowData->Award[i][1]);
				pItemBrief->set_isbind(true);
			}
			break;
		}
	}
}