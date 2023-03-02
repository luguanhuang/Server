#include "pch.h"
#include "scenepvp.h"
#include "scene.h"
#include "util/XRandom.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/pvpbasedata.h"
#include "gamelogic/pvprecord.h"
#include "table/globalconfig.h"
#include "gamelogic/pvpmgr.h"
#include "unit/attributewatcher.h"
#include "pb/project.pb.h"
#include "gamelogic/bagtransition.h"
#include "pk/ptcg2c_pkpreparentf.h"
#include "pvp/ptcg2c_pvpbattlebeginntf.h"
#include "pvp/ptcg2c_pvpbattleendntf.h"
#include "pvp/ptcg2c_pvpbattlekill.h"
#include "sceneconfig.h"
#include "live/livemanager.h"
#include "entity/XObject.h"
#include "unit/unit.h"
#include "component/XBuffComponent.h"
#include "attributepk.h"
#include "component/XAIComponent.h"
#include "gamelogic/reportdatamgr.h"
#include "idutil.h"

ScenePvp::ScenePvp( Scene* scene ):SceneVsBase(scene)
{
	m_redLeaderID = 0;
	m_blueLeaderID = 0;
	m_redwincount = 0;
	m_bluewincount = 0;
	m_drawcount = 0;
	m_mvpID = 0;
}

ScenePvp::~ScenePvp()
{

}

void ScenePvp::InitState()
{
	m_statemgr.Init(this, &VsWaitLoad::GlobalVsWaitLoad);
	m_statemgr.GetCurrentState()->OnEnter(this);
}

bool ScenePvp::Init(const KKSG::CreateBattleParam& roParam)
{
	return Init(roParam.pvpinfo());
}

bool ScenePvp::Init(const KKSG::SceneSwitchData& roData)
{
	return Init(roData.pvpdata());
}

bool ScenePvp::Init(const KKSG::PvpBattleInfo& pvpInfo)
{
	m_pvpData = pvpInfo;

	for (INT32 i = 0; i < pvpInfo.roleinfo_size(); i++)
	{
		const KKSG::PvpRoleInfo& roleInfo = pvpInfo.roleinfo(i);
		UINT64 roleID = roleInfo.roleid();
		int pvpcamp = roleInfo.camp();

		PvpBattleData& data = m_roledatas[roleID];
		data.roleid = roleID;
		data.rolestate = PVP_ROLE_WAITLOAD;
		data.smallInfo.set_roleid(roleID);
		data.smallInfo.set_rolelevel(roleInfo.level());
		data.smallInfo.set_rolename(roleInfo.name());
		data.smallInfo.set_roleprofession(roleInfo.profession());

		if (1 == pvpcamp)
		{
			data.camp = PVP_CAMP_RED;
			SSDebug << __FUNCTION__ << " 1 == " << data.roleid << END;
		}
		else
		{
			data.camp = PVP_CAMP_BLUE;
			SSDebug << __FUNCTION__ << " 2 == " << data.roleid << END;
		}
	}

	LiveInfo* liveInfo = LiveManager::Instance()->CreateLiveInfo(KKSG::LIVE_PROTECTCAPTAIN, GetScene()->GetSceneID(), 0, 0, 0);
	/*if (liveInfo)
	{
		GetScene()->SetLiveID(liveInfo->m_liveID);
	}*/

	InitState();

	return true;
}

void ScenePvp::Update()
{
	m_statemgr.Update();

	if(InFightStage())
	{
		CheckRoleRevive(false);
	}

	return ;
}

