#include "pch.h"
#include "leagueteammgr.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "role/rolemanager.h"
#include "util/timespecificmgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "league/ptcm2c_updateleaguebattleseasoninfo.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "team/team.h"
#include "table/expeditionconfigmgr.h"
#include "tsssdk/MsTssHandler.h"
#include "forbid/forbidwordmgr.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"
#include "team/teammember.h"
#include "config.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "tablemgr/leaguematchtablemgr.h"
#include "define/itemdef.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "network/dblink.h"
#include "db/rpcm2d_saveleagueteamdata.h"
#include "loghelper/tlogr.h"

LeagueTeamMgr::LeagueTeamMgr()
:m_id(0)
,m_gmRoleNum(0)
{
}

LeagueTeamMgr::~LeagueTeamMgr()
{
}

bool LeagueTeamMgr::Init()
{
	if (!LoadFromDB())
	{
		LogError("load from db failed");
		return false;
	}
	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);	

	return true;
}

void LeagueTeamMgr::Uninit()
{
}

void LeagueTeamMgr::SaveAll2DB()
{
	while (!m_dirtyList.empty())
	{
		UINT64 id = m_dirtyList.pop_front();
		auto it = m_teams.find(id);
		if (it == m_teams.end())
		{
			continue;
		}
		it->second.UpdateToDB();
	}
}

void LeagueTeamMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 i = 0;
	while (!m_dirtyList.empty() && i < 20)
	{
		UINT64 id = m_dirtyList.pop_front();
		auto it = m_teams.find(id);
		if (it == m_teams.end())
		{
			continue;
		}
		it->second.UpdateToDB();
		i++;
	}
}

bool LeagueTeamMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::string sql("select _id, base, battlerecord from leagueteam");
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read leagueteam failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// leagueteamId
		UINT64 id;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			id = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}

		KKSG::LeagueTeamSaveData data;
		data.set_id(id);
		// base
		if(poRes->GetFieldValue(1, &pVal, dwLen))
		{
			data.mutable_base()->assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'base' column failed");
			return false;
		}

		// battlerecord
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			data.mutable_battlerecord()->assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'battlerecord' column failed");
			return false;
		}

		LeagueTeam& team = m_teams[id];
		team.LoadFromDB(data);
		team.OnLoaded();
	}

	poRes->Release();
	SSInfo << "Load leagueteam num: " << m_teams.size() << END;

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all leagueteam data from db use time: %llu ms", qwTimeUsed);
	return true;
}

bool LeagueTeamMgr::HasLeagueTeam(UINT64 roleId)
{
	auto it = m_roleId2leagueTeamId.find(roleId);
	if (it == m_roleId2leagueTeamId.end())
	{
		return false;
	}
	return true;
}

bool LeagueTeamMgr::IsInSameLeagueTeam(UINT64 roleId1, UINT64 roleId2)
{
	if (roleId1 == roleId2)
	{
		return false;
	}
	UINT64 leagueTeamId1 = GetLeagueTeamIdByRoleId(roleId1);
	if (leagueTeamId1 == 0)
	{
		return false;
	}
	UINT64 leagueTeamId2 = GetLeagueTeamIdByRoleId(roleId2);
	if (leagueTeamId2 == 0)
	{
		return false;
	}
	return leagueTeamId1 == leagueTeamId2;
}


LeagueTeam* LeagueTeamMgr::GetLeagueTeamByRoleId(UINT64 roleId)
{
	UINT64 teamId = GetLeagueTeamIdByRoleId(roleId);
	if (teamId == 0)
	{
		return NULL;
	}
	return GetLeagueTeamById(teamId);
}

LeagueTeam* LeagueTeamMgr::GetLeagueTeamById(UINT64 teamId)
{
	auto it = m_teams.find(teamId);
	if (it == m_teams.end())
	{
		return NULL;
	}
	return &it->second;
}

