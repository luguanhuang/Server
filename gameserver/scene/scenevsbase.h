#ifndef __SCENEVSBASE_H__
#define __SCENEVSBASE_H__

#include "vsstate.h"
#include "unit/poskeeper.h"
#include "scenecb.h"

class Scene;
class Unit;
class Role;
class DummyRole;
class RoleSummary;
class Enemy;
struct FilterRoleData;

namespace KKSG
{
	class SceneSwitchData;
	class CreateBattleParam;
};

class SceneVsBase
{
public:
	SceneVsBase(Scene* scene);
	virtual ~SceneVsBase();

	//************************************
	//������˭new˭deleteԭ�� ���Ƿ�����չ
	//��ȡVS���͵ĳ���������
	//************************************
	static SceneVsBase* CreateVsHandler(Scene* pScene);

	inline Scene* GetScene(){ return m_scene; }

	//���ֳ�ʼ����ʽ  ��Ҫ����̳� 1.һ��� ˳�㴴������ 2.�ȴ������� Ȼ��ֱ��
	virtual bool Init(const KKSG::SceneSwitchData& roData);
	virtual bool Init(const KKSG::CreateBattleParam& roParam);

	// ���볡���� ����EnterNtf֮ǰ�� ������������loading�����Ϣ
	void PreEnterScene(Role* role);
	virtual void OnEnterBeforeNtf(Role* role) {}
	void PreRoleLoginReconnect(Role* role);
	virtual void OnRoleReconnectBeforeNtf(Role* role) { OnEnterBeforeNtf(role); }

	// ���볡���� ��DoEnterScene���ؿͻ���֮ǰ(ע��:��ʱRole�����ҵ�Scene ����Scene�Ҳ���Role)
	virtual void PreDoEnterScene(Role* pRole) {}

	//������ʼ������λ�� �ͳ���(���������� ����л����� ������groupΪ0, fightgroup Ϊenemy)
	virtual UINT32 GetGroup(UINT64 roleid) = 0;
	virtual UINT64 GetFightGroupUId(UINT64 roleid);

	//��ʼ��״̬OnDied
	virtual void InitState() = 0;
	//�л�״̬
	void SetState(VsStateType type);

	//wait load state
	virtual void GetWaitInfo(std::vector<Role*>& loaded, std::vector<std::string>& unloaded) { };
	virtual void OnEnterWaitLoad();
	virtual VsStateType IsAllLoad();
	virtual VsStateType OnLoadTimeOut();
	virtual UINT32 GetLoadWaitTime();

	//321
	virtual void OnEnterP321();
	virtual VsStateType CheckP321End();

	///> ����׼�����
	void SceneReady();

	//fight 
	virtual void OnEnterFight();
	virtual VsStateType CheckFightEnd();

	//after fight
	virtual void OnEnterAfterFight();
	virtual VsStateType CheckAfterFightEnd();

	//state end
	virtual void OnStateEnd();

	//�����¼�����
	void VsOnEvent(const VsEvent& event);
	//OnTick
	bool VsUpdate(bool vsevent = false);

	//����û�и�״̬�Ŀ�ʼʱ�� 
	inline time_t GetStateStartTime(){ return m_stateStartTime; }
	inline void SetStateStartTime(time_t t){ m_stateStartTime = t; }

	//�Ƿ���� �����Ժ���Update ����Ҳ����� ���߽�������
	inline bool IsEnd(){ return m_IsEnd; }
	inline void SetEnd(){ m_IsEnd = true; }

	void Fight321Ntf(int flag);
	void ChangeWatcherState(Role* pRole, bool watcher);

	EnterPosition GetInitPos(UINT64 unitID);

	//��������
	virtual void OnRoleReconnect(Role* role) {}

	virtual void OnNonRoleEnterScene(Unit* pUnit) {}

	virtual void OnHurt(Unit* pTargetUnit, Unit* pCastUnit, double value) {}

	virtual void OnEnemyDead(Enemy* pEnemy) {}

	template <typename T>
	T *GetHandler()
	{
		return dynamic_cast<T*>(this);
	}

	virtual KKSG::ErrorCode CheckReportPlayer(UINT64 roleId, UINT64 badRoleId, int& num)
	{
		num = 0;
		return KKSG::ERR_SUCCESS;
	}

	virtual void GetSameTeamRole(UINT64 roleid, std::vector<Role*>& sameGroup) {}
	//����
	virtual void FillChatSource(KKSG::ChatSource* source) {}

	void MakePuppet(Unit* pUnit, bool puppet, bool notify = true);
	void AddBuff(UINT64 roleID, int buffid, int bufflevel);
	//���͵������� 
	void TranInitPosition(UINT64 unitID);
	void ForbidSkill(UINT64 unitID, bool forbid);

	void EndSkills(const std::vector<UINT64>& unitids);
	void SkillCoolDown(const std::vector<UINT64>& roleids);
    void MakeSkillCDAtLaunch(const std::vector<UINT64>& roleids);

	//notifyType: 0(����),1(��ɱ��)
	void VsKillNotify(UINT64 deadID, UINT32 reviveTime, int notifyType = 0);

	bool IsInWaitLoaded();
	int GetCurrentStateType();

	Unit* GetUnit(UINT64 unitID);
	Role* GetRole(UINT64 roleID);

	DummyRole* CreateRobot(const FilterRoleData& filter, UINT32 lookupid, UINT32 robotgroup, int dummytype);
	RoleSummary* GetUsedRoleSummary(UINT64 uid);

	StateMgr<SceneVsBase, VsEvent> m_statemgr;

protected:
	Scene* m_scene;

private:
	//�����¼�����
	virtual bool OnEvent(const VsEvent& event) = 0;
	//OnTick
	virtual void Update() = 0;
	//ֻ�ܷ������� 0:ÿ֡
	virtual UINT32 GetUpInterval() { return 1; }

	virtual bool EndOnEvent(const VsEvent& event) { return true; }

	void SetPuppet(UINT64 roleid, bool puppet);	
	bool IsPuppet(UINT64 roleid);

	void AddUsedRobotSummary(RoleSummary* summary);

	inline bool Locked() { return (m_lockNum > 0); }
	inline void AddLock() { ++m_lockNum; }
	inline void ReduceLock() { --m_lockNum; }

	void SetLastEvent(const VsEvent& event);
	void ClearLastEvent();
	inline UINT64 GetLastEventRoleID() { return m_lastEvent.roleid; }

	bool m_IsEnd;
	time_t m_stateStartTime;

	int m_lockNum;
	time_t m_lastVsUpdateTime;

	std::vector<RoleSummary*> m_usedRobotSummary;
	std::unordered_map<UINT64, bool> m_role_puppet;//��¼�Ƿ���ľż Ϊ����������ƥ��ԭ��

	VsEvent m_lastEvent;	
};

#endif
