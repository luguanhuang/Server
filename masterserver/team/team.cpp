#include "pch.h"
#include "team.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "teammember.h"
#include "teammgr.h"
#include "foreach.h"
#include <time.h>
#include "team/ptcm2c_teamchangem2cntf.h"
#include "table/globalconfig.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "network/gslink.h"
#include "teammatchmgr.h"
#include "robotsummarymgr.h"
#include "team/rpcm2g_teamchangeexpcheck.h"
#include "common/ptcm2c_mserrornotify.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildgoblin.h"
#include "teamstartbattle.h"
#include "skycity/skycitymgr.h"
#include "team/rpcm2g_teamsynm2c.h"
#include "partner/partnermgr.h"
#include "leagueteam/leagueteammgr.h"
#include "other/matchhandler.h"
#include "skyteam/skyteammgr.h"
#include "config.h"

Team::Team()
{
	m_id = 0;
	m_leaderID = 0;
	m_teamState = KKSG::TEAM_WAITING;
	m_qwGuildID = 0;
	m_pConf = NULL;
	m_voteTime = 0;
	m_pptlimit = 0;
	m_costindex = TeamCostInvalidID;
	ResetLeaderLastBattleTime();
	m_matchType = KKSG::KMT_NONE;
	m_matchUniIndex = 0;

	LogInfo("create team at %x", this);
}

Team::~Team()
{
	while (!m_members.empty())
	{
		delete m_members.front();
		m_members.pop_front();
	}

	//TeamMgr::Instance()->RemoveTeam(this);

	LogInfo("delete team %d at %x", m_id, this);
}

void Team::Add(UINT64 qwRoleID)
{
	CTeamMember *member = new CTeamMember(qwRoleID);
	m_members.push_back(member);

	//SynsMsTeam();
}

void Team::AddRobotAndNotify()
{
	if(NULL == m_pConf)
	{
		return ;
	}
	RobotSummary* pRobotSummary = RobotSummaryMgr::Instance()->GetRobotSummary(m_leaderID, m_pConf->RobotLookupID);
	if (NULL == pRobotSummary)
	{
		SSError << " rolesummary = NULL " << END;
		return;
	}

	CTeamMember *member = new CTeamMember(pRobotSummary->GetRobotID());
	member->GenerateRoleSummry(pRobotSummary->GetData());
	m_members.push_back(member);

	RobotSummaryMgr::Instance()->FreeRobotSummary(m_leaderID);

	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	member->SaveToTeamMember(oPtcNtf.m_Data.add_addmember());
	BroadCast(oPtcNtf);

	TeamMgr::Instance()->SynAllData2GS(this);
}

void Team::RemoveRobotAndNotify(UINT64 qwRoleID)
{
	Remove(qwRoleID);

	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	oPtcNtf.m_Data.add_leavemember(qwRoleID);
	BroadCast(oPtcNtf);

	TeamMgr::Instance()->SynAllData2GS(this);
}

void Team::Remove(UINT64 qwRoleID)
{
	auto i = FindMemberIterator(qwRoleID);
	if (i != m_members.end())
	{
		delete (*i);
		m_members.erase(i);
	}
	else
	{
		SSWarn << "not found role " << qwRoleID << " in team " << m_id << END;
	}
}

void Team::SetLeader(UINT64 qwRoleID, const std::string &leaderName)
{
	m_leaderID = qwRoleID;
	m_leaderName = leaderName;
}

UINT64 Team::GetLeader()
{
	return m_leaderID;
}

void Team::Foreach(TeamCB cb, void *arg)
{
	foreach(i in m_members)
	{
		cb(*i, arg);
	}
}

void Team::Foreach(TeamOnLineCB cb, void *arg)
{
	foreach(i in m_members)
	{
		if (!(*i)->IsRobot())
		{
			CRole *pRole = CRoleManager::Instance()->GetByRoleID((*i)->m_qwRoleID);
			if (pRole != NULL)
			{
				cb(*i, pRole, arg);
			}
		}
	}
}

