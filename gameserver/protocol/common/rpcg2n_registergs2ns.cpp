#include "pch.h"
#include "common/rpcg2n_registergs2ns.h"
#include "network/controllink.h"
#include "reconnetmgr.h"
#include "network/gatelink.h"
#include "config.h"

// generate by ProtoGen at date: 2016/11/2 1:16:28

RPC_CLIENT_IMPLEMETION(RpcG2N_RegisterGs2Ns, RegisterGs2NsArg, RegisterGs2NsRes)

void RpcG2N_RegisterGs2Ns::OnReply(const RegisterGs2NsArg &roArg, const RegisterGs2NsRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() == KKSG::ERR_SUCCESS)
	{
		LogInfo("Game server line [%u] register to ns success", roArg.line());
		ControlLink::Instance()->SetRegistered(true);
		//GSConfig::Instance()->SetAppType(roRes.apptype());
		//GSConfig::Instance()->SetGameServerOpenTimeStamp(roRes.openservertime());

		if(roRes.linelist_size() != roRes.addrlist_size())
		{
			LogError("line size, addr size is invalid");
			return;
		}

		for(INT32 i = 0; i < roRes.linelist_size(); ++i)
		{
			if(GatetLink::Instance()->IsLineConnected(roRes.linelist(i))) continue;
			if(GatetLink::Instance()->IsInSet(roRes.linelist(i))) continue;

			PeerInfo peer;
			peer.m_action = NA_CONNECT;
			peer.m_state = PS_CONNECT_FAILED;
			peer.m_address.m_ip = roRes.addrlist(i).ip();
			peer.m_address.m_port = roRes.addrlist(i).port();
			peer.m_handler = GatetLink::Instance()->GetName();
			peer.m_dwSendBuffSize = 1024 * 1024 * 4;
			peer.m_dwRecvBuffSize = 1024 * 1024 * 4;
			ReconnectManager::Instance()->AddPeerInfo(peer);
			GatetLink::Instance()->AddToSet(roRes.linelist(i));
		}
	}
	else
	{
		LogError("Game server line [%u] register to ns failed", roArg.line());
	}
}

void RpcG2N_RegisterGs2Ns::OnTimeout(const RegisterGs2NsArg &roArg, const CUserData &roUserData)
{
	LogError("Game server line [%u] register to ns timeout", roArg.line());
}
