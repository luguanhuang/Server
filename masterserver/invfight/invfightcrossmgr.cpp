#include "pch.h"
#include "invfightcrossmgr.h"
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
#include "scene/ptcm2g_enterscenefromms.h"
#include "network/gslink.h"

INSTANCE_SINGLETON(InvFightCrossMgr)


InvCrossRec::InvCrossRec(UINT64 invID, UINT32 fromServer, UINT32 toServer, const std::string& fromAccount, const std::string& toAccount, UINT64 fromID, UINT64 toID)
{
	m_invID = invID;
	m_fromServer = fromServer;
	m_toServer = toServer;
	m_invFromID = fromID;
	m_invToID = toID;
	m_fromAccount = fromAccount;
	m_toAccount = toAccount;

	m_invTime = time(NULL);
}

InvCrossRec::~InvCrossRec()
{

}

void InvCrossRec::TestPrint()
{
	SSDebug << " invID = " << m_invID << " from = " << m_invFromID << " to = " << m_invToID << " time = " << m_invTime << END;
}

void InvCrossRec::FillInvFRoleBrief(KKSG::InvFightRoleBrief& data)
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
		data.set_isplatfriend(true);
	}
	else
	{
		for (int i = 0; i < smallInfo.roles_size(); ++i)
		{
			if (smallInfo.roles(i).roleid() == m_invFromID)
			{
				data.set_name(smallInfo.roles(i).rolename());
				data.set_level(smallInfo.roles(i).rolelevel());
				data.set_profession(smallInfo.roles(i).roleprofession());
				data.set_title(0);
				data.set_isplatfriend(true);
				break;
			}
		}
	}
}

UINT32 InvCrossRec::GetTimeLeft(UINT32 now)
{
	if(m_invTime + GetGlobalConfig().InvFightTimeOver > now)
	{
		return UINT32(m_invTime + GetGlobalConfig().InvFightTimeOver - now);
	}
	return 0;
}


InvFightCrossMgr::InvFightCrossMgr()
{
	m_handler = 0;
}

InvFightCrossMgr::~InvFightCrossMgr()
{

}

bool InvFightCrossMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 2000, -1, __FILE__, __LINE__);

	return true;
}

void InvFightCrossMgr::Uninit()
{
	ClearData();

	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}
}

void InvFightCrossMgr::_AddFrom(InvCrossRec& rec)
{
	auto find = m_invRecMap.find(rec.InvID());
	if (find != m_invRecMap.end())
	{
		if (!find->second.InvToID() && rec.InvToID())
		{
			m_invRecMap.erase(rec.InvID());
		}
	}
	m_invRecMap.insert(std::make_pair(rec.InvID(), rec));
	m_roleFromRecMap[rec.InvFromID()].Add(rec.InvID());

	SSDebug << "invfight from id " << rec.InvID() << END;
}

void InvFightCrossMgr::_AddTo(InvCrossRec& rec)
{
	auto find = m_invRecMap.find(rec.InvID());
	if (find != m_invRecMap.end())
	{
		if (!find->second.InvToID() && rec.InvToID())
		{
			m_invRecMap.erase(rec.InvID());
		}
	}
	m_invRecMap.insert(std::make_pair(rec.InvID(), rec));
	m_accountToRecMap[rec.InvToAccount()].Add(rec.InvID());

	SSDebug << "invfight to id " << rec.InvID() << END;
}

void InvFightCrossMgr::Del(UINT64 invID)
{
	auto it = m_invRecMap.find(invID);
	if(it == m_invRecMap.end())
	{
		return ;
	}

	UINT64 fromID = (it->second).InvFromID();
	std::string& toAccount = (it->second).InvToAccount();
	
	m_roleFromRecMap[fromID].Del(invID);
	m_accountToRecMap[toAccount].Del(invID);

	if(m_roleFromRecMap[fromID].Size() == 0)
	{
		m_roleFromRecMap.erase(fromID);
	}
	if(m_accountToRecMap[toAccount].Size() == 0)
	{
		m_accountToRecMap.erase(toAccount);
	}

	m_invRecMap.erase(invID);

	SSDebug << "delete invfight id " << invID<< END;
}

void InvFightCrossMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckTimeOver();
}

void InvFightCrossMgr::AddFromCrossFight(const KKSG::InvFightCrossMsg& msg)
{
	InvCrossRec rec(msg.invid(), msg.fromsrverid(), msg.toserverid(), msg.fromaccount(), msg.toaccount(), msg.fromid(), msg.toid());	
	_AddFrom(rec);
}

void InvFightCrossMgr::AddToCrossFight(const KKSG::InvFightCrossMsg& msg)
{
	InvCrossRec rec(msg.invid(), msg.fromsrverid(), msg.toserverid(), msg.fromaccount(), msg.toaccount(), msg.fromid(), msg.toid());	
	rec.InvSmallInfo().CopyFrom(msg.smallinfo());
	if (msg.time())
	{
		rec.InvTime() = msg.time();
	}
	_AddTo(rec);
}

