#ifndef __MILITARY_RANKLIST_H__
#define __MILITARY_RANKLIST_H__

#include "ranklist.h"

class MilitaryRankInfo : public CRankInfo
{
public:
	MilitaryRankInfo(UINT64 roleid);
	virtual ~MilitaryRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

public:
	UINT32 military_exploit_;
	UINT32 military_rank_;
	UINT32 total_num_;
	UINT32 time_;
};

struct MilitaryCompare
{
	bool operator () (const MilitaryRankInfo* rankinfoa, const MilitaryRankInfo* rankinfob)
	{
		if (rankinfoa->military_exploit_ > rankinfob->military_exploit_)
		{
			return true;
		}
		if (rankinfoa->military_exploit_  < rankinfob->military_exploit_)
		{
			return false;
		}
		if (rankinfoa->military_exploit_ == rankinfob->military_exploit_)
		{
			if (rankinfoa->total_num_ < rankinfob->total_num_)
			{
				return true;
			}
			if (rankinfoa->total_num_ > rankinfob->total_num_)
			{
				return false;
			}
			if (rankinfoa->total_num_ == rankinfob->total_num_)
			{
				if (rankinfoa->time_ < rankinfob->time_)
				{
					return true;
				}
				if (rankinfoa->time_ > rankinfob->time_)
				{
					return false;
				}

				if (rankinfoa->time_ == rankinfob->time_)
				{
					return rankinfoa->GetUniqueId() < rankinfob->GetUniqueId();
				}
			}

			return false;
		}

		return false;
	}
};

///////////////////////////////////////////////////
class MilitaryDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class MilitaryRankList : public CRankList
{
public:
	MilitaryRankList(int type);
	virtual ~MilitaryRankList();

	virtual void Clear();

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	const MilitaryRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(UINT64 roleid, UINT32 military_exploit, UINT32 military_rank,UINT32 total_num,UINT32 time);	

	void InsertKey(MilitaryRankInfo* rankinfo);

	void GetRankInfo(std::vector<MilitaryRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

	void DeleteRole(UINT64 roleid);

	void AdjustMilitaryRank(const UINT64 role_id,
		const UINT32 military_exploit, 
		const UINT32 military_rank);

	// 通过排名获得特殊头衔
	UINT32 GetMilitaryRankByRank(UINT32 rank);

	// 获取特殊头衔的排名下限
	UINT32 GetMinRankByMilitary(UINT32 military_rank);

	// 更新军衔变化
	void UpdateMilitaryRank(UINT64 role_id,UINT32 military_rank);


	UINT32 GetMilitaryInfoByRoleId(UINT64 role_id);
private:
	bool m_ischanged;
	RankTree<MilitaryRankInfo*, MilitaryCompare> m_rankTree;
};

#endif