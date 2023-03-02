#include "pch.h"
#include "chathandler.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gatelink.h"
#include "chatcondition.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "team.h"
#include "teammgr.h"
#include "teammember.h"
#include "payv2Record.h"
#include "guild/guildchat.h"
#include "time.h"
#include "friendconfig.h"
#include "pb/enum.pb.h"
#include "tsssdk/tss_sdk_uic.h"
#include "tsssdk/TssHandler.h"
#include "pb/enum.pb.h"
///>#include "audioChatMgr.h"
#include "designationMgr.h"
#include "rolesummary.h"
#include "rolesummarymgr.h"
//#include <typeinfo>

UINT32 ChatHandler::CheckChatInfo(ChatConditionParam& param, const KKSG::ChatInfo& chatinfo)
{	
	for (auto i = m_chatconditions.begin(); i != m_chatconditions.end(); ++i)
	{
		if (NULL == (*i))
		{
			continue;
		}
		UINT32 errorcode = (*i)->Check(param);
		if (0 != errorcode)
		{
			return errorcode;
		}
	}
	return 0;
}

UINT32 ChatHandler::CheckChatInfo(ChatConditionParam& param)
{
	//typeid(x) == typeid(Y);
	for (auto i = m_chatconditions.begin(); i != m_chatconditions.end(); ++i)
	{
		if (NULL == (*i))
		{
			continue;
		}
		UINT32 errorcode = (*i)->Check(param);
		if (0 != errorcode)
		{
			return errorcode;
		}
	}
	return 0;
}


/*void ChatHandler::PackChatInfo(Role* role, UINT32 channel, const KKSG::ChatInfo& chatinfo, const std::vector<UINT64>* destroleids)
{
	m_chatforward.m_Data.Clear();
	KKSG::ChatInfo* forwardchatinfo = m_chatforward.m_Data.mutable_chatinfo();
	forwardchatinfo->CopyFrom(chatinfo);

	forwardchatinfo->set_time((UINT32)time(NULL));

	KKSG::ChatSource* chatsource = forwardchatinfo->mutable_source();
	chatsource->set_roleid(role->GetID());
	chatsource->set_profession(role->GetProfession());
	chatsource->set_name(role->GetName());
	chatsource->set_powerpoint((UINT32)role->GetAttr(TOTAL_POWERPOINT));
	CPayV2Record* pay = role->Get<CPayV2Record>();
	chatsource->set_viplevel(pay->GetVipLevel());

	for( int i=0; i<forwardchatinfo->destlist_size(); i++ )
	{
		RoleSummary* sum = RoleSummaryMgr::Instance()->FindRoleSummary( chatinfo.destlist(i).roleid() );
		if( NULL == sum )
		{
			continue; 
		}
		forwardchatinfo->mutable_destlist(i)->set_profession(sum->dwProfession);
		forwardchatinfo->mutable_destlist(i)->set_powerpoint(sum->dwPowerPoint);
		forwardchatinfo->mutable_destlist(i)->set_viplevel(sum->viplevel);
	}

	if (chatinfo.has_issystem() && chatinfo.issystem())
	{
		return ;
	}
	//时间聊天内容需要发到腾讯安全sdk替换敏感字;
	switch (channel)
	{
	case KKSG::WorldChannel:
	case KKSG::GuildChannel:
	case KKSG::TeamChannel:
	case KKSG::PrivateChannel:
		{
			UINT32 uChatID = role->GetChatID();
			if (channel == KKSG::PrivateChannel)
			{
				role->SetDestChatList(uChatID, *destroleids);
			}
			role->SetChatForWard(&m_chatforward);
			TssHandler::Instance()->judge_user_input_chat(role, CONTENT_CATEGORY_CHAT, channel, 1, forwardchatinfo->info().c_str(), uChatID);
			break;
		}
	default:
		break;
	}
}*/

