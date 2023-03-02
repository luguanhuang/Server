#include "pch.h"
#include "guild.h"
#include <time.h>
#include "foreach.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "guildlistener.h"
#include "guildmgr.h"
#include "pb/project.pb.h"
#include "scene/scene.h"
#include "guild/guildboss.h"
#include "tsssdk/tss_sdk_uic.h"
#include "guild/ptcm2c_notifyguildskilldata.h"
#include "guild/ptcg2m_createguildbosssingle.h"
#include "define/maildef.h"
#include "guildlistener.h"
#include "guild/ptcm2d_saveguilddata.h"
#include "network/dblink.h"
#include "guild/ptcm2c_loginguildinfo.h"
#include "guild/ptcm2c_sendguildskillinfo.h"
#include "guild/ptcm2g_synroleinguild.h"
#include "network/gslink.h"
#include "role/ptcm2g_synguildinfo.h"
#include "tsssdk/MsTssHandler.h"
#include "notice/noticemgr.h"
#include "table/guildSkillConfig.h"
#include "guild/ptcm2g_synguildsimpledata.h"
#include "guild/guildapps.h"
#include "guild/guildbonus.h"
#include "guild/guildgoblin.h"
#include "guild/guildcard.h"
#include "guild/guildactivity.h"
#include "guild/guildcheckin.h"
#include "guild/guildhistory.h"
#include "event/eventmgr.h"
#include "account/punishmgr.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "event/eventmgr.h"
#include "guildwage.h"
#include "guild_garden.h"
#include "guild/guildcamp.h"
#include "table/globalconfig.h"
#include "guild/guilddonate.h"
#include "reswar/reswarmgr.h"
#include "loghelper/tlogr.h"
#include "loghelper/reportdatamgr.h"
#include "guild/guildterritorybattle.h"
#include "worldboss/worldbossmgr.h"
#include "guild/ptcm2c_modifyguildnamentf.h"
#include "idutil.h"
#include "util/XCommon.h"
#include "team/teamtranshandler.h"
#include "util/gametime.h"
#include "config.h"
#include "tablemgr/backflowtablemgr.h"

#define CallBack(Fun, ...)   ForeachCall(m_listener, Fun, ## __VA_ARGS__) 

///////////////////////////////////////////////////////////
GuildBrief::GuildBrief()
:m_bindInfo(this)
{
	m_ID = 0;
	m_LeaderID = 0;
	m_Level = 1;
	m_exp = 0;
	m_lastExp = 0;
	m_Icon = 0;
	m_RecruitPPT = 0;
	m_NeedApproval = 0;
	m_rank = 0;
	m_capcity = 0;
	m_dare = 0;
	m_dareLvl = m_Level;
	m_bossIndex = 0;
	m_openTime = 0;
	m_prestige = 0;
	max_territory_level_ = 1;
}

bool GuildBrief::IsChanged()
{
	return chengeRec.TestAndReset();
}

bool GuildBrief::Load(const KKSG::GuildAllDBData & guilddata)
{
	if (!guilddata.has_brief() || guilddata.brief().empty())
	{
		return false;
	}

	KKSG::GStoreBrief brief;
	if (!brief.ParseFromString(guilddata.brief()))
	{
		return false;
	}

	m_ID = brief.gid();
	m_LeaderID = brief.leaderid();
	m_Level = brief.level();
	m_Icon = brief.icon();
	m_RecruitPPT = brief.recuitppt();
	m_NeedApproval = brief.needapproval();
	m_capcity = brief.capacity();
	m_Annoucement = brief.annoucement();
	m_LeaderName = brief.leadername();
	m_Name = brief.name();
	m_exp = brief.exp();
	m_lastExp = brief.lastexp();
	m_dare = brief.dare();
	m_dareLvl = brief.darelvl();
	m_bossIndex = brief.bossindex();
	m_openTime = brief.opentime();
	m_prestige = brief.prestige();
	for(int i = 0; i < brief.members_size(); ++i)
	{
		GuildMember member;
		const KKSG::GuildMemberInfo &m = brief.members(i);
		member.qwRoleID = m.roleid();
		member.position = (GuildPosition)m.position();
		member.contribute = m.contribute();
		member.updateTime = m.updatetime();
		member.joinTime = m.jointime();
		member.groupPos = m.bind_status();
		member.updateBindTime = m.update_bind_time();
		m_Members[member.qwRoleID] = member;
	}
	for (int cnt = 0; cnt < brief.skilldata_size(); ++cnt)
	{
		const KKSG::GuildSkillData &m = brief.skilldata(cnt);
		m_studySkill[m.skillid()] = m.maxlvl();
	}
	m_bindInfo.LoadFromKKSG(brief.bind_info());

	if (brief.max_territory_level() > 1)
	{
		max_territory_level_ = brief.max_territory_level();
	}
	else
	{
		max_territory_level_ = 1;
	}

	return true;
}

const std::string GuildBrief::ToBinary()
{
	KKSG::GStoreBrief brief;
	brief.set_gid(m_ID);
	brief.set_leaderid(m_LeaderID);
	brief.set_level(m_Level);
	brief.set_icon(m_Icon);
	brief.set_recuitppt(m_RecruitPPT);
	brief.set_needapproval(m_NeedApproval);
	brief.set_capacity(m_capcity);
	brief.set_annoucement(m_Annoucement);
	brief.set_leadername(m_LeaderName);
	brief.set_name(m_Name);
	brief.set_exp(m_exp);
	brief.set_lastexp(m_lastExp);
	brief.set_dare(m_dare);
	brief.set_bossindex(m_bossIndex);
	brief.set_darelvl(m_dareLvl);
	brief.set_opentime(m_openTime);
	brief.set_prestige(m_prestige);
	for(auto i = m_Members.begin(); i != m_Members.end(); ++i)
	{
		KKSG::GuildMemberInfo *pInfo = brief.add_members();
		pInfo->set_roleid(i->second.qwRoleID);
		pInfo->set_position(i->second.position);
		pInfo->set_contribute(i->second.contribute);
		pInfo->set_updatetime(i->second.updateTime);
		pInfo->set_jointime(i->second.joinTime);
		pInfo->set_bind_status(i->second.groupPos);
		pInfo->set_update_bind_time(i->second.updateBindTime);
	}
	for (auto iter = m_studySkill.begin(); iter != m_studySkill.end(); ++iter)
	{
		KKSG::GuildSkillData *pData = brief.add_skilldata();
		pData->set_skillid(iter->first);
		pData->set_maxlvl(iter->second);
	}
	m_bindInfo.ToKKSG(*brief.mutable_bind_info());

	brief.set_max_territory_level(max_territory_level_);

	return brief.SerializeAsString();
}

