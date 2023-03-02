#include "pch.h"
#include "unit/rolemanager.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivitymgr.h"
#include "spactivity/spactivitysmall.h"
#include "spactivity/rpcc2g_buydraw.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bag.h"
#include "gamelogic/midas.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/9/12 16:03:13

RPC_SERVER_IMPLEMETION(RpcC2G_BuyDraw, BuyDrawReq, BuyDrawRes)

void RpcC2G_BuyDraw::OnCall(const BuyDrawReq &roArg, BuyDrawRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (GSConfig::Instance()->IsCrossGS())
	{
		LogError("cross server or pvp scene buy draw roleid=%llu", pRole->GetID());
		roRes.set_errorcode(ERR_FAILED);
		return;
	}
	SpActivityBase* pBase = SpActivityMgr::Instance()->GetSpActivity(SpActivity_LuckyTurntable);
	if (pBase == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ACT_NOT_OPEN);
		return;
	}
	if (pRole->Get<RoleMisc>()->GetIsPay() == true)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	UINT32 state = pRole->Get<RoleMisc>()->GetPriceState();
	UINT32 price = 0;
	UINT32 consumeitemid = 0;
	KKSG::ErrorCode result = static_cast<SpActivityLuckyTurntable*>(pBase)->GetPriceAndConsume(state, price, consumeitemid);
	if (result != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode(result);
		return;
	}
	if (consumeitemid == DIAMOND)
	{
		UINT32 nTime = TimeUtil::GetTime();
		UINT32 nLockTime = pRole->GetLockTime();
		if (nTime < nLockTime)
		{
			LogError("luckyactivity，midas ---roleid=%llu",pRole->GetID());
			roRes.set_errorcode(KKSG::ERR_COMMENDWATCH_COUNTLIMIT);
			return;
		}
		UINT32 delayid = DelayRpc();
		KKSG::PayConsumeBase info;
		info.set_type((int)MidasConsumeType_LotteryDraw);
		info.set_amt(price);
		info.set_subreason(ItemFlow_Lottory_Draw);
		info.set_reason(ItemFlow_Lottory);
		info.set_index(delayid);
		info.set_count(0);
		std::vector<ItemDesc> vecItem;
		if(!pRole->ConsumeDiamond(info,vecItem))
		{
			CUserData data;
			data.m_dwUserData = KKSG::ERR_IBSHOP_LACKDIAMOND;
			ReplyDelayRpc(delayid, data);
		}
		pRole->SetLockTime();
	}
	else if (consumeitemid != 0)
	{
		UINT64 nCount;
		if (XItem::IsVirtualItem(consumeitemid))
		{
			nCount = pRole->Get<Bag>()->CountVirtualItem(consumeitemid);
		}
		else
		{
			nCount = pRole->Get<Bag>()->CountItemInBag(consumeitemid);
		}
		if (nCount < price)
		{
			roRes.set_errorcode(KKSG::ERR_SHOP_LACKMONEY);
			return;
		}
		BagTakeItemTransition stTakeTransition(pRole);
		stTakeTransition.SetReason(ItemFlow_Lottory, ItemFlow_Lottory_Draw);
		if(stTakeTransition.TakeItem(consumeitemid, (int)price) == false)
		{
			stTakeTransition.RollBack();
			roRes.set_errorcode(KKSG::ERR_SHOP_LACKMONEY);
			return;
		}
		stTakeTransition.NotifyClient();
		pRole->Get<RoleMisc>()->UpdateIsPay(true);
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
	}
}

void RpcC2G_BuyDraw::OnDelayReplyRpc(const BuyDrawReq &roArg, BuyDrawRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode((KKSG::ErrorCode)(roUserData.m_dwUserData));
}
