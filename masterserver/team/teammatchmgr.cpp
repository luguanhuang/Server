#include "pch.h"
#include "teammatchmgr.h"
#include "teammember.h"
#include "team.h"
#include "teamrequest.h"
#include "teammgr.h"
#include <time.h>
#include "foreach.h"
#include "role/rolemanager.h"
#include "table/expeditionconfigmgr.h"
#include "guild/guildmgr.h"
#include "common/ptcm2c_mserrornotify.h"
#include "team/ptcm2c_rolematchstatem2cnotify.h"
#include "team/ptcm2c_fastmbconfirmm2cntf.h"
#include "team/ptcm2c_fastmbdismissm2cntf.h"
#include "table/globalconfig.h"
#include "robotsummarymgr.h"
#include "teamforceop.h"
#include "other/matchhandler.h"

INSTANCE_SINGLETON(TeamMatchMgr)

static bool ReportError(CRole *pRole, int ret)
{
	if (ret != KKSG::ERR_SUCCESS)
	{
		LogWarn("match team join failed because %d\n", ret);
		PtcM2C_MSErrorNotify oPtc;
		oPtc.m_Data.set_errorno(ret);
		pRole->Send(oPtc);
		return false;
	}
	else
	{
		return true;
	}
}

TeamMatchMgr::TeamMatchMgr()
{
	m_timer = INVALID_HTIMER;
	m_fastMatchHandler = NULL;
}

TeamMatchMgr::~TeamMatchMgr()
{

}

bool TeamMatchMgr::Init()
{
	m_fastMatchHandler = new FastMatchHandler();
	AddHandler(m_fastMatchHandler);
	m_timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	return true;
}

void TeamMatchMgr::Uninit()
{
	foreach(i in m_Listener)
	{
		delete *i;
	}

	m_Listener.clear();

	CTimerMgr::Instance()->KillTimer(m_timer);
}

void TeamMatchMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	std::list<DelayPairRole> roles = m_delayhandler.PopRoles();
	for (auto i = roles.begin(); i != roles.end(); ++i)	
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(i->roleid);
		if (NULL != role)
		{
			DoMatch(role, i->expid);
		}
	}

	std::list<DelayPairTeam> teams = m_delayhandler.PopTeams();
	for (auto i = teams.begin(); i != teams.end(); ++i)
	{
		Team* team = TeamMgr::Instance()->FindTeam(i->teamid);
		if (NULL != team)
		{
			DoMatch(team);	
		}
	}

	/*
	std::list<DelayPairRole> fastroles = m_delayhandler.PopFastRoles(); 
	for(auto it = fastroles.begin(); it != fastroles.end(); ++it)
	{
		Role* role = RoleManager::Instance()->FindByRoleID(it->roleid);
		if (NULL != role && !InMatch(role))
		{
			DoFastMatch(role, it->expid);
		}
	}
	*/
}

int TeamMatchMgr::StartMatch(CRole *pRole, int expID)
{
	//SSDebug << __FUNCTION__ << " roleid = " << pRole->GetID() << " expid = " << expID << END;
	ExpeditionTable::RowData *pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if (pConf == NULL)
	{
		LogWarn("Not found team conf id : %d", expID);
		return KKSG::ERR_TEAM_EXPEDITIONID_NOT_EXIST;
	}
	
	if(pConf->fastmatch)
	{
		DoFastMatch(pRole, expID);
	}
	else
	{
		m_delayhandler.PushRole(pRole->GetID(), expID);
	}
	return KKSG::ERR_SUCCESS;
}

int TeamMatchMgr::StartMatch(Team *pTeam)
{
	SSDebug << __FUNCTION__ << " teamID = " << pTeam->GetID() << END;

	if(pTeam->NeedMatchRole())
	{
		m_delayhandler.PushTeam(pTeam->GetID());
	}
	else if(pTeam->NeedMatchRobot())
	{
		DoFastMatch(pTeam);
	}
	else
	{
		SSWarn << "team start match reason error teamid = " << pTeam->GetID() << END;
		return KKSG::ERR_FAILED;
	}

	pTeam->SetMatching(true);

	return KKSG::ERR_SUCCESS;
}

