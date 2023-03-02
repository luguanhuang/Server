#include "pch.h"
#include "jade/rpcc2g_jadeoperation.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/jademgr.h"
#include "gamelogic/item.h"

// generate by ProtoGen at date: 2015/6/17 16:33:38

RPC_SERVER_IMPLEMETION(RpcC2G_JadeOperation, JadeOperationArg, JadeOperationRes)

void RpcC2G_JadeOperation::OnCall(const JadeOperationArg &roArg, JadeOperationRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	int slot = -1;
	XItem* pItem = pRole->Get<Bag>()->FindUniqueEquipOnBody(roArg.equipuniqueid(), &slot);
	if (NULL == pItem)
	{
		SSWarn<<"Equip Item is NULL, item unique id:"<<roArg.equipuniqueid()<<END;
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	if(pItem->type != EQUIP)
	{
		LogWarn("item is not equip, itemid:%u, uid:%llu", pItem->itemID, pItem->uid);
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	if (0 == roArg.operationtype())
	{
		///> 镶嵌龙玉
		XItem* pJadeItem = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.jadeuniqueid());
		if (NULL == pJadeItem)
		{
			SSWarn<<"Jade Item is NULL, item unique id:"<<roArg.jadeuniqueid()<<END;
			roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
			return;
		}
		KKSG::ErrorCode errorCode = CJadeMgr::Instance()->AttachJade(pRole, pItem, roArg.pos(), pJadeItem);
		roRes.set_errorcode(errorCode);
		if(errorCode != KKSG::ERR_SUCCESS && errorCode != KKSG::ERR_JADE_REPLACE)
		{
			return;
		}
		pRole->OnChangeJadeOnEquip(slot);

		pRole->RecalcuateAttributeAndNotify(RecalAttr_Jade);
		return;
	}
	else if (1 == roArg.operationtype())
	{
		///> 卸下龙玉
		KKSG::ErrorCode errorCode = CJadeMgr::Instance()->DetachJade(pRole, pItem, roArg.pos());
		roRes.set_errorcode(errorCode);
		if(KKSG::ERR_SUCCESS != errorCode)
		{
			return;
		}
		pRole->OnChangeJadeOnEquip(slot);

		pRole->RecalcuateAttributeAndNotify(RecalAttr_Jade);
		return;
	}
	roRes.set_errorcode(KKSG::ERR_UNKNOWN);
	return;
}

void RpcC2G_JadeOperation::OnDelayReplyRpc(const JadeOperationArg &roArg, JadeOperationRes &roRes, const CUserData &roUserData)
{
}
