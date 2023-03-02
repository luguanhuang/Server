#include "pch.h"
#include "gm/rpcm2g_gshttpcommandnew.h"
#include "idip/idiphandler.h"
#include "idip/idip.h"
#include "idip/idipproc.h"

// generate by ProtoGen at date: 2016/7/16 11:48:36

RPC_CLIENT_IMPLEMETION(RpcM2G_GSHttpCommandNew, GMHttpArg, GMHttpRes)

void RpcM2G_GSHttpCommandNew::OnReply(const GMHttpArg &roArg, const GMHttpRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	std::string str = roRes.resultstring();
	///> use field httpid as delay rpc id
	CIdipReply oReply(str);
	oReply.m_nErrCode = (EIdipErrCode)roRes.result();
	data.m_pUserPtr = (void*)&oReply;
	data.m_dwUserData = roRes.result();
	CIdipProcess::Instance()->OnAsyncReply(roArg.httpid(), data);
	//CRpc::ReplyDelayRpc(roArg.httpid(), data);
}

void RpcM2G_GSHttpCommandNew::OnTimeout(const GMHttpArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = IDIP_ERR_TIMEOUT;
	data.m_pUserPtr = NULL;
	CIdipProcess::Instance()->OnAsyncReply(roArg.httpid(), data);
}
