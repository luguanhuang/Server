#ifndef __GUILDCASTLEBASE_H__
#define __GUILDCASTLEBASE_H__

#include "cvsreader.h"
#include "define/itemdef.h"

namespace KKSG
{
	enum GCFJvDianType;
	class GCFBattleField;
}

class GuildCastleRankList;
class CRole;
class PtcM2G_GCFSynM2G;

enum GCFGroup
{
	GCF_GROUP_NONE = 0,
	GCF_GROUP_ONE = 1,
	GCF_GROUP_TWO = 2,
};

UINT32 GetGCWaitMapID();

//据点
class GCJvDian
{
public:
	GCJvDian(KKSG::GCFJvDianType type);
	~GCJvDian();

	inline KKSG::GCFJvDianType CType() { return m_type; }
	inline UINT64 Occupier() { return m_occupier; }
	inline UINT32 CTime() { return m_ctime; }
	inline void SetTime(UINT32 t) { m_ctime = t; }

	void ResetOccupy(UINT64 occupier, UINT32 now);

	void TestPrint();

private:
	KKSG::GCFJvDianType m_type;
	UINT64 m_occupier;//公会ID
	UINT32 m_ctime;
};

//战场(包括等待场景)
class GCBattleField
{
public:
	GCBattleField(UINT32 mapID, bool isWait = false);
	~GCBattleField();

	inline UINT32 MapID() { return m_mapID; }
	inline UINT32 SceneID() { return m_sceneID; }
	inline bool IsWaitField() { return m_jvDians.empty(); }
	std::vector<GCJvDian>& JvDians() { return m_jvDians; }

	inline void SetSceneID(UINT32 sceneID) { m_sceneID = sceneID; }
	void ResetJvDian(KKSG::GCFJvDianType type, UINT64 occupier);

	UINT32 CurRoleCount(GCFGroup group);
	void IncreaseRole(UINT64 roleID, GCFGroup group);
	void ReduceRole(UINT64 roleID);
	bool HaveRole(UINT64 roleID);

	UINT32 GetPointMulti(UINT32 now);
	void SetPointMulti(UINT32 multi, UINT32 t);

	void TestPrint();

private:

	UINT32 m_mapID;
	UINT32 m_sceneID;
	std::vector<GCJvDian> m_jvDians;

	std::unordered_map<UINT64, GCFGroup> m_roleGroup;
	std::map<GCFGroup, UINT32> m_groupCount;

	UINT32 m_pointMulti;
	UINT32 m_pMultiTime;//结束时间
};

//公会信息
class GCGuildInfo
{
public:
	GCGuildInfo(UINT64 guildID, const std::string& guildName, UINT32 guildIcon, GCFGroup group);
	~GCGuildInfo();

	inline UINT64 GuildID() { return m_guildID; }
	const std::string& GuildName() { return m_guildName; }
	inline GCFGroup Group() { return m_group; }

	inline UINT32 GetPoint() { return m_point; }
	inline void AddPoint(UINT32 addp) { m_point += addp;}

	void FillInfo(KKSG::GCFGuildBrief& data);

	void TestPrint();

private:
	UINT64 m_guildID;
	std::string m_guildName;
	UINT32 m_guildIcon;
	UINT32 m_point;

	GCFGroup m_group;
};

enum CGFightStage
{
	CGFS_READY,
	CGFS_FIGHT,
	CGFS_END,
};

//一场战斗
class GCOneFight
{
public:
	GCOneFight(UINT64 uid, UINT32 territoryID);
	~GCOneFight();

	inline UINT64 GetUID() { return m_UID; }
	inline UINT32 TerritoryID() { return m_territoryID; }
	inline UINT32 CCTime() { return m_cTime; }
	inline CGFightStage GetStage() { return m_stage; }
	inline UINT64 GetWinGuildID() { return m_winGuildID; }
	void ChangeStage(CGFightStage newStage); 
	bool HaveGuild(UINT64 guildID);
	GCFGroup GetGuildGroup(UINT64 guildID);
	void AddGuildPoint(UINT64 guildID, UINT32 point);

	GuildCastleRankList* GetRankList() { return m_pRankList; }

	bool Init(const std::vector<UINT64>& guilds);
	
	void Update();

	void CreateSceneBegin();
	void CreateSceneEnd(UINT32 mapID, UINT32 sceneID);

