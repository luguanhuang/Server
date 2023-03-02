#ifndef __SCENEINVFIGHT_H__
#define __SCENEINVFIGHT_H__

#include "scenevsbase.h"
#include "pb/project.pb.h"

namespace KKSG
{
	class RoleSmallInfo;
}

class SceneInvFigtht : public SceneVsBase
{
public:
	SceneInvFigtht(Scene* scene);
	~SceneInvFigtht();

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();	
	//loading信息
	virtual void OnEnterBeforeNtf(Role* role);
	void FillRoleSmallInfo(UINT64 roleID, KKSG::RoleSmallInfo& data);

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual VsStateType IsAllLoad();
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	virtual VsStateType CheckFightEnd();

	virtual void OnStateEnd();

	UINT64 GetRoleIDByIndex(int index);
	UINT64 GetOpRoleID(UINT64 roleID);
	bool IsFightRole(UINT64 roleID);
	std::string GetRoleName(UINT64 roleID);

	UINT64 GetWinRoleID(bool timeOver);

	void FillResult(UINT64 roleID, KKSG::InvFightBattleResult& data);

	UINT64 GetFinalWinRoleID(){return m_winRoleID;}

	void RoleReqFightAgain(Role* pRole);

private:
	//场景事件监听
	virtual bool OnEvent(const VsEvent& event);
	//OnTick
	virtual void Update();

	virtual bool EndOnEvent(const VsEvent& event);

	enum IFAState
	{
		IFAS_NONE = 0,
		IFAS_READY = 1,
		IFAS_END_WAIT = 2,
	};
	void MakeFightAgain();
	void OpLeaveNtf(Role* pRole);

	std::map<UINT64, int> m_fightAgainRole;

	UINT64 m_winRoleID;

	KKSG::InvFightBattleInfo m_data;
};

#endif