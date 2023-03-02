#include "pch.h"
#include "scenegwb.h"
#include "globalconfig.h"
#include "live/liveinfo.h"
#include "live/livemanager.h"
#include "scene.h"
#include "unit/rolemanager.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "scenemanager.h"
#include "guild/ptcg2c_gmfonebattleendntf.h"
#include "guild/ptcg2c_gmfallfightendntf.h"
#include "guild/guildmgr.h"
#include "pb/project.pb.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "unit/rolemanager.h"
#include "sceneswitch.h"
#include "mslink.h"
#include "guild/ptcg2c_gmfbasedatantf.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/ptcg2c_gmfwaitotherload.h"
#include "guild/ptcg2c_gmfwaitfightbegin.h"
#include "gamelogic/noticemgr.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "guild/ptcg2m_gmfupdatedatag2m.h"
#include "vshandler.h"
#include "scene/ptcg2c_gmfkicknty.h"
#include "sceneinspire.h"
#include "scene/ptcg2c_gmfbattlestatentf.h"
#include "guild/guilddef.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/reportdatarecord.h"

const UINT32 SwitchWactherMaxCountOneSec = 500;

SceneGWB::SceneGWB(Scene* scene): SceneVsBase(scene)
{
	m_guildwartimestate = 0;
	m_enterStop = false;
	m_joinStop = false;
	m_lastStateSynTime = time(NULL);
}

SceneGWB::~SceneGWB()
{

}

bool SceneGWB::Init(const KKSG::CreateBattleParam& roParam)
{
	if(roParam.has_gmfpara())
	{
		Init(roParam.gmfpara());
		InitState();
		return true;
	}
	return false;
}

void SceneGWB::Init(const KKSG::GmfScenePara& para)
{
	m_room.LoadData(para.roomdata());		

	m_guildwartimestate = para.guildwartimestate();
}

void SceneGWB::InitState()
{
	m_statemgr.Init(this, &VsReadyGmf::GlobalVsReadyGmf);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

//此group用在pos的计算
UINT32 SceneGWB::GetGroup(UINT64 roleid)
{
	UINT64 gid = GuildSimpleMgr::Instance()->GetGuildId(roleid);
	if(gid == m_room.GetGuildID(GMF_HRT_ONE))
	{
		return GMF_HRT_ONE;
	}
	else if(gid == m_room.GetGuildID(GMF_HRT_TWO))
	{
		return GMF_HRT_TWO;
	}
	else
	{
		//有可能被踢出公会 待处理
	}
	return (GMF_HRT_ONE);
}

UINT64 SceneGWB::GetFightGroupUId(UINT64 roleid)
{
	if(!JoinStoped())		
	{
		return UINT64(-1);//开战之前相互不可攻击
	}
	return (UINT64)(GetGroup(roleid));
}

UINT32 SceneGWB::JoinLeftTime()
{
	auto cur = m_statemgr.GetCurrentState();
	if(NULL == cur || VS_STATE_READY_GMF != cur->GetStateType())
	{
		return 0;
	}
	UINT32 RedayTime =  GetGlobalConfig().GMFReadyTime;
	time_t now = time(NULL);
	if(GetStateStartTime() + RedayTime > now)
	{
		return UINT32(GetStateStartTime() + RedayTime - now);
	}
	return 0;
}

void SceneGWB::OnEnterStop()
{
	m_enterStop = true;

	if(0 == GetScene()->GetLiveID())
	{
		UINT32 liveid = AddLive();
		//同步观战 到 MS 
		AddLiveToMs(liveid);
	}

	EnterStopToMs(m_enterStop);
}

UINT32 SceneGWB::AddLive()
{
	std::vector<UINT64> guildInfo;
	guildInfo.push_back(m_room.GetGuildID(GMF_HRT_ONE));
	guildInfo.push_back(m_room.GetGuildID(GMF_HRT_TWO));
	guildInfo.push_back((UINT64)m_guildwartimestate);

	LiveInfo* info = LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_GUILDBATTLE, GetScene()->GetSceneID(), 0, &guildInfo, 0);
	if (info)
	{
		//GetScene()->SetLiveID(info->m_liveID);
		return info->m_liveID;
	}
	return 0;
}

void SceneGWB::RemoveLive()
{
	if(GetScene()->GetLiveID())
	{
		LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_GUILDBATTLE, GetScene()->GetSceneID(), 0, 0, 0);
		GetScene()->SetLiveID(0);
	}
}

