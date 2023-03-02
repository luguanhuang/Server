#include "pch.h"
#include "dragonguild.h"
#include "dragonguildmgr.h"
#include "network/dblink.h"
#include "network/gslink.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "util/gametime.h"
#include "dragonguild/ptcm2d_updatedragonguilddata.h"
#include "dragonguild/ptcm2g_syndragonguildsimpledata.h"
#include "dragonguild/ptcm2c_logindragonguildinfo.h"
#include "dragonguild/ptcm2c_dragonguildnamentf.h"
#include "dragonguild/ptcm2c_dragonguildsettingchanged.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "team/teamtranshandler.h"
#include "loghelper/tlogr.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "event/eventmgr.h"
#include "table/expeditionconfigmgr.h"
#include "foreach.h"


void DragonGuildBriefStruct::Init()
{
	m_id = 0;
	m_leaderId = 0;
	m_level = 1;
	//场景进度
	//场景通过次数
	m_exp = 0;
	m_rank = 0;
	m_createTime = 0;
	m_leaderName = "";
	m_name = "";

	//m_icon = -1;
	m_recuirtPPT = 0;
	m_needApproval = 1;
	m_mapId = 0;
	m_mapPassCnt = 0;
	m_firstPassTime = 0;

	m_announcement = "";
	m_totalPPT = 0;
}

bool DragonGuildBriefStruct::Load(const KKSG::DragonGuildBaseData& baseData)
{
	if (!baseData.has_brief() || baseData.brief().empty())
	{
		return false;
	}

	KKSG::DragonGuildBrief brief;
	if (!brief.ParseFromString(baseData.brief()))
	{
		return false;
	}

	m_id = brief.id();
	m_leaderId = brief.leaderid();
	m_level = brief.level();
	m_exp = brief.activity();
	m_rank = brief.rank();
	m_createTime = brief.createtime();
	m_leaderName = brief.leadername();
	m_name = brief.name();

	//m_icon = brief.icon();
	m_recuirtPPT = brief.recuirtppt();
	m_needApproval = brief.needapproval();
	m_mapId = brief.mapid();
	m_mapPassCnt = brief.mappasscnt();
	m_firstPassTime = brief.firstpasstime();
	m_announcement = brief.announcement();
	m_totalPPT = brief.totalppt();

	return true;
}

const std::string DragonGuildBriefStruct::ToBinaty()
{
	KKSG::DragonGuildBrief brief;
	brief.set_id(m_id);
	brief.set_leaderid(m_leaderId);
	brief.set_level(m_level);
	brief.set_activity(m_exp);
	brief.set_rank(m_rank);
	brief.set_createtime(m_createTime);
	brief.set_leadername(m_leaderName);
	brief.set_name(m_name);

	//brief.set_icon(m_icon);
	brief.set_recuirtppt(m_recuirtPPT);
	brief.set_needapproval(m_needApproval);
	brief.set_mapid(m_mapId);
	brief.set_mappasscnt(m_mapPassCnt);
	brief.set_firstpasstime(m_firstPassTime);

	brief.set_announcement(m_announcement);
	brief.set_totalppt(m_totalPPT);

	return brief.SerializeAsString();
}

///////////////////////////////////////////////////////

DragonGuild::DragonGuild():m_dirty(false)
#define DRAGONGUILD_MODULE(module)	,m_##module(this)
#include "dragonguildmodule.h"
#undef DRAGONGUILD_MODULE
{
	m_brief.Init();
}

