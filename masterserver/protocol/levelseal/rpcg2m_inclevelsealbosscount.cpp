#include "pch.h"
#include "levelseal/rpcg2m_inclevelsealbosscount.h"
#include "levelseal/levelsealMgr.h"

// generate by ProtoGen at date: 2016/7/28 18:30:04

RPC_SERVER_IMPLEMETION(RpcG2M_IncLevelSealBossCount, IncBossCountArg, IncBossCountRes)

void RpcG2M_IncLevelSealBossCount::OnCall(const IncBossCountArg &roArg, IncBossCountRes &roRes)
{
	UINT32 uType = roArg.type();
	UINT32 uCount = roArg.collectcount();
	roRes.set_result(ERR_UNKNOWN);
	if (uType == CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType)
	{
		//CLevelSealMgr::Instance()->SetBossCount(uCount);
		CLevelSealMgr::Instance()->SetCollectCount(uCount);
		roRes.set_result(ERR_SUCCESS);
	}
}

void RpcG2M_IncLevelSealBossCount::OnDelayReplyRpc(const IncBossCountArg &roArg, IncBossCountRes &roRes, const CUserData &roUserData)
{
}
