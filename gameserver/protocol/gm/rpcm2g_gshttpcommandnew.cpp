#include "pch.h"
#include "gm/rpcm2g_gshttpcommandnew.h"
#include "util.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/rewardmgr.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/noticemgr.h"
#include "idip/idipcmdproc.h"
#include "idip/idiperrdesc.h"
#include "gamelogic/payv2Mgr.h"
#include "util/jsonutil.h"
#include "unit/rolemanager.h"
#include "utility/tlogger.h"


// generate by ProtoGen at date: 2016/7/16 11:48:36

RPC_SERVER_IMPLEMETION(RpcM2G_GSHttpCommandNew, GMHttpArg, GMHttpRes)

void RpcM2G_GSHttpCommandNew::OnCall(const GMHttpArg &roArg, GMHttpRes &roRes)
{
	std::map<std::string, std::string> values;
	for (int i = 0; i < roArg.keys_size(); ++i)
	{
		values[roArg.keys(i)] = roArg.values(i);
	}
	if (roArg.command() == "/idip")
	{
		CIdipReply oReply(*roRes.mutable_resultstring());
		CIdipHandler* poAsyncHandler = CIdipCmdProcess::Instance()->Process(roArg.values(0), oReply);
		if(poAsyncHandler == NULL)
		{
			roRes.set_result(oReply.m_nErrCode);
			roRes.set_errormsg(GetIdipErrDesc(oReply.m_nErrCode));
		}
		else
		{
			poAsyncHandler->SetRpcID(DelayRpc());
			CIdipCmdProcess::Instance()->AddHandler(poAsyncHandler);
		}
	}
	else
	{
		if(roArg.values_size() == 0)
		{
			roRes.set_result(404);
			roRes.set_resultstring(roArg.command() + " is not support !");
			return;
		}
	}
}

void RpcM2G_GSHttpCommandNew::OnDelayReplyRpc(const GMHttpArg &roArg, GMHttpRes &roRes, const CUserData &roUserData)
{
	if(roUserData.m_pUserPtr == NULL)
	{
		roRes.set_result(IDIP_ERR_UNKNOWN);
		roRes.set_errormsg(GetIdipErrDesc(IDIP_ERR_UNKNOWN));
	}
	else
	{
		CIdipReply* poReply = (CIdipReply*)roUserData.m_pUserPtr;
		roRes.set_resultstring(poReply->m_strResult);
		roRes.set_result(poReply->m_nErrCode);
		roRes.set_errormsg(GetIdipErrDesc(poReply->m_nErrCode)); 
	}
}
