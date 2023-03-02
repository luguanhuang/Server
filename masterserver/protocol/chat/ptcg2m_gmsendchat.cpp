#include "pch.h"
#include "chat/ptcg2m_gmsendchat.h"
#include "chat/chatmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "friend/friendop.h"
#include "friend/friendconfig.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "partner/partnermgr.h"
#include "chat/ptcm2f_audiochanneltofm.h"
#include "network/fmlink.h"
#include "chat/fmmgr.h"
#include "team/teamtranshandler.h"
#include "role/rolesummarymgr.h"
#include "loghelper/tlogr.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2016/8/8 20:11:39

void PtcG2M_GMSendChat::Process(UINT32 dwConnID)
{
	const KKSG::ChatInfo& chatinfo = m_Data.chatinfo();	

	switch(chatinfo.channel())
	{
	case KKSG::WorldChannel:
		{
			CChatMgr::Instance()->SendWorld(chatinfo);
			CChatMgr::Instance()->AddChat(0, chatinfo);
			return;
		}
	case KKSG::GuildChannel:
		{
			Guild* guild = CGuildMgr::Instance()->GetGuildByRoleId(chatinfo.source().roleid());
			if (NULL == guild)
			{
				return;
			}
			CChatMgr::Instance()->SendGuild(guild, chatinfo);
			CChatMgr::Instance()->AddChat(guild->GetID(), chatinfo);
			return;
		}
    case KKSG::GroupChatChannel:
        {
            UINT64 groupid = chatinfo.groupchatinfo().groupchatid();
            UINT64 roleid = chatinfo.source().roleid();
            if(GroupChatMgr::Instance()->IsGroupMember(groupid, roleid))
            {
                KKSG::ChatInfo tmpchatinfo;
                tmpchatinfo.CopyFrom(chatinfo);
                tmpchatinfo.set_time(GameTime::GetTime());
                GroupChatMgr::Instance()->SendGroupChat(groupid, tmpchatinfo);
                CChatMgr::Instance()->AddChat(groupid, tmpchatinfo);
            }
            return;
        }
	case KKSG::TeamChannel:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(chatinfo.source().roleid());
			if(NULL == pRole)
			{
				return ;
			}
			if(pRole->HasTeamInMs())
			{
				Team* team = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
				if (NULL == team)
				{
					return;
				}
				CChatMgr::Instance()->SendTeam(team, chatinfo);
			}
			else if(pRole->HasTeamInTs())//跨服队伍
			{
				auto tmpInfo = chatinfo;
				CChatMgr::Instance()->FillSourceData(tmpInfo.mutable_source());
				TeamTransHandler handler;
				handler.HandleChatToTs(pRole->GetID(), tmpInfo);
			}
			return;
		}
	case KKSG::PrivateChannel:
		{
			if(chatinfo.destlist_size() == 0)
			{
				LogError("target roleid is null\n");
				return;
			}
			UINT64 targetRoleID = chatinfo.destlist(0).roleid();
			if(chatinfo.source().roleid() == targetRoleID)
			{
				LogWarn("private chat send message to himself uid:%llu", targetRoleID);
				return;
			}

			CChatMgr::Instance()->AddChat(chatinfo.source().roleid(), chatinfo);
			CChatMgr::Instance()->AddChat(targetRoleID, chatinfo);

			///> 向目标用户发送聊天消息
			CChatMgr::Instance()->SendUser(targetRoleID, chatinfo, true);	

			FriendOp op(chatinfo.source().roleid());
			op.SetOther(targetRoleID);
			op.AddDegree(DegreeChat);
			return;
		}
	/*case KKSG::CurrentChannel:
	case KKSG::SpectateChannel:
		{
			std::vector<UINT64> roleList;
			for(UINT32 i = 0; i < m_Data.roleidlist_size(); i++)
			{
				roleList.push_back(m_Data.roleidlist(i));
			}
			CChatMgr::Instance()->SendGroup(roleList, m_Data.chatinfo());
			return;
		}*/
	case KKSG::PartnerChannel:
		{
			CChatMgr::Instance()->SendPartner(chatinfo);
			return;
		}
	case  KKSG::AudioChannel:
		{
			UINT64 nRoleID = chatinfo.source().roleid();
			if(FMMgr::Instance()->IsInRoom(nRoleID))
			{
				//非主播
				if (!FMMgr::Instance()->IsAudio(nRoleID))
					if (FMMgr::Instance()->GetCount()>=GlobalConfig::GetInstance().AudioMsgMax)
						return;
				bool bFlag = false;
				for(int i = 0; i< chatinfo.param().size(); i++)
				{		
					if (chatinfo.param(i).has_link())
					{
						bFlag = true;
						break;
					}
				}
				if (bFlag)
				{
					std::vector<UINT64 > roleSet;
					FMMgr::Instance()->GetRoleList(nRoleID, roleSet);
					CChatMgr::Instance()->SendGroup(roleSet, chatinfo);

				}else
				{
					PtcM2F_AudioChannelToFm ptc;
					ptc.m_Data.CopyFrom(chatinfo);	
					FMLink::Instance()->SendTo(ptc);
				}	
				if (!FMMgr::Instance()->IsAudio(nRoleID))
				{
					CRoleSummary *pSumary = CRoleSummaryMgr::Instance()->GetRoleSummary(nRoleID);
					if (pSumary != NULL)
					{
						FMMgr::Instance()->DoLog(pSumary->GetAccount(),nRoleID, 2,chatinfo.info());
					}
					FMMgr::Instance()->Incre();
				}
				return;
			}
		}
	}
	
	SSError << "Unknown Chat Channel, Channel:" << chatinfo.channel() << ", RoleID:" << chatinfo.source().roleid() << ", RoleName:" << chatinfo.source().name() << END;
	
}
