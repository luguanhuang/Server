#include "pch.h"
#include "partner.h"
#include "partnermgr.h"
#include "db/rpcm2d_updatepartnerdata.h"
#include "network/dblink.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"
#include "tablemgr/partnertablemgr.h"
#include "util/gametime.h"
#include "role/rolesummarymgr.h"
#include "role/rolemanager.h"
#include "partner/ptcm2g_updatepartnertogs.h"
#include "partner/ptcm2c_updatepartnertoclient.h"
#include "chat/chatmgr.h"
#include "define/maildef.h"
#include "mail/mailmgr.h"
#include "notice/notice.h"

UINT32 PartnerMember::GetLeftLeaveTime()
{
	if (isApplyLeave == false)
	{
		return 0;
	}
	UINT32 now = GameTime::GetTime();
	UINT32 leaveTime = applyLeaveTime + GetGlobalConfig().PartnerLeaveTime;
	UINT32 leftTime = leaveTime > now ? (leaveTime - now) : 0;
	return leftTime;
}

Partner::Partner()
:m_dirty(false)
,m_id(0)
,m_degree(0)
,m_level(0)
,m_oldId(0)
#define PARTNER_MODULE(module)	,m_##module(this)
#include "partnermodule.h"
#undef PARTNER_MODULE
{
}

Partner::~Partner()
{

}

void Partner::FromKKSG(const KKSG::PartnerSaveData& data)
{
	KKSG::PartnerBaseData base;
	if (!base.ParseFromString(data.base()))
	{
		LogError("parse base error");
		return;
	}

	m_degree = base.degree();
	m_level = PartnerTableMgr::Instance()->GetLevel(m_degree);

	m_members.clear();
	for (int i = 0; i < base.member_size(); ++i)
	{
		UINT64 roleId = base.member(i).roleid();
		m_members[roleId].FromKKSG(base.member(i));
		PartnerMgr::Instance()->AddRolePartner(roleId, m_id);
	}
}

void Partner::ToKKSG(KKSG::PartnerSaveData& data)
{
	data.set_id(m_id);

	KKSG::PartnerBaseData base;
	base.set_degree(m_degree);

	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		KKSG::PartnerMemberData* mem = base.add_member();
		it->second.ToKKSG(*mem);
	}
	base.SerializeToString(data.mutable_base());
}

void Partner::OnLoaded()
{
	CheckDissolve();
	m_PartnerShop.CheckRefresh();
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		PartnerMember& m = it->second;
		if (m.isApplyLeave)
		{
			PartnerMgr::Instance()->AddToLeaveTimeList(m.roleId, m.applyLeaveTime + GetGlobalConfig().PartnerLeaveTime);
		}
	}
}

void Partner::LoadFromDB(const KKSG::PartnerSaveData& data)
{
	FromKKSG(data);
#define PARTNER_MODULE(module)	m_##module.LoadFromDB(data);
#include "partnermodule.h"
#undef PARTNER_MODULE
}

void Partner::OnDayPass()
{
	UpdateDegree();
#define PARTNER_MODULE(module)	m_##module.OnDayPass();
#include "partnermodule.h"
#undef PARTNER_MODULE
}

void Partner::OnCreate()
{
	UpdateDegree();
	m_PartnerShop.RefreshItem();
}

void Partner::AddToDB()
{
	RpcM2D_UpdatePartnerData* rpc = RpcM2D_UpdatePartnerData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Insert);
	KKSG::PartnerSaveData& data = *rpc->m_oArg.mutable_data();
	data.set_id(m_id);

	ToKKSG(data);
#define PARTNER_MODULE(module)	m_##module.SaveToDB(data);
#include "partnermodule.h"
#undef PARTNER_MODULE

	DBLink::Instance()->SendTo(*rpc);
	m_dirty = false;
}

void Partner::UpdateToDB()
{
	RpcM2D_UpdatePartnerData* rpc = RpcM2D_UpdatePartnerData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Update);
	KKSG::PartnerSaveData& data = *rpc->m_oArg.mutable_data();
	data.set_id(m_id);
	if (m_dirty)
	{
		ToKKSG(data);
		m_dirty = false;
	}
#define PARTNER_MODULE(module)	\
	if (m_##module.IsDirty())	\
	{	\
		m_##module.SaveToDB(data);	\
		m_##module.SetDirty(false);	\
	}
#include "partnermodule.h"
#undef PARTNER_MODULE
	DBLink::Instance()->SendTo(*rpc);
}

