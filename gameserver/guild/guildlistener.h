#ifndef __GUILDLISTENER_H__
#define __GUILDLISTENER_H__

#include "guild/guilddef.h"

class Guild;
class IGuildListener
{
public:
	virtual ~IGuildListener() {}

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember) = 0;
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember) = 0;
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID) = 0;
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember) = 0;
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember) = 0;
	
	virtual void OnGuildLevelUp(Guild *pGuild) {};
	virtual void OnGuildAddExp() {};
};

#endif // __GUILDLISTENER_H__