#ifndef __SCENECUSTOMPKTWO_H__
#define __SCENECUSTOMPKTWO_H__

#include "scenepkbase.h"
#include "scenecustomlogger.h"

class SceneCustomPkTwo : public SceneVsBase
{
public:
	SceneCustomPkTwo(Scene* scene);
	~SceneCustomPkTwo();

	// init
	virtual bool Init(const KKSG::CreateBattleParam& roData);
	virtual void InitState();
	void InitDRoleData();

	virtual UINT32 GetGroup(UINT64 roleid);
	virtual UINT32 GetLoadWaitTime();

	virtual VsStateType IsAllLoad();
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	// update
	virtual void Update();

	virtual bool OnEvent(const VsEvent& event);
	void OnRoleLeave(UINT64 roleid);

	virtual VsStateType CheckFightEnd();

	// end
	virtual void OnStateEnd();
	void ResultCustom();

	bool IsFairMode() { return m_data.param() > 0 ? true : false; }
	CustomBattleLogger* GetLogger() { return &m_logger; }

private:
	UINT32 GetVsCount() { return 2; }
	std::string GetRoleName(UINT64 roleID);
	DPkRoleData* GetDRoleData(UINT64 roleID);
	KKSG::KMatchRole* GetKKSGRoleData(UINT64 roleID);

	void InfoPrint();

private:
	CustomBattleLogger m_logger;

	KKSG::KMatchFightData m_data;
	std::unordered_map<UINT64, DPkRoleData> m_roles;
	std::string m_strTransTag;
};

#endif