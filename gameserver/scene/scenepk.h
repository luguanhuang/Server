#ifndef __SCENEPK_H__
#define __SCENEPK_H__

#include "scenepkbase.h"

namespace KKSG
{
	class SceneSwitchData;
	class QMHRPkRes;
}
class Unit;
class Role;
class Enemy;
class Scene;
class DummyRole;
class RoleSummary;
class PtcG2C_PkRoleInfoNtf;
class ScenePk : public ScenePkBase
{
public:
	ScenePk(Scene* scene);
	~ScenePk();

	virtual bool Init(const KKSG::CreateBattleParam& roData);
	void InitRobotInfo();
	bool InitBaseData();
	virtual void OnEnterBeforeNtf(Role* role);

	virtual UINT32 GetGroup(UINT64 roleid);

	virtual bool OnEvent(const VsEvent& event);

	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded);

	virtual VsStateType CheckP321End();

	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();
	bool CheckEnd();

	virtual void OnStateEnd();

	void UpdateBasePoint();
	void Result();
	bool IsWin(UINT64 roleid);
	const std::set<UINT64>& GetWins() { return m_wins; }

	void EndScene();
	virtual void FillResult(KKSG::PkResult* result, Role* role);
	SPkResult* GetResult(UINT64 roleid);
	virtual BPkRoleData* GetRoleBaseData(UINT64 roleID);

	void EnableRobot(bool istrue);

	inline void SetEnterTimeout() { m_isentertimeout = true; }
	inline bool IsEnterTimeout() { return m_isentertimeout; }

	std::list<UINT64>& GetRobots() { return m_robots; }
	virtual void FillUnitIDs(std::vector<UINT64>& unitIDs);
	virtual KKSG::PkNVNType GetPkNVNType() { return KKSG::PK_1v1; }

	//for live
	UINT32 GetRolePoint(UINT64 roleID);
	void FillRoleBrief(UINT64 roleID, KKSG::RoleBriefInfo& brief);

private:
	bool OnRoleEnter(UINT64 roleid);
	bool OnRoleLeave(UINT64 roleid);
	bool OnRoleDie(UINT64 roleid);

	bool EndRole(Role* role);

	SPkResult* AddUnit(const KKSG::PkRoleInfo& data);
	void FillOther(SPkResult* infoa, SPkResult* infob);
	bool _IsGuildLadderTime();

	std::string GetRoleName(UINT64 roleID);

	void TestPrint();

private:
	bool m_isentertimeout;
	std::list<UINT64> m_robots;
	std::set<UINT64> m_wins;
	std::map<UINT64, SPkResult> m_units;
	std::set<UINT64> m_guildLadderId;

	KKSG::PkBattleInfo m_pkdata;
};

#endif