void ChatHandler::PushCondition(ChatCondition* condition)
{
	m_chatconditions.push_back(condition);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void ChatWorldHandler::FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids)
{
	//role->ChatWorldOper(&m_chatforward);
	//GatetLink::Instance()->BroadCast(m_chatforward);
	//AudioChatMgr::Instance()->AddAudioChatInfo(KKSG::WorldChannel, 0, role->GetID(), role->GetName(), *m_chatforward.m_Data.mutable_chatinfo());
}

void ChatGuildHandler::FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids)
{
	//role->ChatGuildOper(&m_chatforward);
	#if GUILD_GS
	Guild* guild = GuildMgr::Instance()->FindGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		return;
	}	
#endif
	/*
	UINT64 roleid = role->GetID();
	Guild* guild = GuildMgr::Instance()->FindGuildByRoleId(role->GetID());
	if (guild == NULL)
	{
		return;
	}	
	const std::vector<GuildMember>& members = guild->GetGuildMember();
	for (auto i = members.begin(); i != members.end(); ++i)
	{
		Role* tmprole = RoleManager::Instance()->FindByRoleID(i->qwRoleID);
		if (NULL == tmprole)
		{
			continue;
		}

		if( tmprole->GetID() == roleid )
		{
			SSInfo << "current role is the sender, roleid:" << roleid << END; 
		}

		m_chatforward.m_sessionID = tmprole->GetSessionID();
		GatetLink::Instance()->SendToSession(m_chatforward);
		//UINT64 curRoleID = role->GetID();
		//SSInfo << "send guild chat to client, roleid:" << curRoleID << ", content:" << m_chatforward.m_Data.chatinfo().info() << END;
	}
	*/
	//CGuildChat* guildchat = guild->Get<CGuildChat>();
	//guildchat->Push(m_chatforward.m_Data.chatinfo());
	//AudioChatMgr::Instance()->AddAudioChatInfo(KKSG::GuildChannel, guild->GetID(), role->GetID(), role->GetName(), *m_chatforward.m_Data.mutable_chatinfo());
}

void ChatPrivateHandler::FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids)
{
	if (NULL == destroleids || 0 == destroleids->size())
	{
		return;
	}
	for (auto i = destroleids->begin(); i != destroleids->end(); ++i)
	{
		Role* destrole = RoleManager::Instance()->FindByRoleID(*i);
		if (NULL == destrole || destrole->GetFlag(RSF_ISReconnecting))
		{
			// off line
/*			ChatOfflineOp op(*i);
			op.PushChatOffline(role->GetID(), m_chatforward.m_Data.chatinfo())*/;
			continue;
		}
		//m_chatforward.m_sessionID = destrole->GetSessionID();
		//GatetLink::Instance()->SendToSession(m_chatforward);
	}
	//AudioChatMgr::Instance()->AddAudioChatInfo(KKSG::PrivateChannel, role->GetID(), role->GetID(), role->GetName(), *m_chatforward.m_Data.mutable_chatinfo());
}

void ChatTeamHandler::FowardChatInfo(Role* role, const std::vector<UINT64>* destroleids)
{
	//role->ChatTeamOper(&m_chatforward);
	TeamCopy* team = TeamCopyMgr::Instance()->GetTeamByRoleID(role->GetID());
	if (NULL == team)
	{
		return;
	}
	/*
	const std::list<CTeamMember *>& members = team->GetMember();
	for (auto i = members.begin(); i != members.end(); ++i)
	{
		if ((*i)->IsRobot)
		{
			continue;
		}
		Role* role = RoleManager::Instance()->FindByRoleID((*i)->m_qwRoleID);
		if (NULL == role)
		{
			continue;
		}
		m_chatforward.m_sessionID = role->GetSessionID();
		GatetLink::Instance()->SendToSession(m_chatforward);
	}
	*/
	//AudioChatMgr::Instance()->AddAudioChatInfo(KKSG::TeamChannel, teamid, role->GetID(), role->GetName(), *m_chatforward.m_Data.mutable_chatinfo());
}
