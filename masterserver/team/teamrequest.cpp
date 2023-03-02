#include "pch.h"
#include "teamrequest.h"
#include "teammgr.h"
#include "team.h"
#include "table/expeditionconfigmgr.h"
#include "team.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include <time.h>
#include "foreach.h"
#include "scene/scene.h"
#include "role/role.h"
#include "teammember.h"
#include "role/rolemanager.h"
#include "team/ptcm2c_teamchangem2cntf.h"
#include "team/ptcm2c_leaveteamm2cntf.h"
#include "team/ptcm2c_teamfulldatam2cntf.h"
#include "teaminvitemgr.h"
#include "teammatchmgr.h"
#include "skycity/skycitymgr.h"
#include "table/MultActivityMgr.h"
#include "guild/guildcamp.h"
#include "reswar/reswarmgr.h"
#include "nestweek/nestweekmgr.h"
#include "partner/partnermgr.h"
#include "herobattle/herobattlemgr.h"
#include "util/XCommon.h"
#include "team/rpcm2g_teamrequestm2g.h"
#include "network/gslink.h"
#include "leagueteam/leagueteammgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "teamhandler.h"
#include "other/matchhandler.h"
#include "levelseal/levelsealMgr.h"
#include "team/teamtranshandler.h"
#include "team/teamcommon.h"

TeamRequest::TeamRequest(CRole *pRole)
{
	m_pRole = pRole;
	//m_pTeam = NULL;
}

TeamRequest::~TeamRequest()
{

}

struct BuildFullTeamParam
{
	KKSG::TeamFullDataNtf *pData;
	ExpeditionTable::RowData *pConf;
};

static void BuildFullTeamMemberCB(CTeamMember *pMember, void *arg)
{
	BuildFullTeamParam *param = (BuildFullTeamParam *)arg;
	pMember->SaveToTeamMember(param->pData->add_members());
}

void TeamRequest::QueryFullData()
{
	KKSG::TeamFullDataNtf fullData;

	auto pTeam = GetTeamInMs();
	if (pTeam)
	{
		fullData.set_hasteam(true);

		pTeam->SaveBrief(fullData.mutable_teambrief());

		BuildFullTeamParam param;
		param.pData = &fullData;
		param.pConf = pTeam->GetConf();
		pTeam->Foreach(BuildFullTeamMemberCB, &param);
	}
	else
	{
		fullData.set_hasteam(false);
	}

	TeamTransHandler trans;
	trans.HandleTeamFullData(m_pRole, fullData);
}

int TeamRequest::CreateExpenditionTeam(UINT32 expID, TeamOpType optype, const std::string &password)
{
	if (m_pRole->HasTeam())
	{
		return KKSG::ERR_TEAM_ALREADY_INTEAM;
	}

	ExpeditionTable::RowData *pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if (NULL == pRow)
	{
		return KKSG::ERR_TEAM_EXPEDITIONID_NOT_EXIST;
	}

	if(TEAM_OPT_NORMAL == optype)
	{
		int ret = m_handler.CreateCond(m_pRole, pRow);
		TJUDGERET(ret)
	}
	
	Team *pTeam = TeamMgr::Instance()->CreateTeam(expID);
	pTeam->Add(m_pRole->GetID());
	pTeam->SetConf(pRow);
	pTeam->SetLeader(m_pRole->GetID(), m_pRole->GetName());
	pTeam->SetPassword(password);
	if(pRow->GuildLevel > 0)//要求公会
	{
		pTeam->SetGuildID(CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID()));
	}

	m_pRole->SetTeamID(pTeam->GetID());

	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	pTeam->SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	CTeamMember *pMember = pTeam->FindMember(m_pRole->GetID());
	pMember->SaveToTeamMember(oPtcNtf.m_Data.add_addmember());
	pTeam->BroadCast(oPtcNtf);

	TeamMgr::Instance()->SynAllData2GS(pTeam);
	TeamMgr::Instance()->SynGetExtraData(pTeam, m_pRole->GetID());

	return KKSG::ERR_SUCCESS;
}

