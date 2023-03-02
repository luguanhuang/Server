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
	eLoginActivity			= 29,	// ��¼�
	
	eMilitaryRankTime		= 30,
	eArenaStarSeasonData    = 31,	//����������(by sherlockzhang)
	eSkyCraftEliData		= 32,	//�����������̭��������Ϣ
	eSkyTeamDailyPassTime	= 33,
	eRankListWeekUpdateTime = 34,	//���а�ÿ��һ5��ˢ��
	eRankListTwoWeekUpdateTime  = 35,	//���а�����ˢ��
	eCompeteDragonInfo		= 36,	//����������Ϣ
	eNpcFeelingGlobalData   = 37,   //Npc�øж�
	eSurviveGlobalData      = 38,   //��������
	eRoleTaskResetTime		= 39,	//����
	eConsumeRebate			= 40,	//���ѷ���


	eGlobalEventStart		= 100,	//ȫ���¼�
	eUnUse				    = eGlobalEventStart,
	eSocialMgr				= 101,	
	eActivityMail			= 102, //��ʼ�
	eDuckTicketsMail		= 103, //��Ѽ��Ʊ�ʼ�;
	eQa						= 104, //��¼�������
	eGlobalEventEnd,
	eConfigDummy
};


#endif