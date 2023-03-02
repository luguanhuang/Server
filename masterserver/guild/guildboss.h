#ifndef __GUILDBOSS_H__
#define __GUILDBOSS_H__
#include "timer.h"
#include "scene/scenecreate.h"
#include "table/GuildBossConfigTable.h"
#include "guild/guild.h"
#include "ranklist/guildbossrank.h"
#include "table/GuildBossRewardTable.h"
#include "mail/maildata.h"



struct GuildBossRoleDamage
{
	UINT64 roleid;
	double damage;
	GuildBossRoleDamage()
	{
		roleid = 0;
		damage = 0;
	}
	bool operator < (const GuildBossRoleDamage& info) const
	{
		if (damage != info.damage)
		{
			return damage > info.damage;
		}
		else
		{
			return roleid < info.roleid;
		}
	}
};

class CGuildBossListener : public ICreateBattleListener
{
	DECLARE_SINGLETON(CGuildBossListener);
public:
	bool Init();
	void Uninit();

public:
	virtual void OnCreateReply(CScene* scene, const KKSG::CreateBattleParam& roParam, const KKSG::CreateBattleRes& res);
};

class CGuildBossMgr : public ITimer
{
	CGuildBossMgr();
	~CGuildBossMgr();
	DECLARE_SINGLETON(CGuildBossMgr);

public:
	enum EGuildBossSceneState  // 状态枚举
	{
		BeginPre	= 0,
		Begin		= 1,
		WaitEnd	    = 2,
		End		    = 3,
	};

	static const UINT32 END_DELAY_TIME = 7;       // 结束等待时间
	static const UINT32 GUILD_BOSS_SCENE_ID = 5;  // 公会大厅id
	static const UINT32 GUILD_BOSS_SYS_ID = 85;   // 公会boss系统id

public:
	bool Init();
	void Uninit();
	void ClearFile();
	bool LoadFile();
	bool CheckFile();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void DelGuild(UINT64 guildId);
	inline bool IsGuildMapZero() { return m_guildSceneMap.empty(); }
	inline const std::map<UINT64, std::map<UINT32, UINT32> >& getGuilds() { return m_guildSceneMap; } 
	inline GuildBossRankList* GetRankList() { return m_ranklist; }
	inline void AddFinishGuild(UINT64 guildId) { m_finishGuild.insert(guildId); }

	bool IsSceneOpen(UINT64 guildId);
	bool CanEnterScene(UINT64 guildId);
	bool IsFinish(UINT64 guildId);
	void HintNotify(Guild* pGuild, bool isMove);
	void AddGuildScene(UINT64 guildId, UINT32 sceneId);
	void GiveLeaderReward();
	void DoGuildOpenGuildBoss(UINT64 guildId); // 活动期间开启公会boss处理
	void CheckRoleOnline(UINT64 roleId);
	void RemoveDamageRank(UINT64 roleId);
	void EnterScene(CRole* role, UINT32 sceneid, UINT32 gsline);
	void PushRoleToScene(UINT64 guildId, UINT32 sceneid, UINT32 gsline); // 创建成功后拉等待中的玩家进入场景
	void RoleEnterDelay(UINT64 roleid);

	UINT32 GetBeginTimeLeft();
	UINT32 GetEndTimeLeft();
	UINT32 getNextBossIndex(Guild* guild);
	UINT32 getNeedGuildLvl(Guild* guild);
	UINT32 getNeedBoosIndex(Guild* guild);
	UINT32 GetSceneId(UINT64 guildId, UINT64 roleId);
	UINT32 GetAttrCount(UINT64 guildId);
	std::map<UINT64, std::set<GuildBossRoleDamage> >& GetGuildDamage();
	void AddGuildBossSceneNum(UINT32 sceneId, UINT64 roleId);
	void GiveAuctReward();
	void DelGuildBossHp(UINT64 guildId, double delHp, UINT32 sceneid, UINT64 roleId);
	void AddAttrCount(UINT64 guildId, UINT32 sceneId);
	bool IsJoinOtherBefoer(UINT64 roleId);
	void OnGsClose(UINT32 nSceneID);

private:
	inline void _DoClearBeforeStart() { _ClearDamageRank(); _ClearGuildRank(); _ClearGuild(); _ClearWaitRoleMap(); _ClearFinishGuild(); _ClearBossHp();}  // 开始前的清理工作
	inline void _ClearGuild() { m_guildSceneMap.clear(); m_lastHurt.clear(); m_beginTime.clear(); m_guildid2attrcount.clear(); m_sceneRole.clear();}
	inline void _ClearDamageRank() { m_guildDamage.clear(); }
	inline void _ClearGuildRank() { m_ranklist->Clear(); }
	inline void _ClearWaitRoleMap() { m_waitRoleMap.clear(); }
	inline void _ClearFinishGuild() { m_finishGuild.clear(); }
	inline void _ClearBossHp() { m_guildBossHp.clear(); }

	bool _IsOnTime(EGuildBossSceneState state);
	void _PrepareNotice();         // 开始提示公告
	void _NoticeEndToGSN();
	void _NoticeDoEndToGSN();
	void _CreateGuildBossScene(UINT64 roleId);  // 创建公会boss场景
	void _GetGuildReward(UINT32 rank, std::vector<ItemDesc>& itemVec, bool isChief/*true会长 false副会长*/, std::map<UINT32, UINT32>& guildReward);
	void _GetLine(std::set<UINT32>& lineSet);
	void _GetGuildLine(std::set<UINT32>& lineSet, UINT64 guildId);
	void _NoticeGuildBossStart(UINT64 guildId, UINT32 min);
	void _NoticeGuildBossBegin();
	void _DoGuildBossEnd(UINT64 guildId);
	void _GiveLastHurtReward(UINT64 roleId);
	void _UpdateGuildRank(UINT64 guildId, UINT32 nowTime = 0);
	double _GetGuildBossHp(UINT64 guildId);
	UINT64 _GetDpsRoleId(UINT64 guildId);
	void DoTxLog(UINT64 guildId);


private:
	HTIMER m_timehandler;
	EGuildBossSceneState m_state;             // timer的状态
	GuildBossRankList* m_ranklist;            // 公会boss排行榜
	std::map<UINT64, UINT64> m_lastHurt;      // 最后一击
	std::set<UINT64> m_finishGuild;           // 已经完成的guild
	std::map<UINT64, double> m_guildBossHp;    // boss血量
	std::map<UINT64, UINT32> m_beginTime;     // 开始时间
	std::unordered_map<UINT64, UINT32> m_guildid2attrcount;          // 鼓舞数据
	std::map<UINT64, std::set<UINT64> > m_waitRoleMap;               // 等待玩家map
	std::map<UINT64, std::map<UINT32, UINT32> > m_guildSceneMap;     // 生成的场景id
	std::map<UINT64, std::set<GuildBossRoleDamage> > m_guildDamage;  // 伤害排行榜(不存数据库)
	std::map<UINT32, std::map<UINT64, UINT64> > m_sceneRole;                 // 场景包含的玩家

	GuildBossConfigTable m_bossconf;
	GuildBossRewardTable m_guildreward;
};




#endif