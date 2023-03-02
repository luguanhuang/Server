#include "pch.h"
#include "role/rpcm2g_reqguilddarerewardrec.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"

// generate by ProtoGen at date: 2016/9/11 18:11:39

RPC_SERVER_IMPLEMETION(RpcM2G_ReqGuildDareRewardRec, ReqGuildDareRewardRecArg, ReqGuildDareRewardRecRes)

void RpcM2G_ReqGuildDareRewardRec::OnCall(const ReqGuildDareRewardRecArg &roArg, ReqGuildDareRewardRecRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());

	if (pRole != NULL)
	{
		CGuildRecord* pGuildRecord = pRole->Get<CGuildRecord>();
		const std::set<UINT32>& rewardSet = pGuildRecord->getDareReward();
		for (auto iter = rewardSet.begin(); iter != rewardSet.end(); ++iter)
		{
			roRes.add_rewardrec(*iter);
		}
	}

}

void RpcM2G_ReqGuildDareRewardRec::OnDelayReplyRpc(const ReqGuildDareRewardRecArg &roArg, ReqGuildDareRewardRecRes &roRes, const CUserData &roUserData)
{
}
