#include "pch.h"
#include "equip/rpcc2g_fuseequip.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/fusemgr.h"

// generate by ProtoGen at date: 2017/9/7 16:47:26

RPC_SERVER_IMPLEMETION(RpcC2G_FuseEquip, FuseEquipArg, FuseEquipRes)

void RpcC2G_FuseEquip::OnCall(const FuseEquipArg &roArg, FuseEquipRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	int pos;
	XItem* pItem = pRole->Get<Bag>()->FindUniqueEquipOnBody(roArg.uid(), &pos);
	if (pItem == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ITEM_NOTEXIST);
		return;
	}
	
	switch (roArg.type())
	{
	case 0:
		{
			roRes.set_errorcode(FuseMgr::Instance()->FuseEquipAddExp(pRole, pItem, roArg.itemid()));
			break;
		}
	case 1:
		{
			roRes.set_errorcode(FuseMgr::Instance()->FuseEquipBreak(pRole, pItem, roArg.itemid()));
			break;
		}
	default:
		{
			roRes.set_errorcode(KKSG::ERR_UNKNOWN);
			return;
		}
	}
	pRole->OnFuseEquip(pos);
}

void RpcC2G_FuseEquip::OnDelayReplyRpc(const FuseEquipArg &roArg, FuseEquipRes &roRes, const CUserData &roUserData)
{
}