void Team::SaveBrief(KKSG::TeamBrief *pTeamBrief, bool needpw/* = true*/)
{
	pTeamBrief->set_teamid(m_id);
	pTeamBrief->set_teammembercount(GetMemberCount());
	pTeamBrief->set_leadername(m_leaderName);
	pTeamBrief->set_expid(GetExpeditionID());
	pTeamBrief->set_teamstate(m_teamState);
	pTeamBrief->set_haspassword(!m_password.empty());
	if(needpw)
	{
		pTeamBrief->set_password(m_password);
	}
	if(KKSG::KMatchType_IsValid(m_matchType))
	{
		pTeamBrief->set_matchtype(KKSG::KMatchType(m_matchType));
	}
	pTeamBrief->set_kingback(IsKingBack());

	KKSG::TeamExtraInfo* info = pTeamBrief->mutable_extrainfo();
	info->set_costindex(m_costindex);
	info->set_pptlimit(m_pptlimit);
	info->set_league_teamname(m_leagueTeamName);

	CRoleSummary *pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_leaderID);
	if (pSummary)
	{
		pTeamBrief->set_leaderlevel(pSummary->GetLevel());
		pTeamBrief->set_leaderprofession((KKSG::RoleType)pSummary->GetProfession());
		pTeamBrief->set_leaderpowerpoint(pSummary->GetPowerPoint());
	}
	else
	{
		pTeamBrief->set_leaderlevel(0);
		pTeamBrief->set_leaderprofession((KKSG::Role_INVALID));
		pTeamBrief->set_leaderpowerpoint(0);
	}
}

struct BroadCastParam
{
	CProtocol *proto;
	UINT64 exludeRoleID;
};

static void BroadCastSend(CTeamMember *pMember, CRole *pRole, void *arg)
{
	BroadCastParam *param = (BroadCastParam *)arg;
	if (pMember->m_qwRoleID != param->exludeRoleID)
	{
		pRole->Send(*param->proto);
	}
}

void Team::BroadCast(CProtocol &proto, UINT64 qwExludeRole)
{
	BroadCastParam param;
	param.proto = &proto;
	param.exludeRoleID = qwExludeRole;
	Foreach(BroadCastSend, &param);
}

std::list<CTeamMember *>::iterator Team::FindMemberIterator(UINT64 qwRoleID)
{
	foreach(i in m_members)
	{
		if ((*i)->m_qwRoleID == qwRoleID)
			return i;
	}

	return m_members.end();
}

CTeamMember *Team::FindMember(UINT64 qwRoleID)
{
	auto i = FindMemberIterator(qwRoleID);
	return (i == m_members.end()) ? NULL : (*i);
}

const std::list<CTeamMember *>& Team::GetMember()
{
	return m_members;
}

void Team::StartBattle()
{
	SSDebug << __FUNCTION__ << " " << GetID() << END;
	if(KKSG::TEAM_MATCH == m_teamState)
	{
		SSError << "match team start battle " << END;
		return ;
	}

	int nextState = KKSG::TEAM_IN_BATTLE;
	auto pConf = GetConf();
	switch(pConf->Type)
	{
	case SKYCITY_SCENE:
		{
			SkyCityMgr::Instance()->AddTeam(this);
		}
		break;
	case PARTNER_TEAM_SCENE:
		{
			std::vector<UINT64> roles;
			GetRoleIDs(roles);
			PartnerMgr::Instance()->StartMakePartner(roles);
			nextState = KKSG::TEAM_WAITING;
		}
		break;
	case LEAGUETEAM_SCENE:
		{
			LeagueTeamMgr::Instance()->TeamStartBattle(this);
			nextState = KKSG::TEAM_WAITING;
		}
		break;
	case SKYCRAFT_SCENE:
		{
			SkyTeamMgr::Instance()->StartCreateTeam(this);
			nextState = KKSG::TEAM_WAITING;
		}
		break;
	default:
		{
			TeamStartBattle tsb(this);
			bool ret = tsb.StartBattle();
		}
		break;
	}

	SetTeamState(nextState);
	NotifyTeamStateChanged();

	InfoPrint();
}

void Team::ResetMemberState(bool notify/* = true*/)
{
	foreach( i in m_members)
	{
		(*i)->MemberState = KKSG::EXPTEAM_IDLE;
	}

	if(notify)
	{
		NotifyAllMemberStateChanged();
	}
}

void Team::RemoveAllRobotAndNotify()
{//只有一个机器人

	for (auto i = m_members.begin(); i != m_members.end(); ++i)
	{
		if ((*i)->IsRobot())
		{
			RemoveRobotAndNotify((*i)->m_qwRoleID);
			return ;//迭代器已改变 需要马上return
		}
	}
}

void Team::SetMatching(bool match)
{
	if (match == true)
	{
		m_teamState = KKSG::TEAM_MATCH;
	}
	else
	{
		m_teamState = KKSG::TEAM_WAITING;
	}

	NotifyTeamStateChanged();
}

