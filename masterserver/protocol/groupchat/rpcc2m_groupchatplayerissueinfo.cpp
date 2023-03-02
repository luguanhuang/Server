#include "pch.h"
#include "groupchat/rpcc2m_groupchatplayerissueinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2017/7/13 11:41:33

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatPlayerIssueInfo, GroupChatPlayerIssueInfoC2S, GroupChatPlayerIssueInfoS2C)

void RpcC2M_GroupChatPlayerIssueInfo::OnCall(const GroupChatPlayerIssueInfoC2S &roArg, GroupChatPlayerIssueInfoS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMRoleIssueInfo(pRole, roArg, roRes);
}

void RpcC2M_GroupChatPlayerIssueInfo::OnDelayReplyRpc(const GroupChatPlayerIssueInfoC2S &roArg, GroupChatPlayerIssueInfoS2C &roRes, const CUserData &roUserData)
{
}