struct BuildTeamParam
{
	KKSG::TeamChanged *pData;
	ExpeditionTable::RowData *pConf;
};

static void BuildTeamMemberCB(CTeamMember *pMember, void *arg)
{
	BuildTeamParam *param = (BuildTeamParam *)arg;
	pMember->SaveToTeamMember(param->pData->add_addmember());
}

int TeamRequest::JoinTeam(int teamID, TeamOpType optype, const std::string &password, bool checkpassword)
{
	SSInfo << __FUNCTION__ << " role = " <<  m_pRole->GetID() << " team = " << teamID << END;

	if (m_pRole->HasTeam())
	{
		return KKSG::ERR_TEAM_ALREADY_INTEAM;
	}

	Team *pTeam = TeamMgr::Instance()->FindTeam(teamID);
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (pTeam->IsFull())
	{
		return KKSG::ERR_TEAM_FULL;
	}

	if(checkpassword && !pTeam->GetPassword().empty())
	{
		if (password != pTeam->GetPassword())
		{
			return KKSG::ERR_TEAM_WRONG_PASSWORD;
		}
	}

	if(pTeam->GetMatchType() != KKSG::KMT_NONE && pTeam->GetMatchType() != KKSG::KMT_EXP)
	{
		return KKSG::ERR_PVP_TEAM_MATCH;
	}

	if(TEAM_OPT_NORMAL == optype)
	{
		int ret = m_handler.JoinCond(m_pRole, pTeam);
		TJUDGERET(ret)
	}

	m_handler.OnJoinTeam(m_pRole, pTeam);

	pTeam->Add(m_pRole->GetID());
	m_pRole->SetTeamID(teamID);


	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	pTeam->SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	CTeamMember *pMember = pTeam->FindMember(m_pRole->GetID());
	pMember->SaveToTeamMember(oPtcNtf.m_Data.add_addmember());
	pTeam->BroadCast(oPtcNtf, m_pRole->GetID());

	BuildTeamParam param;
	param.pData = &oPtcNtf.m_Data;
	param.pConf = pTeam->GetConf();
	oPtcNtf.m_Data.clear_addmember();
	pTeam->Foreach(BuildTeamMemberCB, &param);
	m_pRole->Send(oPtcNtf);

	TeamMgr::Instance()->SynAllData2GS(pTeam);
	TeamMgr::Instance()->SynGetExtraData(pTeam, m_pRole->GetID());

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::KickTeam(UINT64 roleID)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_TEAM_ONLY_LEADER_CAN_KICK;
	}

	if (!TeamInState(pTeam, KKSG::TEAM_WAITING) && !TeamInState(pTeam, KKSG::TEAM_MATCH))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	CTeamMember *pMember = pTeam->FindMember(roleID);
	if (pMember == NULL)
	{
		return KKSG::ERR_TEAM_MEMBER_NOT_EXIST;
	}

	if (pMember->IsRobot())
	{
		pTeam->RemoveRobotAndNotify(roleID);

		return KKSG::ERR_SUCCESS;
	}
	else
	{
		CRole *pOther = CRoleManager::Instance()->GetByRoleID(roleID);
		if (pOther == NULL)
		{
			return KKSG::ERR_TEAM_MEMBER_NOT_EXIST;
		}

		TeamRequest tq(pOther);
		return tq.LeaveTeam(KKSG::LTT_KICK);
	}
}

