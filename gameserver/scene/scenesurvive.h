#ifndef __SCENESURVIVE_H__
#define __SCENESURVIVE_H__

#include "scenevsbase.h"
#include "vshandler.h"

class SceneSurvive : public SceneVsBase
{
public:
	SceneSurvive(Scene* scene);
	~SceneSurvive();

	virtual bool Init(const KKSG::CreateBattleParam& roParam);
	virtual void InitState();

	virtual UINT32 GetGroup(UINT64 roleid);
	virtual UINT64 GetFightGroupUId(UINT64 roleid);

	//为了不添加新的状态 用AfterFight代替Ready
	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();
	UINT32 GetReadyLeftTime();

	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();

	void OnStateEnd();

	virtual void OnRoleReconnect(Role* role);

	void InfoPrint();
	void InfoPrint(const KKSG::SurviveRoleFightData& data);

private:
	virtual void Update();
	virtual bool OnEvent(const VsEvent& event);

	void EndRole(UINT64 roleID, KKSG::SurviveFightEndType endType);
	const KKSG::SurviveRoleFightData* GetResult(UINT64 roleID);
	void ResultNotify(Role* pRole);
	void StageNotify(Role* pRole = NULL);
	void KillNotify(const std::string& killerName, const std::string& deadName);

	void CheckStageSynn();
	void FillFightStage(KKSG::SurviveFightSyn& data);
	KKSG::SurviveFightStage GetFightStage();

	UINT32 GetRoleCount();

	void GiveReward(Role* pRole);

	void PointSynToMs(Role* pRole, UINT32 point);

private:
	KKSG::KMatchFightData m_data;
	std::unordered_map<UINT64,KKSG::SurviveRoleFightData> m_result;

	time_t m_lastSynTime;
};

#endif