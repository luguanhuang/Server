#ifndef __GUILDHISTORY_H__
#define __GUILDHISTORY_H__

#include "guild/guildhistorydef.h"
#include "guildlistener.h"
//#include "unit/roleserilizeinterface.h"
#include "role/switch.h"


class  Guild;
struct GuildMember;
namespace KKSG { class GuildAllDBData; }

typedef void (*GuildLogCB)(GuildLogBase *, void *);

class GuildHistory : public IGuildSerializerInterface, public IGuildListener
{
public:

	GuildHistory(Guild *pGuild);
	~GuildHistory();

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

	virtual void OnAddMember(Guild *pGuild, GuildMember *pMember);
	virtual void OnRemoveMember(Guild *pGuild, UINT64 qwRoleID);
	virtual void OnMemberPosChanged(Guild *pGuild, GuildMember *pMember);
	virtual void OnMembergGuildBossDps(Guild *pGuild, GuildMember *pMember);
	virtual void OnAddMemberFromDB(Guild *pGuild, GuildMember *pMember);

	void Foreach(int type, GuildLogCB cb, void *arg);

private:
	void Shrink();

	int m_CleanCount;
	Switch m_isModified;
	Guild *m_pGuild;
	std::list<GuildMemberLog> m_MemberLog;
};

#endif // __GUILDHISTORY_H__