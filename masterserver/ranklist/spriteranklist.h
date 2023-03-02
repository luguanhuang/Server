#ifndef __SPRITERANKLIST_H__
#define __SPRITERANKLIST_H__

#include "ranklist.h"

class SpriteRankInfo : public CRankInfo
{
public:
	SpriteRankInfo(UINT64 roleid);
	virtual ~SpriteRankInfo();

	virtual void Reset();
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);
public:
	UINT32 m_powerpoint;
	UINT32 m_time;
};

class SpriteRankCompare
{
public:
	bool operator()(const SpriteRankInfo* rankinfoa, const SpriteRankInfo* rankinfob)
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

class SpriteRankDb : public RankDbHandler
{
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);
	virtual bool BeforeSaveToDB();
};

class SpriteRankList : public CRankList
{
public:
	SpriteRankList(int type);
	virtual ~SpriteRankList();

	virtual void Clear();

	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();
	const SpriteRankInfo* GetInfoByRank(UINT32 rank);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 profession);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(UINT64 roleid, UINT32 powerpoint, UINT32 time);

	void InsertKey(SpriteRankInfo* rankinfo);

	void GetRankInfo(std::vector<SpriteRankInfo*>& vec);

	bool IsChanged() { return m_ischanged; }
	void SetIsChanged(bool ischanged) { m_ischanged = ischanged; }

private:
	bool m_ischanged;
	RankTree<SpriteRankInfo*, SpriteRankCompare> m_rankTree;

};

#endif