///////////////////////////////////////////////////////////////////////
Guild::Guild()
{
	m_recruitCDTime = 0;
	m_pGuildHall = NULL;
	m_pBossScene = NULL;
	m_pBrief = new GuildBrief();
	memset(m_serializers, 0, sizeof(m_serializers));
}

Guild::~Guild()
{
	for (int i = 0; i < Length<GuildHandlerType>::value; ++i)
	{
		if (m_serializers[i] != NULL)
		{
			delete m_serializers[i];
		}
	}
}

void Guild::OnCreate(CRole *pRole, UINT64 gid, const std::string &gName)
{
	m_pBrief->m_ID = gid;
	m_pBrief->m_LeaderID = pRole->GetID();
	m_pBrief->m_LeaderName = pRole->GetName();
	m_pBrief->m_Level = 1;
	m_pBrief->m_capcity = CGuildConfig::Instance()->GetConfig(1, GuildNumber);
	m_pBrief->m_Name = gName;
	m_pBrief->m_openTime = GameTime::GetTime();
	if (MSConfig::Instance()->IsBackFlowServer())
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfigByLevel(MSConfig::Instance()->GetBackFlowLevel());
		if (pData)
		{
			// 潜规则: bossId = index + 1
			UINT32 index = pData->GuildBoss > 0 ?  (pData->GuildBoss - 1) : 0;
			m_pBrief->m_bossIndex = index;
		}
	}
	m_pBrief->SetChange();
}


int Guild::AddApplication(UINT64 roleId, UINT32 attr)
{
	if (attr < m_pBrief->m_RecruitPPT)
	{
		return KKSG::ERR_GUILD_PPT_REQ;
	}

	if (m_pBrief->m_NeedApproval == 0)
	{
		AddMember(roleId, GPOS_MEMBER);

		return KKSG::ERR_SUCCESS;
	}

	Get<GuildApplication>()->Add(roleId);
	SSInfo << "Guild::AddApplication role : " << roleId << "guildid" << GetID() << END;
	return KKSG::ERR_GUILD_WAITAPPROVAL;
}


void Guild::InitSerializer()
{
	Set<GuildBrief>(m_pBrief);
	Set<GuildApplication>(new GuildApplication(this));
	Set<GuildHistory>(new GuildHistory(this));
	Set<CGuildCheckin>(new CGuildCheckin());
	Set<GuildBonusMgr>(new GuildBonusMgr(this));
	Set<GuildGoblin>(new GuildGoblin(this));
	Set<CGuildCard>(new CGuildCard(this));
	Set<CGuildActivity>(new CGuildActivity(this));
	Set<CGuildWage>(new CGuildWage(this));
	Set<GuildGarden>(new GuildGarden(this));
	Set<CGuildCamp>(new CGuildCamp(this));
	Set<GuildDonate>(new GuildDonate(this));

	AddListener(Get<GuildHistory>());
	AddListener(Get<GuildBonusMgr>());
	AddListener(Get<CGuildActivity>());
	AddListener(Get<CGuildCard>());
	AddListener(Get<GuildDonate>());
}

void Guild::AddMember(UINT64 roleId, GuildPosition pos)
{
	// 已经加入
	if (m_pBrief->m_Members.find(roleId) != m_pBrief->m_Members.end())
	{
		SSWarn << "role : " << roleId << " has joined guild : " << GetID() <<END;
		return ;
	}
	CGuildMgr::Instance()->AddJoinInfo(roleId);
	NotifyPosition(roleId, pos);
	GuildMember mem;
	mem.qwRoleID = roleId;
	mem.position = pos;
	mem.contribute = 0;
	mem.updateTime = 0;
	mem.joinTime = time(NULL);
	m_pBrief->m_Members[roleId] = mem;
	m_pBrief->SetChange();
	CallBack(OnAddMember, this, &m_pBrief->m_Members[roleId]);
	SynSimpleDataToGs(ADD_MEMBER, roleId);

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (NULL != pRole)
	{
		_SynRoleInGuild(pRole);
		Get<CGuildCheckin>()->HintNotify(pRole);
		NoticeMgr::Instance()->NoticeGuildAddMember(GetID(), pRole->GetName());
		CReportDataMgr::Instance()->JoinGuild(pRole);
	}
	EventMgr::Instance()->AddEvent(roleId, ACH_EVENT_OFFSET+ACHIEVE_COM_TYPE_JOIN_GUILD, pos);
	EventMgr::Instance()->AddEventOnlyOnMS(roleId, MENTOR_EVENT_OFFSET+KKSG::MentorTask_JoinGuild, 0, 1);
	ResWarMgr::Instance()->OnJoinGuild(this->GetID(),pRole);

	// 加人tlog日志
	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	DoTxLog(oLog);
	oLog.m_uRoleID = roleId;
	oLog.m_iActType = TX_GUILD_ACT_TYPE_4;
	oLog.Do();

	TeamTransHandler trans;
	trans.SynTeamExtraInfoToTs(roleId);
}