//离队必须保证成功
int TeamRequest::LeaveTeam(int reason)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	SSInfo << __FUNCTION__ << " role = " << m_pRole->GetID() << " team = " << pTeam->GetID() << END;

	m_handler.OnLeaveTeam(m_pRole, pTeam);

	//m_pTeam->SetUseTicket(0, m_pRole->GetID());

	bool LeaderLeave = IsLeader(pTeam);

	PtcM2C_LeaveTeamM2CNtf oLeavePtc;
	oLeavePtc.m_Data.set_errorno(reason);
	m_pRole->Send(oLeavePtc);

	m_pRole->SetTeamID(0);

	pTeam->Remove(m_pRole->GetID());

	if (LeaderLeave)
	{
		pTeam->OnChangeLeader(0);
	}
	///> 所有人都离开了解散队伍
	if (pTeam->GetMemberCountWithoutRobot() == 0)
	{
		TeamMgr::Instance()->DestoryTeam(pTeam);
		pTeam = NULL;
		return KKSG::ERR_SUCCESS;
	}

	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	pTeam->SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	oPtcNtf.m_Data.add_leavemember(m_pRole->GetID());
	if(LeaderLeave)
	{
		CTeamMember* pMember = pTeam->FindMember(pTeam->GetLeader());
		if(pMember)
		{
			pMember->SaveToTeamMember(oPtcNtf.m_Data.add_chgstatemember());
		}
	}
	pTeam->BroadCast(oPtcNtf);

	TeamMgr::Instance()->SynAllData2GS(pTeam);

	return KKSG::ERR_SUCCESS;
}

void TeamRequest::FilterTeam(const std::list<int> &Teams, TeamListCB cb, void *arg)
{
	if (Teams.empty())
	{
		return;
	}

	UINT32 SendCount = 0;

	foreach (i in Teams)
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(*i);
		if(NULL == pTeam)
		{
			LogWarn("team %d not found!", *i);
			continue;
		}
		if(!m_handler.CanSeeCond(m_pRole, pTeam))
		{
			continue;
		}
		cb(pTeam, arg);

		if((++SendCount) >= 100)
		{
			break;
		}
	}
}

void TeamRequest::GetExpenditionTeam(const std::vector<UINT32> &Category, TeamListCB cb, void *arg)
{
	FilterTeam(TeamMgr::Instance()->GetTeams(Category), cb, arg);
}

void TeamRequest::GetExpenditionTeam(UINT32 expID, TeamListCB cb, void *arg)
{
	FilterTeam(TeamMgr::Instance()->GetTeams(expID), cb, arg);
}

int TeamRequest::StartBattleBeginVote(const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes &roRes)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_TEAM_ONLY_LEADER_CAN_STARTBATTLE;
	}

	if (!TeamInState(pTeam, KKSG::TEAM_WAITING) && !TeamInState(pTeam, KKSG::TEAM_MATCH))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	int ret = m_handler.StartBattleCond(m_pRole, pTeam, roArg, roRes);
	TJUDGERET(ret)

	//确定成功了才可以stopmatch 但是不能放在函数最后 因为成功会变成TEAM_VOTE状态
	if(KKSG::KMT_EXP == pTeam->GetMatchType())
	{
		HMatchHandler handler;
		handler.Stop(pTeam, KKSG::KMT_EXP);
	}

	TeamInviteMgr::Instance()->DelInvByTeam(pTeam->GetID());

	//就一个人 不需要投票
	if(pTeam->GetMemberCountWithoutRobot() <= 1)
	{
		pTeam->StartBattle();
	}
	else
	{
		pTeam->ResetMemberState(true);
		pTeam->SetBeginVote();
		pTeam->SetMemberVoteState(m_pRole, KKSG::EXPTEAM_READY);	
	}

	return 0;

}

int TeamRequest::StartBattleVote(UINT64 roleID, bool accept)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!TeamInState(pTeam, KKSG::TEAM_VOTE))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	if (accept == false)
	{
		pTeam->SetMemberVoteState(m_pRole, KKSG::EXPTEAM_DISAGREE);
	}
	else
	{
		pTeam->SetMemberVoteState(m_pRole, KKSG::EXPTEAM_READY);
	}

	return KKSG::ERR_SUCCESS;
}

int  TeamRequest::ChangeExpIDPre(int expID)
{
	if(TeamTransHandler::NeedInTs(expID))
	{
		return KKSG::ERR_TEAM_CHANGE_EXP_TO_CROSS;
	}
	return KKSG::ERR_SUCCESS;
}

