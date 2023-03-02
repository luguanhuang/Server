#ifndef _SKY_TEAM_H_
#define _SKY_TEAM_H_

#include "pb/project.pb.h"
#include "skyteamrecord.h"

struct SkyTeamMember
{
	UINT64 roleId;
	SkyTeamMember(): roleId(0) {}

	void FromKKSG(const KKSG::SkyTeamMemberData& data)
	{
		roleId = data.roleid();
	}

	void ToKKSG(KKSG::SkyTeamMemberData& data)
	{
		data.set_roleid(roleId);
	}

	void FillBattleRole(KKSG::SkyCraftRoleBrief& data);
	void FillMemberInfo(KKSG::SkyTeamMemberInfo& data);
};

class CRole;
class CProtocol;
class SkyTeam
{
public:
	SkyTeam();
	~SkyTeam();

	void LoadFromDB(const KKSG::SkyTeamSaveData& data);
	void SaveToDB(KKSG::SkyTeamSaveData& data, bool all = false);
	void LoadBaseFromDB(const KKSG::SkyTeamSaveData& data);
	void SaveBaseToDB(KKSG::SkyTeamSaveData& data);

	void AddToDB();
	void UpdateToDB();
	void DelToDB();

	void OnLoaded();
	void OnCreate();
	void OnDayPass();
	void OnDissolve();

	void SetId(UINT64 id) { m_id = id; }
	UINT64 GetId() { return m_id; }
	void SetName(const std::string& name) { m_name = name; }
	const std::string& GetName() { return m_name; }
	void SetMemberRoleIds(const std::vector<UINT64>& roleIds);
	void SetCreatorId(UINT64 roleId) { m_creatorId = roleId; }
	UINT64 GetCreatorId() { return m_creatorId; }

	void SetDirty(bool dirty = true);

	bool HasMember(UINT64 roleId);
	SkyTeamMember* GetMember(UINT64 roleId);
	std::vector<SkyTeamMember>& GetAllMember() { return m_members; }
	KKSG::ErrorCode LeaveMember(UINT64 roleId, bool force = false);

	void BroadCast(const CProtocol& ptc);

	UINT32 GetScore() { return m_score; }
	void SetScore(UINT32 score);

	KKSG::SkyTeamState GetState();
	void SetState(KKSG::SkyTeamState state);
	void SetMatchTeamId(UINT32 teamId) { m_matchTeamId = teamId; }
	UINT32 GetMatchTeamId() { return m_matchTeamId; }
	void SetBattleStart(UINT32 sceneId, UINT32 gsLine, bool isCross = false);
	void SetBattleReadyEnd(UINT32 sceneId);
	void EnterBattleScene(std::vector<CRole*>& roles);

	inline SkyTeamRecord& GetTeamRecord() { return m_record; }

	void GMSetScore(UINT32 score);

	void FillBattleData(KKSG::SkyCraftTeamData& data);
	void FillEliTeamInfo(KKSG::SCEliTeamInfo& data);
	void FillTeamDetail(KKSG::SkyTeamDetail& data);

private:
	bool m_dirty;
	UINT64 m_id;
	UINT64 m_creatorId;
	UINT32 m_createTime;
	std::string m_name;

	std::vector<SkyTeamMember> m_members;

	UINT32 m_score;

	// 战斗和匹配相关状态数据
	KKSG::SkyTeamState m_state;		// 状态
	UINT32 m_stateStartTime;		// 状态开始时间
	UINT32 m_matchTeamId;			// 匹配队伍id
	UINT32 m_sceneId;				// 战场id
	UINT32 m_gsLine;				// 场景所在gsLine

	SkyTeamRecord m_record;
};

#endif