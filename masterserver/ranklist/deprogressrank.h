#ifndef __DEPROGRESSRANK_H__
#define __DEPROGRESSRANK_H__

#include "ranklist.h"

namespace KKSG
{
	class DEProgress;
}

class DEPRankInfo : public CRankInfo
{
public:
	DEPRankInfo(UINT64 roleid);
	virtual ~DEPRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	void SetRankData(UINT32 sceneid, int hppercent);
	int GetGroupID(){ return m_groupid; }
	void SetGroupID(int groupid) { m_groupid = groupid; }
	inline UINT32 GetSceneID() const { return m_sceneid; }
	inline int GetBossAVGHP() const { return m_bossavghppercent; }

private:
	UINT32 m_sceneid;
	int m_bossavghppercent;
	int m_groupid;
};

struct DEPCompare
{
	bool operator () (const DEPRankInfo* infoa, const DEPRankInfo* infob)
	{
		if(infoa->GetSceneID() > infob->GetSceneID())
		{
			return true;
		}
		else if(infoa->GetSceneID() == infob->GetSceneID())
		{
			if(infoa->GetBossAVGHP() < infob->GetBossAVGHP())
			{
				return true;
			}
			else if(infoa->GetBossAVGHP() == infob->GetBossAVGHP())
			{
				if(infoa->GetUniqueId() < infob->GetUniqueId())
				{
					return true;
				}
			}
		}
		return false;
	}
};

class DEPDbhandler : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
	virtual void OnReceiveFromDB();
};

class DEPRankList : public CRankList
{
public:
	DEPRankList(int type);
	virtual ~DEPRankList();

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();

	//影响排行
	void OnChangeRank(UINT64 roleid, UINT32 sceneid, int avghpprecent);
	//不影响排行
	void OnChangeGroup(UINT64 roleid, int groupid);

	bool IsFull();
	bool HaveRole(UINT64 roleid);
	int GetGroup(UINT64 roleid);
	void InsertKey(DEPRankInfo* info);
	void GetAllRankInfo(std::vector<DEPRankInfo*>& vec);
	void Clear();

	void FillProgressInfo(UINT64 roleid, KKSG::DEProgress& data);

	void SetIsChanged(bool c){ m_ischanged = c; }
	bool IsChanged(){ return m_ischanged; }

private:

	DEPRankInfo* GetRankInfo(UINT64 roleid);

	RankTree<DEPRankInfo*, DEPCompare> m_ranktree;
	bool m_ischanged;
};

#endif