void TeamMatchMgr::DoMatch(CRole* pRole, int expID)
{
	ExpeditionTable::RowData *pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if (pConf == NULL)
	{
		LogWarn("Not found expedition conf id : %d", expID);
		return;
	}
	auto pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if(NULL == pRoleSummary)
	{
		return;
	}
	// 有队伍，往队伍里放

	//其实可以根据Team::GetGuildID 来判断 
	UINT64 gID = 0;
	if(SMALL_GOBLIN_TEAM_SCENE == pConf->Type || GUILD_TEAM_SCENE == pConf->Type || pConf->Type == GUILDCAMP_TEAM_SCENE)
	{
		gID = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	}
	Team* pTeam = FindMatchTeamByExpID(expID, gID, pRoleSummary->GetPowerPoint());
	if (pTeam)
	{
		HMatchHandler handler;
		handler.Stop(pRole, KKSG::KMT_EXP);
		handler.Stop(pTeam, KKSG::KMT_EXP);

		TeamForceOp forceOp;
		std::vector<CRole*> roles;
		roles.push_back(pRole);
		forceOp.JoinTeam(pTeam->GetID(), roles);

		return ;
	}
	else
	{
		// 没有队伍，有2个单独匹配的玩家，为他们组一个队伍
		CRole *pHitRole = FindMatchRoleByExpID(expID, gID);
		if (pHitRole != NULL)
		{
			HMatchHandler handler;
			handler.Stop(pHitRole, KKSG::KMT_EXP);
			handler.Stop(pRole, KKSG::KMT_EXP);

			std::vector<CRole*> roles;
			roles.push_back(pHitRole);
			roles.push_back(pRole);
			TeamForceOp forceOp;
			forceOp.CreateTeam(roles, expID, TEAM_OPT_MATCH);
		}
		else
		{
			// 只有单独的一个匹配玩家
			SingleMatchRole sg;
			sg.expID = expID;
			sg.pRole = pRole;
			m_MatchRoles.push_back(sg);

			LoopCall(m_Listener, &TeamEventHandler::OnRoleStartMatch, pRole, pConf);
		}
	}
}

void TeamMatchMgr::DoMatch(Team* team)
{
	// 是否有单个匹配的人，可以往队伍里加
	CRole *pHitRole = FindMatchRoleByExpID(team->GetExpeditionID(), team->GetGuildID(), team->GetPPTLimit());
	if (pHitRole != NULL)
	{
		HMatchHandler handler;
		handler.Stop(pHitRole, KKSG::KMT_EXP);
		handler.Stop(team, KKSG::KMT_EXP);
		
		TeamForceOp forceOp;
		std::vector<CRole*> roles;	
		roles.push_back(pHitRole);
		forceOp.JoinTeam(team->GetID(), roles);

		return ;
	}

	m_MatchTeams[team->GetExpeditionID()].push_back(team);

	LoopCall(m_Listener, &TeamEventHandler::OnTeamStartMatch, team);
}

void TeamMatchMgr::DoFastMatch(CRole* pRole, int expID)
{
	ExpeditionTable::RowData *pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if (pConf == NULL)
	{
		LogWarn("Not found expedition conf id : %d", expID);
		return;
	}
	SingleMatchRole sg;
	sg.expID = expID;
	sg.pRole = pRole;
	m_MatchFastRoles.push_back(sg);

	LoopCall(m_Listener, &TeamEventHandler::OnRoleStartMatch, pRole, pConf);
}

void TeamMatchMgr::DoFastMatch(Team* team)
{
	m_MatchFastTeams[team->GetExpeditionID()].push_back(team);

	LoopCall(m_Listener, &TeamEventHandler::OnTeamStartMatch, team);
}

