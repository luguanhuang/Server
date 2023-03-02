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


	//�����Ѵ��ڵĳ���
	//dwSceneInstanceID:	������ʵ��ID
	static bool EnterScene(CRole* poRole, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, UINT32 gsLine = 0, bool isCrossGs = false, UINT32 mapID = INVALID_MAP_ID);

	static bool EnterMainHall(CRole* poRole);
};

#endif