void Partner::DelToDB()
{
	RpcM2D_UpdatePartnerData* rpc = RpcM2D_UpdatePartnerData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_Del);
	rpc->m_oArg.mutable_data()->set_id(m_id);
	DBLink::Instance()->SendTo(*rpc);
}

void Partner::ReplaceIdToDB()
{
	if (m_oldId == 0)
	{
		return;
	}
	RpcM2D_UpdatePartnerData* rpc = RpcM2D_UpdatePartnerData::CreateRpc();
	rpc->m_oArg.set_type(KKSG::DBOper_ReplaceId);
	rpc->m_oArg.mutable_data()->set_id(m_id);
	rpc->m_oArg.set_oldid(m_oldId);
	DBLink::Instance()->SendTo(*rpc);
}

void Partner::SetDirty()
{
	if (m_dirty == false)
	{
		PartnerMgr::Instance()->AddToUpdateList(m_id);
	}
	m_dirty = true;
}

void Partner::SetMember(std::vector<UINT64>& memberIDs)
{
	m_members.clear();
	for (size_t i = 0; i < memberIDs.size(); ++i)
	{
		UINT64 roleId = memberIDs[i];
		m_members[roleId].roleId = roleId;
	}
}

void Partner::UpdateDegree()
{
	UINT32 total = 0;
	std::vector<UINT64> roleIds;
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		roleIds.push_back(it->first);
	}

	for (UINT32 i = 0; i < roleIds.size(); ++i)
	{
		UINT64 roleId1 = roleIds[i];
		Friend* f = FriendMgr::Instance()->GetFriendInfo(roleId1);
		if (f == NULL)
		{
			continue;
		}
		for (size_t j = i + 1; j  < roleIds.size(); ++j)
		{
			UINT64 roleId2 = roleIds[j];
			total += f->GetDegree(roleId2);
		}
	}

	m_degree = total;
	m_level = PartnerTableMgr::Instance()->GetLevel(m_degree);
	SetDirty();
}

void Partner::AddFriendDegree(UINT32 value)
{
	m_degree += value;
	UINT32 oldLevel = m_level;
	m_level = PartnerTableMgr::Instance()->GetLevel(m_degree);

	// 通知客户端战友等级变化
	if (oldLevel != m_level)
	{
		PtcM2C_UpdatePartnerToClient ntf;	
		ntf.m_Data.set_type(KKSG::PUType_Normal);
		ntf.m_Data.set_level(m_level);
		ntf.m_Data.set_degree(m_degree);
		ntf.m_Data.set_partid(m_id);
		BroadCast(ntf);
		Notice notice(PartnerLevelUpNotice);
		notice.Add(m_level);
		notice.SendPartner(m_id);
	}
	SetDirty();
}

void Partner::GetPartnerMemberIds(std::vector<UINT64>& roleIds)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		roleIds.push_back(it->first);
	}
}

void Partner::GetSessions(std::vector<SessionType>& sessions)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(it->first);
		if (pRole != NULL)
		{
			sessions.push_back(pRole->GetSessionID());
		}
	}
}

void Partner::FillAllMemberDetail(KKSG::GetPartnerDetailInfoRes& res)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		FillMemberDetail(it->second, *res.add_members());
	}
}

void Partner::FillMemberDetail(PartnerMember& member, KKSG::PartnerMemberDetail& data)
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
	data.set_is_apply_leave(member.isApplyLeave);
	data.set_left_leave_time(member.GetLeftLeaveTime());
	std::vector<UINT32> fashionIds;
	su->GetFashion(fashionIds);
	for (size_t i = 0; i < fashionIds.size(); ++i)
	{
		data.add_fashion(fashionIds[i]);
	}
}

void Partner::BroadCast(const CProtocol& ptc)
{
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(it->first);
		if (role)
		{
			role->Send(ptc);
		}
	}
}

void Partner::BroadCastToGS(const CProtocol& ptc)
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

PartnerMember* Partner::GetMember(UINT64 roleId)
{
	auto it = m_members.find(roleId);
	if (it == m_members.end())
	{
		return NULL;
	}
	return &it->second;
}

