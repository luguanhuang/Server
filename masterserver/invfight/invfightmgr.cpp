#include "pch.h"
#include "invfightmgr.h"
#include "table/globalconfig.h"
#include <ctime>
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "scene/scenecreate.h"
#include "role/rolemanager.h"
#include "role/rolemanager.h"
#include "scene/scenecreate.h"
#include "scene/sceneswitch.h"
#include "team/teamrequest.h"
#include "team/teamforceop.h"
#include "invfightcrossmgr.h"
#include "invfight/ptcm2c_invfightnotify.h"

INSTANCE_SINGLETON(InvFightMgr)

InvRec::InvRec(UINT64 invFromID, UINT64 invToID)
{
	m_isCross = false;
	m_invFromID = invFromID;
	m_invToID = invToID;
	m_invTime = time(NULL);

	//static UINT64 InvIDIndex = 1;
	static UINT32 InvIDIndex = 1;
	m_invID = InvIDIndex ++;
}

InvRec::InvRec(UINT64 invID, const std::string& toAccount, UINT64 fromID)
{
	m_invID = invID;
	m_isCross = true;
	m_invFromID = fromID;
	m_invToID = 0;
	//m_fromAccount = fromAccount;
	m_toAccount = toAccount;
	m_invTime = time(NULL);
}

InvRec::~InvRec()
{

}

void InvRec::TestPrint()
{
	SSDebug << " invID = " << m_invID << " from = " << m_invFromID << " to = " << m_invToID << " time = " << m_invTime << END;
}

void InvRec::FillInvFRoleBrief(KKSG::InvFightRoleBrief& data)
{
	data.set_invid(m_invID);		
	data.set_ctime(GetTimeLeft((UINT32)time(NULL)));
	CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_invFromID);
	if(pRoleSummary)
	{
		data.set_name(pRoleSummary->GetName());
		data.set_level(pRoleSummary->GetLevel());
		data.set_profession(pRoleSummary->GetProfession());
		data.set_title(pRoleSummary->GetTitleID());
	}
}

UINT32 InvRec::GetTimeLeft(UINT32 now)
{
	if(m_invTime + GetGlobalConfig().InvFightTimeOver > now)
	{
		return UINT32(m_invTime + GetGlobalConfig().InvFightTimeOver - now);	
	}
	return 0;
}

RoleRec::RoleRec()
{

}

RoleRec::~RoleRec()
{

}

void RoleRec::Add(UINT64 invID)
{
	for(size_t i = 0; i < m_invList.size(); ++i)
	{
		if(m_invList[i] == invID)
		{
			return ;//err
		}
	}
	m_invList.push_back(invID);
}

void RoleRec::Del(UINT64 invID)
{
	for(auto it = m_invList.begin(); it != m_invList.end(); ++it)
	{
		if((*it) == invID)
		{
			m_invList.erase(it);
			break;
		}
	}
}

void RoleRec::TestPrint()
{
	for(auto it = m_invList.begin(); it != m_invList.end(); ++it)
	{
		SSDebug << " invID = " << (*it) << END;
	}
}

InvFightMgr::InvFightMgr()
{
	m_handler = 0;
}

InvFightMgr::~InvFightMgr()
{

}

bool InvFightMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);

	return true;
}

void InvFightMgr::Uninit()
{
	ClearData();

	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
}

void InvFightMgr::Add(InvRec& rec)
{
	if(m_invRecMap.find(rec.InvID()) != m_invRecMap.end())
	{
		return ;
	}
	m_invRecMap.insert(std::make_pair(rec.InvID(), rec));
	m_roleFromRecMap[rec.InvFromID()].Add(rec.InvID());
	m_roleToRecMap[rec.InvToID()].Add(rec.InvID());
}

void InvFightMgr::Del(UINT64 invID)
{
	auto it = m_invRecMap.find(invID);
	if(it == m_invRecMap.end())
	{
		return ;
	}

	UINT64 fromID = (it->second).InvFromID();
	UINT64 toID = (it->second).InvToID();
	
	m_roleFromRecMap[fromID].Del(invID);
	m_roleToRecMap[toID].Del(invID);
	m_invRecMap.erase(invID);

	if(m_roleFromRecMap[fromID].Size() == 0)
	{
		m_roleFromRecMap.erase(fromID);
	}
	if(m_roleToRecMap[toID].Size() == 0)
	{
		m_roleToRecMap.erase(toID);
	}
}

void InvFightMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckTimeOver();
	CheckDelayTime();
}

void InvFightMgr::ClearData()
{
	m_invRecMap.clear();
	m_roleFromRecMap.clear();
	m_roleToRecMap.clear();
	m_delayRoles.clear();
}

void InvFightMgr::MakeInvFight(UINT64 fromID, UINT64 toID)
{
	InvRec rec(fromID, toID);	
	Add(rec);
}

void InvFightMgr::CheckDelayTime()
{
	UINT32 now = UINT32(time(NULL));
	UINT32 DelayExistTime = 10;//10s

	for(auto it = m_delayRoles.begin(); it != m_delayRoles.end();)
	{
		if(it->second + DelayExistTime <= now)	
		{
			m_delayRoles.erase(it++);
		}
		else
		{
			it++;
		}
	}
}

void InvFightMgr::CheckTimeOver()
{
	UINT32 now = UINT32(time(NULL));
	InvFListType waitDel;
	for(auto it = m_invRecMap.begin(); it != m_invRecMap.end(); ++it)
	{
		InvRec& rec = (it->second);
		if(0 == rec.GetTimeLeft(now))			
		{
			waitDel.push_back(rec.InvID());
		}
	}

	DelInvList(waitDel);
}

void InvFightMgr::TestPrint()
{
	SSDebug << __FUNCTION__ << END;
	for(auto it = m_invRecMap.begin(); it != m_invRecMap.end(); ++it)
	{
		(it->second).TestPrint();
	}

	for(auto it = m_roleFromRecMap.begin(); it != m_roleFromRecMap.end(); ++it)
	{
		SSDebug << " role = " << (it->first) << END;
		(it->second).TestPrint();
	}

	for(auto it = m_roleToRecMap.begin(); it != m_roleToRecMap.end(); ++it)
	{
		SSDebug << " role = " << (it->first) << END;
		(it->second).TestPrint();
	}

	for(auto it = m_delayRoles.begin(); it != m_delayRoles.end(); ++it)
	{
		SSDebug << " role = " << (it->first) << " time = " << (it->second) << END;
	}
}

void InvFightMgr::DelFromIDInv(UINT64 fromID)
{
	InvFListType invList;
	GetInvFromList(fromID, invList);
	DelInvList(invList);
}

void InvFightMgr::DelToIDInv(UINT64 toID)
{
	InvFListType invList;
	GetInvToList(toID, invList);
	DelInvList(invList);
}

void InvFightMgr::GetInvFromList(UINT64 fromID, InvFListType& invList)
{
	auto it = m_roleFromRecMap.find(fromID);
	if(it != m_roleFromRecMap.end())
	{
		invList = (it->second).List();
	}
}

void InvFightMgr::GetInvToList(UINT64 toID, InvFListType& invList)
{
	auto it = m_roleToRecMap.find(toID);
	if(it != m_roleToRecMap.end())
	{
		invList = (it->second).List();
	}
}

void InvFightMgr::DelInvList(InvFListType& invList)
{
	for(size_t i = 0; i < invList.size(); ++i)
	{
		Del(invList[i]);
	}
}

bool InvFightMgr::IsInDelay(UINT64 roleID)
{
	if(m_delayRoles.find(roleID) != m_delayRoles.end())
	{
		return true;
	}
	return false;
}

bool InvFightMgr::HaveInvTo(UINT64 fromID, UINT64 toID)
{
	auto pRoleRec = GetInvFromRec(fromID);			
	if(NULL == pRoleRec)
	{
		return false;
	}
	const auto& recList = pRoleRec->List();
	for(size_t i = 0; i < recList.size(); ++i)
	{
		auto pUniRec = GetInvUniRec(recList[i]);
		if(pUniRec && pUniRec->InvToID() == toID)
		{
			return true;
		}
	}
	return false;
}

InvRec* InvFightMgr::GetInvUniRec(UINT64 invID)
{
	auto it = m_invRecMap.find(invID);
	if(it != m_invRecMap.end())
	{
		return &(it->second);
	}
	return NULL;
}

RoleRec* InvFightMgr::GetInvFromRec(UINT64 fromID)
{
	auto it = m_roleFromRecMap.find(fromID);
	if(it != m_roleFromRecMap.end())
	{
		return &(it->second);
	}
	return NULL;
}

RoleRec* InvFightMgr::GetInvToRec(UINT64 toID)
{
	auto it = m_roleToRecMap.find(toID);
	if(it != m_roleToRecMap.end())
	{
		return &(it->second);
	}
	return NULL;
}

