#include "pch.h"
#include "garden/rpcc2m_tryfish.h"
#include "common/rpcm2g_msconsumeitems.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "define/tlogenum.h"
#include "garden/garden_utility.h"
#include "loghelper/tlogr.h"
#include "event/eventmgr.h"
#include "common/rpcm2g_msgiveitem.h"
#include "task/roletaskmgr.h"

// generate by ProtoGen at date: 2016/10/4 10:42:16

RPC_SERVER_IMPLEMETION(RpcC2M_TryFish, TryFishArg, TryFishRes)

	void RpcC2M_TryFish::OnCall(const TryFishArg &roArg, TryFishRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}	
	if (roArg.quest_type() == KKSG::GUILD )
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<" err quest type "<<roArg.quest_type()<<END;
		return;
	}

	// 不在家园不能钓鱼
	if (role->GetMapID() != FAMILYGARDEN_MAP_ID)
	{
		roRes.set_result(KKSG::ERR_INVFIGHT_ME_SCENE);
		return;		
	}	

	// 减少一个鱼饵
	RpcM2G_MSConsumeItems* msg = RpcM2G_MSConsumeItems::CreateRpc();
	msg->m_oArg.set_rpc_id(DelayRpc());
	msg->m_oArg.set_role_id(role->GetID());
	msg->m_oArg.set_reason(ItemFlow_Garden);
	msg->m_oArg.set_subreason(ItemFlow_Garden_Fish);
	KKSG::ItemBrief* item = msg->m_oArg.add_items();
	item->set_itemid(FISH_STOSH_ID); // 鱼饵的id
	item->set_itemcount(1);

	GSLink::Instance()->SendToLine(role->GetGsLine(), *msg);	
}

void RpcC2M_TryFish::OnDelayReplyRpc(const TryFishArg &roArg, TryFishRes &roRes, const CUserData &roUserData)
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

	Garden* garden  = GardenUtility::Instance()->GetMyGarden(role->GetID(),role->GetName());
	if (NULL == garden)
	{
		LogWarn("not find the garden,role id is : %llu",role->GetID());
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	UINT64 garden_id = role->GetID();
	if (role->GetID() != roArg.garden_id())
	{
		garden_id = roArg.garden_id();
	}
	// 钓前数据
	FishInfoStore& pre_info = garden->GetFishInfo();
	UINT32 pre_level = pre_info.fish_level;
	UINT32 pre_exp = pre_info.experiences;
	bool pre_quality = pre_info.quality_four;

	std::vector<ItemDesc> items;
	garden->TryFishing(items);

	RoleTaskMgr::Instance()->UpdateTask(role->GetID(),KKSG::TaskOper_Add,KKSG::TaskConn_Activity, KKSG::TaskActType_Fish);

	// 钓后数据
	FishInfoStore& fish_info = garden->GetFishInfo();
	roRes.set_fish_level(fish_info.fish_level);	
	roRes.set_experiences(fish_info.experiences);

	// 通知钓鱼结果		
	KKSG::GardenPlayStageChange arg;					
	arg.set_event_type(KKSG::FISH_RESULT);	
	arg.set_role_id(role->GetID());
	if (items.size() > 0)
	{
		arg.set_exist(true);
		// 钓前如果为空，则表示钓到了第一条
		if (1 == pre_level && 0 == pre_exp)
		{
			// 钓到第一条鱼
			EventMgr::Instance()->AddEvent(role->GetID(), ACH_EVENT_OFFSET+ACHIEVE_COM_GARDEN,firstFish);
		}
		auto item = items.at(0);
		auto  fish  = GardenConfig::Instance()->GetFishInfo().GetByFishID(item.itemID);
		// 第一次钓到4品质的鱼
		if (!pre_quality && QUALITY_FOUR ==  fish->quality)
		{
			//  第一次钓到4品质的鱼
			fish_info.quality_four = true;
			EventMgr::Instance()->AddEvent(role->GetID(), ACH_EVENT_OFFSET+ACHIEVE_COM_GARDEN,firstFourQuality);
		}

		if (1 == pre_level && 2 == fish_info.fish_level)
		{
			EventMgr::Instance()->AddEvent(role->GetID(), DESIGNATION_COM_TYPE_GARDEN,fishLevel2);
		}
		// 升至5级的称号
		if (FISH_LEVEL_FOUR == pre_level && FISH_LEVEL_FIVE == fish_info.fish_level)
		{
			EventMgr::Instance()->AddEvent(role->GetID(), DESIGNATION_COM_TYPE_GARDEN,fishLevel5);
		}
	}
	else
	{
		arg.set_exist(false);
	}
	GardenUtility::Instance()->NotifyEvnetChangeNtf(arg,garden_id);

	// 将钓的鱼发给gs
	RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_reason(ItemFlow_Garden);
	rpc->m_oArg.set_subreason(ItemFlow_Garden_Fish);

	auto it = items.begin();
	auto it_end = items.end();
	for (;it != it_end;++it)
	{	
		// 发给gs的数据
		KKSG::ItemBrief* item = rpc->m_oArg.add_items();
		item->set_itemid(it->itemID);
		item->set_itemcount(it->itemCount);
		item->set_isbind(it->isbind);
		// 回复客户端的数据
		KKSG::ItemBrief* item_brief = roRes.add_item();
		item_brief->set_itemid(it->itemID);
		item_brief->set_itemcount(it->itemCount);
		item_brief->set_isbind(it->isbind);
	}

	GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);

	roRes.set_result(KKSG::ERR_SUCCESS);

	TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uFishLevel = fish_info.fish_level;
	oLog.m_uFishExp = fish_info.experiences;
	oLog.m_uRoleID = role->GetID();
	oLog.m_iOp = TX_Garden_FishPlay;
	oLog.m_iResult = items.size()?0:1;
	oLog.Do();
}
