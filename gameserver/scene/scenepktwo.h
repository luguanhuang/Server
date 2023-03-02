#ifndef __SCENEPKTWO_H__
#define __SCENEPKTWO_H__

#include "scenepkbase.h"

class ScenePkTwo : public ScenePkBase
{
public:
	ScenePkTwo(Scene* scene);
	~ScenePkTwo();

	virtual bool Init(const KKSG::CreateBattleParam& roData);
	void InitRobotInfo(const KKSG::KMatchFightData& tmpData);
	void InitDRoleData();

	virtual void OnEnterBeforeNtf(Role* role);

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual bool OnEvent(const VsEvent& event);
	void OnRoleLeave(Role* pRole);

	virtual VsStateType IsAllLoad();
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	virtual VsStateType CheckFightEnd();

	virtual void OnStateEnd();
	void MakeResult();
	void EndRole(Role* pRole);

	void FillRobots(std::list<UINT64>& robots);
	virtual void FillUnitIDs(std::vector<UINT64>& unitIDs);
	virtual KKSG::PkNVNType GetPkNVNType() { return KKSG::PK_2v2; }
	virtual BPkRoleData* GetRoleBaseData(UINT64 roleID);

	UINT32 GetVsCount();
	UINT64 GetWin();
	const std::unordered_map<UINT64, DPkRoleData>& GetRoleData(){return m_roles;} 
	const KKSG::KMatchFightData GetKMatchRole(){return m_data;}

private:
	KKSG::KMatchRole* GetKKSGRoleData(UINT64 roleID);
	DPkRoleData* GetDRoleData(UINT64 roleID);
	UINT32 GetRolePoint(UINT64 roleID);
	std::string GetRoleName(UINT64 roleID);
	UINT32 GetAVGPoint(UINT32 group);
	void FillOneRec(UINT32 myGroup, KKSG::PkOneRec& rec);

	void InfoPrint();

private:
	KKSG::KMatchFightData m_data;
	std::unordered_map<UINT64, DPkRoleData> m_roles;
	std::string m_strTransTag;
};

#endif