void TeamMatchMgr::StopMatch(Team *pTeam)
{
	//不判断Need 安全
	SSDebug << __FUNCTION__ << " " <<  pTeam->GetID() << END;

	m_delayhandler.PopTeam(pTeam->GetID());
	m_MatchTeams[pTeam->GetExpeditionID()].remove(pTeam);
	m_MatchFastTeams[pTeam->GetExpeditionID()].remove(pTeam);

	pTeam->SetMatching(false);

	LoopCall(m_Listener, &TeamEventHandler::OnTeamStopMatch, pTeam);
}

void TeamMatchMgr::StopMatch(CRole *pRole, int reason/* = KKSG::STOPMATCH_NONE*/)
{
	if(NULL == pRole)
	{
		SSError << " stop match role = null " << END;
		return ;
	}
	SSDebug << __FUNCTION__ << " roleid = " << pRole->GetID() << END;
	m_delayhandler.PopRole(pRole->GetID());
	//m_delayhandler.PopFastRole(pRole->GetID());
	foreach(i in m_MatchRoles)
	{
		if (i->pRole == pRole)
		{
			LoopCall(m_Listener, &TeamEventHandler::OnRoleStopMatch, pRole);
			m_MatchRoles.erase(i);
			break;
		}
	}
	for(auto it = m_MatchFastRoles.begin(); it != m_MatchFastRoles.end(); ++it)
	{
		if(it->pRole == pRole)
		{
			LoopCall(m_Listener, &TeamEventHandler::OnRoleStopMatch, pRole);
			m_MatchFastRoles.erase(it);
			break;
		}
	}

}

CRole * TeamMatchMgr::FindMatchRoleByExpID(int expID, UINT64 gID, UINT32 pptLimit/*= 0*/)
{
	CRole *pHitRole = NULL;
	foreach(i in m_MatchRoles)
	{
		if (i->expID == expID && NULL != i->pRole)
		{
			if(0 != gID && CGuildMgr::Instance()->GetGuildIdByRoleId(i->pRole->GetID()) != gID)
			{
				continue;
			}
			auto pRoleSummry = CRoleSummaryMgr::Instance()->GetRoleSummary(i->pRole->GetID());
			if(NULL == pRoleSummry)
			{
				continue;
			}
			if(pRoleSummry->GetPowerPoint() < pptLimit)
			{
				continue;
			}
			pHitRole = i->pRole;
			break;
		}
	}

	return pHitRole;
}

Team * TeamMatchMgr::FindMatchTeamByExpID(int expID, UINT64 gID, UINT32 roleppt)
{
	if(m_MatchTeams.find(expID) == m_MatchTeams.end())
	{
		return NULL;
	}
	std::list<Team*>& pTeams = m_MatchTeams[expID];
	for(auto it = pTeams.begin(); it != pTeams.end(); ++it)
	{
		Team* pT = (*it);
		if(NULL == pT || (0 != gID && pT->GetGuildID() != gID))
		{
			continue;
		}
		if(pT->GetPPTLimit() > roleppt)
		{
			continue;;
		}
		return pT;
	}
	return NULL;
}

