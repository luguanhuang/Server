#include "pch.h"
#include "common/rpcm2g_msgiveitem.h"
#include "util.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "checkutil.h"
#include "gamelogic/bagtransition.h"

// generate by ProtoGen at date: 2017/6/24 11:30:41

RPC_SERVER_IMPLEMETION(RpcM2G_MsGiveItem, MsGiveItemArg, MsGiveItemRes)

void RpcM2G_MsGiveItem::OnCall(const MsGiveItemArg &roArg, MsGiveItemRes &roRes)
{
	std::string strItem;
	for (int i = 0; i < roArg.items_size(); ++i)
	{
		if (i != 0) strItem += ",";
		const KKSG::ItemBrief& brief = roArg.items(i);
		strItem += ToString(brief.itemid()) + "|" + ToString(brief.itemcount()) + "|" + (brief.has_isbind() ? ToString(brief.isbind()) : "1");
	}
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		LogWarn("role [%llu] is not in this gs, ms give item failed, reason [%d], subreason [%d], items, %s", roArg.roleid(), roArg.reason(), roArg.subreason(), strItem.c_str());
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	if (!roArg.has_reason() || !roArg.has_subreason())
	{
		LogError("role [%llu], ms give item failed, no reason or subreason, items, %s", roArg.roleid(), strItem.c_str());
		CHECK_COND_NORETURN(false);
	}

	LogInfo("role [%llu], ms give item succeed, reason [%d], subreason [%d], items, %s", roArg.roleid(), roArg.reason(), roArg.subreason(), strItem.c_str());

	BagGiveItemTransition trans(pRole);
	trans.SetReason(roArg.reason(), roArg.subreason());
	for (int i = 0; i < roArg.items_size(); ++i)
	{
		const KKSG::ItemBrief& brief = roArg.items(i);
		if (brief.has_isbind())
		{
			trans.GiveItem(brief.itemid(), brief.itemcount(), brief.isbind());
		}
		else
		{
			trans.GiveItem(brief.itemid(), brief.itemcount());
		}
	}
	trans.NotifyClient();
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcM2G_MsGiveItem::OnDelayReplyRpc(const MsGiveItemArg &roArg, MsGiveItemRes &roRes, const CUserData &roUserData)
{
}
