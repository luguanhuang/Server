#include "pch.h"
#include "scenegpr.h"
#include "globalconfig.h"
#include "scene.h"
#include "unit/role.h"
#include "guild/ptcg2c_gmfguildcombatntf.h"
#include "pb/project.pb.h"
#include "guild/ptcg2c_gpronebattleendntf.h"
#include "guild/ptcg2c_gprallfightendntf.h"
#include "gamelogic/notice.h"
#include "guild/guildmgr.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"
#include "define/tlogenum.h"
#include "attributepk.h"

SceneGPR::SceneGPR(Scene* scene):SceneGWB(scene),m_datamgr(scene)
{
	m_lastCombatUpdateTime = time(NULL);
}

SceneGPR::~SceneGPR()
{

}

void SceneGPR::OnJoinStart()
{
	SSDebug << __FUNCTION__ << END;
	if(!JoinStoped())
	{
		return ;
	}
	m_enterStop = false;
	m_joinStop = false;

	EnterStopToMs(m_enterStop);

	CheckRevive(true);
	//不用重置血量

	m_fightRoles.clear();
	m_room.Clear();

	//观战切正常模式
	const std::unordered_map<UINT64, Role*>& tempwatchers = GetScene()->GetWatchers();
	std::vector<Role*> watchers;
	for (auto i = tempwatchers.begin(); i != tempwatchers.end(); ++i)
	{
		watchers.push_back(i->second);
	}
	for(size_t i = 0; i < watchers.size(); ++i)
	{
		Role* pRole = watchers[i];
		SSDebug << __FUNCTION__ << " name = " << pRole->GetName() << END;
		if(pRole && pRole->IsWatcher() && 
			(pRole->getGuildId() == GetGuildID11() || pRole->getGuildId() == GetGuildID22()))
		{
			ChangeWatcherState(pRole, false);
		}
	}

	ResetFightGroup();

	RoomStateToClient();
}

VsStateType SceneGPR::OnJoinStop()
{
	SSDebug << __FUNCTION__ << END;
	m_joinStop = true;

	m_room.GetAllRoles(m_fightRoles);

	//ChangePPT();

	m_fightHistory.insert(m_fightRoles.begin(), m_fightRoles.end());

	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		m_datamgr.AddUnit(m_fightRoles[i], m_room.GetHalfRoomTypeByRoleID(m_fightRoles[i]));
		m_room.SetRoleMFState(m_fightRoles[i], KKSG::GUILD_MF_FIGHTING);
	}

	if(0 == m_room.GetRoleCount(GMF_HRT_ONE) || 0 == m_room.GetRoleCount(GMF_HRT_TWO))
	{
		return VS_STATE_AFTER_FIGHT;
	}

	return VS_STATE_PREPARE321;
}

VsStateType SceneGPR::CheckFightEnd()
{
	if(GetFailedGuildType() != GMF_HRT_NONE)
	{
		return VS_STATE_AFTER_FIGHT;
	}

	if(0 == GetFightLeftTime())
	{
		return VS_STATE_AFTER_FIGHT;
	}

	return VS_STATE_DUMMY;
}

bool SceneGPR::CheckGuildFail(GMFHalfRoomType halftype)
{
	UINT32 quitcount = 0;
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		if(m_room.GetHalfRoomTypeByRoleID(m_fightRoles[i]) == halftype && !m_loadMgr.HaveRole(m_fightRoles[i]))
		{
			++quitcount;
		}
	}

	if(quitcount >= m_room.GetRoleCount(halftype))
	{
		return true;
	}

	return false;
}

GMFHalfRoomType SceneGPR::GetFailedGuildType()
{
	bool failedguild11 = CheckGuildFail(GMF_HRT_ONE);
	bool failedguild22 = CheckGuildFail(GMF_HRT_TWO);

	if(failedguild11 && !failedguild22)
	{
		return GMF_HRT_ONE;
	}
	else if(!failedguild11 && failedguild22)
	{
		return GMF_HRT_TWO;
	}
	else if(failedguild11 && failedguild22)
	{
		if(m_room.GetPrestigeRank(GMF_HRT_ONE) < m_room.GetPrestigeRank(GMF_HRT_TWO))
		{
			return GMF_HRT_TWO;
		}
		else
		{
			return GMF_HRT_ONE;
		}
	}

	return GMF_HRT_NONE;
}