void Team::NotifyTeamStateChanged()
{
	PtcM2C_TeamChangeM2CNtf oPtc;
	SaveBrief(oPtc.m_Data.mutable_teambrief());
	BroadCast(oPtc);
}

void Team::NotifyMemberStateChanged(CTeamMember *pMember, CRole *pHitRole)
{
	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	pMember->SaveToTeamMember(oPtcNtf.m_Data.add_chgstatemember());
	BroadCast(oPtcNtf);
}

void Team::NotifyAllMemberStateChanged()
{
	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	foreach( i in m_members)
	{
		CTeamMember *pMember = *i;
		pMember->SaveToTeamMember(oPtcNtf.m_Data.add_chgstatemember());
	}
	BroadCast(oPtcNtf);
}

void Team::UpdateMemberInfo(CRole *pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	CTeamMember *pMember = FindMember(pRole->GetID());
	if (pMember)
	{
		NotifyMemberStateChanged(pMember, pRole);
	}
}

void Team::UpdateMemberExtraInfo(CRole* pRole, const KKSG::TeamSynExtraData& data)
{
	if(NULL == pRole)
	{
		return ;
	}
	CTeamMember *pMember = FindMember(pRole->GetID());
	if (pMember)
	{
		pMember->SetExtraData(data);
		NotifyMemberStateChanged(pMember, pRole);
	}
	
}

void Team::ChangeLeaderTo(UINT64 roleID)
{//roleID = 0 表示 任意
	bool succesed = false;
	foreach(i in m_members)
	{
		if ((*i)->IsRobot())
		{
			continue;
		}
		if(roleID && roleID != (*i)->m_qwRoleID)
		{
			continue;
		}

		UINT64 qwRoleID = (*i)->m_qwRoleID;
		m_leaderID = qwRoleID;
		m_leaderName = CRoleSummaryMgr::Instance()->GetRoleName(m_leaderID);
		
		succesed = true;
		break;
	}

	if(succesed)
	{
		ResetHelperState(m_leaderID);	
	}
	else
	{
		SSDebug << " find not next leader team id = " << GetID() << " quit leaderID = " << m_leaderID << END;

		if(m_members.empty())
		{
			return ;
		}
		m_leaderID = m_members.front()->m_qwRoleID;
		m_leaderName = CRoleSummaryMgr::Instance()->GetRoleName(m_leaderID);
	}
}

int Team::GetAVGPPT()
{
	int N = 0;
	int PPT = 0;
	foreach (i in m_members)
	{
		CRoleSummary *pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary((*i)->m_qwRoleID);
		if (pSummary)
		{
			++N;
			PPT += pSummary->GetPowerPoint();
		}
	}

	if (N > 0)
	{
		return PPT/N;
	}
	else
	{
		return 0;
	}
	return 0;
}

int Team::GetLeaderLevel()
{
	CRoleSummary *pSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(m_leaderID);
	if (pSummary)
	{
		return pSummary->GetLevel();
	}
	else
	{
		return 0;
	}
	return 0;
}

//int Team::GetDayCountLeft(CRole *pRole, ExpeditionTable::RowData *pConf)
//{
//	/*
//	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pConf->Type);
//	if(pHandler)
//	{
//		return pHandler->GetDayCountLeft(pRole , pConf);
//	}
//	*/
//	return 0;
//}
//
//int Team::GetDayCountLeft(CRole *pRole, int Type)
//{
//	/*
//	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(Type);
//	if(pHandler)
//	{
//		return pHandler->GetDayCountLeft(pRole);
//	}
//	*/
//	return 0;
//}
//
//int Team::GetDayCountLeft(ExpeditionTable::RowData *pConf)
//{
//	/*
//	ITeamTypeHandlerBase * pHandler = ITeamTypeHandlerBase::GetTeamHanderByType(pConf->Type);
//	if(pHandler)
//	{
//		return pHandler->GetDayCountLeft(pConf);
//	}
//	*/
//	return 0;
//}

bool Team::HasRobotInTeam()
{
	foreach (i in m_members)
	{
		if ((*i)->IsRobot())
			return true;
	}

	return false;
}

int Team::GetMemberCountWithoutRobot()
{
	int Count = 0;
	foreach (i in m_members)
	{
		if (!(*i)->IsRobot())
		{
			++Count;
		}
	}
	return Count;
}

