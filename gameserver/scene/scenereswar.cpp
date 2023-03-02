#include "pch.h"
#include "scenereswar.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>
#include "pb/project.pb.h"
#include "unit/rolemanager.h"
#include "reswar/ptcg2m_reswarteamresultntf.h"
#include "reswar/ptcg2c_reswarbattledatantf.h"
#include "scene/ptcg2c_skycitybattledatantf.h"
#include "reswar/ptcg2c_reswarteamresone.h"
//#include "reswar/ptcg2c_reswarestimatentf.h"
#include "mslink.h"
#include "vshandler.h"
#include "unit/dummyrole.h"
#include "unit/combatstatistics.h"
#include "utility/tlogger.h"
#include "scene/attributepk.h"

SceneResWar::SceneResWar( Scene* pScene ):SceneVsBase(pScene),m_datamgr(pScene)
{
	m_lastSynTime = time(NULL);
	m_saveTime = 0;
	m_fightCount = 0;
	m_nSelfBuffTime = 0;
	m_nTargetBuffTime = 0;
	m_nAvgPPT = 0;
}

bool SceneResWar::Init(const KKSG::CreateBattleParam& roParam)
{
	m_oResWar = roParam.reswar();
	m_oResWar.selfaddtime();
	UINT32 nAvg = 0;
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		const auto& data = m_oResWar.self(i);
		m_datamgr.AddUnit(data.uid(), data.teamid());
		m_unitids.push_back(data.uid());
		m_mapPPT[data.uid()]=data.ppt();
		nAvg += data.ppt();
		//total ppt
		SSInfo << __FUNCTION__ << " teamid = " << data.teamid() << " uid = " << data.uid() << " name = " << data.name() << END;
	}
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		const auto& data = m_oResWar.target(i);
		m_datamgr.AddUnit(data.uid(), data.teamid());	
		m_unitids.push_back(data.uid());
		m_mapPPT[data.uid()]=data.ppt();
		nAvg += data.ppt();
		SSInfo << __FUNCTION__ << " teamid = " << data.teamid() << " uid = " << data.uid() << " name = " << data.name() << END;
	}
	InitState();
	if (m_oResWar.self_size() || m_oResWar.target_size())
	{
		m_nAvgPPT = nAvg/(m_oResWar.self_size()+m_oResWar.target_size());
	}
	return true;
}


void SceneResWar::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		UINT64 unitID = m_unitids[i];	
		if(KKSG::Category_Role != Unit::GetUnitType(unitID))
		{
			continue;
		}
		if(m_loadmgr.HaveRole(unitID))
		{
			Role* pRole = GetRole(unitID);
			if(pRole)
			{
				loaded.push_back(pRole);
			}
		}
		else
		{
			unloaded.push_back(GetRoleName(unitID));
		}
	}
}


std::string SceneResWar::GetRoleName(UINT64 roleID)
{
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		const auto& info = m_oResWar.self(i);	
		if(info.uid() == roleID)
		{
			return info.name();
		}
	}
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		const auto& info = m_oResWar.target(i);	
		if(info.uid() == roleID)
		{
			return info.name();
		}
	}
	return "";
}

UINT32 SceneResWar::GetRolePPT(UINT64 nRoleID)
{
	auto iter = m_mapPPT.find(nRoleID);
	if (iter!=m_mapPPT.end())
	{
		return m_mapPPT[nRoleID];
	}
	return 0;
}

UINT32 SceneResWar::GetAvgPPT()
{
	return m_nAvgPPT;
}

void SceneResWar::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

void SceneResWar::OnEnterScene(Role* role)
{
	UINT32 nPPT = GetRolePPT(role->GetID());
	AttributePkMgr::Instance()->AdjustAttrs(role,(KKSG::SceneType)(GetScene()->GetSceneType()),nPPT,m_nAvgPPT);
}

