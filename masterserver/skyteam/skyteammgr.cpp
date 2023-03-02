#include "pch.h"
#include "skyteammgr.h"

#include "skyteam.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "role/rolemanager.h"
#include "util/timespecificmgr.h"
#include "global/GlobalConfigDBBuild.h"
#include "team/team.h"
#include "table/expeditionconfigmgr.h"
#include "tsssdk/MsTssHandler.h"
#include "forbid/forbidwordmgr.h"
#include "util/XCommon.h"
#include "table/globalconfig.h"
#include "team/teammember.h"
#include "config.h"
#include "define/itemdef.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "network/dblink.h"
#include "tablemgr/skycrafttablemgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/skycraftranklist.h"
#include "db/rpcm2d_skyteamdata2db.h"

SkyTeamMgr::SkyTeamMgr()
:m_id(0)
,m_lastUpdateTime(0)
,m_gmRoleNum(0)
{

}

SkyTeamMgr::~SkyTeamMgr()
{

}

bool SkyTeamMgr::Init()
{
	if (!LoadFromDB())
	{
		LogError("load from db failed");
		return false;
	}
	CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);

	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&SkyTeamMgr::OnDayPass, this));
	const std::string temp = CGlobalConfigDBBuild::Instance()->GetConfigValue(eSkyTeamDailyPassTime);
	m_lastUpdateTime = convert<UINT32>(temp);
	if (!GameTime::IsInSameDay(GameTime::GetTime(), m_lastUpdateTime, true))
	{
		OnDayPass();
	}

	return true;
}

void SkyTeamMgr::Uninit()
{
}

void SkyTeamMgr::Save2DB(UINT32 num)
{
	UINT32 i = 0;
	while (!m_dirtyList.empty())
	{
		UINT64 id = m_dirtyList.pop_front();
		auto it = m_teams.find(id);
		if (it == m_teams.end())
		{
			continue;
		}
		it->second.UpdateToDB();
		i++;
		if (num && i >= num)
		{
			break;
		}
	}
}

void SkyTeamMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	Save2DB(20);
}

bool SkyTeamMgr::LoadFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();

	std::string sql("select _id, base, record from skyteam");
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(sql.c_str(), sql.size(), &poRes))
	{
		LogError("Read skyteam failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	UINT64 qwBegin = TimeUtil::GetMilliSecond();
	while(poRes->FetchRow())
	{
		char* pVal   = NULL;
		UINT32 dwLen = 0;

		// id
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

		KKSG::SkyTeamSaveData data;
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

		// record
		if(poRes->GetFieldValue(2, &pVal, dwLen))
		{
			data.mutable_record()->assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'record' column failed");
			return false;
		}

		SkyTeam& team = m_teams[id];
		team.LoadFromDB(data);
		team.OnLoaded();

		SetNameUsed(team.GetName());	
	}

	poRes->Release();
	SSInfo << "Load skyteam num: " << m_teams.size() << END;

	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all skyteam data from db use time: %llu ms", qwTimeUsed);
	return true;
}

bool SkyTeamMgr::HasSkyTeam(UINT64 roleId)
{
	auto it = m_roleId2teamId.find(roleId);
	return it == m_roleId2teamId.end() ? false : true;
}

bool SkyTeamMgr::IsInSameSkyTeam(UINT64 roleId1, UINT64 roleId2)
{
	if (roleId1 == roleId2)
	{
		return false;
	}
	UINT64 stId1 = GetSkyTeamIdByRoleId(roleId1);
	if (stId1 == 0)
	{
		return false;
	}
	UINT64 stId2 = GetSkyTeamIdByRoleId(roleId2);
	if (stId2 == 0)
	{
		return false;
	}
	return stId1 == stId2;
}

SkyTeam* SkyTeamMgr::GetSkyTeamByRoleId(UINT64 roleId)
{
	UINT64 stId = GetSkyTeamIdByRoleId(roleId);
	if (stId == 0)
	{
		return NULL;
	}
	return GetSkyTeamById(stId);
}

SkyTeam* SkyTeamMgr::GetSkyTeamById(UINT64 stId)
{
	auto it = m_teams.find(stId);
	if (it == m_teams.end())
	{
		return NULL;
	}
	return &it->second;
}

UINT64 SkyTeamMgr::GetSkyTeamIdByRoleId(UINT64 roleId)
{
	auto it = m_roleId2teamId.find(roleId);
	if (it == m_roleId2teamId.end())
	{
		return 0;
	}
	return it->second;
}

int SkyTeamMgr::LeaveMember(UINT64 roleId, bool force)
{
	SkyTeam* pSTeam = GetSkyTeamByRoleId(roleId);
	if (pSTeam == NULL)
	{
		return KKSG::ERR_SKY_HAS_NO_TEAM;
	}
	int ret = pSTeam->LeaveMember(roleId, force);
	if (ret != KKSG::ERR_SUCCESS)
	{
		return ret;
	}
	auto& members = pSTeam->GetAllMember();

	// 发送解散邮件
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	const MailConf* mailConf = MailConfig::Instance()->GetMailConf(MailConf_SkyTeamDissolve);
	if (mailConf && su)
	{
		std::string content = MailConfig::Instance()->ReplaceString("%s", su->GetName(), mailConf->m_content);
		std::vector<ItemDesc> item;
		MailData mail(mailConf->m_title, content, item, MAIL_EXPIRE_TIME);
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			CMailMgr::Instance()->SendMail(it->roleId, mail);
		}
	}

	pSTeam->DelToDB();
	pSTeam->OnDissolve();

	for (auto it = members.begin(); it != members.end(); ++it)
	{
		RemoveRoleSkyTeam(it->roleId);
	}
	RemoveNameUsed(pSTeam->GetName());
	RemoveSkyTeam(pSTeam->GetId());
	return KKSG::ERR_SUCCESS;
}

