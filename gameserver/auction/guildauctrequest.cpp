#include "pch.h"
#include "guildauctrequest.h"
#include "gamelogic/bagtransition.h"

GuildAuctRequest::GuildAuctRequest(Role* role)
{
	m_pRole = role;
}

int GuildAuctRequest::HandleBuy(UINT32 price)
{
	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_GuildAuction, ItemFlow_GuildAuct_Take);

	if(!take.TakeItem(DRAGON_COIN, price))
	{
		take.RollBack();

		return KKSG::ERR_AUCT_DRAGONCOINLESS;
	}

	take.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::AuctFailBack(UINT32 price)
{
	BagGiveItemTransition give(m_pRole);
	give.SetReason(ItemFlow_GuildAuction, ItemFlow_GuildAuct_Give);
	
	give.GiveItem(DRAGON_COIN, price);

	give.NotifyClient();

	return KKSG::ERR_SUCCESS;
}
