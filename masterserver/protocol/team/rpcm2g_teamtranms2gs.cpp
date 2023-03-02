#include "pch.h"
#include "team/rpcm2g_teamtranms2gs.h"
#include "team/teamtranshandler.h"

// generate by ProtoGen at date: 2017/4/25 21:36:58

RPC_CLIENT_IMPLEMETION(RpcM2G_TeamTranMs2Gs, TeamTransData, TeamTransRes)

void RpcM2G_TeamTranMs2Gs::OnReply(const TeamTransData &roArg, const TeamTransRes &roRes, const CUserData &roUserData)
{
	TeamTransHandler trans;
	switch(roArg.type())
	{
	case TEAM_TRANS_OP_REQ:
		{
			trans.HandleRoleOpResFromGs(roArg.delayid(), roArg.roleid(), roArg, roRes);
		}
		break;
	case TEAM_TRANS_TEAM_MATCH:
		{
			trans.HandleMatchResFromGs(roArg.delayid(), roArg.roleid(), roArg, roRes);
		}
		break;
	case TEAM_TRANS_ROLE_MATCH:
		{
			trans.HandleRoleMatchFromGs(roArg.delayid(), roArg.roleid(), roRes);
		}
		break;
	case TEAM_TRANS_START_BATTLE:
		{
			;//no need
		}
		break;
	case TEAM_TRANS_CHECK_EXP_CHANGE:
		{
			trans.HandleTransResFromGsToTs(roArg.delayid(), roRes);
		}
		break;
	default:
		{
			SSWarn << " not handle type = " << roArg.type() << END;
		}
		break;
	}
}

void RpcM2G_TeamTranMs2Gs::OnTimeout(const TeamTransData &roArg, const CUserData &roUserData)
{
}
