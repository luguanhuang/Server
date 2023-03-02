#include "pch.h"
#include "groupchat/rpcc2m_groupchatgetgroupinfo.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2017/7/13 11:46:57

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatGetGroupInfo, GroupChatGetGroupInfoC2S, GroupChatGetGroupInfoS2C)

void RpcC2M_GroupChatGetGroupInfo::OnCall(const GroupChatGetGroupInfoC2S &roArg, GroupChatGetGroupInfoS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    GroupChatMgr::Instance()->RpcRoleGetGroupInfo(pRole, roArg.groupchatid(), roRes);
}

void RpcC2M_GroupChatGetGroupInfo::OnDelayReplyRpc(const GroupChatGetGroupInfoC2S &roArg, GroupChatGetGroupInfoS2C &roRes, const CUserData &roUserData)
{
}
