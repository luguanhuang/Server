#include "pch.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "account/accountkick.h"
#include "friend/friendop.h"
#include "friend/friend.h"
#include "notice/notice.h"
#include "notice/noticemgr.h"
#include "define/noticedef.h"
#include "chat/chatmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "table/ProfessionMgr.h"
#include "chat/ptcm2t_chatforward.h"
#include "role/rpcg2m_changeprofessionms.h"

// generate by ProtoGen at date: 2017/1/12 15:22:37

RPC_SERVER_IMPLEMETION(RpcG2M_ChangeProfessionMs, ChangeProfessionMsArg, ChangeProfessionMsRes)

void RpcG2M_ChangeProfessionMs::OnCall(const ChangeProfessionMsArg &roArg, ChangeProfessionMsRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if (NULL == role)
	{
		SSWarn<<"role is NULL, roleid:"<<roArg.roleid()<<END;
		return;
	}
	CAccountKicker::Instance()->KickAccount(role->GetID(), KKSG::KICK_CHANGE_PROFESSION, NULL);


	std::string proname = CProfessionMgr::Instance()->GetProfessionName(roArg.pro());
	// notice friend
	FriendOp op(role->GetID());
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
			chatinfo->mutable_source()->set_roleid(role->GetID());
			chatinfo->set_time(TimeUtil::GetTime());
			// dest
			KKSG::ChatSource* dest = chatinfo->add_destlist();
			dest->set_roleid(i->first);
			// info
			Notice notice(ChangeProNotice1);
			notice.Replace("$C", role->GetName());
			notice.Replace("$C", proname);
			chatinfo->set_info(notice.chatinfo->info());

			CChatMgr::Instance()->AddChat(role->GetID(), *chatinfo);
			CChatMgr::Instance()->AddChat(i->first, *chatinfo);

			///> 向用户发送聊天消息
			CChatMgr::Instance()->SendUser(role->GetID(), *chatinfo, true);
			CChatMgr::Instance()->SendUser(i->first, *chatinfo, true);
		}
	}

	// notice guild
	UINT64 guildid = CGuildMgr::Instance()->GetGuildIdByRoleId(role->GetID());
	if (INVALID_GUILDID != guildid)
	{
		NoticeMgr::Instance()->NoticeProChange(guildid, role->GetName(), proname); 
	}

}

void RpcG2M_ChangeProfessionMs::OnDelayReplyRpc(const ChangeProfessionMsArg &roArg, ChangeProfessionMsRes &roRes, const CUserData &roUserData)
{
}
