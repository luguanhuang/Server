#ifndef __TAJIEHELPRECORD_H__
#define __TAJIEHELPRECORD_H__

#include "unit/roleserilizeinterface.h"

class Role;
class Scene;

#define FAIL_NOTICE_TIMES 3

struct TaJieHelpSceneData
{
	TaJieHelpSceneData()
	{
		sceneID = 0;
		continueFailTimes = 0;
		failNoticeTimes = 0;
	}
	UINT32 sceneID;
	UINT32 continueFailTimes;
	UINT32 failNoticeTimes;
};

class CTaJieHelpRecord : public RoleSerializeInterface
{
public:
	CTaJieHelpRecord(Role* role);
	virtual ~CTaJieHelpRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void TajieSceneProcess(Scene* scene, bool success);

	void OnDayPass();
	void SetChanged();
	void OnLogin();

private:
	void _AddTajieHelpFailTimes(Scene* scene);
	void _TajieHelpSceneSuccess(Scene* scene);
	void _HelpNotice(Scene* scene, bool success);

private:
	Role* m_pRole;
	Switch m_switch;

	int m_updateTime;

	std::map<UINT32, TaJieHelpSceneData> m_tajieHelpSceneData;
};

#endif
