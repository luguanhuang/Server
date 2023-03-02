#ifndef __GUILDAUCTREQUEST_H__
#define __GUILDAUCTREQUEST_H__ 

namespace KKSG
{
	class GAItemDBData;
}

class Role;

class GuildAuctRequest
{
public:
	GuildAuctRequest(Role* role);

	int HandleBuy(UINT32 price);
	int AuctFailBack(UINT32 price);

private:
	Role* m_pRole;
};

#endif