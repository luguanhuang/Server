#include "pch.h"
#include "teamhandler.h"
#include "role/role.h"
#include "util/XCommon.h"
#include "table/expeditionconfigmgr.h"
#include "util/gametime.h"
#include "define/teamdef.h"
#include "nestweek/nestweekmgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guild/guildcamp.h"
#include "team.h"
#include "reswar/reswarmgr.h"
#include "herobattle/herobattlemgr.h"
#include "leagueteam/leagueteammgr.h"
#include "skycity/skycitymgr.h"
#include "table/MultActivityMgr.h"
#include "teammember.h"
#include "partner/partnermgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "other/matchhandler.h"
#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"
#include "custombattle/custombattlerole.h"
#include "custombattle/custombattlerolemgr.h"
#include "marriage/marriage.h"
#include "marriage/marriagemgr.h"

std::unordered_map<int, THBase*> TeamHandler::m_subHandler;

THGoddess THGoddess::GlobalTHGoddess;
THEndlessAbyss THEndlessAbyss::GlobalTHEndlessAbyss;
THWeekNest THWeekNest::GlobalHTWeekNest;
THGuildCamp THGuildCamp::GlobalHTGuildCamp;
THResWar THResWar::GlobalHTResWar;
THLeague THLeague::GlobalHTLeague;
THSkyCraft THSkyCraft::GlobalSkyCraft;
THSkyCity THSkyCity::GlobalTHSkyCity;
THPartner THPartner::GlobalTHPartner;
THCustomPkTwo THCustomPkTwo::GlobalTHCustomPkTwo;
THMarriageCouple THMarriageCouple::GlobalTHMarriageCouple;
THMarriage THMarriage::GlobalTHMarriage;

THBase* TeamHandler::GetSubHandler(int type)
{
	auto it = m_subHandler.find(type);
	if (it != m_subHandler.end())
	{
		return it->second;
	}
	return NULL;
}

int TeamHandler::CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	if(NULL == pRole || NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	if(pRole->IsInBattle())
	{
		return KKSG::ERR_TEAM_NOT_IN_HALL;
	}

	int ret = CheckExpCond(pConf->DNExpeditionID);
	TJUDGERET(ret)

	ret = CheckGuildCond(pRole, pConf);
	TJUDGERET(ret)

	auto pSub = GetSubHandler(pConf->Type);
	if(pSub)
	{
		ret = pSub->CreateCond(pRole, pConf);
		TJUDGERET(ret)
	}

	return KKSG::ERR_SUCCESS;
}

int TeamHandler::CheckExpCond(int expid)
{
	if(GameTime::GetTime() < GetExpOpenTime(expid))
	{
		return KKSG::ERR_TEAM_SERVER_OPEN_TIME;
	}
	return KKSG::ERR_SUCCESS;
}

UINT32 TeamHandler::GetExpOpenTime(int expid)
{
	return ExpeditionConfigMgr::Instance()->GetExpOpenTime(expid, MSConfig::Instance()->IsBackFlowServer(), MSConfig::Instance()->GetBackFlowLevel(), MSConfig::Instance()->GetGameServerOpenTimeStamp());
}

int TeamHandler::CheckGuildCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	if(0 == pConf->GuildLevel)//对公会没有要求
	{
		return KKSG::ERR_SUCCESS;
	}
	auto pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if(NULL == pGuild)
	{
		return KKSG::ERR_GUILD_NOT_IN_GUILD;
	}
	if(pGuild->GetLevel() < UINT32(pConf->GuildLevel))
	{
		return KKSG::ERR_GUILD_LEVEL_REQ;
	}
	return KKSG::ERR_SUCCESS;
}
	
void TeamHandler::CheckStopMatch(Team* pTeam)
{
	HMatchHandler handler;
	handler.CheckTeamStop(pTeam);
}

