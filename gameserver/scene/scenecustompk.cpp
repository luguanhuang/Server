#include "pch.h"
#include "scenecustompk.h"
#include "time.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include "gamelogic/rolesummarymgr.h"
#include "attributepk.h"
#include "foreach.h"
#include "scene.h"
#include "scenemanager.h"
#include "network/mslink.h"
#include "scenestatistics.h"
#include "network/mslink.h"
#include "network/worldlink.h"
#include "custombattle/ptcg2c_custombattleloadingntf.h"
#include "custombattle/ptcg2m_reportcustombattleresult.h"
#include "custombattle/rpcg2k_custombattleresultworld.h"
#include "custombattle/rpcg2m_custombattleresultms.h"
#include "live/livemanager.h"

SceneCustomPk::SceneCustomPk(Scene* scene):SceneVsBase(scene)
{
	m_scene = scene;
	m_isonequit = false;
}

SceneCustomPk::~SceneCustomPk()
{
}

void SceneCustomPk::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

bool SceneCustomPk::Init(const KKSG::CreateBattleParam& roData)
{
	if (!roData.has_custombattle())
	{
		SSWarn<<"has not custombattle data"<<END;
		return false;

	}
	m_battle = roData.custombattle().infoa().battle();
	m_infoa = roData.custombattle().infoa().role();
	m_infob = roData.custombattle().infob().role();

	m_units[m_infoa.roleid()] = SCustomPkResult();
	m_units[m_infob.roleid()] = SCustomPkResult();

	InitState();

	SSInfo<<"init custombattle pk, rolea:"<<m_infoa.roleid()<<" roleb:"<<m_infob.roleid()<<" issystem:"
		<<m_battle.issystem()<<" isfair:"<<m_battle.isfair()<<" uid:"<<m_battle.battleuid()<<END;

	m_logger.Init(m_battle.battleuid(), KKSG::CustomBattle_PK_Normal, m_battle.isfair(), m_battle.issystem());

	LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_CUSTOMPK, GetScene()->GetSceneID(), 0, 0, 0);

	return true;
}

void SceneCustomPk::Update()
{
	m_statemgr.Update();
}

UINT32 SceneCustomPk::GetGroup(UINT64 roleid)
{
	UINT32 Index = 1;
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		if(it->first == roleid)
		{
			return Index;
		}
		++Index;
	}

	return 1;
}

UINT32 SceneCustomPk::GetLoadWaitTime()
{
	return GetGlobalConfig().PkLoadWaitTime;
}

bool SceneCustomPk::OnEvent(const VsEvent& event)
{
	///> 忽略玩家观战
	if(Unit::GetUnitType(event.roleid) == KKSG::Category_Role)
	{
		Role* pRole = m_scene->FindSceneRole(event.roleid);
		if(pRole && pRole->IsWatcher())
		{
			return false;
		}
	}

	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			OnRoleEnter(event.roleid);
			break;
		}
	case VS_EVENT_ROLE_QUIT:
		{
			OnRoleLeave(event.roleid);	
			break;
		}
	case VS_EVENT_ROLE_DIE:
		{
			OnRoleDie(event.roleid);
			break;
		}
	default:
		break;
	}

	return true;
}

bool SceneCustomPk::OnRoleEnter(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return false;
	}
	i->second.state = VS_ROLE_LOADED;
	return true;
}

bool SceneCustomPk::OnRoleLeave(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return false;
	}
	i->second.state = VS_ROLE_QUIT;
	return true;
}

bool SceneCustomPk::OnRoleDie(UINT64 roleid)
{
	auto i = m_units.find(roleid);
	if (i == m_units.end())
	{
		return false;
	}
	i->second.state = VS_ROLE_DIE;
	return true;
}

VsStateType SceneCustomPk::IsAllLoad()
{
	if (m_units.empty())
	{
		return VS_STATE_DUMMY;
	}
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		if (i->second.state == VS_ROLE_WAITLOAD)	
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}

