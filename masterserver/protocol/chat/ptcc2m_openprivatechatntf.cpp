#include "pch.h"
#include "chat/ptcc2m_openprivatechatntf.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "chat/chatmgr.h"

// generate by ProtoGen at date: 2016/10/9 15:21:28

void PtcC2M_OpenPrivateChatNtf::Process(UINT32 dwConnID)
{
	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		return;
	}

	CChatMgr::Instance()->OpenPrivateChat(poRole->GetID(), m_Data.roleid());
}
