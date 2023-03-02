#include "pch.h"
#include "role/rpci2m_getzoneroleinfoi2m.h"
#include "serverrole/readrolecontrol.h"
#include "define/miscdef.h"


// generate by ProtoGen at date: 2017/10/24 15:13:26

RPC_CLIENT_IMPLEMETION(RpcI2M_GetZoneRoleInfoI2M, GetZoneRoleInfoI2MArg, GetZoneRoleInfoI2MRes)

void RpcI2M_GetZoneRoleInfoI2M::OnReply(const GetZoneRoleInfoI2MArg &roArg, const GetZoneRoleInfoI2MRes &roRes, const CUserData &roUserData)
{
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		ReadRoleControl::Instance()->OnReadCompleate(roArg.account(), roArg.serverid(), roRes);
	}
	else if (roArg.opertype() == ChargeBackOperType_Bind)
	{
		CUserData userData(roRes.result(), NULL);
		CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
	}
}

void RpcI2M_GetZoneRoleInfoI2M::OnTimeout(const GetZoneRoleInfoI2MArg &roArg, const CUserData &roUserData)
{
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		ReadRoleControl::Instance()->OnReadTimeOut(roArg.account(), roArg.serverid());
	}
	else
	{
		CUserData userData(KKSG::ERR_TIMEOUT, NULL);
		CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
	}
}




