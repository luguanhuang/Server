#include "pch.h"
#include "equip/rpcc2g_upgradeequip.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"

// generate by ProtoGen at date: 2017/8/29 16:48:14

RPC_SERVER_IMPLEMETION(RpcC2G_UpgradeEquip, UpgradeEquipArg, UpgradeEquipRes)

void RpcC2G_UpgradeEquip::OnCall(const UpgradeEquipArg &roArg, UpgradeEquipRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	KKSG::ErrorCode code = pRole->Get<Bag>()->UpgradeEquip(roArg.uid());
	roRes.set_errorcode(code);
}

void RpcC2G_UpgradeEquip::OnDelayReplyRpc(const UpgradeEquipArg &roArg, UpgradeEquipRes &roRes, const CUserData &roUserData)
{
}
