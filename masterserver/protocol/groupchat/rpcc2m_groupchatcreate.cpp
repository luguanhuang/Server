#include "pch.h"
#include "groupchat/rpcc2m_groupchatcreate.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2017/7/15 11:50:39

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatCreate, GroupChatCreateC2S, GroupChatCreateS2C)

void RpcC2M_GroupChatCreate::OnCall(const GroupChatCreateC2S &roArg, GroupChatCreateS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    UINT64 groupid = 0;
    KKSG::ErrorCode ec = GroupChatMgr::Instance()->CreateGroup(pRole, roArg.groupchatname(), roArg.createtype(), groupid);
    roRes.set_errorcode(ec);
    roRes.set_groupchatid(groupid);
}

void RpcC2M_GroupChatCreate::OnDelayReplyRpc(const GroupChatCreateC2S &roArg, GroupChatCreateS2C &roRes, const CUserData &roUserData)
{
}
