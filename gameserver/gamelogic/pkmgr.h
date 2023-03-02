#ifndef __PKMGR_H__
#define __PKMGR_H__

#include "table/PkPointTable.h"
#include "table/PkRankTable.h"
#include "table/PkParamTable.h"

class Role;
struct ItemDesc;

class PkMgr
{
	PkMgr();
	~PkMgr();
	DECLARE_SINGLETON(PkMgr);
public:
	bool Init();
	void Uninit();

	bool CheckFile();
	bool LoadFile();
	void ClearFile();

	UINT32 GetPrepareTime(UINT32 point);
	void GetPointReward(UINT32 point, UINT32 index, std::vector<ItemDesc>& items);

	void HintNotify(Role* role);

	UINT32 GetParam(UINT32 point, bool iswin);
	int GetPoint(UINT32 myPoint, int pointSpan, KKSG::PkResultType ret);

	PkPointTable& GetPointTable() { return m_pointtable; }

private:

	UINT32 CalculateTime(UINT32 count);

	PkPointTable m_pointtable;
	PkParamTable m_paramtable;
};

#endif