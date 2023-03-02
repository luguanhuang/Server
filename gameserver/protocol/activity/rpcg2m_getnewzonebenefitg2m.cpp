#include "pch.h"
#include "activity/rpcg2m_getnewzonebenefitg2m.h"

// generate by ProtoGen at date: 2017/10/24 14:33:28

RPC_CLIENT_IMPLEMETION(RpcG2M_GetNewZoneBenefitG2M, GetNewZoneBenefitG2MArg, GetNewZoneBenefitG2MRes)

void RpcG2M_GetNewZoneBenefitG2M::OnReply(const GetNewZoneBenefitG2MArg &roArg, const GetNewZoneBenefitG2MRes &roRes, const CUserData &roUserData)
{
	CUserData userData(roRes.result(), (void*)&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcG2M_GetNewZoneBenefitG2M::OnTimeout(const GetNewZoneBenefitG2MArg &roArg, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
