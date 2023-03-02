#include "pch.h"
#include "item/rpcc2g_usesupplement.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/itemconfig.h"
#include "entity/XActionDefine.h"
#include "entity/XRole.h"
#include "scene/scene.h"

// generate by ProtoGen at date: 2016/1/18 17:27:25

RPC_SERVER_IMPLEMETION(RpcC2G_UseSupplement, UseSupplementReq, UseSupplementRes)

void RpcC2G_UseSupplement::OnCall(const UseSupplementReq &roArg, UseSupplementRes &roRes)
{
	//Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	//if (NULL == role)
	//{
	//	roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
	//	return;
	//}
	//if (NULL == role->GetCurrScene() || !role->GetCurrScene()->CanUseSupplement())
	//{
	//	roRes.set_errorcode(KKSG::ERR_UNKNOWN);
	//	return;
	//}
	//Bag* bag = role->Get<Bag>();
	//XItem* item = bag->FindItemUnique(roArg.uniqueid());
	//if (NULL == item)
	//{
	//	roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
	//	return;
	//}
	//if (SUPPLEMENT != item->type)
	//{
	//	roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
	//	return;
	//}
	//SupplementConf* conf = ItemConfig::Instance()->GetSupplementConf(item->itemID);
	//if (NULL == conf)
	//{
	//	roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
	//	return;
	//}
	//// cooldown
	//if (!item->IsCoolDown())
	//{
	//	roRes.set_errorcode(KKSG::ERR_ITEM_COOLDOWN);
	//	return;
	//}
	//// limit
	//if (1 == conf->islimit)
	//{
	//	BagTakeItemTransition transition(role);
	//	transition.SetReason(ItemFlow_Stage, ItemFlow_Stage_Supplement);
	//	if (!transition.TakeUniqueItem(roArg.uniqueid(), 1))
	//	{
	//		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
	//		return;
	//	}
	//	transition.NotifyClient();
	//}
	//// set cooldown
	//XItem* itemleft = bag->FindItemUnique(roArg.uniqueid());
	//if (NULL != itemleft)
	//{
	//	itemleft->SetCoolDown(conf->cooldown);
	//}
	//else
	//{
	//	bag->ClearSupplement(roArg.uniqueid());
	//}
	//// effect
	//if (role->GetCurrScene()->IsSyncScene())
	//{
	//	XAddBuffActionArgs e;
	//	e.BuffID = (int)conf->buffid; 
	//	e.BuffLevel = (int)conf->bufflevel;
	//	e.Caster = role->GetID();
	//	role->GetXObject()->DispatchAction(&e);
	//}

	//role->SetFlag(RSF_ISUseSupplement, true);

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_UseSupplement::OnDelayReplyRpc(const UseSupplementReq &roArg, UseSupplementRes &roRes, const CUserData &roUserData)
{
}
