#include "pch.h"
#include "survive/rpcg2m_survivesynpointg2m.h"
#include "survive/survivemgr.h"
#include "ranklist/surviveranklist.h"

// generate by ProtoGen at date: 2017/9/21 15:34:54

RPC_SERVER_IMPLEMETION(RpcG2M_SurviveSynPointG2M, SurviveSynPointArg, SurviveSynPointRes)

void RpcG2M_SurviveSynPointG2M::OnCall(const SurviveSynPointArg &roArg, SurviveSynPointRes &roRes)
{
	SurviveRankList* pList = CSurviveMgr::Instance()->GetRankList();
	if(pList)
	{
		for(int i = 0; i < m_oArg.roles_size(); ++i)
		{
			pList->OnChange(m_oArg.roles(i));
		}
	}
}

void RpcG2M_SurviveSynPointG2M::OnDelayReplyRpc(const SurviveSynPointArg &roArg, SurviveSynPointRes &roRes, const CUserData &roUserData)
{
}