UINT64 LeagueTeamMgr::GetLeagueTeamIdByRoleId(UINT64 roleId)
{
	auto it = m_roleId2leagueTeamId.find(roleId);
	if (it == m_roleId2leagueTeamId.end())
	{
		return 0;
	}
	return it->second;
}

void LeagueTeamMgr::AddRoleLeagueTeam(UINT64 roleId, UINT64 teamId)
{
	m_roleId2leagueTeamId[roleId] = teamId;
}

void LeagueTeamMgr::RemoveRoleLeagueTeam(UINT64 roleId)
{
	m_roleId2leagueTeamId.erase(roleId);
}

void LeagueTeamMgr::AddToUpdateList(UINT64 leagueTeamId)
{
	m_dirtyList.push_back(leagueTeamId);
}

void LeagueTeamMgr::RemoveLeagueTeam(UINT64 leagueTeamId)
{
	m_teams.erase(leagueTeamId);
}

void LeagueTeamMgr::OnRoleLogin(CRole* pRole)
{
	UpdateSeasonInfo(pRole);
}

void LeagueTeamMgr::UpdateSeasonInfo(CRole* pRole)
{
	if (pRole == NULL)
	{
		return;
	}

	PtcM2C_UpdateLeagueBattleSeasonInfo ntf;	
	ntf.m_Data.set_is_open(LeagueSeasonMgr::Instance()->IsSeasonOpen());
	ntf.m_Data.set_is_cross(LeagueSeasonMgr::Instance()->IsInCrossState());
	ntf.m_Data.set_season_num(LeagueSeasonMgr::Instance()->GetSeasonNum());
	ntf.m_Data.set_matchlefttime(LeagueSeasonMgr::Instance()->GetMatchLeftTime());
	ntf.m_Data.set_state(LeagueSeasonMgr::Instance()->GetTodayState());
	LeagueTeam* team = GetLeagueTeamByRoleId(pRole->GetID());
	if (team == NULL)
	{
		ntf.m_Data.set_league_teamid(0);
	}
	else
	{
		ntf.m_Data.set_league_teamid(team->GetId());
		std::vector<LeagueTeamMember>& members = team->GetAllMember();
		for (size_t i = 0; i < members.size(); ++i)
		{
			ntf.m_Data.add_league_teammember(members[i].roleId);
		}
	}
	pRole->Send(ntf);
}

UINT64 LeagueTeamMgr::NewId()
{
	// 16位serverid 32位时间戳 16位自增id
	UINT64 temp = MSConfig::Instance()->GetServerID() & 0xFFFF;
	temp <<= 48;
	temp |= (TimeUtil::GetTime() & 0xFFFFFFFF) << 16;
	temp |= ((++m_id) & 0xFFFF);
	return temp;
}

KKSG::ErrorCode LeagueTeamMgr::GMLeaveLeagueTeam(UINT64 roleId)
{
	LeagueTeam* team = GetLeagueTeamByRoleId(roleId);
	if (team == NULL)
	{
		return KKSG::ERR_LEAGUE_HAS_NO_TEAM;
	}
	return team->LeaveMember(roleId, true);
}

LeagueTeam* LeagueTeamMgr::CreateLeagueTeam(const std::vector<UINT64>& roleIds, UINT64 creatorId, const std::string& name)
{
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)	
	{
		if (HasLeagueTeam(*it))
		{
			return NULL;
		}
	}
	UINT64 id = NewId();
	LeagueTeam& team = m_teams[id];
	team.SetId(id);
	team.SetName(name);
	team.SetCreatorId(creatorId);
	team.SetMemberRoleIds(roleIds);
	team.OnCreate();
	team.AddToDB();

	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)	
	{
		AddRoleLeagueTeam(*it, id);
	}
	SetNameUsed(name);
	
	std::ostringstream oss;
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)
	{
		oss << *it << ",";
	}
	LogInfo("roles [%s] create leagueteam [%llu] name[%s]", oss.str().c_str(), id, name.c_str());

	TLeagueTeamFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_TeamId = id;
	oLog.m_strName = name;
	oLog.m_roleIds = roleIds;
	oLog.m_Type = Tx_LeagueTeamType_Create;
	oLog.m_Num = roleIds.size();
	oLog.Do();
	return &team;
}

