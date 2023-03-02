#ifndef __SCENECUSTOMPK_H__
#define __SCENECUSTOMPK_H__

#include "scenevsbase.h"
#include "pb/enum.pb.h"
#include "pb/project.pb.h"
#include "scenecustomlogger.h"

struct SCustomPkResult
{
	VsRoleState state;
	KKSG::PkResultType result;

	SCustomPkResult()
	{
		state = VS_ROLE_WAITLOAD;
		result = KKSG::PkResult_Lose;
	}
};

class SceneCustomPk : public SceneVsBase
{
public:
	SceneCustomPk(Scene* scene);
	~SceneCustomPk();

	virtual bool Init(const KKSG::CreateBattleParam& roData);
	virtual void InitState();
	virtual UINT32 GetGroup(UINT64 roleid);

	virtual void Update();
	virtual bool OnEvent(const VsEvent& event);

	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();
	virtual UINT32 GetLoadWaitTime();

	virtual VsStateType CheckP321End();
	virtual VsStateType CheckFightEnd();
	virtual void OnStateEnd();

	// end
	bool CheckEnd();

	///> ≈–∂œ ‰”Æ
	void Result();
	void ResultSystem(Role* role11, Role* role22);
	void ResultCustom(UINT64 role11, UINT64 role22);

	// win
	UINT64 GetRoleWin() { return m_rolewin; }
	bool IsWin(UINT64 roleid) { return m_rolewin == roleid; }
	void SetResult(UINT64 roleid, KKSG::PkResultType type);

	void FillResult(KKSG::PkResult* result, Role* role);
	SCustomPkResult* GetResult(UINT64 roleid);

	const KKSG::CustomBattleMatchBattleInfo& GetCustomBattle() { return m_battle; }

	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	bool IsOneQuit() { return m_isonequit; }
	bool IsFairMode() { return m_battle.isfair(); }

	void DoTxLog(Role* role11, Role* role22);

	const std::map<UINT64, SCustomPkResult>& GetRoleInfo(){return m_units;}

	CustomBattleLogger* GetLogger() { return &m_logger; }

private:
	bool OnRoleEnter(UINT64 roleid);
	bool OnRoleLeave(UINT64 roleid);
	bool OnRoleDie(UINT64 roleid);

	std::string GetRoleName(UINT64 roleID);

private:
	bool m_isonequit;
	UINT64 m_rolewin;
	CustomBattleLogger m_logger;

	std::map<UINT64, SCustomPkResult> m_units;

	KKSG::CustomBattleMatchRoleInfo m_infoa;
	KKSG::CustomBattleMatchRoleInfo m_infob;

	KKSG::CustomBattleMatchBattleInfo m_battle;

	Scene* m_scene;
};

#endif