#include "pch.h"
#include "scenecustompktwo.h"
#include "pk/ptcg2c_pkroleinfontf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "scenebase.h"
#include "scene.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/pkmgr.h"
#include "utility/tlogger.h"
#include "gamelogic/robotconfig.h"
#include "gamelogic/designationMgr.h"
#include "custombattle/rpcg2k_custombattleresultworld.h"
#include "custombattle/rpcg2m_custombattleresultms.h"
#include "network/worldlink.h"
#include "network/mslink.h"

SceneCustomPkTwo::SceneCustomPkTwo(Scene* scene)
	:SceneVsBase(scene)
{

}

SceneCustomPkTwo::~SceneCustomPkTwo()
{

}

bool SceneCustomPkTwo::Init(const KKSG::CreateBattleParam& roData)
{
	m_data = roData.kmatchfightdata();

	InfoPrint();

	if(m_data.roles_size() != GetVsCount() * 2)
	{
		return false;
	}

	InitDRoleData();

	InitState();

	m_logger.Init(m_data.uid(), KKSG::CustomBattle_PKTwo_Normal, IsFairMode(), false);

	stringstream ss;
	for (int i = 0; i < m_data.roles_size(); ++i)
	{
		ss << m_data.roles(i).roleid()<<", ";
	}
	SSInfo<<"uid:"<<m_data.uid()<<" isfair:"<<IsFairMode()<<" roles:"<<ss.str().c_str()<<END;

	m_strTransTag = TTag::Instance()->GetTagString();
	return true;
}

void SceneCustomPkTwo::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

void SceneCustomPkTwo::InitDRoleData()
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		const auto& rd = m_data.roles(i);
		auto& dd = m_roles[rd.roleid()];
		dd.roleID = rd.roleid();
		dd.state = VS_ROLE_WAITLOAD;
		if(i < (int)GetVsCount())
		{
			dd.group = 1;
		}
		else
		{
			dd.group = 2;
		}
	}
}

UINT32 SceneCustomPkTwo::GetGroup(UINT64 roleid)
{
	auto pData = GetDRoleData(roleid);
	if(pData)
	{
		return pData->group;
	}
	return 1;
}

UINT32 SceneCustomPkTwo::GetLoadWaitTime()
{
	return GetGlobalConfig().PkLoadWaitTime;
}

std::string SceneCustomPkTwo::GetRoleName(UINT64 roleID)
{
	auto pData = GetKKSGRoleData(roleID);
	if(pData)
	{
		return pData->rolebrief().rolename();
	}
	return "";
}

VsStateType SceneCustomPkTwo::IsAllLoad()
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		if(it->second.state == VS_ROLE_WAITLOAD)
		{
			return VS_STATE_DUMMY;
		}
	}
	return VS_STATE_PREPARE321;
}

void SceneCustomPkTwo::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
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

void SceneCustomPkTwo::InfoPrint()
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		SSInfo << " role = " << m_data.roles(i).roleid() << " server = " << m_data.roles(i).serverid() << END;
		SSInfo << " point = " << m_data.roles(i).pkrec().point() << END;
	}
}

void SceneCustomPkTwo::Update()
{
	m_statemgr.Update();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SceneCustomPkTwo::OnEvent(const VsEvent& event)
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

	auto pData = GetDRoleData(event.roleid);
	if(NULL == pData)
	{
		//SSError << " find droledata failed roleid = " << event.roleid << END;
		return false;
	}

	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			pData->state = VS_ROLE_LOADED;
		}
		break;
	case VS_EVENT_ROLE_QUIT:
	case VS_EVENT_ROLE_LOADING_QUIT:
		{
			pData->state = VS_ROLE_QUIT;
			OnRoleLeave(event.roleid);
		}
		break;
	case VS_EVENT_ROLE_DIE:
		{
			pData->state = VS_ROLE_DIE;
		}
		break;
	case VS_EVENT_ROLE_REVIVE:
		{
			if (VS_ROLE_DIE == pData->state)
			{
				pData->state = VS_ROLE_LOADED;
			}
		}
		break;
	default:
		break;
	}

	return true;
}

KKSG::KMatchRole* SceneCustomPkTwo::GetKKSGRoleData(UINT64 roleID)
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		if(m_data.roles(i).roleid() == roleID)
		{
			return m_data.mutable_roles(i);
		}
	}
	return NULL;
}

DPkRoleData* SceneCustomPkTwo::GetDRoleData(UINT64 roleID)
{
	auto it = m_roles.find(roleID);
	if(it != m_roles.end())
	{
		return &it->second;
	}
	return NULL;
}

