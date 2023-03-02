#include "pch.h"
#include "spactivity/rpcc2g_backflowactivityoperation.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "spactivity/spactivitydatasmall.h"
#include "spactivity/spactivityrecord.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2017/10/19 11:11:20

RPC_SERVER_IMPLEMETION(RpcC2G_BackFlowActivityOperation, BackFlowActivityOperationArg, BackFlowActivityOperationRes)

void RpcC2G_BackFlowActivityOperation::OnCall(const BackFlowActivityOperationArg &roArg, BackFlowActivityOperationRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (!GSConfig::Instance()->IsHallGS())
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
	if (backflow == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	UINT32 lastBackFlowStartTime = pRole->Get<SpActivityRecord>()->GetLastBackFlowStartTime();
	UINT32 actLeftTime = lastBackFlowStartTime + GetGlobalConfig().BackFlowActivityDayTime * 24 * 3600 - GameTime::GetTime();
	roRes.set_activitylefttime(actLeftTime);
	switch (roArg.type())
	{
	case KKSG::BackFlowAct_TreasureData:
		{
			backflow->FillTreasureData(roRes);
			roRes.set_errorcode(KKSG::ERR_SUCCESS);
			return;
		}
	case KKSG::BackFlowAct_GetTreasure:
		{
			KKSG::ErrorCode code = backflow->GetTreasure(roArg.arg());
			backflow->FillTreasureData(roRes);
			roRes.set_errorcode(code);
			return;
		}
	case KKSG::BackFlowAct_ShopData:
		{
			UINT32 leftTime = backflow->GetShopLeftTime();
			roRes.set_shoplefttime(leftTime);
			if (leftTime == 0)
			{
				roRes.set_errorcode(KKSG::ERR_BACKFLOWSHOP_SHOPCLOSE);
				return;
			}
			backflow->FillShopData(*roRes.mutable_shop());	
			roRes.set_errorcode(KKSG::ERR_SUCCESS);
			return;
		}
	case KKSG::BackFlowAct_ShopBuy:
		{
			UINT32 leftTime = backflow->GetShopLeftTime();
			roRes.set_shoplefttime(leftTime);
			if (leftTime == 0)
			{
				roRes.set_errorcode(KKSG::ERR_BACKFLOWSHOP_SHOPCLOSE);
				return;
			}
			UINT32 delayid = DelayRpc();
			bool needDelay;
			KKSG::ErrorCode code = backflow->BuyGoodInShop(roArg.arg(), delayid, needDelay);
			if (!needDelay)
			{
				CUserData data;
				data.m_dwUserData = code;
				CRpc::ReplyDelayRpc(delayid, data);
			}
			return;
		}
	case KKSG::BackFlowAct_ShopUpdate:
		{
			UINT32 leftTime = backflow->GetShopLeftTime();
			roRes.set_shoplefttime(leftTime);
			if (leftTime == 0)
			{
				roRes.set_errorcode(KKSG::ERR_BACKFLOWSHOP_SHOPCLOSE);
				return;
			}
			KKSG::ErrorCode code = backflow->FreshShop();
			roRes.set_errorcode(code);
			backflow->FillShopData(*roRes.mutable_shop());
			return;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
	}
}

void RpcC2G_BackFlowActivityOperation::OnDelayReplyRpc(const BackFlowActivityOperationArg &roArg, BackFlowActivityOperationRes &roRes, const CUserData &roUserData)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	SpActBackFlowData* backflow = static_cast<SpActBackFlowData*>(pRole->Get<SpActivityRecord>()->GetSpActData(SpActivity_BackFlow));
	if (backflow == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	backflow->FillShopData(*roRes.mutable_shop());
	roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
}