DragonGuild::~DragonGuild()
{

}
bool DragonGuild::DragonGuildLevelCMP::operator()(DragonGuild *pG1, DragonGuild *pG2) const
{
	UINT32 lvlOne = (pG1->m_brief).m_level;
	INT32 expOne = (pG1->m_brief).m_exp; 
	UINT32 lvlTwo = (pG2->m_brief).m_level;
	INT32 expTwo = (pG2->m_brief).m_exp; 

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

bool DragonGuild::DragonGuildMembersCMP::operator ()(DragonGuild *pG1, DragonGuild *pG2) const
{
	if (pG1->m_members.size() != pG2->m_members.size())
	{
		return pG1->m_members.size() > pG2->m_members.size();
	}
	else
	{
		return pG1->GetLevel() > pG2->GetLevel();
	}

	return false;
}

bool DragonGuild::DragonGuildPPTCMP::operator ()(DragonGuild *pG1, DragonGuild *pG2) const
{
	if (pG1->GetTotalPPT() != pG2->GetTotalPPT())
	{
		return pG1->GetTotalPPT() > pG2->GetTotalPPT();
	}
	else
	{
		return pG1->GetLevel() > pG2->GetLevel();
	}

	return false;
}

bool DragonGuild::DragonGuildSceneID::operator()(DragonGuild *pG1, DragonGuild *pG2) const
{
	if (pG1->m_brief.m_mapId != pG2->m_brief.m_mapId)
	{
		return pG1->m_brief.m_mapId > pG2->m_brief.m_mapId;
	}
	else if (pG1->m_brief.m_mapPassCnt != pG2->m_brief.m_mapPassCnt)
	{
		return pG1->m_brief.m_mapPassCnt > pG2->m_brief.m_mapPassCnt;
	}
	else
	{
		return pG1->m_brief.m_firstPassTime < pG2->m_brief.m_firstPassTime;
	}
	return false;
}

bool DragonGuild::DragonGuildName::operator()(DragonGuild *pG1, DragonGuild *pG2) const
{
	if (pG1->GetName() != pG2->GetName())
	{
		return pG1->GetName() < pG2->GetName();
	}

	return false;
}

bool DragonGuild::DragonGuildLeaderName::operator()(DragonGuild *pG1, DragonGuild *pG2) const
{
	if (pG1->GetLeaderName() != pG2->GetLeaderName())
	{
		return pG1->GetLeaderName() < pG2->GetLeaderName();
	}

	return false;
}

void DragonGuild::FromKKSG(const KKSG::DragonGuildSaveData& data)
{
	KKSG::DragonGuildBaseData base;
	if (!base.ParseFromString(data.base()))
	{
		LogError("parse base error");
		return ;
	}

	m_brief.Load(base);

	m_members.clear();
	for (int i = 0; i < base.member_size(); ++ i)
	{
		UINT64 roleId = base.member(i).roleid();
		m_members[roleId].FromKKSG(base.member(i));
		DragonGuildMgr::Instance()->AddRoleDragonGuild(roleId, GetID());
	}
}

void DragonGuild::ToKKSG(KKSG::DragonGuildSaveData& data)
{
	data.set_id(GetID());

	KKSG::DragonGuildBaseData base;
	m_brief.Save(base);

	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		KKSG::DragonGuildMemberData* mem = base.add_member();
		it->second.ToKKSG(*mem);
	}
	base.SerializeToString(data.mutable_base());
}

void DragonGuild::OnLoaded()
{
	//CheckDissolve();
	m_DragonGuildShop.CheckRefresh();
}

void DragonGuild::LoadFromDB(const KKSG::DragonGuildSaveData& data)
{
	FromKKSG(data);

#define DRAGONGUILD_MODULE(module)	m_##module.LoadFromDB(data);
#include "dragonguildmodule.h"
#undef DRAGONGUILD_MODULE

	LogDebug("Load DragonGuild [%s]", m_brief.m_name.c_str());
}

void DragonGuild::OnDayPass()
{
#define DRAGONGUILD_MODULE(module)	m_##module.OnDayPass();
#include "dragonguildmodule.h"
#undef DRAGONGUILD_MODULE
}

void DragonGuild::OnWeekPass()
{

}

void DragonGuild::OnCreate(CRole* pRole, UINT64 ID, std::string name)
{
	// UpdateDegree();
	SetName(name);
	SetLeaderName(pRole->GetName());
	SetLeaderID(pRole->GetID());
	SetID(ID);

	AddMember(pRole->GetID(), DGPOS_LEADER);

	m_DragonGuildShop.RefreshItem();
}

void DragonGuild::AddToDB()
{
	PtcM2D_UpdateDragonGuildData ptc;
	ptc.m_Data.set_type(KKSG::DBOper_Insert);

	KKSG::DragonGuildSaveData& data = *ptc.m_Data.mutable_data();
	data.set_id(GetID());
	data.set_name(m_brief.m_name);

	ToKKSG(data);

#define DRAGONGUILD_MODULE(module)	m_##module.SaveToDB(data);
#include "dragonguildmodule.h"
#undef DRAGONGUILD_MODULE
	
	DBLink::Instance()->SendTo(ptc);
	m_dirty = false;
}

