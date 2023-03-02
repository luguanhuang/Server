#include "pch.h"
#include "jade/rpcc2g_takeoffalljade.h"
#include "unit/rolemanager.h"
#include "gamelogic/item.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2016/7/14 12:06:08

RPC_SERVER_IMPLEMETION(RpcC2G_TakeOffAllJade, TakeOffAllJadeArg, TakeOffAllJadeRes)

void RpcC2G_TakeOffAllJade::OnCall(const TakeOffAllJadeArg &roArg, TakeOffAllJadeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	Bag *pBag = pRole->Get<Bag>();	
	XItem* pItem = pBag->FindUniqueEquipOnBody(roArg.uid());
	if(pItem == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	if(pItem->type != EQUIP)
	{
		LogWarn("item is not equip, itemid:%u, uid:%llu", pItem->itemID, pItem->uid);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	if(NULL == pItem->jadeInfo)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	std::vector<JadeSingle>& vec = pItem->jadeInfo->vecJadeSingle;
	BagGiveItemTransition stGiveTransition(pRole);
	stGiveTransition.SetReason(ItemFlow_Jade, ItemFlow_Jade_TakeOff);
	for(auto i = vec.begin(); i != vec.end(); i++)
	{
		stGiveTransition.GiveItem((*i).jadeid, 1);
	}
	stGiveTransition.NotifyClient();

	vec.clear();
	BagUpdateItemTransition stUpdateTransition(pRole);
	stUpdateTransition.AddChangedItem(pItem);
	stUpdateTransition.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_Jade);

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_TakeOffAllJade::OnDelayReplyRpc(const TakeOffAllJadeArg &roArg, TakeOffAllJadeRes &roRes, const CUserData &roUserData)
{
}
