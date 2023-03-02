#include "pch.h"
#include "equip/rpcc2g_forgeequip.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/randomattrmgr.h"
#include "define/tlogenum.h"

// generate by ProtoGen at date: 2017/1/10 10:36:59

RPC_SERVER_IMPLEMETION(RpcC2G_ForgeEquip, ForgeEquipArg, ForgeEquipRes)

void RpcC2G_ForgeEquip::OnCall(const ForgeEquipArg &roArg, ForgeEquipRes &roRes)
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
	std::vector<ItemDesc> vlist;
	switch (roArg.type())
	{
	case Forge_Equip:
		{
			KKSG::ErrorCode code = RandomAttrMgr::Instance()->ForgeEquip(pItem, roArg.isusedstone(), pRole, vlist);
			if (code == KKSG::ERR_SUCCESS)
			{
				pRole->OnForgeEquip(slot);
				XItem::PrintEquipTLog(pRole, pItem, TX_Item_Forge, vlist);
			}
			roRes.set_errorcode(code);
			return;
		}
	case Forge_Replace:
		{
			KKSG::ErrorCode code = RandomAttrMgr::Instance()->ForgeReplace(pItem, pRole);
			roRes.set_errorcode(code);
			if (code == KKSG::ERR_SUCCESS)
			{
				XItem::PrintEquipTLog(pRole, pItem, TX_Item_ForgeReplace, vlist);
			}
			return;
		}
	case Forge_Retain:
		{
			KKSG::ErrorCode code = RandomAttrMgr::Instance()->ForgeRetain(pItem, pRole);
			roRes.set_errorcode(code);
			return;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
	}
}

void RpcC2G_ForgeEquip::OnDelayReplyRpc(const ForgeEquipArg &roArg, ForgeEquipRes &roRes, const CUserData &roUserData)
{
}