void SkyTeamMgr::AddRoleSkyTeam(UINT64 roleId, UINT64 stId)
{
	m_roleId2teamId[roleId] = stId;
}

void SkyTeamMgr::RemoveRoleSkyTeam(UINT64 roleId)
{
	m_roleId2teamId.erase(roleId);
}

void SkyTeamMgr::AddToUpdateList(UINT64 stId)
{
	m_dirtyList.push_back(stId);
}

void SkyTeamMgr::RemoveSkyTeam(UINT64 stId)
{
	m_teams.erase(stId);
}

int SkyTeamMgr::CheckNameValid(const std::string& name)
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
	if(nameLen < GetGlobalConfig().SkyTeamNameLenMin)
	{
		return KKSG::ERR_NAME_TOO_SHORT;
	}
	if(nameLen > GetGlobalConfig().SkyTeamNameLenMax)
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

bool SkyTeamMgr::IsNameUsed(const std::string& name)
{
	return m_usedName.find(name) != m_usedName.end();
}

void SkyTeamMgr::SetNameUsed(const std::string& name)
{
	m_usedName.insert(name);
}

void SkyTeamMgr::RemoveNameUsed(const std::string& name)
{
	m_usedName.erase(name);
}

void SkyTeamMgr::OnSeasonEnd()
{
	// 赛季结束, 所有队伍解散
	std::string title = "null";
	std::string content = "null";
	auto pMailConf = MailConfig::Instance()->GetMailConf(MailConf_SkyTeamDissolveSeasonEnd);
	if(pMailConf)
	{
		title = pMailConf->m_title;
		content = pMailConf->m_content;
	}
	std::vector<ItemDesc> items;
	MailData mailData(title, content, items, MAIL_EXPIRE_TIME);

	for (auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		const auto& members = it->second.GetAllMember();
		for(size_t i = 0; i < members.size(); ++i)
		{
			UINT64 roleId = members[i].roleId;
			CMailMgr::Instance()->SendMail(roleId, mailData);
		}
	}

	m_teams.clear();
	m_roleId2teamId.clear();
	m_usedName.clear();
	m_dirtyList.clear();

	RpcM2D_SkyTeamData2DB* rpc = RpcM2D_SkyTeamData2DB::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_DelAll);
	DBLink::Instance()->SendTo(*rpc);
}

