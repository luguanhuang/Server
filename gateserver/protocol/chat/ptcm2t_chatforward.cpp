#include "pch.h"
#include "chat/ptcm2t_chatforward.h"
#include "chat/ptct2c_chatnotify.h"
#include "clientlink.h"
#include "gslink.h"
#include "session/sessionmanager.h"


// generate by ProtoGen at date: 2016/7/25 15:31:40

void PtcM2T_chatforward::Process(UINT32 dwConnID)
{
	PtcT2C_ChatNotify chatnotify;
	KKSG::ChatInfo* chatinfo = chatnotify.m_Data.mutable_chatinfo();
	chatinfo->CopyFrom(m_Data.chatinfo());
	///>SSWarn << "Handle M2T Message, CMD=PTCM2T_chatforward, RoleID:" << chatinfo->source().roleid() << ", Name:" << chatinfo->source().name() << ", Channel:" << chatinfo->channel() << ", ChatInfo:" << chatinfo->info() << END;

	if (m_Data.chatinfo().isbroadcast())
	{
		for (auto i = SessionManager::Instance()->Begin(); i != SessionManager::Instance()->End(); ++i)
		{
			if (SESSION_LOGIN != i->m_state)
			{
				continue;
			}
			CClientLink::Instance()->Send(i->m_connID, chatnotify);
		}
	}
	else
	{		
		UINT32 connid = SessionManager::Instance()->GetSessionConnID(m_sessionID);
		CClientLink::Instance()->Send(connid, chatnotify);
		///>SSWarn << "[notbroadcast]Send ChatNotify To Client, ConnID:" << connid << ", ChatInfo:" << chatinfo->info() << END;
	}
	///>SSWarn << "Finish M2T Message, cmd=PTCM2T_chatforward, ChatInfo:" << chatinfo->info() << END;
}