int LeagueTeamMgr::CheckJoinTeam(Team* pTeam, UINT64 roleId)
{
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	auto pConf = pTeam->GetConf();
	if (pConf == NULL)
	{
		return KKSG::ERR_SUCCESS;
	}
	if (pConf->Type != LEAGUETEAM_SCENE)
	{
		return KKSG::ERR_SUCCESS;
	}

	UINT64 leaderId = pTeam->GetLeader();
	LeagueTeam* pLTeam = GetLeagueTeamByRoleId(leaderId);
	// 队长有联赛战队, 组队是为了战斗，非战队成员不能进入
	if (pLTeam)
	{
		if (!pLTeam->HasMember(roleId))
		{
			return KKSG::ERR_LEAGUE_NOT_IN_TEAM;
		}
	}
	// 队长没有战队，组队是为了建立长期关系，有战队的人也应该进不去
	else
	{
		if (HasLeagueTeam(roleId))
		{
			return KKSG::ERR_LEAGUE_ALREADY_HAS_TEAM;
		}
	}

	return KKSG::ERR_SUCCESS;
}

int LeagueTeamMgr::CheckStartTeamVote(Team* pTeam, const KKSG::TeamOPArg& roArg, UINT64& errRoleId)
{
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	auto pConf = pTeam->GetConf();
	if (pConf == NULL)
	{
		return KKSG::ERR_SUCCESS;
	}
	if (pConf->Type != LEAGUETEAM_SCENE)
	{
		return KKSG::ERR_SUCCESS;
	}

	UINT64 leaderId = pTeam->GetLeader();
	LeagueTeam* pLTeam = GetLeagueTeamByRoleId(leaderId);
	// 队长有联赛战队, 投票是为了战斗, 检查是否匹配中
	if (pLTeam)
	{
		// TODO 匹配不用投票
	}
	// 队长没有战队, 投票是为了建立战队
	else
	{
		return CheckCreateTeam(pTeam, roArg.extrainfo().league_teamname(), errRoleId);
	}

	return KKSG::ERR_SUCCESS;
}

int LeagueTeamMgr::CheckCreateTeam(Team* pTeam, const std::string& name, UINT64& errRoleId)
{
	errRoleId = 0;
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	if (!LeagueSeasonMgr::Instance()->IsInApplyTime())
	{
		return KKSG::ERR_LEAGUE_NOT_IN_APPLY_TIME;
	}

	// 人数
	if (pTeam->GetMemberCount() != GetTeamRoleNum())
	{
		return KKSG::ERR_LEAGUE_TEAM_ROLE_NUM_INVALID;
	}
	// 是否未加入联赛战队
	const std::list<CTeamMember*>& members = pTeam->GetMember();
	for (auto it = members.begin(); it != members.end(); ++it)
	{
		CTeamMember* m = *it;
		if (HasLeagueTeam(m->m_qwRoleID))
		{
			errRoleId = m->m_qwRoleID;
			return KKSG::ERR_LEAGUE_ALREADY_HAS_TEAM;
		}
	}
	int ret = CheckNameValid(name);
	if (ret != 0)
	{
		return ret;
	}
	pTeam->SetLeagueTeamName(name);
	return KKSG::ERR_SUCCESS;
}

bool LeagueTeamMgr::GMCreateLeagueTeam(const std::vector<UINT64>& roleIds, const std::string& name, std::string& output)
{
	int ret = CheckNameValid(name);
	if (ret != 0)
	{
		output = "invalid name";
		return false;
	}
	LeagueTeam* team = CreateLeagueTeam(roleIds,roleIds[0], name);
	if (team)
	{
		output = "Create Success";
		return true;
	}

	output = "Create Failed";
	return false;
}


