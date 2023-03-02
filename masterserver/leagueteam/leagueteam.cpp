#include "pch.h"
#include "leagueteam.h"
#include "db/rpcm2d_saveleagueteamdata.h"
#include "network/dblink.h"
#include "leagueteammgr.h"
#include "role/rolemanager.h"
#include "league/ptcm2c_notifyleagueteamcreate.h"
#include "league/ptcm2c_notifyleagueteamdissolve.h"
#include "util/gametime.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "config.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/pkranklist.h"
#include "table/globalconfig.h"
#include "league/ptcm2c_updateleagueteamstate.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/leagueranklist.h"
#include "timeutil.h"
#include "scene/sceneswitch.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "mail/mailmgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "loghelper/tlogr.h"

void LeagueTeamMember::FillMemberDetail(KKSG::LeagueTeamMemberDetail& detail)
{
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su)
	{
		su->FillRoleOutLookBrief(*detail.mutable_brief());
		PkRealTimeRankList* rk = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(PkRealTimeRank);
		detail.set_pkpoint(rk->GetPkPoint(roleId));
	}
}

void LeagueTeamMember::FillBattleRole(KKSG::LeagueBattleRoleBrief& data)
{
	data.set_roleid(roleId);
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su == NULL)
	{
		return;
	}
	data.set_name(su->GetName());
	data.set_level(su->GetLevel());
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_profession(su->GetProfession());
	data.set_ppt(su->GetPowerPoint());
	PkRealTimeRankList* rk = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(PkRealTimeRank);
	data.set_pkpoint(rk->GetPkPoint(roleId));
	data.set_account(su->GetAccount());
}

LeagueTeam::LeagueTeam()
:m_dirty(false)
,m_id(0)
,m_creatorId(0)
,m_createTime(0)
,m_score(0)
,m_crossScore(0)
,m_state(KKSG::LeagueTeamState_Idle)
,m_stateStartTime(0)
,m_matchTeamId(0)
#define LEAGUE_TEAM_MODULE(module) ,m_##module(this)
#include "leagueteammodule.h"
#undef LEAGUE_TEAM_MODULE
{
}

LeagueTeam::~LeagueTeam()
{
}

void LeagueTeam::LoadBaseFromDB(const KKSG::LeagueTeamSaveData& data)
{
	m_id = data.id();
	KKSG::LeagueTeamBaseData base;
	if (!base.ParseFromString(data.base()))
	{
		LogError("parse base error");
		return;
	}
	m_createTime = base.create_time();
	m_name = base.name();
	m_score = base.score();
	m_crossScore = base.cross_score();
	m_creatorId = base.creatorid();

	m_members.clear();
	for (int i = 0; i < base.member_size(); ++i)
	{
		LeagueTeamMember mem;
		mem.FromKKSG(base.member(i));
		m_members.push_back(mem);
		LeagueTeamMgr::Instance()->AddRoleLeagueTeam(mem.roleId, m_id);
	}
	LeagueTeamMgr::Instance()->SetNameUsed(m_name);
}

void LeagueTeam::SaveBaseToDB(KKSG::LeagueTeamSaveData& data)
{
	data.set_id(m_id);

	KKSG::LeagueTeamBaseData base;
	base.set_create_time(m_createTime);
	base.set_name(m_name);
	base.set_score(m_score);
	base.set_cross_score(m_crossScore);
	base.set_creatorid(m_creatorId);

	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		KKSG::LeagueTeamMemberData* mem = base.add_member();
		it->ToKKSG(*mem);
	}
	base.SerializeToString(data.mutable_base());

	m_dirty = false;
}

void LeagueTeam::LoadFromDB(const KKSG::LeagueTeamSaveData& data)
{
	LoadBaseFromDB(data);	
#define LEAGUE_TEAM_MODULE(module)	m_##module.LoadFromDB(data);
#include "leagueteammodule.h"
#undef LEAGUE_TEAM_MODULE
}

void LeagueTeam::AddToDB()
{
	RpcM2D_SaveLeagueTeamData* rpc = RpcM2D_SaveLeagueTeamData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Insert);
	KKSG::LeagueTeamSaveData& data = *rpc->m_oArg.mutable_data();

	SaveBaseToDB(data);
#define LEAGUE_TEAM_MODULE(module) m_##module.SaveToDB(data);
#include "leagueteammodule.h"
#undef LEAGUE_TEAM_MODULE
	DBLink::Instance()->SendTo(*rpc);
}

void LeagueTeam::UpdateToDB()
{
	RpcM2D_SaveLeagueTeamData* rpc = RpcM2D_SaveLeagueTeamData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Update);
	KKSG::LeagueTeamSaveData& data = *rpc->m_oArg.mutable_data();

	data.set_id(m_id);
	if (m_dirty)
	{
		SaveBaseToDB(data);
		m_dirty = false;
	}