void TeamMatchMgr::AddHandler(TeamEventHandler *handler)
{
	m_Listener.push_back(handler);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
DelayMatchHandler::DelayMatchHandler()
{

}
DelayMatchHandler::~DelayMatchHandler()
{

}

void DelayMatchHandler::PushRole(UINT64 roleid, int expid)
{
	DelayPairRole pair;
	pair.roleid = roleid;
	pair.expid = expid;
	pair.time = (UINT32)time(NULL);
	m_roles[roleid] = pair;
}

void DelayMatchHandler::PushTeam(int teamid)
{
	DelayPairTeam pair;
	pair.teamid = teamid;
	pair.time = (UINT32)time(NULL);
	m_teams[teamid] = pair;
}
	
/*
void DelayMatchHandler::PushFastRole(UINT64 roleid, int expid)
{
	DelayPairRole pair;
	pair.roleid = roleid;
	pair.expid = expid;
	pair.time = (UINT32)time(NULL);
	m_fastroles[roleid] = pair;
}
*/

bool DelayMatchHandler::PopRole(UINT64 roleid)
{
	auto i = m_roles.find(roleid);
	if (i != m_roles.end())
	{
		m_roles.erase(i);
		return true;
	}
	return false;
}

bool DelayMatchHandler::PopTeam(int teamid)
{
	auto i = m_teams.find(teamid);
	if (i != m_teams.end())
	{
		m_teams.erase(i);
		return true;
	}
	return false;
}

/*
bool DelayMatchHandler::PopFastRole(UINT64 roleid)
{
	auto i = m_fastroles.find(roleid);
	if (i != m_fastroles.end())
	{
		m_fastroles.erase(i);
		return true;
	}
	return false;
}
*/

std::list<DelayPairRole> DelayMatchHandler::PopRoles()
{
	UINT32 nowtime = (UINT32)time(NULL);
	std::list<DelayPairRole> roles;
	for (auto i = m_roles.begin(); i != m_roles.end();)
	{
		if (i->second.time + GetGlobalConfig().TeamMatchDelay < nowtime)
		{
			roles.push_back(i->second);
			m_roles.erase(i++);
		}
		else
		{
			++i;
		}
	}
	return roles;
}

std::list<DelayPairTeam> DelayMatchHandler::PopTeams()
{
	UINT32 nowtime = (UINT32)time(NULL);
	std::list<DelayPairTeam> teams;
	for (auto i = m_teams.begin(); i != m_teams.end();)
	{
		if (i->second.time + GetGlobalConfig().TeamMatchDelay < nowtime) 
		{
			teams.push_back(i->second);
			m_teams.erase(i++);
		}
		else
		{
			++i;
		}
	}
	return teams;
}

bool DelayMatchHandler::HaveRole(UINT64 roleid)
{
	if(m_roles.find(roleid) != m_roles.end())
	{
		return true;
	}
	return false;
}

/*
std::list<DelayPairRole> DelayMatchHandler::PopFastRoles()
{
	UINT32 nowtime = (UINT32)time(NULL);
	std::list<DelayPairRole> roles;
	for (auto i = m_fastroles.begin(); i != m_fastroles.end();)
	{
		if (i->second.time + GetGlobalConfig().TeamMatchDelay < nowtime)
		{
			roles.push_back(i->second);
			m_fastroles.erase(i++);
		}
		else
		{
			++i;
		}
	}
	return roles;
}
*/

FastMatchTempRoom::FastMatchTempRoom()
{
	m_time = time(NULL);
	m_addrobot = false;
}

void FastMatchTempRoom::AddRole(FastMatchInfo& mr)
{
	m_roles.push_back(mr);
}

bool FastMatchTempRoom::HaveRole(CRole* pRole)
{
	for(size_t i = 0; i< m_roles.size(); ++i)
	{
		if(m_roles[i].pRole == pRole)
		{
			return true;
		}
	}
	return false;
}

void FastMatchTempRoom::StartBattleConFirm()
{
	PtcM2C_FastMBConfirmM2CNtf ntf;
	for(size_t i = 0; i != m_roles.size(); ++i)
	{
		SSDebug << "start battle confirm " << m_roles[i].pRole->GetName() << END;
		m_roles[i].pRole->Send(ntf);
	}
}

void FastMatchTempRoom::StartBattleRefuse(CRole* quitRole, std::list<FastMatchInfo>& leftRoles)
{
	Dismiss(quitRole, leftRoles);

	PtcM2C_fastMBDismissM2CNtf ntf;
	ntf.m_Data.set_quitroleid(quitRole->GetID());
	ntf.m_Data.set_quitrolename(quitRole->GetName());

	for(auto it = leftRoles.begin(); it != leftRoles.end(); ++it)
	{
		SSDebug << "dismiss notify " << (*it).pRole->GetName() << " becaseof " << quitRole->GetName() << END;
		if((*it).pRole == quitRole)
		{
			continue;
		}
		(*it).pRole->Send(ntf);
	}
}

bool FastMatchTempRoom::WaitEnough()
{
	if(m_time + GetGlobalConfig().TeamFastMatchConfirmT <= time(NULL))
	{
		return true;
	}
	return false;
}

bool FastMatchTempRoom::RoleCountEnough()
{
	if(m_roles.empty())
	{
		return false;
	}
	FastMatchInfo& info = m_roles[0];
	if(info.pConf && info.pConf->PlayerLeastNumber <= (int)m_roles.size())
	{
		return true;
	}
	return false;
}

bool FastMatchTempRoom::RolecountFull()
{
	if(m_roles.empty())
	{
		return false;
	}
	FastMatchInfo& info = m_roles[0];
	if(info.pConf && info.pConf->PlayerNumber <= (int)m_roles.size())
	{
		return true;
	}
	return false;
}

void FastMatchTempRoom::Clear()
{
	m_roles.clear();
}

void FastMatchTempRoom::Dismiss(CRole* quitRole, std::list<FastMatchInfo>& leftRoles)
{
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		if(m_roles[i].pRole == quitRole)
		{
			continue;
		}
		leftRoles.push_back(m_roles[i]);
	}

}

