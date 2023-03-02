#include "pch.h"
#include "skyteam.h"

#include "skyteammgr.h"
#include "network/dblink.h"
#include "role/rolemanager.h"
#include "util/gametime.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "config.h"
#include "table/globalconfig.h"
#include "timeutil.h"
#include "scene/sceneswitch.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "mail/mailmgr.h"
#include "db/rpcm2d_skyteamdata2db.h"
#include "skyteam/ptcm2c_notifyskyteamcreate.h"
#include "ranklist/pkranklist.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/skycraftranklist.h"
#include "skycraft/skycraftmatchmgr.h"

void SkyTeamMember::FillBattleRole(KKSG::SkyCraftRoleBrief& data)
{
	data.set_roleid(roleId);
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su == NULL)
	{
		return;
	}
	data.set_name(su->GetName());
	data.set_level(su->GetLevel());
	data.set_profession(su->GetProfession());
	data.set_ppt(su->GetPowerPoint());
	PkRealTimeRankList* rk = (PkRealTimeRankList*)CRankListMgr::Instance()->GetRankList(PkRealTimeRank);
	data.set_pkpoint(rk->GetPkPoint(roleId));
}

void SkyTeamMember::FillMemberInfo(KKSG::SkyTeamMemberInfo& data)
{
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su)
	{
		su->FillRoleOutLookBrief(*data.mutable_brief());
	}
}

SkyTeam::SkyTeam()
:m_dirty(false)
,m_id(0)
,m_creatorId(0)
,m_createTime(0)
,m_score(0)
,m_state(KKSG::SkyTeamState_Idle)
,m_stateStartTime(0)
,m_matchTeamId(0)
,m_sceneId(0)
,m_gsLine(0)
,m_record(this)
{

}

SkyTeam::~SkyTeam()
{

}

void SkyTeam::LoadFromDB(const KKSG::SkyTeamSaveData& data)
{
	LoadBaseFromDB(data);
	m_record.LoadFromDB(data);
}

void SkyTeam::SaveToDB(KKSG::SkyTeamSaveData& data, bool all)
{
	data.set_id(m_id);
	if (all || m_dirty)
	{
		SaveBaseToDB(data);
	}

	if (all || m_record.IsDirty())  m_record.SaveToDB(data);
}

void SkyTeam::LoadBaseFromDB(const KKSG::SkyTeamSaveData& data)
{
	m_id = data.id();
	KKSG::SkyTeamBaseData base;
	if (!base.ParseFromString(data.base()))
	{
		LogError("parse base error");
		return;
	}
	m_createTime = base.create_time();
	m_name = base.name();
	m_score = base.score();
	m_creatorId = base.creatorid();

	m_members.clear();
	for (int i = 0; i < base.member_size(); ++i)
	{
		SkyTeamMember mem;
		mem.FromKKSG(base.member(i));
		m_members.push_back(mem);
	}
}

void SkyTeam::SaveBaseToDB(KKSG::SkyTeamSaveData& data)
{
	data.set_id(m_id);
	KKSG::SkyTeamBaseData base;
	base.set_create_time(m_createTime);
	base.set_name(m_name);
	base.set_score(m_score);
	base.set_creatorid(m_creatorId);

	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		KKSG::SkyTeamMemberData* mem = base.add_member();
		it->ToKKSG(*mem);
	}

	base.SerializeToString(data.mutable_base());

	m_dirty = false;
}

void SkyTeam::AddToDB()
{
	RpcM2D_SkyTeamData2DB* rpc = RpcM2D_SkyTeamData2DB::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Insert);
	SaveToDB(*rpc->m_oArg.mutable_data(), true);
	DBLink::Instance()->SendTo(*rpc);
}

void SkyTeam::UpdateToDB()
{
	RpcM2D_SkyTeamData2DB* rpc = RpcM2D_SkyTeamData2DB::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Update);
	SaveToDB(*rpc->m_oArg.mutable_data(), false);
	DBLink::Instance()->SendTo(*rpc);
}

void SkyTeam::DelToDB()
{
	RpcM2D_SkyTeamData2DB* rpc = RpcM2D_SkyTeamData2DB::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Del);
	rpc->m_oArg.mutable_data()->set_id(m_id);
	DBLink::Instance()->SendTo(*rpc);
}

void SkyTeam::OnLoaded()
{

}

void SkyTeam::OnCreate()
{
	m_createTime = GameTime::GetTime();
	m_score = GetGlobalConfig().SkyTeamInitPoint;

	PtcM2C_NotifySkyTeamCreate ntf;
	ntf.m_Data.set_sky_teamid(m_id);
	ntf.m_Data.set_name(m_name);
	BroadCast(ntf);

	// 邮件
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		CMailMgr::Instance()->SendMail(it->roleId, MailConf_SkyTeamCreate);
	}
}

