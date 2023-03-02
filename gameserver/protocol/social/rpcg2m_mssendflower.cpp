#include "pch.h"
#include "social/rpcg2m_mssendflower.h"

// generate by ProtoGen at date: 2016/8/18 16:37:31

RPC_CLIENT_IMPLEMETION(RpcG2M_MSSendFlower, MSSendFlowerArg, MSSendFlowerRes)

void RpcG2M_MSSendFlower::OnReply(const MSSendFlowerArg &roArg, const MSSendFlowerRes &roRes, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = roRes.error();
	oUserData.m_pUserPtr = (void*)&roRes;

	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}

void RpcG2M_MSSendFlower::OnTimeout(const MSSendFlowerArg &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = KKSG::ERR_SUCCESS;

	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