void InvFightCrossMgr::ClearData()
{
	m_invRecMap.clear();
	m_roleFromRecMap.clear();
	m_accountToRecMap.clear();
}

void InvFightCrossMgr::CheckTimeOver()
{
	UINT32 now = UINT32(time(NULL));
	InvFListType waitDel;
	for(auto it = m_invRecMap.begin(); it != m_invRecMap.end(); ++it)
	{
		InvCrossRec& rec = (it->second);
		if(0 == rec.GetTimeLeft(now))			
		{
			waitDel.push_back(rec.InvID());
		}
	}

	DelInvList(waitDel);
}

void InvFightCrossMgr::TestPrint()
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

	for(auto it = m_accountToRecMap.begin(); it != m_accountToRecMap.end(); ++it)
	{
		SSDebug << " account = " << (it->first) << END;
		(it->second).TestPrint();
	}

}

void InvFightCrossMgr::DelFromIDInv(UINT64 fromID)
{
	InvFListType invList;
	GetInvFromList(fromID, invList);
	DelInvList(invList);
}

void InvFightCrossMgr::DelToIDInv(const std::string& toAccount)
{
	InvFListType invList;
	GetInvToList(toAccount, invList);
	DelInvList(invList);
}

void InvFightCrossMgr::GetInvFromList(UINT64 fromID, InvFListType& invList)
{
	auto it = m_roleFromRecMap.find(fromID);
	if(it != m_roleFromRecMap.end())
	{
		invList = (it->second).List();
	}
}

void InvFightCrossMgr::GetInvToList(/*UINT64 toID*/const std::string& toAccount, InvFListType& invList)
{
	auto it = m_accountToRecMap.find(toAccount);
	if(it != m_accountToRecMap.end())
	{
		invList = (it->second).List();
	}
}

void InvFightCrossMgr::DelInvList(InvFListType& invList)
{
	for(size_t i = 0; i < invList.size(); ++i)
	{
		Del(invList[i]);
	}
}

bool InvFightCrossMgr::HaveInvTo(UINT64 fromID, const std::string& toAccount)
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
		if(pUniRec && strcmp(pUniRec->InvToAccount().c_str(), toAccount.c_str()) == 0)
		{
			return true;
		}
	}
	return false;
}

InvCrossRec* InvFightCrossMgr::GetInvUniRec(UINT64 invID)
{
	auto it = m_invRecMap.find(invID);
	if(it != m_invRecMap.end())
	{
		return &(it->second);
	}
	return NULL;
}

RoleRec* InvFightCrossMgr::GetInvFromRec(UINT64 fromID)
{
	auto it = m_roleFromRecMap.find(fromID);
	if(it != m_roleFromRecMap.end())
	{
		return &(it->second);
	}
	return NULL;
}

RoleRec* InvFightCrossMgr::GetInvToRec(const std::string& toAccount)
{
	auto it = m_accountToRecMap.find(toAccount);
	if(it != m_accountToRecMap.end())
	{
		return &(it->second);
	}
	return NULL;
}

UINT32 InvFightCrossMgr::InvFromCount(UINT64 fromID)
{
	auto pRoleRec = GetInvFromRec(fromID);
	if(pRoleRec)
	{
		return pRoleRec->Size();
	}
	return 0;
}

UINT32 InvFightCrossMgr::InvToCount(const std::string& toAccount)
{
	auto pRoleRec = GetInvToRec(toAccount);
	if(pRoleRec)
	{
		return pRoleRec->Size();
	}
	return 0;
}

void InvFightCrossMgr::FillInvToList(const std::string& toAccount, KKSG::InvFightRes& roRes)
{
	auto pRoleRec = GetInvToRec(toAccount);
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

void InvFightCrossMgr::EnterBattle(UINT32 sceneID, UINT64 roleID, UINT32 gsline, UINT32 mapID)
{
	SSInfo << __FUNCTION__ << " scene = " << sceneID << " role = " << roleID << END;
	if(0 == sceneID)
	{
		return ;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(NULL == pRole)
	{
		SSWarn << " role = null " << roleID << END;
		return ;
	}

	std::vector<CRole*> roles;
	if(pRole->GetTeamID())
	{
		roles.push_back(pRole);
	}
	TeamForceOp op;
	op.LeaveTeam(roles);

	//KKSG::SceneSwitchData tmpData;
	//CSceneSwitch::EnterScene(pRole, sceneID, tmpData, gsline, true, mapID);
	PtcM2G_EnterSceneFromMs ntf;
	ntf.m_Data.set_roleid(pRole->GetID());
	ntf.m_Data.set_sceneid(sceneID);
	ntf.m_Data.set_gsline(gsline);
	ntf.m_Data.set_mapid(mapID);
	ntf.m_Data.set_iscross(true);

	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, ntf);
}