KKSG::ErrorCode Partner::LeaveMember(CRole* pRole)
{
	PartnerMember* m = GetMember(pRole->GetID());
	if (m == NULL)
	{
		return KKSG::ERR_PARTNER_NO_PARTNER;
	}
	if (m->isApplyLeave)
	{
		return KKSG::ERR_PARTNER_ALREADY_APPLY_LEAVE;
	}
	m->isApplyLeave = true;
	m->applyLeaveTime = GameTime::GetTime();
	PartnerMgr::Instance()->AddToLeaveTimeList(pRole->GetID(), m->applyLeaveTime + GetGlobalConfig().PartnerLeaveTime);

	PtcM2G_UpdatePartnerToGS ntf;
	ntf.m_Data.set_type(KKSG::POT_ApplyLeave);
	ntf.m_Data.set_apply_leave_time(m->applyLeaveTime);
	pRole->SendMsgToGS(ntf);

	SetDirty();
	LogInfo("[%llu] begin leave partner", pRole->GetID());
	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode Partner::CancelLeaveMember(CRole* pRole)
{
	PartnerMember* m = GetMember(pRole->GetID());
	if (m == NULL)
	{
		return KKSG::ERR_PARTNER_NO_PARTNER;
	}
	if (m->isApplyLeave == false)
	{
		return KKSG::ERR_PARTNER_NOT_APPLY_LEAVE;
	}
	m->isApplyLeave = false;
	m->applyLeaveTime = 0;
	PartnerMgr::Instance()->RemoveFromLeaveTimeList(pRole->GetID());

	PtcM2G_UpdatePartnerToGS ntf;
	ntf.m_Data.set_type(KKSG::POT_CancelLeave);
	pRole->SendMsgToGS(ntf);

	SetDirty();
	LogInfo("[%llu] cancel leave partner[%llu]", pRole->GetID(), m_id);
	return KKSG::ERR_SUCCESS;
}

void Partner::DoLeaveMember(UINT64 roleId, bool force)
{
	PartnerMember* mem = GetMember(roleId);
	if (mem == NULL)
	{
		return;
	}
	if (!force && !mem->isApplyLeave)
	{
		return;
	}
	PartnerMgr::Instance()->RemoveRolePartner(roleId);
	RemoveMember(roleId);

	// 更新战友度
	UpdateDegree();

	// 通知队友和自己离开
	PtcM2C_UpdatePartnerToClient ntf;	
	ntf.m_Data.set_type(KKSG::PUType_Leave);
	ntf.m_Data.set_leave_id(roleId);
	CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (su)
	{
		ntf.m_Data.set_leave_name(su->GetName());
	}
	ntf.m_Data.set_level(GetLevel());
	ntf.m_Data.set_degree(GetDegree());
	ntf.m_Data.set_partid(GetID());
	BroadCast(ntf);

	// 通知离开GS记录已经离开
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole)
	{
		// 通知自己离开
		pRole->Send(ntf);

		if (!force)
		{
			PtcM2G_UpdatePartnerToGS ntfGs;
			ntfGs.m_sessionID = pRole->GetSessionID();
			ntfGs.m_Data.set_type(KKSG::POT_Leave);
			pRole->SendMsgToGS(ntfGs);
		}
	}

	// notice 自己
	Notice notice(PartnerLeaveNotice);
	notice.SendRole(roleId);

	// notice 其他战友
	Notice noticeOther(PartnerOtherLeaveNotice);
	std::vector<UINT64> memberIds;
	GetPartnerMemberIds(memberIds);
	if (su)
	{
		noticeOther.AddRole(su);
	}
	noticeOther.SendPlays(memberIds);

	// 检查是否解散
	CheckDissolve();

	LogInfo("[%llu] leave partner[%llu]", roleId, m_id);
}

void Partner::CheckDissolve()
{
	// 检查是否解散
	if (m_members.size() > 1)
	{
		return;
	}
	PtcM2C_UpdatePartnerToClient ntf;	
	ntf.m_Data.set_type(KKSG::PUType_Dissolve);
	PtcM2G_UpdatePartnerToGS ntfGs;
	ntfGs.m_Data.set_type(KKSG::POT_Dissolve);

	Notice notice(PartnerDissolveNotice);
	std::vector<UINT64> roleIds;
	for (auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		UINT64 roleId = it->first;
		roleIds.push_back(roleId);
		PartnerMgr::Instance()->RemoveRolePartner(roleId);
		// 发送邮件
		CMailMgr::Instance()->SendMail(roleId, MailConf_PartnerDissolve);
		// 推送在线玩家解散
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
		if (pRole == NULL)
		{
			continue;
		}
		pRole->Send(ntf);
		pRole->SendMsgToGS(ntfGs);
	}
	notice.SendPlays(roleIds);

	m_members.clear();

	PartnerMgr::Instance()->AddToDeleteList(m_id);	
	CChatMgr::Instance()->RemovePartnerChat(m_id);
	LogInfo("[%llu] partner dissovle", m_id);
}

void Partner::RemoveMember(UINT64 roleId)
{
	m_members.erase(roleId);
	SetDirty();
}