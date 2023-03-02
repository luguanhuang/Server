#ifndef __BRIEFRANKLIST_H__
#define __BRIEFRANKLIST_H__

#include "ranklist.h"


namespace KKSG
{
	class TShowTopListRes;
}
class CBriefRankInfo : public CRankInfo
{
public:
	CBriefRankInfo(UINT64 roleid);
	virtual ~CBriefRankInfo();
	
	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);
public:
	UINT32 m_level;
	UINT32 m_powerpoint;
	UINT32 m_time;
	UINT32 m_intervalTimeStamp;
};

class BriefRankCompare
{
public:
	virtual bool operator () (const CBriefRankInfo& RankInfoA, const CBriefRankInfo& RankInfoB) {return false;};
};

class PowerPointCompare : public BriefRankCompare
{
public:
	virtual bool operator () (const CBriefRankInfo& RankInfoA, const CBriefRankInfo& RankInfoB) 
	{
		if (RankInfoA.m_powerpoint > RankInfoB.m_powerpoint)
		{
			return true;
		}
		else if (RankInfoA.m_powerpoint == RankInfoB.m_powerpoint)
		{
			if (RankInfoA.m_level > RankInfoB.m_level)			
			{
				return true;
			}
			else if (RankInfoA.m_level == RankInfoB.m_level)
			{
				if (RankInfoA.m_time < RankInfoB.m_time)
				{
					return true;
				}
				else if (RankInfoA.m_time == RankInfoB.m_time)
				{
					return RankInfoA.GetUniqueId() < RankInfoB.GetUniqueId();
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
		else
		{
			return false;
		}
	};
};

class LevelCompare : public BriefRankCompare
{
public:
	virtual bool operator () (const CBriefRankInfo& RankInfoA, const CBriefRankInfo& RankInfoB) 
	{
		if (RankInfoA.m_level > RankInfoB.m_level)
		{
			return true;
		}
		else if (RankInfoA.m_level == RankInfoB.m_level)
		{
			if (RankInfoA.m_powerpoint > RankInfoB.m_powerpoint)			
			{
				return true;
			}
			else if (RankInfoA.m_powerpoint == RankInfoB.m_powerpoint)
			{
				if (RankInfoA.m_time < RankInfoB.m_time)
				{
					return true;
				}
				else if (RankInfoA.m_time == RankInfoB.m_time)
				{
					return RankInfoA.GetUniqueId() < RankInfoB.GetUniqueId();
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
		else
		{
			return false;
		}
	};
};

struct BriefRankCmpWrap
{
	BriefRankCompare *m_cmp;

	BriefRankCmpWrap(BriefRankCompare *cmp) : m_cmp(cmp){}

	bool operator () (const CBriefRankInfo* RankInfoA, const CBriefRankInfo* RankInfoB) 
	{
		return m_cmp->operator()(*RankInfoA, *RankInfoB);
	}
};

/////////////////////////////////////////////////////////////////////////////////////
/*class CBriefRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
	virtual void OnReceiveFromDB();
	virtual bool BeforeSaveToDB();

public:
	CBriefRankDb(){m_fillLastInfo = true;}
	bool m_fillLastInfo;
};*/

class CBriefRankList: public CRankList
{
public:
	CBriefRankList(int type, BriefRankCompare* cmp);
	virtual ~CBriefRankList();	

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid); 
	virtual UINT32 GetSize();
	const CBriefRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);	

	bool OnTimer();
	void OnGSConnected(UINT32 dwLine);

	void OnChange(UINT64 roleid, UINT32 powerpoint, UINT32 level, UINT32 time);	

	void InsertKey(CBriefRankInfo* rankinfo);

	void GetRankInfo(std::vector<CBriefRankInfo*>& vec);

	void GMSetUpdateNextWorldLevelFalse();

private:
	RankTree<CBriefRankInfo*, BriefRankCmpWrap> m_rankTree;
	bool m_GMUpdateNextWorldLevel;
};

#endif