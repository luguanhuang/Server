#ifndef __RANDOMBOSSMGR_H__
#define __RANDOMBOSSMGR_H__

struct BossProb
{
	int bossID;
	int prob;
};

struct RandomBossGroup
{
	int totalProb;
	std::vector<BossProb> bosses;

	RandomBossGroup()
	{
		totalProb = 0;
	}

	void AddBossProb(int bossID, int Prob)
	{
		totalProb += Prob;
		BossProb bp;
		bp.bossID = bossID;
		bp.prob = Prob;
		bosses.push_back(bp);
	}

	int ChooseRandomBossID();

	void Reset()
	{
		totalProb = 0;
		bosses.clear();
	}
};

class RandomBossMgr
{
	RandomBossMgr();
	~RandomBossMgr();
	DECLARE_SINGLETON(RandomBossMgr)

public:

	bool Init();
	void Uninit();

	int GetRandomBoss(int RandomID);

private:

	std::unordered_map<int, RandomBossGroup> m_oBossGroups;
};

#endif // __RANDOMBOSSMGR_H__