void SceneCustomPk::SetResult(UINT64 roleid, KKSG::PkResultType type)
{
	auto i = m_units.find(roleid);
	if (i != m_units.end())
	{
		i->second.result = type;
	}
}

void SceneCustomPk::ResultSystem(Role* role11, Role* role22)
{
	// to ms
	PtcG2M_ReportCustomBattleResult report11;
	report11.m_Data.set_roleid(role11->GetID());
	report11.m_Data.set_iswin(m_rolewin == role11->GetID());
	*report11.m_Data.mutable_battle() = m_battle;
	role11->SendToMS(report11);

	PtcG2M_ReportCustomBattleResult report22;
	report22.m_Data.set_roleid(role22->GetID());
	report22.m_Data.set_iswin(m_rolewin == role22->GetID());
	*report22.m_Data.mutable_battle() = m_battle;
	role22->SendToMS(report22);
}

void SceneCustomPk::ResultCustom(UINT64 role11, UINT64 role22)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		///> 跨服发给world
		RpcG2K_CustomBattleResultWorld* rpc = RpcG2K_CustomBattleResultWorld::CreateRpc();
		rpc->m_oArg.set_uid(m_battle.battleuid());
		rpc->m_oArg.set_sceneuid(m_scene->GetSceneID());
		rpc->m_oArg.add_roleida(role11);
		if (IsWin(role11))
		{
			rpc->m_oArg.add_rolewin(role11);
		}
		rpc->m_oArg.add_roleidb(role22);
		if (IsWin(role22))
		{
			rpc->m_oArg.add_rolewin(role22);
		}
		WorldLink::Instance()->SendTo(*rpc);
	}
	else
	{
		///> 本服发给master
		RpcG2M_CustomBattleResultMs* rpc = RpcG2M_CustomBattleResultMs::CreateRpc();
		rpc->m_oArg.set_uid(m_battle.battleuid());
		rpc->m_oArg.set_sceneuid(m_scene->GetSceneID());
		rpc->m_oArg.add_roleida(role11);
		if (IsWin(role11))
		{
			rpc->m_oArg.add_rolewin(role11);
		}
		rpc->m_oArg.add_roleidb(role22);
		if (IsWin(role22))
		{
			rpc->m_oArg.add_rolewin(role22);
		}
		MSLink::Instance()->SendTo(*rpc);
	}
}

void SceneCustomPk::Result()
{
	Role* role11 = NULL;
	Role* role22 = NULL;
	VsRoleState rolestate11 = VS_ROLE_QUIT;
	VsRoleState rolestate22 = VS_ROLE_QUIT;
	int count = 0;
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		Role* pRole = m_scene->FindSceneRole(i->first);
		if (NULL == pRole)
		{
			SSWarn<<"not find role:"<<i->first<<END;
		}
		else
		{
			if (0 == count)
			{
				role11 = pRole;
				rolestate11 = i->second.state;
			}
			else if (1 == count)
			{
				role22 = pRole;
				rolestate22 = i->second.state;
			}
			++count;
		}
	}

	if (NULL == role11 || NULL == role22)
	{
		///> 有玩家不在场景，不结算
		if (NULL != role11)
		{
			m_rolewin = role11->GetID();
		}
		else
		{
			SSWarn<<"role11 is not find"<<END;
		}
		if (NULL != role22)
		{
			m_rolewin = role22->GetID();
		}
		else
		{
			SSWarn<<"role22 is not find"<<END;
		}
		m_isonequit = true;
		return;
	}
	SetResult(role11->GetID(), KKSG::PkResult_Lose);
	SetResult(role22->GetID(), KKSG::PkResult_Lose);
	if (VS_ROLE_LOADED == rolestate11 && VS_ROLE_LOADED == rolestate22)
	{
		if(role11->GetPreciseHpPercent() > role22->GetPreciseHpPercent())
		{
			m_rolewin = role11->GetID();
			SetResult(role11->GetID(), KKSG::PkResult_Win);
		}
		else if(role11->GetPreciseHpPercent() < role22->GetPreciseHpPercent())
		{
			m_rolewin = role22->GetID();
			SetResult(role22->GetID(), KKSG::PkResult_Win);
		}
		else if(role11->GetAttr(BASIC_CurrentHP) > role22->GetAttr(BASIC_CurrentHP))
		{
			m_rolewin = role11->GetID();
			SetResult(role11->GetID(), KKSG::PkResult_Win);
		}
		else
		{
			m_rolewin = role22->GetID();
			SetResult(role22->GetID(), KKSG::PkResult_Win);
		}
	}
	else 
	{
		if (VS_ROLE_LOADED == rolestate11)
		{
			m_rolewin = role11->GetID();
			SetResult(role11->GetID(), KKSG::PkResult_Win);
		}
		else if (VS_ROLE_LOADED == rolestate22)
		{
			m_rolewin = role22->GetID();
			SetResult(role22->GetID(), KKSG::PkResult_Win);
		}
	}

	SSInfo<<"roleida:"<<role11->GetID()<<" battleid:"<<m_battle.battleuid()<<" is system:"<<m_battle.issystem()<<" iswin:"<<(m_rolewin == role11->GetID())<<END;
	SSInfo<<"roleidb:"<<role22->GetID()<<" battleid:"<<m_battle.battleuid()<<" is system:"<<m_battle.issystem()<<" iswin:"<<(m_rolewin == role22->GetID())<<END;

	if (m_battle.issystem())
	{
		ResultSystem(role11, role22);
	}
	else
	{
		ResultCustom(role11->GetID(), role22->GetID());
	}

	DoTxLog(role11, role22);
}

