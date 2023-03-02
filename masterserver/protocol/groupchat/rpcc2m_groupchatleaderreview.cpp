#include "pch.h"
#include "groupchat/rpcc2m_groupchatleaderreview.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/13 11:44:25

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatLeaderReview, GroupChatLeaderReviewC2S, GroupChatLeaderReviewS2C)

void RpcC2M_GroupChatLeaderReview::OnCall(const GroupChatLeaderReviewC2S &roArg, GroupChatLeaderReviewS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMLeaderReview(pRole, roArg, roRes);
}

void RpcC2M_GroupChatLeaderReview::OnDelayReplyRpc(const GroupChatLeaderReviewC2S &roArg, GroupChatLeaderReviewS2C &roRes, const CUserData &roUserData)
{
}
