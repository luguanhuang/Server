#include "pch.h"
#include "battlewatch/rpcg2m_getwatchinfobytype.h"

// generate by ProtoGen at date: 2016/7/29 17:09:29

RPC_CLIENT_IMPLEMETION(RpcG2M_GetWatchInfoByType, GetWatchInfoByTypeArg, GetWatchInfoByTypeRes)

void RpcG2M_GetWatchInfoByType::OnReply(const GetWatchInfoByTypeArg &roArg, const GetWatchInfoByTypeRes &roRes, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = roRes.error();
	oUserData.m_pUserPtr = (void*)&roRes;

	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}

void RpcG2M_GetWatchInfoByType::OnTimeout(const GetWatchInfoByTypeArg &roArg, const CUserData &roUserData)
{
}
