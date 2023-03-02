#ifndef __COMBATSTATISTICS_H__
#define __COMBATSTATISTICS_H__

class Unit;

class CombatStatistics
{
public:
	CombatStatistics();
	~CombatStatistics();

	void SetUnit(Unit *pUnit);
	Unit* GetUnit() { return m_pUnit; }

	void UpdateDPS();

	void Update();
	void UpdateAssit(UINT64 nowTime);

	void AddTreatInfo(Unit *pTargetUnit, double value);
	void AddAttackInfo(Unit *pTargetUnit, double value);
	void AddComboCount();

	void AddOpenChest();
	void AddDeathCount();
	void Reset();
	void BeHit();
	void BeHurt(Unit* pCastTarget, double value);

	void SetMaxCombo(int combo) { m_comboMax = combo; }
	void SetBehit(int behit) { m_behitCount = behit; }
	void SetOpenChestCount(int count) { m_openChestCount = count; }
	
	int GetAssistNum() const { return m_assistNum; }
	void AddAssistNum() { m_assistNum++; }
	void DelAssistNum() { if (m_assistNum > 0) m_assistNum--; }
	std::vector<UINT64>& GetAssistRoleIDs() { return m_assistIDs; }
	UINT64 GetLastAssist();

	int GetDeathCount() const { return m_deathCount; }
	int GetMaxCombo() const { return m_comboMax; }
	int GetCombo() const { return m_comboCount; }
	int GetBehit() const { return m_behitCount; }
	int GetOpenChestCount() const { return m_openChestCount; }
	double GetTotalDamage() const { return m_totalDamage; }
	float GetTotalSkillTime() const { return m_timeTotal; }
	double GetBossDamage() const { return m_bossdamage; }
	double GetTotalTreat() const { return m_totalTreat; }
	double GetBeHitDamage() const { return m_beHitDamage; }

	void UpdateMultiKillCount();
	UINT32 GetMultiKillCount();
	UINT32 GetMultiKillCountMax() const { return m_multiKillCountMax; };

	void AddKillCount();
	int GetKillCount() const;
	int GetKillContinueCount();
	int GetKillContinueCountMax()const;
	void ClearKillContinueCount() { m_killContinueCount = 0; }

	void AddKillEnemyCount(UINT32 enemyTemplateID);
	std::map<UINT32, UINT32> GetKillEnemyCount() const;
	int GetKillEnemyScore() const;

	void AppendTime();
	void MarkTimeBaseLine();
	void MarkTimEndLine();
	void StartPrintDamage(float time);
	double StopPrintDamage();

public:
	UINT64      m_lastdpstime;
	UINT64		m_lastAssitTime;
	float		m_dps;

	UINT64      m_lastComboTime;
	double      m_totalDamage;
	double		m_bossdamage;
	double		m_totalTreat;		///> 治疗
	double		m_beHitDamage;		///> 承受伤害量
	int         m_deathCount;
	int         m_comboCount;
	int         m_comboMax;
	int         m_behitCount;
	int         m_openChestCount;

	///>  kill 只是记录了击杀的Role和DummyRole
	int         m_killCount;		///> 击杀多少次
	int         m_killContinueCount;///> 当前多杀
	int         m_killContinueCountMax;///> 最大多杀
	int			m_assistNum;
	int			m_multiKillCount;	///> 连杀
	int			m_multiKillCountMax;///> 最大连杀数
	UINT64		m_lastKillTime;		///> 最后一次杀人时间

	float		m_timeTotal;
	float		m_timeBase;
	float		m_timeEnd;

	double		m_startPrintDamage;		///> GM指令打印伤害量的起点

	std::map<UINT32, UINT32> m_killEnemyCount;
	std::map<UINT64, UINT64> m_HurtRoleID;	///> 打过自己的Role
	std::vector<UINT64> m_assistIDs;	

	Unit       *m_pUnit;
};

#endif // __COMBATSTATISTICS_H__