#include "pch.h"
#include "findback/rpcc2g_itemfindbackinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "findback/findbackrecord.h"

// generate by ProtoGen at date: 2016/8/30 16:18:36

RPC_SERVER_IMPLEMETION(RpcC2G_ItemFindBackInfo, ItemFindBackInfoArg, ItemFindBackInfoRes)

void RpcC2G_ItemFindBackInfo::OnCall(const ItemFindBackInfoArg &roArg, ItemFindBackInfoRes &roRes)
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

	UINT32 error = record->GetItemFindBackInfoRes(roRes);
	roRes.set_error((KKSG::ErrorCode)error);
}

void RpcC2G_ItemFindBackInfo::OnDelayReplyRpc(const ItemFindBackInfoArg &roArg, ItemFindBackInfoRes &roRes, const CUserData &roUserData)
{
}