int TeamHandler::JoinCond(CRole* pRole, Team* pTeam)
{
	if(NULL == pRole || NULL == pTeam)
	{
		return KKSG::ERR_FAILED;
	}

	if(ROLE_STATE_IN_GAME != pRole->GetState() || pRole->IsInBattle())
	{
		return KKSG::ERR_TEAM_NOJOININBATTLE;
	}

	if (KKSG::TEAM_WAITING != pTeam->GetTeamState() && KKSG::TEAM_MATCH != pTeam->GetTeamState())
	{
		return pTeam->GetTeamStateErrorCode();
	}

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		int ret = pSub->JoinCond(pRole, pTeam);
		TJUDGERET(ret)
	}

	return KKSG::ERR_SUCCESS;
}

int TeamHandler::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	if(NULL == pRole || NULL == pTeam)
	{
		return KKSG::ERR_FAILED;
	}

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		int ret = pSub->StartBattleCond(pRole, pTeam, roArg, roRes);
		TJUDGERET(ret)
	}
	return KKSG::ERR_SUCCESS;
}

void TeamHandler::OnJoinTeam(CRole* pRole, Team* pTeam)
{
	if(NULL == pRole || NULL == pTeam)
	{
		return;
	}

	CheckStopMatch(pTeam);

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		pSub->OnJoinTeam(pRole, pTeam);
	}
}

void TeamHandler::OnLeaveTeam(CRole* pRole, Team* pTeam)
{
	if(NULL == pRole || NULL == pTeam)
	{
		return;
	}

	CheckStopMatch(pTeam);

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		pSub->OnLeaveTeam(pRole, pTeam);
	}
}

void TeamHandler::OnChangeExp(Team* pTeam, ExpeditionTable::RowData* pToConf)
{
	if(NULL == pTeam || NULL == pToConf)
	{
		return;
	}

	CheckStopMatch(pTeam);

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		pSub->OnChangeExpFrom(pTeam, pToConf);
	}

}

bool TeamHandler::CanSeeCond(CRole* pRole, Team* pTeam)
{
	if(NULL == pRole || NULL == pTeam)
	{
		return false;
	}
	if(KKSG::TEAM_WAITING != pTeam->GetTeamState() && KKSG::TEAM_MATCH != pTeam->GetTeamState())//队伍状态
	{
		return false;
	}
	if(0 != pTeam->GetGuildID() && pTeam->GetGuildID() != CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID()))//公会
	{
		return false;
	}

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		if(!pSub->CanSeeCond(pRole, pTeam))
		{
			return false;
		}
	}

	return true;

}

int TeamHandler::ChangeExpCond(CRole* pRole, Team* pTeam, ExpeditionTable::RowData* pConf, KKSG::TeamOPRes& roRes)
{
	if(NULL == pRole || NULL == pTeam || NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}

	int ret = CreateCond(pRole, pConf);
	TJUDGERET(ret)

	auto pSub = GetSubHandler(pConf->Type);
	if(pSub)
	{
		ret = pSub->ChangeExpCond(pRole, pTeam, roRes);
		TJUDGERET(ret)
	}

	return KKSG::ERR_SUCCESS;
}

int TeamHandler::InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole)
{
	if(NULL == pTeam || NULL == pFromRole || NULL == pToRole)
	{
		return KKSG::ERR_FAILED;
	}

	auto pSub = GetSubHandler(pTeam->GetConf()->Type);
	if(pSub)
	{
		int ret = pSub->InviteCond(pTeam, pFromRole, pToRole);
		TJUDGERET(ret)
	}

	return KKSG::ERR_SUCCESS;
}

THBase::THBase(int type)
{
	TeamHandler::m_subHandler[type] = this;
}

THBase::~THBase()
{

}

int THBase::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	auto pConf = pTeam->GetConf();
	if(NULL == pConf)
	{
		return KKSG::ERR_FAILED;
	}
	//默认PVP玩法不能开始战斗
	if(KKSG::KMT_NONE != pConf->MatchType)
	{
		SSWarn << " pvp try start battle role = " << END;
		return ERR_FAILED;
	}
	return KKSG::ERR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int THWeekNest::CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	if(NestWeekMgr::Instance()->GetCurExp() != pConf->DNExpeditionID)
	{
		return KKSG::ERR_TEAM_WEEK_NEST_EXP;
	}
	return KKSG::ERR_SUCCESS;
}

