#include "pch.h"
#include "common/rpcg2n_registergs2ns.h"
#include "network/gatelink.h"
#include "network/gslink.h"
#include "config.h"
#include "common/ptcn2g_updatefunctionopen2gs.h"
#include "config.h"

// generate by ProtoGen at date: 2016/11/2 1:16:28

RPC_SERVER_IMPLEMETION(RpcG2N_RegisterGs2Ns, RegisterGs2NsArg, RegisterGs2NsRes)

void RpcG2N_RegisterGs2Ns::OnCall(const RegisterGs2NsArg &roArg, RegisterGs2NsRes &roRes)
{
	KKSG::ErrorCode nErrCode = GSLink::Instance()->AddGameServer(GetConnID(), roArg.serverid(), roArg.line());
	roRes.set_result(nErrCode);

	//roRes.set_apptype(MSConfig::Instance()->GetAppType());
	//roRes.set_openservertime(MSConfig::Instance()->GetGameServerOpenTimeStamp());

	const std::map<UINT32, GateServerInfo>& roGateMap = GateLink::Instance()->GetGateInfoMap();
	for(std::map<UINT32, GateServerInfo>::const_iterator it = roGateMap.begin(); it != roGateMap.end(); ++it)
	{
		roRes.add_linelist(it->second.dwLine);
		KKSG::ListenAddress* poAddr = roRes.add_addrlist();
		poAddr->set_ip(it->second.listenIP);
		poAddr->set_port(it->second.listenPort);
	}
	roRes.set_backflowlevel(ControlConfig::Instance()->GetBackFlowLevel());


	/*
	PtcN2G_UpdateFunctionOpen2Gs ntf;
	ntf.m_Data.set_flag(FunctionOpenMgr::Instance()->GetOpenFlag());
	GSLink::Instance()->SendToLine(roArg.line(), ntf);
	*/
}

void RpcG2N_RegisterGs2Ns::OnDelayReplyRpc(const RegisterGs2NsArg &roArg, RegisterGs2NsRes &roRes, const CUserData &roUserData)
{
}
