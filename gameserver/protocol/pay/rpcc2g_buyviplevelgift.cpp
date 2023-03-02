#include "pch.h"
#include "pay/rpcc2g_buyviplevelgift.h"
#include "unit/rolemanager.h"
#include "gamelogic/payv2Mgr.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/bagtransition.h"
#include <time.h>

// generate by ProtoGen at date: 2016/8/22 19:55:43

RPC_SERVER_IMPLEMETION(RpcC2G_BuyVipLevelGift, BuyVipLevelGiftArg, BuyVipLevelGiftRes)

void RpcC2G_BuyVipLevelGift::OnCall(const BuyVipLevelGiftArg &roArg, BuyVipLevelGiftRes &roRes)
{
	//本协议是处理领取vip礼包
	roRes.set_errcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	CPayV2Record* pRecord = pRole->Get<CPayV2Record>();
	if (pRecord->GetVipLevel() < roArg.viplevel())
	{
		roRes.set_errcode(ERR_FAILED);
		SSWarn<<"vip level:"<<pRecord->GetVipLevel()<<"not enough "<<roArg.viplevel()<<END;
		return;
	}

	VIPTable::RowData* pRowData = PayV2Mgr::Instance()->GetVipConf(roArg.viplevel());
	if (NULL == pRowData)
	{
		roRes.set_errcode(ERR_FAILED);
		SSWarn<<"error vip config."<<END;
		return;
	}

	for (auto i = pRecord->vipLevelGiftInfo.begin(); i != pRecord->vipLevelGiftInfo.end(); i ++)
	{
		if (i->id() == roArg.viplevel())
		{
			roRes.set_errcode(ERR_FAILED);
			SSWarn<<"already got this vip level gift"<<END;
			return;
		}
	}
	KKSG::PayAwardRecord roInfo;
	roInfo.set_id(roArg.viplevel());
	roInfo.set_lastgetawardtime((UINT32)time(NULL));
	pRecord->vipLevelGiftInfo.push_back(roInfo);
	pRecord->m_isModified.Set();

	roRes.set_errcode(ERR_SUCCESS);
	for (auto i = pRecord->vipLevelGiftInfo.begin(); i != pRecord->vipLevelGiftInfo.end(); i ++)
	{
	   roRes.add_viplevelgift(i->id());
	}

	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Pay, ItemFlow_VipLevelGift);
	transition.GiveItem(pRowData->ItemID, 1);
	transition.NotifyClient();
}

void RpcC2G_BuyVipLevelGift::OnDelayReplyRpc(const BuyVipLevelGiftArg &roArg, BuyVipLevelGiftRes &roRes, const CUserData &roUserData)
{
}
