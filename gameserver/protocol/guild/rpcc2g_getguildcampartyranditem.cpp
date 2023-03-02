#include "pch.h"
#include "guild/rpcc2g_getguildcampartyranditem.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/guildcampparty.h"
#include "math/XVector3.h"
#include "unit/enemy.h"
#include "entity/XObject.h"
#include "config/itemdropconfig.h"
#include "unit/enemymanager.h"
#include "gamelogic/bagtransition.h"
#include "scene/scene.h"
#include "guild/guildcampparty.h"

// generate by ProtoGen at date: 2017/4/24 17:12:02

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildCamPartyRandItem, GetGuildCamPartyRandItemArg, GetGuildCamPartyRandItemRes)

	void RpcC2G_GetGuildCamPartyRandItem::OnCall(const GetGuildCamPartyRandItemArg &roArg, GetGuildCamPartyRandItemRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}
	if(!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_GUILDCAMP_PARTY))
	{
		roRes.set_result(KKSG::ERR_GUILDACMPPATY_NOTOPEN);
		return;
	}
	
	KKSG::ErrorCode result = CGuildCampPartyMgr::Instance()->CheckPartyConditon(role);
	if ( result != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(result);
		return ;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);

	Sequence<float,4> coordinate;
	UINT32 drop_id = 0;
	if (roArg.query_type() == 1)
	{
		if (!CGuildCampPartyMgr::Instance()->DefendPlug(role->GetRoleID()))
		{
			roRes.set_result(KKSG::ERR_ACCOUNT_ROLE_FULL);
			SSWarn<<" oprate too faset role id :"<<role->GetRoleID()<<END;
			return;
		}

		auto it = GetGlobalConfig().GuildCampPartyNPC.begin();
		auto it_end = GetGlobalConfig().GuildCampPartyNPC.end();
		for (; it != it_end; ++it)
		{
			if (roArg.npc_id() == it->seq[0])
			{
				coordinate.seq[1] = it->seq[1];
				coordinate.seq[2] = it->seq[2];
				coordinate.seq[3] = it->seq[3];
				drop_id = GetGlobalConfig().GuildCampPartyNPCDropID.seq[0];
			}
		}
	}
	else
	{
		// 1. 场景里是否有这个sprite id
		Scene *scene = role->GetCurrScene();
		if (NULL == scene)
		{
			roRes.set_result(KKSG::ERR_INVALID_REQUEST);
			return;
		}
		Sequence<float,3> temp_vec;
		if (!CGuildCampPartyMgr::Instance()->CheckSpriteExist(scene->GetSceneID(),roArg.npc_id(),temp_vec))
		{
			// 小妖不存在了
			roRes.set_result(KKSG::ERR_GUILDACMPPATY_SPRITE_ONTEXIST);
			LogInfo("the npc id is %d not exist ",roArg.npc_id());
			CGuildCampPartyMgr::Instance()->UpdateSprite(scene);
			return;
		}

		coordinate.seq[1] = temp_vec.seq[0];
		coordinate.seq[2] = temp_vec.seq[1];
		coordinate.seq[3] = temp_vec.seq[2];

		drop_id = GetGlobalConfig().GuildCampPartyNPCDropID.seq[1];

		Vector3 targe(coordinate.seq[1],coordinate.seq[2],coordinate.seq[3]);	
		float distance = Vector3::Distance(role->GetXObject()->GetPosition_p(), targe);
		LogDebug("role vec x=%f,y=%f,z=%f",role->GetXObject()->GetPosition_p().x,role->GetXObject()->GetPosition_p().y,role->GetXObject()->GetPosition_p().z);
		LogDebug("npc  vec x=%f,y=%f,z=%f",coordinate.seq[1],coordinate.seq[2],coordinate.seq[3]);

		LogDebug("distance =%f",distance);
		if (distance > GetGlobalConfig().RandomItemDistance +1.2)
		{
			roRes.set_result(KKSG::ERR_GUILDACMPPATY_DISTANCE);
			return;
		}
	}	

	auto desc = ItemDropConfig::Instance()->GetRandomDrop(drop_id);
	if(desc)
	{
		BagGiveItemTransition trans(role);
		trans.SetReason(ItemFlow_GuildCampParty,ItemFlow_GuildCampParty_RandomItem);
		trans.GiveItem(desc->itemID, desc->itemCount);
		trans.NotifyClient();
		KKSG::ItemBrief *item = roRes.add_random_items();
		item->set_itemid(desc->itemID);
		item->set_itemcount(desc->itemCount);
	}
	if (roArg.query_type() != 1)
	{
		// 干掉这个ncp
		Scene *scene = role->GetCurrScene();
		if (NULL != scene)
		{
			  auto delete_result = CGuildCampPartyMgr::Instance()->DeleteSprite(scene,roArg.npc_id(),role->getGuildId());
			  roRes.set_result(delete_result);
		}
	}
}

void RpcC2G_GetGuildCamPartyRandItem::OnDelayReplyRpc(const GetGuildCamPartyRandItemArg &roArg, GetGuildCamPartyRandItemRes &roRes, const CUserData &roUserData)
{
}
