#include "pch.h"
#include "ranklist/ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/firstpassranklist.h"
#include "role/rolesummarymgr.h"
#include "role/rolesummary.h"
#include "firstpass/rpcg2m_mscommendfirstpass.h"

// generate by ProtoGen at date: 2016/8/24 17:31:58

RPC_SERVER_IMPLEMETION(RpcG2M_MSCommendFirstPass, MSCommendFirstPassArg, MSCommendFirstPassRes)

void RpcG2M_MSCommendFirstPass::OnCall(const MSCommendFirstPassArg &roArg, MSCommendFirstPassRes &roRes)
{
	roRes.set_error(KKSG::ERR_FAILED);
	UINT32 type = CRankListMgr::Instance()->GetFirstPassRankKey(roArg.fistpassid());
	FirstPassRankList* list = (FirstPassRankList*)CRankListMgr::Instance()->GetRankList(type, roArg.serverid());
	if (!list)
	{
		return;
	}
	/*if (!roArg.isgettopinfo())
	{
		if (!list->GetSize())
		{
			return;
		}

		list->AddCommend(1);

		roRes.set_error(KKSG::ERR_SUCCESS);
		roRes.set_commendnum(list->GetCommend());

		return;
	}
	else*/
	{
		roRes.set_error(KKSG::ERR_SUCCESS);
		auto rankInfo = list->GetRankInfo(1);
		if (!rankInfo)
		{
			return;
		}

		auto roleIDs = rankInfo->GetRoleIDs();
		for (int i = 0; i < roleIDs.size(); ++i)
		{
			CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleIDs[i]);
			if (NULL != summary)
			{
				roRes.add_roleids(roleIDs[i]);
				KKSG::UnitAppearance* info = roRes.add_infos();
				summary->Build(info, SUMMARY_ALL);
			}
		}

		roRes.set_commendnum(list->GetCommend());
		roRes.set_passtime(rankInfo->m_time);
		roRes.set_starlevel(rankInfo->m_starLevel);
		//roRes.set_error(KKSG::ERR_SUCCESS);
	}

}

void RpcG2M_MSCommendFirstPass::OnDelayReplyRpc(const MSCommendFirstPassArg &roArg, MSCommendFirstPassRes &roRes, const CUserData &roUserData)
{
}