void FastMatchTempRoom::StartBattle(bool addrobot/* = false */)
{
	//注意: 这里调用的函数里面不能修改 FastMatchHandler::m_fastMatchRooms 否则会破坏迭代器
	SSDebug << __FUNCTION__ << END;
	if(m_roles.empty())
	{
		return;
	}
	FastMatchInfo& info = m_roles[0]; // 使用第一个 向下兼容需要保证第一个最低
	if(NULL == info.pRole || NULL == info.pConf)
	{
		return;
	}

	std::vector<CRole*> tmp;
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		tmp.push_back(m_roles[i].pRole);
	}
	TeamForceOp forceOp;
	Team* pTeam = forceOp.CreateTeam(tmp, info.pConf->DNExpeditionID, TEAM_OPT_MATCH);
	if(pTeam)
	{
		if(m_addrobot)
		{
			pTeam->AddRobotAndNotify();
		}
		pTeam->StartBattle();
	}
}

void FastMatchTempRoom::FetchRoles(std::vector<CRole*>& roles)
{
	for(size_t i = 0; i < m_roles.size(); ++i)
	{
		roles.push_back(m_roles[i].pRole);
	}
}

void FastMatchTempRoom::TestPrint()
{
	SSWarn << "fash match temp room  time = " << time << END;
	for(auto it = m_roles.begin(); it != m_roles.end(); ++it)
	{
		(*it).TestPrint();
	}
}

FastMatchHandler::FastMatchHandler()
{
	m_handler = INVALID_HTIMER;
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	m_checkStartBattleLocked = false;
	m_downMatchTime = GetGlobalConfig().GetInt("TeamDownMatchTime" , 20);
}

FastMatchHandler::~FastMatchHandler()
{
	if(m_handler != INVALID_HTIMER)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
	}
}

void FastMatchHandler::OnTeamStartMatch(Team *pTeam)
{
	if(pTeam->NeedMatchRobot())
	{
		m_fastMatchTeams[pTeam] = time(NULL);
	}
}

void FastMatchHandler::OnTeamStopMatch(Team *pTeam)
{
	m_fastMatchTeams.erase(pTeam);
}

void FastMatchHandler::OnRoleStartMatch(CRole *pRole, ExpeditionTable::RowData *pConf)
{
	if(pRole && pConf && pConf->fastmatch)
	{
		FastMatchInfo info;
		info.pConf = pConf;
		info.pRole = pRole;
		info.time = time(NULL);
		m_matchRoles.push_back(info);
	}
	/*
	SSWarn << __FUNCTION__ << END;
	TestPrint();
	*/
}

