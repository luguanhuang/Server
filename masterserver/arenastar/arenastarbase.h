#ifndef __ARENASTARBASE_H__
#define __ARENASTARBASE_H__

typedef std::pair<UINT32,UINT64> ASPair; // rank,roleid

class PkRankInfo;
class HeroRankInfo;
class NestWeekRankInfo;
struct LeagueRankInfo;

class ArenaStarRankInterface
{
public:
	ArenaStarRankInterface(){}
	virtual ~ArenaStarRankInterface(){}

	virtual void ASGetTopInfo(UINT32 count, std::vector<PkRankInfo*>& infoVec) {}
	virtual void ASGetTopInfo(UINT32 count, std::vector<HeroRankInfo*>& infoVec) {}
	virtual void ASGetTopInfo(UINT32 count, std::vector<NestWeekRankInfo*>& infoVec) {}
	virtual void ASGetTopInfo(UINT32 count, std::vector<LeagueRankInfo*>& infoVec) {}

	virtual void ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec) = 0;

	virtual void ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank) {}
	virtual UINT32 ASGetTopViewRank() { return 4; }

	virtual void InfoPrint() {}
};

struct ArenaStarRankP
{
	ArenaStarRankP()
	{
		pCurRank = NULL;
		pLastRank = NULL;
	}
	ArenaStarRankInterface* pCurRank;
	ArenaStarRankInterface* pLastRank;
};

#endif