#ifndef _H_Scene_Switch_H__
#define _H_Scene_Switch_H__

#include "commondef.h"

namespace KKSG
{
	class SceneSwitchData;
}

class CRole;


class CSceneSwitch
{
public:
	typedef std::vector<CRole*> RoleList;


	//进入已存在的场景
	//dwSceneInstanceID:	场景的实例ID
	static bool EnterScene(CRole* poRole, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, UINT32 gsLine = 0, bool isCrossGs = false, UINT32 mapID = INVALID_MAP_ID);

	static bool EnterMainHall(CRole* poRole);
};

#endif