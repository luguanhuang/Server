#include "pch.h"
#include "pay/rpcm2d_savegiftibhist.h"
#include "db/task/giftibhisttask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/4/17 14:46:31

RPC_SERVER_IMPLEMETION(RpcM2D_SaveGiftIbHist, SaveIbGiftRecordArg, SaveIbGiftRecordRes)

void RpcM2D_SaveGiftIbHist::OnCall(const SaveIbGiftRecordArg &roArg, SaveIbGiftRecordRes &roRes)
{
	GiftIBHistTask* poTask = new GiftIBHistTask();
	poTask->SetData(m_oArg);
	CMysqlMgr::Instance()->PushTask(poTask);
}

void RpcM2D_SaveGiftIbHist::OnDelayReplyRpc(const SaveIbGiftRecordArg &roArg, SaveIbGiftRecordRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_error(nResult);
}
