#ifndef __SCENEMOBAINFO_H__
#define __SCENEMOBAINFO_H__

#include "unit/combatstatistics.h"
#include "utility/tlogger.h"


enum MobaRoleField
{
	MobaRole_Exp			= 1 << 0,
	MobaRole_HeroID			= 1 << 1,
	MobaRole_KillStats		= 1 << 2,
	MobaRole_AddAttr		= 1 << 3,
	MobaRole_ReviveTime		= 1 << 4,


	MobaRole_All			= (1U << 31) - 1,
};


class MobaEnemySet
{
public:
	typedef std::unordered_set<UINT64>::const_iterator const_iterator;

	void AddSet(const std::unordered_set<UINT64>& _set)
	{
		for (auto i = _set.begin(); i != _set.end(); ++i)
		{
			m_set.insert(*i);
		}
	}

	const_iterator begin() const
	{
		return m_set.begin();
	}

	const_iterator end() const
	{
		return m_set.end();
	}

private:
	std::unordered_set<UINT64> m_set;
};


class Scene;
struct MobaHintNotify
{
	UINT32 index;
	UINT32 group;
	Scene* scene;

	MobaHintNotify(Scene* tempscene, UINT32 tempgroup, UINT32 tempindex)
	{
		index = tempindex;
		group = tempgroup;
		scene = tempscene;
	}
	void Notify(bool toother = true);
};

struct MobaTeamInfo
{	
	MobaTeamInfo()
	{
		groupLevel = 1;
		headCount = 0;
		allExp = 0;
		lasthomebeattacked = 0;
	}
	int groupLevel;
	UINT32 headCount;
	double allExp;
	double eloPoint;
	time_t lasthomebeattacked;
	std::set<UINT32> canSeeInMiniMap;
};


class SceneMobaBattle;
class Role;

class MobaRole
{
public:
	MobaRole(){};
	MobaRole(const KKSG::PvpRoleInfo& _info, SceneMobaBattle* _pSceneMoba);

	void ToKKSG(KKSG::MobaRoleData& data, UINT32 field);

	inline UINT32 GetChangeFlag() const { return m_changeFlag; }
	inline const KKSG::PvpRoleInfo& GetPvpRoleInfo() const { return m_info; }
	inline UINT64 GetRoleID() const { return m_info.roleid(); }
	inline std::string GetRoleName() const { return m_info.name(); }
	inline UINT32 GetTeamID() const { return m_info.camp(); }
	///> 游戏end之前只要在场景role都是存在的，end之后会置空
	inline Role* GetRole() const { return m_pRole; }

	inline UINT64 GetEnterGameTime() const { return m_enterGameTime; }

	inline UINT32 GetHeroID() const { return m_heroid; }
	inline bool GetCanChooseHeroID() const { return m_canChooseHero; }
	inline const std::vector<UINT32>& GetFreeWeekHero() const { return m_freeWeekHero; }
	inline const std::map<UINT32, UINT32>& GetExperienceHero() const { return m_experienceHero; }

	inline double GetExp() const { return m_exp; }
	inline UINT32 GetLevel() const { return m_level; }
	inline float GetKDA() const { return m_kda; }
	inline bool GetIsEscape() const { return m_isEscape; }

	CombatStatistics& GetCombatStatistics();

	void OnEnterGame(Role* pRole);
	void OnLeaveGame(bool isIntentional);

	void OnGameEnd(UINT64 nowTime, UINT32 timeSpan);

	void AddExp(double addExp, UINT32 posxz = 0);
	void OnLevelUp();

	void SetNextReviveTime(UINT64 nextRevive);
	void CheckRevive(UINT64 nowTime, bool isBeRevive);

	void AddExperienceHero(UINT32 heroid, UINT32 endTime);
	KKSG::ErrorCode SetHero(UINT32 heroid);	

	KKSG::ErrorCode Upgrade(UINT32 param, UINT32& nowParam);

	KKSG::ErrorCode SendSignal(UINT32 type);

	bool IsAlreadyReport(UINT64 roleid);
	void AddReporter(UINT64 roleid);	

	void FillTLog(TMobaBattleFlow& tlog);

private:
	SceneMobaBattle* m_pSceneMoba;

	Role* m_pRole;

	UINT32 m_changeFlag;		///>改变标记

	KKSG::PvpRoleInfo m_info;

	UINT64 m_enterGameTime;
	UINT64 m_nextReviveTime;
	// hero
	UINT32 m_heroid;
	bool m_canChooseHero;
	std::vector<UINT32> m_freeWeekHero;
	std::map<UINT32, UINT32> m_experienceHero;
	// level
	double m_exp;
	UINT32 m_level;
	UINT32 m_upgradeNum;

	UINT32 m_attackLevel;
	UINT32 m_defenseLevel;

	CombatStatistics m_combatStatistics;	
	// signal
	UINT64 m_firstSendSignalTime;
	UINT32 m_signalCount;

	float m_kda;
	bool m_isEscape;
	UINT64 m_escapeTime;			///> 离开的时间点

	std::set<UINT64> m_reportIDs;	///> 举报我的人

	std::string m_strOpenID;		///> OpenID;
};

#endif