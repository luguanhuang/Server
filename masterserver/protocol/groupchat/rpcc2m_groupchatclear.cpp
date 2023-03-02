#include "pch.h"
#include "groupchat/rpcc2m_groupchatclear.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2017/7/17 16:58:02

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatClear, GroupChatClearC2S, GroupChatClearS2C)

void RpcC2M_GroupChatClear::OnCall(const GroupChatClearC2S &roArg, GroupChatClearS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    KKSG::ErrorCode ec = GroupChatMgr::Instance()->ClearGroupChat(pRole);
    roRes.set_errorcode(ec);
}

void RpcC2M_GroupChatClear::OnDelayReplyRpc(const GroupChatClearC2S &roArg, GroupChatClearS2C &roRes, const CUserData &roUserData)
{
}