void DragonGuild::UpdateToDB()
{
	PtcM2D_UpdateDragonGuildData ptc;
	ptc.m_Data.set_type(KKSG::DBOper_Update);

	KKSG::DragonGuildSaveData& data = *ptc.m_Data.mutable_data();
	data.set_id(GetID());
	if (m_dirty)
	{
		ToKKSG(data);
		m_dirty = false;
	}
#define DRAGONGUILD_MODULE(module)	\
	if (m_##module.IsDirty())	\
	{	\
	m_##module.SaveToDB(data);	\
	m_##module.SetDirty(false);	\
	 }
#include "dragonguildmodule.h"
#undef DRAGONGUILD_MODULE
	DBLink::Instance()->SendTo(ptc);
}

void DragonGuild::DelToDB()
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		EventMgr::Instance()->AddEvent(it->first, DESIGNATION_COM_TYPE_DRAGONGUILD, 0, GlobalEvent::EVENT_OP_DEL, GetName());
	}

	PtcM2D_UpdateDragonGuildData ptc;
	ptc.m_Data.set_type(KKSG::DBOper_Del);
	KKSG::DragonGuildSaveData &data = *ptc.m_Data.mutable_data();
	data.set_id(GetID());
	DBLink::Instance()->SendTo(ptc);
}

void DragonGuild::ReplaceIdToDB()
{
	
}

void DragonGuild::SetDirty()
{
	if (m_dirty == false)
	{
		DragonGuildMgr::Instance()->AddToUpdateList(GetID());
	}
	m_dirty = true;
}

void DragonGuild::SetMember(std::vector<UINT64>& memberIDs)
{
	m_members.clear();
	for (size_t i = 0; i < memberIDs.size(); ++ i)
	{
		UINT64 roleId = memberIDs[i];
		m_members[roleId].roleId = roleId;
	}
}

void DragonGuild::GetDragonGuildMemberIds(std::vector<UINT64>& roleIds)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		roleIds.push_back(it->first);
	}
}

void DragonGuild::GetSessions(std::vector<SessionType>& sessions)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(it->first);
		if (pRole)
		{
			sessions.push_back(pRole->GetSessionID());
		}
	}
}

UINT32 DragonGuild::GetCapacity()
{
	DragonGuildConfigTable::RowData* config = DragonGuildTableMgr::Instance()->GetDragonGuildConfig(GetLevel());
	if (config == NULL)
	{
		LogError("GetDragonGuildConfig failed, level: %d", GetLevel());
		return 0;
	}
	return config->DragonGuildNumber;
}

void DragonGuild::FillAllMemberDetail(KKSG::DragonGuildDetailInfoRes& res)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it )
	{
		FillMemberDetail(it->second, *res.add_members());
	}
}

void DragonGuild::FillMemberDetail(DragonGuildMember& member, KKSG::DragonGuildMemberDetail& data)
{
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(member.roleId, true);
	if (su == NULL)
	{
		return;
	}

	data.set_memberid(member.roleId);
	data.set_profession((KKSG::RoleType)su->GetProfession());
	data.set_name(su->GetName());
	data.set_level(su->GetLevel());
	data.set_ppt(su->GetPowerPoint());
	su->SetOutLook(data.mutable_outlook());
	data.set_viplevel(su->GetVipLevel());
	data.set_paymemberid(su->GetPayMemberID());
	std::vector<UINT32> fashionIds;
	su->GetFashion(fashionIds);
	for (size_t i = 0; i < fashionIds.size(); ++i)
	{
		data.add_fashion(fashionIds[i]);
	}
}

void DragonGuild::BroadCast(const CProtocol& ptc)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(it->first);
		if (role)
		{
			role->Send(ptc);
		}
	}
}

void DragonGuild::BroadCastToGS(const CProtocol& ptc)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(it->first);
		if (role)
		{
			role->SendMsgToGS(ptc);
		}
	}
}

void DragonGuild::SendDragonGuildBaseInfo(CRole* pRole)
{
	if (pRole == NULL)	return;

	PtcM2C_LoginDragonGuildInfo msg;
	msg.m_Data.set_dgid(GetID());
	msg.m_Data.set_level(GetLevel());
	msg.m_Data.set_position((UINT32)GetPosition(pRole->GetID()));
	msg.m_Data.set_name(GetName());
	msg.m_Data.set_totalppt(GetTotalPPT());
	msg.m_Data.set_capacity(GetCapacity());
	msg.m_Data.set_membercount(m_members.size());
	msg.m_Data.set_mapid(m_brief.m_mapId);
	msg.m_Data.set_mapcnt(m_brief.m_mapPassCnt);
	msg.m_Data.set_firstpasstime(m_brief.m_firstPassTime);
	msg.m_Data.set_exp(m_brief.m_exp);
	pRole->Send(msg);
}

