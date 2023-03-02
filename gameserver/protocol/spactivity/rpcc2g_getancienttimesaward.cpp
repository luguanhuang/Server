#include "pch.h"
#include "spactivity/rpcc2g_getancienttimesaward.h"
#include "spactivity/spactivitymgr.h"
#include "unit/rolemanager.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivitydatasmall.h"
#include "gamelogic/bagtransition.h"
#include "spactivity/spactivitysmall.h"


// generate by ProtoGen at date: 2017/7/15 11:15:47

RPC_SERVER_IMPLEMETION(RpcC2G_GetAncientTimesAward, AncientTimesArg, AncientTimesRes)

void RpcC2G_GetAncientTimesAward::OnCall(const AncientTimesArg &roArg, AncientTimesRes &roRes)
{

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	SpActivityAncient* ancient = static_cast<SpActivityAncient*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_AncientTimes));

	if (pRole == NULL|| ancient == NULL ||roArg.pos()==0|| roArg.pos()>ancient->GetAcientTimesTableSize())
	{
		roRes.set_error(KKSG::ERR_UNKNOWN);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	SpActivityBase* pBase = SpActivityMgr::Instance()->GetSpActivity(SpActivity_AncientTimes);
	if (pBase==NULL)
	{
		roRes.set_error(KKSG::ERR_ACT_NOT_OPEN);
		return;
	}

	SpActAncientData* pAncientData = static_cast<SpActAncientData*>(pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_AncientTimes));
	if (pAncientData == NULL)
	{
		LogWarn("ArgentData is NULL, role [%llu]", pRole->GetID());
		roRes.set_error(KKSG::ERR_UNKNOWN);
		return;
	}

	UINT32 nAward = pAncientData->GetAward();
	if ((nAward&(1<<roArg.pos()))==0)
	{
		std::vector<Sequence<uint, 2>> awarditems =  ancient->GetAcientTimesAward(pRole,roArg.pos());
		if(awarditems.empty()==true)
		{
			roRes.set_error(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}
		nAward |= (1<<roArg.pos());
		roRes.set_reward(nAward);
		pAncientData->SetAward(nAward);
		BagGiveItemTransition transition(pRole);
		transition.SetReason(ItemFlow_SpActivity, ItemFlow_AncientTimes);
		for(auto i = awarditems.begin(); i != awarditems.end(); i++)
		{
			transition.GiveItem((*i)[0], (*i)[1]);
		}
		transition.NotifyClient();
		roRes.set_error(KKSG::ERR_SUCCESS);
		return;
	}
	roRes.set_error(KKSG::ERR_HAVE_GOT_AWARD);

}

void RpcC2G_GetAncientTimesAward::OnDelayReplyRpc(const AncientTimesArg &roArg, AncientTimesRes &roRes, const CUserData &roUserData)
{
}
