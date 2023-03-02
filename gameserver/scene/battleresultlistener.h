#ifndef __BATTLERESULTLISTENER_H__
#define __BATTLERESULTLISTENER_H__

#include "gamelogic/stagereward.h"
#include "battle/ptcg2c_battleresultntf.h"
#include "scene/ptcg2c_battlewatcherntf.h"
#include "scene/ptcg2m_synscenerewardg2m.h"
#include "pb/project.pb.h"

class Scene;
class Role;
class DummyRole;
class CombatStatistics;

class NormalBattleResultListener : public IStageRewardListener
{
public:
	// watcher
	PtcG2C_BattleWatcherNtf watchptc;
	// role
	PtcG2C_BattleResultNtf BattlePtc;
	KKSG::StageRoleResult *pRoleResult;
	// ms
	PtcG2M_SynSceneRewardG2M msptc;

	NormalBattleResultListener(Scene *pScene, bool win);

	void SetInitStatus(DummyRole *pRobot);
	void SetInitStatus(UINT64 nRoleID, UINT64 gid, bool IsLeader,const std::string &szName,UINT32 nLv,UINT32 nExp,UINT32 nPro,UINT32 nServerID);
	void SetInitStatus(const KKSG::PvpRoleInfo& info);
	void SetDummyRoleEndState(DummyRole *pRobot);

	void SetInitStatus(Role *pRole, UINT64 gid = 0, bool IsLeader = false);
	void SetRoleEndState(Role *pRole);

	virtual void SetSceneRank(int Rank, UINT32 firststar = 0);
	virtual void AddItem(int itemID, int itemCount, bool isbind, int AddReason = NormalStageReward);
	virtual void ModifyExp(Role* role);
	virtual void SetExpSeal(bool isexpseal);

	void SetIsHelper(bool helper);
	void SetIsSelectBoxExcept(bool istrue);

	void SetTimeSpan(UINT32 timespan);
	void SetScore(int score, int vipscore);
	void SetVipLevel(int viplevel);
	void SetEnd(bool bEnd);
	void SetStatisticsInfo(const CombatStatistics &stat);
	void SetGoblinResult(int rank, bool bGetGuildBonus);
	void SetArenaup(UINT32 arenaup, bool ismissed);
	void SetBossRush(UINT32 currentmax, UINT32 lastmax);
	void SetTowerResult(int towerFloor, bool isNewRecord);
	void SetGuildExp(UINT32 exp);
	void SetResWar(UINT32 nRes);
	void SetGuildCon(UINT32 con);
	void SetGuildDargon(UINT32 dargon);

	void SetBox(RoleBoxRewardInfo* boxinfo);

	KKSG::DragonExpResult* GetDEResult();

	KKSG::PkResult* GetPkResult();
	KKSG::InvFightBattleResult* GetInvFightResult();
	KKSG::CustomBattleResult* GetCustomBattleResult();
	KKSG::BigMeleeBattleResult * GetBigMeleeBattleResult();

	KKSG::BattleFieldBattleResult * GetBattleFieldResult();
	KKSG::PVPResult* GetPvpResult();
	KKSG::HorseAward* GetHorseResult();
	KKSG::SkyCityFinalInfo* GetSkyCityResult();
	KKSG::HeroBattleResult* GetHeroBattleResult();

	KKSG::MobaBattleResult* GetMobaBattleResult();
	KKSG::MobaBattleRoleResult* GetMobaBattleRoleResult();

    KKSG::WeekEnd4v4TeamResult* GetWeekEnd4v4TeamResult();
    KKSG::WeekEnd4v4BattleRoleData* GetWeekEnd4v4RoleResult();

	KKSG::GuildExpAndConData* GetGuildInfo();

	void SetIsFinalResult(bool final);

	void SetWatchPvp(UINT64 winuid, UINT64 mvp = 0);
	void SetWatchInfo(Scene* scene);

	void NotifyClient(Role *pRole);
	void NotifyWatcher(Scene* scene);
	void NotifyMS(std::vector<Role*>& roles);
};

#endif // __BATTLERESULTLISTENER_H__