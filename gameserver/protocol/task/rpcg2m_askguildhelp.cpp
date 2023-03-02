#include "pch.h"
#include "task/rpcg2m_askguildhelp.h"

// generate by ProtoGen at date: 2016/11/16 11:19:49

RPC_CLIENT_IMPLEMETION(RpcG2M_AskGuildHelp, AskGuildHelpReq, AskGuildHelpRes)

void RpcG2M_AskGuildHelp::OnReply(const AskGuildHelpReq &roArg, const AskGuildHelpRes &roRes, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = roRes.code();
	userData.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcG2M_AskGuildHelp::OnTimeout(const AskGuildHelpReq &roArg, const CUserData &roUserData)
{
	CUserData userData;
	userData.m_dwUserData = KKSG::ERR_TIMEOUT;
	userData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
