#include "pch.h"
#include "common/rpcn2i_registerns2login.h"
#include "network/controllink.h"
#include "config.h"

// generate by ProtoGen at date: 2016/11/3 11:34:29

RPC_SERVER_IMPLEMETION(RpcN2I_RegisterNs2Login, RegisterMs2LoginArg, RegisterMs2LoginRes)

void RpcN2I_RegisterNs2Login::OnCall(const RegisterMs2LoginArg &roArg, RegisterMs2LoginRes &roRes)
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if (roArg.serverids_size())
	{
		for (int i = 0; i < roArg.serverids_size(); ++i)
		{
			nErrCode = ControlLink::Instance()->RegisterNs(m_dwConnID, roArg.zoneid(), roArg.serverids(i));
			if (nErrCode != KKSG::ERR_SUCCESS)
			{
				break;
			}
		}
	}
	else
	{
		nErrCode = ControlLink::Instance()->RegisterNs(m_dwConnID, roArg.zoneid(), roArg.serverid());
	}

	roRes.set_result(nErrCode);
	roRes.set_loginsvrid(LoginConfig::Instance()->GetServerID());
}

void RpcN2I_RegisterNs2Login::OnDelayReplyRpc(const RegisterMs2LoginArg &roArg, RegisterMs2LoginRes &roRes, const CUserData &roUserData)
{
}
