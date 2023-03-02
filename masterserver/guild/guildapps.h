#ifndef __GUILDAPPMGR_H__
#define __GUILDAPPMGR_H__

#include "guild/guilddef.h"
#include "guild/guildmgr.h"
#include "role/switch.h"

class CRole;
class Guild;

struct GuildApplicationInfo
{
	UINT64 qwRoleID;
	time_t appTime;
	bool   deleted;
};

typedef void (*GAppCB)(GuildApplicationInfo *pInfo, void *);

class GuildApplication : public IGuildSerializerInterface
{
public:
	GuildApplication(Guild *pGuild);
	~GuildApplication();

	inline UINT32 Count() { return m_appRoles.size(); }

	void Add(UINT64 roleId);
	bool Accept(UINT64 qwRole);
	void Reject(UINT64 qwRole);
	void RejectAll();

	bool IsContain(UINT64 qwRole) { return FindAppIterator(qwRole) != m_appRoles.end();}

	void ClearDeleted();

	void Foreach(GAppCB cb, void *arg);

	virtual bool IsChanged();
	virtual bool Load(const KKSG::GuildAllDBData & guilddata);
	virtual void Save(KKSG::GuildAllDBData & guilddata);
	virtual const std::string ToBinary();

private:
	std::list<GuildApplicationInfo>::iterator FindAppIterator(UINT64 qwRoleID);

	Guild *m_pGuild;
	Switch m_ismodified;
	std::list<GuildApplicationInfo> m_appRoles;
};



#endif // __GUILDAPPMGR_H__