void SceneGPR::OnEnterAfterFight()
{
	SSDebug << __FUNCTION__ << END;

	EndSkills(m_fightRoles);

	GMFHalfRoomType winhalf = GMF_HRT_ONE;
	GMFHalfRoomType losehalf = GMF_HRT_TWO;

	GMFHalfRoomType tmphalf = GetFailedGuildType();
	if(GMF_HRT_ONE == tmphalf)
	{
		std::swap(winhalf, losehalf);
	}
	else if(GMF_HRT_TWO == tmphalf)
	{
		;
	}
	else
	{
		UINT32 killcount11 = m_datamgr.GetGroupKillCount(GMF_HRT_ONE);
		UINT32 killcount22 = m_datamgr.GetGroupKillCount(GMF_HRT_TWO);
		double totaldamage11 = m_datamgr.GetGroupTotalDamage(GMF_HRT_ONE);
		double totaldamage22 = m_datamgr.GetGroupTotalDamage(GMF_HRT_TWO);

		if(killcount11 > killcount22)
		{
			;
		}
		else if(killcount11 < killcount22)
		{
			std::swap(winhalf, losehalf);
		}
		else if(totaldamage11 > totaldamage22)
		{
			;
		}
		else if(totaldamage11 < totaldamage22)
		{
			std::swap(winhalf, losehalf);
		}

	}

	AddScore(winhalf);

	RoomStateToClient();

	OneBattleEndToClient(winhalf, losehalf);
	BattleEndNotice(winhalf, losehalf, false);
	TLogOneBattleEnd(winhalf, losehalf);
	RemoveLive();

	m_datamgr.Clear();//最后
}

VsStateType SceneGPR::CheckAfterFightEnd()
{
	if(GetAfterFightLeftTime() > 0)
	{
		return VS_STATE_DUMMY;
	}

	SSDebug << __FUNCTION__ << " score = " << GetScore(GMF_HRT_ONE) << " " << GetScore(GMF_HRT_TWO) << END;

	GMFHalfRoomType type = GetFailedGuildType();

	if(GMF_HRT_NONE != type && 0 == m_room.GetRoleCount(type))
	{
		GMFHalfRoomType optype = m_room.GetOpHalfRoomType(type);
		int Count = 0;//防意外
		while(GetAllScore() < MaxFightCount() && Count < 10)
		{
			AddScore(optype);
			++Count;
		}
	}


	if(GetAllScore() < MaxFightCount())
	{
		return VS_STATE_READY_GMF;
	}

	return VS_STATE_END;
}

void SceneGPR::OnStateEnd()
{
	SSDebug << __FUNCTION__ << " score = " << GetScore(GMF_HRT_ONE) << " " << GetScore(GMF_HRT_TWO) << END;
	RoomStateToClient();

	GMFHalfRoomType winhalftype = GMF_HRT_ONE;
	GMFHalfRoomType losehalftype = GMF_HRT_TWO;
	if(GetScore(GMF_HRT_ONE) < GetScore(GMF_HRT_TWO))
	{
		std::swap(winhalftype, losehalftype);
	}

	AllEndToMs(m_room.GetGuildID(winhalftype), m_room.GetGuildID(losehalftype), GetScore(winhalftype), GetScore(losehalftype));
	AllBattleEndToClient(winhalftype, losehalftype);
	BattleEndNotice(winhalftype, losehalftype, true);

	GetScene()->SetDontDestory(false);
	SetEnd();
}

void SceneGPR::OnRoleQuit(UINT64 roleid)
{
	SceneGWB::OnRoleQuit(roleid);

	m_datamgr.OnUnitCombatStop(roleid);
}

int SceneGPR::HandlerJoinRequest(Role* pRole, const KKSG::GmfReadyArg& oArg, KKSG::GmfReadyRes& oRes)
{
	if(NULL == pRole)
	{
		return KKSG::ERR_FAILED;
	}

	if(JoinStoped() || pRole->IsWatcher())
	{
		return KKSG::ERR_FAILED;
	}

	if(KKSG::GMF_READY_UP == oArg.type() && m_fightHistory.find(pRole->GetID()) != m_fightHistory.end())
	{
		return KKSG::ERR_GMF_UPBATTLE_REPEAT;
	}

	return SceneGWB::HandlerJoinRequest(pRole, oArg, oRes);
}

