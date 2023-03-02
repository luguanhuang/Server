#include "pch.h"
#include "sceneskycity.h"
#include "unit/enemy.h"
#include "scene/scene.h"
#include "foreach.h"
#include "unit/role.h"
#include <vector>
#include "pb/project.pb.h"
#include "unit/rolemanager.h"
#include "define/skycitydef.h"
#include "skycity/ptcg2m_skycityteamresultntf.h"
#include "mslink.h"
#include "vshandler.h"
#include "scene/ptcg2c_skycitybattledatantf.h"
#include "skycity/ptcg2c_skycityteamres.h"
#include "skycity/ptcg2c_skycityestimateres.h"
#include "unit/dummyrole.h"
#include "skycitycfg/skycityconfig.h"
#include "unit/combatstatistics.h"
#include "utility/tlogger.h"
#include "gamelogic/robotconfig.h"
#include "define/tlogenum.h"
#include "gamelogic/levelsealMgr.h"

SceneSkyCity::SceneSkyCity( Scene* pScene ):SceneVsBase(pScene),m_datamgr(pScene)
{
	m_lastSynTime = time(NULL);
	m_saveTime = 0;
	m_fightCount = 0;
	SetCheckReviveFlag(false);
}

bool SceneSkyCity::Init(const KKSG::CreateBattleParam& roParam)
{
	m_oSkyCity = roParam.skycity();	
			
	CheckInitRobot();

	SSInfo << __FUNCTION__ << " sceneid = " << GetScene()->GetSceneID() << " group = " << GetSkyGroup() << " ngame = " << GetGameNum() << " floor = " << GetFloor() << END;

	if(0 == m_oSkyCity.self_size() || 0 == m_oSkyCity.target_size())
	{
		SSError << __FUNCTION__ << " sceneid = " << GetScene()->GetSceneID() << " self_size = " << m_oSkyCity.self_size() << " target_size = " << m_oSkyCity.target_size() << END;
		return false;
	}

	for(int i = 0; i < m_oSkyCity.self_size(); ++i)
	{
		const auto& data = m_oSkyCity.self(i);
		m_datamgr.AddUnit(data.uid(), data.teamid());	

		m_unitids.push_back(data.uid());

		SSInfo << __FUNCTION__ << " teamid = " << data.teamid() << " uid = " << data.uid() << " name = " << data.name() << END;
	}

	for(int i = 0; i < m_oSkyCity.target_size(); ++i)
	{
		const auto& data = m_oSkyCity.target(i);
		m_datamgr.AddUnit(data.uid(), data.teamid());	

		m_unitids.push_back(data.uid());

		SSInfo << __FUNCTION__ << " teamid = " << data.teamid() << " uid = " << data.uid() << " name = " << data.name() << END;
	}

	InitState();					

	return true;
}

void SceneSkyCity::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

bool SceneSkyCity::OnEvent(const VsEvent& event)
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

	SSDebug << __FUNCTION__ << " rolename = " << pRole->GetName() << " event = " << event.type << END;

	switch(event.type)
	{
	case VS_EVENT_ROLE_LOADED:
		{
			m_loadmgr.AddRole(event.roleid);
			//InitDataToClient(pRole);

			//if(IsInWaitLoaded())
			//{
			//	MakePuppet(pRole, true);//不让动
			//}
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
	case VS_EVENT_ROLE_REVIVE:
		{
			;//no need 
		}
		break;
	default:
		break;
	}

	return true;
}

void SceneSkyCity::OnStateEnd()
{
	if(!IsLastGame())//不是最后一场才可以
	{
		//CheckRevive();
		SetCheckReviveFlag(false);
		ResetPosition(true);
	}

	m_datamgr.OnUnitCombatStop();
	EndSkills(m_unitids);

	AllInfoToClient();
	
	GameResultToClient();
	GameResultToMS();

	GetScene()->SetDontDestory(false);
	//SetEnd(); 故意不结束 利用玩家为空在结束场景
}

