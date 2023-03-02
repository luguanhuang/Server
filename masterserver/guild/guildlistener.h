#ifndef __CGUILDLISTENER_H__
#define __CGUILDLISTENER_H__

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
	virtual void OnGuildAddExp(Guild* pGuild) {};
	virtual void OnGuildAddPrestige(Guild* pGuild) {};
};

#endif // __GUILDLISTENER_H__