bool ScenePvp::OnEvent( const VsEvent& event )
{
	SSDebug << __FUNCTION__ << " " << event.roleid << " " << event.type << END;
	Role* pRole = RoleManager::Instance()->FindByRoleID(event.roleid);
	if(NULL == pRole)
	{
		return true;
	}
	if(pRole->IsWatcher())
	{
		return true;
	}

	switch(event.type)
	{
	case VS_EVENT_ROLE_DIE:
		{
			AddRoleDie(event.roleid);
		}
		break;
	case VS_EVENT_ROLE_LOADED:
		{
			SetRoleBattleState(event.roleid, PVP_ROLE_LOADED);
		}
		break;
	case VS_EVENT_ROLE_QUIT:
	case VS_EVENT_ROLE_LOADING_QUIT:
		{
			SetRoleBattleState(event.roleid, PVP_ROLE_QUIT);
		}
		break;
	case VS_EVENT_ROLE_REVIVE:
		{
			auto pData = GetBattleData(event.roleid);
			if(pData)
			{
				pData->lastDieTime = time_t(0);
			}
		}
		break;
	default:
		break;
	}
	return true;
}

void ScenePvp::RandomLeader()
{
	m_redLeaderID = 0;
	m_blueLeaderID = 0;

	std::vector<UINT64> redrand;
	std::vector<UINT64> bluerand;

	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		PvpBattleData& data = it->second; 
		data.isleader = false;
		if(data.camp == PVP_CAMP_RED && data.rolestate == PVP_ROLE_LOADED)
		{
			redrand.push_back(data.roleid);
		}
		if(data.camp == PVP_CAMP_BLUE && data.rolestate == PVP_ROLE_LOADED)
		{
			bluerand.push_back(data.roleid);
		}
	}

	if(!redrand.empty())
	{
		UINT64 leaderID = redrand[XRandom::randInt(0,redrand.size())];
		auto it = m_roledatas.find(leaderID);
		if(it != m_roledatas.end())
		{
			(it->second).isleader = true;
			m_redLeaderID = leaderID;
		}
		
	}
	if(!bluerand.empty())
	{
		UINT64 leaderID = bluerand[XRandom::randInt(0,bluerand.size())];
		auto it = m_roledatas.find(leaderID);
		if(it != m_roledatas.end())
		{
			(it->second).isleader = true;
			m_blueLeaderID = leaderID;
		}
	}

	LiveManager::Instance()->OnChange(GetScene()->GetLiveID());

	SSDebug << __FUNCTION__ << " redleader = " << m_redLeaderID << " blueleader = " << m_blueLeaderID << END;
}

void ScenePvp::ClearBuff()
{
	SSDebug << __FUNCTION__ << END;
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		PvpBattleData& data = it->second;
		Role* pRole = GetScene()->FindSceneRoleNotWatcher(data.roleid);
		if(pRole && pRole->GetXObject() && pRole->GetXObject()->BuffComponent())
		{
			pRole->GetXObject()->BuffComponent()->ClearBuff();	
		}
	}
}

void ScenePvp::SetRoleBattleState( UINT64 roleID, PvpBattleState rolestate )
{
	auto it = m_roledatas.find(roleID);
	if(it != m_roledatas.end())
	{
		(it->second).rolestate = rolestate;
	}
}

void ScenePvp::InitBattleData()
{
	RandomLeader();
	ResetHpMpContiKill();
	LeaderAddBuff();
}

VsStateType ScenePvp::IsAllLoad()
{
	if(0 == GetRoleCount(PVP_CAMP_NONE, PVP_ROLE_WAITLOAD))
	{
		SSDebug << __FUNCTION__ << " all loaded " << END;
		return VS_STATE_PREPARE321;
	}
	//一方全退出 一方全loading 怎么处理? ****
	if(GetRoleCount(PVP_CAMP_RED, PVP_ROLE_QUIT) == GetRoleCount(PVP_CAMP_RED, PVP_ROLE_NONE))//red all quit
	{
		SSDebug << __FUNCTION__ << " red all quit " << END;
		return VS_STATE_PREPARE321;
	}
	if(GetRoleCount(PVP_CAMP_BLUE, PVP_ROLE_QUIT) == GetRoleCount(PVP_CAMP_BLUE, PVP_ROLE_NONE))//blue all quit
	{
		SSDebug << __FUNCTION__ << " blue all quit " << END;
		return VS_STATE_PREPARE321;
	}
	return VS_STATE_DUMMY;
}