int LeagueTeamMgr::TeamStartBattle(Team* pTeam)
{
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	UINT64 leaderId = pTeam->GetLeader();
	LeagueTeam* pLTeam = GetLeagueTeamByRoleId(leaderId);
	// 队长有联赛战队, 投票是为了战斗, 检查是否匹配中
	if (pLTeam)
	{
		// TODO
	}
	// 队长没有战队, 投票是为了建立战队
	else
	{
		UINT64 errRoleId = 0;
		int ret = CheckCreateTeam(pTeam, pTeam->GetLeagueTeamName(), errRoleId);
		if (ret != 0)
		{
			return ret;
		}
		// 建立关系
		std::vector<UINT64> roleIds;
		const std::list<CTeamMember*>& members = pTeam->GetMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			roleIds.push_back((*it)->m_qwRoleID);
		}
		CreateLeagueTeam(roleIds, leaderId, pTeam->GetLeagueTeamName());
	}

	return KKSG::ERR_SUCCESS;
}

int LeagueTeamMgr::CheckNameValid(const std::string& name)
{
	//请求腾讯sdk验证名字是否合法;
	KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(name);
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		return nErrCode;
	}

	if(ForbidWordMgr::Instance()->HaveForbidWord(name))
	{
		return KKSG::ERR_INVALID_NAME;
	}
	
	UINT32 nameLen = (UINT32)XCommon::CountUTF8Char(name);
	if(nameLen < GetGlobalConfig().LeagueTeamNameLenMin)
	{
		return KKSG::ERR_NAME_TOO_SHORT;
	}
	if(nameLen > GetGlobalConfig().LeagueTeamNameLenMax)
	{
		return KKSG::ERR_NAME_TOO_LONG;
	}

	bool isAllNumber = true;
	for(UINT32 i = 0; i < name.length(); i++)
	{
		char c = name.at(i);
		if(c < '0' || c > '9')
		{
			isAllNumber = false;
			break;
		}
	}
	if(isAllNumber)
	{
		return KKSG::ERR_NAME_ALLNUM;
	}

	if (IsNameUsed(name))
	{
		return KKSG::ERR_NAME_EXIST;
	}

	return KKSG::ERR_SUCCESS;
}

bool LeagueTeamMgr::IsNameUsed(const std::string& name)
{
	return m_usedName.find(name) != m_usedName.end();
}

void LeagueTeamMgr::SetNameUsed(const std::string& name)
{
	m_usedName.insert(name);
}

void LeagueTeamMgr::RemoveNameUsed(const std::string& name)
{
	m_usedName.erase(name);
}

void LeagueTeamMgr::OnSeasonEnd()
{
	// 赛季结束, 所有队伍解散
	std::string title = "null";
	std::string content = "null";
	auto pMailConf = MailConfig::Instance()->GetMailConf(MailConf_LeagueSeasonEndDissolve);
	if(pMailConf)
	{
		title = pMailConf->m_title;
		content = pMailConf->m_content;
	}
	std::vector<ItemDesc> items;
	MailData mailData(title, content, items, MAIL_EXPIRE_TIME);

	for (auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		const auto& leagueMemebers = it->second.GetAllMember();
		for(size_t i = 0; i < leagueMemebers.size(); ++i)
		{
			UINT64 roleId = leagueMemebers[i].roleId;
			CMailMgr::Instance()->SendMail(roleId, mailData);
		}
	}

	m_teams.clear();
	m_roleId2leagueTeamId.clear();
	m_usedName.clear();
	m_dirtyList.clear();

	RpcM2D_SaveLeagueTeamData* rpc = RpcM2D_SaveLeagueTeamData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_DelAll);
	DBLink::Instance()->SendTo(*rpc);
}

UINT32 LeagueTeamMgr::GetTeamRoleNum()
{
	return m_gmRoleNum ? m_gmRoleNum : GetGlobalConfig().LeagueTeamRoleNum;
}

