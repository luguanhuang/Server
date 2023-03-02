#ifndef __DRAGON_GUILD_APPS_H__
#define __DRAGON_GUILD_APPS_H__

#include "util/gametime.h"
#include "dragonguildbase.h"
#include "table/globalconfig.h"

struct DragonGuildApplicationInfo
{
	DragonGuildApplicationInfo()
	{
		roleId = 0;
		appTime = 0;
		deleted = false;
	}

	UINT64 roleId;
	time_t appTime;
	bool deleted;
};

class DragonGuildApplication: public DragonGuildBase
{
public:
	DragonGuildApplication(DragonGuild* dragonguild);

	virtual bool LoadFromDB(const KKSG::DragonGuildSaveData& data);
	virtual void SaveToDB(KKSG::DragonGuildSaveData& data);
	virtual void OnDayPass() {}

	void Add(UINT64 roleId);
	KKSG::ErrorCode Accept(UINT64 qwRole);
	void Reject(UINT64 qwRole);
	void RejectAll();

	bool IsContain(UINT64 qwRole) { return FindAppIterator(qwRole) != m_appRoles.end();}
	bool IsApplyFull();

	void ClearDeleted();
	const std::string ToBinary();

	void GetApplicationInfo(KKSG::FetchDGAppRes& res);

private:
	std::list<DragonGuildApplicationInfo>::iterator FindAppIterator(UINT64 qwRoleID);

	std::list<DragonGuildApplicationInfo> m_appRoles;
};

#endif