void ScenePvp::AddRoleDie( UINT64 roleID )
{
	if(!InFightStage())
	{
		return;
	}
	SSDebug << __FUNCTION__ << " roleID" << END;
	auto it = m_roledatas.find(roleID);
	if(it != m_roledatas.end())
	{
		(it->second).lastDieTime = time(NULL);
	}
	VsKillNotify(roleID, GetGlobalConfig().PVPDieReviveTime, 0);
	
	Role* pRole = GetRole(roleID);
	if(NULL == pRole)
	{
		return;
	}
	UINT64 killerID = pRole->GetKillerID();	
	Role* killerRole = GetRole(killerID); 
	if(NULL == killerRole)
	{
		return ;
	}
	
	if(m_redLeaderID == roleID || m_blueLeaderID == roleID)
	{
		auto it = m_roledatas.find(killerID);
		if(it != m_roledatas.end())
		{
			(it->second).killLeaderCount++;	

			int extrKillCount = GetGlobalConfig().PVPLeaderKillCount - 1;
			for(int i = 0; i < extrKillCount && i < 10; ++i)
			{
				killerRole->Statistics().AddKillCount();
			}
		}
	}
}

void ScenePvp::ReviveRole( UINT64 roleID )
{
	Role* pRole = GetScene()->FindSceneRoleNotWatcher(roleID);
	if(pRole && pRole->IsDead())
	{
		SSDebug << __FUNCTION__ << " " << roleID << END;
		pRole->Revive();
	}
}

void ScenePvp::CheckRoleRevive( bool force /*= false*/ )
{
	time_t reviveTime = GetGlobalConfig().PVPDieReviveTime;
	time_t now = time(NULL);
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		PvpBattleData& data = it->second;
		if(PVP_ROLE_LOADED != data.rolestate)
		{
			continue;
		}
		if((data.lastDieTime && data.lastDieTime + reviveTime <= now)
			|| force)
		{
			ReviveRole(data.roleid);
			TranInitPosition(data.roleid);
			data.lastDieTime = (time_t)0;//置未死亡
		}
		else if(force)
		{
			TranInitPosition(data.roleid);
		}
	}
}

UINT32 ScenePvp::GetGroupByRoleID(UINT64 roleID)
{
	auto it = m_roledatas.find(roleID);
	if(it == m_roledatas.end())
	{
		return 0;
	}
	return (it->second).camp;
}

void ScenePvp::OneGameEnd(PvpCampType win, int type)
{
	ClearBuff();

	if(PVP_CAMP_RED == win)
	{
		++m_redwincount;
	}
	else if(PVP_CAMP_BLUE == win)
	{
		++m_bluewincount;
	}
	else
	{
		++m_drawcount;
	}
	SSDebug << __FUNCTION__ << " redwin = " << m_redwincount << " bluewin = " << m_bluewincount 
		<< " drawwin = " << m_drawcount << " reason = " << type << END;

	
	if(KKSG::PVP_OGE_ROLE_QUIT == type)//玩家不在场景过多
	{
		FillScore(win);//填满分数 直接结束总战斗
	}

	BattleEndNtf(win, type);

	ResetOther();
}

bool ScenePvp::AllGameEnd()
{
	int allCount = GetGlobalConfig().PVPGameCount;
	if((m_redwincount + m_bluewincount + m_drawcount) 
		>= allCount)
	{
		return true;
	}
	
	if(m_redwincount > (allCount - m_redwincount - m_drawcount))
	{
		return true;
	}

	if(m_bluewincount > (allCount - m_bluewincount - m_drawcount))
	{
		return true;
	}

	return false;
}

void ScenePvp::LeaderAddBuff()
{
	Sequence<int,2>& buff =  GetGlobalConfig().PVPLeaderBuff;
	SSDebug << __FUNCTION__ << END;
	AddBuff(m_redLeaderID, buff[0], buff[1]);	
	AddBuff(m_blueLeaderID, buff[0], buff[1]);
}

