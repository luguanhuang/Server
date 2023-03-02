#include "pch.h"
#include "garden/rpcc2m_gardenexpelsprite.h"
#include "garden/garden_manager.h"
#include "role/rolemanager.h"
#include "garden/garden_utility.h"
#include "define/tlogenum.h"
#include "network/gslink.h"
#include "loghelper/tlogr.h"
#include "common/rpcm2g_msgiveitem.h"

// generate by ProtoGen at date: 2016/10/24 12:30:20

RPC_SERVER_IMPLEMETION(RpcC2M_GardenExpelSprite, GardenExpelSpriteArg, GardenExpelSpriteRes)

	void RpcC2M_GardenExpelSprite::OnCall(const GardenExpelSpriteArg &roArg, GardenExpelSpriteRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT32  EXPEL_SPRITE_NUM = GetGlobalConfig().GetInt("GardenExpelSpriteTimes" , 10);

	Garden* garden = NULL;
	GuildGardenPlant* guild_plant = NULL;
	UINT32 result = 0;
	UINT64 garden_id = role->GetID();
	std::string garden_name = role->GetName();
	if (roArg.quest_type() == KKSG::MYSELF)
	{
		garden = GardenUtility::Instance()->GetMyGarden(garden_id,garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
	}
	else if (roArg.quest_type() == KKSG::FRIEND)
	{
		garden = GardenUtility::Instance()->GetFriendGarden(roArg.garden_id(),garden_name);
		if (NULL == garden)
		{
			LogWarn("not find the garden,role id is : %llu",roArg.garden_id());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
	}
	else if (roArg.quest_type() == KKSG::GUILD)
	{
		GuildGarden* guild_garden = GardenUtility::Instance()->GetGuildGarden(roArg.garden_id());
		if (NULL == guild_garden)
		{
			LogWarn("not find the guild garden,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}
		guild_plant =  guild_garden->GetGuildPlant();
		if (NULL == guild_plant)
		{
			LogWarn("not find the guild garden plant info ,role id is : %llu",role->GetID());
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return ;
		}
	}

	std::vector<ItemDesc> award_items; 
	if (roArg.quest_type() == KKSG::GUILD)
	{
		result = guild_plant->ExpelSprite(role->GetID(),role->GetName(),roArg.sprite_id(),award_items);
	}
	else
	{
		if (role->GetMapID() == FAMILYGARDEN_MAP_ID)
		{
			result = garden->ExpelSprite(role->GetID(),role->GetName(),roArg.sprite_id(),award_items);					
		}	
		else
		{
			result = KKSG::ERR_INVFIGHT_ME_SCENE;
		}
	}

	if (result == KKSG::ERR_SUCCESS)
	{
		bool gieve_item = true;
		if (garden != NULL && roArg.quest_type() == KKSG::MYSELF)
		{
			if (garden->GetExpelspriteTimes() >= EXPEL_SPRITE_NUM )
			{
				gieve_item = false;
			}
			garden->AddExpelspriteTimes();
		}
		else if (roArg.quest_type() == KKSG::FRIEND)
		{
			UINT64  mygarden_id = role->GetID();
			std::string mygarden_name = role->GetName();
			auto mygarden = GardenUtility::Instance()->GetMyGarden(mygarden_id,mygarden_name);
			if (NULL != mygarden)
			{
				if (mygarden->GetExpelspriteTimes() >= EXPEL_SPRITE_NUM )
				{
					gieve_item = false;
				}
				mygarden->AddExpelspriteTimes();
			}
		}

		if (gieve_item)
		{
			// 奖励发放
			RpcM2G_MsGiveItem* rpc = RpcM2G_MsGiveItem::CreateRpc();
			rpc->m_oArg.set_roleid(role->GetID());
			rpc->m_oArg.set_reason(ItemFlow_Garden);
			rpc->m_oArg.set_subreason(ItemFlow_Garden_PlantSeed);

			auto it = award_items.begin();
			auto it_end = award_items.end();
			for (; it != it_end; ++it)
			{
				KKSG::ItemBrief* itme  = roRes.add_items();
				itme->set_itemid(it->itemID);
				itme->set_itemcount((UINT32)it->itemCount);

				// 发给gs的数据
				KKSG::ItemBrief* item = rpc->m_oArg.add_items();
				item->set_itemid(it->itemID);
				item->set_itemcount((UINT32)it->itemCount);
			}

			GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);
		}
		else
		{
			result = KKSG::ERR_GARDEN_EXPELSPRITE_MAX;			
		}

		TGardenFlow oLog(TLogMgr::Instance()->GetTagString());
		oLog.m_uRoleID = role->GetID();
		oLog.m_uGardenID = roArg.garden_id();
		oLog.m_iOp = Tx_Garden_ExpelSprite;
		oLog.m_iQuestType = roArg.quest_type();
		oLog.Do();
	}

	roRes.set_result((KKSG::ErrorCode)result);
}

void RpcC2M_GardenExpelSprite::OnDelayReplyRpc(const GardenExpelSpriteArg &roArg, GardenExpelSpriteRes &roRes, const CUserData &roUserData)
{
}
