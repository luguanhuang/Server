#ifndef __ALLY_H__
#define __ALLY_H__

#include "unit.h"
#include "table/XEntityStatistics.h"
#include "pb/project.pb.h"
#include "math/XVector3.h"
#include "combatstatistics.h"

class XLevelSpawner;
class XDummyRole;
class RoleSummary;
class DummyRole;
std::ostream &operator<<(std::ostream &stream, DummyRole *pUnit);

enum DummyType
{
	SCENE_ENEMY = 0,
	ARENA_ENEMY,
	SCENE_ALLY,
};

class DummyRole : public Unit
{
public:
	DummyRole();
	~DummyRole();

	void Init(RoleSummary *pSummary, const Vector3& pos, const float face, UINT32 fightGroup, const EnterSceneInfo& info, DummyType type = SCENE_ALLY);
	void Uninit();

	void SetName(const std::string& pName){ m_Name = pName; }
	const std::string &GetName() { return m_Name;}
	UINT32 GetExp() { return m_exp; }
	UINT32 GetProfession() { return m_prof; }

	virtual void Update(float delta);

	virtual void GetAppearance(KKSG::UnitAppearance &Appearance, bool isbrief = true);
	virtual void StreamOutput(std::ostream &stream) { stream << this; }
	virtual void CleanUpInScene();
	virtual std::string GetAIString();
	virtual void GetAIArgs(AIDataArgs &args);
	virtual UINT32 GetSkillCasterTypeID() { return 0; }
	virtual UINT32 GetTag() { return 1; }

	virtual void OnRevive();

	CombatStatistics &Statistics() { return m_statitics; }
	const CombatStatistics& GetStatistics() { return m_statitics; }

	UINT64 GetSourceRoleID() { return m_srcRoleID; }

	bool BindSkillSlot(UINT32 slot, UINT32 skillhash);
	std::vector<UINT32>& GetSkills() { return m_skills; }
	std::vector<UINT32>& GetBindSkills() { return m_skillslot; }

	void AddExp(UINT32 exp);

	void  SetWaveID(int waveID){ m_WaveID = waveID; }
	int   GetWaveID(){ return m_WaveID; }
	void  SetSpawner(XLevelSpawner *pSpawner){ m_pSpawner = pSpawner; }

private:
	void InitAllyAttribute(RoleSummary *pRobotSummary);

	virtual void OnTreat(Unit *pTargetUnit, double value);
	virtual void OnAttack(Unit *pTargetUnit, double value);
	virtual void OnHurt(Unit* pCastUnit, double value);
	virtual void OnDied();
	virtual void OnEnterScene(Scene *scene);
	virtual void OnLeaveScene();

private:
	XDummyRole  *m_pXAlly;
	CombatStatistics m_statitics;
	KKSG::UnitAppearance m_appearance;
	std::string m_Name;
	UINT32 m_exp;
	UINT32 m_maxExp;
	UINT32 m_prof;
	UINT64  m_srcRoleID;
	int m_WaveID;
	XLevelSpawner* m_pSpawner;
	DummyType m_DummyType;

	std::vector<UINT32> m_skills;
	std::vector<UINT32> m_skillslot;
};

#endif