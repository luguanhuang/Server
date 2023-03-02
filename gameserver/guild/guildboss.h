#ifndef __GUILDBOSS_H__
#define __GUILDBOSS_H__

#include "unit/enemy.h"
#include "math/XVector3.h"
#include "table/globalconfig.h"
#include "table/GuildBossRewardTable.h"
#include "table/GuildBossRoleRewardTable.h"
#include "table/GuildBossConfigTable.h"
#include "gamelogic/MultActivityMgr.h"
#include "timer.h"


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

class Enemy;
class GuildBossInfo : public EnemyEventListener , public ITimer
{
public:
	bool Init();
	void Uninit();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	
public:
	GuildBossInfo(UINT64 guildid, UINT32 bossIndex, Enemy* enemy, bool isFirstKill, UINT32 sceneId, std::string guildName, double bossHp);
	virtual ~GuildBossInfo();

public:
	virtual void OnHurt(Enemy* enemy, Unit* castunit, double value);
	virtual void OnDied(Enemy* enemy);

	inline bool IsDead() { return NULL == m_boss; }
	inline bool IsFirstKill() { return m_isFirstKill; }
	inline bool IsFirstKillFinished() { return IsFirstKill() && IsDead(); }
	inline UINT32 GetBossIndex() { return m_bossIndex; }
	inline UINT64 GetGuildId() { return m_guildid; }
	inline UINT32 GetSceneId() {return m_sceneId; }
	inline double GetBossHp() { return m_bossHp; }
	inline std::string GetBossName() { return m_bossName; } 
	inline void DelBossHp(double hp) { if (m_boss != NULL) { m_boss->SubHp(NULL, hp); } }
	void SetEnd(); 
	void RemoveBoss();
	void NoticeBossEnd();
	void DoBossDie();
	void ClearListener();

private:
	void _GiveReward();  // boss杀死直接发奖  否则等时间结束统一发
	void _SendKillMsg(); // 击杀boss 发送消息到ms

public:
	std::unordered_map<UINT64, double> m_roleid2damage;
	std::map<UINT64, double> m_tempDamage;
	double m_dosakaHurt;//孤魂伤害，找不到roleid

private:
	HTIMER m_timehandler;
	bool m_isFirstKill;
	UINT32 m_begintime;
	UINT32 m_bossIndex;
	UINT32 m_sceneId;
	double m_bossHp;
	UINT64 m_guildid;
	std::string m_bossName;
	std::string m_guildName;
	Enemy* m_boss;
};

struct ItemDesc;
class Guild;
class Role;
class GuildBossRankList;

class GuildBossMgr// : public ITimer
{
	GuildBossMgr();
	~GuildBossMgr();
	DECLARE_SINGLETON(GuildBossMgr);

public:
	bool Init();
	void Uninit(){}
	void ClearFile();
	bool LoadFile();
	bool CheckFile();
	void AddBoss(Scene* pScene, UINT64 guildId, UINT32 bossIndex, UINT32 curIndex,const std::string &guildName, double bossHp,UINT32 count);
	const std::unordered_map<UINT64, vector<GuildBossInfo*> >& GetInfoMap() { return m_guildid2bossinfo; }
	void ClearAfterEnd();

public:
	time_t GetBeginTime(time_t nowtime);
	time_t GetEndTime(time_t nowtime);
	void OnEnterScene(Role* role);
	void GetRoleJoinReward(UINT64 guildId, std::vector<ItemDesc>& itemVec);
	void GetRoleFirstKillReward(UINT64 guildId, std::vector<ItemDesc>& itemVec);
	void GetRoleLastHurtReward(UINT32 bossIndex, std::vector<ItemDesc>& itemVec);
	void DoGiveReward();
	void BroadCastHint(bool isremove);
	void NoticeCastHint(UINT64 guildId);
	UINT32 GetBeginTimeLeft();
	UINT32 GetEndTimeLeft();
	UINT32 GetAttrCount(UINT64 guildId);
	UINT32 AddAttrCount(UINT64 guildId);	
	std::string GetBossAllName(UINT64 guildId);
	GuildBossInfo* GetGuildBossInfo(UINT64 guild);
	void SetAttrCount(UINT64 guildId,UINT32 nCount);
	UINT32 IsInCDtime(UINT64 guildId);

private:
	float m_rotate;
	Vector3 m_pos;
	std::unordered_map<UINT64, UINT32> m_guildid2attrcount;
	std::unordered_map<UINT64, vector<GuildBossInfo*> > m_guildid2bossinfo;
	GuildBossConfigTable m_bossconf;
	std::map<UINT64, UINT32> m_cdTime;
};

#endif
