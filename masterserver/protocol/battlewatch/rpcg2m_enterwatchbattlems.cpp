#include "pch.h"
#include "battlewatch/rpcg2m_enterwatchbattlems.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/7/29 17:15:57

RPC_SERVER_IMPLEMETION(RpcG2M_EnterWatchBattleMs, EnterWatchBattleMsArg, EnterWatchBattleMsRes)

void RpcG2M_EnterWatchBattleMs::OnCall(const EnterWatchBattleMsArg &roArg, EnterWatchBattleMsRes &roRes)
{
	UINT32 sceneID = 0;
	UINT32 templateID = 0;
	UINT32 error = LiveManager::Instance()->EnterWatchBattle(roArg.livetype(), roArg.liveid(), sceneID, templateID);
	roRes.set_error((ErrorCode)error);
	roRes.set_sceneid(sceneID);
	roRes.set_scenetemplateid(templateID);

	UINT32 crossLine = 0;
	bool isCross = LiveManager::Instance()->IsCrossInfo(roArg.liveid(), &crossLine);
	roRes.set_istocross(isCross);
	roRes.set_crossgslineid(crossLine);
}

void RpcG2M_EnterWatchBattleMs::OnDelayReplyRpc(const EnterWatchBattleMsArg &roArg, EnterWatchBattleMsRes &roRes, const CUserData &roUserData)
{
}