void SkyTeamMgr::SendAllRankReward()
{
	SkyCraftRankList* rlist = CRankListMgr::Instance()->GetSkyCraftRankList();
	std::vector<SkyCraftRankInfo*> ranks;
	rlist->GetRankInfo(ranks);
	for (size_t i = 0; i < ranks.size(); ++i)
	{
		SendRankReward(ranks[i]->GetUniqueId(), i + 1);
	}
}

void SkyTeamMgr::SendRankReward(UINT64 skyId, UINT32 rank)
{
	SkyTeam* pSTeam = GetSkyTeamById(skyId);
	if (pSTeam == NULL)
	{
		return;
	}
	LogInfo("send rank reward [%llu], rank [%u]", skyId, rank);
	std::vector<Sequence<UINT32, 2>> rewards;
	SkyCraftTableMgr::Instance()->GetRankReward(rank, rewards);

	std::vector<ItemDesc> items;
	for(size_t i = 0; i < rewards.size(); ++i)
	{
		items.push_back(ItemDesc(rewards[i][0], rewards[i][1], true));
	}

	std::string title = "null";
	std::string content = "null";
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_SkyCraftRankReward);
	if (pMailConf)
	{
		title = pMailConf->m_title;
		content = XCommon::ReplaceStr(pMailConf->m_content, "0", ToString(rank));
	}

	MailData mailData(title, content, items, MAIL_EXPIRE_TIME);
	const auto& members = pSTeam->GetAllMember();
	for(size_t i = 0; i < members.size(); ++i)
	{
		UINT64 roleId = members[i].roleId;
		CMailMgr::Instance()->SendMail(roleId, mailData);
	}
}

void SkyTeamMgr::SendEliRoundReward(UINT32 round, UINT64 stId, bool isWin)
{
	if(0 == stId)
	{
		return;
	}
	SkyTeam* pSTeam = GetSkyTeamById(stId);
	if (pSTeam == NULL)
	{
		return;
	}
	SKRRoundType type = SKR_RT_NONE;
	switch (round)
	{
	case KKSG::SCEliRound_8to4: // 8进4奖励
		{
			type = isWin ? SKR_8_WIN : SKR_8_LOSE;
		}
		break;
	case KKSG::SCEliRound_4to2: // 4进2奖励
		{
			type = isWin ? SKR_4_WIN : SKR_4_LOSE;
		}
		break;
	case KKSG::SCEliRound_2to1: // 决赛
		{
			type = isWin ? SKR_2_WIN : SKR_2_LOSE;
		}
		break;
	default:
		LogError("invalid round[%u]", round);
		return;
	}

	SkyCraftRewardTable::RowData* pConf = SkyCraftTableMgr::Instance()->GetBattleRewardConf(type);
	if(NULL == pConf)
	{
		LogError("get skycraft eli reward conf error, type[%u]", type);
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
	if (pMailConf)
	{
		title = pMailConf->m_title;
		content = pMailConf->m_content;
	}

	MailData mailData(title, content, items, MAIL_EXPIRE_TIME);
	const auto& members = pSTeam->GetAllMember();
	for(size_t i = 0; i < members.size(); ++i)
	{
		UINT64 roleId = members[i].roleId;
		CMailMgr::Instance()->SendMail(roleId, mailData);
	}
}

UINT64 SkyTeamMgr::NewId()
{
	// 16位serverid 32位时间戳 16位自增id
	UINT64 temp = MSConfig::Instance()->GetServerID() & 0xFFFF;
	temp <<= 48;
	temp |= (TimeUtil::GetTime() & 0xFFFFFFFF) << 16;
	temp |= ((++m_id) & 0xFFFF);
	return temp;
}

int SkyTeamMgr::CheckCreateTeam(Team* pTeam, const std::string& name, UINT64& errRoleId)
{
	errRoleId = 0;
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}

	/* TODO
	if (!LeagueSeasonMgr::Instance()->IsInApplyTime())
	{
		return KKSG::ERR_LEAGUE_NOT_IN_APPLY_TIME;
	}
	*/

	// 人数
	if (pTeam->GetMemberCount() != GetTeamRoleNum())
	{
		return KKSG::ERR_SKY_TEAM_ROLE_NUM_INVALID;
	}

	// 是否未加入联赛战队
	std::vector<UINT64> roleIds;
	pTeam->GetRoleIDs(roleIds);
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)
	{
		UINT64 roleId = *it;
		if (HasSkyTeam(roleId))
		{
			errRoleId = roleId;
			return KKSG::ERR_SKY_ALREADY_HAS_TEAM;
		}
	}
	int ret = CheckNameValid(name);
	if (ret != 0)
	{
		return ret;
	}
	return KKSG::ERR_SUCCESS;
}

