#include "pch.h"
#include "rewardhandler.h"
#include "unit/role.h"
#include "rewardrecord.h"
#include "arenamgr.h"
#include "rewardmgr.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildboss.h"
#include "unit/rolemanager.h"
#include "pkmgr.h"
#include "util/timespecificmgr.h"

#define TIMEOFFSET 10

CRewardHandler::CRewardHandler(UINT32 dwType)
{
	m_dwType = dwType;
}

CRewardHandler::~CRewardHandler()
{

}

bool CRewardHandler::GetReward(Role* pRole, SRewardInfo& stRewardInfo)
{
	stRewardInfo.vecItem = CRewardMgr::Instance()->GetDefaultReward(stRewardInfo.dwType);
	if (stRewardInfo.vecItem.empty())
	{
		SSWarn<<"reward is empty, type:"<<stRewardInfo.dwType<<END;
		return false;
	}
	return true;
}

bool CRewardHandler::OnTimer(UINT32 dwType)
{
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* role = *i;
		CRewardMgr::Instance()->PushReward(role, dwType);
	}
	return true;
}

///////////////////////////////////////////////////////////////////////
CDinnerRewardHandler::CDinnerRewardHandler(UINT32 dwType)
:CRewardHandler(dwType)
{
	TimeSpecific begin;
	begin.hour =  GetGlobalConfig().DinnerTime[0];
	begin.min =  GetGlobalConfig().DinnerTime[1];
	begin.sec = 0;

	TimeSpecific end;
	end.hour =  GetGlobalConfig().DinnerTime[2];
	end.min =  GetGlobalConfig().DinnerTime[3];
	end.sec = TIMEOFFSET;

	TimeSpecific crossday;
	crossday.hour = 0;	
	crossday.min = 0;
	crossday.sec = TIMEOFFSET;

	m_begin = CTimeSpecificMgr::Instance()->RegisterEvent(begin, std::bind(&CDinnerRewardHandler::OnTimer, this, dwType));
	m_end = CTimeSpecificMgr::Instance()->RegisterEvent(end, std::bind(&CDinnerRewardHandler::OnTimer, this, dwType));
	m_crossday= CTimeSpecificMgr::Instance()->RegisterEvent(crossday, std::bind(&CDinnerRewardHandler::OnTimer, this, dwType));
}

CDinnerRewardHandler::~CDinnerRewardHandler()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_begin);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_end);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_crossday);
}

bool CDinnerRewardHandler::GetReward(Role* pRole, SRewardInfo& stRewardInfo)
{
	if (!XCommon::IsWhithinTime( GetGlobalConfig().DinnerTime[0],  GetGlobalConfig().DinnerTime[1], 0,
		 GetGlobalConfig().DinnerTime[2],  GetGlobalConfig().DinnerTime[3], 0))
	{
		return false;
	}
	stRewardInfo.vecParam.push_back(ToString( GetGlobalConfig().DinnerTime[0]*60 +  GetGlobalConfig().DinnerTime[1]));
	stRewardInfo.vecParam.push_back(ToString( GetGlobalConfig().DinnerTime[2]*60 +  GetGlobalConfig().DinnerTime[3]));
	for (auto i =  GetGlobalConfig().DinnerReward.begin(); i !=  GetGlobalConfig().DinnerReward.end(); ++i)
	{
		stRewardInfo.vecItem.push_back(ItemDesc(i->seq[0], i->seq[1]));
	}
	return true;
}

bool CDinnerRewardHandler::Update(CRewardRecord* pRecord, UINT32 dwType)
{
	return pRecord->Update(dwType,  GetGlobalConfig().DinnerTime[2],  GetGlobalConfig().DinnerTime[3]);
}

///////////////////////////////////////////////////////////////////////
CSupperRewardHandler::CSupperRewardHandler(UINT32 dwType)
:CRewardHandler(dwType)
{
	TimeSpecific begin;
	begin.hour =  GetGlobalConfig().SupperTime[0];
	begin.min =  GetGlobalConfig().SupperTime[1];
	begin.sec = 0;

	TimeSpecific end;
	end.hour =  GetGlobalConfig().SupperTime[2];
	end.min =  GetGlobalConfig().SupperTime[3];
	end.sec = TIMEOFFSET;

	TimeSpecific crossday;
	crossday.hour = 0;
	crossday.min = 0;	
	crossday.sec = TIMEOFFSET;

	m_begin = CTimeSpecificMgr::Instance()->RegisterEvent(begin, std::bind(&CSupperRewardHandler::OnTimer, this, dwType));
	m_end = CTimeSpecificMgr::Instance()->RegisterEvent(end, std::bind(&CSupperRewardHandler::OnTimer, this, dwType));
	m_crossday = CTimeSpecificMgr::Instance()->RegisterEvent(crossday, std::bind(&CSupperRewardHandler::OnTimer, this, dwType));
}

CSupperRewardHandler::~CSupperRewardHandler()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_begin);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_end);
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_crossday);
}

bool CSupperRewardHandler::GetReward(Role* pRole, SRewardInfo& stRewardInfo)
{
	if (!XCommon::IsWhithinTime( GetGlobalConfig().SupperTime[0],  GetGlobalConfig().SupperTime[1], 0,
		 GetGlobalConfig().SupperTime[2],  GetGlobalConfig().SupperTime[3], 0))
	{
		return false;
	}
	stRewardInfo.vecParam.push_back(ToString( GetGlobalConfig().SupperTime[0]*60 +  GetGlobalConfig().SupperTime[1]));
	stRewardInfo.vecParam.push_back(ToString( GetGlobalConfig().SupperTime[2]*60 + GetGlobalConfig().SupperTime[3]));
	for (auto i =  GetGlobalConfig().SupperReward.begin(); i !=  GetGlobalConfig().SupperReward.end(); ++i)
	{
		stRewardInfo.vecItem.push_back(ItemDesc(i->seq[0], i->seq[1]));
	}
	return true;
}

bool CSupperRewardHandler::Update(CRewardRecord* pRecord, UINT32 dwType)
{
	return pRecord->Update(dwType,  GetGlobalConfig().SupperTime[2],  GetGlobalConfig().SupperTime[3]);
}
