#include "pch.h"
#include "sceneinvfight.h"
#include "unit/role.h"
#include <ctime>
#include "unit/rolemanager.h"
#include "scene.h"
#include "invfight/ptcg2c_invfightbefenterscenentf.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/rolesummary.h"
#include "invfight/ptcg2k_invfightagainreqg2w.h"
#include "invfight/ptcg2m_invfightagainreqg2m.h"
#include "network/worldlink.h"
#include "network/mslink.h"

SceneInvFigtht::SceneInvFigtht(Scene* scene):SceneVsBase(scene)
{
	m_winRoleID = 0;
}

SceneInvFigtht::~SceneInvFigtht()
{

}

bool SceneInvFigtht::Init(const KKSG::CreateBattleParam& roParam)
{
	m_data = roParam.invfightinfo();

	if(m_data.inunit_size() < 2)
	{
		SSError << " inunit = " << m_data.inunit_size() << END;
	}

	InitState();

	return true;
}

void SceneInvFigtht::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

UINT32 SceneInvFigtht::GetGroup(UINT64 roleid)
{
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		if(roleid == m_data.inunit(i).roleid())
		{
			return (i+1);
		}
	}
	return 1;
}

VsStateType SceneInvFigtht::IsAllLoad()
{
	UINT64 roleID11 = GetRoleIDByIndex(0);
	UINT64 roleID22 = GetRoleIDByIndex(1);
	if(GetRole(roleID11) && GetRole(roleID22))
	{
		return VS_STATE_PREPARE321;
	}
	return VS_STATE_DUMMY;
}

UINT64 SceneInvFigtht::GetRoleIDByIndex(int index)
{
	if(index >= 0 && index < m_data.inunit_size())
	{
		return m_data.inunit(index).roleid();
	}
	return 0;
}

bool SceneInvFigtht::IsFightRole(UINT64 roleID)
{
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		if(m_data.inunit(i).roleid() == roleID)
		{
			return true;
		}
	}
	return false;
}

UINT64 SceneInvFigtht::GetOpRoleID(UINT64 roleID)
{
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		if(m_data.inunit(i).roleid() != roleID)
		{
			return m_data.inunit(i).roleid();
		}
	}
	return 0;
}

VsStateType SceneInvFigtht::CheckFightEnd()
{
	bool timeOver = false;
	UINT32 FightTimeConfig = GetGlobalConfig().InvFightFightTime;
	if(GetStateStartTime() + FightTimeConfig <= time(NULL))
	{
		timeOver = true;
	}

	UINT64 winRoleID = GetWinRoleID(timeOver);
	if(0 != winRoleID)
	{
		m_winRoleID = winRoleID;
		return VS_STATE_END;
	}
	else if(timeOver)
	{
		SSError << " invfight err sceneid = " << GetScene()->GetSceneID() << END;
		return VS_STATE_END;
	}

	return VS_STATE_DUMMY;
}

UINT64 SceneInvFigtht::GetWinRoleID(bool timeOver)
{
	UINT64 roleID11 = GetRoleIDByIndex(0);
	UINT64 roleID22 = GetRoleIDByIndex(1);
	
	Role* pRole11 = GetRole(roleID11);
	if(NULL == pRole11 || pRole11->IsDead())
	{
		return roleID22;
	}

	Role* pRole22 = GetRole(roleID22);
	if(NULL == pRole22 || pRole22->IsDead())
	{
		return roleID11;	
	}

	if(timeOver && pRole11 && pRole22)//±ÈÑªÁ¿
	{
		if(pRole11->GetPreciseHpPercent() > pRole22->GetPreciseHpPercent())
		{
			return roleID11;
		}
		else if(pRole11->GetPreciseHpPercent() < pRole22->GetPreciseHpPercent())
		{
			return roleID22;
		}
		else if(pRole11->GetAttr(BASIC_CurrentHP) > pRole22->GetAttr(BASIC_CurrentHP))
		{
			return roleID11;
		}
		else
		{
			return roleID22;
		}
	}

	return 0;
}

bool SceneInvFigtht::OnEvent(const VsEvent& event)
{
	UINT64 roleID = event.roleid;
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);

	if(NULL == pRole)
	{
		return false;
	}

	if(pRole->IsWatcher())
	{
		return false;
	}

	if(VS_EVENT_ROLE_LOADED == event.type)
	{
		//if(IsInWaitLoaded())
		//{
		//	MakePuppet(pRole, true);
		//}
	}

	return true;
}

void SceneInvFigtht::Update()
{
	m_statemgr.Update();
}

void SceneInvFigtht::OnStateEnd()
{
	SSDebug << __FUNCTION__ << " " << GetRoleIDByIndex(0) << " " << GetRoleIDByIndex(1) << " " << m_winRoleID << END;
	GetScene()->SetDontDestory(false);	
	SetEnd();
}