VsStateType SceneCustomPkTwo::CheckFightEnd()
{
	UINT32 winGroup = 0;	
	UINT32 score1 = 0;
	UINT32 score2 = 0;
	for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		auto& data = (it->second);
		if(data.state != VS_ROLE_LOADED)
		{
			if(data.group == 1)
			{
				score2 ++;
			}
			else if(data.group == 2)
			{
				score1 ++;
			}
		}
	}
	if(score1 == GetVsCount())
	{
		winGroup = 1;
	}
	else if(score2 == GetVsCount())
	{
		winGroup = 2;
	}

	float hpscore1 = 0.0f;
	float hpscore2 = 0.0f;
	if(0 == winGroup && GetStateStartTime() +  GetGlobalConfig().PkTwoUpdateTime <= time(NULL))
	{
		if(score1 > score2)
		{
			winGroup = 1;
		}
		else if(score1 < score2)
		{
			winGroup = 2;
		}
		else
		{
			///> 算血量
			for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
			{
				auto& data = (it->second);
				if(data.state == VS_ROLE_LOADED)
				{
					if(data.group == 1)
					{
						Role* role = m_scene->FindSceneRole(data.roleID);
						if (NULL != role)
						{
							hpscore1 += role->GetHpPercent();
						}
					}
					else if(data.group == 2)
					{
						Role* role = m_scene->FindSceneRole(data.roleID);
						if (NULL != role)
						{
							hpscore2 += role->GetHpPercent();
						}
					}
				}
				if (hpscore1 > hpscore2)
				{
					winGroup = 1;
				}
				else if (hpscore2 > hpscore1)
				{
					winGroup = 2;
				}
				else
				{
					winGroup = 3;
				}
			}
		}
	}

	if(0 != winGroup)
	{
		for (auto it = m_roles.begin(); it != m_roles.end(); ++it)
		{
			Role* role = m_scene->FindSceneRole(it->first);
			if (NULL != role)
			{
				m_logger.PushRole(role);
			}

			auto& data = (it->second);
			if(data.isdone)//提前结算了
			{
				continue;
			}
			data.result = (data.group == winGroup) ? KKSG::PkResult_Win : KKSG::PkResult_Lose;
		}
		return VS_STATE_END;
	}
	return VS_STATE_DUMMY;
}

void SceneCustomPkTwo::OnStateEnd()
{
	GetScene()->SetDontDestory(false);
	SetEnd();

	ResultCustom();
}

void SceneCustomPkTwo::ResultCustom()
{
	std::stringstream ss;
	ss<<"uid:"<<m_data.uid()<<" sceneuid:"<<m_scene->GetSceneID()<<" ";
	if (GSConfig::Instance()->IsCrossGS())
	{
		///> 跨服发给world
		RpcG2K_CustomBattleResultWorld* rpc = RpcG2K_CustomBattleResultWorld::CreateRpc();
		rpc->m_oArg.set_sceneuid(m_scene->GetSceneID());
		rpc->m_oArg.set_uid(m_data.uid());
		for (auto i = m_roles.begin(); i != m_roles.end(); ++i)
		{
			if (i->second.group == 1)
			{
				rpc->m_oArg.add_roleida(i->first);
			}
			else if (i->second.group == 2)
			{
				rpc->m_oArg.add_roleidb(i->first);
			}
			if (i->second.result == KKSG::PkResult_Win)
			{
				rpc->m_oArg.add_rolewin(i->first);
				ss<<i->first<<" win, ";
			}
			else
			{
				ss<<i->first<<" lose, ";
			}
		}
		WorldLink::Instance()->SendTo(*rpc);
	}
	else
	{
		///> 本服发给master
		RpcG2M_CustomBattleResultMs* rpc = RpcG2M_CustomBattleResultMs::CreateRpc();
		rpc->m_oArg.set_sceneuid(m_scene->GetSceneID());
		rpc->m_oArg.set_uid(m_data.uid());
		for (auto i = m_roles.begin(); i != m_roles.end(); ++i)
		{
			if (i->second.group == 1)
			{
				rpc->m_oArg.add_roleida(i->first);
			}
			else if (i->second.group == 2)
			{
				rpc->m_oArg.add_roleidb(i->first);
			}
			if (i->second.result == KKSG::PkResult_Win)
			{
				rpc->m_oArg.add_rolewin(i->first);
				ss<<i->first<<" win, ";
			}
			else
			{
				ss<<i->first<<" lose, ";
			}
		}
		MSLink::Instance()->SendTo(*rpc);
	}
	LogInfo("%s", ss.str().c_str());
}

void SceneCustomPkTwo::OnRoleLeave(UINT64 roleid)
{
	auto pData = GetDRoleData(roleid);
	if(NULL == pData)
	{
		return;
	}
	if(pData->isdone)
	{
		return;
	}
	pData->isdone = true;
	pData->result = KKSG::PkResult_Lose;//提前退出判负
}