bool SceneResWar::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(!pRole) return false;
	if(pRole->IsWatcher()) return false;
	SSDebug << __FUNCTION__ << " rolename = " << pRole->GetName() << " event = " << event.type << END;
	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			m_loadmgr.AddRole(event.roleid);
			if(IsInWaitLoaded())
				MakePuppet(pRole, true);//不让动
		}
		break;
	case VS_EVENT_ROLE_QUIT:
		{
			m_loadmgr.DelRole(event.roleid);
			m_datamgr.OnUnitCombatStop(event.roleid);
			AllInfoToClient();//比分变了
		}
		break;
	case VS_EVENT_ROLE_DIE:
		{
			AllInfoToClient();//比分变了
		}
		break;
	default:
		break;
	}

	return true;
}

void SceneResWar::OnStateEnd()
{
	m_datamgr.OnUnitCombatStop();
	EndSkills(m_unitids);
	AllInfoToClient();
	GameResultToClient();
	GameResultToMS();
	GetScene()->SetDontDestory(false);
}

void SceneResWar::Update()
{
	m_statemgr.Update();
	time_t now = time(NULL);
	if(m_lastSynTime + 3 < now)
	{
		m_lastSynTime = now;
		AllInfoToClient();
	}
	if(m_statemgr.GetCurrentState() && m_statemgr.GetCurrentState()->GetStateType() == VS_STATE_END)
	{
		SetEnd();
	}
}

void SceneResWar::DoTxLog(UINT64 roleid, UINT32 TeamID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
	if (!pRole) return;
	TResWarFlow oLog(pRole);
	oLog.m_iResult = TeamID==(UINT32)-1?-1:TeamID;
	oLog.Do();
}

void SceneResWar::GameResultToMS()
{
	PtcG2M_ResWarTeamResultNtf ptc;
	ptc.m_Data.set_group(GetSkyGroup());
	ptc.m_Data.set_winteamid(GetWinTeam());	
	SSInfo << __FUNCTION__ << " sceneid = " << GetScene()->GetSceneID() << " group = " << GetSkyGroup() << END;
	MSLink::Instance()->SendTo(ptc);
}

UINT32 SceneResWar::GetGroup(UINT64 roleid)
{
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		if(m_oResWar.self(i).uid() == roleid)
			return GetGroup11();
	}
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		if(m_oResWar.target(i).uid() == roleid)
			return GetGroup22();
	}
	return GetGroup11();
}


UINT64 SceneResWar::GetGuildID(UINT64 roleid)
{
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		if(m_oResWar.self(i).uid() == roleid)
			return m_oResWar.self(i).guildid();
	}
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		if(m_oResWar.target(i).uid() == roleid)
			return m_oResWar.target(i).guildid();
	}
	return 0;
}



UINT64 SceneResWar::GetGuildBuffTime(UINT64 nGuildID)
{
	if (m_oResWar.selfguildid()== nGuildID)
	{
		return m_oResWar.selfaddtime();
	}else if (m_oResWar.targetguildid()== nGuildID)
	{
		return m_oResWar.targetaddtime();
	}
	return 0;
}

UINT32 SceneResWar::GetTeamID(UINT64 roleid)
{
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		if(m_oResWar.self(i).uid() == roleid)
			return m_oResWar.self(i).teamid();
	}
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		if(m_oResWar.target(i).uid() == roleid)
			return m_oResWar.target(i).teamid();
	}
	return 0;
}

VsStateType SceneResWar::IsAllLoad()
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		if(!m_loadmgr.HaveRole(m_unitids[i]))
			return VS_STATE_DUMMY;
		else if(m_loadmgr.LoadedTime(m_unitids[i]) < GetGlobalConfig().SkyArenaStartAnimTime)
			return VS_STATE_DUMMY;
	}
	return VS_STATE_PREPARE321;
}

void SceneResWar::OnEnterP321()
{
	if(GetFightCount() > 0)	
		ResetPosition();
}

UINT32 SceneResWar::GetTeam11()
{
	for(int i = 0; i < m_oResWar.self_size(); ++i)
		return m_oResWar.self(i).teamid();
	SSError << " getteam11 = null sceneid = " << GetScene()->GetSceneID() << END;
	return 1;
}

UINT32 SceneResWar::GetTeam22()
{
	for(int i = 0; i < m_oResWar.target_size(); ++i)
		return m_oResWar.target(i).teamid();
	SSError << " getteam22 = null sceneid = " << GetScene()->GetSceneID() << END;
	return 2;
}

