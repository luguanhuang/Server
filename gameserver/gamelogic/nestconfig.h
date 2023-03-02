#ifndef __NESTCONFIG_H__
#define __NESTCONFIG_H__

#include "table/NestListTable.h"

class Role;

class NestConfig
{
public:
	NestConfig();
	~NestConfig();
	DECLARE_SINGLETON(NestConfig)

	bool Init();
	void Uninit();

	bool LoadFile();

	const NestListTable& GetNestListTable();
	const NestListTable::RowData* GetNestListTableData(UINT32 nestExpID);

	UINT32 CalcHardestNestExpID(Role* role);
	bool IsStarNestStart(UINT32 sceneID);

private:
	NestListTable m_oNestListTable;
	std::unordered_set<UINT32> m_starNestSceneIds;

};
#endif // 