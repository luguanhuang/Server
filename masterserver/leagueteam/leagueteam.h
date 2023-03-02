#ifndef _LEAGUE_TEAM_H_
#define _LEAGUE_TEAM_H_

#include "leagueteambattlerecord.h"

struct LeagueTeamMember
{
	UINT64 roleId;
	UINT32 lastBattleTime;

	LeagueTeamMember(): roleId(0), lastBattleTime(0) {}

	void FromKKSG(const KKSG::LeagueTeamMemberData& data)
	{
		roleId = data.roleid();
		lastBattleTime = data.last_battle_time();
	}

	void ToKKSG(KKSG::LeagueTeamMemberData& data)
	{
		data.set_roleid(roleId);
		data.set_last_battle_time(lastBattleTime);
	}

	void FillMemberDetail(KKSG::LeagueTeamMemberDetail& detail);
	void FillBattleRole(KKSG::LeagueBattleRoleBrief& data);
};

class CRole;
class CProtocol;
class LeagueTeam
{
public:
	LeagueTeam();
	~LeagueTeam();

	void LoadFromDB(const KKSG::LeagueTeamSaveData& data);
	void LoadBaseFromDB(const KKSG::LeagueTeamSaveData& data);
	void SaveBaseToDB(KKSG::LeagueTeamSaveData& data);

	void OnLoaded();
	void OnCreate();
	void OnDayPass();
	void OnDissolve();

	void AddToDB();
	void UpdateToDB();
	void DelToDB();

	void SetId(UINT64 id) { m_id = id; }
	UINT64 GetId() { return m_id; }
	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() { return m_name; }
	void SetMemberRoleIds(const std::vector<UINT64>& roleIds);
	void SetCreatorId(UINT64 roleId) { m_creatorId = roleId; }
	UINT64 GetCreatorId() { return m_creatorId; }

	void FillLeagueTeamDetail(KKSG::LeagueTeamDetail& detail);
	void FillLeagueTeamBattleData(KKSG::LeagueBattleTeamData& data);
	void FillCrossLeagueTeamInitData(KKSG::CrossLeagueTeamData& data);

	void SetDirty(bool dirty = true);

	bool HasMember(UINT64 roleId);
	LeagueTeamMember* GetMember(UINT64 roleId);
	std::vector<LeagueTeamMember>& GetAllMember() { return m_members; }
	KKSG::ErrorCode LeaveMember(UINT64 roleId, bool force = false);

	void BroadCast(const CProtocol& ptc);

	UINT32 GetScore() { return m_score; }
	UINT32 GetCrossScore() { return m_crossScore; }
	void SetScore(UINT32 score);
	void SetCrossScore(UINT32 score);

	KKSG::LeagueTeamState GetState();
	void SetState(KKSG::LeagueTeamState state);
	void SetMatchTeamId(UINT32 teamId) { m_matchTeamId = teamId; }
	UINT32 GetMatchTeamId() { return m_matchTeamId; }
	void SetBattleStart(UINT32 sceneId, UINT32 gsLine, bool isCross = false);
	void SetBattleReadyEnd(UINT32 sceneId);
	void EnterBattleScene(std::vector<CRole*>& roles);

	void GMSetScore(UINT32 score);

	// 同步跨服战队上的数据
	void SyncCrossData(const KKSG::CrossLeagueTeamData& data);


private:
	bool m_dirty;
	UINT64 m_id;
	UINT64 m_creatorId;
	UINT32 m_createTime;
	std::string m_name;
	std::vector<LeagueTeamMember> m_members;
	UINT32 m_score;
	UINT32 m_crossScore;

	// 战斗和匹配相关状态数据
	KKSG::LeagueTeamState m_state;	// 状态
	UINT32 m_stateStartTime;		// 状态开始时间
	UINT32 m_matchTeamId;			// 匹配队伍id
	UINT32 m_sceneId;				// 战场id
	UINT32 m_gsLine;				// 场景所在gsLine
	bool   m_isBattleReadyEnd;		// 是否准备结束
	bool   m_isBattleCross;			// 是否跨服

#define LEAGUE_TEAM_MODULE(module) \
	private:\
	module m_##module; \
	public: \
	module& Get##module() { return m_##module; }
#include "leagueteammodule.h"
#undef LEAGUE_TEAM_MODULE
};

#endif