#include "pch.h"
#include "role/rpcm2i_getzoneroleinfom2i.h"

// generate by ProtoGen at date: 2017/10/24 15:01:59

RPC_CLIENT_IMPLEMETION(RpcM2I_GetZoneRoleInfoM2I, GetZoneRoleInfoM2IArg, GetZoneRoleInfoM2IRes)

void RpcM2I_GetZoneRoleInfoM2I::OnReply(const GetZoneRoleInfoM2IArg &roArg, const GetZoneRoleInfoM2IRes &roRes, const CUserData &roUserData)
{
	CUserData userData(roRes.result(), (void*)&roRes);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}

void RpcM2I_GetZoneRoleInfoM2I::OnTimeout(const GetZoneRoleInfoM2IArg &roArg, const CUserData &roUserData)
{
	CUserData userData(KKSG::ERR_TIMEOUT, NULL);
	CRpc::ReplyDelayRpc(roArg.rpcid(), userData);
}
