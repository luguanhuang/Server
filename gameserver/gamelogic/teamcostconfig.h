#ifndef __TEAMCOSTCONFIG_H__
#define __TEAMCOSTCONFIG_H__

#include "table/TeamCostDaily.h"

class Role;

class TeamCostConfig
{
public:
	TeamCostConfig();
	~TeamCostConfig();
	DECLARE_SINGLETON(TeamCostConfig);

	bool Init();
	void Uninit();

	UINT32 GiveReward(Role* pRole, UINT32 activity, UINT32 lastvalue);

private:
	TeamCostDaily m_table;
	std::map<UINT32, std::vector<TeamCostDaily::RowData*>> m_activity2reward;
};

#endif