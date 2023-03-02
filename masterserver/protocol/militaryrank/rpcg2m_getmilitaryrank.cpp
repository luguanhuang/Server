#include "pch.h"
#include "militaryrank/rpcg2m_getmilitaryrank.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "ranklist/military_ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/ranklist.h"

// generate by ProtoGen at date: 2017/3/9 11:07:28

RPC_SERVER_IMPLEMETION(RpcG2M_GetMilitaryRank, GetMilitaryRankArg, GetMilitaryRankRes)

void RpcG2M_GetMilitaryRank::OnCall(const GetMilitaryRankArg &roArg, GetMilitaryRankRes &roRes)
{
	MilitaryRankList* ranklist = static_cast<MilitaryRankList*>(CRankListMgr::Instance()->GetRankList(KKSG::MilitaryRank));
	UINT32 rank = ranklist->GetMilitaryInfoByRoleId(roArg.role_id());
	roRes.set_military_rank(rank);
	roRes.set_role_id(roArg.role_id());
}

void RpcG2M_GetMilitaryRank::OnDelayReplyRpc(const GetMilitaryRankArg &roArg, GetMilitaryRankRes &roRes, const CUserData &roUserData)
{
}
