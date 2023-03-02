#include "pch.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "role/rpcc2m_changenamenew.h"
#include "role/rpcm2g_changenamegs.h"
#include "network/gslink.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "notice/noticemgr.h"
#include "chat/ptcm2t_chatforward.h"
#include "chat/chatmgr.h"
#include "friend/friendop.h"
#include "marriage/marriagemgr.h"

// generate by ProtoGen at date: 2016/10/10 17:39:23

RPC_SERVER_IMPLEMETION(RpcC2M_ChangeNameNew, ChangeNameArg, ChangeNameRes)

void RpcC2M_ChangeNameNew::OnCall(const ChangeNameArg &roArg, ChangeNameRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	///> 验证名字合法性	
	UINT32 errorcode = CRoleSummaryMgr::Instance()->IsNameInvalid(roArg.name());
	if (KKSG::ERR_SUCCESS != errorcode)
	{
		roRes.set_errorcode(KKSG::ErrorCode(errorcode));
		return;
	}
	CRoleSummaryMgr::Instance()->LockName(roArg.name());

	RpcM2G_ChangeNameGs* rpc = RpcM2G_ChangeNameGs::CreateRpc();
	rpc->m_oArg.set_delayrpcid(DelayRpc());
	rpc->m_oArg.set_name(roArg.name());
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_iscostitem(roArg.iscostitem());
	GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);
}

void RpcC2M_ChangeNameNew::OnDelayReplyRpc(const ChangeNameArg &roArg, ChangeNameRes &roRes, const CUserData &roUserData)
{
	UINT32 errorcode = roUserData.m_dwUserData;	
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		UINT64 roleid = *(UINT64*)roUserData.m_pUserPtr;
		CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
		if (NULL != role)
		{
			role->SetName(roArg.name());
		}
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);	
		if (NULL != summary)
		{
			std::string lastname = summary->GetName();
			CRoleSummaryMgr::Instance()->ChangeName(lastname, roArg.name());
			summary->SetName(roArg.name());
			MarriageMgr::Instance()->ChangeDesName(summary->GetID(), roArg.name());

			// notice friend
			FriendOp op(summary->GetID());
			const std::map<UINT64, FriendInfo>* friendlist = op.GetFriendList();
			if (NULL != friendlist)
			{
				for (auto i = friendlist->begin(); i != friendlist->end(); ++i)
				{
					PtcM2T_chatforward ptc;
					KKSG::ChatInfo* chatinfo = ptc.m_Data.mutable_chatinfo();
					// channel
					chatinfo->set_channel(KKSG::PrivateChannel);
					// source
					chatinfo->mutable_source()->set_roleid(summary->GetID());
					chatinfo->set_time(TimeUtil::GetTime());
					// dest
					KKSG::ChatSource* dest = chatinfo->add_destlist();
					dest->set_roleid(i->first);
					// info
					Notice notice(ChangeNameNotice1);
					notice.Replace("$C", lastname);
					notice.Replace("$C", roArg.name());
					chatinfo->set_info(notice.chatinfo->info());

					CChatMgr::Instance()->AddChat(summary->GetID(), *chatinfo);
					CChatMgr::Instance()->AddChat(i->first, *chatinfo);

					///> 向用户发送聊天消息
					CChatMgr::Instance()->SendUser(summary->GetID(), *chatinfo, true);
					CChatMgr::Instance()->SendUser(i->first, *chatinfo, true);
				}
			}

			// notice guild
			UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(summary->GetID());
			if (INVALID_GUILDID != guildid)
			{
				NoticeMgr::Instance()->NoticeNameChange(guildid, lastname, roArg.name());
				Guild *pGuild = CGuildMgr::Instance()->GetGuild(guildid);
				if (pGuild!=NULL)
				{
					pGuild->ChangeLeaderName(summary->GetID(),roArg.name());
				}

			}
		}
	}
	roRes.set_errorcode(KKSG::ErrorCode(errorcode));
	CRoleSummaryMgr::Instance()->UnlockName(roArg.name());
}