void SkyTeam::OnDayPass()
{
	m_record.OnDayPass();
}

void SkyTeam::OnDissolve()
{
	CRankListMgr::Instance()->GetSkyCraftRankList()->DelSkyTeam(m_id);
	SkyCraftMatchMgr::Instance()->ForceCancelMatch(m_id);
}

void SkyTeam::SetMemberRoleIds(const std::vector<UINT64>& roleIds)
{
	m_members.clear();
	for (size_t i = 0; i < roleIds.size(); ++i)
	{
		UINT64 roleId = roleIds[i];
		if (HasMember(roleId))
		{
			continue;
		}
		SkyTeamMember member;
		member.roleId = roleId;
		m_members.push_back(member);
	}
	SetDirty();
}

void SkyTeam::SetDirty(bool dirty)
{
	if (m_dirty == false && dirty)
	{
		SkyTeamMgr::Instance()->AddToUpdateList(m_id);
	}
	m_dirty = dirty;
}

bool SkyTeam::HasMember(UINT64 roleId)
{
	return GetMember(roleId) != NULL;
}

SkyTeamMember* SkyTeam::GetMember(UINT64 roleId)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		SkyTeamMember& mem = *it;
		if (mem.roleId == roleId)
		{
			return &mem;
		}
	}
	return NULL;
}

KKSG::ErrorCode SkyTeam::LeaveMember(UINT64 roleId, bool force /*= false*/)
{
	SkyTeamMember* mem = GetMember(roleId);
	if (mem == NULL)
	{
		return KKSG::ERR_FAILED;
	}
	auto state = GetState();
	if (state == KKSG::SkyTeamState_Match)
	{
		return KKSG::ERR_SKY_TEAM_IN_MATCH;
	}
	if (state == KKSG::SkyTeamState_Battle)
	{
		return KKSG::ERR_SKY_TEAM_IN_BATTLE;
	}

	// 已经参战过了不能退出战队 
	if (!force && GetTeamRecord().GetTotalNum() > 0)
	{
		return KKSG::ERR_SKY_ALREADY_FIGHTED;
	}

	LogInfo("role [%llu] leave sky team[%llu] name[%s]", roleId, m_id, m_name.c_str());
	return KKSG::ERR_SUCCESS;
}

void SkyTeam::BroadCast(const CProtocol& ptc)
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

KKSG::SkyTeamState SkyTeam::GetState()
{
	if (m_state == KKSG::SkyTeamState_Idle)
	{
		return m_state;
	}

	// 状态容错
	UINT32 now = TimeUtil::GetTime();
	if (now >= m_stateStartTime + 15 * 60)
	{
		m_state = KKSG::SkyTeamState_Idle;
	}
	return m_state;
}

void SkyTeam::SetState(KKSG::SkyTeamState state)
{
	if (m_state == state)
	{
		return;
	}
	m_state = state;
	m_stateStartTime = TimeUtil::GetTime();
	LogInfo("team[%llu] state[%u]", m_id, m_state);

	// 更新队伍状态
	/*
	PtcM2C_UpdateLeagueTeamState ntf;
	ntf.m_Data.set_state(m_state);
	BroadCast(ntf);
	*/
}


void SkyTeam::SetScore(UINT32 score)
{
	m_score = score;
	SetDirty();
}

void SkyTeam::SetBattleStart(UINT32 sceneId, UINT32 gsLine, bool isCross)
{
	m_sceneId = sceneId;	
	m_gsLine = gsLine;
}

void SkyTeam::GMSetScore(UINT32 score)
{
	LogInfo("skyteam[%llu] gm set score [%u]", m_id, score);
	m_score = score;

	CRankListMgr::Instance()->GetSkyCraftRankList()->OnChange(m_id, m_score, m_record.GetTotalWinRate(), GameTime::GetTime());
	SetDirty();
}

void SkyTeam::FillBattleData(KKSG::SkyCraftTeamData& data)
{
	data.set_stid(m_id);
	data.set_name(m_name);
	data.set_score(m_score);
	data.set_rank(CRankListMgr::Instance()->GetSkyCraftRankList()->GetRank(m_id));
	data.set_total_num(m_record.GetTotalNum());
	data.set_win_num(m_record.GetTotalWinNum());
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		it->FillBattleRole(*data.add_members());
	}
}

void SkyTeam::FillEliTeamInfo(KKSG::SCEliTeamInfo& data)
{
	data.set_stid(m_id);
	data.set_name(m_name);
}

void SkyTeam::FillTeamDetail(KKSG::SkyTeamDetail& data)
{
	data.set_stid(m_id);
	data.set_name(m_name);
	data.set_score(m_score);
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		it->FillMemberInfo(*data.add_members());
	}
}

