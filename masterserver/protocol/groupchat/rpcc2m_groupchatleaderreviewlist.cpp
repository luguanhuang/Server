#include "pch.h"
#include "groupchat/rpcc2m_groupchatleaderreviewlist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/13 11:31:17

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatLeaderReviewList, GroupChatLeaderReviewListC2S, GroupChatLeaderReviewListS2C)

void RpcC2M_GroupChatLeaderReviewList::OnCall(const GroupChatLeaderReviewListC2S &roArg, GroupChatLeaderReviewListS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMLeaderGetReviewList(pRole, roRes);
}

void RpcC2M_GroupChatLeaderReviewList::OnDelayReplyRpc(const GroupChatLeaderReviewListC2S &roArg, GroupChatLeaderReviewListS2C &roRes, const CUserData &roUserData)
{
}
