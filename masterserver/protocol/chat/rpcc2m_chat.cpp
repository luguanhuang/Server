#include "pch.h"
#include "chat/rpcc2m_chat.h"
#include "chat/rpcm2g_chatcansend.h"
#include "role/rolemanager.h"
#include "mail/mailmgr.h"
#include "timeutil.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "network/gslink.h"
#include "chat/chatmgr.h"
#include "guild/guildmgr.h"
#include "team/teammgr.h"
#include "friend/friendop.h"
#include "friend/friendconfig.h"
#include "account/punishmgr.h"
#include "common/ptcm2c_idippunishinfomsntf.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "notice/notice.h"
#include "chat/fmmgr.h"
#include "chat/ptcm2f_audiochanneltofm.h"
#include "groupchat/groupchatmgr.h"
#include "dragonguild/dragonguildmgr.h"

// generate by ProtoGen at date: 2016/7/25 15:26:06

RPC_SERVER_IMPLEMETION(RpcC2M_chat, ChatArg, ChatRes)

void RpcC2M_chat::OnCall(const ChatArg &roArg, ChatRes &roRes)
{
	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		return;
	}

	KKSG::ChatInfo chatinfo = roArg.chatinfo();
	// black list
	if (KKSG::PrivateChannel == roArg.chatinfo().channel())
	{
		if(chatinfo.dest().roleid_size() == 0 && chatinfo.destlist_size() == 0)
		{
			LogWarn("Invalid privatechat, no destid, roleid:%llu", poRole->GetID());
			return;
		}
		if(chatinfo.dest().roleid_size() != 0)
		{
			chatinfo.clear_destlist();
			chatinfo.add_destlist()->set_roleid(chatinfo.dest().roleid(0));
			chatinfo.clear_dest();
		}

		UINT64 destid = chatinfo.destlist(0).roleid();
		CRoleSummary* destRole = CRoleSummaryMgr::Instance()->GetRoleSummary(destid);
		if(NULL == destRole)
		{
			LogWarn("Invalid privatechat, can't find this destid:%llu, roleid:%llu", destid, poRole->GetID());
			return;
		}
		FriendOp op(poRole->GetID());
		op.SetOther(destid);
		if (op.IsInBlackList(destid))
		{
			roRes.set_errorcode(KKSG::ERR_CHAT_BLACK_INSELF);
			return;
		}
		if (op.IsInOtherBlackList(poRole->GetID()))
		{
			roRes.set_errorcode(KKSG::ERR_CHAT_BLACK_INOTHER);
			return;
		}
	}
	if (KKSG::AudioChannel == roArg.chatinfo().channel())
	{
		if (FMMgr::Instance()->GetAudioSize()==0)
		{
			roRes.set_errorcode(KKSG::ERR_FM_NOINAUDIO);
			return;
		}
		if(!FMMgr::Instance()->IsInRoom(poRole->GetID()))
		{
			roRes.set_errorcode(KKSG::ERR_FM_NOINROOM);
			return;
		}
	}


	//禁言了
	if (CPunishMgr::Instance()->CheckPunishAndNotify(poRole, poRole->GetID(), KKSG::PUNISH_USER_CHAT))
	{
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		return ;
	}	

	if (chatinfo.has_isrecruit() && chatinfo.isrecruit())
	{
		Guild* pGuild = CGuildMgr::Instance()->GetGuildByRoleId(poRole->GetID());
		if (pGuild == NULL)
		{
			SSWarn << " GUILD RECRUIT PGUILD IS NULL!" << END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return ;
		}
		if (!pGuild->isLeader(poRole->GetID()))
		{
			SSWarn << " GUILD RECRUIT ROLE IS NOT LEADER, ROLEID : " << poRole->GetID() << END;
			roRes.set_errorcode(KKSG::ERR_GUILD_NO_PERMISSION);
			return ;
		}

		UINT32 nowTime = time(NULL);
		if ((nowTime - pGuild->GetRecruitCDTime()) >= GetGlobalConfig().GuildRecruitTime * 60 * 60)
		{
			pGuild->SetRecruitCDTime(nowTime);
			Notice notice(GuildRecruit);
			notice.AddGuild(pGuild->GetID());
			notice.AddLink(pGuild);

			KKSG::ChatInfo& info = *notice.chatinfo;
			info.mutable_source()->set_roleid(poRole->GetID());
			info.set_time(TimeUtil::GetTime());

			CChatMgr::Instance()->AddChat(chatinfo.source().roleid(), *notice.chatinfo);
			CChatMgr::Instance()->SendWorld(*notice.chatinfo);
			roRes.set_errorcode(KKSG::ERR_SUCCESS);	

			SSInfo << " GUILD CHAT SUCCESS, ROLEID : " << poRole->GetID() << END;
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_CHAT_TIMELIMIT);
			roRes.set_cooldown(GetGlobalConfig().GuildRecruitTime * 60 * 60 - (nowTime - pGuild->GetRecruitCDTime()));
			SSInfo << " GUILD CHAT TIME OUT, ROLEID : " << poRole->GetID() << END;
		}
		return;
	}

	if (chatinfo.has_isdragonguildrecruit() && chatinfo.isdragonguildrecruit())
	{
		DragonGuild* pDragonGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(poRole->GetID());
		if (pDragonGuild == NULL)
		{
			SSWarn << "DragonGuild Recruit, DragonGuild is NULL!" << END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return ;
		}

		DragonGuildMember* m = pDragonGuild->GetMember(poRole->GetID());
		if (m == NULL)
		{
			SSWarn << "Cannot Get DragonGuileMember or member.Position is not leader " << END;
			roRes.set_errorcode(KKSG::ERR_DG_NOT_IN_GUILD);
			return ;
		}
		if (m->position != DGPOS_LEADER)
		{
			roRes.set_errorcode(KKSG::ERR_DG_NO_PERMISSION);
			return ;
		}

		UINT32 nowTime = GameTime::GetTime();
		if (nowTime - pDragonGuild->GetRecruitCDTime() >= GetGlobalConfig().DragonGuildRecruitTime * 60 * 60)
		{
			pDragonGuild->SetRecruitCDTime(nowTime);
			Notice notice(DragonGuileRecruitNotice);
			notice.AddDragonGuild(pDragonGuild->GetID());
			notice.AddLink(pDragonGuild);

			KKSG::ChatInfo& info = *notice.chatinfo;
			info.mutable_source()->set_roleid(poRole->GetID());
			info.set_time(GameTime::GetTime());

			CChatMgr::Instance()->AddChat(chatinfo.source().roleid(), *notice.chatinfo);
			CChatMgr::Instance()->SendWorld(*notice.chatinfo);
			roRes.set_errorcode(KKSG::ERR_SUCCESS);

			SSInfo << "DragonGuild Chat Success, RoleId: " << poRole->GetID() << END;
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_CHAT_TIMELIMIT);
			roRes.set_cooldown(GetGlobalConfig().DragonGuildRecruitTime * 60 * 60 - (nowTime - pDragonGuild->GetRecruitCDTime()));
			SSInfo << "DragonGuild Chat Time Limit, RoleId: " << poRole->GetID() << END;
		}		
		return ;
	}

	chatinfo.mutable_source()->set_roleid(poRole->GetID());
	chatinfo.set_time(TimeUtil::GetTime());

	if (KKSG::AudioChannel == roArg.chatinfo().channel())
	{
		CChatMgr::Instance()->FillSourceData(chatinfo.mutable_source());
	}

    if (KKSG::GroupChatChannel == roArg.chatinfo().channel())
	{
		UINT64 groupID = roArg.chatinfo().groupchatinfo().groupchatid();
		GroupChatGroupInfo* groupinfo = GroupChatMgr::Instance()->GetGroupInfo(groupID);
		if(NULL == groupinfo) 
		{
			SSWarn << " GroupChat GroupID " << groupID << " Error." << END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		if(!GroupChatMgr::Instance()->IsGroupMember(groupID, poRole->GetID()))
		{
			SSWarn << " GroupChat GroupID " << groupID << " has not roleid " << poRole->GetID() << END;
			roRes.set_errorcode(KKSG::ERR_FAILED);
			return;
		}
		groupinfo->FillProtoc(chatinfo.mutable_groupchatinfo());
    }

	RpcM2G_ChatCanSend* rpc = RpcM2G_ChatCanSend::CreateRpc();
	KKSG::ChatInfo* pInfo = m_oArg.mutable_chatinfo();
	pInfo->CopyFrom(chatinfo);
	rpc->m_oArg.set_rpcid(DelayRpc());
	KKSG::ChatInfo* pRpcChatInfo = rpc->m_oArg.mutable_chatinfo();
	pRpcChatInfo->CopyFrom(chatinfo);
	poRole->SendMsgToGS(*rpc);
}

void RpcC2M_chat::OnDelayReplyRpc(const ChatArg &roArg, ChatRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_errorcode(code);
	if(KKSG::ERR_CHAT_TIMELIMIT == code)
	{
		ChatCanSendRes* cansend = (ChatCanSendRes*)(roUserData.m_pUserPtr);
		if (cansend != NULL)
		{
			roRes.set_cooldown(cansend->cooldown());
		}
	}
}
