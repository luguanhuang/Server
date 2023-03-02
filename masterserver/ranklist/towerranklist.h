#ifndef __TOWERRANKLIST_H__
#define __TOWERRANKLIST_H__

#include "ranklist.h"

#define TOWERRANKCOUNT 100

class TowerRankInfo
{
public:
	TowerRankInfo(std::vector<UINT64>& roleids);
	~TowerRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);

	std::vector<UINT64>& GetRoleIDs(){return m_roleIDs;}

public:
	std::vector<UINT64> m_roleIDs;
	UINT32 m_towerHardLevel;
	UINT32 m_towerFloor;
	UINT32 m_bestTime;
	UINT32 m_time;
};

struct TowerCompare
{
	bool operator () (const TowerRankInfo* rankinfoa, const TowerRankInfo* rankinfob)
	{
		if (rankinfoa->m_towerHardLevel > rankinfob->m_towerHardLevel)
		{
			return true;
		}
		else if (rankinfoa->m_towerHardLevel == rankinfob->m_towerHardLevel)
		{
			if (rankinfoa->m_towerFloor > rankinfob->m_towerFloor)
			{
				return true;
			}
			else if (rankinfoa->m_towerFloor == rankinfob->m_towerFloor)
			{
				if (rankinfoa->m_bestTime < rankinfob->m_bestTime)
				{
					return true;
				}
				else if (rankinfoa->m_bestTime == rankinfob->m_bestTime)
				{
					if (rankinfoa->m_time < rankinfob->m_time)
					{
						return true;
					}
					return false;
				}

				return false;
			}
			else
				return false;
		}
		else
		{
			return false;
		}
	}
};
class TowerRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
	virtual bool BeforeSaveToDB();
};

class TowerRankList : public CRankList
{
public:
	TowerRankList(UINT32 type);
	virtual ~TowerRankList();

	virtual void Clear();

	virtual inline UINT32 GetClientQueryCount() { return TOWERRANKCOUNT; }
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(std::vector<UINT64>& roleids, UINT32 towerHardLevel, UINT32 towerFloor, UINT32 useTime, UINT32 time);	

	std::set<TowerRankInfo*, TowerCompare>& GetCurRankInfo(){ return m_rankinfos; }	
	
	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	void DeleteRole(UINT64 roleid);

private:
	bool m_ischanged;
	std::set<TowerRankInfo*, TowerCompare> m_rankinfos;
};

#endif