void Guild::SetMemberPosition(GuildMember *pMember, GuildPosition pos)
{
	if (pos == GPOS_LEADER)
	{
		pMember->position = GPOS_LEADER;
		CRoleSummary *pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(pMember->qwRoleID);
		if (pRole != NULL)
		{
			m_pBrief->m_LeaderID = pMember->qwRoleID;
			m_pBrief->m_LeaderName = pRole->GetName();
			m_pBrief->SetChange();
			CallBack(OnMemberPosChanged, this, pMember);
		}
	}
	else
	{
		pMember->position = pos;
		CallBack(OnMemberPosChanged, this, pMember);
	}

	NotifyPosition(pMember->qwRoleID, pos);
}

void Guild::RecordBossDps(UINT64 roleId)
{
	GuildMember* pMember = GetMember(roleId);
	if (pMember != NULL)
	{
		//CallBack(OnMembergGuildBossDps, this, pMember);
	}
}

void Guild::RemoveMember(UINT64 roleId)
{
	auto iter = m_pBrief->m_Members.find(roleId);
	if (iter != m_pBrief->m_Members.end())
	{
		m_pBrief->m_Members.erase(iter);
	}

	ResWarMgr::Instance()->OnLeaveGuild(this->GetID(),roleId);
	CallBack(OnRemoveMember, this, roleId);

	if (roleId == m_pBrief->m_LeaderID)
	{
		GuildMember *pMember = NULL;

		foreach(i in m_pBrief->m_Members)
		{
			if (pMember == NULL || pMember->position > i->second.position)
			{
				pMember = &(i->second);
			}
		}

		if (pMember == NULL)
		{
			TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
			DoTxLog(oLog);
			oLog.m_iActType = TX_GUILD_ACT_TYPE_2;
			oLog.m_uRoleID = GetLeaderID();
			oLog.m_DismissMode = Tx_GuildDismiss_Self;
			oLog.Do();
			CGuildMgr::Instance()->AddDismissGuild(this);
		}
		else
		{
			SetMemberPosition(pMember, GPOS_LEADER);
			OnLeaderChange(roleId, pMember->qwRoleID);
		}
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole != NULL)
	{
		NotifyPosition(pRole->GetID(), GPOS_MAX);
	}

	SynSimpleDataToGs(DEL_MEMBER, roleId);

	///> 处理世界boss的工会排行
	WorldBossMgr::Instance()->OnRoleLeaveGuild(m_pBrief->m_ID, roleId);
	TeamTransHandler trans;
	trans.SynTeamExtraInfoToTs(roleId);
	m_pBrief->SetChange();
}

GuildMember* Guild::GetMember(UINT64 qwRoleID)
{
	auto iter = m_pBrief->m_Members.find(qwRoleID);
	if (iter != m_pBrief->m_Members.end())
	{
		return &(iter->second);
	}

	return NULL;
}

int Guild::SetMemberPos(UINT64 qwRoleID, GuildPosition pos)
{
	GuildMember *pMember = GetMember(qwRoleID);
	if (pMember)
	{
		if (pos == GPOS_LEADER)
		{
			GuildMember *pLeader = GetMember(m_pBrief->m_LeaderID);
			SetMemberPosition(pLeader, GPOS_MEMBER);
			SynSimpleDataToGs(SYN_POSTION, pLeader->qwRoleID);
			if (pLeader)
			{
				UINT64 oldLeaderId = pLeader->qwRoleID;
				OnLeaderChange(oldLeaderId, qwRoleID);
			}
		}
		else 
		{
			GuildConf *pConf = CGuildConfig::Instance()->GetGuildConf(m_pBrief->m_Level);
			if (pos == GPOS_VICELEADER)
			{
				if (CountPosition(GPOS_VICELEADER) >= pConf->VicePresidentNum)
				{
					return KKSG::ERR_GUILD_VICE_FULL;
				}
			}
			else if (pos == GPOS_OFFICER)
			{
				if (CountPosition(GPOS_OFFICER) >= pConf->OfficialNum)
				{
					return KKSG::ERR_GUILD_OFFICER_FULL;
				}
			}
			else if (pos == GPOS_ELITEMEMBER)
			{
				if(CountPosition(GPOS_ELITEMEMBER) >= pConf->EliteMemberNum)
				{
					return KKSG::ERR_GUILD_ELITE_FULL;
				}
			}
		}

		SetMemberPosition(pMember, pos);
		SynSimpleDataToGs(SYN_POSTION, qwRoleID);
		m_pBrief->SetChange();
	}

	return KKSG::ERR_SUCCESS;
}

bool Guild::IsPlayerInGuild(UINT64 roleId)
{
	return m_pBrief->m_Members.find(roleId) == m_pBrief->m_Members.end() ? false : true;
}

void Guild::ForeachMember(MemberCB cb, void *arg)
{
	foreach(i in m_pBrief->m_Members)
	{
		cb(&(i->second), arg);
	}
}

void Guild::SetAnnoucement(const std::string &s)
{
	m_pBrief->SetChange();
	TssSdkUicNameUserInputInfoV2 info;
	memset(&info, 0, sizeof(info));
	info.msg_ = (unsigned char*)s.c_str();
	info.msg_len_ = s.length();
	info.door_level_ = 1;
	info.if_replace_ = 1;
	if (0 == CTssHandler::Instance()->judge_user_input_name(&info))
	{
		m_pBrief->m_Annoucement = (char *)info.msg_;
	}
	else
	{
		m_pBrief->m_Annoucement = s;
	}
}

void Guild::SetIcon(int icon)
{
	m_pBrief->SetChange();
	m_pBrief->m_Icon = icon;
	SynSimpleDataToGs(SYN_COIN);
}

void Guild::SetRecruitPowerPoint(int ppt)
{
	m_pBrief->SetChange();
	m_pBrief->m_RecruitPPT = ppt;
}

void Guild::SetNeedApproval(int n)
{
	m_pBrief->SetChange();
	m_pBrief->m_NeedApproval = n;
}

UINT32 Guild::GetAllExp()
{
	UINT32 allExp = 0;
	for (UINT32 lvl = 1; lvl < m_pBrief->m_Level; ++lvl)
	{
		allExp += CGuildConfig::Instance()->GetConfig(lvl, GuildExp); 
	}

	return allExp + m_pBrief->m_exp;
}