void DragonGuild::SendDragonGuildSettingInfo(CRole* pRole)
{
	if (pRole == NULL)	return;

	PtcM2C_DragonGuildSettingChanged ntf;
	ntf.m_Data.set_annoucement(GetAnnoucement());
	ntf.m_Data.set_recuitppt(GetRecuitPPT());
	ntf.m_Data.set_needapproval(GetNeedApproval());
	pRole->Send(ntf);
}

void DragonGuild::SendDragonGuildBaseInfoToAll()
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(it->first);
		if (pRole != NULL)
		{
			SendDragonGuildBaseInfo(pRole);
		}
	}
}

void DragonGuild::SendDragonGuildSettinfInfoToAll()
{
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(it->first);
		if (pRole != NULL)
		{
			SendDragonGuildSettingInfo(pRole);
		}
	}
}

DragonGuildMember* DragonGuild::GetMember(UINT64 roleId)
{
	auto it = m_members.find(roleId);
	if (it == m_members.end())
	{
		return NULL;
	}
	return &it->second;
}

KKSG::ErrorCode DragonGuild::DoLeaveMember(UINT64 roleId, bool force)
{
	DragonGuildMember* m = GetMember(roleId);
	if (m == NULL)
	{
		return KKSG::ERR_PARTNER_NO_PARTNER;
	}
	
	if (m->position == DGPOS_LEADER)
	{
		// 去解散
		return KKSG::ERR_INVALID_REQUEST;
	}

	ReduceTotalPPT(m->ppt);
	DragonGuildMgr::Instance()->OnDragonGuildSorterChanged(KKSG::DragongGuildSortByTotalPPT);
	EventMgr::Instance()->AddEvent(roleId, DESIGNATION_COM_TYPE_DRAGONGUILD, 0, GlobalEvent::EVENT_OP_DEL, GetName());
	RemoveMember(roleId);
	DragonGuildMgr::Instance()->OnDragonGuildSorterChanged(KKSG::DragonGuildSortByMemberCount);
	SendDragonGuildBaseInfoToAll();
	SynSimpleDataToGs(DEL_MEMBER, roleId);

	TeamTransHandler trans;
	trans.SynTeamExtraInfoToTs(roleId);

	SetDirty();
	LogInfo("[%llu] begin leave dragonguild [%llu]", roleId, GetID());

	TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_5;
	oLog.m_uRoleID = roleId;
	DoTxLog(oLog);
	oLog.Do();

	return KKSG::ERR_SUCCESS;
}

void DragonGuild::OnChangePPT(UINT64 roleId, UINT32 ppt)
{
	DragonGuildMember* m = GetMember(roleId);
	if (m == NULL)
	{
		return ;
	}

	UINT32 oldPPT = m->ppt;
	m->ppt = ppt;
	if (ppt > oldPPT)
	{
		AddTotalPPT(ppt - oldPPT);
	}
	else
	{
		ReduceTotalPPT(oldPPT - ppt);
	}

	SendDragonGuildBaseInfoToAll();
	SetDirty();
}

void DragonGuild::DoRoleLogin(CRole* pRole)
{
	if (pRole == NULL)
	{
		SSWarn << "pRole is NULL" << END;
		return ;
	}

	DragonGuildMember* m = GetMember(pRole->GetID());
	if (m != NULL)
	{
		SendDragonGuildBaseInfo(pRole);
		SendDragonGuildSettingInfo(pRole);
	}
}

void DragonGuild::RemoveMember(UINT64 roleId)
{
	DragonGuildMgr::Instance()->RemoveRoleDragonGuild(roleId);
	NotifyPosition(roleId, DGPOS_MAX);
	m_members.erase(roleId);
	SetDirty();
}

bool DragonGuild::IsFull()
{
	DragonGuildConfigTable::RowData* config = DragonGuildTableMgr::Instance()->GetDragonGuildConfig(GetLevel());
	if (config == NULL)
	{
		LogError("GetDragonGuildConfig failed, level: %d", GetLevel());
		return true;
	}
	if (m_members.size() >= config->DragonGuildNumber)
	{
		return true;
	}
	return false;
}

