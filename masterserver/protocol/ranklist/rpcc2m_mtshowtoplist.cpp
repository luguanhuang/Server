#include "pch.h"
#include "ranklist/rpcc2m_mtshowtoplist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/ranklist.h"
#include "ranklist/flowerranklist.h"


// generate by ProtoGen at date: 2016/9/22 16:09:14

RPC_SERVER_IMPLEMETION(RpcC2M_MTShowTopList, TShowTopListArg, TShowTopListRes)

void RpcC2M_MTShowTopList::OnCall(const TShowTopListArg &roArg, TShowTopListRes &roRes)
{
	//CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	//if (pRole == NULL)
	//{
	//	roRes.set_error(KKSG::ERR_FAILED);
	//	return;
	//}
	//
	//UINT32 count;
	//if (roArg.type() == 1) 
	//{
	//	count = 5;
	//}
	//else
	//{
	//	count = 10;
	//}

	////CRankList* list = CRankListMgr::Instance()->GetRankList(TShowVoteCountRank);
	//CRankList* list = CRankListMgr::Instance()->GetRankList(FlowerTotalRank);
	//if (!list)
	//{
	//	roRes.set_error(KKSG::ERR_FAILED);
	//	return;
	//}
	//FlowerRankList* flowerList = (FlowerRankList*)list;

	//std::vector<FlowerRankInfo*> vecInfo;
	//flowerList->GetSomeTopInfo(count, vecInfo);
	//UINT32 vecSize = vecInfo.size();
	//for (UINT32 i = 0; i < vecSize; i++)
	//{
	//	FlowerRankInfo* rankinfo = vecInfo[i];
	//	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(rankinfo->GetUniqueId());
	//	if (!summary)
	//	{
	//		continue;
	//	}
	//	KKSG::RoleBriefInfo* brief = roRes.add_rolesinfo();
	//	brief->set_type((KKSG::RoleType)summary->GetProfession());
	//	brief->set_roleid(rankinfo->GetUniqueId());
	//	brief->set_name(summary->GetName());
	//	brief->set_level(summary->GetLevel());
	//}

	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcC2M_MTShowTopList::OnDelayReplyRpc(const TShowTopListArg &roArg, TShowTopListRes &roRes, const CUserData &roUserData)
{
}
