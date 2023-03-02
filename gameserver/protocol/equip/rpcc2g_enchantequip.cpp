#include "pch.h"
#include "equip/rpcc2g_enchantequip.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/enchantmgr.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/bagtransition.h"
#include "tablemgr/XEntityInfoLibrary.h"

// generate by ProtoGen at date: 2016/12/8 17:20:44

RPC_SERVER_IMPLEMETION(RpcC2G_EnchantEquip, EnchantEquipArg, EnchantEquipRes)

void RpcC2G_EnchantEquip::OnCall(const EnchantEquipArg &roArg, EnchantEquipRes &roRes)
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
	EnchantConf* conf = EnchantMgr::Instance()->GetEnchantConf(roArg.enchantid());
	if (conf == NULL)
	{
		LogWarn("Enchantid:%u is not in EnchantEquip.txt", roArg.enchantid());
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	
	KKSG::ErrorCode code = EnchantMgr::Instance()->CanEnchantWearEquip(pItem->itemID, roArg.enchantid());
	if (code != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode(code);
		return;
	}

	BagTakeItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Enchant, ItemFlow_Enchant_Take);
	if (!transition.TakeItem(roArg.enchantid(), conf->Num))
	{
		transition.RollBack();
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	for (UINT32 i = 0; i < conf->Cost.size(); i++)
	{
		if (!transition.TakeItem(conf->Cost[i][0], conf->Cost[i][1]))
		{
			transition.RollBack();
			roRes.set_errorcode(KKSG::ERR_ENCHANT_LACKITEM);
			return;
		}
	}
	transition.NotifyClient();

	if (pItem->enchant == NULL)
	{
		pItem->enchant = new EnchantAttr;
	}

	ItemChangeAttr attr;
	if (conf->BaodiCount == 0 || conf->BaodiAttribute[0] == 0 || pItem->enchant->GetBaodiCount(roArg.enchantid()) < conf->BaodiCount - 1)
	{
		attr = EnchantMgr::Instance()->GetRandomEnchantAttr(roArg.enchantid());
		if (attr.AttrID == 0)
		{
			roRes.set_errorcode(KKSG::ERR_FAILED);
			LogError("role [%llu] enchant equip get attribute by GetRandomEnchantAttr failed, enchantid [%u]", pRole->GetID(), roArg.enchantid());
			return;
		}
		pItem->enchant->AddBaodiCount(roArg.enchantid());
	}
	else
	{
		attr.AttrID = conf->BaodiAttribute[0];
		attr.AttrValue = XRandom::randInt(conf->BaodiAttribute[1], conf->BaodiAttribute[2] + 1);
	}

	///> 保底清零
	if (attr.AttrID == conf->BaodiAttribute[0])
	{
		pItem->enchant->ClearBaodiCount(roArg.enchantid());
	}

	///> 根据职业改变攻击力属性
	const ProfessionTable::RowData* profConf = XEntityInfoLibrary::Instance()->TemplateID2Data_Role(pRole->GetProfession());
	if (profConf != NULL)
	{
		if (profConf->AttackType == 2 && attr.AttrID == BASIC_PhysicalAtk)
		{
			attr.AttrID = BASIC_MagicAtk;
		}
		if (profConf->AttackType == 1 && attr.AttrID == BASIC_MagicAtk)
		{
			attr.AttrID = BASIC_PhysicalAtk;
		}
	}
	else
	{
		LogError("role [%llu], Profession [%d] not find in Profession.txt when Enchant Equip", pRole->GetID(), pRole->GetProfession());
	}

	///> 再次尝试保底清零
	if (attr.AttrID == conf->BaodiAttribute[0])
	{
		pItem->enchant->ClearBaodiCount(roArg.enchantid());
	}

	KKSG::AttributeInfo* attrInfo = roRes.mutable_attr();
	attrInfo->set_id(attr.AttrID);
	attrInfo->set_value(attr.AttrValue);
	pItem->enchant->Update(attr.AttrID, attr.AttrValue, roArg.enchantid());

	BagUpdateItemTransition updateTrans(pRole);
	updateTrans.AddChangedItem(pItem);
	updateTrans.NotifyClient();

	pRole->RecalcuateAttributeAndNotify(RecalAttr_Enchant);

	pRole->OnEnchantEquip(slot);

	std::vector<ItemDesc> vlist;
	vlist.push_back(ItemDesc(roArg.enchantid(), conf->Num));
	XItem::PrintEquipTLog(pRole, pItem, TX_Item_Enchant, vlist, roArg.enchantid());

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2G_EnchantEquip::OnDelayReplyRpc(const EnchantEquipArg &roArg, EnchantEquipRes &roRes, const CUserData &roUserData)
{
}
