#ifndef __TSHOWRANKLIST_H__
#define __TSHOWRANKLIST_H__

#include "ranklist.h"

#define TSHOWRANKCOUNT 100
#define TSHOWLASTRANKCOUNT 5

class CRoleSummary;
namespace KKSG
{
	class TShowTopListRes;
}
class TShowRankInfo : public CRankInfo
{
public:
	TShowRankInfo(UINT64 roleid);
	~TShowRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);

public:
	UINT32 m_tshowvotecount;
	UINT32 m_time;
	UINT32 m_timestamp;
};

struct TShowCompare
{
	bool operator () (const TShowRankInfo* rankinfoa, const TShowRankInfo* rankinfob)
	{
		if (rankinfoa->m_tshowvotecount > rankinfob->m_tshowvotecount)
		{
			return true;
		}
		else if (rankinfoa->m_tshowvotecount == rankinfob->m_tshowvotecount)
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
class TShowRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
	virtual void OnReceiveFromDB();
	virtual bool BeforeSaveToDB();
};
class TShowRankList : public CRankList
{
public:
	TShowRankList(UINT32 type);
	virtual ~TShowRankList();
	
	virtual void Clear();

	bool OnTimer();

	virtual inline UINT32 GetClientQueryCount() { return TSHOWRANKCOUNT; }	
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(UINT64 roleid, UINT32 tshowvotecount, UINT32 time);	

	std::vector<TShowRankInfo>& GetRankInfo(){return m_lastrankinfos;}

	std::set<TShowRankInfo*, TShowCompare>& GetCurRankInfo(){return m_rankinfos;}

	UINT32 GetLastRankInfo(int count, KKSG::TShowTopListRes& res);	

	void SaveToDb();
	void Update();
	void GiveReward(UINT64 roleID);
	UINT32 GetLastRank(UINT64 roleID);
	void SendTShowVoteRank(CRoleSummary* role); 

private:
	UINT32 m_timehandle;
	std::set<TShowRankInfo*, TShowCompare> m_rankinfos;
	std::vector<TShowRankInfo> m_lastrankinfos;
};

#endif