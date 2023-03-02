#include "pch.h"
#include "partner/rpcm2g_makepartnerm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/rolemisc.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/rolepartner.h"
#include "define/systemiddef.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/12/8 16:06:49

RPC_SERVER_IMPLEMETION(RpcM2G_MakePartnerM2G, MakePartnerM2GArg, MakePartnerM2GRes)

void RpcM2G_MakePartnerM2G::OnCall(const MakePartnerM2GArg &roArg, MakePartnerM2GRes &roRes)
{
	// GS检查等级、龙币、当天没有解除过战友关系
	std::vector<Role*> roles;
	for (int i = 0; i < roArg.roleids_size(); ++i)
	{
		UINT64 roleID = roArg.roleids(i);
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
		// 不在主城
		if (pRole == NULL)
		{
			roRes.set_roleid(roleID);
			roRes.set_result(KKSG::ERR_PARTNER_NOT_IN_MAIN_HALL);
			return;
		}
		RolePartner* record = pRole->Get<RolePartner>();
		KKSG::ErrorCode code = record->CheckCanMakePartner();
		if (code != KKSG::ERR_SUCCESS)
		{
			roRes.set_roleid(roleID);
			roRes.set_result(code);
			return;
		}

		roles.push_back(pRole);
	}

	// 扣龙币
	roRes.set_result(KKSG::ERR_SUCCESS);
	for (auto it = roles.begin(); it != roles.end(); ++it)
	{
		Role* pRole = *it;
		BagTakeItemTransition trans(pRole);
		trans.SetReason(ItemFlow_Partner, ItemFlow_MakePartner);
		trans.TakeItem(DRAGON_COIN, GetGlobalConfig().PartnerNeedDragonCoin);
		trans.NotifyClient();
		LogInfo("role[%llu] make partner", pRole->GetID());
	}

	std::ostringstream oss;	
	for (int i = 0; i < roArg.roleids_size(); ++i)
	{
		if (i != 0)
		{
			oss << ",";
		}
		oss << roArg.roleids(i);
	}
	LogInfo("make partner [%s] check success", oss.str().c_str());
}

void RpcM2G_MakePartnerM2G::OnDelayReplyRpc(const MakePartnerM2GArg &roArg, MakePartnerM2GRes &roRes, const CUserData &roUserData)
{
}
