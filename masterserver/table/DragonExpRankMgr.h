#ifndef __DRAGONEXPRANKMGR_H__
#define __DRAGONEXPRANKMGR_H__

#include "table/DragonExpRankReward.h"
#include "mail/maildata.h"
#include "timer.h"

#define RANDOMGROUPCOUNT 5

namespace KKSG
{
	class DERankRes;
	class DERank;
}

typedef std::pair<UINT32, UINT64> DPPAIR;

class DragonExpGroup
{
public:
	DragonExpGroup();

	void SetGroup(int group);
	int GetGroup(){ return m_groupid; }

	void AddRole(UINT64 roleid);
	bool HaveRole(UINT64 roleid);

	bool IsFull();
	bool IsEmpty(){ return m_roleids.empty(); }
	void Clear();

	void TestPrint();

	int GetRoleGroupRank(UINT64 roleid);
	void GetRankAllRoleid(std::vector<UINT64>& roleids);
private:
	int m_groupid;
	std::unordered_set<UINT64> m_roleids;//根据rank先后
};
class DEPRankList;

class DragonExpRankMgr : public ITimer
{
	DragonExpRankMgr();
	~DragonExpRankMgr();
	DECLARE_SINGLETON(DragonExpRankMgr);

public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void InitGroupByRankList();

	void SynDragonExpProgress(UINT64 roleid, UINT32 sceneid, int avghppercent, bool isbattleend);
	void FillRankData(UINT64 roleid, KKSG::DERankRes& roRes);

	void GiveRankReward();
	void ResetGroup();

	DEPRankList* GetDEPRankList();

	void TestPrint();

private:

	void FillRankInfo(UINT64 roleid, UINT32 grouprank, KKSG::DERank& info, DEPRankList* ranklist);
	void FillRankReward(UINT32 grouprank, KKSG::DERank& info);
	void GetRankReward(UINT32 grouprank, std::vector<ItemDesc>& rewards);
	UINT32 GetGiveRewardLeftTime(time_t now);
	int AddNewRole(UINT64 roleid);
	DragonExpGroup* GetGroupInfo(int groupid);

	void ReadDBData();
	void WriteDBData();

	std::map<int, DragonExpGroup> m_groups;

	DragonExpRankReward m_rrTable;
	time_t m_lastGiveRewardTime;

	HTIMER m_handler;
};

#endif