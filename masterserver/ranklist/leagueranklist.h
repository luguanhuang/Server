#ifndef _LEAGUE_RANK_LIST_H_
#define _LEAGUE_RANK_LIST_H_

#include "ranklist.h"
#include "arenastar/arenastarbase.h"

struct LeagueRankInfo :  public CRankInfo
{
public:
	LeagueRankInfo(UINT64 leagueTeamId);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);
	void ToRankKKSG(KKSG::LeagueRankData& data) const;
	void FillTeamData(KKSG::LeagueRankData& data) const;

	void SetData(UINT32 point, float winRate, UINT32 ctime, UINT32 continueWin, UINT32 joinCount);

	void InfoPrint();

	UINT32 m_time;
	KKSG::LeagueRankData m_rankData;
	std::list<UINT64> m_roleids;//仅用于竞技场名人堂
};

struct LeagueCompare
{
	bool operator () (const LeagueRankInfo* infoa, const LeagueRankInfo* infob)
	{
		if (infoa->m_rankData.point() > infob->m_rankData.point())
		{
			return true;
		}
		if (infoa->m_rankData.point() < infob->m_rankData.point())
		{
			return false;
		}
		if (infoa->m_rankData.winrate() > infob->m_rankData.winrate())
		{
			return true;
		}
		if (infoa->m_rankData.winrate() < infob->m_rankData.winrate())
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

class LeagueRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class LeagueRankList : public CRankList, public ArenaStarRankInterface
{
public:
	LeagueRankList(int type);
	virtual ~LeagueRankList();

	virtual void Clear();
	virtual void RemoveAll();
	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 leagueTeamId);
	virtual UINT32 GetSize();
	const LeagueRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);
	virtual void NewRankInfoToClient(KKSG::RankData* rankdata, UINT64 uniqeID);

	void OnChange(UINT64 leagueTeamId, UINT32 point, float winRate, UINT32 time, UINT32 continueWin, UINT32 joinCount);

	void InsertKey(LeagueRankInfo* rankinfo);

	void GetRankInfo(std::vector<LeagueRankInfo*>& vec);
	void GetTopNRankInfo(UINT32 count, std::vector<LeagueRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	void DelLeagueTeam(UINT64 id);

	virtual void ASGetTopInfo(UINT32 count, std::vector<LeagueRankInfo*>& infoVec);
	virtual void ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec);
	void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<LeagueRankInfo*, LeagueCompare> m_rankTree;
};

class LastWeekLeagueRankList : public LeagueRankList
{
public:
	LastWeekLeagueRankList(int type);
	~LastWeekLeagueRankList();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();

	bool ASUpdateLastWeekRankInfoLeague(LeagueRankList* pCurRank, UINT64 leagueTeamID);//只记第一名
	virtual UINT32 ASGetTopViewRank();
	virtual void ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec);
};

#endif