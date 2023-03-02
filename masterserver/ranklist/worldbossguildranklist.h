#ifndef __WORLDBOSSGUILDRANKLIST_H__
#define __WORLDBOSSGUILDRANKLIST_H__

#include "ranklist.h"

#define WORLDBOSSGUILD_RANKCOUNT 1000
#define WORLDBOSSGUILD_CLIENTQUERYCOUNT 5

class BossGuildRankInfo : public CRankInfo
{
public:
	BossGuildRankInfo(UINT64 guildid);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	float m_damage;
};

class BossGuildCompare
{
public:
	bool operator()(const BossGuildRankInfo* rankinfoa, const BossGuildRankInfo* rankinfob) const
	{
		if (rankinfoa->m_damage != rankinfob->m_damage)
		{
			return rankinfoa->m_damage > rankinfob->m_damage;
		}
		return rankinfoa->GetUniqueId() < rankinfob->GetUniqueId();
	}
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class BossGuildRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
};

class WorldBossGuildRankList : public CRankList
{
public:
	WorldBossGuildRankList(int type);
	virtual ~WorldBossGuildRankList();

	virtual void Clear();

	virtual UINT32 GetNeedRankCount() { return WORLDBOSSGUILD_RANKCOUNT; }
	virtual UINT32 GetRank(UINT64 guildid) { return m_rankTree.GetRankByID(guildid); }
	virtual UINT32 GetSize() { return m_rankTree.GetSize(); }
	const BossGuildRankInfo* GetInfoByRank(UINT32 rank);

	virtual UINT32 GetClientQueryCount() { return WORLDBOSSGUILD_CLIENTQUERYCOUNT; }

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	UINT32 GetDamage(UINT64 guildid);
	void AddDamage(UINT64 guildid, UINT64 roleid, float damage);

	void RemoveRole(UINT64 guildid, UINT64 roleid);

	void InsertKey(BossGuildRankInfo* rankinfo);

	void GetRankInfo(std::vector<BossGuildRankInfo*>& vec) { m_rankTree.GetAllKey(vec); }
	void GetSomeRankInfo(UINT32 count, std::vector<BossGuildRankInfo*>& vec) { m_rankTree.GetSomeTopKey(count, vec); }

	void SaveToDb();

	const std::unordered_map<UINT64, float>* GetGuildRoleRank(UINT64 guildid);
public:
	RankTree<BossGuildRankInfo*, BossGuildCompare> m_rankTree;
	std::unordered_map<UINT64, std::unordered_map<UINT64, float>> m_guild2info;
};

#endif 