UINT32 SceneResWar::GetGroup11(){ return 1; }

UINT32 SceneResWar::GetGroup22(){  return 2;}

void SceneResWar::InitDataToClient(Role* pRole)
{
	PtcG2C_ResWarTeamResOne ptc;
	FillInitData(ptc.m_Data);
	if(pRole)
		pRole->Send(ptc);
	else
		GetScene()->Broadcast(ptc);
}

void SceneResWar::AllInfoToClient(Role* pRole)
{
	PtcG2C_ResWarBattleDataNtf ptc;
	FillAllInfo(ptc.m_Data);	
	if(pRole)
		pRole->Send(ptc);
	else
		GetScene()->Broadcast(ptc);
}

void SceneResWar::FillNowData(KKSG::ResWarRes& roRes)
{
	FillInitData(*roRes.mutable_baseinfo());
	FillAllInfo(*roRes.mutable_allinfo());
}

void SceneResWar::FillInitData(KKSG::ResWarAllTeamBaseInfo& data)
{
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		const auto& tmp = m_oResWar.self(i);
		data.add_info()->CopyFrom(tmp);	
	}
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		const auto& tmp = m_oResWar.target(i);
		data.add_info()->CopyFrom(tmp);	
	}
	for(int i = 0; i < data.info_size(); ++i)
	{
		SSDebug << " name = " << data.info(i).name() << " lev = " << data.info(i).lv() << " profresion = " << data.info(i).job() << END;
	}
}

void SceneResWar::FillAllInfo(KKSG::ResWarAllInfo& data)
{
	UINT32 teamid11 = GetTeam11();
	UINT32 teamid22 = GetTeam22();
	auto group11 = data.add_groupdata();
	group11->set_teamid(teamid11);
	group11->set_killcount(GetTeamScore(teamid11));
	group11->set_totaldamage(m_datamgr.GetGroupTotalDamage(teamid11));
	auto group22 = data.add_groupdata();
	group22->set_teamid(teamid22);
	group22->set_killcount(GetTeamScore(teamid22));
	group22->set_totaldamage(m_datamgr.GetGroupTotalDamage(teamid22));
	data.set_timetype(GetTimeType());
	data.set_lefttime(GetStateLeftTime());
}

void SceneResWar::GameResultToClient()
{
	/*PtcG2C_ResWarEstimateNtf ptc;
	ptc.m_Data.set_winteamid(GetWinTeam());
	SSDebug << __FUNCTION__ << " winteam = " << GetWinTeam() << END;
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		UINT64 unitid = m_unitids[i];
		auto info = ptc.m_Data.add_info();
		info->set_roleid(unitid);
		info->set_killer(m_datamgr.GetUnitKillCount(unitid));
		info->set_damage((UINT64)m_datamgr.GetUnitTotalDamage(unitid));
	}
	GetScene()->Broadcast(ptc);*/
}

KKSG::ResWarTimeType SceneResWar::GetTimeType()
{
	if(NULL == m_statemgr.GetCurrentState())
		return KKSG::ResWarNone;
	if(VS_STATE_IN_FIGHT == m_statemgr.GetCurrentState()->GetStateType())
		return KKSG::RaceTime;
	if(VS_STATE_END == m_statemgr.GetCurrentState()->GetStateType())
		return KKSG::EndTime;
	return KKSG::ResWarNone;
}

UINT32 SceneResWar::GetStateLeftTime()
{
	time_t now = time(NULL);
	switch(GetTimeType())
	{
	case KKSG::RaceTime:
		{
			UINT32 fightTime =  GetGlobalConfig().ResWarPVPTime;
			if(GetFightCount() > 0)
			{
				fightTime =  fightTime;
			}
			if(GetStateStartTime() + fightTime > now)
			{
				return UINT32(GetStateStartTime() + fightTime - now);
			}
			return 0;
		}
		break;
	case KKSG::EndTime:
		{
			if(GetStateStartTime() +  GetGlobalConfig().ResWarPVPTime > now)
			{
				return UINT32(GetStateStartTime() +  GetGlobalConfig().ResWarPVPTime- now);
			}
			return 0;
		}
		break;
	default:
		break;
	}
	return 0;
}