	GCBattleField* GetBattleFieldByMapID(UINT32 mapID);
	GCBattleField* GetBattleFieldBySceneID(UINT32 sceneID);

	UINT32 GetLeftTime();
	UINT32 GetMaxGroupPoint();

	void KillCountSyn(UINT64 roleID, UINT32 killCount);

	void FillCreateSceneCommon(KKSG::GCFCreateBattleInfo& data);
	void FillZhanChInfo(KKSG::GCFZhanChBriefInfo& data, GCBattleField& field, GCFGroup myGroup, UINT32 now);
	void FillReadyInfo(KKSG::GCFReadyInfoRes& data, UINT64 roleID);
	void FillFightInfo(KKSG::GCFFightInfoRes& data, UINT64 roleID, UINT32 mapID);
	void FillBattleReport(KKSG::GCFCommonRes& data, UINT64 roleID);
	void FillResult(KKSG::GCFCommonRes& data, UINT64 roleID);

	void TestPrint();
	void TestAddPoint(UINT32 add);
	void TestMakeMultiPoint(UINT32 mapID, UINT32 multi, UINT32 endTime);
	
private:
	void MakeStop();
	void MakeMultiPoint(UINT32 mapID, UINT32 multi, UINT32 endTime);
	void BroadcastSyn(PtcM2G_GCFSynM2G& ptc);

	void MulPointNotice(UINT32 mapID, UINT32 multi);
	void CheckGuildWinNotice();

	void BreakDrawPoint(GCGuildInfo* pWinGuild);
	void CheckAddPoint();
	void CheckRandMulPoint();

	std::string GetGuildName(UINT64 guildID);
	GCGuildInfo* GetGuildInfo(UINT64 guildID);
	GCGuildInfo* GetWinGuild();
	GCGuildInfo* GetBettleGuild(GCGuildInfo* guild11, GCGuildInfo* guild22);

	UINT64 m_UID;
	UINT32 m_territoryID;

	CGFightStage m_stage;
	UINT32 m_cTime;//不同阶段的开始时间 
	UINT64 m_winGuildID;

	GuildCastleRankList* m_pRankList;
	std::vector<GCGuildInfo> m_guildInfos;
	std::vector<GCBattleField> m_battleFields;

	UINT32 m_lastMulPointTime;
	UINT32 m_lastGuildWinNoticeTime;
};

struct GCGResult
{
	GCGResult()
	{
		m_territoryID = 0;
		m_isWin = false;
	}
	UINT32 m_territoryID;
	bool m_isWin;
};

enum GCJoinRewardType
{
	GC_JRT_ATTACK_WIN = 1,
	GC_JRT_ATTACK_LOSE = 2,
	GC_JRT_DEFENT_WIN = 3,
	GC_JRT_DEFENT_LOSE = 4,
};

class GCGuildJoin
{
public:
	GCGuildJoin();
	~GCGuildJoin();

	void AddRole(UINT64 roleID);
	const std::unordered_set<UINT64>& AllRoles() { return m_roles; }

	void SetAttack(UINT32 territoryID, bool haveFight);
	void SetDefent(UINT32 territoryID, bool haveFight);
	GCGResult GetDefent();
	GCGResult GetAttack();
	void SetResult(UINT32 territoryID, bool win);
	void GiveReward(UINT64 guildID);
	void GetRoles(std::vector<UINT64> &refRoles);

private:
	void GiveJoinReward(UINT32 territoryID, GCJoinRewardType rewardType, UINT64 guildID);
	void SendMail(const std::vector<ItemDesc>& mailReward, UINT64 roleID, int mailID, const std::string& terrName);

	std::unordered_set<UINT64> m_roles;//参与者(目前用不到 奖励发给所有人)
	GCGResult m_attackResult;
	GCGResult m_defentResult;
};

enum GCActState
{
	GCF_ACT_S_NONE,
	GCF_ACT_S_RUNNING,
	GCF_ACT_S_ENE,
};

class GCActStateMgr
{
public:
	GCActStateMgr();
	~GCActStateMgr();

	void ChangeState(GCActState state, UINT32 now);
	inline GCActState GetState() { return m_state; }
	inline UINT32 GetTime() { return m_ctime; }

private:
	GCActState m_state;
	UINT32 m_ctime;
};

#endif