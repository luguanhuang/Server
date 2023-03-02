#include "pch.h"
#include "guild/rpcc2g_guildpartysummonspirit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/guildcampparty.h"
#include "scene/scene.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/midas.h"

// generate by ProtoGen at date: 2017/10/10 10:23:06

RPC_SERVER_IMPLEMETION(RpcC2G_GuildPartySummonSpirit, GuildPartySummonSpiritArg, GuildPartySummonSpiritRes)

	void RpcC2G_GuildPartySummonSpirit::OnCall(const GuildPartySummonSpiritArg &roArg, GuildPartySummonSpiritRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);	
	if (NULL == role  )
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;	
		return;
	}

	KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(role);
	if ( result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(result);
		return ;
	}

	Scene *scene = role->GetCurrScene();
	if (NULL == scene)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	// 还有没有时间
	if (!CGuildCampPartyMgr::Instance()->CheckSummonSpiritTime())
	{
		roRes.set_result(KKSG::ERR_GUILDPARTY_ENDING);
		return;
	}
	// 还有，等会再招
	if (CGuildCampPartyMgr::Instance()->CheckSummonSpiritExist(scene->GetSceneID()))
	{
		roRes.set_result(KKSG::ERR_GUILDPARTY_HAVESUMMON);
		return;
	}
			
	// 扣消耗
	UINT32 diamond_count = 0;
	std::vector<KKSG::ItemBrief> itemlist;
	for (auto it = GetGlobalConfig().GuildCampSummonSpiritCost.begin(); it != GetGlobalConfig().GuildCampSummonSpiritCost.end(); ++it)
	{
		if (it->seq[0] == DIAMOND)
		{
			diamond_count += it->seq[1];
		}
		else
		{
			KKSG::ItemBrief item_brief;
			item_brief.set_itemid(it->seq[0]);
			item_brief.set_itemcount(it->seq[1]);
			itemlist.push_back(item_brief);
		}
	}
	if (diamond_count > 0)
	{		
		// 有人正在招
		if (CGuildCampPartyMgr::Instance()->CheckHaveSummonSpiritIng(role->getGuildId()))
		{
			roRes.set_result(KKSG::ERR_GUILDPARTY_SUMMONING);
			return;
		}
		UINT32 delayid = DelayRpc();
		KKSG::PayConsumeBase info;
		info.set_type((int)MidasConsumeType_SummonSpirit);
		info.set_amt(diamond_count);
		info.set_subreason(ItemFlow_GuildCampParty);
		info.set_reason(ItemFlow_GuildCampParty_RandomItem);
		info.set_index(delayid);
		info.set_count(0);
		std::vector<ItemDesc> vecItem;
		if(!role->ConsumeDiamond(info,vecItem))
		{
			CUserData data;
			data.m_dwUserData = KKSG::ERR_IBSHOP_LACKDIAMOND;
			ReplyDelayRpc(delayid, data);
		}
		// 设置正在处理中
		CGuildCampPartyMgr::Instance()->SetHaveSummonSpiritIng(role->getGuildId());
	}
	else
	{
		BagTakeItemTransition transition(role);
		transition.SetReason(ItemFlow_GuildCampParty,ItemFlow_GuildCampParty_RandomItem);
		for (auto it = itemlist.begin(); it != itemlist.end(); ++it)
		{
			if (!transition.TakeItem(it->itemid(),it->itemcount()))
			{
				transition.RollBack();	
				roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
				return;
			}

			SSInfo<<"itemid:"<<it->itemid()<<" count:"<<it->itemcount()<<END;
		}
		transition.NotifyClient();	

		// 生成精灵
		CGuildCampPartyMgr::Instance()->SummonSpirit(role);
	}
}

void RpcC2G_GuildPartySummonSpirit::OnDelayReplyRpc(const GuildPartySummonSpiritArg &roArg, GuildPartySummonSpiritRes &roRes, const CUserData &roUserData)
{
	roRes.set_result((KKSG::ErrorCode)(roUserData.m_dwUserData));
}
