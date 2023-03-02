#include "pch.h"
#include "groupchat/rpcc2m_groupchatleaderaddrole.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/25 13:15:43

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatLeaderAddRole, GroupChatLeaderAddRoleC2S, GroupChatLeaderAddRoleS2C)

void RpcC2M_GroupChatLeaderAddRole::OnCall(const GroupChatLeaderAddRoleC2S &roArg, GroupChatLeaderAddRoleS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMLeadAddRole(pRole, roArg, roRes);
}

void RpcC2M_GroupChatLeaderAddRole::OnDelayReplyRpc(const GroupChatLeaderAddRoleC2S &roArg, GroupChatLeaderAddRoleS2C &roRes, const CUserData &roUserData)
{
}