void ScenePvp::MakeAccounts()
{
	SSDebug << __FUNCTION__ << " " << m_redwincount << " " << m_bluewincount << " " << m_drawcount << END;
	PvpCampType win = PVP_CAMP_ERR;
	if(m_redwincount > m_bluewincount)
	{
		win = PVP_CAMP_RED;
	}
	else if(m_redwincount < m_bluewincount)
	{
		win = PVP_CAMP_BLUE;
	}

	UINT64 mvpUserID = 0;
	int mvpKillCont = 0, mvpDieCount = 0 ;
	double mvpTotalDamage = 0;
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		PvpBattleData& data = (it->second);
		Role* pRole = RoleManager::Instance()->FindByRoleID(data.roleid);
		if(pRole && PVP_ROLE_LOADED == data.rolestate)
		{
			int pkillcount = pRole->Statistics().GetKillCount();
			int pdiecount = pRole->Statistics().GetDeathCount();
			double ptotalDamage = pRole->Statistics().GetTotalDamage();
			if(0 == mvpUserID || pkillcount > mvpKillCont || (pkillcount == mvpKillCont && pdiecount < mvpDieCount)
				|| (pkillcount == mvpKillCont && pdiecount == mvpDieCount && ptotalDamage > mvpTotalDamage ))
			{
				mvpUserID = data.roleid;
				mvpKillCont = pkillcount;
				mvpDieCount = pdiecount;
				mvpTotalDamage = ptotalDamage;
			}
		}
	}

	m_mvpID = mvpUserID;

	KKSG::PvpOneRec redRec, blueRec;
	FillPvpRec(redRec, PVP_CAMP_RED, m_redwincount, m_bluewincount, m_drawcount, m_mvpID);
	FillPvpRec(blueRec, PVP_CAMP_BLUE, m_bluewincount, m_redwincount, m_drawcount, m_mvpID);

	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		PvpBattleData& data = (it->second);

		Role *pRole = GetRole(data.roleid);	
		if(pRole && PVP_ROLE_LOADED == data.rolestate)
		{
			if(PVP_CAMP_RED == data.camp)
			{
				AddPvpRecord(pRole, redRec);
			}
			else if(PVP_CAMP_BLUE == data.camp)
			{
				AddPvpRecord(pRole, blueRec);
			}
		}
	}

	//LiveManager::Instance()->RemoveLiveInfo(KKSG::LIVE_PROTECTCAPTAIN, GetScene()->GetSceneID(), 0, 0, 0);
}

void ScenePvp::AddPvpRecord( Role* pRole, KKSG::PvpOneRec& rec )
{
	SSDebug << __FUNCTION__ << " " << pRole->GetName() << END;
	if(pRole)
	{
		pRole->Get<MyPvpRecord>()->AddOneRec(rec);
		PvpMgr::Instance()->OnRoleFinishBattle(pRole);
		ReportDataMgr::Instance()->PvpChange(pRole, rec);
	}
}

void ScenePvp::TestPrint()
{
	SSDebug << " Scene Pvp : " << END;
	SSDebug << " red win = " << m_redwincount << END;
	SSDebug << " blue win = " << m_bluewincount << END;
	SSDebug << " draw win = " << m_drawcount << END;
	SSDebug << " mvp ID = " << m_mvpID << END;
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		(it->second).TestPrint();
	}
}

void ScenePvp::ResetHpMpContiKill()
{
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		PvpBattleData& data = it->second;
		Role* pRole = GetScene()->FindSceneRoleNotWatcher(data.roleid);
		if(pRole)
		{
			AttributeWatcher watcher(pRole);
			watcher.SetAttr(BASIC_CurrentHP, pRole->GetAttr(TOTAL_MaxHP));
			watcher.SetAttr(BASIC_CurrentMP, pRole->GetAttr(TOTAL_MaxMP));
			watcher.CheckChangedAttributeAndNotify(true);
			pRole->Statistics().ClearKillContinueCount();
		}
	}
}

