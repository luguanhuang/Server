#ifndef __HEROBATTLERECORD_H__
#define __HEROBATTLERECORD_H__

#include "unit/roleserilizeinterface.h"


enum UseHeroType
{
	UseHeroType_HeroBattle	= 0,
	UseHeroType_MorphInfo	= 1,
	UseHeroType_MobaBattle	= 2,
};


struct THeroBattleRoundVar
{
	THeroBattleRoundVar()
	{
		m_IsMvp = 0;
	}
	UINT32 m_elo;
	UINT32 m_newElo;
	UINT32 m_result;
	UINT32 m_killNum;
	UINT32 m_deadNum;
	UINT32 m_IsMvp;
};

class Role;
class CHeroBattleRecord : public RoleSerializeInterface
{
public:
	CHeroBattleRecord(Role* pRole);
	~CHeroBattleRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	void Update();

	UINT32 GetRandomHero(const std::vector<UINT32>& freeHero, const std::map<UINT32, UINT32>& experienceHero, const std::set<UINT32>& exceptHero);
	bool IsHaveHero(UINT32 heroid);
	KKSG::ErrorCode BuyHero(UINT32 heroid);

	void FillHeroBattleInfo(KKSG::GetHeroBattleInfoRes& res);	
	void FillHeroBattleGameRecord(KKSG::GetHeroBattleGameRecordRes& res);

	UINT32 GetTodaySpCount();
	void AddTodaySpCount();

	UINT32 GetTodayBigRewardRate();
	void AddTodayBigRewardCount();

	UINT32 GetWeekPrize();
	bool CanGetNextWeekPrize();

	double GetELOPoint();
	void SetELOPoint(double point);

	void UpdateMSRankList();

	const std::unordered_set<UINT32>& GetHero() { Update(); return m_hero; }
	const std::vector<UINT32>& GetFreeWeekHero() { Update(); return m_freeWeekHero; }
	const std::map<UINT32, UINT32>& GetExperienceHero() { Update(); return m_experienceHero; }

	void AddGameRecord(const KKSG::HeroBattleOneGame& oneGame, UINT32 killNum);

	void RunAway(UINT32 killNum);

	KKSG::ErrorCode GetWeekNextReward();

	KKSG::ErrorCode UseExperienceHeroTicket(UINT32 itemid);

	void PrintHeroBattleRoundTLog(const THeroBattleRoundVar& var, const std::string& battleTag);	

	UINT32 GetDayTimes() { Update(); return m_daytimes; }

	UINT32 GetTotalNum() { Update(); return m_totalNum; }

	void OnBuyPayMember(int ID);

private:
	Role* m_pRole;
	Switch m_isModify;

	std::unordered_set<UINT32> m_hero;
	
	UINT32 m_winThisWeek;
	UINT32 m_todaySpCount;
	UINT32 m_lastUpdateTime;

	UINT32 m_totalNum;
	UINT32 m_winNum;
	UINT32 m_loseNum;

	UINT32 m_bigRewardCount;
	std::vector<UINT32> m_freeWeekHero;
	std::map<UINT32, UINT32> m_experienceHero;

	std::list<KKSG::HeroBattleOneGame> m_gameRecord;

	UINT32 m_weekprize;			// 每周奖励领取阶段

	double m_eloPoint;			// ELO分数
	UINT32 m_dayTime;			// 当日时间
	UINT32 m_daytimes;			// 每日参加次数

	UINT32 m_continueWinNum;	// 最大连胜次数
	UINT32 m_maxKillNum;		// 最大杀人次数
};


#endif