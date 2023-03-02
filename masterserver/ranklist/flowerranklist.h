#ifndef __FLOWERRANKLIST_H__
#define __FLOWERRANKLIST_H__

#include "ranklist.h"

#define FLOWERRANKCOUNT 100

class CRoleSummary;
namespace KKSG
{
	class GetFlowerRewardListRes;
	class TShowTopListRes;
}
class FlowerRankInfo : public CRankInfo
{
public:
	FlowerRankInfo(UINT64 roleid);
	~FlowerRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);

public:
	UINT32 m_flowercount;
	std::map<UINT32, UINT32> m_mapFlowers;
	UINT32 m_time;
};

struct FlowerCompare
{
	bool operator () (const FlowerRankInfo* rankinfoa, const FlowerRankInfo* rankinfob) const
	{
		if (rankinfoa->m_flowercount > rankinfob->m_flowercount)
		{
			return true;
		}
		else if (rankinfoa->m_flowercount == rankinfob->m_flowercount)
		{
			if (rankinfoa->m_time < rankinfob->m_time)
			{
				return true;
			}
			else if (rankinfoa->m_time == rankinfob->m_time)
			{
				return rankinfoa->GetUniqueId() < rankinfob->GetUniqueId();
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
};

class FlowerRankList : public CRankList
{
public:
	FlowerRankList(UINT32 type);
	virtual ~FlowerRankList();

	virtual void Clear();

	bool OnTimer();

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();	
	const FlowerRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);
	virtual void NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uniqueID);

	virtual inline UINT32 GetClientQueryCount() { return FLOWERRANKCOUNT; }

	void OnChange(UINT64 roleid, UINT32 flowercount, std::map<UINT32, UINT32>& flowers, UINT32 time, bool isAddPunished = false);

	void GetSomeTopInfo(int count, std::vector<FlowerRankInfo*>& vecInfo);

	void SendTShowWalkRank(CRoleSummary* role);
	void SendTShowTop3Rank(); 
	void SendTShowTop3Rank(CRoleSummary* role);	

	//IDIP
	void DeleteRole(UINT64 roleid);
	void InsertPunishedRankInfo(std::string info); //插入禁止上榜玩家之前的数据（禁止上榜可能会保留原来的数据）
	std::string GetPunishedRankInfo(UINT64 roleid);

	void SendThisWeekRankReward();

	void SendValentinesDayReward();
	void Send520DayReward();
	void SendActiviyDayReward();
private:
	UINT32 m_timehandle;
	RankTree<FlowerRankInfo*, FlowerCompare> m_rankTree;
};

#endif