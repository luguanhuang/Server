#include "pch.h"
#include "social/rpcc2g_sendflower.h"
#include "unit/rolemanager.h"
#include "gamelogic/flowerrecord.h"
#include "gamelogic/item.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/rolesummarymgr.h"
#include "mail/mailop.h"
#include "social/ptcg2c_receiveflowerntf.h"
#include "social/ptcg2c_flowerrainntf.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "foreach.h"
#include "gamelogic/payv2Record.h"
#include "gamelogic/chatcondition.h"
#include "social/rpcg2m_mssendflower.h"
#include "utility/tlogger.h"
#include "network/mslink.h"
#include "utility/uidhelper.h"
#include "config.h"

// generate by ProtoGen at date: 2015/9/23 16:12:41


RPC_SERVER_IMPLEMETION(RpcC2G_SendFlower, SendFlowerArg, SendFlowerRes)

#define SENDFLOWERNTF 520
void RpcC2G_SendFlower::OnCall(const SendFlowerArg &roArg, SendFlowerRes &roRes)
{
	/*if(!GSConfig::Instance()->IsHallGS())
	{
		LogWarn("Receive request not in hall gs");
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}*/

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role || !role->GetCurrScene())
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	UINT64 destRoleID = roArg.roleid();
	UINT32 destServerID = UIDHelper::GetRoleIDServerID(destRoleID);
	if(!GSConfig::Instance()->IsServerIDIn(destServerID))
	{
		roRes.set_errorcode(ERR_ROLE_IN_OTHER_SERVER);
		return;
	}

	Scene* pScene = role->GetCurrScene();
	if (pScene->GetSceneType() != KKSG::SCENE_HALL && pScene->GetSceneType() != KKSG::SCENE_GUILD_HALL && pScene->GetSceneType() != KKSG::SCENE_FAMILYGARDEN)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	roRes.set_errorcode(ERR_FAILED);

	if (role->GetID() == roArg.roleid() || !roArg.count())
	{
		return;
	}

	int costID = 0;
	int costNum = 0;
	bool bFind = false;
	foreach(i in  GetGlobalConfig().FlowerCostNum)
	{
		if (i->seq[0] == roArg.senditemid())
		{
			bFind = true;
			costID = i->seq[1];
			costNum = i->seq[2];
			break;
		}
	}
	if(!bFind)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	Bag* bag = role->Get<Bag>();
	int num = bag->CountItemInBag(roArg.senditemid());
	if (num >= roArg.count())
	{
		BagTakeItemTransition transition(role);
		transition.SetReason(ItemFlow_Flower, ItemFlow_Flower_Send);
		if (!transition.TakeItem(roArg.senditemid(), roArg.count()))
		{
			transition.RollBack();
			return;
		}
		transition.NotifyClient();
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		//return;
	}
	else
	{
		if (costID == DIAMOND)
		{
			roRes.set_errorcode(KKSG::ERR_ITEM_NEED_DIAMOND);
			return;
		}

		int needbuy = roArg.count() - num;
		if (!roArg.has_costitemid() || !roArg.has_costitemnum())
		{
			roRes.set_errorcode(KKSG::ERR_ITEM_NOT_ENOUGH);
			return;
		}
		
		if (roArg.costitemid() != costID)
		{
			return;
		}
		if (!XItem::IsVirtualItem(costID) && costNum * needbuy > bag->CountItemInBag(roArg.costitemid()))
		{
			roRes.set_errorcode(KKSG::ERR_SHOP_LACKMONEY);
			return;
		}
		if (XItem::IsVirtualItem(costID) && costNum * needbuy > bag->CountVirtualItem(roArg.costitemid()))
		{
			roRes.set_errorcode(KKSG::ERR_SHOP_LACKMONEY);
			return;
		}

		BagTakeItemTransition transition(role);
		transition.SetReason(ItemFlow_Flower, ItemFlow_Flower_Send);
		if (!transition.TakeItem(roArg.costitemid(), costNum * needbuy))
		{
			transition.RollBack();
			return;
		}
		if (!transition.TakeItem(roArg.senditemid(), num))
		{
			transition.RollBack();
			return;
		}	
		transition.NotifyClient();
		
	}

	RpcG2M_MSSendFlower* rpc = RpcG2M_MSSendFlower::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_itemid(roArg.senditemid());
	rpc->m_oArg.set_itemcount(roArg.count());
	rpc->m_oArg.set_roleid(roArg.roleid());
	role->SendToMS(*rpc);

	role->OnSendFlower();
	
	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	TSendFlowerFlow oLog(role);
	oLog.m_iCostID = costID;
	oLog.m_iCostNum = costNum;
	oLog.m_iSendItemID = roArg.senditemid();
	oLog.m_iSendCount = roArg.count();
	oLog.m_ToRoleId = roArg.roleid();
	oLog.Do();
	return;
}

void RpcC2G_SendFlower::OnDelayReplyRpc(const SendFlowerArg &roArg, SendFlowerRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode(KKSG::ErrorCode(roUserData.m_dwUserData));
}
