#include "pch.h"
#include "common/rpcm2d_mssavedbend.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/3/9 17:34:30

RPC_SERVER_IMPLEMETION(RpcM2D_MsSaveDBEnd, MsSaveDBEndArg, MsSaveDBEndRes)

void RpcM2D_MsSaveDBEnd::OnCall(const MsSaveDBEndArg &roArg, MsSaveDBEndRes &roRes)
{
	LogInfo("MS Save DB End");
	CMysqlMgr::Instance()->SetMSSaveDBEnd(true);
}

void RpcM2D_MsSaveDBEnd::OnDelayReplyRpc(const MsSaveDBEndArg &roArg, MsSaveDBEndRes &roRes, const CUserData &roUserData)
{
}