UINT32 DragonGuild::AddApplication(UINT64 roleId, UINT32 attr)
{
	if (attr < m_brief.m_recuirtPPT)
	{
		return KKSG::ERR_DG_PPT_REQ;
	}

	if(m_brief.m_needApproval == 0)
	{
		AddMember(roleId, DGPOS_MEMBER);
		return KKSG::ERR_SUCCESS;
	}

	m_DragonGuildApplication.Add(roleId);
	return KKSG::ERR_DG_WAITAPPROVAL;
}

void DragonGuild::SynSimpleDataToGs(SYN_TYPE type, UINT64 roleId /* = 0 */)
{
	PtcM2G_SynDragonGuildSimpleData msg;
	KKSG::dragonguildSimpleData* data = msg.m_Data.add_data();
	data->set_dragonguildid(GetID());

	switch (type)
	{
		case (ADD_MEMBER) :
			{
				DragonGuildMember* pMember = GetMember(roleId);
				if (NULL == pMember)
				{
					return ;
				}

				data->set_dragonguildlv(GetLevel());
				data->set_dragonguildname(GetName());
				KKSG::dragonguildSimpleRoleInfo* pRoleInfo = data->add_addmembers();
				pRoleInfo->set_roleid(pMember->roleId);
				pRoleInfo->set_position(pMember->position);
				break;
			}
		case (DEL_MEMBER) :
			{
				data->add_delmembers(roleId);
				break;
			}
		case (DEL_GUILD) :
			{
				data->set_isdel(true);
				break;
			}
		case (SYN_LEVEL) :
			{
				data->set_dragonguildlv(GetLevel());
				break;
			}
		case (SYN_NAME):
			{
				data->set_dragonguildname(GetName());
				break;
			}
		case (SYN_POSTION) :
			{
				DragonGuildMember* pMember = GetMember(roleId);
				if (pMember == NULL)
				{
					return ;
				}
				KKSG::dragonguildSimpleRoleInfo * pData = data->add_addmembers();
				pData->set_roleid(pMember->roleId);
				pData->set_position(pMember->position);
				break;
			}
		default:
			return ;
	}

	GSLink::Instance()->SendToAllLine(msg);
}

void DragonGuild::AddMember(UINT64 roleId, DragonGuildPosition pos)
{
	if (m_members.find(roleId) != m_members.end())
	{
		SSWarn << "role: " << roleId << " has joined dragonguild:　" << GetID() << END;
		return ;
	}

	DragonGuildMgr::Instance()->AddJoinInfo(roleId);
	EventMgr::Instance()->AddEvent(roleId, DESIGNATION_COM_TYPE_DRAGONGUILD, pos, GlobalEvent::EVENT_OP_ADD, GetName());

	CRoleSummary* pSum = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);

	DragonGuildMember mem;
	mem.roleId = roleId;
	mem.position = pos;
	mem.activity = 0;
	mem.joinTime = time(NULL);
	mem.ppt = pSum ? pSum->GetPowerPoint() : 0;
	mem.refreshShopLevel = pSum ? pSum->GetLevel(): 0;
	mem.openShopTime = 0;
	m_members[roleId] = mem;
	AddTotalPPT(mem.ppt);
	DragonGuildMgr::Instance()->OnDragonGuildSorterChanged(KKSG::DragongGuildSortByTotalPPT);
	DragonGuildMgr::Instance()->OnDragonGuildSorterChanged(KKSG::DragonGuildSortByMemberCount);
	SetDirty();
	SynSimpleDataToGs(ADD_MEMBER, roleId);

	NotifyPosition(roleId, pos);
	DragonGuildMgr::Instance()->AddRoleDragonGuild(roleId, GetID());

	SendDragonGuildBaseInfoToAll();
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole != NULL)
	{
		SendDragonGuildSettingInfo(pRole);
		//CReportDataMgr::Instance()->JoinGuild(pRole);
	}

	TeamTransHandler trans;
	trans.SynTeamExtraInfoToTs(roleId);

	TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	DoTxLog(oLog);
	oLog.m_uRoleID = roleId;
	oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_4;
	oLog.Do();
}