void SceneGWB::AddLiveToMs(UINT32 liveID)
{
	SSDebug << __FUNCTION__ << " liveid = " << liveID << END;
	PtcG2M_GmfUpdateDataG2M ptc;
	m_room.SaveData(*ptc.m_Data.mutable_roomdata());
	ptc.m_Data.mutable_addlive()->set_liveid(liveID);
	// 考虑一下跨服的观战上报
	MSLink::Instance()->SendTo(ptc);
}

void SceneGWB::ResetLine()
{
	std::vector<Role*> rolemems;
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(m_fightRoles[i]);
		if(pRole)
		{
			rolemems.push_back(pRole);
		}
	}

	GetScene()->ChangeLine(rolemems);
}

void SceneGWB::FillGmfRoleBrief(KKSG::GmfRoleBrief& data, UINT64 roleid)
{
	auto it = m_roleBrief.find(roleid);
	if(it != m_roleBrief.end())
	{
		data = (it->second);
	}
	else
	{
		data.set_roleid(roleid);
		data.set_rolename("null");
		data.set_profession(1);
	}
}

void SceneGWB::FillGmfGuildBrief(KKSG::GmfGuildBrief& data, UINT64 gid)
{
	data.set_guildid(gid);
	auto* pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(gid);
	if(pGuild)
	{
		data.set_guildname(pGuild->GetGuildName());
		data.set_guildicon(pGuild->GetGuildIcon());
	}
}

void SceneGWB::ResetFightGroup()
{
	const std::list<Role*>& roles = GetScene()->GetAllRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)
	{
		Role* role = *i;
		UINT32 group = GetScene()->GetFightGroup(role);//最终会调用到GetFightGroupUId
		role->ChangeFightGroup(group);//改变fightgroup让打起来
		SSDebug << __FUNCTION__ << " name = " << role->GetName() << " group = " << group << END;
	}
}

int SceneGWB::HandlerJoinRequest(Role* pRole, const KKSG::GmfReadyArg& oArg, KKSG::GmfReadyRes& oRes)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}
	UINT64 roleid = pRole->GetID();

	UINT64 mygid = GuildSimpleMgr::Instance()->GetGuildId(roleid);
	GMFHalfRoomType mytype = m_room.GetHalfRoomTypeByRoleID(roleid);
	if(GMF_HRT_NONE == mytype)
	{
		if(mygid == m_room.GetGuildID(GMF_HRT_ONE))
		{
			mytype = GMF_HRT_ONE;
		}
		else if(mygid == m_room.GetGuildID(GMF_HRT_TWO))
		{
			mytype = GMF_HRT_TWO;
		}
	}

	if(GMF_HRT_NONE == mytype)
	{
		return KKSG::ERR_FAILED;
	}

	switch(oArg.type())
	{
	case KKSG::GMF_READY_UP:
		{
			if(m_room.GetRoleCount(mytype) >= GetJoinMaxCount())
			{
				return KKSG::ERR_GMF_UP_FULL;
			}
			UINT32 coolDownTime = m_kickMgr.GetCoolDownTime(roleid);
			if(coolDownTime > 0)
			{
				oRes.set_cooldown(coolDownTime);	
				return KKSG::ERR_GMF_UP_INCOOL;
			}
			
			if(m_room.HaveRoleID(roleid))
			{
				return KKSG::ERR_SUCCESS;//忽略请求
			}
			m_room.AddRoleID(mygid, roleid, pRole->GetName(), pRole->Get<CPkRecord>()->GetPoint(), pRole->GetProfession());
			SetRoleBrief(pRole);
		}
		break;
	case KKSG::GMF_READY_DOWN:
		{
			if(!m_room.HaveRoleID(roleid))
			{
				return KKSG::ERR_SUCCESS;//忽略请求
			}
			KKSG::GuildMatchFightState gmfState = m_room.GetRoleMFState(roleid);
			if(KKSG::GUILD_MF_FIGHTING == gmfState)
			{
				return KKSG::ERR_GMF_DOWN_FIGHTING;
			}
			if(KKSG::GUILD_MF_WAITING != gmfState)
			{
				return KKSG::ERR_GMF_DOWN_HAVEFAILED;
			}
			m_room.DelRoleID(roleid);
		}
		break;
	case KKSG::GMF_READY_KICK:
		{
			auto pGuild = GuildSimpleMgr::Instance()->GetGuildSimple(mygid);
			if(NULL == pGuild)
			{
				return KKSG::ERR_FAILED;
			}

			int ffpos = (int)pGuild->GetPosition(roleid);
			int ttpos = (int)pGuild->GetPosition(oArg.roleid());

			if(GPOS_LEADER != ffpos && GPOS_VICELEADER != ffpos)
			{
				return KKSG::ERR_FAILED;
			}

			if(GPOS_VICELEADER == ffpos && (GPOS_LEADER == ttpos || GPOS_VICELEADER == ttpos))
			{
				return KKSG::ERR_GMF_NOPOWER_KICK_LEADER;
			}

			if(!m_room.HaveRoleID(oArg.roleid()))
			{
				return KKSG::ERR_SUCCESS;//忽略请求
			}

			if(KKSG::GUILD_MF_WAITING != m_room.GetRoleMFState(oArg.roleid()))
			{
				return KKSG::ERR_FAILED;
			}

			if(m_room.GetHalfRoomTypeByRoleID(oArg.roleid()) != mytype)
			{
				return KKSG::ERR_FAILED;
			}

			m_room.DelRoleID(oArg.roleid());
			m_kickMgr.AddRole(oArg.roleid());

			PtcG2C_GmfKickNty ptc;
			ptc.m_Data.set_kickname(pRole->GetName());
			ptc.m_Data.set_cooldowntime( GetGlobalConfig().GMFKickCoolDown);
			if(pGuild->GetGuildLeaderId() == roleid)
			{
				ptc.m_Data.set_leaderkick(1);
			}
			else
			{
				ptc.m_Data.set_leaderkick(0);
			}
			Role* kickRole = RoleManager::Instance()->FindByRoleID(oArg.roleid());
			if(kickRole)
			{
				kickRole->Send(ptc);
			}

			Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
			if(pRole && kickRole)
			{
				NoticeMgr::Instance()->NoticeGuildArenaKick(kickRole, pRole);
			}
		}
		break;
	default:
		break;
	}

	RoomStateToClient();

	return KKSG::ERR_SUCCESS;
}

