#include "pch.h"
#include "garden/rpcc2m_gardencookingfood.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "garden/garden_config.h"
#include "role/rolesummarymgr.h"
#include "table/ProfessionMgr.h"
#include "loghelper/tlogr.h"
#include "event/eventmgr.h"
#include "common/rpcm2g_msgiveitem.h"
#include "task/roletaskmgr.h"
// generate by ProtoGen at date: 2016/10/10 14:15:05



RPC_SERVER_IMPLEMETION(RpcC2M_GardenCookingFood, GardenCookingFoodArg, GardenCookingFoodRes)

	void RpcC2M_GardenCookingFood::OnCall(const GardenCookingFoodArg &roArg, GardenCookingFoodRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	auto config = GardenConfig::Instance()->GetCooking().GetByFoodID(roArg.food_id());
	if (config == NULL)
	{
		roRes.set_result(KKSG::ERR_GARDEN_NOEXIST_FOODID);
		SSWarn<<"config is  NULL, food_id:"<<roArg.food_id()<<END;
		return;
	}
	// 检查烹饪条件
	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	// 是否激活菜谱
	auto rowdata = GardenConfig::Instance()->GetCooking().GetByFoodID(roArg.food_id());
	if (rowdata == NULL)
	{
		SSWarn<<" no this food id : "<<roArg.food_id()<<END;
		roRes.set_result(KKSG::ERR_ITEM_NOTEXIST);
		return ;
	}
	if (rowdata->CookBookID > 0)
	{	
		if (!garden->ExistCookBook(rowdata->CookBookID))
		{
			roRes.set_result(KKSG::ERR_ITEM_NOTEXIST);
			SSWarn<<"check exist cookbook err    :"<<rowdata->CookBookID<<END;
			return;
		}
	}
	// 是否满足等级等要求
	if (KKSG::ERR_SUCCESS != garden->CheckCookingCondion(roArg.food_id()))
	{
		roRes.set_result(KKSG::ERR_ITEM_LEVELLIMIT);
		SSWarn<<"CheckCookingCondion err   :"<<roArg.food_id()<<END;
		return;
	}

	// 检查职业是否满足烹饪要求
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(role->GetID());
	if (NULL == summary)
	{			
		SSWarn<<"summary is NULL, role  id:"<<role->GetID()<<END;
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);				
		return ;
	}

	UINT32 profession_id	= CProfessionMgr::Instance()->GetBasicProfession(summary->GetProfession());
	LogDebug("base profession id : %d ,and now profession id :%d",summary->GetProfession(),profession_id);
	auto row_it				= rowdata->Profession.begin();
	auto row_it_end			= rowdata->Profession.end();
	bool allow_cooking = false;
	for (; row_it != row_it_end; ++row_it)
	{
		if (profession_id == *row_it)
		{
			allow_cooking = true;
			break;
		}
	}

	if (!allow_cooking)
	{
		roRes.set_result(KKSG::ERR_GARDEN_FOOD_NOALLOW);				
		return ; 
	}

	// 检查材料 
	RpcM2G_MSConsumeItems* msg = RpcM2G_MSConsumeItems::CreateRpc();
	msg->m_oArg.set_rpc_id(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	msg->m_oArg.set_reason(ItemFlow_Garden);
	msg->m_oArg.set_subreason(ItemFlow_Garden_Fish);

	auto it = config->Ingredients.begin();
	auto it_end = config->Ingredients.end();
	for (; it != it_end; ++it)
	{
		UINT32 item_id = it->seq[0];
		UINT32 item_count = it->seq[1];
		KKSG::ItemBrief* item = msg->m_oArg.add_items();
		item->set_itemid(item_id); 
		item->set_itemcount(item_count);
	}	

	GSLink::Instance()->SendToLine(role->GetGsLine(), *msg);

}

void RpcC2M_GardenCookingFood::OnDelayReplyRpc(const GardenCookingFoodArg &roArg, GardenCookingFoodRes &roRes, const CUserData &roUserData)
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
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}	

	RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_reason(ItemFlow_Garden);
	rpc->m_oArg.set_subreason(ItemFlow_Garden_Cooking);
	KKSG::ItemBrief* item = rpc->m_oArg.add_items();
	item->set_itemid(roArg.food_id());
	item->set_itemcount(1);
	item->set_isbind(false);
	GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

	Garden* garden = GardenManager::Instance()->GetGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}
	FoodInfo pre_info;
	garden->GetGardenCookingInfo(pre_info);
	UINT32 pre_level = pre_info.cooking_level;
	UINT32 pre_exp = pre_info.cooking_experiences;

	garden->CookingFoodComplete(roArg.food_id());

	FoodInfo food_info;
	garden->GetGardenCookingInfo(food_info);
	roRes.set_cooking_level(food_info.cooking_level);
	roRes.set_cooking_experiences(food_info.cooking_experiences);
	roRes.set_result(KKSG::ERR_SUCCESS);

	if (1 == pre_level && 0 == pre_exp)
	{
		// 第一次烹饪成功
		EventMgr::Instance()->AddEvent(role->GetID(), ACH_EVENT_OFFSET+ACHIEVE_COM_GARDEN,firstCook);
	}
	if (2 == food_info.cooking_level &&  1 == pre_level)
	{
		// 第一次升级
		EventMgr::Instance()->AddEvent(role->GetID(), DESIGNATION_COM_TYPE_GARDEN,cookLevel2);
	}
	if (COOK_LEVEL_FOUR == food_info.cooking_level )
	{
		// 4级		
		EventMgr::Instance()->AddEvent(role->GetID(), DESIGNATION_COM_TYPE_GARDEN,cookLevel4);
	}

	RoleTaskMgr::Instance()->UpdateTask(role->GetID(),KKSG::TaskOper_Add,KKSG::TaskConn_Activity,KKSG::TaskActType_Cooking);

	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = role->GetID();
	oLog.m_uGardenID = garden->GetGardenID();
	oLog.m_uFoodID = roArg.food_id();
	oLog.m_uCookLevel = food_info.cooking_level;
	oLog.m_uCookExp = food_info.cooking_experiences;
	oLog.m_iOp = TX_Garden_CookingPlay;
	oLog.Do();
}
