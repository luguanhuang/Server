#include "pch.h"
#include "garden/rpcc2m_activecookbook.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2016/10/21 11:36:48


RPC_SERVER_IMPLEMETION(RpcC2M_ActiveCookbook, ActiveCookbookArg, ActiveCookbookRes)

	void RpcC2M_ActiveCookbook::OnCall(const ActiveCookbookArg &roArg, ActiveCookbookRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());	
	if (NULL == garden)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	if (garden->ExistCookBook(roArg.cook_book_id()))
	{
		// 无需激活
		roRes.set_result(KKSG::ERR_GARDEN_FOODBOOK_ACTIVED);
		return;
	}

	// 检查材料 
	RpcM2G_MSConsumeItems* msg = RpcM2G_MSConsumeItems::CreateRpc();
	msg->m_oArg.set_rpc_id(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	msg->m_oArg.set_reason(ItemFlow_Garden);
	msg->m_oArg.set_subreason(ItemFlow_Garden_Cooking);

	KKSG::ItemBrief* item = msg->m_oArg.add_items();
	item->set_itemid(roArg.cook_book_id()); 
	item->set_itemcount(1);	

	GSLink::Instance()->SendToLine(role->GetGsLine(), *msg);
}

void RpcC2M_ActiveCookbook::OnDelayReplyRpc(const ActiveCookbookArg &roArg, ActiveCookbookRes &roRes, const CUserData &roUserData)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if ( KKSG::ERR_SUCCESS  !=  (KKSG::ErrorCode)roUserData.m_dwUserData)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	
	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());	
	if (NULL == garden)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	garden->AddCookBook(roArg.cook_book_id());

	auto cfg = GardenConfig::Instance()->GetByCookBookID(roArg.cook_book_id());
	if (cfg == NULL)
	{
		SSWarn<<"config is  NULL, cook_book_id:"<<roArg.cook_book_id()<<END;
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	roRes.set_food_id(cfg->FoodID);
	roRes.set_result(KKSG::ERR_SUCCESS);

	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_uCookBookID = roArg.cook_book_id();
	oLog.m_uGardenID = roArg.garden_id();
	oLog.m_iOp = TX_Garden_ActiveCookBook;

	oLog.Do();
}
