#include "pch.h"
#include "levelseal/rpcg2m_inclevelsealbosscount.h"
#include "gamelogic/levelsealMgr.h"

// generate by ProtoGen at date: 2016/7/28 18:30:04

RPC_CLIENT_IMPLEMETION(RpcG2M_IncLevelSealBossCount, IncBossCountArg, IncBossCountRes)

void RpcG2M_IncLevelSealBossCount::OnReply(const IncBossCountArg &roArg, const IncBossCountRes &roRes, const CUserData &roUserData)
{
	if (roRes.result() == ERR_SUCCESS)
	{
		UINT32 uCount = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uIncCollectCount > roArg.collectcount() ? CLevelSealMgr::Instance()->GetLevelSealInfo().m_uIncCollectCount - roArg.collectcount() : 0;
		CLevelSealMgr::Instance()->SetCollectCount(uCount);
	}
}

void RpcG2M_IncLevelSealBossCount::OnTimeout(const IncBossCountArg &roArg, const CUserData &roUserData)
{
}