void SceneGWB::RoomStateToClient(Role* pRole/* = NULL*/)
{
	BattleBaseDataToClient(pRole);
	BattleStateToClient(pRole);
	BattleCombatToClient(pRole);
}

void SceneGWB::Update()
{
	SubUpdate();
	CheckStateSyn();

	if(JoinStoped() && VS_STATE_AFTER_FIGHT != GetCurrentStateType())
	{
		m_loadMgr.CheckSwitchWatcher(SwitchWactherMaxCountOneSec, this);		
	}
	m_statemgr.Update();//会改变 m_IsEnd 状态 所以顺序不能变
}

bool SceneGWB::IsFightRole(UINT64 roleID)
{
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		if(m_fightRoles[i] == roleID)
		{
			return true;
		}
	}
	return false;
}

void SceneGWB::OnEnterP321()
{
	FightLog();

	ResetPosition();

	ResetLine();//切线
	ResetFightGroup();

	RoomStateToClient();
}

void SceneGWB::ResetPosition()
{
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		TranInitPosition(m_fightRoles[i]);
	}
}

void SceneGWB::FightLog()
{
	if(KKSG::SCENE_GMF == GetScene()->GetSceneType())
	{
		//公会擂台战 巅峰对决
	}
	else if(KKSG::SCENE_GPR == GetScene()->GetSceneType())
	{
		//公会擂台战 积分赛
	}
	else
	{
		//no way
	}

	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		Role* pRole = GetRole(m_fightRoles[i]);
		if(pRole)
		{
			TGuildFightFlow oLog(pRole);
			oLog.m_nGuildID = m_room.GetGuildIDByRoleID(pRole->GetID());
			oLog.m_nChallengeType = TX_GUILD_ARENA;
			oLog.Do();
		}
	}
}

void SceneGWB::OnEnterFight()
{
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		SSDebug << __FUNCTION__ << " role = " << m_fightRoles[i] << END;
	}

    MakeSkillCDAtLaunch(m_fightRoles);

	//加buff
	Sequence<int, 2>& buff =  GetGlobalConfig().GMFBuff;
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		AddBuff(m_fightRoles[i], buff[0], buff[1]);
	}

	if(GetScene()->GetInspire())//鼓舞属性
	{
		for(size_t i = 0; i < m_fightRoles.size(); ++i)
		{
			GetScene()->GetInspire()->GwbOnFightBegin(m_fightRoles[i], m_room.GetGuildIDByRoleID(m_fightRoles[i]));
		}
	}

	RoomStateToClient();//需要倒计时
}