void Guild::AddExp(UINT32 exp)
{
	m_pBrief->m_exp += exp;
	m_pBrief->m_lastExp += exp;
	m_pBrief->SetChange();

	bool isLvlUp = false;
	while (true)
	{
		if (m_pBrief->m_Level >= (int)CGuildConfig::Instance()->GetMaxLevel())
		{
			break;
		}
		UINT32 levelexp = CGuildConfig::Instance()->GetConfig(m_pBrief->m_Level, GuildExp);
		if (m_pBrief->m_exp < (int)levelexp)
		{
			break;
		}
		m_pBrief->m_exp  -= levelexp;

		LevelUp();
		isLvlUp = true;
	}
	CallBack(OnGuildAddExp, this);

	if (isLvlUp)
	{
		SynSimpleDataToGs(SYN_LEVEL);
	}

	UpdateGuildSkillDataToEveryone();

	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	DoTxLog(oLog);
	oLog.m_iActType = TX_GUILD_ACT_TYPE_7;
	oLog.m_iChangeValue = exp;
	oLog.Do();
}

void Guild::DelExp(UINT32 delExp)
{
	/*
	UINT32 allExp = GetAllExp();
	UINT32 oldLvl = m_pBrief->m_Level; 
	m_pBrief->m_Level = 1;
	m_pBrief->m_exp = 0;
	m_pBrief->m_lastExp = m_pBrief->m_lastExp >= delExp ? (m_pBrief->m_lastExp - delExp) : 0;
	allExp = allExp > delExp ? (allExp - delExp) : 0;

	m_pBrief->m_exp += allExp;
	m_pBrief->SetChange();

	while (true)
	{
		if (m_pBrief->m_Level >= (int)CGuildConfig::Instance()->GetMaxLevel())
		{
			break;
		}
		UINT32 levelexp = CGuildConfig::Instance()->GetConfig(m_pBrief->m_Level, GuildExp);
		if (m_pBrief->m_exp < (int)levelexp)
		{
			break;
		}
		m_pBrief->m_exp  -= levelexp;
		++m_pBrief->m_Level;
		m_pBrief->m_capcity = CGuildConfig::Instance()->GetConfig(m_pBrief->m_Level, GuildNumber);
		m_pBrief->SetChange();
	}
	*/

	m_pBrief->m_exp = m_pBrief->m_exp > delExp ? (m_pBrief->m_exp - delExp) : 0;
	m_pBrief->SetChange();
	CallBack(OnGuildAddExp, this);
	/*
	if (oldLvl != m_pBrief->m_Level)
	{
		SynSimpleDataToGs(SYN_LEVEL);
	}
	*/
	UpdateGuildSkillDataToEveryone();
}

void Guild::LevelUp()
{
	++m_pBrief->m_Level;
	m_pBrief->m_capcity = CGuildConfig::Instance()->GetConfig(m_pBrief->m_Level, GuildNumber);
	m_pBrief->SetChange();

	PtcM2C_LoginGuildInfo msg;
	for (auto iter = m_pBrief->m_Members.begin(); iter != m_pBrief->m_Members.end(); ++iter)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(iter->first);
		if (role == NULL)
		{
			continue;
		}

		msg.m_Data.set_gid(m_pBrief->m_ID);
		msg.m_Data.set_position(iter->second.position);
		msg.m_Data.set_level(m_pBrief->m_Level);
		msg.m_Data.set_name(m_pBrief->m_Name);
		msg.m_Data.set_icon(m_pBrief->m_Icon);
		role->Send(msg);
		CReportDataMgr::Instance()->GuildLevelUp(role, m_pBrief->m_Level);
	}

	CGuildBossMgr::Instance()->DoGuildOpenGuildBoss(GetID());
	CGuildConfig::Instance()->NoticeGuildSystemOpen(GetID(), GetLevel());
	CallBack(OnGuildLevelUp, this);

	//加人tlog日志;
	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	DoTxLog(oLog);
	oLog.m_iActType = TX_GUILD_ACT_TYPE_3;
	oLog.Do();
}

void Guild::Dismiss()
{
	CGuildTerritoryBattle::Instance()->DoGuildDismiss(GetID());
	foreach(i in m_pBrief->m_Members)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
		if (pRole != NULL)
		{
			NotifyPosition(pRole->GetID(), GPOS_MAX);
		}
		CallBack(OnRemoveMember, this, i->first);
	}

	m_pBrief->m_Members.clear();

	PtcM2D_SaveGuildData msg;
	msg.m_Data.set_gid(m_pBrief->m_ID);
	msg.m_Data.set_optype(GDBOP_DELETE);
	DBLink::Instance()->SendTo(msg);
	SynSimpleDataToGs(DEL_GUILD);

	CReportDataMgr::Instance()->DismissGuild(GetLeaderID());
	LogInfo("Dismiss Guild: %s", m_pBrief->m_Name.c_str());
}

void Guild::sendCmdToAllMember(const CProtocol &roPtc)
{
	foreach(i in m_pBrief->m_Members)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
		if (!pRole)
		{
			continue;
		}
		pRole->Send(roPtc);
	}
}

GuildMember * Guild::FindViceLeader()
{
	foreach(i in m_pBrief->m_Members)
	{
		if (i->second.position == GPOS_VICELEADER)
		{
			return &(i->second);
		}
	}

	return NULL;
}

void Guild::LoadFromDB(const KKSG::GuildAllDBData & guilddata)
{
	for (UINT32 i = 0; i < Length<GuildHandlerType>::value; ++i)
	{
		m_serializers[i]->Load(guilddata);
	}

	foreach(i in m_pBrief->m_Members)
	{
		CallBack(OnAddMemberFromDB, this, &(i->second));
	}

	LogDebug("Load Guild [%s]", m_pBrief->m_Name.c_str());
	LogDebug("server [%u]", GetServerId()); 
}