int TeamRequest::ChangeExpID(int expID, KKSG::TeamOPRes& roRes, const KKSG::TeamHelper& nothelpers)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_FAILED;
	}

	if (expID == (int)pTeam->GetExpeditionID())
	{
		return KKSG::ERR_SUCCESS;
	}

	ExpeditionTable::RowData* pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expID);
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	if (!TeamInState(pTeam, KKSG::TEAM_WAITING) && !TeamInState(pTeam, KKSG::TEAM_MATCH))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	int ret = m_handler.ChangeExpCond(m_pRole, pTeam, pConf, roRes);
	TJUDGERET(ret)

	m_handler.OnChangeExp(pTeam, pConf);

	pTeam->SetUseTicket(0, 0);
	pTeam->RemoveAllRobotAndNotify();

	for(int i = 0; i < nothelpers.roles_size(); ++i)
	{
		pTeam->ResetHelperState(nothelpers.roles(i));
	}

	pTeam->SetConf(pConf);
	if(pConf->GuildLevel > 0)
	{
		pTeam->SetGuildID(CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID()));
	}
	pTeam->NotifyAllMemberStateChanged();

	pTeam->OnChangeExpId();

	TeamMgr::Instance()->SynAllData2GS(pTeam);
	TeamMgr::Instance()->SynGetExtraData(pTeam);

	return KKSG::ERR_SUCCESS;
}

int	TeamRequest::InvitePre(UINT64 roleID, const std::string& account)
{
	//本服不支持邀请平台好友
	if(0 == roleID && !account.empty())
	{
		return ERR_TEAM_CANNOT_FORNOW;
	}
	CRole* invtoRole = CRoleManager::Instance()->GetByRoleID(roleID);

	if(NULL == invtoRole)
	{
		return KKSG::ERR_TEAM_INV_LOGOUT;
	}

	if(invtoRole->IsInFamily())
	{
		return KKSG::ERR_TEAM_INV_IN_FAMILY;
	}

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::InviteAft(UINT64 roleID)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if(m_pRole->GetID() == roleID)
	{
		return KKSG::ERR_FAILED;
	}

	if(TeamInviteMgr::Instance()->IsInInviting(m_pRole->GetID(), roleID))
	{
		return 0;
	}


	if (!TeamInState(pTeam, KKSG::TEAM_WAITING) && !TeamInState(pTeam, KKSG::TEAM_MATCH))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	CRole *pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if (NULL == pRole)
	{
		return KKSG::ERR_TEAM_INV_LOGOUT;
	}

	if(pRole->IsInBattle())
	{
		return KKSG::ERR_TEAM_INVITE_ROLE_IS_IN_BATTLE;
	}

	if(pRole->GetTeamID())
	{
		if(pRole->GetTeamID() == pTeam->GetID())
		{
			return KKSG::ERR_TEAM_ALREADY_INTEAM;
		}
		else if(pRole->GetTeamID() != pTeam->GetID())
		{
			return KKSG::ERR_TEAM_ALREADY_INOTHERTEAM;
		}
	}

	int ret = m_handler.InviteCond(pTeam, m_pRole, pRole);
	TJUDGERET(ret)

	KKSG::TeamInvite data;
	m_pRole->FillTeamInviteData(data);
	pTeam->SaveBrief(data.mutable_teambrief(), false);
	data.set_invtime(UINT32(time(NULL)));
	data.set_invtoroleid(roleID);
	TeamInviteMgr::Instance()->MakeInvite(data);

	return KKSG::ERR_SUCCESS;
}

