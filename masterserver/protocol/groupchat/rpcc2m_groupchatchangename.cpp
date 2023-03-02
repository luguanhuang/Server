#include "pch.h"
#include "groupchat/rpcc2m_groupchatchangename.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/27 19:55:28

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatChangeName, GroupChatChangeNameC2S, GroupChatChangeNameS2C)

void RpcC2M_GroupChatChangeName::OnCall(const GroupChatChangeNameC2S &roArg, GroupChatChangeNameS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcChangeGroupName(pRole, roArg, roRes);
}

void RpcC2M_GroupChatChangeName::OnDelayReplyRpc(const GroupChatChangeNameC2S &roArg, GroupChatChangeNameS2C &roRes, const CUserData &roUserData)
{
}
