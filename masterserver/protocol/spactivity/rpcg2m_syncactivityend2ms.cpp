#include "pch.h"
#include "spactivity/rpcg2m_syncactivityend2ms.h"
#include "define/spactivityiddef.h"
#include "ranklist/ranklist.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/flowerranklist.h"
#include "spactivity/spactivitymgr.h"
#include "social/socialmgr.h"
#include "ranklist/biohelllrankList.h"

// generate by ProtoGen at date: 2017/2/23 11:36:44

RPC_SERVER_IMPLEMETION(RpcG2M_SyncActivityEnd2MS, SyncActivityEnd2MSArg, SyncActivityEnd2MSRes)

void RpcG2M_SyncActivityEnd2MS::OnCall(const SyncActivityEnd2MSArg &roArg, SyncActivityEnd2MSRes &roRes)
{
	roRes.set_error(KKSG::ERR_FAILED);

	if (m_oArg.actid())
	{
		if (m_oArg.actid() == SpActivity_ValentinesId)
		{
			CRankList* ranklist = CRankListMgr::Instance()->GetRankList(KKSG::FlowerThisWeekRank);
			if (!ranklist)
			{
				return;
			}

			((FlowerRankList*)ranklist)->SendValentinesDayReward();
		}
		else if (m_oArg.actid() == SpActivity_520flowerId)
		{
			CRankList* ranklist = CRankListMgr::Instance()->GetRankList(KKSG::FlowerRank);
			if (!ranklist)
			{
				return;
			}

			((FlowerRankList*)ranklist)->Send520DayReward();
		}
		else if (m_oArg.actid() == SpActivity_FlowerId)
		{
			SocialMgr::Instance()->ClearFlowerActivityInfo();
		}
		else if (m_oArg.actid() == SpActivity_BioHell)
		{
			BioHelllRankList* pCurRankList = (BioHelllRankList*)CRankListMgr::Instance()->GetRankList(KKSG::BioHelllRank);
			if (pCurRankList!=NULL)
			{
				pCurRankList->GiveReward();
			}
		}

		roRes.set_error(KKSG::ERR_SUCCESS);

		LogInfo("sync spactivity id:%u success.", m_oArg.actid());
	}
	else
	{
		SpActivityMgr::Instance()->SyncActivityData(roArg);
	}

	return;
}

void RpcG2M_SyncActivityEnd2MS::OnDelayReplyRpc(const SyncActivityEnd2MSArg &roArg, SyncActivityEnd2MSRes &roRes, const CUserData &roUserData)
{
}
