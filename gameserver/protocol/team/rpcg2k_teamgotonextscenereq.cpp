#include "pch.h"
#include "team/rpcg2k_teamgotonextscenereq.h"

// generate by ProtoGen at date: 2017/10/16 17:03:04

RPC_CLIENT_IMPLEMETION(RpcG2K_TeamGoToNextSceneReq, TeamStartBattleArg, TeamStartBattleRes)

void RpcG2K_TeamGoToNextSceneReq::OnReply(const TeamStartBattleArg &roArg, const TeamStartBattleRes &roRes, const CUserData &roUserData)
{
}

void RpcG2K_TeamGoToNextSceneReq::OnTimeout(const TeamStartBattleArg &roArg, const CUserData &roUserData)
{
}