//单人匹配
int TeamRequest::StartMatch(int expid, bool downMatch/* = false*/)
{
	if(m_pRole->HasTeam())
	{
		return KKSG::ERR_FAILED;
	}
	ExpeditionTable::RowData *pRow = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);
	if(NULL == pRow)
	{
		return KKSG::ERR_FAILED;
	}
	if(pRow->MatchType != KKSG::KMT_NONE)
	{
		return KKSG::ERR_TEAM_CANNOT_FORNOW;
	}

	int ret = m_handler.CreateCond(m_pRole, pRow);
	TJUDGERET(ret)	

	HMatchHandler matchHandler;
	matchHandler.Data().expid = expid;
	ret = matchHandler.Start(m_pRole, KKSG::KMT_EXP);
	if(0 == ret && downMatch && ExpeditionConfigMgr::Instance()->CanDownMatch(pRow))
	{
		TeamMatchMgr::Instance()->GetFastMatchHandler()->SetRoleDownMatch(m_pRole->GetID());
	}

	return ret;
}

//单人取消匹配
int TeamRequest::StopMatch()
{
	HMatchHandler handler;
	handler.Stop(m_pRole, KKSG::KMT_EXP);

	return KKSG::ERR_SUCCESS;
}

bool TeamRequest::IsLeader(Team* pTeam)
{
	if(NULL == pTeam)
	{
		return false;
	}
	return pTeam->GetLeader() == m_pRole->GetID();
}

bool TeamRequest::TeamInState(Team* pTeam, int state)
{
	if(NULL == pTeam)
	{
		return false;
	}
	return (int)pTeam->GetTeamState() == state;
}

//bool TeamRequest::HasTeam()
//{
//	if (0 == m_pRole->GetTeamID())
//	{
//		return false;
//	}
//
//	m_pTeam = TeamMgr::Instance()->FindTeam(m_pRole->GetTeamID());
//	if (m_pTeam == NULL)
//	{
//		return false;
//	}
//
//	return true;
//}

Team* TeamRequest::GetTeamInMs()
{
	if(NULL == m_pRole)
	{
		return NULL;
	}
	return TeamMgr::Instance()->FindTeam(m_pRole->GetTeamID());
}

//int TeamRequest::CheckGuildCondition(CRole* pRole,ExpeditionTable::RowData *pRow, UINT64& gid)
//{
//	if(NULL == pRole || NULL == pRow)
//	{
//		return KKSG::ERR_FAILED;
//	}
//
//	if(GUILD_TEAM_SCENE == pRow->Type || pRow->Type == GUILDCAMP_TEAM_SCENE || pRow->Type == RESWAR_SCENE)
//	{
//		gid = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
//		if(0 == gid)
//		{
//			return KKSG::ERR_GUILD_NOT_IN_GUILD;
//		}
//		Guild* pGuild = CGuildMgr::Instance()->GetGuild(gid);
//		if (!pGuild)
//		{
//			return KKSG::ERR_GUILD_NOT_IN_GUILD;
//		}
//		if (pRow->Type == GUILDCAMP_TEAM_SCENE && pGuild->Get<CGuildCamp>()->GetCurGuildCampID() != pRow->DNExpeditionID)
//		{
//			return KKSG::ERR_FAILED;
//		}
//	}
//
//	return KKSG::ERR_SUCCESS;
//}

//int TeamRequest::SwitchHelper(bool helper)
//{
//	if (!HasTeam())
//	{
//		return KKSG::ERR_TEAM_NOT_EXIST;
//	}
//
//	if(IsLeader())
//	{
//		return KKSG::ERR_FAILED;
//	}
//
//	if(!TeamInState(KKSG::TEAM_WAITING) && !TeamInState(KKSG::TEAM_MATCH))
//	{
//		return m_pTeam->GetTeamStateErrorCode();
//	}
//
//	CTeamMember* pMember = m_pTeam->FindMember(m_pRole->GetID());
//	if(NULL == pMember)
//	{
//		return KKSG::ERR_FAILED;
//	}
//
//	pMember->SetIsHelper(helper);
//
//	m_pTeam->UseTicketNtf(m_pRole->GetID());//有可能改变 强行通知
//
//	m_pTeam->NotifyMemberStateChanged(pMember, m_pRole);
//
//	TeamMgr::Instance()->SynAllData2GS(m_pTeam);
//
//	return KKSG::ERR_SUCCESS;
//}