int THGuildCamp::CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	auto pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if(NULL == pGuild)
	{
		return KKSG::ERR_GUILD_NOT_IN_GUILD;
	}
	if(pGuild->Get<CGuildCamp>()->GetCurGuildCampID() != pConf->DNExpeditionID)
	{
		return KKSG::ERR_FAILED;
	}
	return KKSG::ERR_SUCCESS;
}

int THResWar::JoinCond(CRole* pRole, Team* pTeam)
{
	bool bState=ResWarMgr::Instance()->CheckFightState(pTeam,pRole->GetID());
	if (!bState)
	{
		return KKSG::ERR_RESWAR_TEAMFIGHTING;
	}
	Guild *pSelfGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	Guild *pLeaderGuild = CGuildMgr::Instance()->GetGuild(pTeam->GetGuildID());
	if (pSelfGuild!=pLeaderGuild)
	{
		return KKSG::ERR_RESWAR_DIFF_GUILD;
	}
	return KKSG::ERR_SUCCESS;
}

void THResWar::OnJoinTeam(CRole* pRole, Team* pTeam)
{
	ResWarMgr::Instance()->JoinTeam(pTeam,pRole->GetID());
}

void THResWar::OnLeaveTeam(CRole* pRole, Team* pTeam)
{
	ResWarMgr::Instance()->LeaveTeam(pTeam,pRole->GetID());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int THLeague::JoinCond(CRole* pRole, Team* pTeam)
{
	UINT64 leaderId = pTeam->GetLeader();
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamByRoleId(leaderId);
	// 队长有联赛战队, 组队是为了战斗，非战队成员不能进入
	if (pLTeam)
	{
		if (!pLTeam->HasMember(pRole->GetID()))
		{
			return KKSG::ERR_LEAGUE_NOT_IN_TEAM;
		}
	}
	// 队长没有战队，组队是为了建立长期关系，有战队的人也应该进不去
	else
	{
		if (LeagueTeamMgr::Instance()->HasLeagueTeam(pRole->GetID()))
		{
			return KKSG::ERR_LEAGUE_ALREADY_HAS_TEAM;
		}
	}

	return KKSG::ERR_SUCCESS;
}

int THLeague::ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes)
{
	std::vector<UINT64> roles;
	pTeam->GetRoleIDs(roles);
	for(size_t i = 0; i < roles.size(); ++i)
	{
		UINT64 roleID = roles[i];
		if(pRole->GetID() == roleID)//队长
		{
			continue;
		}
		CRole* other = CRoleManager::Instance()->GetByRoleID(roleID);
		if(NULL == other)
		{
			roRes.set_problem_roleid(roleID);
			return KKSG::ERR_TEAM_NOT_IN_HALL;
		}
		int ret = JoinCond(other, pTeam);
		if(KKSG::ERR_SUCCESS != ret)
		{
			roRes.set_problem_roleid(roleID);
			return ret;
		}
	}
	return KKSG::ERR_SUCCESS;
}

