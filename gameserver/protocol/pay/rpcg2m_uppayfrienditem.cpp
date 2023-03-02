#include "pch.h"
#include "pay/rpcg2m_uppayfrienditem.h"


// generate by ProtoGen at date: 2017/3/14 10:36:09

RPC_CLIENT_IMPLEMETION(RpcG2M_UpPayFriendItem, UpPayFriendItemArg, UpPayFriendItemRes)

void RpcG2M_UpPayFriendItem::OnReply(const UpPayFriendItemArg &roArg, const UpPayFriendItemRes &roRes, const CUserData &roUserData)
{
	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	PayFriendItemRes res;
	res.set_ret(roRes.errorcode());
	res.set_billno(roRes.billno());
	res.set_token(roRes.token());
	res.set_url_param(roRes.url_param());
	oUser.m_dwUserData = roRes.errorcode();
	oUser.m_pUserPtr = (void*)&res;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}

void RpcG2M_UpPayFriendItem::OnTimeout(const UpPayFriendItemArg &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
