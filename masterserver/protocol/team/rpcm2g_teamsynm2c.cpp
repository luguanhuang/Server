#include "pch.h"
#include "team/rpcm2g_teamsynm2c.h"
#include "team/teammgr.h"

// generate by ProtoGen at date: 2016/8/16 20:52:03

RPC_CLIENT_IMPLEMETION(RpcM2G_TeamSynM2C, TeamSynPara, TeamSynRes)

void RpcM2G_TeamSynM2C::OnReply(const TeamSynPara &roArg, const TeamSynRes &roRes, const CUserData &roUserData)
{
	switch(roArg.type())
	{
	case STM2C_GETEXTRADATA:
		{
			TeamMgr::Instance()->SynGetExtraDataBack(roArg.teamid(), roRes);
		}
		break;
	default:
		break;
	}
}

void RpcM2G_TeamSynM2C::OnTimeout(const TeamSynPara &roArg, const CUserData &roUserData)
{
}
