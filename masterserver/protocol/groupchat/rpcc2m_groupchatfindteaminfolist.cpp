#include "pch.h"
#include "groupchat/rpcc2m_groupchatfindteaminfolist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"
// generate by ProtoGen at date: 2017/7/13 11:28:01

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatFindTeamInfoList, GroupChatFindTeamInfoListC2S, GroupChatFindTeamInfoListS2C)

void RpcC2M_GroupChatFindTeamInfoList::OnCall(const GroupChatFindTeamInfoListC2S &roArg, GroupChatFindTeamInfoListS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcZMFindTeamInfoList(pRole, roArg, roRes);
}

void RpcC2M_GroupChatFindTeamInfoList::OnDelayReplyRpc(const GroupChatFindTeamInfoListC2S &roArg, GroupChatFindTeamInfoListS2C &roRes, const CUserData &roUserData)
{
}
