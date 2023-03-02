#include "pch.h"
#include "groupchat/rpcc2m_groupchatquit.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "groupchat/groupchatmgr.h"

// generate by ProtoGen at date: 2017/7/13 11:50:37

RPC_SERVER_IMPLEMETION(RpcC2M_GroupChatQuit, GroupChatQuitC2S, GroupChatQuitS2C)

void RpcC2M_GroupChatQuit::OnCall(const GroupChatQuitC2S &roArg, GroupChatQuitS2C &roRes)
{
    CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
    if(NULL == pRole)
    {
        roRes.set_errorcode(ERR_INVALID_REQUEST);
        return;
    }
    KKSG::ErrorCode ec = GroupChatMgr::Instance()->QuitGroup(pRole, roArg.groupchatid());
    roRes.set_errorcode(ec);
}

void RpcC2M_GroupChatQuit::OnDelayReplyRpc(const GroupChatQuitC2S &roArg, GroupChatQuitS2C &roRes, const CUserData &roUserData)
{
}
