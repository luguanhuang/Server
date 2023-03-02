#ifndef __HERORANKLIST_H__
#define __HERORANKLIST_H__

#include "ranklist.h"
#include "arenastar/arenastarbase.h"

class HeroRankInfo : public CRankInfo
{
public:
	HeroRankInfo(UINT64 roleid);
	virtual ~HeroRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	void InfoPrint();

public:
	UINT32 m_time;
	KKSG::HeroRankData m_rankData;
};

struct HeroRankCompare
{
	bool operator () (const HeroRankInfo* rankinfoa, const HeroRankInfo* rankinfob)
	{
		if (rankinfoa->m_rankData.elopoint()!= rankinfob->m_rankData.elopoint())
		{
			return rankinfoa->m_rankData.elopoint() > rankinfob->m_rankData.elopoint();
		}
		if ((UINT64)rankinfoa->m_rankData.winnum()* rankinfob->m_rankData.totalnum() != (UINT64)rankinfob->m_rankData.winnum() * rankinfoa->m_rankData.totalnum())
		{
			return (UINT64)rankinfoa->m_rankData.winnum() * rankinfob->m_rankData.totalnum() > (UINT64)rankinfob->m_rankData.winnum() * rankinfoa->m_rankData.totalnum();
		}
		if (rankinfoa->m_time != rankinfob->m_time)
		{
			return rankinfoa->m_time < rankinfob->m_time;
		}
		return rankinfoa->GetUniqueId() < rankinfob->GetUniqueId();
	}
};

///////////////////////////////////////////////////
class HeroBattleRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class HeroBattleRankList : public CRankList, public ArenaStarRankInterface
{
public:
	HeroBattleRankList(int type);
	virtual ~HeroBattleRankList();

	virtual void Clear();
	virtual void RemoveAll();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	const HeroRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(UINT64 roleid, UINT32 time, const KKSG::HeroRankData& rankData);

	void InsertKey(HeroRankInfo* rankinfo);

	void GetRankInfo(std::vector<HeroRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	virtual void ASGetTopInfo(UINT32 count, std::vector<HeroRankInfo*>& infoVec);
	virtual void ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec);
	void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<HeroRankInfo*, HeroRankCompare> m_rankTree;
};

class LastWeekHeroBattleRankList : public HeroBattleRankList
{
public:
	LastWeekHeroBattleRankList(int type);
	~LastWeekHeroBattleRankList();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();

	virtual void ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank);

	void AddRankInfo(HeroRankInfo* info);
};

#endif