#ifndef __ARENARANKLIST_H__
#define __ARENARANKLIST_H__

#include "ranklist.h"
#include "table/globalconfig.h"

#define ARENA_RANKCOUNT (UINT32)( GetGlobalConfig().ArenaNpcCount)

class CArenaRankInfo : public CRankInfo
{
public:
	CArenaRankInfo(UINT64 roleid);
	virtual ~CArenaRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);
};

/////////////////////////////////////////////////////////////////////
///> 竞技场实时排行榜
class CArenaRealTimeDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class CArenaRealTimeRankList : public CRankList
{
public:
	CArenaRealTimeRankList(int type);
	virtual ~CArenaRealTimeRankList();

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);
	virtual UINT32 GetNeedRankCount() { return ARENA_RANKCOUNT; }
	virtual UINT32 GetSize() { return m_rankinfos.size(); }

	UINT32 GetNextEmptyPos(UINT64 roleid);

	bool SwapPos(UINT64 roleid1, UINT32 rank1, UINT64 roleid2, UINT32 rank2);

	void DeleteRole(UINT64 roleid);

	inline void AddRankInfo(UINT64 roleid, UINT32 rank)
	{
		return;
		CArenaRankInfo* rankinfo = new CArenaRankInfo(roleid);
		m_rankinfos.push_back(rankinfo);
		SetRank(roleid, rank);
	}
	inline UINT64 GetRoleId(UINT32 rank)
	{
		if (0 == rank || m_rankinfos.size() < rank)
		{
			return 0;
		}
		return NULL == m_rankinfos[rank - 1] ? 0 : m_rankinfos[rank - 1]->GetUniqueId();
	}
	inline UINT32 GetRank(UINT64 roleid)
	{
		return INVALID_RANK;

		auto i = m_roleid2rank.find(roleid);
		if (i == m_roleid2rank.end())
		{
			return INVALID_RANK;
		}
		return i->second;
	}

	inline void SetRank(UINT64 roleid, UINT32 rank)
	{
		return;
		m_roleid2rank[roleid] = rank;
	}
	inline void SetChanged(UINT64 roleid, UINT32 rank)
	{
		return;
		m_roleid2changed[roleid] = rank;
	}

public:
	std::vector<CRankInfo*> m_rankinfos;
	std::unordered_map<UINT64, UINT32> m_roleid2rank;
	std::unordered_map<UINT64, UINT32> m_roleid2changed;
	std::set<UINT64> m_outsideRobot; //for punish someone and use outside robot to replace
};

/////////////////////////////////////////////////////////////////////
//> 竞技场每日排行榜
class CArenaRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class CArenaRankList : public CRankList
{
public:
	CArenaRankList(int type);
	virtual ~CArenaRankList();

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);
	virtual UINT32 GetNeedRankCount() { return ARENA_RANKCOUNT; }
	virtual UINT32 GetSize() { return m_rankinfos.size(); }
	inline void AddRankInfo(UINT64 roleid, UINT32 rank)
	{
		CArenaRankInfo* rankinfo = new CArenaRankInfo(roleid);
		m_rankinfos.push_back(rankinfo);
		SetRank(roleid, rank);
	}
	inline void ReplaceRankInfo(UINT64 roleid, UINT32 rank)
	{
		if (m_rankinfos.size() >= rank)
		{
			CRankInfo* rankinfo = m_rankinfos[rank - 1];
			rankinfo->SetUniqueId(roleid);
			SetRank(roleid, rank);	
		}
	}	
	inline UINT32 GetRank(UINT64 roleid)
	{
		auto i = m_roleid2rank.find(roleid);
		if (i == m_roleid2rank.end())
		{
			return INVALID_RANK;
		}
		return i->second;

	}
	inline void SetRank(UINT64 roleid, UINT32 rank)
	{
		m_roleid2rank[roleid] = rank;
	}

public:
	std::vector<CRankInfo*> m_rankinfos;
	std::unordered_map<UINT64, UINT32> m_roleid2rank;
};

#endif