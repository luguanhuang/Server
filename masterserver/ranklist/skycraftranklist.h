#ifndef _SKYCRAFT_RANK_LIST_H_
#define _SKYCRAFT_RANK_LIST_H_

#include "ranklist.h"

struct SkyCraftRankInfo : public CRankInfo
{
public:
	SkyCraftRankInfo(UINT64 stId);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	void ToRankKKSG(KKSG::SkyCraftRankData& data) const;
	void SetData(UINT32 point, float winRate, UINT32 time);

	UINT32 m_point;
	float m_winRate;
	UINT32 m_time;
};

struct SkyCraftCompare
{
	bool operator() (const SkyCraftRankInfo* infoa, const SkyCraftRankInfo* infob)
	{
		if (infoa->m_point > infob->m_point)
		{
			return true;
		}
		if (infoa->m_point < infob->m_point)
		{
			return false;
		}
		if (infoa->m_winRate > infob->m_winRate)
		{
			return true;
		}
		if (infoa->m_winRate < infob->m_winRate)
		{
			return false;
		}
		if (infoa->m_time < infob->m_time)
		{
			return true;
		}
		if (infoa->m_time > infob->m_time)
		{
			return false;
		}
		return infoa->GetUniqueId() < infob->GetUniqueId();
	}
};

class SkyCraftRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class SkyCraftRankList : public CRankList
{	
public:
	SkyCraftRankList(int type);
	virtual ~SkyCraftRankList();

	virtual void Clear();
	virtual void RemoveAll();
	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 stId);
	virtual UINT32 GetSize();
	const SkyCraftRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);
	virtual void NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uid);

	void OnChange(UINT64 stId, UINT32 point, float winRate, UINT32 time);

	void InsertKey(SkyCraftRankInfo* rankinfo);

	void GetRankInfo(std::vector<SkyCraftRankInfo*>& vec);
	void GetTopNRankInfo(UINT32 count, std::vector<SkyCraftRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	void DelSkyTeam(UINT64 id);

protected:
	bool m_ischanged;
	RankTree<SkyCraftRankInfo*, SkyCraftCompare> m_rankTree;
};

#endif