#define LEAGUE_TEAM_MODULE(module) \
	if (m_##module.IsDirty())\
	{\
		m_##module.SaveToDB(data);\
		m_##module.SetDirty(false);\
	}
#include "leagueteammodule.h"
#undef LEAGUE_TEAM_MODULE
	DBLink::Instance()->SendTo(*rpc);
}

void LeagueTeam::DelToDB()
{
	RpcM2D_SaveLeagueTeamData* rpc = RpcM2D_SaveLeagueTeamData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Del);
	rpc->m_oArg.mutable_data()->set_id(m_id);
	DBLink::Instance()->SendTo(*rpc);
}

void LeagueTeam::SetMemberRoleIds(const std::vector<UINT64>& roleIds)
{
	m_members.clear();
	for (size_t i = 0; i < roleIds.size(); ++i)
	{
		UINT64 roleId = roleIds[i];
		if (HasMember(roleId))
		{
			continue;
		}
		LeagueTeamMember member;
		member.roleId = roleId;
		member.lastBattleTime = 0;
		m_members.push_back(member);
	}
}

void LeagueTeam::OnLoaded()
{

}

void LeagueTeam::OnCreate()
{
	m_createTime = GameTime::GetTime();
	m_score = GetGlobalConfig().LeagueInitPoint;
	m_crossScore = GetGlobalConfig().LeagueInitPoint;

	PtcM2C_NotifyLeagueTeamCreate ntf;
	ntf.m_Data.set_league_teamid(m_id);
	ntf.m_Data.set_name(m_name);
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		ntf.m_Data.add_members(it->roleId);
	}
	BroadCast(ntf);

	// 邮件
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		CMailMgr::Instance()->SendMail(it->roleId, MailConf_LeagueTeamCreate);
	}
}

void LeagueTeam::OnDissolve()
{
	TLeagueTeamFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_TeamId = GetId();
	oLog.m_strName = GetName();
	for (auto i = m_members.begin(); i != m_members.end(); i ++)
	{
		oLog.m_roleIds.push_back(i->roleId);
	}
	oLog.m_Type = Tx_LeagueTeamType_Dissolve;
	oLog.m_Num = m_members.size();
	oLog.Do();

	// 删除排行榜
	CRankListMgr::Instance()->GetLeagueRankList()->DelLeagueTeam(m_id);
	LeagueBattleMatchMgr::Instance()->ForceCancelMatch(m_id);
}

void LeagueTeam::OnDayPass()
{
#define LEAGUE_TEAM_MODULE(module) m_##module.OnDayPass();
#include "leagueteammodule.h"
#undef LEAGUE_TEAM_MODULE
}

void LeagueTeam::SetDirty(bool dirty)
{
	if (m_dirty == false && dirty)
	{
		LeagueTeamMgr::Instance()->AddToUpdateList(m_id);
	}
	m_dirty = dirty;
}

bool LeagueTeam::HasMember(UINT64 roleId)
{
	return GetMember(roleId) != NULL;
}

LeagueTeamMember* LeagueTeam::GetMember(UINT64 roleId)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		LeagueTeamMember& mem = *it;
		if (mem.roleId == roleId)
		{
			return &mem;
		}
	}
	return NULL;
}

KKSG::ErrorCode LeagueTeam::LeaveMember(UINT64 roleId, bool force)
{
	LeagueTeamMember* mem = GetMember(roleId);
	if (mem == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	auto state = GetState();
	if (state == KKSG::LeagueTeamState_Match)
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_MATCH;
	}
	if (state == KKSG::LeagueTeamState_Battle)
	{
		return KKSG::ERR_LEAGUE_TEAM_IN_BATTLE;
	}

	// 已经参战过了不能退出战队
	if (!force && GetLeagueTeamBattleRecord().GetTotalNum() > 0)
	{
		return KKSG::ERR_LEAGUE_ALREADY_FIGHTED;
	}

	LogInfo("role [%llu] leave league team[%llu] name[%s]", roleId, m_id, m_name.c_str());

	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		LeagueTeamMgr::Instance()->RemoveRoleLeagueTeam(it->roleId);
	}
	LeagueTeamMgr::Instance()->RemoveNameUsed(m_name);

	PtcM2C_NotifyLeagueTeamDissolve ntf;
	ntf.m_Data.set_leave_roleid(roleId);
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su)
	{
		ntf.m_Data.set_leave_rolename(su->GetName());
	}
	BroadCast(ntf);
	DelToDB();

	// 发送邮件
	const MailConf* mailConf = MailConfig::Instance()->GetMailConf(MailConf_LeagueTeamDissolve);
	if (mailConf && su)
	{
		std::string content = MailConfig::Instance()->ReplaceString("%s", su->GetName(), mailConf->m_content);
		std::vector<ItemDesc> item;
		MailData mail(mailConf->m_title, content, item, MAIL_EXPIRE_TIME);
		for (auto it = m_members.begin(); it != m_members.end(); ++it)
		{
			CMailMgr::Instance()->SendMail(it->roleId, mail);
		}
	}

	OnDissolve();
	// 这个之后就被删了
	LeagueTeamMgr::Instance()->RemoveLeagueTeam(m_id);
	return KKSG::ERR_SUCCESS;
}

