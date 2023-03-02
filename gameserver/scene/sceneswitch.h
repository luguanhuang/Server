#ifndef _H_Scene_Switch_H__
#define _H_Scene_Switch_H__


#include "pb/project.pb.h"
class Role;


class ICreateBattleListener
{
public:
	virtual void OnCreateReply(KKSG::ErrorCode nErrCode, UINT32 dwSceneInstanceID, const KKSG::CreateBattleParam& roParam) = 0;
};

class CSceneSwitch
{
public:
	typedef std::vector<Role*> RoleList;


	//1. 流程成功走完， 在某个GS上(包括本地)创建了一个Scene，并将roRoleList中的玩家放进场景
	//2. 流程中间失败，踢掉roRoleList中的玩家


	//切换场景
	//参数传递错误时返回false
	//1. roRoleList:		切换角色列表
	//2. dwSceneTemplateID: 场景配置ID
	//3. roData:			切换到新场景要传输的数据
	static bool ChangeScene(RoleList& roRoleList, UINT32 dwSceneTemplateID, const KKSG::SceneSwitchData& roData);


	//进入已存在的场景
	//参数传递错误时返回false
	//1. dwSceneTemplateID:	场景配置ID (condition check)
	//2. dwSceneInstanceID:	场景的实例ID
	static bool EnterScene(Role* poRole, UINT32 dwSceneTemplateID, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, bool isToCross, UINT32 gsLine);
};


#endif