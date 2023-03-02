#ifndef __GUILDHISTORYDEF_H__
#define __GUILDHISTORYDEF_H__

#include <time.h>

struct GuildLogBase
{
	int          type;
	UINT64       roleid;
	std::string  rolename;
	time_t       time;
};

struct GuildMemberLog : public GuildLogBase
{
	int          position;
};

#endif // __GUILDHISTORYDEF_H__