PvpCampType ScenePvp::GetFinalWiner()
{
	if(m_redwincount > m_bluewincount)
	{
		return PVP_CAMP_RED;
	}
	else if(m_redwincount < m_bluewincount)
	{
		return PVP_CAMP_BLUE;
	}
	return PVP_CAMP_ERR;
}

void ScenePvp::FillPvpResult( KKSG::PVPResult* result, Role* pRole)
{
	if(NULL == result || NULL == pRole)
	{
		return ;	
	}
	auto it = m_roledatas.find(pRole->GetID());
	if(it == m_roledatas.end())
	{
		return ;
	}
	PvpBattleData& data = it->second;
	result->set_mygroup((int)(data.camp));
	result->set_wingroup((int)(GetFinalWiner()));
	result->set_killleadercount(data.killLeaderCount);
	result->set_ismvp((pRole->GetID() == m_mvpID));

	if(PVP_ROLE_LOADED != data.rolestate)
	{
		return;
	}

	BagGiveItemTransition give(pRole);
	give.SetReason(ItemFlow_Stage, ItemFlow_Stage_Pvp);
	//每日参与奖
	if(pRole->Get<MyPvpRecord>()->LeftJoinCountTodayInT() >= 0 && PvpMgr::Instance()->IsInOpenTime())
	{
		for(size_t i = 0; i <  GetGlobalConfig().PVPDayRewards.size(); ++i)
		{
			Sequence<UINT32, 2>& seq =  GetGlobalConfig().PVPDayRewards[i];
			KKSG::ItemBrief* ibrief = result->add_dayjoinreward();
			ibrief->set_itemid(seq[0]);	
			ibrief->set_itemcount(seq[1]);
			ibrief->set_isbind(false);
			
			give.GiveItem(seq[0], seq[1], false);
		}
	}

	//胜利
	if(data.camp == GetFinalWiner())
	{
		for(size_t i = 0; i <  GetGlobalConfig().PVPWinRewards.size(); ++i)
		{
			Sequence<UINT32, 2>& seq =  GetGlobalConfig().PVPWinRewards[i];
			KKSG::ItemBrief* ibrief = result->add_winreward();
			ibrief->set_itemid(seq[0]);	
			ibrief->set_itemcount(seq[1]);
			ibrief->set_isbind(false);

			give.GiveItem(seq[0], seq[1], false);
		}
	}

	give.NotifyClient();
	
}

void ScenePvp::BattleBeginNtf()
{
	SSDebug << __FUNCTION__ << END;
	PtcG2C_PvpBattleBeginNtf beginntf;
	beginntf.m_Data.set_thisgamecount(m_redwincount + m_bluewincount + m_drawcount + 1);
	beginntf.m_Data.set_allgamecount(GetGlobalConfig().PVPGameCount);
	beginntf.m_Data.set_group1leader(m_redLeaderID);
	beginntf.m_Data.set_group2leader(m_blueLeaderID);
	beginntf.m_Data.set_contitime(GetGlobalConfig().PVPGameTimeConti);

	if(GetScene())
	{
		GetScene()->Broadcast(beginntf);
	}
}

void ScenePvp::BattleEndNtf(PvpCampType win, int type)
{
	SSDebug << __FUNCTION__ << " win = " << win << " type = " << type << END;
	PtcG2C_PvpBattleEndNtf endntf;
	if(PVP_CAMP_NONE == win)
	{
		win = PVP_CAMP_ERR;//wiat change with client
	}
	endntf.m_Data.set_wingroup(win);
	endntf.m_Data.set_isallend(AllGameEnd());
	if(KKSG::PVP_ONEGAMEEND_REASON_IsValid(type))
	{
		endntf.m_Data.set_reason(KKSG::PVP_ONEGAMEEND_REASON(type));
	}
	
	if(GetScene())
	{
		GetScene()->Broadcast(endntf);
	}
}

bool ScenePvp::IsFirstInToBattle()
{
	if(!m_redwincount && !m_bluewincount && !m_drawcount)
	{
		return true;
	}
	return false;
}