int TeamRequest::SetTeamCostPre(UINT32 index, UINT32& diamondCostCount)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_FAILED;
	}
	
	if (!TeamInState(pTeam, KKSG::TEAM_WAITING) && !TeamInState(pTeam, KKSG::TEAM_MATCH))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	if(TeamCostInvalidID != index)
	{
		const Sequence<UINT32, 3>* ret = ExpeditionConfigMgr::Instance()->GetTeamCost(pTeam->GetConf()->DNExpeditionID, index);
		if (NULL == ret)
		{
			return KKSG::ERR_UNKNOWN;
		}

		if(1 == ret->seq[0])//钻石
		{
			diamondCostCount = ret->seq[2];	
            auto tmp = DiamondCostCond();
            TJUDGERET(tmp);
		}
	}

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode TeamRequest::DiamondCostCond()
{
    if(m_pRole->GetCostDiamond() < GetGlobalConfig().TeamDiamondCostLimit)//消耗不够
    {
        return KKSG::ERR_TEAM_DIAMOND_COST_LIMIT;
    }
    return KKSG::ERR_SUCCESS;
}

int TeamRequest::SetTeamCost(UINT32 index)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_FAILED;
	}

	pTeam->SetCostIndex(index);
	return KKSG::ERR_SUCCESS;
}

int TeamRequest::SetTeamPPTLimit(UINT32 ppt)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_FAILED;
	}

	pTeam->SetPPTLimit(ppt);
	return KKSG::ERR_SUCCESS;

}

int TeamRequest::GetWeekNestCurExp()
{
	return NestWeekMgr::Instance()->GetCurExp();
}

int TeamRequest::TransLeader(UINT64 roleID)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_TEAM_ONLY_LEADER_CAN_DO;
	}

	CTeamMember* pMem = pTeam->FindMember(roleID);
	if(pMem && pMem->IsRobot())
	{
		pTeam->RemoveRobotAndNotify(roleID);
		return KKSG::ERR_SUCCESS;
	}

	pMem = pTeam->FindMember(roleID);
	if(NULL == pMem)
	{
		return KKSG::ERR_FAILED;
	}

	pTeam->OnChangeLeader(roleID);

	PtcM2C_TeamChangeM2CNtf oPtcNtf;
	pTeam->SaveBrief(oPtcNtf.m_Data.mutable_teambrief());
	pMem->SaveToTeamMember(oPtcNtf.m_Data.add_chgstatemember());
	pMem = pTeam->FindMember(m_pRole->GetID());
	if(pMem)
	{
		pMem->SaveToTeamMember(oPtcNtf.m_Data.add_chgstatemember());
	}
	pTeam->BroadCast(oPtcNtf);


	TeamMgr::Instance()->SynAllData2GS(pTeam);

	return KKSG::ERR_SUCCESS;
}

