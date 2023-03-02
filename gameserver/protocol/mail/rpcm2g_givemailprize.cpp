#include "pch.h"
#include "mail/rpcm2g_givemailprize.h"
#include "gamelogic/item.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/11/1 20:20:08

RPC_SERVER_IMPLEMETION(RpcM2G_GiveMailPrize, GiveMailPrizeArg, GiveMailPrizeRes)

bool GiveMailReward(Role* pRole, const KKSG::SMail& mail)
{
	std::vector<ItemDesc> items;
	for(int i = 0; i < mail.items_size(); i++)
	{
		ItemDesc item;
		item.ToThis(mail.items(i));
		items.push_back(item);
	}
	std::vector<XItem> xitems;
	for(int i = 0; i < mail.xitems_size(); i++)
	{
		XItem item;
		XItem::KKSGItem2Item(&item, &mail.xitems(i));
		xitems.push_back(item);
	}
	if(!pRole->Get<Bag>()->CanAddItem(&items, &xitems))
	{
		return false;
	}	

	BagGiveItemTransition trans(pRole);
	trans.SetReason(mail.reason(), mail.subreason());
	INT32 iDiamond = 0;
	for(UINT32 i = 0; i < items.size(); i++)
	{
		if (items[i].itemID == DIAMOND)
		{
			iDiamond += items[i].itemCount;
			continue;
		}
		trans.GiveItem(items[i]);
	}
	UINT32 nowTime = (UINT32)GameTime::GetTime();
	for(UINT32 i = 0; i < xitems.size(); i++)
	{
		if (xitems[i].IsExpire(nowTime))
		{
			continue;
		}
		trans.GiveItem(&xitems[i]);
	}
	trans.NotifyClient();

	if (iDiamond)
	{
		pRole->AddDiamond(iDiamond, mail.reason(), mail.subreason(), mail.extparam());
	}
	return true;
}

void RpcM2G_GiveMailPrize::OnCall(const GiveMailPrizeArg &roArg, GiveMailPrizeRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (pRole == NULL)
	{
		LogError("Can't find role by roleid: %llu", roArg.roleid());
		for (int i = 0; i < roArg.mail_size(); i++)
		{
			roRes.add_issucceed(false);
		}
		return;
	}
	for (int i = 0; i < roArg.mail_size(); i++)
	{
		roRes.add_issucceed(GiveMailReward(pRole, roArg.mail(i)));
	}
}

void RpcM2G_GiveMailPrize::OnDelayReplyRpc(const GiveMailPrizeArg &roArg, GiveMailPrizeRes &roRes, const CUserData &roUserData)
{
}
