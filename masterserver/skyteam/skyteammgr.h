#ifndef _SKY_TEAM_MGR_H_
#define _SKY_TEAM_MGR_H_

#include "util/utilsingleton.h"
#include "util/maplist.h"
#include "timer.h"

#include "skyteam.h"

class CRole;
class SkyTeam;
class Team;
class SkyTeamMgr : public ITimer, public Singleton<SkyTeamMgr>
{
public:
	SkyTeamMgr();
	~SkyTeamMgr();

	bool Init();
	void Uninit();
	void Save2DB(UINT32 num = 0);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	bool OnDayPass();

	bool LoadFromDB();

	bool HasSkyTeam(UINT64 roleId);
	bool IsInSameSkyTeam(UINT64 roleId1, UINT64 roleId2);

	SkyTeam* GetSkyTeamByRoleId(UINT64 roleId);
	SkyTeam* GetSkyTeamById(UINT64 teamId);
	UINT64 GetSkyTeamIdByRoleId(UINT64 roleId);

	int LeaveMember(UINT64 roleId, bool force = false);

	void AddRoleSkyTeam(UINT64 roleId, UINT64 teamId);
	void RemoveRoleSkyTeam(UINT64 roleId);

	void AddToUpdateList(UINT64 stId);

	void RemoveSkyTeam(UINT64 teamId);

	int CheckNameValid(const std::string& name);

	bool IsNameUsed(const std::string& name);
	void SetNameUsed(const std::string& name);
	void RemoveNameUsed(const std::string& name);

	// 奖励
	void OnSeasonEnd();
	void SendAllRankReward();
	void SendRankReward(UINT64 stId, UINT32 rank);
	void SendEliRoundReward(UINT32 round, UINT64 stId, bool isWin);

	// 组队相关
	int CheckCreateTeam(Team* pTeam, const std::string& name, UINT64& errRoleId);
	int StartCreateTeam(Team* pTeam);
	int TeamStartBattle(Team* pTeam);

	// GM
	bool GMCreateTeam(const std::vector<UINT64>& roleIds, const std::string& name, std::string& output);
	void GMSetRoleNum(UINT32 num) { m_gmRoleNum = num; }
	UINT32 GetTeamRoleNum();

private:
	UINT64 NewId();
	SkyTeam* CreateSkyTeam(const std::vector<UINT64>& roleIds, UINT64 creatorId, const std::string& name);

private:
	UINT32 m_id;
	UINT32 m_lastUpdateTime;
	std::unordered_map<UINT64, SkyTeam> m_teams;
	std::unordered_map<UINT64, UINT64> m_roleId2teamId;
	std::unordered_set<std::string> m_usedName;
	MapList<UINT64> m_dirtyList;
	UINT32 m_gmRoleNum;
};

#endif