void Guild::InsertTODB()
{
	PtcM2D_SaveGuildData msg;
	msg.m_Data.set_gid(GetID());
	msg.m_Data.set_optype(GDBOP_CREATE);
	msg.m_Data.set_name(GetName());

	for (UINT32 i = 0; i < Length<GuildHandlerType>::value; ++i)
	{
		m_serializers[i]->Save(msg.m_Data);
	}

	DBLink::Instance()->SendTo(msg);
}

void Guild::SaveToDB(UINT32& num)
{
	bool hasChanged = false;
	PtcM2D_SaveGuildData msg;

	for (UINT32 i = 0; i < Length<GuildHandlerType>::value; ++i)
	{
		if (m_serializers[i]->IsChanged())
		{
			hasChanged = true;
			m_serializers[i]->Save(msg.m_Data);
		}
	}

	if (hasChanged)
	{
		++num;
		msg.m_Data.set_gid(m_pBrief->m_ID);
		msg.m_Data.set_optype(GDBOP_UPDATE);
		DBLink::Instance()->SendTo(msg);
	}
}

void Guild::NotifyPosition(UINT64 roleId, GuildPosition pos)
{
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)  // 没在线  不发通知协议
	{
		return;
	}

	PtcM2C_LoginGuildInfo msg;
	if (pos == GPOS_MAX) // 表示退出公会
	{
		msg.m_Data.set_gid(INVALID_GUILDID);
		msg.m_Data.set_position(GPOS_MAX);
		msg.m_Data.set_level(0);
	}
	else
	{
		msg.m_Data.set_gid(m_pBrief->m_ID);
		msg.m_Data.set_position(pos);
		msg.m_Data.set_level(m_pBrief->m_Level);
		msg.m_Data.set_name(m_pBrief->m_Name);
		msg.m_Data.set_icon(m_pBrief->m_Icon);
	}
	pRole->Send(msg);
}

bool Guild::GuildLevelCMP::operator()(Guild *pG1, Guild *pG2) const
{
	UINT32 lvlOne = pG1->m_pBrief->m_Level;
	UINT32 expOne = pG1->m_pBrief->m_exp; 
	UINT32 lvlTwo = pG2->m_pBrief->m_Level;
	UINT32 expTwo = pG2->m_pBrief->m_exp; 

	// 尝试从蛋蛋那里获取数据
	if (lvlOne != lvlTwo)
	{
		return lvlOne > lvlTwo;
	}
	else
	{
		return expOne > expTwo;
	}

	return false;
}

bool Guild::GuildMemberCountCMP::operator()(const Guild *pG1, const Guild *pG2) const
{
	if (pG1->m_pBrief->m_Members.size() != pG2->m_pBrief->m_Members.size())
	{
		return pG1->m_pBrief->m_Members.size() > pG2->m_pBrief->m_Members.size();
	}
	else
	{
		return pG1->m_pBrief->m_Level > pG2->m_pBrief->m_Level;
	}

	return false;
}

bool Guild::GuildNameCMP::operator()(const Guild *pG1, const Guild *pG2) const
{
	if (pG1->m_pBrief->m_Name != pG2->m_pBrief->m_Name)
	{
		return pG1->m_pBrief->m_Name < pG2->m_pBrief->m_Name;
	}

	return false;
}

bool Guild::GuildExpCMP::operator()(const Guild *pG1, const Guild *pG2) const
{
	if (pG1->m_pBrief->m_exp != pG2->m_pBrief->m_exp)
	{
		return pG1->m_pBrief->m_exp < pG2->m_pBrief->m_exp;
	}

	return false;
}

bool Guild::GuildPrestigeCMP::operator()(Guild *pG1, Guild *pG2) const
{
	UINT32 prestigeOne = pG1->m_pBrief->m_prestige;
	UINT32 prestigeTwo = pG2->m_pBrief->m_prestige; 
	// 尝试从蛋蛋那里获取数据
	KKSG::PunishData punishData;
	if (CPunishMgr::Instance()->GetPunishInfo(pG1->GetID(), PUNISH_USER_GUILD_RANK, punishData))
	{
		KKSG::IdipGuildInfo rankdata;
		if (punishData.has_rankdata() && rankdata.ParseFromString(punishData.rankdata()))
		{
			prestigeOne = rankdata.prestige();
		}
	}

	punishData.Clear();
	if (CPunishMgr::Instance()->GetPunishInfo(pG2->GetID(), PUNISH_USER_GUILD_RANK, punishData))
	{
		KKSG::IdipGuildInfo rankdata;
		if (punishData.has_rankdata() && rankdata.ParseFromString(punishData.rankdata()))
		{
			prestigeOne = rankdata.prestige();
		}
	}

	return prestigeOne > prestigeTwo;
}

void Guild::BroadCast(const CProtocol& ptc)
{
	for (auto i = m_pBrief->m_Members.begin(); i != m_pBrief->m_Members.end(); ++i)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(i->first);
		if (NULL != role)
		{
			role->Send(ptc);
		}
	}
}

void Guild::UpdateGuildSkillDataToEveryone()
{

	PtcM2C_SendGuildSkillInfo oGuildSkillData;

	const std::map<UINT32, UINT32>& studySkillMap = GetStudySkill();
	for (auto iter = studySkillMap.begin(); iter != studySkillMap.end(); ++iter)
	{
		KKSG::GuildSkillData* pData = oGuildSkillData.m_Data.add_skilllevel();
		pData->set_skillid(iter->first);
		pData->set_maxlvl(iter->second);
	}
	oGuildSkillData.m_Data.set_lastguildexp(GetLastExp());

	BroadCast(oGuildSkillData);
}

int Guild::GetOnLineNum()
{
	int num = 0;
	foreach(i in m_pBrief->m_Members)
	{
		if (CRoleManager::Instance()->IsOnline(i->first))
		{
			++num;
		}
	}

	return num;
}

