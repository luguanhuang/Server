#ifndef __SURVIVERANKLIST_H__
#define __SURVIVERANKLIST_H__

#include "ranklist.h"

class SurviveRankInfo : public CRankInfo
{
public:
	SurviveRankInfo(UINT64 roleID);
	virtual ~SurviveRankInfo();

	virtual void Reset() {}
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	void SetData(const KKSG::SurviveRankData& data) { m_data = data; }

	UINT64 RoleID() const { return m_data.roleid(); }
	UINT32 ServerID() const { return m_data.serverid(); }
	UINT32 Point() const { return m_data.point(); }

	void InfoPrint();

private:
	KKSG::SurviveRankData m_data;

};

struct SurviveCompare
{
	bool operator () (const SurviveRankInfo* info1, const SurviveRankInfo* info2)
	{
		if(info1->Point() > info2->Point())
		{
			return true;
		}
		if(info1->Point() < info2->Point())
		{
			return false;
		}
		if(info1->GetUniqueId() < info2->GetUniqueId())
		{
			return true;
		}
		if(info1->GetUniqueId() > info2->GetUniqueId())
		{
			return false;
		}
		return false;
	}
};

class SurviveRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class SurviveRankList : public CRankList
{
public:
	SurviveRankList(int type);
	virtual ~SurviveRankList();
	virtual void Clear();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();

	inline bool IsChanged() { return m_ischanged; }
	inline void SetChanged(bool flag) { m_ischanged = flag; }

	void GetAllInfo(std::vector<SurviveRankInfo*>& vec);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(const KKSG::SurviveRankData& data);	
	void DeleteLast();

	void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<SurviveRankInfo*, SurviveCompare> m_rankTree;

};

#endif