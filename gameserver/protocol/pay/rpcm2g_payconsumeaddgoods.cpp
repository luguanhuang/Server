#include "pch.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/payv2Mgr.h"
#include "utility/tlogger.h"
#include "gamelogic/ibshopmgr.h"
#include "gamelogic/ibshoprecord.h"
#include "spactivity/spactivitysmall.h"
#include "spactivity/spactivitymgr.h"

// generate by ProtoGen at date: 2016/12/21 23:24:06

RPC_SERVER_IMPLEMETION(RpcM2G_PayConsumeAddGoods, PayConsumeAddgoodsArg, PayConsumeAddGoodsRes)

void RpcM2G_PayConsumeAddGoods::OnCall(const PayConsumeAddgoodsArg &roArg, PayConsumeAddGoodsRes &roRes)
{
	roRes.set_result(ERR_SUCCESS);
	Role* role = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == role)
	{
		LogError("Can't find role by roleid: %llu", roArg.roleid());
		roRes.set_result(ERR_FAILED);
		return;
	}
	if (!role->Get<CPayV2Record>()->AddConsumeBill(roArg.list().billno()))
	{
		LogError("roleid=%llu,repeated billno=%s", roArg.roleid(), roArg.list().billno().c_str());
		return ;
	}
	IConsumeListener* plisten = PayV2Mgr::Instance()->GetConsumeLister(roArg.list().type());
	if (plisten)
	{
		plisten->OnFinish(role, roArg.list().index(), roArg.list().count());
		role->SetFlag(RSF_ISRoleDataNeedSave, true);
	}
	std::vector<ItemDesc> items;
	for(int i = 0; i < roArg.list().item_size(); i++)
	{
		ItemDesc item;
		item.ToThis(roArg.list().item(i));
		items.push_back(item);
	}
	if (items.size())
	{
		BagGiveItemTransition trans(role);
		trans.SetReason(roArg.list().reason(), roArg.list().subreason());
		for(UINT32 i = 0; i < items.size(); i++)
		{
			trans.GiveItem(items[i]);
		}
		trans.NotifyClient();
	}
	PayV2Mgr::Instance()->NotifyDiamond(role, roArg.balance());

	TMoneyFlowLog oLog(role);
	oLog.m_nMoneyType = TX_DIAMOND;
	oLog.m_nMoneyChange = roArg.list().amt();
	oLog.m_nReason = roArg.list().reason();
	oLog.m_nSubReason = roArg.list().subreason();
	oLog.SetTransTag(roArg.list().billno());
	oLog.m_nAddOrReduce = TX_REDUCE;
	oLog.Do();

	if (roArg.list().type() == MidasConsumeType_IBShop)
	{
		IBShop::RowData *pCfg = IBShopMgr::Instance()->GetByGID(roArg.list().index());
		auto it = role->Get<IBShopRecord>()->m_mapIBShopItem.find(roArg.list().index());
		if (NULL != pCfg && it != role->Get<IBShopRecord>()->m_mapIBShopItem.end())
		{
		TShopFlow oLog(role);
		oLog.m_iShopType = TX_NORMAL_SHOP;
		oLog.m_iMoneyType = DIAMOND;
		oLog.m_iLimitFlag = it->second.nLimitCount?1:0;
		oLog.m_iMoneyValue = roArg.list().amt();
		oLog.m_iItemID = pCfg->itemid;
		oLog.m_iItemCount = roArg.list().count();
		oLog.m_iGoodsType = pCfg->type;
		oLog.Do();
		}
	}
	else if (roArg.list().type() == MidasConsumeType_BackFlowShop)
	{
		TShopFlow oLog(role);
		oLog.m_iShopType = TX_NORMAL_SHOP;
		oLog.m_iMoneyType = DIAMOND;
		oLog.m_iMoneyValue = roArg.list().amt();
		SpActivityBackFlow* backflow = static_cast<SpActivityBackFlow*>(SpActivityMgr::Instance()->GetSpActivity(SpActivity_BackFlow));
		if (backflow)
		{
			auto pConf = backflow->GetBackflowShopConf(roArg.list().count());
			if (pConf)
			{
				oLog.m_iItemID = pConf->ItemID;
				oLog.m_iItemCount = pConf->ItemCount;
			}
		}
		oLog.Do();
	}
	if (roArg.list().subreason() != ItemFlow_TeamCostTake && roArg.list().subreason() != ItemFlow_IDIP_Oper)
	{
		role->Get<CPayV2Record>()->AddScore(roArg.list().amt());
	}
}

void RpcM2G_PayConsumeAddGoods::OnDelayReplyRpc(const PayConsumeAddgoodsArg &roArg, PayConsumeAddGoodsRes &roRes, const CUserData &roUserData)
{
}
