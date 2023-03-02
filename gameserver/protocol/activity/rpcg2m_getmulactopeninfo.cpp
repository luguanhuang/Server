#include "pch.h"
#include "activity/rpcg2m_getmulactopeninfo.h"

// generate by ProtoGen at date: 2017/2/10 21:44:51

RPC_CLIENT_IMPLEMETION(RpcG2M_GetMulActOpenInfo, GetMulActOpenInfoArg, GetMulActOpenInfoRes)

void RpcG2M_GetMulActOpenInfo::OnReply(const GetMulActOpenInfoArg &roArg, const GetMulActOpenInfoRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = roRes.result();
	data.m_pUserPtr = (void*)&roRes;
	CRpc::ReplyDelayRpc(roArg.rpcid(), data);
}

void RpcG2M_GetMulActOpenInfo::OnTimeout(const GetMulActOpenInfoArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = KKSG::ERR_TIMEOUT;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), data);
}
