#ifndef __PKRANKLIST_H__
#define __PKRANKLIST_H__

#include "ranklist.h"
#include "arenastar/arenastarbase.h"

class PkRankInfo : public CRankInfo
{
public:
	PkRankInfo(UINT64 roleid);
	virtual ~PkRankInfo();

	void SetData(UINT32 point, UINT32 ctime, const KKSG::PkRankExtraData& extraData);

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	void InfoPrint();
public:
	UINT32 m_point;
	UINT32 m_time;
	KKSG::PkRankExtraData m_extraData;
};

struct PkCompare
{
	bool operator () (const PkRankInfo* rankinfoa, const PkRankInfo* rankinfob)
	{
		if (rankinfoa->m_point > rankinfob->m_point)
		{
			return true;
		}
		else if (rankinfoa->m_point == rankinfob->m_point)
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

///////////////////////////////////////////////////
class PkRealTimeDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class PkRealTimeRankList : public CRankList, public ArenaStarRankInterface
{
public:
	PkRealTimeRankList(int type);
	virtual ~PkRealTimeRankList();

	virtual void Clear();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	const PkRankInfo* GetInfoByRank(UINT32 rank);
	UINT32 GetPkPoint(UINT64 roleId);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(UINT64 roleid, UINT32 point, UINT32 time, const KKSG::PkRankExtraData& extraData);	

	void InsertKey(PkRankInfo* rankinfo);

	void GetRankInfo(std::vector<PkRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	void DeleteRole(UINT64 roleid);

	virtual void ASGetTopInfo(UINT32 count, std::vector<PkRankInfo*>& infoVec);
	virtual void ASGetTopRank(UINT32 count, std::vector<ASPair>& rankVec);
	virtual void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<PkRankInfo*, PkCompare> m_rankTree;
};

class LastWeekPkRankList : public PkRealTimeRankList
{
public:
	LastWeekPkRankList(int type);
	virtual ~LastWeekPkRankList();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();

	virtual void ASUpdateLastWeekRankInfo(ArenaStarRankInterface* pCurRank);
};

#endif