UINT32 InvFightMgr::InvFromCount(UINT64 fromID)
{
	auto pRoleRec = GetInvFromRec(fromID);
	if(pRoleRec)
	{
		return pRoleRec->Size();
	}
	return 0;
}

UINT32 InvFightMgr::InvToCount(UINT64 toID)
{
	auto pRoleRec = GetInvToRec(toID);
	if(pRoleRec)
	{
		return pRoleRec->Size();
	}
	return 0;
}

void InvFightMgr::FillInvToList(UINT64 toID, KKSG::InvFightRes& roRes)
{
	auto pRoleRec = GetInvToRec(toID);
	if(NULL == pRoleRec)
	{
		return;
	}

	const auto& recList = pRoleRec->List();
	for(size_t i = 0; i < recList.size() && i < InvFightToCountMax; ++i)
	{
		auto pUniRec = GetInvUniRec(recList[i]);
		if(pUniRec)
		{
			pUniRec->FillInvFRoleBrief(*roRes.add_roles());
		}
	}
}

void InvFightMgr::OnLogin(CRole* role)
{
	if (!role)
	{
		return;
	}
	UINT32 CurInvFightToCount = InvToCount(role->GetID());
	CurInvFightToCount += InvFightCrossMgr::Instance()->InvToCount(role->GetAccount());

	if (CurInvFightToCount <= 0)
	{
		return;
	}
	
	PtcM2C_InvFightNotify ptc;
	ptc.m_Data.set_ntftype(KKSG::IFNT_INVITE_ME);
	ptc.m_Data.set_count(CurInvFightToCount);
	role->Send(ptc);
}

void InvFightMgr::ReadyFight(UINT64 fromID, UINT64 toID)
{
	UINT32 now = UINT32(time(NULL));
	m_delayRoles[fromID] = now;
	m_delayRoles[toID] = now;

	CreateSceneBegin(fromID, toID);
}

void InvFightMgr::CreateSceneBegin(UINT64 fromID, UINT64 toID)
{
	SSInfo << __FUNCTION__ << " from = " << fromID << " to = " << toID << END;
	auto pFromSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(fromID);
	auto pToSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(toID);
	if(NULL == pFromSummary || NULL == pToSummary)
	{
		return ;
	}

	KKSG::CreateBattleParam data;
	KKSG::InvFightBattleInfo& info = *data.mutable_invfightinfo();
	KKSG::InvFightUnit& kkfrom = *info.add_inunit();
	kkfrom.set_roleid(fromID);
	pFromSummary->FillSmallInfo(*kkfrom.mutable_smallinfo());
	KKSG::InvFightUnit& kkto = *info.add_inunit();
	kkto.set_roleid(toID);
	pToSummary->FillSmallInfo(*kkto.mutable_smallinfo());

	bool flag = CSceneCreator::Instance()->CreateBattleScene(GetGlobalConfig().InvFightMapID, &ICreateInvFightListener::GlobalCreateInvFightListener, data);
	if(false == flag)
	{
		SSError << " create battle scene err fromID = " << fromID << " toID = " << toID << END;
	}
	
}

void InvFightMgr::CreateSceneEnd(UINT32 sceneID, UINT64 fromID, UINT64 toID)
{
	SSInfo << __FUNCTION__ << " scene = " << sceneID << " from = " << fromID << " to = " << toID << END;
	if(0 == sceneID)
	{
		return ;
	}
	CRole* pFromRole = CRoleManager::Instance()->GetByRoleID(fromID);
	if(NULL == pFromRole)
	{
		SSWarn << " from role = null " << fromID << END;
		return ;
	}
	CRole* pToRole = CRoleManager::Instance()->GetByRoleID(toID);
	if(NULL == pToRole)
	{
		SSWarn << " to role = null " << toID << END;
		return ;
	}

	std::vector<CRole*> roles;
	if(pFromRole->GetTeamID())
	{
		roles.push_back(pFromRole);
	}
	if(pToRole->GetTeamID())
	{
		roles.push_back(pToRole);
	}
	TeamForceOp op;
	op.LeaveTeam(roles);

	KKSG::SceneSwitchData tmpData;
	CSceneSwitch::EnterScene(pFromRole, sceneID, tmpData);
	CSceneSwitch::EnterScene(pToRole, sceneID, tmpData);
}
