#include "pch.h"
#include "militaryrank/rpcg2m_getmilitaryrank.h"
#include "unit/rolemanager.h"
#include "gamelogic/military_rank_record.h"
#include "gamelogic/military_rank_manager.h"

// generate by ProtoGen at date: 2017/3/9 11:07:28

RPC_CLIENT_IMPLEMETION(RpcG2M_GetMilitaryRank, GetMilitaryRankArg, GetMilitaryRankRes)

void RpcG2M_GetMilitaryRank::OnReply(const GetMilitaryRankArg &roArg, const GetMilitaryRankRes &roRes, const CUserData &roUserData)
{	
	Role *role = RoleManager::Instance()->FindByRoleID(roRes.role_id());
	if (role != NULL)
	{
		UINT32 rank = role->Get<MilitaryRankRecord>()->GetMilitaryRank();
		if (roRes.military_rank() != rank )
		{
			role->Get<MilitaryRankRecord>()->SetMilitaryRrank(roRes.military_rank());			
		}		
	}
}

void RpcG2M_GetMilitaryRank::OnTimeout(const GetMilitaryRankArg &roArg, const CUserData &roUserData)
{
}
