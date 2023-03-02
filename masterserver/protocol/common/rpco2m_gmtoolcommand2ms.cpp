#include "pch.h"
#include "common/rpco2m_gmtoolcommand2ms.h"
#include "utility/reloadmgr.h"

// generate by ProtoGen at date: 2016/8/27 10:56:34

RPC_SERVER_IMPLEMETION(RpcO2M_GMToolCommand2Ms, GMToolCommandArg, GMToolCommandRes)

void RpcO2M_GMToolCommand2Ms::OnCall(const GMToolCommandArg &roArg, GMToolCommandRes &roRes)
{
	if(roArg.cmd().compare("reload") != 0)
	{
		LogWarn("[MsReload] Invalid gmtool command: %s", roArg.cmd().c_str());
		roRes.set_result(false);
		return;	
	}
	LogInfo("Start reload table %s", roArg.args().c_str());
	UINT32 delayid = DelayRpc();
	ReloadErrorCode ret = CReloadMgr::Instance()->ReloadNew(roArg.type(), roArg.args(), *roRes.mutable_errmsg(), delayid);
	if(ret == Reload_Succeed || ret == Reload_Failed)
	{
		if(ret == Reload_Succeed) roRes.set_result(true);
		else roRes.set_result(false);
		CUserData data;
		ReplyDelayRpc(delayid, data);
		return;
	}
	if(ret == Reload_DelayAndSucceed) roRes.set_result(true);
	else roRes.set_result(false);
}

void RpcO2M_GMToolCommand2Ms::OnDelayReplyRpc(const GMToolCommandArg &roArg, GMToolCommandRes &roRes, const CUserData &roUserData)
{
	ReloadData* data = (ReloadData*)roUserData.m_pUserPtr;
	if(data == NULL)
	{
		return;
	}
	bool result = roRes.result();
	std::string strErrMsg = roRes.errmsg();
	for(auto i = data->replyData.begin(); i != data->replyData.end(); i++)
	{
		result &= (*i).result();
		strErrMsg += (*i).errmsg() + "\n";
	}
	roRes.set_result(result);
	roRes.set_errmsg(strErrMsg);
}