int THLeague::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	UINT64 errorId = 0;
	int ret = LeagueTeamMgr::Instance()->CheckStartTeamVote(pTeam, roArg, errorId);
	if (ret != 0)
	{
		roRes.set_problem_roleid(errorId);
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

bool THLeague::CanSeeCond(CRole* pRole, Team* pTeam)
{
	if(KKSG::ERR_SUCCESS != JoinCond(pRole, pTeam))
	{
		return false;
	}
	return true;
}

int THLeague::InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole)
{
	int ret = JoinCond(pToRole, pTeam);
	if(KKSG::ERR_LEAGUE_NOT_IN_TEAM == ret)
	{
		return KKSG::ERR_LEAGUE_INV_NOT_IN_TEAM;
	}
	if(KKSG::ERR_LEAGUE_ALREADY_HAS_TEAM == ret)
	{
		return KKSG::ERR_LEAGUE_INV_ALREADY_HAS_TEAM;
	}
	
	return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int THSkyCraft::JoinCond(CRole* pRole, Team* pTeam)
{
	UINT64 leaderId = pTeam->GetLeader();
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamByRoleId(leaderId);
	// 队长有战队, 组队是为了战斗，非战队成员不能进入
	if (pSTeam)
	{
		if (!pSTeam->HasMember(pRole->GetID()))
		{
			return KKSG::ERR_SKY_NOT_TEAM_MEMBER;
		}
	}
	// 队长没有战队，组队是为了建立长期关系，有战队的人也应该进不去
	else
	{
		if (SkyTeamMgr::Instance()->HasSkyTeam(pRole->GetID()))
		{
			return KKSG::ERR_SKY_ALREADY_HAS_TEAM;
		}
	}

	return KKSG::ERR_SUCCESS;
}

int THSkyCraft::InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole)
{
	int ret = JoinCond(pToRole, pTeam);
	if(KKSG::ERR_SKY_NOT_TEAM_MEMBER == ret)
	{
		return KKSG::ERR_SKY_INV_NOT_TEAM_MEMBER;
	}
	if(KKSG::ERR_SKY_ALREADY_HAS_TEAM == ret)
	{
		return KKSG::ERR_SKY_INV_ALREADY_HAS_TEAM;
	}
	
	return ret;
}

int THSkyCraft::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	auto pConf = pTeam->GetConf();
	if (pConf == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	if (pConf->Type != SKYCRAFT_SCENE)
	{
		return KKSG::ERR_FAILED;
	}

	UINT64 leaderId = pTeam->GetLeader();
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamByRoleId(leaderId);
	// 队长有战队, 投票是为了战斗, 直接发送匹配协议，而不是开始战斗协议
	if (pSTeam)
	{
		return KKSG::ERR_FAILED;
	}

	// 队长没有战队, 投票是为了建立战队
	UINT64 errorId = 0;
	const std::string& teamname = roArg.extrainfo().league_teamname();
	int ret = SkyTeamMgr::Instance()->CheckCreateTeam(pTeam, teamname, errorId);
	if (ret != KKSG::ERR_SUCCESS)
	{
		roRes.set_problem_roleid(errorId);
	}
	pTeam->SetLeagueTeamName(teamname);

	return ret;
}

int THSkyCraft::ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes)
{
	std::vector<UINT64> roles;
	pTeam->GetRoleIDs(roles);
	for(size_t i = 0; i < roles.size(); ++i)
	{
		UINT64 roleID = roles[i];
		if(pRole->GetID() == roleID)//队长
		{
			continue;
		}
		CRole* other = CRoleManager::Instance()->GetByRoleID(roleID);
		if(NULL == other)
		{
			roRes.set_problem_roleid(roleID);
			return KKSG::ERR_ROLE_LOGOUT;
		}
		int ret = JoinCond(other, pTeam);
		if(KKSG::ERR_SUCCESS != ret)
		{
			roRes.set_problem_roleid(roleID);
			return ret;
		}
	}
	return KKSG::ERR_SUCCESS;
}

bool THSkyCraft::CanSeeCond(CRole* pRole, Team* pTeam)
{
	if(KKSG::ERR_SUCCESS != JoinCond(pRole, pTeam))
	{
		return false;
	}
	return true;
}