void FastMatchHandler::OnRoleStopMatch(CRole *pRole)
{
	for(auto it = m_matchRoles.begin(); it != m_matchRoles.end(); ++it)
	{
		if(it->pRole == pRole)
		{
			m_matchRoles.erase(it);
			break;
		}
	}

	//加锁 防止重入 需要优化
	if(false == m_checkStartBattleLocked)
	{
		for(auto it = m_fastMatchRooms.begin(); it != m_fastMatchRooms.end(); ++it)
		{
			if(it->HaveRole(pRole))
			{
				std::list<FastMatchInfo> leftRoles;
				it->StartBattleRefuse(pRole, leftRoles);
				m_fastMatchRooms.erase(it);

				for(auto index = leftRoles.begin(); index != leftRoles.end(); ++index)
				{
					m_matchRoles.push_back((*index));//剩下的重新放回队列匹配
				}

				break;
			}
		}
	}
	/*
	SSWarn << __FUNCTION__ << END;
	TestPrint();
	*/
}

void FastMatchHandler::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckMatchRoom();
	CheckMatchRobotRoom();
	CheckDownMatch();
	CheckStartBattle();
	CheckMatchRobotTeam();
}

void FastMatchHandler::TestPrint()
{
	SSDebug << " fash match handler " << END;
	for(auto it = m_matchRoles.begin(); it != m_matchRoles.end(); ++it)
	{
		(*it).TestPrint();	
	}
	for(auto it = m_fastMatchRooms.begin(); it != m_fastMatchRooms.end(); ++it)
	{
		(*it).TestPrint();
	}
}

void FastMatchHandler::CheckMatchRoom()
{
	std::vector<int> pFastExpIDs = ExpeditionConfigMgr::Instance()->GetFastMatch();			
	for(size_t i = 0; i < pFastExpIDs.size(); ++i)
	{
		int expid = pFastExpIDs[i];
		FastMatchTempRoom pRoom;
		for(auto it = m_matchRoles.begin(); it != m_matchRoles.end(); )
		{
			if(it->pConf && it->pConf->DNExpeditionID == expid)
			{
				pRoom.AddRole((*it));
				m_matchRoles.erase(it++);
			}
			else
			{
				it++;
			}
			if(pRoom.RolecountFull())
			{
				pRoom.StartBattleConFirm();
				m_fastMatchRooms.push_back(pRoom);
				pRoom.Clear();
			}
		}

		//if(pRoom.RoleCountEnough())
		if(false)
		{
			pRoom.StartBattleConFirm();
			m_fastMatchRooms.push_back(pRoom);
			pRoom.Clear();
		}
		else
		{
			std::list<FastMatchInfo> pLeftRoles;
			pRoom.Dismiss(NULL, pLeftRoles);
			for(auto le = pLeftRoles.begin(); le != pLeftRoles.end(); ++le)
			{
				m_matchRoles.push_back((*le));
			}
			pRoom.Clear();
		}
	}

}

void FastMatchHandler::CheckStartBattle()
{
	m_checkStartBattleLocked = true;//加锁

	std::vector<CRole*> pStopMatchRoles;
	for(auto it = m_fastMatchRooms.begin(); it != m_fastMatchRooms.end();)
	{
		FastMatchTempRoom& pRoom = (*it);
		if(pRoom.WaitEnough())
		{
			pRoom.FetchRoles(pStopMatchRoles);
			pRoom.StartBattle();

			m_fastMatchRooms.erase(it++);
		}
		else
		{
			it++;
		}
	}

	HMatchHandler handler;
	for(size_t i = 0; i < pStopMatchRoles.size(); ++i)
	{
		handler.Stop(pStopMatchRoles[i], KKSG::KMT_EXP);
	}

	m_checkStartBattleLocked = false;
}

void FastMatchHandler::CheckMatchRobotRoom()
{
	time_t now = time(NULL);
	for(auto it = m_matchRoles.begin(); it != m_matchRoles.end(); )
	{
		FastMatchInfo& info = (*it);
		if(info.pRole && info.pConf && info.pConf->FMARobotTime > 0
			&& info.time + info.pConf->FMARobotTime <= now)
		{
			if(!RobotSummaryMgr::Instance()->HaveCanUseRobotSummary(info.pRole->GetID(), info.pConf->RobotLookupID))
			{
				RobotSummaryMgr::Instance()->RegisterRobotSummary(info.pRole->GetID(), info.pConf->RobotLookupID);
				it++;
				continue;
			}

			FastMatchTempRoom tmpRoom;
			tmpRoom.AddRole(info);
			tmpRoom.SetAddRobot();
			tmpRoom.StartBattleConFirm();
			
			m_fastMatchRooms.push_back(tmpRoom);

			m_matchRoles.erase(it++);	
		}
		else
		{
			it++;
		}
	}
}

