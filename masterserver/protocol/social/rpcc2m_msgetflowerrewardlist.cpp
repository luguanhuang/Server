#include "pch.h"
#include "social/rpcc2m_msgetflowerrewardlist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "social/rpcm2g_getflowerrewardstate.h"
#include "network/gslink.h"
#include "ranklist/flowerranklist.h"
#include "ranklist/ranklistmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/8/19 9:55:51

RPC_SERVER_IMPLEMETION(RpcC2M_MSGetFlowerRewardList, NewGetFlowerRewardListArg, NewGetFlowerRewardListRes)

#define REWARDRANKNUM 10
void RpcC2M_MSGetFlowerRewardList::OnCall(const NewGetFlowerRewardListArg &roArg, NewGetFlowerRewardListRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}


	RpcM2G_GetFlowerRewardState* rpc = RpcM2G_GetFlowerRewardState::CreateRpc();

	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());

	role->SendMsgToGS(*rpc);
}

void RpcC2M_MSGetFlowerRewardList::OnDelayReplyRpc(const NewGetFlowerRewardListArg &roArg, NewGetFlowerRewardListRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode(KKSG::ErrorCode(roUserData.m_dwUserData));
	GetFlowerRewardStateRes* poRes = (GetFlowerRewardStateRes*)roUserData.m_pUserPtr;
	if (poRes)
	{
		roRes.set_cangetreward(poRes->getstate());
		FlowerRankList* list = (FlowerRankList*)CRankListMgr::Instance()->GetRankList(KKSG::FlowerYesterdayRank);
		if (list)
		{
			std::vector<FlowerRankInfo*> vecInfo;
			list->GetSomeTopInfo(REWARDRANKNUM, vecInfo);
			for (UINT32 i = 0; i < vecInfo.size(); ++i)
			{
				FlowerRankInfo* rankinfo = vecInfo[i];
				CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(rankinfo->GetUniqueId());
				if (!summary)
				{
					continue;
				}
				KKSG::RoleBriefInfo* brief = roRes.add_brieflist();
				brief->set_type((KKSG::RoleType)summary->GetProfession());
				brief->set_roleid(rankinfo->GetUniqueId());
				brief->set_name(summary->GetName());
				brief->set_level(summary->GetLevel());
			}

		}
	}
}