void SceneSkyCity::Update()
{
	m_statemgr.Update();

	time_t now = time(NULL);
	if(m_lastSynTime + 3 < now)
	{
		m_lastSynTime = now;
		AllInfoToClient();
	}

	if(IsLastGame() && m_statemgr.GetCurrentState() 
		&& m_statemgr.GetCurrentState()->GetStateType() == VS_STATE_END
		&& GetStateStartTime() +  GetGlobalConfig().SkyCityLastResultDelay < now)//wait
	{
		MakeAllStatistics();
		SetEnd();
	}

	if(m_statemgr.GetCurrentState() && 
		(m_statemgr.GetCurrentState()->GetStateType() == VS_STATE_AFTER_FIGHT 
		|| m_statemgr.GetCurrentState()->GetStateType() == VS_STATE_END))
	{
		if(!CheckReviveFlag() && GetStateStartTime() + 3 <= now)
		{
			SetCheckReviveFlag(true);
			CheckRevive();
		}
	}

}

void SceneSkyCity::DoTxLog(UINT64 roleid, UINT32 TeamID, std::string& trans)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
	if (NULL == pRole)
	{
		return;
	}
	TSkyCityFlow oLog(pRole);
	oLog.SetTransTag(trans);
	if (TeamID == 0)
	{
		oLog.m_iResult = TX_PKResult_Draw;
	}
	else if (TeamID == GetGroup(roleid))
	{
		oLog.m_iResult = TX_PkResult_Win;
	}
	else
	{
		oLog.m_iResult = TX_PKResult_Lost;
	}
	oLog.m_TeamID = GetGroup(roleid);
	oLog.Do();
}

void SceneSkyCity::GameResultToMS()
{
	PtcG2M_SkyCityTeamResultNtf ptc;
	ptc.m_Data.set_group(GetSkyGroup());
	ptc.m_Data.set_winteamid(GetWinTeam());	
	SSInfo << __FUNCTION__ << " sceneid = " << GetScene()->GetSceneID() << " group = " << GetSkyGroup() << " ngame = " << GetGameNum() << " floor = " << GetFloor() << END;

	std::string strTrans = TTag::Instance()->GetTagString();//同一次战斗用同一个流水号;
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		if(KKSG::Category_Role == Unit::GetUnitType(m_unitids[i]))		
		{
			UINT64 roleid = m_unitids[i];
			auto& data = *ptc.m_Data.add_rolestatistics();
			data.set_roleid(roleid);
			data.set_killcount(m_datamgr.GetUnitKillCount(roleid));
			data.set_deadcount(m_datamgr.GetUnitDeadCount(roleid));
			data.set_totaldamage(m_datamgr.GetUnitTotalDamage(roleid));

			DoTxLog(roleid, GetWinTeam(), strTrans);
			//SSInfo << __FUNCTION__ << " roleid = " << data.roleid() << " killcount = " << data.killcount() << " deadcount = " << data.deadcount() << " totaldamage = " << data.totaldamage() << END;
		}
	}

	MSLink::Instance()->SendTo(ptc);
}

UINT32 SceneSkyCity::GetGroup(UINT64 roleid)
{
	for(int i = 0; i < m_oSkyCity.self_size(); ++i)
	{
		if(m_oSkyCity.self(i).uid() == roleid)
		{
			return GetGroup11();
		}
	}
	for(int i = 0; i < m_oSkyCity.target_size(); ++i)
	{
		if(m_oSkyCity.target(i).uid() == roleid)
		{
			return GetGroup22();
		}
	}
	return GetGroup11();
}

VsStateType SceneSkyCity::IsAllLoad()
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		if(!m_loadmgr.HaveRole(m_unitids[i]))
		{
			return VS_STATE_DUMMY;
		}
		else if(m_loadmgr.LoadedTime(m_unitids[i]) < GetGlobalConfig().SkyArenaStartAnimTime)
		{
			return VS_STATE_DUMMY;
		}

	}
	return VS_STATE_PREPARE321;
}