void Team::OnAllSceneEnd()
{
	SSDebug << __FUNCTION__ << " " << GetID() << END;
	LogInfo("reset team %d state to waiting!", m_id);
	m_teamState = KKSG::TEAM_WAITING;
	SetUseTicket(0, 0);//重置使用门票状态
	ResetMemberState();
	///> 重置赏金团
	if (TeamCostInvalidID != m_costindex)
	{
		SetCostIndex(TeamCostInvalidID);
		ResetCostNtf2G();
	}
}

const char * Team::GetTeamStateStr()
{
	switch(m_teamState)
	{
	case KKSG::TEAM_WAITING: return "<waiting>";
	case KKSG::TEAM_IN_BATTLE: return "<battle>";
	case KKSG::TEAM_VOTE: return "<vote>";
	case KKSG::TEAM_MATCH: return "<match>";
	}

	return "<unknow>";
}

int Team::GetTeamStateErrorCode()
{
	switch(m_teamState)
	{
	case KKSG::TEAM_MATCH: return KKSG::ERR_TEAM_MATCHING;
	case KKSG::TEAM_IN_BATTLE: return KKSG::ERR_TEAM_IN_BATTLE;
	case KKSG::TEAM_VOTE: return KKSG::ERR_TEAM_VOTE;
	}

	return KKSG::ERR_TEAM_STATE_ERROR;
}

/*
ITeamTypeHandlerBase * Team::GetTeamHandler()
{
	if(m_pConf)
	{
		return ITeamTypeHandlerBase::GetTeamHanderByType(m_pConf->Type);
	}
	return NULL;
}
*/

/*
void Team::CreateRobotAt(Scene *pScene, const Vector3&pos, float face)
{
	foreach (i in m_members)
	{
		CTeamMember *pMember = *i;
		if (pMember->m_pRobotSummary)
		{
			DummyRole *pAlly = DummyRoleManager::Instance()->Create(pMember->m_pRobotSummary, pos , face, (UINT32)(KKSG::FightRole), (float)m_pConf->RobotLifePercent, SCENE_ALLY);
			pScene->AddUnitAndNotify(pAlly);
		}
	}
}
*/

UINT64 Team::GetRobotID()
{
	foreach (i in m_members)
	{
		if ((*i)->IsRobot())
		{
			return (*i)->m_qwRoleID;
		}
	}

	return 0;
}


void Team::OnRoleLeaveBattle(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	CheckFinishBattle();
	CTeamMember* mem = FindMember(pRole->GetID());
	if(mem)
	{
		NotifyMemberStateChanged(mem, pRole);
	}
	TeamMgr::Instance()->SynGetExtraData(this, pRole->GetID());
}

void Team::CheckFinishBattle()
{
	if(KKSG::TEAM_IN_BATTLE != m_teamState)
	{
		return ;
	}
	if(AllMemLeaveBattle())
	{
		ChangeTeamExpIDCheck();

		OnAllSceneEnd();

		ResetLeaderLastBattleTime();

	}
}

bool Team::AllMemLeaveBattle()
{
	UINT32 leaveCount = 0;
	UINT32 robotCount = 0;
	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		if((*it)->IsRobot())
		{
			++robotCount;
			continue;
		}
		CRole* pRole = CRoleManager::Instance()->GetByRoleID((*it)->m_qwRoleID);
		if(pRole && !pRole->IsInBattle())
		{
			++leaveCount;
		}
	}
	return (leaveCount + robotCount == m_members.size());
}

void Team::SetInBattleState()
{
	SSDebug << __FUNCTION__ << " " << GetID() << END;
	m_teamState = KKSG::TEAM_IN_BATTLE;
}

bool Team::NeedStartMatch()
{
	if(!AllMemLeaveBattle())
	{
		return false;
	}

	int type = GetConf()->Type;
	if(SKYCITY_SCENE == type 
		|| RESWAR_SCENE == type 
		|| HEROBATTLE_TEAM_SCENE == type
		|| PVP_TEAM_SCENE == type
		|| TEAM_TYPE_PKTWO == type
		|| TEAM_TYPE_CUSTOMPKTWO == type
		|| LEAGUETEAM_SCENE == type
		)
	{
		return false;
	}

	if(NeedMatchRole() || NeedMatchRobot())
	{
		return true;
	}
	return false;
}

bool Team::NeedStopMatch()
{
	if(KKSG::KMT_EXP == GetMatchType() && IsFull())
	{
		return true;
	}

	return false;
}

bool Team::NeedMatchCommon()
{
	if(NULL == m_pConf)
	{
		return false;
	}
	if(IsFull())
	{
		return false;
	}
	if(KKSG::TEAM_WAITING != m_teamState)
	{
		return false;
	}
	if(KKSG::KMT_NONE != GetMatchType())
	{
		return false;
	}
	if(!m_password.empty())
	{
		return false;
	}
	return true;
}

