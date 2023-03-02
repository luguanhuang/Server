#include "pch.h"
#include "groupchat/rpcc2m_groupchatleaderissueinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/13 11:42:54

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatLeaderIssueInfo, GroupChatLeaderIssueInfoC2S, GroupChatLeaderIssueInfoS2C)

void RpcC2M_GroupChatLeaderIssueInfo::OnCall(const GroupChatLeaderIssueInfoC2S &roArg, GroupChatLeaderIssueInfoS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMLeaderIssueInfo(pRole, roArg, roRes);
}

void RpcC2M_GroupChatLeaderIssueInfo::OnDelayReplyRpc(const GroupChatLeaderIssueInfoC2S &roArg, GroupChatLeaderIssueInfoS2C &roRes, const CUserData &roUserData)
{
}