void SceneSkyCity::OnEnterP321()
{
	if(GetFightCount() > 0)	
	{
		ResetPosition();
	}
}

UINT32 SceneSkyCity::GetTeam11()
{
	for(int i = 0; i < m_oSkyCity.self_size(); ++i)
	{
		return m_oSkyCity.self(i).teamid();
	}
	SSError << " getteam11 = null sceneid = " << GetScene()->GetSceneID() << END;
	return 1;
}

UINT32 SceneSkyCity::GetTeam22()
{
	for(int i = 0; i < m_oSkyCity.target_size(); ++i)
	{
		return m_oSkyCity.target(i).teamid();
	}
	SSError << " getteam22 = null sceneid = " << GetScene()->GetSceneID() << END;
	return 2;
}

UINT32 SceneSkyCity::GetGroup11()
{
	return 1;
}

UINT32 SceneSkyCity::GetGroup22()
{
	return 2;
}

void SceneSkyCity::InitDataToClient(Role* pRole/* = NULL*/)
{
	PtcG2C_SkyCityTeamRes ptc;
	FillInitData(ptc.m_Data);
	if(pRole)
	{
		pRole->Send(ptc);
	}
	else
	{
		GetScene()->Broadcast(ptc);
	}
}

void SceneSkyCity::AllInfoToClient(Role* pRole/* = NULL*/)
{
	PtcG2C_SkyCityBattleDataNtf ptc;
	FillAllInfo(ptc.m_Data);	
	if(pRole)
	{
		pRole->Send(ptc);
	}
	else
	{
		GetScene()->Broadcast(ptc);
	}
}

void SceneSkyCity::FillNowData(KKSG::SkyCityRes& roRes)
{
	FillInitData(*roRes.mutable_baseinfo());
	FillAllInfo(*roRes.mutable_allinfo());
}

void SceneSkyCity::FillInitData(KKSG::SkyCityAllTeamBaseInfo& data)
{
	for(int i = 0; i < m_oSkyCity.self_size(); ++i)
	{
		const auto& tmp = m_oSkyCity.self(i);
		data.add_info()->CopyFrom(tmp);	
	}

	for(int i = 0; i < m_oSkyCity.target_size(); ++i)
	{
		const auto& tmp = m_oSkyCity.target(i);
		data.add_info()->CopyFrom(tmp);	
	}

	for(int i = 0; i < data.info_size(); ++i)
	{
		SSDebug << " name = " << data.info(i).name() << " lev = " << data.info(i).lv() << " profresion = " << data.info(i).job() << END;
	}


	data.set_floor(GetFloor());
	data.set_games(GetGameNum());
	SSDebug << __FUNCTION__ << " floor = " << GetFloor() << " gamen = " << GetGameNum() << END;
}

void SceneSkyCity::FillAllInfo(KKSG::SkyCityAllInfo& data)
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
	//SSDebug << __FUNCTION__ << " timetype = " << data.timetype() << " lefttime = " << GetStateLeftTime() << END;
}

void SceneSkyCity::GameResultToClient()
{
	PtcG2C_SkyCityEstimateRes ptc;

	ptc.m_Data.set_winteamid(GetWinTeam());
	ptc.m_Data.set_floor(GetFloor());

	KKSG::SkyCityTeamScore* ts11 = ptc.m_Data.add_teamscore();
	ts11->set_teamid(GetTeam11());
	ts11->set_score(GetTeamScore(GetTeam11()));
	KKSG::SkyCityTeamScore* ts22 = ptc.m_Data.add_teamscore();
	ts22->set_teamid(GetTeam22());
	ts22->set_score(GetTeamScore(GetTeam22()));

	SSDebug << __FUNCTION__ << " winteam = " << ptc.m_Data.winteamid() << " floor = " << ptc.m_Data.floor() << END;
	//for(int i = 0; i < ptc.m_Data.teamscore_size(); ++i)
	//{
	//	SSDebug << __FUNCTION__ << " teamid = " << ptc.m_Data.teamscore(i).teamid() << " score = " << ptc.m_Data.teamscore(i).score() << END;
	//}

	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		UINT64 unitid = m_unitids[i];
		auto info = ptc.m_Data.add_info();
		info->set_roleid(unitid);
		info->set_killer(m_datamgr.GetUnitKillCount(unitid));
		info->set_damage((UINT64)m_datamgr.GetUnitTotalDamage(unitid));
	}

	GetScene()->Broadcast(ptc);
}

