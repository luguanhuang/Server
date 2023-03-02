#ifndef _DRAGON_GUILD_MGR_H_
#define _DRAGON_GUILD_MGR_H_

#include "util/utilsingleton.h"
#include "util/maplist.h"
#include "dragonguild.h"
#include "timer.h"
#include "util/simpletimer.h"
#include "dragonguildsorter.h"
#include "dragonguilddef.h"

class DragonGuildMgr: public Singleton<DragonGuildMgr>
{
public:
	DragonGuildMgr();
	~DragonGuildMgr();

public:
	void DoRoleLogin(CRole* pRole);
	UINT32 CanCreateDragonGuild(CRole* pRole, std::string name);

public:
	bool Init();
	void Uninit();
	void SaveAll2DB();
	bool InitPermissionConfig();

	bool OnTimer();
	void Update();

	bool LoadFromDB() { return true; }

	bool HasDragonGuild(UINT64 roleID);
	bool IsInsameDragonGuild(UINT64 roleID1, UINT64 roleID2);
	void OnChangePPT(UINT64 roleId, UINT32 ppt);
	void SortByPPT();
	void OnDragonGuildSorterChanged(KKSG::DragonGuildSortType type) { m_sorter.SetChanged(type); }

	UINT64 GetDragonGuildIdByRole(UINT64 roleID);
	DragonGuild* GetDragonGuildByRole(UINT64 roleID);
	DragonGuild* GetDragonGuild(UINT64 dragonguildId);

	void AddRoleDragonGuild(UINT64 roleId, UINT64 id);
	void RemoveRoleDragonGuild(UINT64 roleId);

	bool RefreshShop();
	UINT32 GetLastRefreshShopTime() { return m_lastPassDayTime; }

	//小分队任务
	void PassSceneAndFinishTask(UINT64 dragonguildId, std::vector<UINT64>& roleid, UINT32 enterSceneRoleCnt,UINT32 sceneId, UINT32 noDieCnt, UINT32 timeSpan);
	void UnLockSpeicalAchieve(UINT64 dragonguildId, std::vector<UINT64>& roleid, UINT32 enterSceneRoleCnt, UINT32 sceneId, UINT32 noDieCnt, UINT32 timeSpan);

	UINT32 CreateDragonGuild(CRole* pRole, const std::string name);
	UINT32 JoinDragonGuild(CRole* pRole, UINT32 attr, UINT64 dragonguildId);
	DragonGuild* FindQuickJoinDragonGuild(UINT32 ppt);

	DragonGuildSorter& GetSorter() { return m_sorter; }

	void AddToUpdateList(UINT64 partnerId)
	{
		m_dirtyList.push_back(partnerId);
	}
	void AddToDeleteList(UINT64 partnerId)
	{
		m_delList.push_back(partnerId);
		m_dirtyList.remove(partnerId);
	}
	bool IsNameNotUsed(const std::string &name)
	{ 
		return m_mapGuildNames.find(name) == m_mapGuildNames.end();
	}

	const std::unordered_map<UINT64, DragonGuild*>& GetDragonGuilds() { return m_dragonguilds; }
	void Dismiss(UINT64 dragonguildId);

	void RemoveGuildName(const std::string &name);
	bool ModifyName(DragonGuild* pDGuild, const std::string& oldname, const std::string& newname);
	bool HasPermission(DragonGuildPosition pos, DragonGuildPermission pem) { return m_PERMMATRIX[pem][pos]; }
	bool IsLockName(const std::string &name) 
	{
		auto iter=m_LockName.find(name);
		if(iter!=m_LockName.end())
		{
			return true;
		}
		return false;
	}
	void AddLockName(const std::string &name)
	{
		m_LockName.insert(name);
	}
	void UnlockName(const std::string &name)
	{
		auto iter = m_LockName.find(name);
		if(iter!=m_LockName.end())
		{
			m_LockName.erase(iter);
		}
	}

	void AddJoinInfo(UINT64 nRoleID);
	void ClearJoinCount();
	bool CheckJoin(UINT64 nRoleID);

private:
	UINT64 NewID();
	void AddDragonGuild(UINT64 id, DragonGuild* dragonguild);

	struct GuildDoor
	{
		UINT32 nNextTime;
		UINT32 nCount;
	};

private:
	UINT32 m_curId;
	UINT32 m_lastPassDayTime;
	std::unordered_map<UINT64, DragonGuild*> m_dragonguilds;
	std::unordered_map<UINT64, UINT64>	m_roleId2dragonguildId;
	std::unordered_map<std::string,DragonGuild*> m_mapGuildNames;

	MapList<UINT64> m_dirtyList;
	MapList<UINT64> m_delList;
	std::set<std::string> m_LockName;

	DragonGuildSorter m_sorter;

	bool m_PERMMATRIX[DGPEM_MAX][DGPOS_MAX];

	//重复加入CD信息
	std::unordered_map<UINT64, GuildDoor> m_mapGuildDoor;

	bool m_sortByPPT;
};


#endif
