#include "pch.h"
#include "scenesurvive.h"
#include "globalconfig.h"
#include "scenebase.h"
#include "unit/combatstatistics.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scenevsbase.h"
#include "scene/scene.h"
//#include "survive/ptcg2c_survivefightntf.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bagtransition.h"
#include "survive/rpcg2m_survivesynpointg2m.h"
//#include "survive/ptcg2c_surviveresultntf.h"
//#include "survive/ptcg2c_survivekillntf.h"

SceneSurvive::SceneSurvive(Scene* scene)
	:SceneVsBase(scene)
{
	m_lastSynTime = time(NULL);
}

SceneSurvive::~SceneSurvive()
{

}

bool SceneSurvive::Init(const KKSG::CreateBattleParam& roParam)
{
	m_data = roParam.kmatchfightdata();
	InfoPrint();
	InitState();
	return true;
}

void SceneSurvive::InitState()
{
	m_statemgr.Init(this, &VsAfterFight::GlobalVsAfterFight);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

UINT32 SceneSurvive::GetGroup(UINT64 roleid)
{
	return 1;//出生在同一点
}

UINT64 SceneSurvive::GetFightGroupUId(UINT64 roleid)
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		if(m_data.roles(i).roleid() == roleid)	
		{
			return UINT64(i + 1);
		}
	}
	SSError << " find role index failed roleid = " << roleid << END;
	return UINT64(m_data.roles_size() + 1);
}

void SceneSurvive::OnEnterAfterFight()
{

}

UINT32 SceneSurvive::GetReadyLeftTime()
{
	UINT32 now = UINT32(time(NULL));
	if(GetStateStartTime() + GetGlobalConfig().SurviveReadyTime > now)
	{
		return UINT32((GetStateStartTime() + GetGlobalConfig().SurviveReadyTime) - now);
	}
	return 0;
}

VsStateType SceneSurvive::CheckAfterFightEnd()
{
	if(0 == GetReadyLeftTime())
	{
		return VS_STATE_IN_FIGHT;
	}
	return VS_STATE_DUMMY;
}

void SceneSurvive::OnEnterFight()
{
	SSDebug << __FUNCTION__ << END;
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		UINT64 roleID = m_data.roles(i).roleid();
		Role* pRole = GetRole(roleID);
		if(NULL == pRole)
		{
			EndRole(roleID, KKSG::SURVIVE_LOAD_TIMEOUT);
		}
		else
		{
			//pRole->ChangeFightGroup(GetScene()->GetFightGroup(pRole));
		}
	}
}

VsStateType SceneSurvive::CheckFightEnd()
{
	if(GetRoleCount() == m_result.size())
	{
		return VS_STATE_END;
	}
	return VS_STATE_DUMMY;
}

void SceneSurvive::OnStateEnd()
{
	SSDebug << __FUNCTION__ << END;
	SetEnd();
	GetScene()->SetDontDestory(false);	
	InfoPrint();
}

void SceneSurvive::InfoPrint()
{
	for(int i = 0; i < m_data.roles_size(); ++i)
	{
		SSInfo << " roleid = " << m_data.roles(i).roleid() << " rolename = " << m_data.roles(i).pvpinfo().name() << END; 
	}
	for(auto it = m_result.begin(); it != m_result.end(); ++it)
	{
		InfoPrint(it->second);
	}
}

void SceneSurvive::InfoPrint(const KKSG::SurviveRoleFightData& data)
{
	SSInfo << " role = " << data.roleid() << " rank = " << data.myrank() << " endtype = " << data.endtype() << END;
	SSInfo << " addpoint = " << data.addpoint() << " killcount = " << data.killcount() << " killer = " << data.killedby() << END;
}

void SceneSurvive::Update()
{
	if(GetRoleCount() == m_result.size() + 1)
	{
		for(int i = 0; i < m_data.roles_size(); ++i)
		{
			UINT64 roleID = m_data.roles(i).roleid();
			if(NULL == GetResult(roleID))
			{
				EndRole(roleID, KKSG::SURVIVE_WIN);
				break;
			}
		}
	}

	m_statemgr.Update();
	
	if(GetCurrentStateType() != VS_STATE_END)
	{
		CheckStageSynn();
	}
}

bool SceneSurvive::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(NULL == pRole)
	{
		return false;
	}
	if(pRole->IsWatcher())
	{
		return false;
	}

	switch(event.type)
	{
	case VS_EVENT_ROLE_DIE:
		{
			EndRole(event.roleid, KKSG::SURVIVE_DIE);
		}
		break;
	case VS_EVENT_ROLE_QUIT:
		{
			if(GetCurrentStateType() == VS_STATE_IN_FIGHT)
			{
				EndRole(event.roleid, KKSG::SURVIVE_QUIT);
			}
		}
		break;
	case VS_EVENT_ROLE_LOADED:
		{
			StageNotify(pRole);
			if(NULL != GetResult(event.roleid))
			{
				MakePuppet(pRole, true);
				pRole->SetAttr(BASIC_CurrentHP, 0.0);
				ResultNotify(pRole);
			}
		}
		break;
	default:
		break;
	}

	return true;
}

