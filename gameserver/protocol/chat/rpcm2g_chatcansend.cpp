#include "pch.h"
#include "chat/rpcm2g_chatcansend.h"

#include "unit/rolemanager.h"
#include "gamelogic/chatmgr.h"
#include "timeutil.h"
///>#include "gamelogic/audioChatMgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "gamelogic/rolesummary.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/chatmgr.h"
#include "gamelogic/chatrecord.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "chat/ptcg2m_reportwatchntf.h"
#include "network/mslink.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2016/7/25 15:35:56

RPC_SERVER_IMPLEMETION(RpcM2G_ChatCanSend, ChatCanSendReq, ChatCanSendRes)

void RpcM2G_ChatCanSend::OnCall(const ChatCanSendReq &roArg, ChatCanSendRes &roRes)
{
	const KKSG::ChatInfo& chatinfo = roArg.chatinfo();
	SSDebug << "Handle M->G Message, CMD=RpcM2G_ChatCanSend, RoleID:" << chatinfo.source().roleid() << ", Name:" << chatinfo.source().name() << ", Channel:" << chatinfo.channel() << ", ChatInfo:" << chatinfo.info() << ", SessionID:" << m_sessionID << END;
	Role* pRole = RoleManager::Instance()->FindByRoleID( chatinfo.source().roleid() );
	if (pRole == NULL)
	{
		SSError << "Find Role Failed, RoleID:" << chatinfo.source().roleid() << ", Name:" << chatinfo.source().name() << END;
		return; 
	}

	KKSG::ErrorCode code = ChatMgr::Instance()->ChatLegal(pRole, chatinfo, roRes);
	roRes.set_result(code);

	SSDebug << "Finish Handle M->G Message, CMD=RpcM2G_ChatCanSend, RoleID:" << pRole->GetID() << ", Name:" << pRole->GetName() << ", Result:" << roRes.result() << END;

	ChatRecord* record = pRole->Get<ChatRecord>();	
	if(KKSG::ERR_SUCCESS == roRes.result())
	{
		if (KKSG::WorldChannel == chatinfo.channel())
		{
			record->UpdateChatTimes();
			ChatMgr::Instance()->SendWorldChannelLeftTimesNotify(pRole);
		}
		INT32 iChatType = GetChatType(chatinfo.channel());
		if (iChatType >= 0)
		{
			TSecTalkFlow oLog(pRole);
			oLog.m_iChatType = iChatType;
			oLog.m_iMsgType = chatinfo.audiouid()?1:0;
			oLog.m_strChatContents = chatinfo.info();
			if (chatinfo.channel() == KKSG::PrivateChannel && chatinfo.destlist_size())
			{
				oLog.m_uReceiverRoleID = chatinfo.destlist(0).roleid();
			}
			Scene* pScene = pRole->GetCurrScene();
			if (pScene)
			{
				oLog.m_iSceneType = pScene->GetSceneType();
			}
			oLog.Do();
		}
	}
}

void RpcM2G_ChatCanSend::OnDelayReplyRpc(const ChatCanSendReq &roArg, ChatCanSendRes &roRes, const CUserData &roUserData)
{
}
