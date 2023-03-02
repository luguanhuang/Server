#ifndef __ROBOTCONFIG_H__
#define __ROBOTCONFIG_H__

#include "table/RobotTemplateTable.h"
#include "table/RobotLookupTable.h"
#include "table/RandomName.h"

class RoleSummary;
typedef RobotTemplateTable::RowData RobotConf;
typedef RobotLookupTable::RowData RobotLookupConf;

struct FilterRoleData
{
	UINT64 qwRoleID;
	int level;
	int ppt;
	int prof;
};

class RobotConfig
{
	RobotConfig();
	~RobotConfig();
	DECLARE_SINGLETON(RobotConfig)

public:

	bool Init();
	void Uninit();
	bool Reload();

	RobotLookupConf *FindLookupConf(int lookupID);
	RobotConf *FindRobotConf(int robotID);
	RoleSummary *CreateRobotSummary(const FilterRoleData &filter, int lookupID);
	RoleSummary *CreateRobotSummary(const FilterRoleData &filter, RobotLookupConf *pConf);
	RobotTemplateTable &GetTemplateTable() { return m_oTemplateTable; }
	std::string CreateRandomName();

	void TogglePrint();

private:

	RobotTemplateTable m_oTemplateTable;
	RobotLookupTable m_oLookupTable;
	RandomName m_oRandNameTable;
};


#endif // __ROBOTCONFIG_H__