void SceneInvFigtht::FillResult(UINT64 roleID, KKSG::InvFightBattleResult& data)
{
	if(m_winRoleID == roleID)
	{
		data.set_resulttype(KKSG::PkResult_Win);
	}
	else
	{
		data.set_resulttype(KKSG::PkResult_Lose);
	}

	UINT64 opRoleID = GetOpRoleID(roleID);
	if(opRoleID)
	{
		std::string opRoleName = GetRoleName(opRoleID);
		data.set_opname(opRoleName);
	}
}

void SceneInvFigtht::OnEnterBeforeNtf(Role* role)
{
	if(NULL == role)
	{
		return ;
	}
	
	PtcG2C_InvFightBefEnterSceneNtf ptc;

	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		FillRoleSmallInfo(m_data.inunit(i).roleid(), *ptc.m_Data.add_roles());
	}

	role->Send(ptc);
}

void SceneInvFigtht::FillRoleSmallInfo(UINT64 roleID, KKSG::RoleSmallInfo& data)
{
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		if(m_data.inunit(i).roleid() == roleID)
		{
			data.CopyFrom(m_data.inunit(i).smallinfo());
			data.set_roleprofession(data.roleprofession()%10);
		}
	}
}

std::string SceneInvFigtht::GetRoleName(UINT64 roleID)
{
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		if(roleID == m_data.inunit(i).roleid())
		{
			return m_data.inunit(i).smallinfo().rolename();
		}
	}
	return "";
}

void SceneInvFigtht::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		UINT64 roleID = m_data.inunit(i).roleid();
		Role* pRole = GetRole(roleID);
		if(pRole)
		{
			loaded.push_back(pRole);
		}
		else
		{
			unloaded.push_back(GetRoleName(roleID));
		}
	}
}

bool SceneInvFigtht::EndOnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(NULL == pRole)
	{
		return true;;
	}
	if(pRole->IsWatcher())
	{
		return true;
	}
	if(!IsFightRole(event.roleid))
	{
		return true;
	}
	if(event.type != VS_EVENT_ROLE_QUIT)
	{
		return true;
	}
	if(m_fightAgainRole[event.roleid] == IFAS_END_WAIT)
	{
		return true;
	}

	m_fightAgainRole[event.roleid] = IFAS_NONE;

	UINT64 opRoleID = GetOpRoleID(event.roleid);
	if(IsFightRole(opRoleID))
	{
		if(m_fightAgainRole[opRoleID] == IFAS_READY)
		{
			OpLeaveNtf(GetRole(opRoleID));	
		}
	}

	return true;
}

void SceneInvFigtht::RoleReqFightAgain(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	if(pRole->IsWatcher())
	{
		return;
	}
	if(!IsEnd())
	{
		return;
	}
	if(!IsFightRole(pRole->GetID()))
	{
		return;
	}
	if(m_fightAgainRole[pRole->GetID()] == IFAS_END_WAIT)
	{
		return;
	}
	//if(m_fightAgainRole[pRole->GetID()] == IFAS_READY)
	//{
	//	return;
	//}
	m_fightAgainRole[pRole->GetID()] = IFAS_READY;
	UINT64 opRoleID = GetOpRoleID(pRole->GetID());
	if(IsFightRole(opRoleID))
	{
		if(m_fightAgainRole[opRoleID] == IFAS_READY)
		{
			m_fightAgainRole[pRole->GetID()] = IFAS_END_WAIT;
			m_fightAgainRole[opRoleID] = IFAS_END_WAIT;
			MakeFightAgain();
		}
		else if(!GetRole(opRoleID))
		{
			OpLeaveNtf(pRole);
		}
	}
} 

void SceneInvFigtht::MakeFightAgain()
{
	KKSG::InvFightAgainPara para;
	KKSG::CreateCrossBattleSceneArg& data = *para.mutable_data();
	data.set_mapid(GetScene()->GetSceneTemplateID());
	for(int i = 0; i < m_data.inunit_size(); ++i)
	{
		const auto& ind = m_data.inunit(i);
		UINT64 roleID = ind.roleid();
		*data.mutable_smallinfo()->add_roles() = ind.smallinfo();
		auto& info = *data.add_createinfos();
		Role* pRole = GetRole(roleID);
		if(NULL == pRole)
		{
			SSError << " find role failed roleid = " << roleID << END;
			return;
		}
		info.set_roleid(pRole->GetID());
		info.set_serverid(pRole->GetFromServerId());
	}

	if(GSConfig::Instance()->IsCrossGS())			
	{
		PtcG2K_InvfightAgainReqG2W ptc;
		ptc.m_Data = para;
		WorldLink::Instance()->SendTo(ptc);
	}
	else
	{
		PtcG2M_InvfightAgainReqG2M ptc;
		ptc.m_Data = para;
		MSLink::Instance()->SendTo(ptc);
	}
}

void SceneInvFigtht::OpLeaveNtf(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	SSDebug << " op leave ntf role = " << pRole->GetName() << END;
	pRole->ErrorCodeNtf(KKSG::ERR_OP_ROLE_LEAVE, true);
}