int THSkyCity::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	UINT32 nExpID = pTeam->GetExpeditionID();
	if (nExpID == SKYCITY_DNE_ID)
	{
		if (MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_SKYCITY)==false)
		{
			return KKSG::ERR_SKYCITY_TEAM_OUTTIME;
		}
		if (pRole)
		{
			if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
			{
				return KKSG::ERR_INVALID_REQUEST;
			}
		}
	}

	return KKSG::ERR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int THPartner::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	const std::list<CTeamMember*>& members = pTeam->GetMember();
	std::vector<UINT64> roleIds;
	for (auto it = members.begin(); it != members.end(); ++it)
	{
		CTeamMember* m = *it;
		if (m) roleIds.push_back(m->m_qwRoleID);
	}
	UINT64 errorId = 0;
	UINT32 ret = PartnerMgr::Instance()->CheckMakePartner(roleIds, errorId);
	if (ret != KKSG::ERR_SUCCESS)
	{
		roRes.set_problem_roleid(errorId);
		return ret;
	}

	return KKSG::ERR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool THGoddess::CanSeeCond(CRole* pRole, Team* pTeam)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if(0 == pTeam->GetLeaderLeftCount())
	{
		return false;
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool THEndlessAbyss::CanSeeCond(CRole* pRole, Team* pTeam)
{
	if(0 == pTeam->GetLeaderLeftCount())
	{
		return false;
	}
	return true;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int THCustomPkTwo::CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	///> 是否参加全民pk2v2
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(pRole->GetID());
	if (NULL == roleinfo)
	{
		return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
	if (roleinfo->IsJoinPvpTwo())
	{
		return KKSG::ERR_SUCCESS;
	}
	else
	{
		return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
}

int THCustomPkTwo::JoinCond(CRole* pRole, Team* pTeam)
{

	///> 是否参加全民pk2v2
	UINT64 leader = pTeam->GetLeader();
	SCustomBattleRole* leaderinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(leader);
	if (NULL == leaderinfo || 0 == leaderinfo->GetJoinId())
	{
		return KKSG::ERR_CUSTOM_SAMEMATCH;
	}
	SCustomBattleRole* roleinfo = CustomBattleRoleMgr::Instance()->TryGetInfo(pRole->GetID());
	if (NULL == roleinfo || 0 == roleinfo->GetJoinId())
	{
		return KKSG::ERR_CUSTOMBATTLE_NOTJOIN;
	}
	if (roleinfo->GetJoinId() != leaderinfo->GetJoinId())
	{
		return KKSG::ERR_CUSTOM_SAMEMATCH;
	}
	return KKSG::ERR_SUCCESS;
}

int THMarriageCouple::CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	Marriage* pMarriage = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (!pMarriage)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	if (!pMarriage->IsOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	if (!pMarriage->IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	return KKSG::ERR_SUCCESS;
}

int THMarriageCouple::JoinCond(CRole* pRole, Team* pTeam)
{
	Marriage* pMarriage = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (!pMarriage)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	if (!pMarriage->IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	UINT64 coupleID = pTeam->GetLeader();
	if (coupleID != pMarriage->GetCoupleID())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	return KKSG::ERR_SUCCESS;
}

int THMarriageCouple::ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes)
{
	return KKSG::ERR_FAILED;
}

int THMarriageCouple::InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole)
{
	int ret = JoinCond(pToRole, pTeam);

	return ret;
}

bool THMarriageCouple::CanSeeCond(CRole* pRole, Team* pTeam)
{
	int ret = JoinCond(pRole, pTeam);

	return ret == KKSG::ERR_SUCCESS;
}

int THMarriageCouple::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	Marriage* pMarriage = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (!pMarriage)
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	if (!pMarriage->IsOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	if (!pMarriage->IsInMarriagePos())
	{
		return KKSG::ERR_MARRIAGE_NOTINMARRIAGE;
	}

	return KKSG::ERR_SUCCESS;
}

int THMarriage::CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf)
{
	Marriage* pMarriage = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (!pMarriage)
	{
		return KKSG::ERR_ROLE_NOT_ONLINE;
	}

	if (!pMarriage->IsOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;

}

int THMarriage::JoinCond(CRole* pRole, Team* pTeam)
{
	Marriage* pMarriage = MarriageMgr::Instance()->GetMarriage(pRole->GetID());
	if (!pMarriage)
	{
		return KKSG::ERR_ROLE_NOT_ONLINE;
	}

	if (!pMarriage->IsOpen())
	{
		return KKSG::ERR_SYS_NOTOPEN;
	}

	return KKSG::ERR_SUCCESS;
}

int THMarriage::ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes)
{
	return KKSG::ERR_SUCCESS;
}

int THMarriage::InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole)
{
	return JoinCond(pToRole, pTeam);
}

bool THMarriage::CanSeeCond(CRole* pRole, Team* pTeam)
{
	return (ErrorCode)JoinCond(pRole, pTeam) == KKSG::ERR_SUCCESS;
}

int THMarriage::StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes)
{
	return KKSG::ERR_FAILED;
}
