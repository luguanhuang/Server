#include "pch.h"
#include "common/rpcm2d_mssavedbend.h"
#include "network/dblink.h"
#include "process.h"

// generate by ProtoGen at date: 2017/3/9 17:34:30

RPC_CLIENT_IMPLEMETION(RpcM2D_MsSaveDBEnd, MsSaveDBEndArg, MsSaveDBEndRes)

void RpcM2D_MsSaveDBEnd::OnReply(const MsSaveDBEndArg &roArg, const MsSaveDBEndRes &roRes, const CUserData &roUserData)
{
	LogInfo("MsSaveDBEnd Reply");
	SetMsSaveDBEnd(true);
}

void RpcM2D_MsSaveDBEnd::OnTimeout(const MsSaveDBEndArg &roArg, const CUserData &roUserData)
{
	LogInfo("MsSaveDBEnd Timeout, ReSend rpc");
	RpcM2D_MsSaveDBEnd* rpc = RpcM2D_MsSaveDBEnd::CreateRpc();
	DBLink::Instance()->SendTo(*rpc);
}
