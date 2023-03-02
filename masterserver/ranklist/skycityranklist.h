#ifndef __SKYCITYRANKLIST_H__
#define __SKYCITYRANKLIST_H__

#include "ranklist.h"

class SkyCityRankInfo : public CRankInfo
{
public:
	SkyCityRankInfo(UINT64 roleID);
	virtual ~SkyCityRankInfo();

	virtual void Reset() {}
	virtual void RankInfoToKKSG(KKSG::RankData* rankdata) const;
	virtual void RankInfoToDBKKSG(KKSG::RankData* rankdata) const;
	virtual void DBKKSGToRankInfo(const KKSG::RankData* rankdata);

	void SetData(const KKSG::SkyCityRankData& data) { m_data = data; }
	UINT64 RoleID() const { return m_data.roleid(); }
	UINT32 Floor() const { return m_data.floor(); }
	UINT32 PowPPT() const {return m_data.fight(); }

	void InfoPrint();

private:
	KKSG::SkyCityRankData m_data;

};

struct SkyCityCompare
{
	bool operator () (const SkyCityRankInfo* info1, const SkyCityRankInfo* info2)
	{
		if(info1->Floor() != info2->Floor())
		{
			return info1->Floor()>info2->Floor();
		}
		if(info1->PowPPT() != info2->PowPPT())
		{
			return info1->PowPPT()>info2->PowPPT();
		}
		if(info1->GetUniqueId() != info2->GetUniqueId())
		{
			return info1->GetUniqueId()>info2->GetUniqueId();
		}
		return false;
	}
};

class SkyCityRankDb : public RankDbHandler
{
public:
	virtual void PackDBData(KKSG::RankList* ranklist);
	virtual void UnpackDBData(const KKSG::RankData& rankdata, UINT32 rank);

	virtual bool BeforeSaveToDB();
};

class SkyCityRankList : public CRankList
{
public:
	SkyCityRankList(int type);
	virtual ~SkyCityRankList();
	virtual void Clear();

	virtual int GetUpdateInterval(); 
	virtual UINT32 GetNeedRankCount();
	virtual UINT32 GetRank(UINT64 roleid);
	virtual UINT32 GetSize();

	inline bool IsChanged() { return m_ischanged; }
	inline void SetChanged(bool flag) { m_ischanged = flag; }

	void GetAllInfo(std::vector<SkyCityRankInfo*>& vec);

	virtual void RankListToClient(KKSG::RankList* rankdatas, UINT32 count, UINT32 professsion = 0);
	virtual void RankInfoToClient(KKSG::RankData* rankdata, UINT32 rank);

	void OnChange(const KKSG::SkyCityRankData& data);	
	void DeleteLast();

	void InfoPrint();

protected:
	bool m_ischanged;
	RankTree<SkyCityRankInfo*, SkyCityCompare> m_rankTree;

};

#endif