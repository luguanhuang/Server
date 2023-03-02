#include "pch.h"
#include "findback/rpcc2g_itemfindback.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "findback/findbackrecord.h"

// generate by ProtoGen at date: 2016/8/30 16:45:47

RPC_SERVER_IMPLEMETION(RpcC2G_ItemFindBack, ItemFindBackArg, ItemFindBackRes)

void RpcC2G_ItemFindBack::OnCall(const ItemFindBackArg &roArg, ItemFindBackRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CFindBackRecord* record = pRole->Get<CFindBackRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	UINT32 error = record->GetItemFindBackInfo().GetItemFindBackReward(roArg.id(), roArg.findbackcount(), roArg.backtype());
	roRes.set_error((KKSG::ErrorCode)error);
}

void RpcC2G_ItemFindBack::OnDelayReplyRpc(const ItemFindBackArg &roArg, ItemFindBackRes &roRes, const CUserData &roUserData)
{
}