void Guild::AddRoleContribute(UINT64 qwRoleID, int contribute)
{
	GuildMember *pMember = GetMember(qwRoleID);
	if (pMember)
	{
		pMember->contribute += contribute;
		pMember->updateTime = (UINT32)time(NULL);
		m_pBrief->SetChange();
	}
}

void Guild::SetRoleContribute(UINT64 qwRoleID, int contribute)
{
	GuildMember *pMember = GetMember(qwRoleID);
	if (pMember)
	{
		pMember->contribute = contribute;
		pMember->updateTime = (UINT32)time(NULL);
		m_pBrief->SetChange();
	}
}

int Guild::CountPosition(GuildPosition pos)
{
	int Count = 0;
	foreach(i in m_pBrief->m_Members)
	{
		if (i->second.position == pos)
		{
			++Count;
		}
	}
	return Count;
}

UINT32 Guild::GetSkillLvl(UINT32 skillId)
{
	auto iter = m_pBrief->m_studySkill.find(skillId);
	if (iter != m_pBrief->m_studySkill.end())
	{
		return iter->second;
	} 

	return 0;
}

void Guild::AddSkillLvl(UINT32 skillId)
{
	auto iter = m_pBrief->m_studySkill.find(skillId);
	if (iter != m_pBrief->m_studySkill.end())
	{
		iter->second++;
		m_pBrief->SetChange();
		NoticeMgr::Instance()->NoticeGuildSkillUp(CGuildSkillConfig::Instance()->GetSkillName(skillId, iter->second), iter->second, GetID());
	}
}

void Guild::AddDareTimes(UINT32 addNum)
{
	m_pBrief->m_dare += addNum;
	m_pBrief->SetChange();

	for (UINT32 i = m_pBrief->m_dare - addNum + 1; i <= m_pBrief->m_dare; ++i)  // 红点事件
	{
		if (CheckDareReward(i))
		{
			//NoticeDareRewardNtf();
			break;
		}
	}

	//  TLOG
	TGuildDareFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uGuildID = m_pBrief->m_ID;
	oLog.m_iGuildLevel = m_pBrief->m_Level;
	oLog.m_iDareNum = addNum;
	oLog.m_iAfterDareNum = m_pBrief->m_dare;
	oLog.Do();
}
GuildPosition Guild::GetPosition(UINT64 roleId)
{
	GuildMember* pGuildMember = GetMember(roleId);
	if (pGuildMember != NULL)
	{
		return pGuildMember->position;
	}

	return GPOS_MAX;
}

UINT32 Guild::canStudySkill(UINT64 roleId, UINT32 skillId)
{
	if (!CGuildConfig::Instance()->HasPermission(GetPosition(roleId), GPEM_STUDY_SKILL))
	{
		return ERR_GUILD_NO_PERMISSION;
	}

	UINT32 skillLvl = GetSkillLvl(skillId); 

	int needGuildLvl = CGuildSkillConfig::Instance()->needGuildLvl(skillId, skillLvl + 1);
	if (needGuildLvl == -1 || GetLevel() < needGuildLvl)
	{
		return GUILD_SKILL_STUDY_LEVEL_LIMIT; 
	}

	UINT32 needGuildExp = 0; 
	if (!CGuildSkillConfig::Instance()->needGuildExp(skillId, skillLvl + 1, needGuildExp) || GetLastExp() < (UINT32)needGuildExp)
	{
		return ERR_GUILD_EXP_LIMIT; 
	}

	return ERR_SUCCESS;
}

void Guild::doStudySkill(UINT32 skillId)
{
	UINT32 needExp = 0;
	if (!CGuildSkillConfig::Instance()->needGuildExp(skillId, GetSkillLvl(skillId) + 1, needExp))
	{
		return ;
	}

	if (GetLastExp() < needExp)
	{
		return ;
	}

	DecLastExp(needExp);
	AddSkillLvl(skillId);

}

void Guild::SetBossIndex(UINT32 bossIndex)
{
	if (bossIndex == (m_pBrief->m_bossIndex + 1)) // 公会boss只能一个一个往上玩 不能跳过 不能回退
	{
		++m_pBrief->m_bossIndex;
		m_pBrief->SetChange();
	}
}

void Guild::GMSetBossIndex(UINT32 bossIndex)
{
	m_pBrief->m_bossIndex = bossIndex;
	m_pBrief->SetChange();
}


void Guild::AddPrestige(UINT32 addNum)
{
	m_pBrief->m_prestige += addNum;
	m_pBrief->SetChange();
	CallBack(OnGuildAddPrestige, this);

	TGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	DoTxLog(oLog);
	oLog.m_iActType = TX_GUILD_ACT_TYPE_8;
	oLog.m_iChangeValue = addNum;
	oLog.Do();
}

void Guild::DelPrestige(UINT32 delNum)
{
	m_pBrief->m_prestige = m_pBrief->m_prestige > delNum ? (m_pBrief->m_prestige - delNum) : 0;
	m_pBrief->SetChange();
	CallBack(OnGuildAddPrestige, this);
}

void Guild::FindRuler(std::vector<UINT64>& ruleVec)
{
	foreach (i in m_pBrief->m_Members)
	{
		if (i->second.position == GPOS_VICELEADER || i->second.position == GPOS_OFFICER)
		{
			ruleVec.push_back(i->first);
		}
	}
}

void Guild::GetVicLeader(std::set<UINT64>& vicLeaderSet)
{
	vicLeaderSet.clear();
	foreach (i in m_pBrief->m_Members)
	{
		if (i->second.position == GPOS_VICELEADER)
		{
			vicLeaderSet.insert(i->first);
		}
	}
}


void Guild::ChangeLeaderName(UINT64 roleid,const std::string &name)
{
	if (roleid == m_pBrief->m_LeaderID)
	{
		m_pBrief->m_LeaderName = name;
		m_pBrief->SetChange();
	}
}

