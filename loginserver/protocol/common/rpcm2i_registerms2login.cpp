#include "pch.h"
#include "common/rpcm2i_registerms2login.h"
#include "network/mslink.h"
#include "config.h"
#include "gatemgr/gatemgr.h"
#include "iplist/accountbanmgr.h"

// generate by ProtoGen at date: 2016/8/5 1:17:21

RPC_SERVER_IMPLEMETION(RpcM2I_RegisterMs2Login, RegisterMs2LoginArg, RegisterMs2LoginRes)

void RpcM2I_RegisterMs2Login::OnCall(const RegisterMs2LoginArg &roArg, RegisterMs2LoginRes &roRes)
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if (roArg.serverids_size())
	{
		for (int i = 0; i < roArg.serverids_size(); ++i)
		{
			nErrCode = MsLink::Instance()->RegisterMs(m_dwConnID, roArg.zoneid(), roArg.serverids(i));
			if (nErrCode != KKSG::ERR_SUCCESS)
			{
				break;
			}
		}
	}
	else
	{
		nErrCode = MsLink::Instance()->RegisterMs(m_dwConnID, roArg.zoneid(), roArg.serverid());
	}

	roRes.set_result(nErrCode);
	roRes.set_loginsvrid(LoginConfig::Instance()->GetServerID());

	AccountBanMgr::Instance()->SyncBanList(m_dwConnID);
	if (KKSG::ERR_SUCCESS == nErrCode)
	{
		GateMgr::Instance()->ServerInfoNotifyToMs(m_dwConnID);
	}
}

void RpcM2I_RegisterMs2Login::OnDelayReplyRpc(const RegisterMs2LoginArg &roArg, RegisterMs2LoginRes &roRes, const CUserData &roUserData)
{
}
