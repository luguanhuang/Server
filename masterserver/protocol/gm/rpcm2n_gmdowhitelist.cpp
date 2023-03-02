#include "pch.h"
#include "gm/rpcm2n_gmdowhitelist.h"
#include "idip/idip.h"
#include "idip/idiphandler.h"
#include "idip/idipproc.h"

// generate by ProtoGen at date: 2017/8/22 15:21:06

RPC_CLIENT_IMPLEMETION(RpcM2N_GMDoWhiteList, GMDoWhiteListArg, GMDoWhiteListRes)

void RpcM2N_GMDoWhiteList::OnReply(const GMDoWhiteListArg &roArg, const GMDoWhiteListRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	std::string str = roRes.resultstring();
	CIdipReply oReply(str);
	oReply.m_nErrCode = (EIdipErrCode)roRes.result();
	data.m_pUserPtr = (void*)&oReply;
	data.m_dwUserData = roRes.result();
	CIdipProcess::Instance()->OnAsyncReply(roArg.httpid(), data);
}

void RpcM2N_GMDoWhiteList::OnTimeout(const GMDoWhiteListArg &roArg, const CUserData &roUserData)
{
	CUserData data;
	data.m_dwUserData = IDIP_ERR_TIMEOUT;
	data.m_pUserPtr = NULL;
	CIdipProcess::Instance()->OnAsyncReply(roArg.httpid(), data);
}
