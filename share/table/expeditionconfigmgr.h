#ifndef __EXPEDITIONCONFIGMGR_H__
#define __EXPEDITIONCONFIGMGR_H__

#include "table/ExpeditionTable.h"
#include "table/RandomSceneTable.h"
#include "table/TeamCostReward.h"
#include "table/NestWeekList.h"
#include "table/AbyssPartyListTable.h"
#include "table/DragonNestTable.h"

struct ItemDesc;
class ExpeditionConfigMgr
{
	ExpeditionConfigMgr();
	~ExpeditionConfigMgr();
	DECLARE_SINGLETON(ExpeditionConfigMgr)

public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetRandomScene(UINT32 randomID);
	UINT32 GetExpRandomScene(int expID);

	ExpeditionTable::RowData* GetExpeditionConf(int expID);
    ExpeditionTable::RowData* GetTeamType2ExpConf(int teamtype);
	DragonNestTable::RowData * GetDnListConf(int expID);

	const AbyssPartyListTable::RowData* GetAbssParty(const UINT32 nID);

	bool NeedInTs(int expID);
	bool NeedInTs(int expID, UINT32 serverID);
	bool IsGrayExp(int expID);

	UINT32 GetAbyssSceneByExpID(int expID);
	int GetAbyssExpIDByScene(UINT32 sceneTempid);

	UINT32 GetPvpScene();
	UINT32 GetPkTwoScene();
	UINT32 GetHeroBattleScene();
	UINT32 GetMobaBattleScene();
	std::vector<int>& GetFastMatch(){ return m_fastMatch; }

	bool CanDownMatch(ExpeditionTable::RowData* conf);
	std::vector<int>& GetDownMatch(){ return m_downMatch; }

	const Sequence<UINT32, 3>* GetTeamCost(UINT32 expid, UINT32 index);
	ItemDesc GetTeamCostReward(UINT32 count, UINT32 expid);
	UINT32 GetLevelSealTeamCost(UINT32 expid);

	int GetNestWeekCount(int sealType);
	void GetNestWeekExp(int sealType, std::vector<int>& expIDs);

	//star
	void GetAllStarExpByType(UINT32 type, std::map<UINT32, int>& expids);
	bool IsStarExp(ExpeditionTable::RowData* pRow);
	UINT32 GetStarExpStar(ExpeditionTable::RowData* pRow);
	UINT32 GetStarExpType(ExpeditionTable::RowData* pRow);
	UINT32 GetBioHellScene(ExpeditionTable::RowData* pRow);

	UINT32 GetType2Count(int type);

	UINT32 GetExpOpenTime(int expID, bool isBackFlow, UINT32 backFlowLevel, UINT32 srverOpenTime);

private:
	ExpeditionTable::RowData* GetAbyssConfByScene(UINT32 sceneTempid);
	void   BuildRandomSceneGroup();
	void   BuildFastMatch();
	void   BuildDownMatch();
	void   BuildTeamCost();
    void   BuildTeamType2Conf();
	void   BuildStarExp();

private:
	ExpeditionTable m_oExpeditionTable;
	RandomSceneTable m_oRandomSceneTable;
	TeamCostReward m_costrewardtable;
	NestWeekList m_nestWeekTable;
	AbyssPartyListTable m_abyssParty;
	DragonNestTable m_dnListTable;

	struct RandomSceneGroup
	{
		RandomSceneGroup():totalProb(0) {}

		UINT32 totalProb;
		std::map<UINT32, UINT32> prob2SceneID;	// key: ¿€º∆∏≈¬  value: sceneId

		UINT32 GetRandomScene();
	};

	std::vector<int> m_fastMatch;
	std::vector<int> m_downMatch;
	std::map<UINT32, RandomSceneGroup> m_RandomSceneGroup;

    std::unordered_map<int, int> m_TeamType2Exp;//team type --> first config id
	std::map<UINT32, std::map<UINT32, std::vector<Sequence<UINT32, 2>>>> m_expid2reward;

	std::map<UINT32, std::map<UINT32, int> > m_star2exp;//<star,<star,int>>
};

#endif // __EXPEDITIONCONFIGMGR_H__
