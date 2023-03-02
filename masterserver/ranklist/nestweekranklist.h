#ifndef  __NESTWEEKRANKLIST__H_
#define  __NESTWEEKRANKLIST__H_

#include "ranklist.h"
#include "arenastar/arenastarbase.h"

typedef std::vector<UINT64> NWRKey;

class NestWeekRankInfo
{
public:
	NestWeekRankInfo(NWRKey& roles);
	~NestWeekRankInfo();

	void Reset();
	void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	inline UINT32 LeastCostT() const { return m_leastCostT; }
	inline UINT32 LastPassT() const { return m_lastPassT; }
	const NWRKey& GetKey() const { return m_roles; }
	bool HaveRole(UINT64 roleID);

	void SetData(UINT32 leastCostT, UINT32 lastPassT);

	void InfoPrint();

private:
	UINT32 m_leastCostT;//通关最短用时
	UINT32 m_lastPassT;//最后一次通过的时间	
	NWRKey m_roles;
};

struct NestWeekCompare
{
	bool operator () (const NestWeekRankInfo* info1, const NestWeekRankInfo* info2)
	{
		if(info1->LeastCostT() < info2->LeastCostT())
		{
			return true;
		}
		else if(info1->LeastCostT() == info2->LeastCostT())
		{
			if(info1->LastPassT() < info2->LastPassT())
			{
				return true;
			}
			else if(info1->LastPassT() == info2->LastPassT())
			{
				if(info1->GetKey() < info2->GetKey())
				{
					return true;
				}
			}
		}
		return false;
	}
};

class NestWeekDB : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class NestWeekRankList : public CRankList, public ArenaStarRankInterface
{
public:
	NestWeekRankList(int type);
	virtual ~NestWeekRankList();

	virtual void Clear();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	
	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	inline bool IsChanged() { return m_ischanged; }
	inline void SetChanged(bool flag) { m_ischanged = flag; }

	void GetAllInfo(std::vector<NestWeekRankInfo*>& vec);

	void OnChange(NWRKey& roles, UINT32 costtime, UINT32 passtime);
	void DeleteLast();

	virtual void ASGetTopInfo(UINT32 count, std::vector<NestWeekRankInfo*>& infoVec);
	virtual void ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec);
	void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<NestWeekRankInfo*, NestWeekCompare, NWRKey> m_rankTree;
};

class LastWeekNestWeekRankList : public NestWeekRankList
{
public:
	LastWeekNestWeekRankList(int type);
	virtual ~LastWeekNestWeekRankList();

	virtual int GetUpdateInterval(); 

	virtual UINT32 GetNeedRankCount();
	void AddRankInfo(NestWeekRankInfo* info);

	virtual void ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank);
	virtual UINT32 ASGetTopViewRank();

};

#endif