bool Team::NeedMatchRole()
{
	if(!NeedMatchCommon())
	{
		return false;
	}
	if(m_pConf->fastmatch)
	{
		return false;
	}

	return true;
}

bool Team::NeedMatchRobot()
{
	if(!NeedMatchCommon())
	{
		return false;
	}
	if(0 == m_pConf->fastmatch)
	{
		return false;
	}

	if(m_pConf->FMARobotTime <= 0)
	{
		return false;
	}

	if(HasRobotInTeam())
	{
		return false;
	}

	if(GetMemberCountWithoutRobot() != 1)
	{
		return false;
	}
	
	return true;
}

void Team::ChangeTeamExpIDCheck()
{
	RpcM2G_TeamChangeExpCheck* rpc = RpcM2G_TeamChangeExpCheck::CreateRpc();
	SaveSynData(*rpc->m_oArg.mutable_teamalldata());
	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
}

void Team::GetSessions(std::vector<SessionType>& sessions)
{
	for (auto i = m_members.begin(); i != m_members.end(); ++i)
	{
		if((*i)->IsRobot())
		{
			continue;
		}
		CRole* role = CRoleManager::Instance()->GetByRoleID((*i)->m_qwRoleID);
		if (NULL != role)
		{
			sessions.push_back(role->GetSessionID());
		}
	}
}

void Team::InfoPrint()
{
	SSInfo << " teamid = " << GetID() << " leader = " << GetLeader() << " expid = " << GetExpeditionID() << " state = " << GetTeamState() << END;
	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		(*it)->InfoPrint();
	}
}

void Team::SaveSynData(KKSG::TeamSynAll& data)
{
	data.set_teamid(GetID());
	data.set_expid(GetExpeditionID());
	data.set_leaderid(GetLeader());
	data.set_guildid(GetGuildID());
	data.set_pptlimit(m_pptlimit);
	data.set_teamcost(m_costindex);	
	data.set_haspassword(!m_password.empty());

	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		KKSG::TeamSynMember* tmp = data.add_members();	
		tmp->set_roleid((*it)->RoleID());
		tmp->set_memtype((*it)->GetMemType());
		tmp->set_serverid(MSConfig::Instance()->GetServerID());
	}
}

void Team::ResetHelperState(UINT64 roleid/* = 0*/)
{
	if(0 != roleid)
	{
		CTeamMember* pMember = FindMember(roleid);
		if(pMember && pMember->IsHelper())
		{
			pMember->SetIsHelper(false);
			if(GetLeader() == roleid)
			{
				PtcM2C_MSErrorNotify ntf;
				ntf.m_Data.set_errorno(KKSG::ERR_TEAM_LEADER_NOTHELPER);
				CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
				if(pRole)
				{
					pRole->Send(ntf);
				}
			}
		}
	}
	else
	{
		for(auto it = m_members.begin(); it != m_members.end(); ++it)
		{
			(*it)->SetIsHelper(false);
		}
	}
}

void Team::SetBeginVote()
{
	m_teamState = KKSG::TEAM_VOTE; 
	m_voteTime = time(NULL);
}

void Team::SetMemberVoteState(CRole *pRole, int state)
{
	// 设置自己的状态
	CTeamMember *pMember = FindMember(pRole->GetID());
	if(NULL == pMember)
	{
		return ;
	}
	if(KKSG::EXPTEAM_IDLE != pMember->MemberState)
	{
		SSInfo << " repeat set member state " << END;
		return;
	}
	pMember->MemberState = state;

	// 通知所有队员状态改变
	NotifyMemberStateChanged(pMember, pRole);

	CheckVoteResult();
}

void Team::VoteUpdate(time_t Now)
{
	if (Now - m_voteTime > GetGlobalConfig().TeamVoteTime + 1)
	{
		int count = 0;
		foreach( i in m_members)
		{
			CTeamMember *pMember = *i;
			if (pMember->MemberState == KKSG::EXPTEAM_IDLE)
			{
				if(m_pConf && m_pConf->AutoRefuse)
				{
					pMember->MemberState = KKSG::EXPTEAM_DISAGREE;
				}
				else
				{
					pMember->MemberState = KKSG::EXPTEAM_READY;
				}
			}
		}

		NotifyAllMemberStateChanged();
		CheckVoteResult();
	}
}

