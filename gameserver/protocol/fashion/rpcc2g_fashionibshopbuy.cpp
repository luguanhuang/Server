#include "pch.h"
#include "fashion/rpcc2g_fashionibshopbuy.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/ibshopmgr.h"
#include "gamelogic/rolefashion.h"
#include "util/gametime.h"
// generate by ProtoGen at date: 2017/9/11 14:17:51
#define BUYCOUNT_LIMIT 9999

RPC_SERVER_IMPLEMETION(RpcC2G_FashionIBShopBuy, FashionIBShopBuyArg, FashionIBShopBuyRes)

	void RpcC2G_FashionIBShopBuy::OnCall(const FashionIBShopBuyArg &roArg, FashionIBShopBuyRes &roRes)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	if (GSConfig::Instance()->IsCrossGS())
	{
		LogError("cross server ibbuy or pvp scene buy item roleid=%llu", role->GetID());
		roRes.set_result(ERR_FAILED);
		return;
	}

	UINT32  FashinSynthesisBuyCD = GetGlobalConfig().GetInt("FashinSynthesisBuyCD" , 2);
	UINT32 last_time = role->Get<RoleFashion>()->GetFashionIBShopBuyTime();
	UINT32 now_time = (UINT32)GameTime::GetTime();
	if (now_time - last_time <  FashinSynthesisBuyCD)
	{
		SSWarn<<" too quick "<<END;
		roRes.set_result(ERR_WEDDING_OPER_CD);
		return;
	}
	for (UINT32 index = 0; index <  (UINT32)roArg.item_size(); ++index)
	{
		UINT32 item_id = roArg.item(index).itemid();		
		auto row_info = IBShopMgr::Instance()->GetConfsByItemID(item_id);
		if (row_info == NULL)
		{
			roRes.set_result(KKSG::ERR_IBSHOP_LACKGOODS);
			return;
		}

		UINT32 nGoodsID  = 0;
		for (auto it = row_info->begin(); it != row_info->end(); ++it)
		{
			nGoodsID = (*it)->id ;			
		}
		UINT32 nCount   = roArg.item(index).itemcount();
		if (nCount > BUYCOUNT_LIMIT)
		{
			roRes.set_result(ERR_FAILED);
			return;
		}
		if (nCount < 1)
		{
			continue;
		}

		KKSG::ErrorCode  nError = IBShopMgr::Instance()->BuyItem(role, nGoodsID, nCount);
		if (nError != KKSG::ERR_SUCCESS)
		{
			roRes.set_result(nError);
			return;
		}
	}

	role->Get<RoleFashion>()->SetFashionIBShopBuyTime(now_time);
}

void RpcC2G_FashionIBShopBuy::OnDelayReplyRpc(const FashionIBShopBuyArg &roArg, FashionIBShopBuyRes &roRes, const CUserData &roUserData)
{
}
