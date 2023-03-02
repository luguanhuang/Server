#include "pch.h"
#include "pay/rpcm2g_giftibreqgsgoods.h"
#include "utility/tlogger.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/ibshoprecord.h"
#include "gamelogic/ibshopmgr.h"

// generate by ProtoGen at date: 2017/5/5 13:37:55

RPC_SERVER_IMPLEMETION(RpcM2G_GiftIbReqGSGoods, GiftIbReqGSGoodsArg, GiftIbReqGSGoodsRes)

void RpcM2G_GiftIbReqGSGoods::OnCall(const GiftIbReqGSGoodsArg &roArg, GiftIbReqGSGoodsRes &roRes)
{
	roRes.set_error(KKSG::ERR_SUCCESS);
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.item().toroleid());
	if (pRole==NULL)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	if (pRecord->HasOrder(roArg.item().billno()))
	{
		LogError("role recv same order role=%llu,order=%s",pRole->GetID(),roArg.item().billno().c_str());
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}
	pRecord->AddOrder(roArg.item().billno(),roArg.item().ts());
	pRecord->SetSaveFlag();
	BagGiveItemTransition trans(pRole);
	trans.SetReason(ItemFlow_GiftIBItem, ItemFlow_IBItemGiveItem);
	IBShop::RowData *pRowData =   IBShopMgr::Instance()->GetByGID(roArg.item().goodsid());
	if (pRowData)
	{
		trans.GiveItem(pRowData->itemid,roArg.item().count());
	}
	pRecord->AddOrder(roArg.item().billno(),roArg.item().ts());
	pRecord->SetSaveFlag();
	trans.NotifyClient();
}

void RpcM2G_GiftIbReqGSGoods::OnDelayReplyRpc(const GiftIbReqGSGoodsArg &roArg, GiftIbReqGSGoodsRes &roRes, const CUserData &roUserData)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.item().toroleid());
	if (pRole==NULL)
		return;
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	pRecord->DelOrder(roArg.item().billno(),false);
	pRecord->SetSaveFlag();
}
