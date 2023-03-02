#include "pch.h"
#include "pay/rpcg2m_paydegreereq.h"
#include "gamelogic/ibshoprecord.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/4/24 11:09:30

RPC_CLIENT_IMPLEMETION(RpcG2M_PayDegreeReq, PayDegreeArg, PayDegreeRes)

void RpcG2M_PayDegreeReq::OnReply(const PayDegreeArg &roArg, const PayDegreeRes &roRes, const CUserData &roUserData)
{
	if (roRes.error() == KKSG::ERR_SUCCESS)
	{
		Role* pRole = RoleManager::Instance()->FindBySession(roArg.roleid());
		if (pRole)
		{
			IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
			pRecord->SetPayDegree(roRes.degree());
		}
	}
	UINT64 roleid = roArg.roleid();
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)roRes.error();
	oUser.m_pUserPtr   = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}

void RpcG2M_PayDegreeReq::OnTimeout(const PayDegreeArg &roArg, const CUserData &roUserData)
{
	//如果在Midas拉取不到数据
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	oUser.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
