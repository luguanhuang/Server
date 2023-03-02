#ifndef _CROSS_LEAGUE_RANK_LIST_H_
#define _CROSS_LEAGUE_RANK_LIST_H_

#include "ranklist.h"

struct CrossLeagueRankInfo :  public CRankInfo
{
public:
	CrossLeagueRankInfo(UINT64 leagueTeamId);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);
	void ToRankKKSG(KKSG::LeagueRankData& data) const;

	UINT32 m_point;
	float m_winRate;
	UINT32 m_time;
};

struct CrossLeagueCompare
{
	bool operator () (const CrossLeagueRankInfo* infoa, const CrossLeagueRankInfo* infob)
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

class CrossLeagueRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class CrossLeagueRankList : public CRankList
{
public:
	CrossLeagueRankList(int type);
	virtual ~CrossLeagueRankList();

	virtual void Clear();
	virtual void RemoveAll();
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 leagueTeamId);
	virtual UINT32 GetSize();
	const CrossLeagueRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);
	virtual void NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uid);

	void OnChange(UINT64 leagueTeamId, UINT32 point, float winRate, UINT32 time);

	void InsertKey(CrossLeagueRankInfo* rankinfo);

	void GetRankInfo(std::vector<CrossLeagueRankInfo*>& vec);
	void GetTopNRankInfo(UINT32 count, std::vector<CrossLeagueRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	void DelLeagueTeam(UINT64 id);

private:
	bool m_ischanged;
	RankTree<CrossLeagueRankInfo*, CrossLeagueCompare> m_rankTree;
};

#endif