void ScenePvp::FillNowGameData( KKSG::PvpNowGameData& data )
{
	data.set_group1wincount(m_redwincount);
	data.set_group2wincount(m_bluewincount);
	data.set_drawwincount(m_drawcount);
	data.set_group1leader(m_redLeaderID);
	data.set_group2leader(m_blueLeaderID);
	data.set_isallend(AllGameEnd());
	data.set_lefttime((UINT32)GetOneGameLeftTime());

	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		//if(PVP_ROLE_LOADED != (it->second).rolestate)
		//{
		//	continue;
		//}
		auto& roleinfo = (it->second);
		KKSG::PvpNowUnitData* nud = data.add_nowunitddata();
		nud->set_roleid(roleinfo.roleid);
		nud->set_groupid(int(roleinfo.camp));
		nud->set_rolename(roleinfo.smallInfo.rolename());
		nud->set_rolelevel(roleinfo.smallInfo.rolelevel());
		nud->set_roleprofession(roleinfo.smallInfo.roleprofession());
		Role* pRole = RoleManager::Instance()->FindByRoleID((it->second).roleid);
		if(pRole)
		{
			nud->set_killcount(pRole->Statistics().GetKillCount());
			nud->set_diecount(pRole->Statistics().GetDeathCount());
		}
	}
}

time_t ScenePvp::GetOneGameLeftTime()
{
	if(InFightStage())
	{
		time_t now = time(NULL);
		if(GetStateStartTime() + GetGlobalConfig().PVPGameTimeConti <= now)
		{
			return time_t(0);
		}
		else
		{
			return (GetStateStartTime() + GetGlobalConfig().PVPGameTimeConti) - now;
		}
	}
	return time_t(0);
}

PvpCampType ScenePvp::CheckRoleOutWin()
{
	UINT32 redInNum = GetRoleCount(PVP_CAMP_RED, PVP_ROLE_LOADED);
	UINT32 redOutNum = GetRoleCount(PVP_CAMP_RED, PVP_ROLE_WAITLOAD) + GetRoleCount(PVP_CAMP_RED, PVP_ROLE_QUIT);

	UINT32 blueInNum = GetRoleCount(PVP_CAMP_BLUE, PVP_ROLE_LOADED);
	UINT32 blueOutNum = GetRoleCount(PVP_CAMP_BLUE, PVP_ROLE_WAITLOAD) + GetRoleCount(PVP_CAMP_BLUE, PVP_ROLE_QUIT);

	if(redOutNum >=  GetGlobalConfig().PVPQuitFailQuitCount || 0 == redInNum)
	{
		return PVP_CAMP_BLUE;
	}
	else if(blueOutNum >=  GetGlobalConfig().PVPQuitFailQuitCount || 0 == blueInNum)
	{
		return PVP_CAMP_RED;
	}

	return PVP_CAMP_ERR;
}

PvpCampType ScenePvp::CheckLeaderQuitWin()
{
	auto data = GetBattleData(m_redLeaderID);	
	if(NULL == data || PVP_ROLE_QUIT == data->rolestate)
	{
		return PVP_CAMP_BLUE;	
	}
	data = GetBattleData(m_blueLeaderID);
	if(NULL == data || PVP_ROLE_QUIT == data->rolestate)
	{
		return PVP_CAMP_RED;
	}
	return PVP_CAMP_ERR;
}

PvpCampType ScenePvp::CheckLeaderDieWin()
{
	auto pRole = GetRole(m_redLeaderID);
	if(NULL == pRole || pRole->IsDead())
	{
		return PVP_CAMP_BLUE;	
	}
	pRole = GetRole(m_blueLeaderID);
	if(NULL == pRole || pRole->IsDead())
	{
		return PVP_CAMP_RED;
	}
	return PVP_CAMP_ERR;
}

