#ifndef __ARENAMGR_H__
#define __ARENAMGR_H__

#include "table/ArenaTable.h"
#include "pb/project.pb.h"

class Role;
struct ItemDesc;
class CArenaMgr
{
	CArenaMgr();
	~CArenaMgr();
	DECLARE_SINGLETON(CArenaMgr);
public:
	bool Init();
	void Uninit();
	void Reset();

	bool LoadFile();
	bool Reload();

public:
	void PopBattleInfo(UINT64 roleid);
	void PushBattleInfo(UINT64 roleid, const KKSG::ArenaBattleInfo& info);
	void FillBattleInfo(Role* role, KKSG::UnitAppearance* pUnitAppearance);

	const KKSG::ArenaBattleInfo* GetBattleInfo(UINT64 roleid);
	
	// end
	void OnEnd(Role* pRole, bool iswin, bool isnpc, UINT64 challengeid);

	// reward
	int GetUpReward(UINT32 lastrank, UINT32 nowrank);

private:
	const ArenaTable::RowData* GetRowData(UINT32 rank);

private:
	ArenaTable m_oTable;
	std::map<UINT64, KKSG::ArenaBattleInfo> m_roleid2battle;
};

#endif
