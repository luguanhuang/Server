#include "pch.h"
#include "battlewatch/rpcg2m_enterwatchbattlems.h"

// generate by ProtoGen at date: 2016/7/29 17:15:57

RPC_CLIENT_IMPLEMETION(RpcG2M_EnterWatchBattleMs, EnterWatchBattleMsArg, EnterWatchBattleMsRes)

void RpcG2M_EnterWatchBattleMs::OnReply(const EnterWatchBattleMsArg &roArg, const EnterWatchBattleMsRes &roRes, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = roRes.error();
	oUserData.m_pUserPtr = (void*)&roRes;

	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);

}

void RpcG2M_EnterWatchBattleMs::OnTimeout(const EnterWatchBattleMsArg &roArg, const CUserData &roUserData)
{
}
