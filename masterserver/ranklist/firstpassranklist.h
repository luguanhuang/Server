#ifndef __FIRSTPASSRANKLIST_H__
#define __FIRSTPASSRANKLIST_H__

#include "ranklist.h"

#define FIRSTPASSRANKCOUNT 1000
#define CLIENTFIRSTPASSRANKCOUNT 100

class FirstPassInfo
{
public:
	FirstPassInfo();
	~FirstPassInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);

	std::vector<UINT64>& GetRoleIDs(){return m_roleIDs;}

public:
	std::vector<UINT64> m_roleIDs;
	UINT32 m_time;
	UINT32 m_usecTime;
	UINT32 m_commendNum;
	UINT32 m_starLevel;
};

struct FirstPassCompare
{
	bool operator () (const FirstPassInfo* rankinfoa, const FirstPassInfo* rankinfob)
	{
		if (rankinfoa->m_starLevel != rankinfob->m_starLevel)
		{
			return rankinfoa->m_starLevel > rankinfob->m_starLevel;
		}
		if (rankinfoa->m_time != rankinfob->m_time)
		{
			return rankinfoa->m_time < rankinfob->m_time;
		}
		if (rankinfoa->m_usecTime != rankinfob->m_usecTime)
		{
			return rankinfoa->m_usecTime < rankinfob->m_usecTime;
		}
		if (rankinfoa->m_roleIDs.size() != rankinfob->m_roleIDs.size())
		{
			return rankinfoa->m_roleIDs.size() < rankinfob->m_roleIDs.size();
		}
		return rankinfoa->m_roleIDs < rankinfob->m_roleIDs;	
	}
};

class FirstPassRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
	virtual bool BeforeSaveToDB();
};

class FirstPassRankList : public CRankList
{
public:
	FirstPassRankList(UINT32 type, UINT32 serverid);
	virtual ~FirstPassRankList();

	virtual void Clear();
	virtual void RemoveAll();

	virtual inline UINT32 GetClientQueryCount() { return CLIENTFIRSTPASSRANKCOUNT; }
	virtual UINT32 GetNeedRankCount();	
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	UINT32 GetRank(std::vector<UINT64>& roleids, int starLevel);
	void EraseRank(std::vector<UINT64>& roleids);

	int OnChange(std::vector<UINT64>& roleids, UINT32 starLevel, UINT32 time, UINT32 usecTime);	

	std::set<FirstPassInfo*, FirstPassCompare>& GetCurRankInfo(){return m_rankinfos;}	
	
	void AddCommend(int num);
	int GetCommend();
	FirstPassInfo* GetRankInfo(int rank);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

private:	
	bool m_ischanged;
	std::set<FirstPassInfo*, FirstPassCompare> m_rankinfos;
};

#endif