KKSG::SkyCityTimeType SceneSkyCity::GetTimeType()
{
	if(NULL == m_statemgr.GetCurrentState())
	{
		return KKSG::SC_NONE;
	}
	if(VS_STATE_IN_FIGHT == m_statemgr.GetCurrentState()->GetStateType())
	{
		return KKSG::Race;
	}
	if(VS_STATE_AFTER_FIGHT == m_statemgr.GetCurrentState()->GetStateType())
	{
		return KKSG::FirstWaiting;
	}
	if(VS_STATE_END == m_statemgr.GetCurrentState()->GetStateType())
	{
		return KKSG::SecondWaiting;
	}
	return KKSG::SC_NONE;
}

UINT32 SceneSkyCity::GetStateLeftTime()
{
	time_t now = time(NULL);
	//要根据状态判断
	switch(GetTimeType())
	{
	case KKSG::Race:
		{
			UINT32 fightTime =  GetGlobalConfig().SkyCityFirstHalfTime;
			if(GetFightCount() > 0)
			{
				fightTime =  GetGlobalConfig().SkyCitySecondHalfTime;
			}
			if(GetStateStartTime() + fightTime > now)
			{
				return UINT32(GetStateStartTime() + fightTime - now);
			}
			return 0;
		}
		break;
	case KKSG::FirstWaiting:
		{
			if(GetStateStartTime() +  GetGlobalConfig().SkyCityHalfTime > now)
			{
				return UINT32(GetStateStartTime() +  GetGlobalConfig().SkyCityHalfTime- now);
			}
			return 0;
		}
		break;
	case KKSG::SecondWaiting:
		{
			if(GetEndTime() > now)
			{
				return UINT32(GetEndTime() - now);
			}
			return 0;
		}
		break;
	default:
		break;
	}
	return 10;
}

UINT32 SceneSkyCity::GetWinTeam()
{
	UINT32 score11 = GetTeamScore(GetTeam11());
	UINT32 score22 = GetTeamScore(GetTeam22());
	double totaldamage11 = m_datamgr.GetGroupTotalDamage(GetTeam11());
	double totaldamage22 = m_datamgr.GetGroupTotalDamage(GetTeam22());

	if(score11 > score22)
	{
		return GetTeam11();
	}
	if(score11 < score22)
	{
		return GetTeam22();
	}

	if(totaldamage11 > totaldamage22)
	{
		return GetTeam11();
	}

	if(totaldamage11 < totaldamage22)
	{
		return GetTeam22();
	}

	return 0;//平局
}

void SceneSkyCity::CheckInitRobot()
{
	if(m_oSkyCity.target_size() > 0)
	{
		return ;
	}

	if(m_oSkyCity.self_size() == 0)
	{
		SSError << " sky city self size = 0 sceneid = " << GetScene()->GetSceneID() << END;
		return ;
	}

	FilterRoleData filter;
	filter.qwRoleID = m_oSkyCity.self(0).uid();
	filter.level = m_oSkyCity.self(0).lv();
	filter.ppt = m_oSkyCity.self(0).ppt();
	filter.prof = m_oSkyCity.self(0).job();

	//对机器人进行初始化, 填数据
	UINT32 robotteam = GetRobotTeam();
	UINT32 robotlookupid =  GetGlobalConfig().SkyCityLookupId;

	for(int i = 0; i < 3; ++i)
	{
		DummyRole* pDummy = CreateRobot(filter, robotlookupid, GetGroup22(), SCENE_ENEMY);	
		if(pDummy)
		{
			auto& data = *m_oSkyCity.add_target();
			data.set_teamid(robotteam);
			data.set_uid(pDummy->GetID());
			data.set_name(pDummy->GetName());
			data.set_lv(pDummy->GetLevel());
			data.set_ppt(pDummy->GetAttr(TOTAL_POWERPOINT));
			data.set_job(pDummy->GetProfession());
			data.set_online(true);

			m_loadmgr.AddRole(data.uid());//机器人默认loaded
		}
	}

}