void SceneGPR::OnRoleDie(UINT64 roleid)
{
	m_dieRecord[roleid] = time(NULL);

	VsKillNotify(roleid, GetGlobalConfig().GPRReviveTime, 0);
	
	BattleCombatToClient(NULL);
}

void SceneGPR::OnRoleRevive(UINT64 roleid)
{
	m_dieRecord.erase(roleid);
}

void SceneGPR::SubUpdate()
{
	CheckRevive(false);	

	time_t now = time(NULL);
	if(VS_STATE_IN_FIGHT == GetCurrentStateType() && m_lastCombatUpdateTime + 3 <= now)
	{
		m_lastCombatUpdateTime = now;

		BattleCombatToClient(NULL);
	}
}

void SceneGPR::CheckRevive(bool force)
{
	std::vector<UINT64> wait;
	time_t now = time(NULL);
	for(auto it = m_dieRecord.begin(); it != m_dieRecord.end();)
	{
		if((it->second) + GetGlobalConfig().GPRReviveTime < now || force)	
		{
			wait.push_back(it->first);
			m_dieRecord.erase(it++);
		}
		else
		{
			it++;
		}
	}

	for(size_t i = 0; i < wait.size(); ++i)
	{
		Role* pRole = GetRole(wait[i]);
		if(pRole && pRole->IsDead())
		{
			pRole->Revive();
			TranInitPosition(wait[i]);
		}
	}
}

void SceneGPR::BattleCombatToClient(Role* pRole)
{
	PtcG2C_GmfGuildCombatNtf ptc;
	FillGuildCombat(*ptc.m_Data.mutable_guildcombat11(), GetGuildID11());
	FillGuildCombat(*ptc.m_Data.mutable_guildcombat22(), GetGuildID22());
	//SSDebug << __FUNCTION__ << " kill11 = " << ptc.m_Data.guildcombat11().combat().killcount() 
	//	<< " damage11 = " << ptc.m_Data.guildcombat11().combat().damage() 
	//	<< " score = " << ptc.m_Data.guildcombat11().score() << END;
	//SSDebug << __FUNCTION__ << " kill22 = " << ptc.m_Data.guildcombat22().combat().killcount() 
	//	<< " damage11 = " << ptc.m_Data.guildcombat22().combat().damage()
	//	<< " score = " << ptc.m_Data.guildcombat22().score() << END;
	if(pRole)
	{
		pRole->Send(ptc);
	}
	else
	{
		GetScene()->Broadcast(ptc);
	}
}

void SceneGPR::FillGuildCombat(KKSG::GmfGuildCombat& guildCombat, UINT64 guildid)
{
	FillGmfGuildBrief(*guildCombat.mutable_gmfguild(), guildid);
	auto& data = *guildCombat.mutable_combat();
	GMFHalfRoomType halftype = m_room.GetHalfRoomTypeByGuildID(guildid);
	data.set_killcount(m_datamgr.GetGroupKillCount(halftype));
	data.set_damage(m_datamgr.GetGroupTotalDamage(halftype));
	guildCombat.set_score(GetScore(halftype));
}

void SceneGPR::OneBattleEndToClient(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf)
{
	SSDebug << __FUNCTION__ << " win = " << m_room.GetGuildID(winhalf) << " lose = " << m_room.GetGuildID(losehalf) << END;
	PtcG2C_GprOneBattleEndNtf ptc;	
	FillGmfGuildBrief(*ptc.m_Data.mutable_winguild(), m_room.GetGuildID(winhalf));
	FillGmfGuildBrief(*ptc.m_Data.mutable_loseguild(), m_room.GetGuildID(losehalf));

	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		if(m_room.GetHalfRoomTypeByRoleID(m_fightRoles[i]) == winhalf)
		{
			FillRoleCombat(*ptc.m_Data.add_winrolecombat(), m_fightRoles[i]);
		}
		else
		{
			FillRoleCombat(*ptc.m_Data.add_loserolecombat(), m_fightRoles[i]);
		}
	}

	for(int i = 0; i < ptc.m_Data.winrolecombat_size(); ++i)
	{
		auto& data = ptc.m_Data.winrolecombat(i);
		SSDebug << __FUNCTION__ << " role = " << data.gmfrole().rolename() << " kill = " << data.combat().killcount() << " damage = " << data.combat().damage() << END;
	}
	for(int i = 0; i < ptc.m_Data.loserolecombat_size(); ++i)
	{
		auto& data = ptc.m_Data.loserolecombat(i);
		SSDebug << __FUNCTION__ << " role = " << data.gmfrole().rolename() << " kill = " << data.combat().killcount() << " damage = " << data.combat().damage() << END;
	}

	GetScene()->Broadcast(ptc);
}

