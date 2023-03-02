#include "pch.h"
#include "ibshop/rpcc2g_buyibitem.h"
#include "unit/rolemanager.h"
#include "gamelogic/ibshopmgr.h"
// generate by ProtoGen at date: 2016/9/1 15:44:55

RPC_SERVER_IMPLEMETION(RpcC2G_BuyIBItem, IBBuyItemReq, IBBuyItemRes)

#define BUYCOUNT_LIMIT 9999

void RpcC2G_BuyIBItem::OnCall(const IBBuyItemReq &roArg, IBBuyItemRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		LogError("cross server ibbuy or pvp scene buy item roleid=%llu", role->GetID());
		roRes.set_errorcode(ERR_FAILED);
		return;
	}

	UINT32 nGoodsID = roArg.goodsid();
	UINT32 nCount   = roArg.count();
	if (nCount > BUYCOUNT_LIMIT)
	{
		roRes.set_errorcode(ERR_FAILED);
		return;
	}

	KKSG::ErrorCode  nError = IBShopMgr::Instance()->BuyItem(role, nGoodsID, nCount);
	roRes.set_errorcode(nError);
}

void RpcC2G_BuyIBItem::OnDelayReplyRpc(const IBBuyItemReq &roArg, IBBuyItemRes &roRes, const CUserData &roUserData)
{
}
