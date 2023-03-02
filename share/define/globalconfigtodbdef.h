#ifndef __GLOBALCONFIGTODBDEF_H__
#define __GLOBALCONFIGTODBDEF_H__


enum EGlobalConfigToDb
{
	eArenaRankInit			= 0,
	eWorldBossId			= 1,
	eWorldBossEnemyId		= 2,
	eLevelSealType			= 4,
	eLevelSealStartTime		= 5,
	eLevelSealUnLockTime	= 6,
	eHellHoundHardFlag		= 7,
	eLevelSealCollectCount	= 8,
	ePkRankRewardLastTime	= 9,
	eTitleMaxLevel			= 10,
	eTitleMaxLevelCnt		= 11,
	eGuildArena             = 12,
	eWorldLevel				= 13,
	eWorldLevelDay			= 14,
	eTeamCostMailLimit		= 15,
	eWeekNestLastUpTime     = 16,
	eWeekNestLastSceneIndex = 17,
	eGuildTerritory         = 18,
	ePartnerDailyResetTime  = 19,
	eLeagueBattleTime		= 20,
	eLeagueBattleEliData	= 21,
	eWeekNestLastSealType   = 22,
	eHeroRankUpdateTime		= 23,
	eGadenUpdateTime		= 24,
	eDragonExpLastUpTime    = 25,
	eGuildMgrTime           = 26,
	eCombineServerTime		= 27,
	eDragonGlobal			= 28,
	eLoginActivity			= 29,	// 登录活动
	
	eMilitaryRankTime		= 30,
	eArenaStarSeasonData    = 31,	//竞技名人堂(by sherlockzhang)
	eSkyCraftEliData		= 32,	//天空争霸赛淘汰赛对阵信息
	eSkyTeamDailyPassTime	= 33,
	eRankListWeekUpdateTime = 34,	//排行榜每周一5点刷新
	eRankListTwoWeekUpdateTime  = 35,	//排行榜两周刷新
	eCompeteDragonInfo		= 36,	//竞技龙本信息
	eNpcFeelingGlobalData   = 37,   //Npc好感度
	eSurviveGlobalData      = 38,   //绝地求生
	eRoleTaskResetTime		= 39,	//任务
	eConsumeRebate			= 40,	//消费返利


	eGlobalEventStart		= 100,	//全局事件
	eUnUse				    = eGlobalEventStart,
	eSocialMgr				= 101,	
	eActivityMail			= 102, //活动邮件
	eDuckTicketsMail		= 103, //疯鸭门票邮件;
	eQa						= 104, //记录答题次数
	eGlobalEventEnd,
	eConfigDummy
};


#endif