void Guild::SetLeader(UINT64 roleId) 
{ 
	if (roleId == GetLeaderID())
	{
		return ;
	}

	CRoleSummary* pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pRole == NULL)
	{
		return ;
	}

	GuildMember* pNewLeader = GetMember(roleId);
	if (pNewLeader == NULL)
	{
		return ;
	}

	GuildMember* pOldLeader = GetMember(GetLeaderID());
	if (pOldLeader != NULL)
	{
		pOldLeader->position = GPOS_MEMBER;
		SynSimpleDataToGs(SYN_POSTION, pOldLeader->qwRoleID);
	}

	pNewLeader->position = GPOS_LEADER;
	m_pBrief->m_LeaderID = roleId;
	m_pBrief->m_LeaderName = pRole->GetName();

	m_pBrief->SetChange();
	SynSimpleDataToGs(SYN_POSTION, roleId);
}



void Guild::GetSessions(std::vector<SessionType>& sessions)
{
	for (auto i = m_pBrief->m_Members.begin(); i != m_pBrief->m_Members.end(); ++i)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(i->first);
		if (NULL != role)
		{
			sessions.push_back(role->GetSessionID());
		}
	}
}


void Guild::FillGuildBrief(KKSG::MyGuild* ginfo)
{
	if(NULL == ginfo)
	{
		return ;
	}
	ginfo->set_gid(m_pBrief->m_ID);
	ginfo->set_name(m_pBrief->m_Name);
	ginfo->set_level(m_pBrief->m_Level);
	ginfo->set_icon(m_pBrief->m_Icon);
}

void Guild::DoRoleLogin(CRole *pRole)
{
	if (pRole == NULL)
	{
		SSWarn << "Guild::DoRoleLogin pRole is NULL" << END;
		return ;
	}

	_SendGuildInfo(pRole);
	_SendGuildSkillInfo(pRole);

	Get<GuildBonusMgr>()->onLogin(pRole);
	Get<CGuildCard>()->OnLogin(pRole);
	Get<CGuildWage>()->OnlineCheck(pRole);
}

void Guild::DoDailyWork()
{
	ClearDareData();
	SetDareLvl(GetLevel());

	Get<CGuildCard>()->Update();
	Get<GuildDonate>()->OnDayPass();
}

void Guild::DoWeekWork()
{
	Get<CGuildWage>()->DoCheckWork();
	_DelPrestigeWeekly();  // 公会威望周衰减
}

void Guild::_SendGuildInfo(CRole* pRole)
{
	if (pRole == NULL)
	{
		SSWarn << "Guild::_SendGuildInfo pRole is NULL" << END;
		return;
	}

	PtcM2C_LoginGuildInfo msg;
	msg.m_Data.set_gid(0);
	msg.m_Data.set_level(0);
	msg.m_Data.set_position(GPOS_MAX);
	msg.m_Data.set_name("");
	msg.m_Data.set_icon(0);

	GuildMember* pMember = GetMember(pRole->GetID());
	if (pMember != NULL)
	{
		msg.m_Data.set_gid(GetID());
		msg.m_Data.set_level(GetLevel());
		msg.m_Data.set_position(pMember->position);
		msg.m_Data.set_name(GetName());
		msg.m_Data.set_icon(GetIcon());
	}

	pRole->Send(msg);
	SSInfo << "Guild::_SendGuildInfo success and roleid : " << pRole->GetID() << END; 
}

void Guild::_SendGuildSkillInfo(CRole* pRole)
{
	if (pRole == NULL)
	{
		return ;
	}

	PtcM2C_SendGuildSkillInfo msg;
	const std::map<UINT32, UINT32>& studySkillMap = GetStudySkill();
	for (auto iter = studySkillMap.begin(); iter != studySkillMap.end(); ++iter)
	{
		KKSG::GuildSkillData* pData = msg.m_Data.add_skilllevel();
		pData->set_skillid(iter->first);
		pData->set_maxlvl(iter->second);
	}
	msg.m_Data.set_lastguildexp(GetLastExp());

	pRole->Send(msg);
}

void Guild::_SynRoleInGuild(CRole* pRole)
{
	if (pRole == NULL)
	{
		return ;
	}

	PtcM2G_SynRoleInGuild msg;
	msg.m_Data.set_roleid(pRole->GetID());
	pRole->SendMsgToGS(msg);
}

void Guild::GuildDataToGS(const OpGuildReqDataArg &roArg, KKSG::OpGuildReqDataRes& res)
{
	switch(roArg.optype())
	{
	case (KKSG::STUDY_SKILL):
		{
			if (roArg.has_skillid())
			{
				res.set_guildskilllvl(GetSkillLvl(roArg.skillid()));
			}
		}
		break;
	case (KKSG::GUILD_DARE_INFO):
		{
			KKSG::GuildDareData* pData = res.mutable_guilddaredata();
			pData->set_guildlv(GetLevel());
			pData->set_darenum(GetDareTimes());
			foreach (i in m_pBrief->m_Members)
			{
				CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(i->first);
				KKSG::RoleGuildContribute* pCon = pData->add_rolecontr();
				pCon->set_roleid(i->first);
				pCon->set_rolename(pRoleSummary->GetName());
				pCon->set_contribute(i->second.contribute);
				pCon->set_updatetime(i->second.updateTime);
			}
		}
		break;
	}
}