void LeagueTeamMgr::SendAllRankReward()
{
	LeagueRankList* list = CRankListMgr::Instance()->GetLeagueRankList();
	std::vector<LeagueRankInfo*> ranks;
	list->GetRankInfo(ranks);

	for (size_t i = 0; i < ranks.size(); ++i)
	{
		UINT64 ltid = ranks[i]->GetUniqueId();
		SendRankReward(ltid, i + 1, false);
	}
}

void LeagueTeamMgr::SendRankReward(UINT64 ltId, UINT32 rank, bool isCross)
{
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(ltId);
	if (pLTeam == NULL)
	{
		return;
	}
	LogInfo("send rank reward [%llu], rank [%u], is_cross[%u]", ltId, rank, isCross);
	std::vector<Sequence<UINT32, 2>> rewards;
	LeagueMatchTableMgr::Instance()->GetRankReward(rank, rewards);

	std::vector<ItemDesc> items;
	for(size_t i = 0; i < rewards.size(); ++i)
	{
		items.push_back(ItemDesc(rewards[i][0], rewards[i][1], true));
	}

	std::string title = "null";
	std::string content = "null";
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(isCross ? MailConf_LeagueCrossRank: MailConf_LeagueRank);
	if(pMailConf)
	{
		title = pMailConf->m_title;
		content = XCommon::ReplaceStr(pMailConf->m_content, "0", ToString(rank));
	}

	MailData mailData(title, content, items, MAIL_EXPIRE_TIME);

	const auto& leagueMemebers = pLTeam->GetAllMember();
	for(size_t i = 0; i < leagueMemebers.size(); ++i)
	{
		UINT64 roleId = leagueMemebers[i].roleId;
		CMailMgr::Instance()->SendMail(roleId, mailData);
	}
}

void LeagueTeamMgr::SendEliRoundReward(UINT32 round, UINT64 ltId, bool isWin, bool isCross)
{
	if(0 == ltId)
	{
		return;
	}
	LeagueTeam* pLTeam = LeagueTeamMgr::Instance()->GetLeagueTeamById(ltId);
	if(NULL == pLTeam)
	{
		SSError << " get leagueteam err leagueid = " << ltId << END;
		return;
	}

	LBRRoundType type = LBR_RT_NONE;
	switch(round)
	{
	case 1:// 8强
		{
			type  = (isWin)?LBR_8_WIN:LBR_8_LOSE; 
		}
		break;
	case 2:// 4强
		{
			type = (isWin)?LBR_4_WIN:LBR_4_LOSE;
		}
		break;
	case 3:// 2强
		{
			type = (isWin)?LBR_2_WIN:LBR_2_LOSE;
		}
		break;
	default:
		break;
	}

	LeagueBattleReward::RowData* pConf = LeagueMatchTableMgr::Instance()->GetBattleRewardConf(isCross ? LBR_SEV_CROSS : LBR_SEV_SELF, type);
	if(NULL == pConf)
	{
		SSError << " get leaguebattlereward conf err type = " << type << ", isCross" << isCross << END;
		return;
	}

	std::vector<ItemDesc> items;
	for(size_t i = 0; i < pConf->reward.size(); ++i)
	{
		items.push_back(ItemDesc(pConf->reward[i][0], pConf->reward[i][1], true));
	}
	std::string title = "null";
	std::string content = "null";
	auto pMailConf = MailConfig::Instance()->GetMailConf(pConf->mailid);
	if(pMailConf)
	{
		title = pMailConf->m_title;
		content = pMailConf->m_content;
	}

	MailData mailData(title, content, items, MAIL_EXPIRE_TIME);

	const auto& leagueMemebers = pLTeam->GetAllMember();
	for(size_t i = 0; i < leagueMemebers.size(); ++i)
	{
		UINT64 roleId = leagueMemebers[i].roleId;
		CMailMgr::Instance()->SendMail(roleId, mailData);
	}
}

void LeagueTeamMgr::TestGiveRoundReward(UINT32 round, UINT64 leagueId, bool isWin, bool isCross)
{
	SendEliRoundReward(round, leagueId, isWin, isCross);
}