void SceneGPR::FillRoleCombat(KKSG::GmfRoleCombat& roleCombat, UINT64 roleid)
{
	FillGmfRoleBrief(*roleCombat.mutable_gmfrole(), roleid);
	roleCombat.mutable_combat()->set_killcount(m_datamgr.GetUnitKillCount(roleid));
	roleCombat.mutable_combat()->set_damage(m_datamgr.GetUnitTotalDamage(roleid));
}

void SceneGPR::AllBattleEndToClient(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf)
{
	PtcG2C_GprAllFightEndNtf ptc;
	FillGmfGuildBrief(*ptc.m_Data.mutable_winguild(), m_room.GetGuildID(winhalf));
	FillGmfGuildBrief(*ptc.m_Data.mutable_loseguild(), m_room.GetGuildID(losehalf));
	ptc.m_Data.set_winscore(GetScore(winhalf));
	ptc.m_Data.set_losescore(GetScore(losehalf));

	GetScene()->Broadcast(ptc);
}

void SceneGPR::TLogOneBattleEnd(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf)
{
	UINT64 winguild = m_room.GetGuildID(winhalf);
	UINT64 loseguild = m_room.GetGuildID(losehalf);
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		UINT64 roleID = m_fightRoles[i];//玩家ID
		UINT64 roleGID = m_room.GetGuildIDByRoleID(roleID);//公会ID
		UINT32 score = 0;//积分
		

		if(m_room.GetHalfRoomTypeByRoleID(roleID) == winhalf)
		{
			score = OneBattleScore();
		}
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
		if (pRole)
		{
			TGuildArenaFlow oLog(pRole);
			oLog.m_iPoint = score;
			oLog.m_iOp = TGuildArena_Type3;
			oLog.m_uGuildID = score?winguild:loseguild;
			oLog.m_uGuildID2 = score?loseguild:winguild;
			oLog.Do();
		}
		//to do

	}
}

void SceneGPR::BattleEndNotice(GMFHalfRoomType winhalf, GMFHalfRoomType losehalf, bool allend)
{
	UINT64 winguild = m_room.GetGuildID(winhalf);
	UINT64 loseguild = m_room.GetGuildID(losehalf);
	
	if(allend)
	{
		MakeNotice(GPRAllGameWin, winguild, loseguild, GetScore(winhalf) * OneBattleScore());
		MakeNotice(GPRAllGameLose, loseguild, winguild, GetScore(losehalf) * OneBattleScore());
	}
	else
	{
		MakeNotice(GPROneGameWin, winguild, loseguild, GetAllScore());
		MakeNotice(GPROneGameLose, loseguild, winguild, GetAllScore());
	}
}

void SceneGPR::MakeNotice(UINT32 noticeid, UINT64 mygid, UINT64 opgid, UINT32 para)
{
	SSDebug << __FUNCTION__ << " noticeid = " << noticeid << " mygid = " << mygid << " opgid = " << opgid << " para = " << para << END;
	Notice notice(noticeid);
	notice.Replace("$G", GuildSimpleMgr::Instance()->GetGuildName(opgid));
	notice.Replace("$N", ToString(para));
	notice.SendGuild(mygid);
}

void SceneGPR::ChangePPT()
{
	if(m_fightRoles.empty())
	{
		return ;
	}

	double allppt = 0.0;
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(m_fightRoles[i]);
		if(pRole)
		{
			allppt += pRole->GetAttr(TOTAL_POWERPOINT);
		}
	}

	double avgppt = allppt / m_fightRoles.size();
	for(size_t i = 0; i < m_fightRoles.size(); ++i)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(m_fightRoles[i]);
		if(pRole)
		{
			AttributePkMgr::Instance()->AdjustAttrs(pRole, KKSG::SceneType(GetScene()->GetSceneType()), pRole->GetAttr(TOTAL_POWERPOINT), avgppt);
		}
	}
	
}
