#include "pch.h"
#include "chat/rpcc2m_clearprivatechatlist.h"
#include "chat/chatmgr.h"
#include "role/rolemanager.h"
#include "role/role.h"

// generate by ProtoGen at date: 2016/8/26 10:05:34

RPC_SERVER_IMPLEMETION(RpcC2M_ClearPrivateChatList, ClearPrivateChatListArg, ClearPrivateChatListRes)

void RpcC2M_ClearPrivateChatList::OnCall(const ClearPrivateChatListArg &roArg, ClearPrivateChatListRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}

	if(0 == roArg.roleid())
	{
		CChatMgr::Instance()->ClearPrivateListAll(pRole->GetID());
	}
	else
	{
		CChatMgr::Instance()->ClearPrivateList(pRole->GetID(), roArg.roleid());
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2M_ClearPrivateChatList::OnDelayReplyRpc(const ClearPrivateChatListArg &roArg, ClearPrivateChatListRes &roRes, const CUserData &roUserData)
{
}
