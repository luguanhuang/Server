#include "pch.h"
#include "equip/rpcc2g_enchanttransfer.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/enchantmgr.h"

// generate by ProtoGen at date: 2017/1/14 17:47:09

RPC_SERVER_IMPLEMETION(RpcC2G_EnchantTransfer, EnchantTransferArg, EnchantTransferRes)

void RpcC2G_EnchantTransfer::OnCall(const EnchantTransferArg &roArg, EnchantTransferRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	XItem* origin = pRole->Get<Bag>()->FindUniqueItemInBag(roArg.originuid());
	XItem* dest = pRole->Get<Bag>()->FindUniqueEquipOnBody(roArg.destuid());
	if (origin == NULL || dest == NULL)
	{
		LogWarn("uid can't find, is NULL");
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}
	KKSG::ErrorCode code = EnchantMgr::Instance()->DoTransfer(pRole, origin, dest, true);
	if (code == KKSG::ERR_SUCCESS)
	{
		pRole->RecalcuateAttributeAndNotify(RecalAttr_Enchant);
	}
	roRes.set_errorcode(code);
}

void RpcC2G_EnchantTransfer::OnDelayReplyRpc(const EnchantTransferArg &roArg, EnchantTransferRes &roRes, const CUserData &roUserData)
{
}
