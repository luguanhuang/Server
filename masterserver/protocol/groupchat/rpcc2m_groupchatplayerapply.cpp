#include "pch.h"
#include "groupchat/rpcc2m_groupchatplayerapply.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/13 11:45:10

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatPlayerApply, GroupChatPlayerApplyC2S, GroupChatPlayerApplyS2C)

void RpcC2M_GroupChatPlayerApply::OnCall(const GroupChatPlayerApplyC2S &roArg, GroupChatPlayerApplyS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMRoleApply(pRole, roArg, roRes);
}

void RpcC2M_GroupChatPlayerApply::OnDelayReplyRpc(const GroupChatPlayerApplyC2S &roArg, GroupChatPlayerApplyS2C &roRes, const CUserData &roUserData)
{
}
