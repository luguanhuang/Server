#include "pch.h"
#include "pay/rpcm2d_savegiftibitem.h"
#include "db/task/savegiftibitem.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/3/18 15:00:31

RPC_SERVER_IMPLEMETION(RpcM2D_SaveGiftIbItem, GiftIbItemArg, GiftIbItemRes)

void RpcM2D_SaveGiftIbItem::OnCall(const GiftIbItemArg &roArg, GiftIbItemRes &roRes)
{
	CGiftIBItemTask* poTask = new CGiftIBItemTask();
	poTask->m_Data.CopyFrom(roArg);
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcM2D_SaveGiftIbItem::OnDelayReplyRpc(const GiftIbItemArg &roArg, GiftIbItemRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_error(nResult);
}