PvpCampType ScenePvp::CheckTimeOutWin()
{
	if(GetOneGameLeftTime() > 0)
	{
		return PVP_CAMP_ERR;
	}
	Role* redRole = GetRole(m_redLeaderID);
	if(NULL == redRole)
	{
		return PVP_CAMP_BLUE;
	}
	Role* blueRole = GetRole(m_blueLeaderID);
	if(NULL == blueRole)
	{
		return PVP_CAMP_RED;
	}

	double redlife = redRole->GetPreciseHpPercent();
	double bluelife = blueRole->GetPreciseHpPercent();

	if(redlife > bluelife)
	{
		return PVP_CAMP_RED;
	}
	else if(redlife < bluelife)
	{
		return PVP_CAMP_BLUE;
	}
	else
	{
		return PVP_CAMP_NONE;
	}

	return PVP_CAMP_NONE;
}

void ScenePvp::FillScore(PvpCampType win)
{
	int allCount = GetGlobalConfig().PVPGameCount;
	int leftGameCount = allCount - (m_redwincount + m_bluewincount + m_drawcount);	
	while(leftGameCount > 0 && !AllGameEnd())
	{
		if(PVP_CAMP_BLUE == win)
		{
			++m_bluewincount;
		}
		else if(PVP_CAMP_RED == win)
		{
			++m_redwincount;
		}
		--leftGameCount;
	}
}

void ScenePvp::GetSamePvpCampRoles(UINT64 roleID, std::vector<UINT64>& IDS)
{
	IDS.clear();
	auto myit = m_roledatas.find(roleID);
	if(myit == m_roledatas.end())
	{
		return ;
	}
	PvpCampType pCamp = (myit->second).camp;

	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		if((it->second).roleid == roleID || (it->second).camp != pCamp)
		{
			continue;
		}
		IDS.push_back((it->second).roleid);
	}
}

UINT64 ScenePvp::GetWinUId()
{
	auto type = GetFinalWiner();
	if(PVP_CAMP_BLUE == type)
	{
		return m_blueLeaderID;
	}
	else if(PVP_CAMP_RED == type)
	{
		return m_redLeaderID;
	}
	return 0;
}

void ScenePvp::ResetOther()
{
	//GetScene(); 场景指针
	GetScene()->GetGlobalAI()->AIComponent()->ResetAILogic();
}

void ScenePvp::OnEnterP321()
{
	if(!IsFirstInToBattle())
	{
		CheckRoleRevive(true);
	}
}

void ScenePvp::OnEnterFight()
{
	InitBattleData();
	BattleBeginNtf();
}

//各种判断之间的顺序不能变(因为有优先级)
VsStateType ScenePvp::CheckFightEnd()
{
	PvpCampType win = PVP_CAMP_ERR;

	win = CheckRoleOutWin();// 退出/没有进来 太多
	if(PVP_CAMP_ERR != win) 
	{
		OneGameEnd(win, KKSG::PVP_OGE_ROLE_QUIT);	
		return VS_STATE_AFTER_FIGHT;
	}	
		
	win = CheckLeaderQuitWin();
	if(PVP_CAMP_ERR != win) 
	{
		OneGameEnd(win, KKSG::PVP_OGE_LEADER_QUIT);	
		return VS_STATE_AFTER_FIGHT;
	}

	win = CheckLeaderDieWin();
	if(PVP_CAMP_ERR != win)
	{
		OneGameEnd(win, KKSG::PVP_OGE_LEADER_DIE);	
		return VS_STATE_AFTER_FIGHT;
	}

	win = CheckTimeOutWin();
	if(PVP_CAMP_ERR != win)
	{
		OneGameEnd(win, KKSG::PVP_OGE_TIMELIMIT);	
		return VS_STATE_AFTER_FIGHT;
	}

	return VS_STATE_DUMMY;	
}

void ScenePvp::OnStateEnd()
{
	MakeAccounts();

	GetScene()->SetDontDestory(false);
	SetEnd();//最后设置
}

