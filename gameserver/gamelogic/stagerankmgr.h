#ifndef __STAGERANKMGR_H__
#define __STAGERANKMGR_H__

#include "table/StageRankTable.h"
#include "table/NestRewardTable.h"
#include "table/NestScoresTable.h"

enum SceneRank
{
	RANK_EMPTY = 0,

	RANK_S = 0x01,
	RANK_SS_1 = 0x03,
	RANK_SS_2 = 0x05, 
	RANK_SSS = 0x07,
	RANK_A = 0x08,
	RANK_B = 0x10,
};

struct StageRankInfo
{
	int rank;
	int score;
	int vipscore;

	UINT32 hppercent;
	UINT32 timespan;
	UINT32 found;
	UINT32 behit;
	UINT32 npchp;
	UINT32 combo;
	UINT32 deathcount;
	int killenemyscore;
	int totalkillenemyscore;
	//bossrush
	UINT32 BRsucceedcount;
	std::vector<UINT32> BRgive;
	// tower 
	UINT32 towerfloor;
	bool isnewrecord;


	StageRankInfo()
	{
		Reset();
	}

	void Reset()
	{
		rank = RANK_EMPTY;
		score = 0;
		vipscore = 0;
		hppercent = 0;
		timespan =0;
		found= 0;
		behit = 0;
		npchp = 0;
		combo = 0;
		deathcount = 0;
		killenemyscore = 0;
		totalkillenemyscore = 0;
		BRsucceedcount = 0;
		isnewrecord = false;
		towerfloor = 0;
	}
};

class Role;
class Scene;
struct ItemDesc;
class StageRankMgr
{
	StageRankMgr();
	~StageRankMgr();
	DECLARE_SINGLETON(StageRankMgr);
public:
	bool Init();
	void Uninit();

	void ClearFile();
	bool CheckFile();
	bool LoadFile();

public:
	bool HasRank(UINT32 sceneid);
	StageRankInfo CalculateNestRankInfo(Scene *pScene, Role *pRole, int dmgRank, int pptRank, int treatrank);
	void CalculateRankInfo(UINT32 sceneid, StageRankInfo& info);

	//ItemDesc GetNestChestReward(UINT32 sceneid, UINT32 pos);
	//std::vector<Sequence<UINT32, 2>>* GetNestReward(UINT32 sceneid);

	bool Compare(int rank1, int rank2);

	UINT32 GetStarByRank(int rank);
	UINT32 GetConcernNpcId(UINT32 sceneid);

private:
	int CalculateRank(bool star2, bool star3);
	bool CheckOneCondition(Sequence<uint, 3>& cond, StageRankInfo& info);

private:
	StageRankTable m_oTable;
	//NestRewardTable m_nesttable;
	NestScoresTable m_nestScoreTable;
};

#endif