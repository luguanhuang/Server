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


	//1. ���̳ɹ����꣬ ��ĳ��GS��(��������)������һ��Scene������roRoleList�е���ҷŽ�����
	//2. �����м�ʧ�ܣ��ߵ�roRoleList�е����


	//�л�����
	//�������ݴ���ʱ����false
	//1. roRoleList:		�л���ɫ�б�
	//2. dwSceneTemplateID: ��������ID
	//3. roData:			�л����³���Ҫ���������
	static bool ChangeScene(RoleList& roRoleList, UINT32 dwSceneTemplateID, const KKSG::SceneSwitchData& roData);


	//�����Ѵ��ڵĳ���
	//�������ݴ���ʱ����false
	//1. dwSceneTemplateID:	��������ID (condition check)
	//2. dwSceneInstanceID:	������ʵ��ID
	static bool EnterScene(Role* poRole, UINT32 dwSceneTemplateID, UINT32 dwSceneInstanceID, const KKSG::SceneSwitchData& roData, bool isToCross, UINT32 gsLine);
};


#endif