void Guild::SynSimpleDataToGs(SYN_TYPE type, UINT64 roleId)
{

	PtcM2G_SynGuildSimpleData msg;
	KKSG::GuildSimpleData* pSimpleData = msg.m_Data.add_datas();
	pSimpleData->set_guildid(GetID());

	switch (type)
	{
	case (ADD_MEMBER) :
		{
			GuildMember* pMember = GetMember(roleId);
			if (pMember == NULL)
			{
				return ;
			}
			pSimpleData->set_guildlvl(GetLevel());
			pSimpleData->set_guildname(GetName());
			pSimpleData->set_icon(GetIcon());
			KKSG::guildmemberSimple* pData = pSimpleData->add_addmembers();
			pData->set_roleid(pMember->qwRoleID);
			pData->set_position(pMember->position);

			break;
		}
	case (DEL_MEMBER) :
		{
			pSimpleData->add_delmembers(roleId);
			break;
		}
	case (DEL_GUILD) :
		{
			pSimpleData->set_isdel(true);
			break;
		}
	case (SYN_COIN) :
		{
			pSimpleData->set_icon(GetLeaderID());
			break;
		}
	case (SYN_LEVEL) :
		{
			pSimpleData->set_guildlvl(GetLevel());
			break;
		}
	case (SYN_NAME):
		{
			pSimpleData->set_guildname(GetName());
			break;
		}
	case (SYN_POSTION) :
		{
			GuildMember* pMember = GetMember(roleId);
			if (pMember == NULL)
			{
				return ;
			}
			KKSG::guildmemberSimple* pData = pSimpleData->add_addmembers();
			pData->set_roleid(pMember->qwRoleID);
			pData->set_position(pMember->position);
			break;
		}
	default:
		return ;
	}

	GSLink::Instance()->SendToAllLine(msg);
}

UINT32 Guild::GetJoinTime(UINT64 roleId)
{
	GuildMember* pMember = GetMember(roleId);
	if (pMember != NULL)
	{
		return pMember->joinTime;
	}
	return 0;
}

void Guild::_AddDesignation(UINT64 nRole,UINT32 nRank)
{
	UINT32 nType = 0;
	UINT32 nPosition = GetPosition(nRole);
	if (nPosition == GPOS_LEADER)
		nType = 1;
	else if(nPosition == GPOS_VICELEADER)
		nType = 2;
	else
		nType = 3;
	EventMgr::Instance()->AddEvent(nRole, DESIGNATION_COM_TYPE_GUILD_ACTIVITY_RANK, (nRank == 1 ? 0 : (nRank <= 10 ? 1 : 2) )*3 +nType);
}

void Guild::SendGuildRankReward(UINT32 rank)
{
	UINT32 nowTime = time(NULL);
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(6);
	if (pConf == NULL)
	{
		return ;
	}

	foreach (i in m_pBrief->m_Members)
	{
		if (nowTime - (GetJoinTime(i->first)) < GetGlobalConfig().GuildRankRewardKeepTime * 24 * 60 * 60) // 加入时间太晚 不发奖励
		{
			continue;
		}
		
		UINT32 type = 0;
		if (GetPosition(i->first) == GPOS_LEADER)
		{
			type = 1;
		}
		else if (GetPosition(i->first) == GPOS_VICELEADER || GetPosition(i->first) == GPOS_OFFICER)
		{
			type = 2;
		}
		else
		{
			type = 3;
		}

		std::map<UINT32, UINT32> guildReward;
		std::vector<ItemDesc> rewardVec;
		CGuildConfig::Instance()->GetGuildRankReward(rank, type, rewardVec, guildReward);
		if (type == 1) // 会长增加公会奖励
		{
			AddGuildReward(guildReward);
		}

		if (rewardVec.empty())
		{
			return ;
		}
		std::string content = MailConfig::Instance()->ReplaceString("0", ToString(rank), pConf->m_content);
		MailData maildata(pConf->m_title, content, rewardVec, MAIL_EXPIRE_TIME);
		maildata.SetReason(ItemFlow_GuildRankdAward);
		CMailMgr::Instance()->SendMail(i->first, maildata);
		_AddDesignation(i->first,rank);

	}
	NoticeMgr::Instance()->NoticeGuildRankReward(GetID(), rank);
}

void Guild::AddGuildReward(std::map<UINT32, UINT32>& guildRewardMap)
{
	foreach (i in guildRewardMap)
	{
		if (i->first == GUILD_EXP)
		{
			AddExp(i->second);
		}
		else if (i->first == GUILD_PRESTIGE)
		{
			AddPrestige(i->second);
		}
	}
}

void Guild::_DelPrestigeWeekly()
{
	UINT32 delPer = GetGlobalConfig().GuildPrestigeDle > 100 ? 100 : GetGlobalConfig().GuildPrestigeDle; 
	m_pBrief->m_prestige -= m_pBrief->m_prestige * delPer / 100; 
	Get<CGuildWage>()->SetPrestige(m_pBrief->m_prestige);
}

void Guild::DoTxLog(TGuildFlowLog& oLog)
{
	oLog.m_uGuildID = m_pBrief->m_ID;
	oLog.m_iGuildLevel = m_pBrief->m_Level;
	oLog.m_iGuildExp = m_pBrief->m_exp;
	oLog.m_iPrestige = m_pBrief->m_prestige;
	oLog.m_iMemberNum = GetMemberCount();
	oLog.m_CreateTime = m_pBrief->m_openTime;
	oLog.m_strName = m_pBrief->m_Name;
}


void Guild::SetMaxTerritoryLevel(UINT32 max_territory_level)
{
	if (max_territory_level > m_pBrief->max_territory_level_)
	{
		m_pBrief->max_territory_level_ = max_territory_level;
		m_pBrief->SetChange();
	}
}

UINT32 Guild::GetMaxTerritoryLevel()
{
	return m_pBrief->max_territory_level_;
}

void Guild::SetName(const std::string &name)
{
	m_pBrief->m_Name = name;
	m_pBrief->SetChange();
	PtcM2C_ModifyGuildNameNtf ntf;
	ntf.m_Data.set_name(name);
	sendCmdToAllMember(ntf);
	SynSimpleDataToGs(SYN_NAME);
}

void Guild::OnLeaderChange(UINT64 oldLeaderId, UINT64 newLeaderId)
{
	m_pBrief->m_bindInfo.DoUnlinkWhenLeaderChange(oldLeaderId, newLeaderId);
}

UINT32 Guild::GetServerId()
{
	return GetServerIDByGuildID(GetID());
}
void Guild::SendModifyNameMail(MailData &mail)
{
	for (auto it = GetGuildMember().begin(); it !=  GetGuildMember().end(); ++it)
		CMailMgr::Instance()->SendMail(it->first, mail);

}