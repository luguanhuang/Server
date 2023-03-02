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
	enum EGuildBossSceneState  // ״̬ö��
	{
		BeginPre	= 0,
		Begin		= 1,
		WaitEnd	    = 2,
		End		    = 3,
	};

	static const UINT32 END_DELAY_TIME = 7;       // �����ȴ�ʱ��
	static const UINT32 GUILD_BOSS_SCENE_ID = 5;  // �������id
	static const UINT32 GUILD_BOSS_SYS_ID = 85;   // ����bossϵͳid

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
	void DoGuildOpenGuildBoss(UINT64 guildId); // ��ڼ俪������boss����
	void CheckRoleOnline(UINT64 roleId);
	void RemoveDamageRank(UINT64 roleId);
	void EnterScene(CRole* role, UINT32 sceneid, UINT32 gsline);
	void PushRoleToScene(UINT64 guildId, UINT32 sceneid, UINT32 gsline); // �����ɹ������ȴ��е���ҽ��볡��
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
	inline void _DoClearBeforeStart() { _ClearDamageRank(); _ClearGuildRank(); _ClearGuild(); _ClearWaitRoleMap(); _ClearFinishGuild(); _ClearBossHp();}  // ��ʼǰ��������
	inline void _ClearGuild() { m_guildSceneMap.clear(); m_lastHurt.clear(); m_beginTime.clear(); m_guildid2attrcount.clear(); m_sceneRole.clear();}
	inline void _ClearDamageRank() { m_guildDamage.clear(); }
	inline void _ClearGuildRank() { m_ranklist->Clear(); }
	inline void _ClearWaitRoleMap() { m_waitRoleMap.clear(); }
	inline void _ClearFinishGuild() { m_finishGuild.clear(); }
	inline void _ClearBossHp() { m_guildBossHp.clear(); }

	bool _IsOnTime(EGuildBossSceneState state);
	void _PrepareNotice();         // ��ʼ��ʾ����
	void _NoticeEndToGSN();
	void _NoticeDoEndToGSN();
	void _CreateGuildBossScene(UINT64 roleId);  // ��������boss����
	void _GetGuildReward(UINT32 rank, std::vector<ItemDesc>& itemVec, bool isChief/*true�᳤ false���᳤*/, std::map<UINT32, UINT32>& guildReward);
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
	EGuildBossSceneState m_state;             // timer��״̬
	GuildBossRankList* m_ranklist;            // ����boss���а�
	std::map<UINT64, UINT64> m_lastHurt;      // ���һ��
	std::set<UINT64> m_finishGuild;           // �Ѿ���ɵ�guild
	std::map<UINT64, double> m_guildBossHp;    // bossѪ��
	std::map<UINT64, UINT32> m_beginTime;     // ��ʼʱ��
	std::unordered_map<UINT64, UINT32> m_guildid2attrcount;          // ��������
	std::map<UINT64, std::set<UINT64> > m_waitRoleMap;               // �ȴ����map
	std::map<UINT64, std::map<UINT32, UINT32> > m_guildSceneMap;     // ���ɵĳ���id
	std::map<UINT64, std::set<GuildBossRoleDamage> > m_guildDamage;  // �˺����а�(�������ݿ�)
	std::map<UINT32, std::map<UINT64, UINT64> > m_sceneRole;                 // �������������

	GuildBossConfigTable m_bossconf;
	GuildBossRewardTable m_guildreward;
};




#endif