VsStateType ScenePvp::CheckAfterFightEnd()
{
	bool allend = AllGameEnd();
	if(GetStateStartTime() + PvpMgr::Instance()->GetGameTimeDown(allend) <= time(NULL))
	{
		if(allend)
		{
			return VS_STATE_END;
		}
		else
		{
			return VS_STATE_PREPARE321;
		}
	}

	return VS_STATE_DUMMY;
}

UINT32 ScenePvp::GetGroup(UINT64 roleid)
{
	auto it = m_roledatas.find(roleid);
	if(it == m_roledatas.end())
	{
		return 1;
	}
	if(PVP_CAMP_ERR == (it->second).camp)
	{
		return 1;
	}
	return (it->second).camp;
}

void ScenePvp::FillPvpRec(KKSG::PvpOneRec& rec, PvpCampType type, UINT32 win, UINT32 lose, UINT32 draw, UINT64 mvp)
{
	rec.set_wincount(win);
	rec.set_losecount(lose);
	rec.set_drawcount(draw);
	rec.set_mvpid(mvp);

	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		if((it->second).camp == type)
		{
			FillPvpRoleBrief(*rec.add_myside(), it->second.smallInfo);
		}
		else
		{
			FillPvpRoleBrief(*rec.add_opside(), it->second.smallInfo);
		}
	}
}

void ScenePvp::FillPvpRoleBrief(KKSG::PvpRoleBrief& data, const KKSG::RoleSmallInfo& info)
{
	data.set_roleid(info.roleid());
	data.set_rolename(info.rolename());
	data.set_rolelevel(info.rolelevel());
	data.set_roleprofession(info.roleprofession());
	data.set_roleserverid(GetServerIDByRoleID(info.roleid()));
}

UINT32 ScenePvp::GetLoadWaitTime()
{
	return GetGlobalConfig().PVPLoadWaitTime;
}

void ScenePvp::GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded)
{
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		UINT64 roleID = (it->first);
		auto& roleinfo = (it->second);
		if(PVP_ROLE_LOADED == roleinfo.rolestate)
		{
			Role* pRole = GetRole(roleID);
			if(pRole)
			{
				loaded.push_back(pRole);
			}
		}
		else if(PVP_ROLE_WAITLOAD == roleinfo.rolestate)
		{
			unloaded.push_back(GetRoleName(roleID));
		}
	}
}

std::string ScenePvp::GetRoleName(UINT64 roleID)
{
	for(int i = 0; i < m_pvpData.roleinfo_size(); ++i)
	{
		if(m_pvpData.roleinfo(i).roleid() == roleID)
		{
			return m_pvpData.roleinfo(i).name();
		}
	}
	return "";
}

UINT32 ScenePvp::GetRoleCount(PvpCampType type, PvpBattleState state)
{
	UINT32 count = 0;
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		const auto& tmp = (it->second);
		if(PVP_CAMP_NONE == type || tmp.camp == type)
		{
			if(PVP_ROLE_NONE == state || tmp.rolestate == state)
			{
				++count;
			}
		}
	}
	return count;
}

bool ScenePvp::InFightStage()
{
	return (VS_STATE_IN_FIGHT == GetCurrentStateType());
}

PvpBattleData* ScenePvp::GetBattleData(UINT64 roleID)
{
	auto it = m_roledatas.find(roleID);
	if(it != m_roledatas.end())
	{
		return &(it->second);
	}
	return NULL;
}

void ScenePvp::OnEnterAfterFight()
{
	std::vector<UINT64> fightRoles;
	for(auto it = m_roledatas.begin(); it != m_roledatas.end(); ++it)
	{
		const auto& data = it->second;
		if(PVP_ROLE_LOADED == data.rolestate)
		{
			fightRoles.push_back(it->first);
		}
	}
	EndSkills(fightRoles);
}

PvpCampType ScenePvp::GetRoleCamp(UINT64 roleId)
{
	PvpBattleData* pData = GetBattleData(roleId);
	if (pData == NULL)
	{
		return PVP_CAMP_ERR;
	}
	return pData->camp;
}

