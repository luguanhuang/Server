#ifndef __PETRANKLIST_H__
#define __PETRANKLIST_H__

#include "ranklist.h"

class PetRankInfo : public CRankInfo
{
public:
	PetRankInfo(UINT64 petuid);
	virtual ~PetRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);
public:	
	UINT64 m_roleid;
	UINT32 m_powerpoint;
	UINT32 m_time;
	UINT32 m_petid;
};

class PetRankCompare 
{
public:
	bool operator()(const PetRankInfo* rankinfoa, const PetRankInfo* rankinfob)
	{
		if(rankinfoa->m_powerpoint != rankinfob->m_powerpoint)
		{
			return rankinfoa->m_powerpoint > rankinfob->m_powerpoint;
		}
		if(rankinfoa->m_time != rankinfob->m_time)
		{
			return rankinfoa->m_time < rankinfob->m_time;
		}
		return rankinfoa->GetUniqueId() < rankinfob->GetUniqueId();
	}
};

class PetRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
	virtual bool BeforeSaveToDB();
};

class PetRankList : public CRankList
{
public:
	PetRankList(int type);
	virtual ~PetRankList();

	virtual void Clear();

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	const PetRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(UINT64 petuid, UINT32 petid, UINT64 roleid, UINT32 powerpoint, UINT32 time);

	void InsertKey(PetRankInfo* rankinfo);
	void EraseKey(PetRankInfo* rankinfo);

	void GetRankInfo(std::vector<PetRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

private:
	bool m_ischanged;
	RankTree<PetRankInfo*, PetRankCompare> m_rankTree;
	std::unordered_map<UINT64, std::set<PetRankInfo*, PetRankCompare>> m_roleToPets;

};

#endif