bool SceneCustomPk::CheckEnd()
{
	for (auto i = m_units.begin(); i != m_units.end(); ++i)
	{
		if (VS_ROLE_LOADED != i->second.state)	
		{
			return true;
		}
	}
	return false;
}

VsStateType SceneCustomPk::OnLoadTimeOut()
{
	return VS_STATE_END;
}

void SceneCustomPk::OnStateEnd()
{
	GetScene()->SetDontDestory(false);
	SetEnd();
	Result();
}

VsStateType SceneCustomPk::CheckFightEnd()
{
	if(CheckEnd())
	{
		return VS_STATE_END;
	}
	if (GetStateStartTime() +  GetGlobalConfig().PkUpdateTime <= time(NULL))
	{
		return VS_STATE_END;
	}
	return VS_STATE_DUMMY;
}

VsStateType SceneCustomPk::CheckP321End()
{
	if(CheckEnd())
	{
		return VS_STATE_END;
	}
	return SceneVsBase::CheckP321End();
}

std::string SceneCustomPk::GetRoleName(UINT64 roleID)
{
	if (m_infoa.roleid() == roleID)
	{
		return m_infoa.rolename();
	}
	if (m_infob.roleid() == roleID)
	{
		return m_infob.rolename();
	}
	return "";
}

void SceneCustomPk::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(auto it = m_units.begin(); it != m_units.end(); ++it)
	{
		UINT64 roleID = it->first;
		if(Unit::GetUnitType(roleID) != KKSG::Category_Role)
		{
			continue;
		}

		if(VS_ROLE_LOADED == (it->second).state)
		{
			Role* pRole = GetRole(roleID);
			if(pRole)
			{
				loaded.push_back(pRole);
			}
		}
		else if(VS_ROLE_WAITLOAD == (it->second).state)
		{
			unloaded.push_back(GetRoleName(roleID));
		}
	}
}

void SceneCustomPk::DoTxLog(Role* role11, Role* role22)
{
	if (NULL == role11 || NULL == role22)
	{
		return;
	}

	m_logger.PushRole(role11);
	m_logger.PushRole(role22);

	if (m_rolewin == role11->GetID())
	{
		role11->OnSceneComplete(GetScene());
		role22->OnSceneFailed(GetScene());
	}
	else
	{
		role22->OnSceneComplete(GetScene());
		role11->OnSceneFailed(GetScene());
	}
}