int SkyTeamMgr::StartCreateTeam(Team* pTeam)
{
	if (pTeam == NULL)
	{
		return KKSG::ERR_TEAM_NOT_EXIST;
	}
	UINT64 leaderId = pTeam->GetLeader();
	SkyTeam* pSTeam = GetSkyTeamByRoleId(leaderId);
	if (pSTeam)
	{
		return KKSG::ERR_SKY_ALREADY_HAS_TEAM;
	}

	UINT64 errRoleId = 0;
	int ret = CheckCreateTeam(pTeam, pTeam->GetLeagueTeamName(), errRoleId);
	if (ret != 0)
	{
		return ret;
	}

	// 建立关系
	std::vector<UINT64> roleIds;
	pTeam->GetRoleIDs(roleIds);
	CreateSkyTeam(roleIds, leaderId, pTeam->GetLeagueTeamName());

	return KKSG::ERR_SUCCESS;
}

SkyTeam* SkyTeamMgr::CreateSkyTeam(const std::vector<UINT64>& roleIds, UINT64 creatorId, const std::string& name)
{
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)	
	{
		if (HasSkyTeam(*it))
		{
			return NULL;
		}
	}
	UINT64 id = NewId();
	SkyTeam& team = m_teams[id];
	team.SetId(id);
	team.SetName(name);
	team.SetCreatorId(creatorId);
	team.SetMemberRoleIds(roleIds);
	team.OnCreate();
	team.AddToDB();

	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)	
	{
		AddRoleSkyTeam(*it, id);
	}
	SetNameUsed(name);
	
	std::ostringstream oss;
	for (auto it = roleIds.begin(); it != roleIds.end(); ++it)
	{
		oss << *it << ",";
	}
	LogInfo("roles [%s] create skyteam [%llu] name[%s]", oss.str().c_str(), id, name.c_str());
	return &team;
}

bool SkyTeamMgr::GMCreateTeam(const std::vector<UINT64>& roleIds, const std::string& name, std::string& output)
{
	int ret = CheckNameValid(name);
	if (ret != 0)
	{
		output = "invalid name";
		return false;
	}
	SkyTeam* pSTeam = CreateSkyTeam(roleIds, roleIds[0], name);
	if (pSTeam)
	{
		output = "Create Success";
		return true;
	}

	output = "Create Failed";
	return false;
}

UINT32 SkyTeamMgr::GetTeamRoleNum()
{
	return m_gmRoleNum ? m_gmRoleNum : GetGlobalConfig().SkyTeamRoleNum;
}

bool SkyTeamMgr::OnDayPass()
{
	for (auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		it->second.OnDayPass();
	}
	m_lastUpdateTime = GameTime::GetTime();

	CGlobalConfigDBBuild::Instance()->SetConfigValue(eSkyTeamDailyPassTime, ToString(m_lastUpdateTime));	
	CGlobalConfigDBBuild::Instance()->UpdateToDb(eSkyTeamDailyPassTime);
	return true;
}
