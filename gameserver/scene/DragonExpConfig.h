#ifndef __DRAGONEXPCONFIG_H__
#define __DRAGONEXPCONFIG_H__

#include "table/DragonExpList.h"

namespace KKSG
{
	class DEProgressRes;
}

class Role;

class DragonExpConfig
{
	DragonExpConfig();
	~DragonExpConfig();
	DECLARE_SINGLETON(DragonExpConfig);

public:
	bool Init();
	void Uninit();

	void FillProgress(Role* pRole, KKSG::DEProgressRes &roRes);
	UINT32 GetFightSceneID(Role* pRole);

	void SynProgressToMS(Role* pRole, bool isbattleend = false);
	void OnSystemOpened(Role* pRole);
	bool IsSystemOpened(Role* pRole);

	void GetJoinReward(UINT32 sceneID, std::vector<Sequence<UINT32,2>>& rewards);
	void GetWinReward(UINT32 sceneID, std::vector<Sequence<UINT32,2>>& rewards);

	void ResetProgress(Role* pRole, UINT32 sceneid);

	void GetSealBuff(UINT32& buffid, UINT32& bufflevel, UINT32 sceneid);
	bool IsDESceneGroup(UINT32 groupID);
	UINT32 GetSceneGroupID();

	void OnRoleLogin(Role* pRole);

private:

	UINT32 GetNextSceneID(UINT32 curSceneID);
	UINT32 GetLastSceneID();

	DragonExpList m_deTable;
	UINT32 m_groupsceneid;
};

#endif