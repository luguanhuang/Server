#ifndef __DRAGONHELPCONFIG_H__
#define __DRAGONHELPCONFIG_H__

#include "table/DargonReward.h"

class Role;

class DragonHelpConfig 
{
	DragonHelpConfig();
	~DragonHelpConfig();
	DECLARE_SINGLETON(DragonHelpConfig);

public:
	bool Init();
	void Uninit();

	bool LoadFile();

	bool ReadConf(DargonReward& table);

	void FillRewardState(Role* pRole, KKSG::DHRRes& roRes);
	KKSG::ErrorCode HandleFetchReward(Role* pRole, int id);
	
	void OnRoleLogin(Role* pRole);
	void CheckRoleHint(Role* pRole);

	void Print(Role* pRole);

private:
	KKSG::DHRState GetOneRewardState(Role* pRole, DargonReward::RowData* pConf);

	DargonReward::RowData* GetConf(int id);

	DargonReward m_table;	
};

#endif