void SceneSkyCity::OnEnterFight()
{
	//SkillCoolDown(m_unitids);

	AllInfoToClient();
}

VsStateType SceneSkyCity::CheckFightEnd()
{
	UINT32 lefttime = GetStateLeftTime();
	if(0 == lefttime || OneGroupAllDeadOrQuit())
	{
		m_saveTime += lefttime;//节约时间	

		if(GetFightCount() > 0)
		{
			return VS_STATE_END;
		}
		else
		{
			return VS_STATE_AFTER_FIGHT;
		}
	}

	return VS_STATE_DUMMY;
}

void SceneSkyCity::OnEnterAfterFight()
{
	AllInfoToClient();

	//CheckRevive();
	SetCheckReviveFlag(false);
	ResetPosition(true);

	AddFightCount();//last
}

VsStateType SceneSkyCity::CheckAfterFightEnd()
{
	if(0 == GetStateLeftTime())
	{
		return VS_STATE_PREPARE321;
	}
	return VS_STATE_DUMMY;
}

void SceneSkyCity::ResetPosition(bool rest /*= flase*/)
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		if(!rest)
		{
			TranInitPosition(m_unitids[i]);
		}
		else
		{
			const std::vector<float>& confpos = GetGlobalConfig().SkyCityRestPos;
			Unit* pUnit = GetUnit(m_unitids[i]);
			if(pUnit && 6 == confpos.size())
			{
				EnterPosition  pos;
				pos.face = pUnit->GetXObject()->GetFaceDegree();
				if(GetGroup(m_unitids[i]) == GetGroup11())
				{
					pos.position.x = confpos[0] + (float)(XRandom::randDouble(-0.5, 0.5));
					pos.position.y = confpos[1];
					pos.position.z = confpos[2] + (float)(XRandom::randDouble(-0.5, 0.5));
				}
				else
				{
					pos.position.x = confpos[3] + (float)(XRandom::randDouble(-0.5, 0.5));
					pos.position.y = confpos[4];
					pos.position.z = confpos[5] + (float)(XRandom::randDouble(-0.5, 0.5));
				}
				XEntity* entity = dynamic_cast<XEntity*>(pUnit->GetXObject());
				if(entity)
				{
					entity->TransferLocation(pos.position, pos.face);
				}
			}
		}
	}
}

void SceneSkyCity::CheckRevive()
{
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		Unit* pUnit = GetUnit(m_unitids[i]);
		if(NULL == pUnit)
		{
			continue;
		}
		if(pUnit->IsDead())
		{
			pUnit->Revive();
		}
		else
		{
			pUnit->ResetHpMp();
		}
	}
}

bool SceneSkyCity::OneGroupAllDeadOrQuit()
{
	UINT32 have = 0;
	for(int i = 0; i < m_oSkyCity.self_size(); ++i)
	{
		UINT64 unitID = m_oSkyCity.self(i).uid();
		if(!DeadOrQuit(unitID))
		{
			have++;
		}
	}
	if(0 == have)
	{
		return true;
	}
	have = 0;
	for(int i = 0; i < m_oSkyCity.target_size(); ++i)
	{
		UINT64 unitID = m_oSkyCity.target(i).uid();
		if(!DeadOrQuit(unitID))
		{
			have++;
		}
	}
	if(0 == have)
	{
		return true;
	}
	return false;
}

bool SceneSkyCity::DeadOrQuit(UINT64 unitID)
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

bool SceneSkyCity::IsLastGame()
{
	return (GetGameNum() ==  GetGlobalConfig().SkyCityGames);
}

