#include "pch.h"
#include "enchant/rpcc2g_enchantactiveattribute.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/item.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/5/8 20:49:58

RPC_SERVER_IMPLEMETION(RpcC2G_EnchantActiveAttribute, EnchantActiveAttributeArg, EnchantActiveAttributeRes)

void RpcC2G_EnchantActiveAttribute::OnCall(const EnchantActiveAttributeArg &roArg, EnchantActiveAttributeRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	int slot = -1;
	XItem* pItem = pRole->Get<Bag>()->FindUniqueEquipOnBody(roArg.uid(), &slot);
	if (pItem == NULL)
	{
		SSWarn<<"Equip Item is NULL, item unique id:"<<roArg.uid()<<END;
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	if (pItem->enchant == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	KKSG::ErrorCode code = pItem->enchant->ChangeAttribute(roArg.attrid());
	roRes.set_errorcode(code);
	if (code == KKSG::ERR_SUCCESS)
	{
		BagUpdateItemTransition updateTrans(pRole);
		updateTrans.AddChangedItem(pItem);
		updateTrans.NotifyClient();

		pRole->RecalcuateAttributeAndNotify(RecalAttr_Enchant);

		pRole->OnEnchantEquip(slot);
	}
}

void RpcC2G_EnchantActiveAttribute::OnDelayReplyRpc(const EnchantActiveAttributeArg &roArg, EnchantActiveAttributeRes &roRes, const CUserData &roUserData)
{
}
