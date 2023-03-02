#include "pch.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "team/teammgr.h"
#include "team/team.h"

// generate by ProtoGen at date: 2016/8/18 17:31:11

RPC_CLIENT_IMPLEMETION(RpcM2G_TeamStartBattleM2G, TeamStartBattleM2GArg, TeamStartBattleM2GRes)

void RpcM2G_TeamStartBattleM2G::OnReply(const TeamStartBattleM2GArg &roArg, const TeamStartBattleM2GRes &roRes, const CUserData &roUserData)
{
	int ret = roRes.errcode();
	if((int)ERR_SUCCESS != ret)
	{
		int teamid = roArg.ssdata().teamdata().teamsyndata().teamid();
		SSInfo << " start battle failed teamid = " << teamid << " errcode " << ret << END;
		Team* pTeam = TeamMgr::Instance()->FindTeam(teamid);
		if(pTeam)
		{
			pTeam->OnStartBattleFailed();
		}
	}
}

void RpcM2G_TeamStartBattleM2G::OnTimeout(const TeamStartBattleM2GArg &roArg, const CUserData &roUserData)
{
}