KKSG::ErrorCode DragonGuild::ChangeMemberPosition(DragonGuildMember* pMember, DragonGuildPosition pos)
{
	if (!pMember) return KKSG::ERR_INVALID_REQUEST;
	if (pos == DGPOS_LEADER)
	{
		UINT64 oldLeaderId = GetLeaderId();
		DragonGuildMember* m = GetMember(oldLeaderId);
		if (m)
		{
			pMember->position = pos;
			m->position = DGPOS_MEMBER;
			NotifyPosition(oldLeaderId, DGPOS_MEMBER);
			CRoleSummary *pRole = CRoleSummaryMgr::Instance()->GetRoleSummary(pMember->roleId);
			if (pRole != NULL)
			{
				m_brief.m_leaderId = pRole->GetID();
				m_brief.m_leaderName = pRole->GetName();
			}
			GetDragonGuildBindInfo().DoUnlinkWhenLeaderChange(oldLeaderId, pMember->roleId);
		}
		else
		{
			SSError << "Cannot find leader member, leaderId: " << oldLeaderId << END;
			return KKSG::ERR_INVALID_REQUEST;
		}
	}
	else if(pos == DGPOS_VICELEADER)
	{
		DragonGuildConfigTable::RowData* config = DragonGuildTableMgr::Instance()->GetDragonGuildConfig(GetLevel());
		if (config == NULL)
		{
			LogError("GetDragonGuildConfig failed, level: %d", GetLevel());
			return KKSG::ERR_INVALID_REQUEST;
		}
		if (GetViceLeaderCnt() >= config->VicePresidentNum)
		{
			return KKSG::ERR_DG_POSITION_FULL;
		}

		pMember->position = pos;
	}
	else
	{
		pMember->position = pos;
	}

	SetDirty();
	NotifyPosition(pMember->roleId, pos);
	SynSimpleDataToGs(SYN_POSTION, pMember->roleId);
	return KKSG::ERR_SUCCESS;
}

void DragonGuild::NotifyPosition(UINT64 roleId, DragonGuildPosition pos)
{
	CRole *pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)  // 没在线  不发通知协议
	{
		return;
	}

	PtcM2C_LoginDragonGuildInfo msg;
	if (pos == DGPOS_MAX)
	{
		msg.m_Data.set_dgid(0);
		msg.m_Data.set_position(DGPOS_MAX);
		msg.m_Data.set_level(0);
		msg.m_Data.set_name("");
		msg.m_Data.set_totalppt(0);
		msg.m_Data.set_capacity(0);
		msg.m_Data.set_membercount(0);
		msg.m_Data.set_mapid(0);
		msg.m_Data.set_mapcnt(0);
		msg.m_Data.set_firstpasstime(0);
		msg.m_Data.set_exp(0);
		pRole->Send(msg);
	}
	else
	{
		SendDragonGuildBaseInfo(pRole);
	}
}

KKSG::ErrorCode DragonGuild::DoMiss()
{
	foreach (i in m_members)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(i->first);
		if (pRole != NULL)
		{
			NotifyPosition(pRole->GetID(), DGPOS_MAX);

			TeamTransHandler trans;
			trans.SynTeamExtraInfoToTs(pRole->GetID());
		}
		DragonGuildMgr::Instance()->RemoveRoleDragonGuild(i->second.roleId);
	}

	// 发送邮件 
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_DragonGuildDismiss);
	if (pConf!=NULL)
	{
		char szBuf[64] = {0};
		string szContent = pConf->m_content;
		string szTitle   = pConf->m_title;
		string szMailContent = XCommon::ReplaceStr(szContent, "%s", GetName());
		std::vector<ItemDesc> item;
		MailData mail( pConf->m_title, szMailContent, item, MAIL_EXPIRE_TIME);	
		BroadCastMail(mail);
	}

	m_members.clear();

	PtcM2D_UpdateDragonGuildData msg;
	msg.m_Data.set_type(KKSG::DBOper_Del);
	KKSG::DragonGuildSaveData& data = *msg.m_Data.mutable_data();
	data.set_id(GetID());
	DBLink::Instance()->SendTo(msg);

	SynSimpleDataToGs(DEL_GUILD);
	LogInfo("Dismiss Guild: %s", GetName().c_str());

	TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_2;
	oLog.m_uRoleID = GetLeaderId();
	oLog.m_DismissMode = 1;
	oLog.Do();

	return KKSG::ERR_SUCCESS;
}