void LeagueTeam::BroadCast(const CProtocol& ptc)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(it->roleId);
		if (role)
		{
			role->Send(ptc);
		}
	}
}

void LeagueTeam::FillLeagueTeamDetail(KKSG::LeagueTeamDetail& detail)
{
	detail.set_league_teamid(m_id);
	detail.set_teamname(m_name);
	detail.set_serverid(MSConfig::Instance()->GetServerID());
	detail.set_servername(MSConfig::Instance()->GetName());
	detail.set_zonename(MSConfig::Instance()->GetZoneName());
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		it->FillMemberDetail(*detail.add_members());
	}
}

void LeagueTeam::FillLeagueTeamBattleData(KKSG::LeagueBattleTeamData& data)
{
	LeagueRankList* ranklist = CRankListMgr::Instance()->GetLeagueRankList();
	data.set_league_teamid(m_id);
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_name(m_name);
	data.set_score(m_score);
	data.set_rank(ranklist->GetRank(m_id));
	data.set_servername(MSConfig::Instance()->GetName());
	auto& record = GetLeagueTeamBattleRecord();
	data.set_total_num(record.GetTotalNum());
	data.set_total_win(record.GetTotalWinNum());

	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		it->FillBattleRole(*data.add_members());
	}
}

void LeagueTeam::FillCrossLeagueTeamInitData(KKSG::CrossLeagueTeamData& data)
{
	data.set_id(m_id);
	data.set_serverid(MSConfig::Instance()->GetServerID());
	data.set_name(m_name);
	data.set_score(GetGlobalConfig().LeagueInitPoint);

	data.clear_memberids();
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		data.add_memberids(it->roleId);
	}
}

void LeagueTeam::SetState(KKSG::LeagueTeamState state)
{
	if (m_state == state)
	{
		return;
	}
	m_state = state;
	m_stateStartTime = TimeUtil::GetTime();
	LogInfo("team[%llu] state[%u]", m_id, m_state);

	PtcM2C_UpdateLeagueTeamState ntf;
	ntf.m_Data.set_state(m_state);
	BroadCast(ntf);
}

KKSG::LeagueTeamState LeagueTeam::GetState()
{ 
	if (m_state == KKSG::LeagueTeamState_Idle)
	{
		return m_state;
	}

	// 状态容错
	UINT32 now = TimeUtil::GetTime();
	if (now >= m_stateStartTime + 15 * 60)
	{
		m_state = KKSG::LeagueTeamState_Idle;
	}
	return m_state;
}

void LeagueTeam::SetScore(UINT32 score)
{
	m_score = score;
	SetDirty();
}

void LeagueTeam::SetCrossScore(UINT32 score)
{
	m_crossScore = score;
	SetDirty();
}

void LeagueTeam::SetBattleStart(UINT32 sceneId, UINT32 gsLine, bool isCross)
{
	m_sceneId = sceneId;	
	m_gsLine = gsLine;
	m_isBattleCross = isCross;
	m_isBattleReadyEnd = false;
}

void LeagueTeam::SetBattleReadyEnd(UINT32 sceneId)
{
	if (m_sceneId == sceneId)
	{
		m_isBattleReadyEnd = true;
	}
}

void LeagueTeam::EnterBattleScene(std::vector<CRole*>& roles)
{
	if (m_state != KKSG::LeagueTeamState_Battle)
	{
		return;
	}
	for (auto it = roles.begin(); it != roles.end(); ++it)
	{
		CRole* pRole = *it;
		if (pRole == NULL)
		{
			continue;
		}
		if (pRole->IsInBattle())
		{
			LogInfo("role[%llu] is already in battle", pRole->GetID());
			continue;
		}

		KKSG::SceneSwitchData oData;
		// 准备结束, 用观战者形式进入
		if (m_isBattleReadyEnd)
		{
			oData.add_watchers(pRole->GetID());
		}
		CSceneSwitch::EnterScene(pRole, m_sceneId, oData, m_gsLine, m_isBattleCross);
	}
}

void LeagueTeam::GMSetScore(UINT32 score)
{
	m_score = score;
	CRankListMgr::Instance()->GetLeagueRankList()->OnChange(m_id, score, m_LeagueTeamBattleRecord.GetTotalWinRate(),
		GameTime::GetTime(), m_LeagueTeamBattleRecord.GetMaxContinueWin(), m_LeagueTeamBattleRecord.GetTotalNum());
	SetDirty();
}

void LeagueTeam::SyncCrossData(const KKSG::CrossLeagueTeamData& data)
{
	if (m_id != data.id())
	{
		return;
	}

	m_crossScore = data.score();
	m_LeagueTeamBattleRecord.SyncCrossData(data);
	SetDirty();
}


