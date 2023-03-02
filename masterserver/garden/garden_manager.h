#ifndef _HOME_GARDEN_MANAGER_
#define _HOME_GARDEN_MANAGER_
#include "garden/garden.h"
#include "guild/guild_garden.h"
#include "pch.h"
#include "role/role.h"

class GardenManager : ITimer
{
	GardenManager();
	~GardenManager();
	DECLARE_SINGLETON(GardenManager);

public:
	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	Garden* GetGarden(
		const UINT64 role_id,
		const std::string& role_name);
	bool InitGuildInfo();

	void SaveAllToDB();

	void StartUpGardenEvent();
public:
	GuildGarden* GuildGetGarden(const UINT64 garden_id);
	void AddGuildGarden(const UINT64 garden_id,GuildGarden* guild_garden);
	void RemoveGuildGarden(const UINT64 garden_id);
public:
	void CheckGardenNtf(const UINT64 garden_id);
	void SetChanged(UINT64 role_id);

	void LoadFromDB(const KKSG::GardenInfo& garden_info);
private:
	//bool Refresh();
private:
	HTIMER garden_timehandle_;

	std::unordered_map<UINT64, Garden*> all_garden_;
	std::unordered_map<UINT64, GuildGarden*> all_guild_garden_;
private:
	std::set<UINT64> changes_;
	std::queue<UINT64> queue_changes_;
	UINT32 garden_dbfreshtime_;
	std::unordered_set<UINT64> unload_roles_;
};


#endif // _HOME_GARDEN_MANAGER_