void DragonGuild::SetName(std::string name)
{
	m_brief.m_name = name;
	SetDirty();
	SynSimpleDataToGs(SYN_NAME);

	PtcM2C_DragonGuildNameNtf ntf;
	ntf.m_Data.set_name(name);
	BroadCast(ntf);

	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		EventMgr::Instance()->AddEvent(it->first, DESIGNATION_COM_TYPE_DRAGONGUILD, 0, GlobalEvent::EVENT_OP_MODIFY, GetName());
	}
}

void DragonGuild::ReduceTotalPPT(UINT32 ppt)
{
	 if (ppt > m_brief.m_totalPPT) 
	 {
		 m_brief.m_totalPPT = 0;
	 }
	 else 
	 {
		 m_brief.m_totalPPT -= ppt; 
	 }
}

UINT32 DragonGuild::GetViceLeaderCnt()
{
	INT32 viceleader = 0;
	for (auto it = m_members.begin(); it != m_members.end(); ++ it)
	{
		if (it->second.position == DGPOS_VICELEADER)
			++ viceleader;
	}
	return viceleader;
}

DragonGuildPosition DragonGuild::GetPosition(UINT64 roleId)
{
	auto it = m_members.find(roleId);
	if (it == m_members.end())
	{
		return DGPOS_MAX;
	}
	return it->second.position;
}

void DragonGuild::BroadCastMail(MailData &mail)
{
	for (auto it = GetAllMember().begin(); it !=  GetAllMember().end(); ++it)
		CMailMgr::Instance()->SendMail(it->first, mail);
}

void DragonGuild::AddPassSceneCount(UINT32 sceneID)
{
	//大师龙本不计入排行榜
	DragonNestTable::RowData * row = ExpeditionConfigMgr::Instance()->GetDnListConf(sceneID);
	if(row == NULL)
		return;
	if(row->DragonNestDifficulty == 0)
		return;
	if(m_brief.m_mapId > sceneID)
		return;
	else if(m_brief.m_mapId < sceneID)
	{
		m_brief.m_mapId = sceneID;
		m_brief.m_mapPassCnt = 1;
		m_brief.m_firstPassTime =GameTime::GetTime();
	}
	else if(m_brief.m_mapId == sceneID)
	{
		m_brief.m_mapPassCnt = m_brief.m_mapPassCnt + 1;
	}
	DragonGuildMgr::Instance()->OnDragonGuildSorterChanged(KKSG::DragonGuildSortBySceneID);
	SetDirty();
}

void DragonGuild::AddExp(UINT32 exp)
{
	if (exp <= 0)
	{
		return ;
	}

	m_brief.m_exp += exp;

	bool isLevelUp = false;
	while (true)
	{
		DragonGuildConfigTable::RowData* data = DragonGuildTableMgr::Instance()->GetDragonGuildConfig(m_brief.m_level);
		if (data == NULL || m_brief.m_exp < data->DragonGuildExpNeed)
		{
			break;
		}
		if (m_brief.m_level >= DragonGuildTableMgr::Instance()->GetDragonGuildMaxLevel())
		{
			if (m_brief.m_exp > data->DragonGuildExpNeed)
			{
				m_brief.m_exp = data->DragonGuildExpNeed;
			}
			break;
		}

		m_brief.m_exp -= data->DragonGuildExpNeed;
		m_brief.m_level ++;
		isLevelUp = true;
	}

	if (isLevelUp)
	{
		SetDirty();
		SynSimpleDataToGs(SYN_LEVEL);

		TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_3;
		oLog.m_iDGuildExp = m_brief.m_exp;
		DoTxLog(oLog);
		oLog.Do();
	}
	DragonGuildMgr::Instance()->OnDragonGuildSorterChanged(KKSG::DragonGuildSortByLevel);
	SendDragonGuildBaseInfoToAll();

	TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_6;
	oLog.m_iChangeValue = exp;
	DoTxLog(oLog);
	oLog.Do();
}

void DragonGuild::DoTxLog(TDragonGuildFlowLog& oLog)
{
	oLog.m_uDGuildID = m_brief.m_id;
	oLog.m_iDGuildLevel = m_brief.m_level;
	oLog.m_iDGuildExp = m_brief.m_exp;
	oLog.m_iMemberNum = m_members.size();
	oLog.m_CreateTime = m_brief.m_createTime;
	oLog.m_strName = m_brief.m_name;
}
