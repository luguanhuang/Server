#include "pch.h"
#include "pay/rpcc2g_payfrienditem.h"
#include "pay/rpcg2m_uppayfrienditem.h"
#include "gamelogic/ibshopmgr.h"
#include "unit/rolemanager.h"
#include "gamelogic/itemconfig.h"
#include "network/mslink.h"
#include "unit/systeminfo.h"
#include "gamelogic/rolemisc.h"

#define BUYCOUNT_LIMIT 9999
// generate by ProtoGen at date: 2017/3/14 10:25:06

RPC_SERVER_IMPLEMETION(RpcC2G_PayFriendItem, PayFriendItemArg, PayFriendItemRes)

void RpcC2G_PayFriendItem::OnCall(const PayFriendItemArg &roArg, PayFriendItemRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_ret(ERR_INVALID_REQUEST);
		return;
	}
	CRoleSystem* pRoleSys = role->Get<CRoleSystem>();
	if (pRoleSys->IsSystemOpened(SYS_IBSHOP_GIFT) == false)
	{
		roRes.set_ret(KKSG::ERR_SYS_NOTOPEN);
		return;
	}
	if (role->GetLevel() < GetGlobalConfig().IBShopLevel)
	{
		roRes.set_ret(KKSG::ERR_IBGIFT_NOT_ENOUCH);
		return;
	}

	UINT32 nCount   = roArg.count();
	if (nCount>GetGlobalConfig().IBEveryTimeMaxBuy)
	{
		roRes.set_ret(KKSG::ERR_IBGIFT_MAX_COUNT);
		return;
	}

	
	if (GetGlobalConfig().IBDayMaxBuyCount>0 &&  role->Get<RoleMisc>()->GetDayGiftItem() >= GetGlobalConfig().IBDayMaxBuyCount)
	{
		roRes.set_ret(KKSG::ERR_IBGIFT_DAY_MAXBUYCOUNT);
		return;
	}

	UINT32 nLen = (UINT32)XCommon::CountUTF8Char(roArg.text());
	if (nLen>GetGlobalConfig().IBShopGiftTextLen)
	{
		roRes.set_ret(ERR_INVALID_IBBUY);
		return;
	}


	if(GSConfig::Instance()->IsCrossGS())
	{
		roRes.set_ret(ERR_CROSS_IBBUY);
		return;
	}

	if (role->GetID()==roArg.toroleid())
	{	
		roRes.set_ret(ERR_SELF_GIFT_SELF);
		return;
	}

	IBShop::RowData* pRow = IBShopMgr::Instance()->GetByGID(roArg.goodsid());
	if (pRow == NULL||pRow->rmb==0)
	{
		roRes.set_ret(ERR_INVALID_IBBUY);
		return;
	}
	ItemConf  *pConf = ItemConfig::Instance()->GetItemConf(pRow->itemid);
	if (pConf==NULL)
	{
		roRes.set_ret(ERR_INVALID_IBBUY);
		return;
	}

	RpcG2M_UpPayFriendItem *pRpc = RpcG2M_UpPayFriendItem::CreateRpc();
	pRpc->m_oArg.set_itemid(pRow->itemid);
	pRpc->m_oArg.set_text(roArg.text());
	pRpc->m_oArg.set_count(nCount);
	pRpc->m_oArg.mutable_payparam()->CopyFrom(roArg.payparam());
	pRpc->m_oArg.set_goodsid(pRow->goodsid);
	pRpc->m_oArg.set_desc(pConf->ItemDescription);
	pRpc->m_oArg.set_name(pRow->name);
	pRpc->m_oArg.set_toroleid(roArg.toroleid());
	pRpc->m_oArg.set_fromroleid(role->GetID());	
	pRpc->m_oArg.set_price(pRow->rmb);
	pRpc->m_oArg.set_rpcid(DelayRpc());
	role->SendToMS(*pRpc);
}

void RpcC2G_PayFriendItem::OnDelayReplyRpc(const PayFriendItemArg &roArg, PayFriendItemRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == KKSG::ERR_SUCCESS && roUserData.m_pUserPtr)
	{
		KKSG::PayFriendItemRes* pInfo = (KKSG::PayFriendItemRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);

	}else
	{
		roRes.set_ret((KKSG::ErrorCode)roUserData.m_dwUserData);
	}
}
