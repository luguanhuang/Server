#include "pch.h"
#include "groupchat/rpcc2m_groupchatfindroleinfolist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/13 11:29:44

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatFindRoleInfoList, GroupChatFindRoleInfoListC2S, GroupChatFindRoleInfoListS2C)

void RpcC2M_GroupChatFindRoleInfoList::OnCall(const GroupChatFindRoleInfoListC2S &roArg, GroupChatFindRoleInfoListS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMFindRoleInfoList(pRole, roArg, roRes);
}

void RpcC2M_GroupChatFindRoleInfoList::OnDelayReplyRpc(const GroupChatFindRoleInfoListC2S &roArg, GroupChatFindRoleInfoListS2C &roRes, const CUserData &roUserData)
{
}
