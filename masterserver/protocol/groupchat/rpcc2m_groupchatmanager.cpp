#include "pch.h"
#include "groupchat/rpcc2m_groupchatmanager.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2017/7/13 11:49:24

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatManager, GroupChatManagerC2S, GroupChatManagerS2C)

void RpcC2M_GroupChatManager::OnCall(const GroupChatManagerC2S &roArg, GroupChatManagerS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    
    KKSG::ErrorCode ec = ERR_SUCCESS;
    for(int i = 0; i<roArg.subrolelist().size(); ++i)
    {
        KKSG::ErrorCode ectmp = GroupChatMgr::Instance()->SubRole(pRole, roArg.groupchatid(), roArg.subrolelist(i));
        if(ec != ectmp) ec = ectmp;
    }
    for(int i = 0; i<roArg.addrolelist().size(); ++i)
    {
        KKSG::ErrorCode ectmp = GroupChatMgr::Instance()->AddRole(pRole, roArg.groupchatid(), roArg.addrolelist(i));
        if(ec != ectmp) ec = ectmp;
    }

    roRes.set_errorcode(ec);
}

void RpcC2M_GroupChatManager::OnDelayReplyRpc(const GroupChatManagerC2S &roArg, GroupChatManagerS2C &roRes, const CUserData &roUserData)
{
}
