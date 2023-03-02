#ifndef  __COMPETEDRAGONRANKLIST__H_
#define  __COMPETEDRAGONRANKLIST__H_
#include "pch.h"
#include "ranklist.h"

typedef std::vector<UINT64> CDRKey;

class CompeteDragonRankInfo
{
public:
	CompeteDragonRankInfo(CDRKey& roles);
	~CompeteDragonRankInfo();

	void Reset();
	void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	inline UINT32 LeastCostT() const { return m_leastCostT; }
	inline UINT32 LastPassT() const { return m_lastPassT; }
	const CDRKey& GetKey() const { return m_roles; }
	bool HaveRole(UINT64 roleID);

	void SetData(UINT32 leastCostT, UINT32 lastPassT);

	void InfoPrint();

private:
	UINT32 m_leastCostT;//通关最短用时
	UINT32 m_lastPassT;//最后一次通过的时间	
	CDRKey m_roles;
};

struct CompeteDragonCompare
{
	bool operator () (const CompeteDragonRankInfo* info1, const CompeteDragonRankInfo* info2)
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

class CompeteDragonDB : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class CompeteDragonRankList : public CRankList
{
public:
	CompeteDragonRankList(int type);
	virtual ~CompeteDragonRankList();

	virtual void Clear();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	
	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	inline bool IsChanged() { return m_ischanged; }
	inline void SetChanged(bool flag) { m_ischanged = flag; }

	void GetAllInfo(std::vector<CompeteDragonRankInfo*>& vec);

	void OnChange(CDRKey& roles, UINT32 costtime, UINT32 passtime);
	void DeleteLast();

	void OnTwoWeekEvent();

	void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<CompeteDragonRankInfo*, CompeteDragonCompare, CDRKey> m_rankTree;
};

#endif