void SceneSurvive::EndRole(UINT64 roleID, KKSG::SurviveFightEndType endType)
{
	if(NULL != GetResult(roleID))
	{
		return;
	}
	UINT32 allRoleCount = GetRoleCount();
	if(allRoleCount <= m_result.size())
	{
		SSError << " result count to much sceneid = " << GetScene()->GetSceneID() << END;
		return;
	}
	UINT32 myrank = allRoleCount - m_result.size();

	auto& ret = m_result[roleID];
	ret.set_roleid(roleID);
	ret.set_myrank(myrank);
	ret.set_endtype(endType);
	ret.set_allrank(allRoleCount);
	UINT32 liveTime = UINT32(time(NULL) - GetStateStartTime());
	ret.set_livetime(liveTime);
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
	if(pRole)
	{
		ret.set_killcount(pRole->Statistics().GetKillCount());
		if(KKSG::SURVIVE_DIE == endType)
		{
			Role* pKiller = RoleManager::Instance()->FindByRoleID(pRole->GetKillerID());
			if(pKiller)
			{
				ret.set_killedby(pKiller->GetName());
			}
		}

		const auto& PointConf = GetGlobalConfig().SurviveAddPoint;
		if(myrank <= PointConf.size())
		{
			ret.set_addpoint(int(PointConf[myrank-1]));
		}

		auto pRecord = pRole->Get<RoleMisc>();

		if(!pRecord->SurviveGetRewardFlag())
		{
			if(myrank <= GetGlobalConfig().SurviveRewardTopRank)
			{
				pRecord->SurviveAddTopCount();
			}
			if(pRecord->SurviveGetTopCount() >= GetGlobalConfig().SurviveRewardTopCount)
			{
				pRecord->SurviveSetRewardFlag();
				GiveReward(pRole);
			}
		}

		if(ret.addpoint() != 0)
		{
			pRecord->SurviveAddPoint(ret.addpoint());
			PointSynToMs(pRole, pRecord->SurviveGetPoint());
		}
	}

	InfoPrint(ret);

	if(pRole && (KKSG::SURVIVE_DIE == endType || KKSG::SURVIVE_WIN == endType))
	{
		ResultNotify(pRole);	
	}

	if(pRole && KKSG::SURVIVE_DIE == endType && !ret.killedby().empty())
	{
		KillNotify(ret.killedby(), pRole->GetName());
	}

	StageNotify(NULL);
}


const KKSG::SurviveRoleFightData* SceneSurvive::GetResult(UINT64 roleID)
{
	auto it = m_result.find(roleID);
	if(it != m_result.end())
	{
		return &it->second;
	}
	return NULL;
}

void SceneSurvive::ResultNotify(Role* pRole)
{
	//if(NULL == pRole)
	//{
	//	return;
	//}
	//PtcG2C_SurviveResultNtf ptc;
	//const auto pResult = GetResult(pRole->GetID());
	//if(NULL == pResult)
	//{
	//	return;
	//}
	//*ptc.m_Data.mutable_result() = *pResult;
	//pRole->Send(ptc);
	//SSDebug << " role = " << pRole->GetName() << " result notify " << " rank = " << pResult->myrank() << END;
}

UINT32 SceneSurvive::GetRoleCount()
{
	return m_data.roles_size();
}

void SceneSurvive::CheckStageSynn()
{
	time_t now = time(NULL);
	if(m_lastSynTime + 5 > now)
	{
		return;
	}
	m_lastSynTime = now;
	StageNotify(NULL);
}

void SceneSurvive::KillNotify(const std::string& killerName, const std::string& deadName)
{
	//SSDebug << " killer = " << killerName << " dead = " << deadName << END;
	//PtcG2C_SurviveKillNtf ptc;
	//ptc.m_Data.set_killername(killerName);
	//ptc.m_Data.set_deadname(deadName);
	//GetScene()->Broadcast(ptc);
}

void SceneSurvive::StageNotify(Role* pRole /*= NULL*/)
{
	//PtcG2C_SurviveFightNtf ptc;
	//FillFightStage(ptc.m_Data);
	//if(pRole)
	//{
	//	pRole->Send(ptc);
	//}
	//else
	//{
	//	GetScene()->Broadcast(ptc);
	//}
}

void SceneSurvive::FillFightStage(KKSG::SurviveFightSyn& data)
{
	data.set_stage(GetFightStage());
	if(data.stage() == KKSG::SURVIVE_STAGE_READY)	
	{
		data.set_lefttime(GetReadyLeftTime());
	}
	else
	{
		data.set_starttime(UINT32(time(NULL) - GetStateStartTime()));
	}
	if(GetRoleCount() > m_result.size())
	{
		data.set_liverolecount(GetRoleCount() - m_result.size());
	}
	else
	{
		data.set_liverolecount(1);
	}
}

KKSG::SurviveFightStage SceneSurvive::GetFightStage()
{
	if(GetCurrentStateType() == VS_STATE_AFTER_FIGHT)
	{
		return KKSG::SURVIVE_STAGE_READY;
	}
	else
	{
		return KKSG::SURVIVE_STAGE_FIGHT;
	}
}

void SceneSurvive::GiveReward(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Survive, ItemFlow_Survive_DayReward);
	const auto& Rewards = GetGlobalConfig().SurviveRewardItems;
	for(size_t i = 0; i < Rewards.size(); ++i)
	{
		give.GiveItem(Rewards[i][0], Rewards[i][1], true);
	}
	give.NotifyClient();
}

void SceneSurvive::PointSynToMs(Role* pRole, UINT32 point)
{
	if(NULL == pRole)
	{
		return;
	}
	RpcG2M_SurviveSynPointG2M* rpc = RpcG2M_SurviveSynPointG2M::CreateRpc();
	auto& data = *rpc->m_oArg.add_roles();
	data.set_roleid(pRole->GetID());
	data.set_rolename(pRole->GetName());
	data.set_point(point);
	data.set_serverid(pRole->GetFromServerId());
	pRole->SendToMS(*rpc);
}

void SceneSurvive::OnRoleReconnect(Role* role)
{
	if(NULL == role)
	{
		return;
	}
	if(NULL == GetResult(role->GetID()))
	{
		return;
	}
	ResultNotify(role);
}