void Team::CheckVoteResult()
{
	SSDebug << __FUNCTION__ << END;
	bool AllReady = true;
	bool HasDisAgree = false;
	foreach(i in m_members)
	{
		if ((*i)->MemberState != KKSG::EXPTEAM_READY)
		{
			AllReady = false;
		}
		if((*i)->MemberState == KKSG::EXPTEAM_DISAGREE)
		{
			HasDisAgree = true;
		}
	}

	//玩家状态 在使用的时候才会重置
	if (AllReady)
	{
		StartBattle();
	}
	else if (HasDisAgree)
	{
		SetTeamState(KKSG::TEAM_WAITING);
		NotifyTeamStateChanged();
	}
}

void Team::Update()
{
	time_t now = time(NULL);
	if (KKSG::TEAM_VOTE == m_teamState)
	{
		VoteUpdate(now);
	}
}

void Team::OnStartBattleFailed()
{
	SetTeamState(KKSG::TEAM_WAITING);
	NotifyTeamStateChanged();
}

void Team::OnChangeLeader(UINT64 roleID)
{
	ResetTeamSetting();
	ChangeLeaderTo(roleID);
	ResetLeaderLastBattleTime();
}

void Team::OnChangeExpId()
{
	ResetTeamSetting();
}

void Team::ResetCostNtf2G()
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_leaderID);
	if (NULL != role)
	{
		RpcM2G_TeamSynM2C* rpc = RpcM2G_TeamSynM2C::CreateRpc();
		rpc->m_oArg.set_leaderid(m_leaderID);
		rpc->m_oArg.set_type(KKSG::STM2C_RESETCOST);

		GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);
	}
}

void Team::ResetTeamSetting()
{
	if (TeamCostInvalidID != m_costindex)
	{
		ResetCostNtf2G();
	}
	m_pptlimit = 0;
	m_costindex = TeamCostInvalidID;
}

void Team::GetRoleIDs(std::vector<UINT64>& roleIDs)
{
	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		if(!(*it)->IsRobot())
		{
			roleIDs.push_back((*it)->m_qwRoleID);
		}
	}
}

void Team::ResetLeaderLastBattleTime()
{
	m_lastLeaderBattleTime = time(NULL);
}

time_t Team::GetLeaderLastBattleTime()
{
	return m_lastLeaderBattleTime;
}

bool Team::NeedKickLeader(time_t now)
{
	if(( KKSG::TEAM_WAITING == m_teamState || KKSG::TEAM_MATCH == m_teamState) 
		&& m_pConf->KickLeaderTime && GetLeaderLastBattleTime() + m_pConf->KickLeaderTime <= now)
	{
		return true;
	}
	return false;
}

UINT32 Team::GetLeaderLeftCount()
{
	auto pMem = FindMember(m_leaderID);
	if(pMem)
	{
		return pMem->LeftCount();
	}
	return 0;
}

//此判断依赖TeamMgr::OnTimer的间隔(1s)
bool Team::NeedKickLeaderBeforeNotice(time_t now, UINT32 beforeTime)
{
	if(( KKSG::TEAM_WAITING == m_teamState || KKSG::TEAM_MATCH == m_teamState) 
		&& m_pConf->KickLeaderTime && GetLeaderLastBattleTime() + m_pConf->KickLeaderTime == now + beforeTime)
	{
		return true;
	}
	return false;
}
	
void Team::SetMatchType(int type) 
{ 
	SSInfo << " match team = " << GetID() << " old = " << m_matchType << " new = " << type << END;
	m_matchType = type; 
	m_matchUniIndex = HMatchHandler::GetMatchUniqueIndex(true);
	NotifyTeamStateChanged();
}

void Team::SetUseTicket(int useTicket, UINT64 roleID /*= 0*/)
{
	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		auto pMem = (*it);
		if(0 == roleID || pMem->RoleID() == roleID)
		{
			pMem->SetUsetTicket(useTicket);
		}
	}
}

bool Team::IsKingBack()
{
	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		if((*it)->IsKingBack())
		{
			return true;
		}
	}
	return false;
}

//void Team::UseTicketNtf(UINT64 roleID)
//{
//	auto pMember = FindMember(roleID);
//	if(NULL == pMember)
//	{
//		return;	
//	}
//	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
//	if(NULL == pRole)
//	{
//		return;
//	}
//
//	PtcM2C_TeamUseTicketNtf ptc;
//	ptc.m_Data.set_useticket(pMember->UseTicket());
//	pRole->Send(ptc);
//}
