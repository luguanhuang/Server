#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "unit.h"
#include "table/XEntityStatistics.h"
#include "pb/project.pb.h"

typedef XEntityStatistics::RowData EnemyTemplate;

class Enemy;
class EnemyEventListener
{
public:
	virtual ~EnemyEventListener() {};

	virtual void OnAttack(Enemy* pEnemy, Unit* pCastUnit, double value) {};
	virtual void OnHurt(Enemy* pEnemy, Unit* pCastUnit, double value) {};
	virtual void OnDied(Enemy* pEnemy) {};
};

class XLevelSpawner;
class XEnemy;
class Vector3;

std::ostream &operator<<(std::ostream &stream, Enemy *pUnit);

class Enemy : public Unit
{
	friend std::ostream &operator<<(std::ostream &stream, Enemy *pUnit);
public:
	Enemy();
	~Enemy();

	void Init(const EnemyTemplate *template_, const Vector3& pos, const float face, float scale);
	void Uninit();

	virtual void CleanUpInScene();
	virtual void Update(float delta);
	virtual void GetAppearance(KKSG::UnitAppearance &Appearance, bool brief = true);
	virtual void OnEnterScene(Scene *scene);
	virtual void OnLeaveScene();
	virtual void StreamOutput(std::ostream &stream) { stream << this; }
	virtual void OnHurt(Unit* pCastUnit, double value);
	virtual void OnTreat(Unit *pTargetUnit, double value);
	virtual void OnAttack(Unit *pTargetUnit, double value);
	virtual void OnDied();
	virtual bool HasDeathSlowCamera();
	virtual std::string GetAIString();
	virtual void GetAIArgs(AIDataArgs &args);
	virtual UINT32 GetTag() { return m_Tag; }

	const EnemyTemplate *GetTemplate();
	float GetNormalAttackProb();
	float EnterFightRange();
	void  SetWaveID(int waveID);
	int   GetWaveID();
	void  SetSpawner(XLevelSpawner *pSpawner);
	inline bool  IsBoss(){ return m_EnemyTemplate->Type == KKSG::Species_Boss; }
	inline bool  IsElite(){ return m_EnemyTemplate->Type == KKSG::Species_Elite; }
	inline bool  IsPuppet(){ return m_EnemyTemplate->Type == KKSG::Species_Puppet; }
	
	///> AI
	void EnableAI(bool isenable = true);
	///> нч╣п
	void Invincible(bool isvincible = true);

	void SetName(const std::string& name);
	const std::string& GetName();

	void AddListener(EnemyEventListener* pListener);
	void ClearListener(){ m_eventListener.clear(); }

	inline KKSG::EntitySpecies Type(){ return (KKSG::EntitySpecies)m_EnemyTemplate->Type; }

private:
	bool DoDeadDisappear();
	void _CreateTag();
private:
	int m_WaveID;
	std::string m_name;
	const EnemyTemplate* m_EnemyTemplate;

	XEnemy *m_pXEnemy;
	XLevelSpawner *m_pSpawner;

	std::list<EnemyEventListener*> m_eventListener;

	UINT32 m_Tag;
};

#endif 