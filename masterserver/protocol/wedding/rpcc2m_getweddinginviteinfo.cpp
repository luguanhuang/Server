#include "pch.h"
#include "wedding/rpcc2m_getweddinginviteinfo.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "marriage/weddingmgr.h"
#include "role/rolesummarymgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"

// generate by ProtoGen at date: 2017/7/17 16:36:23

RPC_SERVER_IMPLEMETION(RpcC2M_GetWeddingInviteInfo, GetWeddingInviteInfoArg, GetWeddingInviteInfoRes)

bool FillCanInviteRoleInfo(Wedding* pWed, UINT64 roleId, google::protobuf::RepeatedPtrField<KKSG::WeddingRoleBrief>* vec)
{
	if (pWed->CanEnter(roleId))
	{
		return false;
	}
	if (pWed->InApplyList(roleId))
	{
		return false;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleId);
	if (pRole == NULL)
	{
		return false;
	}
	CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
	if (pSu == NULL)
	{
		return false;
	}
	pSu->FillWeddingRoleInfo(*vec->Add());
	return true;
}

void RpcC2M_GetWeddingInviteInfo::OnCall(const GetWeddingInviteInfoArg &roArg, GetWeddingInviteInfoRes &roRes)
{
	CRole*  pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	Wedding* pWed = WeddingMgr::Instance()->GetWeddingByRoleId(pRole->GetID());
	if (pWed == NULL)
	{
		return;
	}
	roRes.set_permitstranger(pWed->StrangerCanSee());
	roRes.set_weddingid(pWed->GetId());

	Friend* pFriend = FriendMgr::Instance()->GetFriendInfo(pRole->GetID());
	if (pFriend)
	{
		for (auto it = pFriend->m_friends.begin(); it != pFriend->m_friends.end(); ++it)
		{
			UINT64 roleId = it->second.m_roleid;
			FillCanInviteRoleInfo(pWed, roleId, roRes.mutable_friends());
		}
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(pRole->GetID());
	if (pGuild)
	{
		const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			UINT64 roleId = it->second.qwRoleID;
			FillCanInviteRoleInfo(pWed, roleId, roRes.mutable_guildmembers());
		}
	}
	pWed->FillInviteInfo(pRole->GetID(), roRes);
}

void RpcC2M_GetWeddingInviteInfo::OnDelayReplyRpc(const GetWeddingInviteInfoArg &roArg, GetWeddingInviteInfoRes &roRes, const CUserData &roUserData)
{
}