void SceneSkyCity::FillLastResult(Role* pRole, KKSG::SkyCityFinalInfo& data)
{
	if(NULL == pRole)
	{
		return ;
	}
	UINT64 roleid = pRole->GetID();

	UINT32 winteam = GetWinTeam();
	UINT32 myteam = 0;
	if(GetGroup(roleid) == GetGroup11())
	{
		myteam = GetTeam11();
	}
	else
	{
		myteam = GetTeam22();
	}

	UINT32 myfloor = GetFloor();

	if(myteam == winteam)
	{
		++myfloor;
	}

	UINT32 levelseal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	levelseal = SkyCityConfig::Instance()->GetLevelSeal(levelseal);

	data.set_floor(myfloor);	
	data.set_ismvp((roleid == m_datamgr.GetMVPID(GetGroup(roleid))));
	SSDebug << __FUNCTION__ << " rolename = " << pRole->GetName() << " myfloor " << myfloor << " ismvp = " << data.ismvp() << END;

	auto pConf = SkyCityConfig::Instance()->GetSkyArenaReward(levelseal, myfloor);
	if(pConf)
	{
		for(size_t i = 0; i < pConf->Reward.size(); ++i)
		{
			const auto& seq = pConf->Reward[i];
			auto gitem = data.add_item();
			gitem->set_itemid(seq[0]);
			gitem->set_itemcount(seq[1]);
			gitem->set_isbind(false);
		}
	}
	
}

void SceneSkyCity::MakeAllStatistics()
{
	int IndexMax = 100;
	for(int i = 0; i < m_oSkyCity.rolsstatistics_size(); ++i)
	{
		const auto& data = m_oSkyCity.rolsstatistics(i);
		SSDebug << __FUNCTION__ << " roleid = " << data.roleid() << " killcount = " << data.killcount() << " deadcount = " << data.deadcount() << " totaldamage = " << data.totaldamage() << END;
		Role* pRole = GetRole(data.roleid());	
		if(pRole)
		{
			for(int j = 0; j < data.killcount() && j < IndexMax; ++j)
			{
				pRole->Statistics().AddKillCount();	
			}
			for(int j = 0; j < data.deadcount() && j < IndexMax; ++j)
			{
				pRole->Statistics().AddDeathCount();
			}
			pRole->Statistics().AddAttackInfo(NULL, data.totaldamage());

		}
	}
}

void SceneSkyCity::GetSameGroupRoles(Role* pRole, std::vector<Role*>& roles)
{
	if(NULL == pRole)
	{
		return ;
	}
	for(size_t i = 0; i < m_unitids.size(); ++i)
	{
		Unit* pUnit = GetUnit(m_unitids[i]);	
		if(pUnit && pUnit->IsRole() && pUnit->GetFightGroup() == pRole->GetFightGroup())
		{
			roles.push_back((Role*)(pUnit));
		}
	}
}

UINT32 SceneSkyCity::GetTeamScore(UINT32 teamid)
{
	UINT32 score = m_datamgr.GetGroupKillCount(teamid);

	auto& others = (teamid == GetTeam11())?m_oSkyCity.target():m_oSkyCity.self();

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

std::string SceneSkyCity::GetRoleName(UINT64 roleID)
{
	for(int i = 0; i < m_oSkyCity.self_size(); ++i)
	{
		const auto& info = m_oSkyCity.self(i);	
		if(info.uid() == roleID)
		{
			return info.name();
		}
	}
	for(int i = 0; i < m_oSkyCity.target_size(); ++i)
	{
		const auto& info = m_oSkyCity.target(i);	
		if(info.uid() == roleID)
		{
			return info.name();
		}
	}
	return "";
}

void SceneSkyCity::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
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

void SceneSkyCity::PreDoEnterScene(Role* pRole)
{
	//SSDebug << __FUNCTION__ << " name = " << pRole->GetName() << END;
	InitDataToClient(pRole);	
	//先不考虑击杀和伤害	
}
