#include "pch.h"
#include "guild/rpcg2m_notifycheckinbox.h"

// generate by ProtoGen at date: 2016/9/18 13:55:59

RPC_CLIENT_IMPLEMETION(RpcG2M_NotifyCheckinBox, NotifyCheckinBoxArg, NotifyCheckinBoxRes)

void RpcG2M_NotifyCheckinBox::OnReply(const NotifyCheckinBoxArg &roArg, const NotifyCheckinBoxRes &roRes, const CUserData &roUserData)
{
	std::map<UINT32, UINT32> itemMap;
	for (UINT32 cnt = 0; cnt < roRes.items_size(); ++cnt)
	{
		itemMap[roRes.items(cnt).itemid()] = roRes.items(cnt).itemcount(); 
	}

	CUserData oUser;
	if (roRes.has_errorcode() && roRes.errorcode() != ERR_SUCCESS)
	{
		oUser.m_dwUserData = (UINT32)roRes.errorcode();
	}
	else
	{
		oUser.m_dwUserData = (UINT32)ERR_SUCCESS;
	}
	oUser.m_pUserPtr = (void*)&itemMap;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcG2M_NotifyCheckinBox::OnTimeout(const NotifyCheckinBoxArg &roArg, const CUserData &roUserData)
{
}
