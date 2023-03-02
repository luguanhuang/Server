#ifndef __TSHOW_H__
#define __TSHOW_H__

//#include "item.h"

#define TSHOWRECORDLIMIT 20
#define TSHOWROLELIMIT 100

#include "table/FashionRankTable.h"

struct TShowRoleCountRecord
{
	UINT64 roleid;
	UINT32 count;
};

struct TShow
{
	TShow()
	{
		m_ischanged = false;
		m_updatetime = 0;
		m_week = 0;
	}

	void Update();
	void Sort(bool isall = false);

	void RecordRoleCount(UINT64 besendid, UINT64 roleid, UINT32 count);
	UINT32 GetVotedNum();
	void DebugReset();

	static bool IsLess(const TShowRoleCountRecord& a, const TShowRoleCountRecord& b);


	bool m_ischanged;
	UINT32 m_updatetime;
	UINT32 m_week;
	std::vector<TShowRoleCountRecord> m_rolecount;

};
class TShowConfig
{
public:
	TShowConfig();
	~TShowConfig();

	DECLARE_SINGLETON(TShowConfig);

	bool Init();
	void Uninit();
	//bool GetFashionRankReward(UINT32 rank, std::vector<ItemDesc>& vec);

	FashionRankTable m_oFashionRankTable;
};

#endif