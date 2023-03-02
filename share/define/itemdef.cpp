#include "pch.h"
#include "itemdef.h"
#include "pb/project.pb.h"

void ItemDesc::ToKKSG(KKSG::ItemBrief* itembrief) const
{
	itembrief->set_itemid(itemID);
	itembrief->set_itemcount(itemCount);
	itembrief->set_isbind(isbind);
	itembrief->set_cooldown(cooldown);
}

void ItemDesc::ToThis(const KKSG::ItemBrief& itembrief)
{
	itemID = itembrief.itemid();
	itemCount = itembrief.itemcount();
	if (itembrief.has_isbind()) isbind = itembrief.isbind();
	else isbind = true;
	cooldown = itembrief.cooldown();
}

KKSG::ErrorCode GetErrorCode(UINT32 itemId)
{
	switch (itemId)
	{
	case GOLD: return KKSG::ERR_LACKCOIN;
	case DRAGON_COIN: return  KKSG::ERR_DRAGONCOIN_LIMIT;
	case DIAMOND: return KKSG::ERR_LACKDIAMOND;
	}
	return KKSG::ERR_ITEM_NOT_ENOUGH_COMMON;
}