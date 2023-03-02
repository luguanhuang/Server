#ifndef __WORLDBOSSRANKLIST_H__
#define __WORLDBOSSRANKLIST_H__

#include "ranklist.h"

#define WORLDBOSS_RANKCOUNT 5000
#define WORLDBOSS_CLIENTQUERYCOUNT 6 

class BossDamageRankInfo : public CRankInfo
{
public:
	BossDamageRankInfo(UINT64 roleid);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* pRankData) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* pRankData) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* pRankData);

	float m_damage;
};

class BossDamageCompare
{
public:
	bool operator()(const BossDamageRankInfo* rankinfoa, const BossDamageRankInfo* rankinfob) const
	{
		if(rankinfoa->m_damage != rankinfob->m_damage)
		{
			return rankinfoa->m_damage > rankinfob->m_damage;
		}
		return rankinfoa->GetUniqueId() < rankinfob->GetUniqueId();	
	}
};

/////////////////////////////////////////////////////////////////////////////////////
class BossDamageRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
};

class BossDamageRankList : public CRankList
{
public:
	BossDamageRankList(int type);
	virtual ~BossDamageRankList();

	virtual void Clear();

	virtual UINT32 GetNeedRankCount() { return WORLDBOSS_RANKCOUNT; }
	virtual UINT32 GetRank(UINT64 roleid) { return m_rankTree.GetRankByID(roleid); }
	virtual UINT32 GetSize() { return m_rankTree.GetSize(); }
	const BossDamageRankInfo* GetInfoByRank(UINT32 rank);

	virtual UINT32 GetClientQueryCount() { return WORLDBOSS_CLIENTQUERYCOUNT; }

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	UINT32 GetDamage(UINT64 roleid);
	void AddDamage(UINT64 roleid,  float damage);	

	void InsertKey(BossDamageRankInfo* rankinfo);

	void GetRankInfo(std::vector<BossDamageRankInfo*>& vec) { m_rankTree.GetAllKey(vec); }
	void GetSomeRankInfo(UINT32 count, std::vector<BossDamageRankInfo*>& vec) { m_rankTree.GetSomeTopKey(count, vec); }

	void SaveToDb();

	void DeleteRole(UINT64 roleid);

public:
	RankTree<BossDamageRankInfo*, BossDamageCompare> m_rankTree;
};

#endif // __WORLDBOSSRANKLIST_H__
