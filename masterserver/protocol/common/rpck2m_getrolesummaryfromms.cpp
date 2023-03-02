#include "pch.h"
#include "common/rpck2m_getrolesummaryfromms.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2017/2/2 21:25:22

RPC_SERVER_IMPLEMETION(RpcK2M_GetRoleSummaryFromMS, GetRoleSummaryFromMSArg, GetRoleSummaryFromMSRes)

void RpcK2M_GetRoleSummaryFromMS::OnCall(const GetRoleSummaryFromMSArg &roArg, GetRoleSummaryFromMSRes &roRes)
{
	for (int i = 0; i < roArg.roleids_size(); ++i)
	{
		UINT64 roleId = roArg.roleids(i);
		CRoleSummary* su = CRoleSummaryMgr::Instance()->GetRoleSummary(roleId);
		if (su == NULL)
		{
			continue;
		}
		roRes.add_rsus()->CopyFrom(su->GetData());
	}
}

void RpcK2M_GetRoleSummaryFromMS::OnDelayReplyRpc(const GetRoleSummaryFromMSArg &roArg, GetRoleSummaryFromMSRes &roRes, const CUserData &roUserData)
{
}