void FastMatchHandler::CheckMatchRobotTeam()
{
	time_t now = time(NULL);

	std::vector<Team*> pNeedStopMatch;
	for(auto it = m_fastMatchTeams.begin(); it != m_fastMatchTeams.end(); ++it)
	{
		Team* pTeam = (it->first);
		if(NULL == pTeam || NULL == pTeam->GetConf())
		{
			pNeedStopMatch.push_back(pTeam);
			continue;
		}
		if((it->second) + pTeam->GetConf()->FMARobotTime <= now)
		{
			if(!RobotSummaryMgr::Instance()->HaveCanUseRobotSummary(pTeam->GetLeader(), pTeam->GetConf()->RobotLookupID))
			{
				RobotSummaryMgr::Instance()->RegisterRobotSummary(pTeam->GetLeader(), pTeam->GetConf()->RobotLookupID);
				continue;
			}

			pTeam->AddRobotAndNotify();
			pNeedStopMatch.push_back(pTeam);
		}
	}

	for(size_t i = 0; i < pNeedStopMatch.size(); ++i)
	{
		HMatchHandler handler;
		handler.Stop(pNeedStopMatch[i], KKSG::KMT_EXP);
	}
}

void FastMatchHandler::CheckDownMatch()
{
	//目前逻辑 只支持二人队伍

	time_t now = time(NULL);
	std::vector<int>& downMatch = ExpeditionConfigMgr::Instance()->GetDownMatch();
	
	std::vector<FastMatchInfo> tmpVec;

	for(size_t i = 0; i < downMatch.size(); ++i)
	{

		for(auto j = m_matchRoles.begin(); j != m_matchRoles.end();)
		{
			if(j->pConf && j->pConf->DNExpeditionID == downMatch[i] && j->downMatch)
			{
				tmpVec.push_back((*j));
				m_matchRoles.erase(j++);
			}
			else
			{
				j++;
			}
		}
	}
	
	for(size_t i = 0; i < tmpVec.size(); ++i)
	{
		if(i + 1 < tmpVec.size() && CanDownMatch(tmpVec[i], tmpVec[i+1], now))			
		{
			FastMatchTempRoom room;
			room.AddRole(tmpVec[i]);
			room.AddRole(tmpVec[i+1]);
			room.StartBattleConFirm();
			m_fastMatchRooms.push_back(room);

			++i;
		}
		else
		{
			m_matchRoles.push_back(tmpVec[i]);
		}
	}

}

void FastMatchHandler::SetRoleDownMatch(UINT64 roleID)
{
	for(auto it = m_matchRoles.begin(); it != m_matchRoles.end(); ++it)
	{
		if(it->pRole && it->pRole->GetID() == roleID)
		{
			it->downMatch = true;
			break;
		}
	}
}

bool FastMatchHandler::CanDownMatch(FastMatchInfo& info1, FastMatchInfo& info2, time_t now)
{
	if(NULL == info1.pConf || NULL == info2.pConf || 0 == m_downMatchTime 
		|| info1.pConf->Type != info2.pConf->Type || info1.pConf->DNExpeditionID >= info2.pConf->DNExpeditionID)
	{
		return false;
	}
	
	return ((now - info2.time) / m_downMatchTime) >= (info2.pConf->DNExpeditionID - info2.pConf->DNExpeditionID);

	return false;
}

void FastMatchInfo::TestPrint()
{
	SSWarn << "fast match info = " << END;
	SSWarn << time << " " << pRole->GetName() << " " << pConf->DNExpeditionID << END; 
}