UINT32 SceneResWar::GetWinTeam()
{
	UINT32 killcount11 = m_datamgr.GetGroupKillCount(GetTeam11());
	UINT32 killcount22 = m_datamgr.GetGroupKillCount(GetTeam22());
	double totaldamage11 = m_datamgr.GetGroupTotalDamage(GetTeam11());
	double totaldamage22 = m_datamgr.GetGroupTotalDamage(GetTeam22());
	if(killcount11 > killcount22)
		return GetTeam11();
	if(killcount11 < killcount22)
		return GetTeam22();
	if(totaldamage11 > totaldamage22)
		return GetTeam11();
	if(totaldamage11 < totaldamage22)
		return GetTeam22();
	return 0;
}

void SceneResWar::OnEnterFight()
{
	AllInfoToClient();
}

VsStateType SceneResWar::CheckFightEnd()
{
	UINT32 lefttime = GetStateLeftTime();
	bool bAllDead = OneGroupAllDeadOrQuit();
	if(lefttime == 0 || bAllDead)
	{
		m_saveTime += lefttime;
		if(GetFightCount() > 0)
			return VS_STATE_END;
		else
			return VS_STATE_AFTER_FIGHT;
	}
	return VS_STATE_DUMMY;
}

void SceneResWar::OnEnterAfterFight()
{
	AllInfoToClient();
	AddFightCount();
}

VsStateType SceneResWar::CheckAfterFightEnd()
{
	if(0 == GetStateLeftTime())
	{
		return VS_STATE_END;
	}
	return VS_STATE_DUMMY;
}

void SceneResWar::ResetPosition(bool rest)
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		TranInitPosition(m_unitids[i]);		
	}
}


bool SceneResWar::OneGroupAllDeadOrQuit()
{
	UINT32 have = 0;
	for(int i = 0; i < m_oResWar.self_size(); ++i)
	{
		UINT64 unitID = m_oResWar.self(i).uid();
		if(!DeadOrQuit(unitID))
			have++;
	}
	if(0 == have) 
		return true;
	have = 0;
	for(int i = 0; i < m_oResWar.target_size(); ++i)
	{
		UINT64 unitID = m_oResWar.target(i).uid();
		if(!DeadOrQuit(unitID))
			have++;
	}
	if(0 == have) 
		return true;
	return false;
}

bool SceneResWar::DeadOrQuit(UINT64 unitID)
{
	if(!m_loadmgr.HaveRole(unitID))
	{
		return true;
	}
	Unit* pUnit = GetUnit(unitID);
	if(NULL == pUnit)
	{
		return true;
	}
	if(pUnit->IsDead())
	{
		return true;
	}
	return false;
}

void SceneResWar::BeforeDoEnterScene(Role* pRole)
{
	SSDebug << __FUNCTION__ << " name = " << pRole->GetName() << END;
	InitDataToClient(pRole);	
}


void SceneResWar::GetSameGroupRoles(Role* pRole, std::vector<Role*>& roles)
{
	if(!pRole) return;
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		Unit* pUnit = GetUnit(m_unitids[i]);	
		if(pUnit && pUnit->IsRole() && pUnit->GetFightGroup() == pRole->GetFightGroup())
		{
			roles.push_back((Role*)(pUnit));
		}
	}
}

UINT32 SceneResWar::GetTeamScore(UINT32 teamid)
{
	UINT32 score = m_datamgr.GetGroupKillCount(teamid);
	auto& others = (teamid == GetTeam11())?m_oResWar.target():m_oResWar.self();
	int PkCount = 3;
	if(PkCount > others.size())//未参与者
	{
		score += (PkCount - others.size());
	}
	if(!IsInWaitLoaded())
	{
		for(int i = 0; i < others.size(); ++i)
		{
			const auto& data = others.Get(i);
			if(!m_loadmgr.HaveRole(data.uid()))//退出者
			{
				++score;
			}
		}
	}
	return score;
}
