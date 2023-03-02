#ifndef __STAGEREWARD_H__
#define __STAGEREWARD_H__

#include "bagtransition.h"
#include "scene/sceneconfig.h"
#include "math.h"

class Role;
class DummyRole;
class Scene;
struct SceneInfo;

class IStageRewardListener
{
public:
	enum AddItemReason
	{
		FirstStar,
		NormalStageReward,
		GuildReward,
		GuildDragonReward,
		TeamCostReward,
	};

	virtual ~IStageRewardListener(){}
	virtual void SetSceneRank(int Rank, UINT32 firststar = 0) = 0;
	virtual void AddItem(int itemID, int itemCount, bool isbind, int AddReson = NormalStageReward) = 0;
	virtual void SetBossRush(UINT32 currentmax, UINT32 lastmax) { }
	virtual void ModifyExp(Role* role) = 0;
	virtual void SetExpSeal(bool isexpseal) = 0;
};

///> ¹Ø¿¨½±Àø
class StageReward
{
public:
	StageReward(Role *pRole, Scene* scene, UINT32 scenetype);
	~StageReward();

	void SetRank(int rank) { m_Rank = rank; m_pListener->SetSceneRank(rank); }
	void SetExpTrans();
	void SetListener(IStageRewardListener *l) { m_pListener = l; }
	
	void GiveReward();
	void GiveTeamCostReward(ItemDesc item);

	void GiveSweepReward(SceneInfo *pSceneInfo);
	void GiveSweepExp(SceneInfo *pSceneInfo);


	void NotifyExpClient(INT32 nReason, INT32 nSubReason);
	void NotifyClient(INT32 nReason, INT32 nSubReason);

	void OnFirstPass(Scene* scene);

	inline BagGiveItemTransition* GetBagTrans() { return &m_transition; }
private:
	void GiveBossRushReward(Scene *pScene);
	void GiveScenePickedDoodad(Scene *pScene);
	void GiveSyncScenePickedDoodad(Scene *pScene);
	bool CheckNotGiveTowerReward(Scene* pScene);
	void GiveTowerReward(Scene *pScene);
	void GiveRiskReward(Scene *pScene);
	void GiveHorseRaceReward(Scene *pScene);
	void GiveResWarPVPReward(Scene *pScene);
	void GiveResWarPVEReward(Scene *pScene);
	
	bool GiveFirstReward(SceneInfo* sceneinfo, Scene *scene);
	void GiveCommonReward(SceneInfo *sceneinfo);
	void _GiveSweep(SceneInfo *sceneinfo, int Rank, bool exponly);
	void GiveSpActivityReward(SceneInfo *sceneinfo);

	void PlatShareResult(UINT32 scene_id);

	inline int GetStageCoin(SceneInfo* info) { return (int)(info->m_pConf->Money * m_coinper); };
	inline int GetStageFirstCoin(SceneInfo* info) { return (int)(info->m_pConf->FirstDownMoney * m_coinper); };
	inline int GetStageExp(SceneInfo* info) { return info->m_pConf->Exp; };
	inline int GetStageFirstExp(SceneInfo* info) { return info->m_pConf->FirstDownExp; };

	void AddRandomTaskItem(int itemID, int itemCount);
	void AddItem(UINT32 itemID, UINT32 Count, bool isbind = true, int AddReason = IStageRewardListener::NormalStageReward);
	void AddSweepExp(UINT32 Count, bool isbind = true, int AddReason = IStageRewardListener::NormalStageReward);

	void _GiveGuildReward(Scene *pScene);

	bool WeekNestRewardCount(Scene* pScene);

	bool NotGiveSmallDragonReward(Scene* pScene);

private:
	int  m_Rank;
	double m_coinper;
	Role* m_pRole;
	Scene* m_scene;

	int m_subreason;
	BagGiveItemTransition m_transition;
	BagGiveItemTransition* m_exptransition;
	std::stringstream m_rewardStream;
	IStageRewardListener *m_pListener;
};


class DummyRoleStageReward
{
public:
	DummyRoleStageReward(DummyRole *pRobot)
	{
		m_pRobot= pRobot;
	}
	~DummyRoleStageReward(){};

	void SetListener(IStageRewardListener *l) { m_pListener = l; }

	void SetRank(int rank) { m_pListener->SetSceneRank(rank); }

	DummyRole *m_pRobot;
	IStageRewardListener *m_pListener;
};

#endif // __STAGEREWARD_H__