UINT64 SceneGWB::GetInspireID(UINT64 roleid, std::vector<UINT64>& addAttrIDs)
{
	UINT64 gid = GuildSimpleMgr::Instance()->GetGuildId(roleid);
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		if(m_room.GetGuildIDByRoleID(m_fightRoles[i]) == gid)
		{
			addAttrIDs.push_back(m_fightRoles[i]);	
		}
	}
	if(m_room.GetGuildID(GMF_HRT_ONE) == gid)
	{
		return gid;
	}
	if(m_room.GetGuildID(GMF_HRT_TWO) == gid)
	{
		return gid;
	}
	return 0;
}

void SceneGWB::TLogInspireID(Role* pRole, UINT64 guildID)
{
	if (NULL == pRole)return;
	TGuildArenaFlow oLog(pRole);
	oLog.m_iOp = TGuildArena_Type1;
	oLog.m_uGuildID = guildID;
	oLog.Do();
}

UINT32 SceneGWB::GetAfterFightLeftTime()
{
	time_t now = time(NULL);
	UINT32 ConfTime = GetGlobalConfig().GMFFightAfterTime;
	if(KKSG::SCENE_GPR == GetScene()->GetSceneType())
	{
		ConfTime = GetGlobalConfig().GPRFightAfterTime;
	}
	if(GetStateStartTime() + ConfTime > now)
	{
		return GetStateStartTime() + ConfTime - now;
	}
	return 0;
}

bool SceneGWB::OnEvent(const VsEvent& event)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(NULL == pRole)
	{
		return true;
	}

	if(VS_EVENT_ROLE_LOADED == event.type)
	{
		RoomStateToClient(pRole);	

		JoinMakeRecord(pRole);
	}

	if(pRole->IsWatcher())
	{
		SSDebug << " gmf watcher name = " << pRole->GetName() << " event = " << event.type << END;
		return true;
	}
	else
	{
		if(VS_EVENT_ROLE_LOADED == event.type)
		{
			OnRoleLoad(event.roleid);
		}
		else if(VS_EVENT_ROLE_QUIT == event.type)
		{
			OnRoleQuit(event.roleid);
		}
		else if(VS_EVENT_ROLE_DIE == event.type)
		{
			OnRoleDie(event.roleid);
		}
		else if(VS_EVENT_ROLE_REVIVE == event.type)
		{
			OnRoleRevive(event.roleid);
		}
	}

	return true;
}

void SceneGWB::OnRoleLoad(UINT64 roleid)
{
	SSDebug << __FUNCTION__ << " role = " << roleid << END;
	m_loadMgr.AddRole(roleid);

	if(JoinStoped())
	{
		MakePuppet(GetUnit(roleid), true);
	}

}

void SceneGWB::OnRoleQuit(UINT64 roleid)
{
	SSDebug << __FUNCTION__ << " role = " << roleid << END;
	m_loadMgr.DelRole(roleid);

	Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
	if(NULL == pRole)
	{
		return ;
	}

	MakePuppet(pRole, false);

	if(pRole->IsWatcher() || !JoinStoped())
	{
		KKSG::GmfReadyArg oArg;
		KKSG::GmfReadyRes oRes;
		oArg.set_type(KKSG::GMF_READY_DOWN);
		HandlerJoinRequest(pRole, oArg, oRes);
	}
}

void SceneGWB::AllEndToMs(UINT64 winGuildID, UINT64 loseGuildID, UINT32 winscore, UINT32 losescore)
{
	SSDebug << __FUNCTION__ << END;
	PtcG2M_GmfUpdateDataG2M ptc;
	m_room.SaveData(*ptc.m_Data.mutable_roomdata());
	KKSG::GmfFightResult& result = (*ptc.m_Data.mutable_result());
	result.set_winguild(winGuildID);
	result.set_loseguild(loseGuildID);
	result.set_winscore(winscore);
	result.set_losescore(losescore);
	MSLink::Instance()->SendTo(ptc);
}

int SceneGWB::GetBattleState(UINT32& lefttime)
{
	lefttime = 5;
	if(NULL == m_statemgr.GetCurrentState())
	{
		return KKSG::GMF_BS_NONE;
	}
	switch(m_statemgr.GetCurrentState()->GetStateType())
	{
	case VS_STATE_READY_GMF:
		{
			lefttime = JoinLeftTime();
			return KKSG::GMF_BS_WAIT;	
		}
		break;
	case VS_STATE_WAIT_LOAD:
	case VS_STATE_PREPARE321:
		{
			lefttime = GetGlobalConfig().GMFOneFightTime;
			return KKSG::GMF_BS_FIGHT;
		}
		break;
	case VS_STATE_IN_FIGHT:
		{
			lefttime = GetFightLeftTime();
			return KKSG::GMF_BS_FIGHT;
		}
		break;
	case VS_STATE_AFTER_FIGHT:
		{
			lefttime = GetAfterFightLeftTime();
			return KKSG::GMF_BS_RESULT;
		}
		break;
	case VS_STATE_END:
		{
			return KKSG::GMF_BS_RESULT;
		}
		break;
	default:
		break;
	}
	return KKSG::GMF_BS_NONE;
}