int TeamRequest::ChangePassword(const std::string& password)
{
	auto pTeam = GetTeamInMs();
	if (NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	if (!IsLeader(pTeam))
	{
		return KKSG::ERR_TEAM_ONLY_LEADER_CAN_DO;
	}

	if(KKSG::KMT_EXP == pTeam->GetMatchType())
	{
		HMatchHandler handler;
		handler.Stop(pTeam, KKSG::KMT_EXP);
	}

	pTeam->SetPassword(password);

	pTeam->NotifyTeamStateChanged();

	TeamMgr::Instance()->SynAllData2GS(pTeam);

	return KKSG::ERR_SUCCESS;
}

UINT32 TeamRequest::GetExpOpenTime(int expid)
{
	return m_handler.GetExpOpenTime(expid);
}

void TeamRequest::TransSendToGs(UINT32 delayid, const KKSG::TeamOPArg& roArg)
{
	// gs handle 
	RpcM2G_TeamRequestM2G *rpc = RpcM2G_TeamRequestM2G::CreateRpc();

	rpc->m_oArg.set_roleid(m_pRole->GetID());
	rpc->m_oArg.set_delayid(delayid);
	rpc->m_oArg.mutable_c2marg()->CopyFrom(roArg);

	// fill ms team info for gs 
	Team* pTeam = GetTeamInMs();
	if(NULL == pTeam)
	{
		if (0 != roArg.teamid())
		{
			pTeam = TeamMgr::Instance()->FindTeam(roArg.teamid());
		}
	}
	if(pTeam)
	{
		pTeam->SaveSynData(*rpc->m_oArg.mutable_dataall());
	}

	if (m_pRole)
	{
		rpc->m_oArg.set_league_teamid(LeagueTeamMgr::Instance()->GetLeagueTeamIdByRoleId(m_pRole->GetID()));
	}

	m_pRole->SendMsgToGS(*rpc);
}

int TeamRequest::BattleContinue(const KKSG::TeamHelper& helpers)
{
	auto pTeam = GetTeamInMs();
	if(NULL == pTeam)
	{
		return KKSG::ERR_FAILED;
	}

	if(0 == helpers.roles_size())
	{
		return KKSG::ERR_SUCCESS;
	}

	for(int i = 0; i < helpers.roles_size(); ++i)
	{
		UINT64 roleID = helpers.roles(i);
		if(pTeam->GetLeader() == roleID)
		{
			continue;
		}
		auto pMem = pTeam->FindMember(roleID);
		if(NULL == pMem)
		{
			continue;
		}
		if(pMem->IsHelper())
		{
			continue;
		}
		pMem->SetIsHelper(true);
	}

	pTeam->NotifyAllMemberStateChanged();

	return KKSG::ERR_SUCCESS;
}

//int TeamRequest::SwitchUseTicket(int useTicket)
//{
//	if(!HasTeam())
//	{
//		return KKSG::ERR_FAILED;
//	}
//
//	if(!TeamInState(KKSG::TEAM_WAITING) && !TeamInState(KKSG::TEAM_MATCH))
//	{
//		return m_pTeam->GetTeamStateErrorCode();
//	}
//
//	auto pMember = m_pTeam->FindMember(m_pRole->GetID());
//	if(NULL == pMember)
//	{
//		return KKSG::ERR_FAILED;
//	}
//
//	m_pTeam->SetUseTicket(useTicket, m_pRole->GetID());
//
//	m_pTeam->NotifyMemberStateChanged(pMember, m_pRole);
//
//	TeamMgr::Instance()->SynAllData2GS(m_pTeam);
//
//	return KKSG::ERR_SUCCESS;
//}

int TeamRequest::SwitchMemType(KKSG::TeamMemberType type)
{
	auto pTeam = GetTeamInMs();
	if(NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if(KKSG::TMT_HELPER == type && IsLeader(pTeam))//队长不能切助战
	{
		return KKSG::ERR_FAILED;
	}

	if(!TeamInState(pTeam, KKSG::TEAM_WAITING) && !TeamInState(pTeam, KKSG::TEAM_MATCH))
	{
		return pTeam->GetTeamStateErrorCode();
	}

	CTeamMember* pMember = pTeam->FindMember(m_pRole->GetID());
	if(NULL == pMember)
	{
		return KKSG::ERR_FAILED;
	}

	pMember->SetMemType(type);

	pTeam->NotifyMemberStateChanged(pMember, m_pRole);

	TeamMgr::Instance()->SynAllData2GS(pTeam);

	return KKSG::ERR_SUCCESS;
}

//只是条件判断
int TeamRequest::TeamMatch(const KKSG::KMatchCommonArg& roArg)
{
	auto pTeam = GetTeamInMs();
	if(NULL == pTeam)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if(pTeam->GetLeader() != m_pRole->GetID())
	{
		return KKSG::ERR_TEAM_ONLY_LEADER_CAN_DO;
	}

	if(pTeam->GetConf() && (pTeam->GetConf()->MatchType) != roArg.type())
	{
		return KKSG::ERR_FAILED;
	}

	return KKSG::ERR_SUCCESS;
}