UINT32 SceneGWB::GetFightLeftTime()
{
	auto cur = m_statemgr.GetCurrentState();
	if(NULL == cur || VS_STATE_IN_FIGHT != cur->GetStateType())
	{
		return 0;
	}
	time_t now = time(NULL);
	UINT32 OneFightTime = GetGlobalConfig().GMFOneFightTime;
	if(KKSG::SCENE_GPR == GetScene()->GetSceneType())
	{
		OneFightTime = GetGlobalConfig().GPROneFightTime;
	}
	if(GetStateStartTime() +  OneFightTime <= now)
	{
		return 0;
	}
	return UINT32(GetStateStartTime() +  OneFightTime - now);
}

void SceneGWB::BattleBaseDataToClient(Role* pRole)
{
	PtcG2C_GmfBaseDataNtf gbdn;
	m_room.FillBaseData(gbdn.m_Data);

	auto& group11 = *gbdn.m_Data.mutable_halfrole11();
	auto& group22 = *gbdn.m_Data.mutable_halfrole22();

	FillGmfGuildBrief(*group11.mutable_guildb(), GetGuildID11());
	FillGmfGuildBrief(*group22.mutable_guildb(), GetGuildID22());

	if(GetScene()->GetInspire())
	{
		group11.set_inspire(GetScene()->GetInspire()->GetInspireCount(m_room.GetGuildID(GMF_HRT_ONE)));
		group22.set_inspire(GetScene()->GetInspire()->GetInspireCount(m_room.GetGuildID(GMF_HRT_TWO)));
	}
	SSDebug << __FUNCTION__ << " inspire11 = " << group11.inspire() << END;
	SSDebug << __FUNCTION__ << " inspire22 = " << group22.inspire() << END;


	if(pRole)
	{
		pRole->Send(gbdn);
	}
	else
	{
		GetScene()->Broadcast(gbdn);
	}
}

void SceneGWB::BattleStateToClient(Role* pRole)
{
	PtcG2C_GmfBattleStateNtf ptc;
	UINT32 lefttime = 0;
	int state = GetBattleState(lefttime);
	ptc.m_Data.set_state(KKSG::GmfBattleState(state));
	ptc.m_Data.set_lefttime(lefttime);

	SSDebug << __FUNCTION__ << " state = " << state << " lefttime = " << lefttime << END;

	if(pRole)
	{
		pRole->Send(ptc);
	}
	else
	{
		GetScene()->Broadcast(ptc);
	}
}

void SceneGWB::EnterStopToMs(bool stop /*= false*/)
{
	SSDebug << __FUNCTION__ << " stop = " << stop << END;
	PtcG2M_GmfUpdateDataG2M ptc;
	m_room.SaveData(*ptc.m_Data.mutable_roomdata());
	ptc.m_Data.mutable_enterstop()->set_stop(stop);
	MSLink::Instance()->SendTo(ptc);
}

void SceneGWB::OnBeforeChangeWather(Role* pRole)
{
	MakePuppet(pRole, false);
}

bool SceneGWB::IsWatchEnd()
{
	if(0 == GetScene()->GetLiveID())
	{
		return true;
	}
	return false;
}

void SceneGWB::SetRoleBrief(Role* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	UINT64 roleID = pRole->GetID();
	m_roleBrief[roleID].set_roleid(roleID);
	m_roleBrief[roleID].set_rolename(pRole->GetName());
	m_roleBrief[roleID].set_profession(UINT32(pRole->GetProfession()));
}

void SceneGWB::CheckStateSyn()
{
	time_t now = time(NULL);	
	if(m_lastStateSynTime + 10 > now)
	{
		return;
	}
	m_lastStateSynTime = now;

	RoomStateToClient(NULL);
}

void SceneGWB::JoinMakeRecord(Role* pRole)
{
	if(NULL == pRole)
	{
		return;
	}

	if(GetScene()->GetSceneType() != KKSG::SCENE_GPR)
	{
		return;
	}

	UINT64 gid = GuildSimpleMgr::Instance()->GetGuildId(pRole->GetID());
	if(GetGuildID11() == gid || GetGuildID22() == gid)
	{
		pRole->Get<CReportDataRecord>()->UpdateWeekData(KKSG::WeekReportData_GuildArena);
	}

}
