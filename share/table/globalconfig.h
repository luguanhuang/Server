#ifndef _SHARE_GLOBALCONFIG_H_
#define _SHARE_GLOBALCONFIG_H_

#include "cvsreader.h"

typedef std::function<bool(void)> ReloadGlobalConfigCallBack;

struct GlobalConfig
{
private:
	GlobalConfig() {};

public:
	bool LoadFile();
	const std::string &GetConfig(const char *Key);
	int GetInt(const char *Key, int default_);
	float GetFloat(const char *Key, float default_);

	static GlobalConfig& GetInstance()
	{
		static GlobalConfig instance;
		return instance;
	}

	std::map<std::string, ReloadGlobalConfigCallBack> m_key2reload;
	std::unordered_map<std::string, std::string> m_mapKeyValue;

	bool InitFunc();
	bool InitKeyValue();


	bool Reload(const std::string& param);
	bool InitBigMelee();
	bool InitTeam(); // ���
	bool InitPk(); // ������
	bool InitPvp(); // �����ӳ�
	bool InitGmf(); //������̨ս-ս����(������ �۷�Ծ���)
	bool InitCastle(); //�������-ս����
	bool InitInvFight(); //�д�
	bool InitDEBossRush(); //��֮Զ��/BossRush
	bool InitFriend();
	bool InitMail();
	bool InitArena();
	bool InitCard();
	bool InitCombat();
	bool InitCommon();
	bool InitBoss();
	bool InitSceneView();
	bool InitGoldClick();
	bool InitItem();
	bool InitAuct();
	bool InitGuildAuct();
	bool InitGarden();
	bool InitPoker();
	bool InitPersonalCareer();
	bool InitGuildInherit();  // ���ᴫ��
	bool InitGuildTerr();     // �������
	bool InitGuildLadder();   // ��������
	bool InitFlower();   // �ʻ�
	bool InitMentor(); //ʦͽ
	bool InitArenaStar(); //����������
	bool InitPlatForm();	// ƽ̨���
	bool InitPartner();
	bool InitAI();
	bool InitCustomBattle();

	bool InitMilitaryRank();
	bool InitSkyCraft();

	bool InitGuildCampParty();
	bool InitBattleField();
	bool InitWedding();
	bool InitTask();

	bool InitNpcFeeling();
	bool InitSurvive();

	bool InitHG();//������Ϸ;
	bool InitLuckyTable();//ת�̻

	// ����
	bool InitPlatFormShare();
	// friend 1
	UINT32 GiftSendMinDegree;
	UINT32 GiftSendMaxTimes;
	UINT32 GiftReceiveMaxTimes;
	UINT32 FriendMaxDegree;
	UINT32 FriendMaxCount;
	UINT32 ApplyMaxCount;
	UINT32 BlackMaxCount;
	UINT32 FriendSystemRefreshAddListCD;
	int FriendExpireTime;
	int FriendExpireLevel;
	int FriendUpdateCount;
	///////////////////////////////////////////////////////////////////////////////////

	// team 1
	UINT32 TeamMatchDelay;
	UINT32 TeamFastMatchConfirmT;
	UINT32 TeamFastMatchBuff;
	UINT32 TeamVoteTime; 
	UINT32 TeamInviteConfirmTime;
	UINT32 TeamInvUnfConfirmTime;
	UINT32 TeamUnfRefuseForNowT;
	int TeamInviteHistoryCount;
	UINT32 TeamInvRefuseHistoryTime;
	UINT32 TeamInvIgnoreHistoryTime;
	UINT32 TeamKickLeaderNoticeTime;
	Sequence<UINT32, 2> TeamGuildBuff;
	int TeamInviteFriendCount;
	int TeamInviteGuildCount;
	int TeamInviteRecCount;
	UINT32 TeamInviteHistoryTime;
	UINT32 TeamCostMailLimit;
	vector<UINT32> BuyNestCountCost;
	vector<UINT32> BuyAbyssCountCost;
	UINT32 TeamDiamondCostCount;
	UINT32 TeamDiamondCostLimit;//����ʯ�ͽ�������
	UINT32 TeamDiamondRevenue;//��ʯ����˰
	UINT32 TeamKingBackRewardCountLimit;
	vector<Sequence<UINT32,2>> TeamKingBackRewards;

	bool TeamGrayOpen;
	std::unordered_set<UINT32> TeamGrayServerIDs;//�Ҷȷ�

	int TeamLevelSealGap;
	int TeamCostWorldLevel;

	UINT32 TeamUseTicketCount;
	//��Ѩʹ����Ʊ��ɻ����Ʒ�嵥
	std::set<UINT32>	NestDropItemsOnUseTicket;

	///////////////////////////////////////////////////////////////////////////////////

	// arena 1
	int ArenaNpcCount;
	int ArenaRankBegin;
	int ArenaScale;
	vector<Sequence<int, 2>> ArenaFighters;
	///////////////////////////////////////////////////////////////////////////////////

	// mail 1
	UINT32		m_dwRoleMailCacheMaxNum;
	std::string SendGold_MailTitle;
	std::string SendGold_MailContent;
	std::string SendDiamond_MailTitle;
	std::string SendDiamond_MailContent;
	std::string SendFlower_MailTitle;
	std::string SendFlower_MailContent;
	std::string FlowerWeekRank_MailTitle;
	std::string FlowerWeekRank_MailContent;
	std::string FlowerValentinesDayRank_MailTitle;
	std::string FlowerValentinesDayRank_MailContent;
	std::string Flower520DayRank_MailTitle;
	std::string Flower520DayRank_MailContent;
	std::string FlowerActivityDayRank_MailTitle;
	std::string FlowerActivityDayRank_MailContent;
	std::string MentorTask_MailMasterTitle;
	std::string MentorTask_MailMasterContent;
	std::string MentorTask_MailStudentTitle;
	std::string MentorTask_MailStudentContent;
	std::string Mentor_CompleteRewardTitle;
	std::string Mentor_CompleteRewardContent;
	std::string MentorBreakTitle;
	std::string MentorBreakContent;
	std::string GuildArenaPointRaceWinTitle;
	std::string GuildArenaPointRaceWinContent;
	std::string GuildArenaPointRaceLoseTitle;
	std::string GuildArenaPointRaceLoseContent;
	std::string GuildCampRankMailTitle;
	std::string GuildCampRankMailContent;
	///////////////////////////////////////////////////////////////////////////////////

	// pk 1
	Sequence<UINT32, 5> PkTime;
	Sequence<UINT32, 3> PkDayReward;
	int PkSceneId;
	int PkPrepareTime;
	int PkUpdateTime;
	int PkTwoUpdateTime;
	UINT32 PkMatchDelay;
	Sequence<UINT32, 2> PkAddRobotPoint;
	UINT32 PkWeekOffset;
	UINT32 PkWeekResetNum;
	UINT32 PkWeekResetBaseTime;
	UINT32 PkMatchToWorldTime;
	int PkBasePoint;
	std::vector<Sequence<UINT32,2>> PkResetPoint;
	UINT32 PkLoadWaitTime;
	UINT32 PkTwoMatchToWorldTime;
	UINT32 PkRoleMatchPairDayLimit;
	UINT32 PkRoleMatchPairWeekLimit;
	std::vector<Sequence<UINT32,2>> PkMatchLevelRange;
	UINT32 PkGetResetLeftTime(UINT32 lastResetTime, UINT32 now);
	bool PkInSameLevelRange(UINT32 lev1, UINT32 lev2);
	std::string GetCoverName(const std::string &name,UINT32 nType);
	///////////////////////////////////////////////////////////////////////////////////

	// auction 1
	UINT32 AuctOnSaleTime;
	UINT32 AuctSaleDelayMin;
	UINT32 AuctSaleDelayMax;
	UINT32 AuctRecoverTime;
	INT32 AuctCartItemAllCount;
	INT32 AuctCartItemTreasurePercent;
	INT32 AuctCartItemTreasureCount;
	INT32 AuctCartItemNormalCount;
	UINT32 AuctWeekTradeLimit;
	UINT32 AuctTradeRecordTime;
	UINT32 AuctAVGPricePreHigh;
	UINT32 AuctAVGPricePreLow; 
	UINT32 AuctFreeRefreshCount;
	UINT32 AuctAutoRefreshTime;
	UINT32 AuctFreeRefreshTime;
	UINT32 AuctRevenueRate;
	UINT32 AuctRefreshPayCount;
	UINT32 AuctNormalItemCoolDownDay;
	UINT32 AuctTreasureItemCoolDownDay;
	///> ����������ʱ��
	Sequence<UINT32,2> AuctSupplementTime;
	UINT32 AuctSupplementFrequency;
	///> ��ʱ��ˢ�µ�ʱ��
	std::vector<Sequence<UINT32,4>> AuctRefreshTimeSegment;
	std::vector<Sequence<UINT32,3>> AuctItemCoolDownDay;
	// ���ﳵ��Ҫ���ͼ��������͵�����
	std::unordered_set<UINT32> AuctItemSpecialPush;

	// 7��ƽ������������
	UINT32 AverageConsume7Day;

	///////////////////////////////////////////////////////////////////////////////////

	// guild auction 1
	vector<UINT32> GuildActLvl;
	UINT32 GuildActLeaDay;
	UINT32 GuildActDelMax;
	std::string GuildActDailyTitle;
	std::string GuildActDailyCon;
	std::string GuildActWeekTitle;
	std::string GuildActWeekCon;
	std::string GuildActDelTitle;
	std::string GuildActDelCon;
	std::string GuildActLeaDownTitle;
	std::string GuildActLeaDownCon;
	std::string GuildActLeaUpTitle;
	std::string GuildActLeaUpCon;

	UINT32 GuildAuctGuildSaleTime;
	UINT32 GuildAuctWorldSaleTime;
	UINT32 GuildAuctUpRate;
	UINT32 GuildAuctGuildDue;
	UINT32 GuildAuctWorldDue;
	///> ���������ڹ�ʾʱ��
	UINT32 GuildAuctGuildItemPublicityTime;
	///> ������������Ʒ��ʱ
	UINT32 GuildAuctItemDelayTime;
	std::vector<Sequence<UINT32,2>> GuildAuctProfitMax;
	///> ����������Ʒ�ɽ���������������еı���ϵ��
	std::vector<Sequence<float,2>> GuildAuctGuildItemExRate;
	///////////////////////////////////////////////////////////////////////////////////

	// guild castle 1
	UINT32 GuildCastleReadyTime;
	UINT32 GuildCastleFightTime;
	UINT32 GuildCastleBattleRoleMax;
	UINT32 GuildCastleAddPointInterval;
	UINT32 GuildCastleAddPointCount;
	UINT32 GuildCastleEndPoint;
	UINT32 GuildCastleJvDianOccupyTime;
	std::vector<Sequence<float, 3>> GuildCastleJvDianPos;
	std::vector<float> GuildCastleJvDianRadius;
	std::vector<UINT32> GuildCastleScenes;
	std::vector<UINT32> GuildCastleMulPoint;
	std::vector<UINT32> GuildCastleFeatsRules;
	///////////////////////////////////////////////////////////////////////////////////

	// guild boss
	UINT32 GuildBossScenePlayer;
	std::string GuildBossJoinMailTitle;
	std::string GuildBossJoinMailContent;
	std::string GuildBossFinishMailTitle;
	std::string GuildBossFinishMailContent;
	std::string GuildBossFirstkillMailTitle;
	std::string GuildBossFirstkillMailContent;
	std::string GuildBossPresidentMailTitle;
	std::string GuildBossPresidentMailContent;
	std::string GuildBossOfficialMailTitle;
	std::string GuildBossOfficialMailContent;
	std::string DesignationFlowerMailTitle;
	std::string DesignationSendFlowerMailContent;
	///////////////////////////////////////////////////////////////////////////////////

	// guild arena 
	std::string GuildArenaBattleWinTitle;
	std::string GuildArenaBattleWinContent;	// none 
	std::string GuildArenaBattleLoseTitle;
	std::string GuildArenaBattleLoseContent;
	std::string GuildArenaBattleChampionTitle;
	std::string GuildArenaBattleChampionContent;
	std::string GuildArenaBattleSecondTitle;
	std::string GuildArenaBattleSecondContent;
	std::string GuildArenaBattleChampionLeaderTitle;
	std::string GuildArenaBattleChampionLeaderContent;
	std::string GuildArenaBattleSecondLeaderTitle;
	std::string GuildArenaBattleSecondLeaderContent;
	Sequence<UINT32, 3> GuildArenaBeforeWar;
	std::vector<Sequence<UINT32, 15> > GuildArenaTimeVec;

	UINT32 GuildArenaDelayTime;
	UINT32 GuildArenaOriginal;    // ������̨ս�����ڼ��ܿ�ʼ
	UINT32 GuildArenaSignupLevel; // ������̨ս�����ȼ�
	std::vector<Sequence<UINT32, 3> > GuildArenaSignupTime; // ������̨ս����ʱ��
	std::vector<Sequence<UINT32, 3> > GuildArenaSingle;     // ������̨ս���ܿ���ʱ��
	std::vector<Sequence<UINT32, 3> > GuildArenaDouble;     // ������̨ս˫�ܿ���ʱ��

	Sequence<UINT32, 5> GuildArenaFinal;
	Sequence<UINT32, 4> GuildArenaFinalLamp;
	Sequence<UINT32, 4> GuildArenaFinalNotice;
	std::vector<Sequence<UINT32, 5> > GuildArenaApply;     // ������̨ս��������ʱ��
	std::vector<Sequence<UINT32, 2> > GuildArenaSingleNotice; // 
	std::vector<Sequence<UINT32, 2> > GuildArenaDoubleNotice; //

	///////////////////////////////////////////////////////////////////////////////////

	// sky city
	UINT32 SkyCityFirstHalfTime;
	UINT32 SkyCityHalfTime;
	UINT32 SkyCitySpaceTime;
	UINT32 SkyCitySecondHalfTime;
	UINT32 SkyCityGames;
	UINT32 SkyCityFloor;
	UINT32 SkyCityWaitingRoomTime;
	UINT32 SkyCityChangeSceneTime;
	UINT32 SkyCityEnterLv;
	vector<UINT32> SkyCityFightSceneID; 
	vector<float> SkyCityRestPos;
	UINT32 SkyCityLastResultDelay;
	UINT32 SkyCityLookupId;
	int SkyArenaStartAnimTime;
	UINT32 SkyCity2FloorGroup;

	///////////////////////////////////////////////////////////////////////////////////

	// res war
	UINT32 ResWarWaitingTime;
	UINT32 ResWarPVPSceneID;
	UINT32 ResWarPVESceneID;
	UINT32 ResWarExploreTime;
	UINT32 ResWarOpenGroupWeek;
	UINT32 ResWarPVPTime;
	UINT32 ResWarDNEID;
	UINT32 ResWarPVEMineSize;
	float ResWarFightRate;
	UINT32 ResWarMatchTime;
	UINT32 ResWarGuildLv;
	UINT32 ResWarNoticeTime;
	UINT32 ResWarDestoryTime;
	UINT32 ResWarLevelLimit;
	UINT32 ResWarEnemyAttack;
	UINT32 ResWarSwitch;
	UINT32 ResWarPullGroup;
	///////////////////////////////////////////////////////////////////////////////////

	// guild
	UINT32 MaxGetGuildCheckInBonusNum;
	UINT32 GuildBonusAskTimeSpan;
	UINT32 GuildBonusNoticeGetAllTime;
	vector<Sequence<UINT32, 2>> GuildCheckInBonusTime;
	vector<Sequence<UINT32, 2>> GuildBonusOnLineNumAddPercent;
	UINT32 GuildApplyMax;
	UINT32 GuildLadderNum;
	UINT32 LeaveGuildCount;
	UINT32 LeaveGuildCD;
	UINT32 GuildLadderRewardTimes;
	vector<Sequence<UINT32, 2>> GuildLadderReward;
	vector<Sequence<UINT32, 3>> GuildLadderTime;
	std::string GuildLadderTitle;
	std::string GuildLadderCon;
	std::string GuildLadderRoleTitle;
	std::string GuildLadderRoleCon;
	std::set<UINT32> GuildLadderWinNotice;
	UINT32 GuildRankRewardTime;
	UINT32 GuildRankRewardKeepTime;
	UINT32 DelGuildExp;
	UINT32 DelGuildPre;
	///////////////////////////////////////////////////////////////////////////////////

	// card 1
	UINT32 CardMatchWaiting;
	UINT32 CardMatchRoundWaiting;
	UINT32 CardMatchRounding;
	UINT32 CardMatchRound;
	UINT32 CardMatchChange;
	UINT32 CardMatchChangeAdd;
	///////////////////////////////////////////////////////////////////////////////////

	// combat 1
	double CriticalLimit;
	double CritResistLimit;
	double ParalyzeLimit;
	double ParaResistLimit;
	double StunLimit;
	double StunResistLimit;
	double MagicAvoidenceLimit;
	double PhycialAvoidenceLimit;
	double FinalDamageLimit;
	double CritDamageBase;
	double ElemAtkLimit;
	double ElemDefLimit;
	double GeneralCombatParam;
	double AttrChangeDamageLimit;
	float CDChangeUpperBound;
	float CDChangeLowerBound;
	double AttackSpeedUpperBound;
	double AttackSpeedLowerBound;
	double BuffCastDamageChangeUpperBound;
	double BuffCastDamageChangeLowerBound;
	double BuffReceivedDamageChangeUpperBound;
	double BuffReceivedDamageChangeLowerBound;
	double MpCostChangeUpperBound;
	double MpCostChangeLowerBound;
	double CritDamageUpperBound;
	double CritDamageLowerBound;
	double DamageRandomUpperBound;
	double DamageRandomLowerBound;
	double RunSpeed;
	double WalkSpeed;
	double DashSpeed;
	double TurnSpeed;
	double AutoTurnSpeed;
	double AttackSpeed;
	double SkillCD;
	float StunTime;
	float Hit_PresentStraight;
	float Hit_HardStraight;
	float Hit_Offset;
	float Hit_Height;
	float InitEnmityValue;
	vector<Sequence<int, 3>> ComboBuff;
	UINT32 ComboInterval;
	Sequence<int, 2> ReviveBuff;
	vector<int> EntitySummonGroupLimit;

	std::vector<UINT32> PowPointAdjust;
	float  PowPointBaseValue;
	float  PowPointMaxPoint;

	float BuffMinAuraInterval;
	float BuffMinRegenerateInterval;

	float SkillReviveHpPer;
	///////////////////////////////////////////////////////////////////////////////////

	// common 1
	int InitialReturnSceneID;
	int MaxFatigue;
	int MaxRecoverFatigue;
	int FatigueRegeneration;
	int RoleSaveInterval;
	int RoleSummarySaveInterval;
	UINT32 MaxRole;
	vector<int> MpRecovery;
	int InitialSceneID;
	int BraveReturnServerSceneID;
	UINT32 MailMax;
	int ArenaSceneID;
	int SkillPointPPTWeight;
	int DailyFlowerCount;

	UINT32 SkillPageNewOpen;
	UINT32 ProfessionChangeCount;
	UINT32 ProfessionChangeLevel;
	UINT32 ProfessionChangeTime;
	bool ProfessionChangeSkill;
	Sequence<UINT32, 2>	ProfessionChangeUseItem;
	std::vector<UINT32> ProfessionChangeShop;
	std::vector<UINT32> LevelChangeShop;

	Sequence<int, 4> DinnerTime;
	Sequence<int, 4> SupperTime;
	vector<Sequence<int, 2>> DinnerReward;
	vector<Sequence<int, 2>> SupperReward;

	///> ��¼����
	std::vector<Sequence<UINT32,3>> LoginRewards;
	INT32 LoginRewardOpenLevel;

	UINT32 RoleNameLenMin;
	UINT32 RoleNameLenMax;

	UINT32 SecondPromoteSecneID;	// ��ת����ID

	std::vector<UINT32> SkillSlotUnlockLevel;

	float LevelSealExpRate;

	int RolesummaryExpireTime;
	///////////////////////////////////////////////////////////////////////////////////

	// guild boss/ world boss 1
	vector<UINT32> WorldBossGuildConsume;
	vector<Sequence<int, 2>> WorldBossGuildAddAttr;
	vector<Sequence<int, 2>> WorldBossAddAttr;
	vector<Sequence<int, 2>> WorldBossKillReward;
	Sequence<int, 4> WorldBossPoint;
	vector<Sequence<int, 2>> GuildBossAddAttr;
	Sequence<int, 4> GuildBossPoint;
	int WorldBossSceneLimit;
	int WorldBossGameEndCountDown;
	int WorldBossRoleLimit;
	int WorldBossPlayTime;
	int WorldBossPlayCount;
	int WorldBossAttrCount;
	int WorldBossGuildAttrCount;

	///////////////////////////////////////////////////////////////////////////////////


	int WorldBossCountDown;
	int WorldBossTimeEnd;
	int WorldBossSyncTime;


	// item 1
	std::vector<int> AttributeEmblemSlotLevelLimit;
	std::vector<int> SkillEmblemSlotLevelLimit;
	std::vector<int> ExtraSkillEmblemSlotLevelLimit;
	std::vector<int> ExtraSkillEmblemSlotDragonCoin;

	vector<int> JadeMosaicLevel;
	vector<UINT32> JadeLevelUpCost;
	int DecomposeRandom;
	float DecomposeParam;
	std::vector<Sequence<float, 2>> EnhanceTransRate;
	///> ϴ��
	std::vector<UINT32> SmeltCorlorRange;
	UINT32 ForgeSmeltTransRate;

	std::vector<UINT32> EquipAttackSmeltExchanged;
	std::vector<UINT32> EquipDefenseSmeltExchanged;
	std::vector<UINT32> EmblemSmeltExchanged;

	std::set<UINT32> EnhanceNotice1;
	std::set<UINT32> EnhanceNotice2;
	std::set<UINT32> EnhanceNotice3;

	std::vector<Sequence<UINT32, 3>> ExchangeRate;

	///> ����۸�
	UINT32 blueBirdPrice;
	///////////////////////////////////////////////////////////////////////////////////

	// pet
	bool IsOpenPairPet;
	vector<Sequence<UINT32, 2>> HungryExpPercent;
	vector<UINT32> PetSeatBuy;
	vector<Sequence<UINT32, 2>> PetExpTransferCost;
	///////////////////////////////////////////////////////////////////////////////////

	// T show
	int TShowFreeVoteNum;
	Sequence<int,2> TShowVoteCostNum;
	vector<Sequence<int, 2>> TShowFirstVoteReward;
	///////////////////////////////////////////////////////////////////////////////////

	// flower
	vector<Sequence<int, 2>> Flower2Charm;
	vector<UINT32> GetFlowerTime;
	UINT32 GetFlowerNum;
	vector<Sequence<int, 3>> FlowerCostNum;
	UINT32 FlowerRainTime;
	///////////////////////////////////////////////////////////////////////////////////

	// scene view 1
	int ViewGridCount;
	int ViewGridLen;
	int ViewGridSec;
	UINT32 ViewGridFriendDegree;
	UINT32 ViewGridCanBeSee;
	UINT32 ViewGridOnceLimit;
	UINT32 ViewGridTryLimit;
	std::set<int> ViewGridScene;
	UINT32 SceneWaitTimeout;
	UINT32 SceneGroupUpdate;
	std::set<int> PVPOne;
	std::set<int> CanEnterScene;
	std::vector<Sequence<UINT32, 2>> DragonSceneCanReviveCD;
	std::set<int> DragonSceneReviveExcept;
	std::set<int> ViewGroupScene;
	UINT32 ViewGroupLen;
	///////////////////////////////////////////////////////////////////////////////////

	// ƽ̨���
	// QQ/WX
	std::vector<INT32> QQVipHintDay;					// QQ��Աÿ�ܼ���ʾ���
	std::vector<Sequence<UINT32, 2>> QQVipGift;
	std::vector<Sequence<UINT32, 2>> QQSVipGift;
	std::vector<Sequence<UINT32,2>> QQVipSignIn;
	std::vector<Sequence<UINT32,2>> QQSVipSignIn;
	Sequence<UINT32,2> QQVipLevelReward;
	Sequence<UINT32,2> QQSVipLevelReward;

	std::vector<Sequence<UINT32,2>> QQGameCenterSingnIn;
	std::vector<Sequence<UINT32,2>> WXGameCenterSingnIn;
	Sequence<UINT32,2> QQGameCenterLevelReward;
	Sequence<UINT32,2> WXGameCenterLevelReward;
	std::vector<INT32> EGameHintDay;				// ���羺ÿ�ܼ���ʾ���
	std::vector<INT32> XinYueHintDay;				// ����ÿ�ܼ���ʾ���

	std::string OpenCgiUrl;
	std::vector<Sequence<UINT32, 4>> OpenCGIChest;
	std::vector<Sequence<UINT32, 4>> OpenWebChatChest;
	///////////////////////////////////////////////////////////////////////////////////

	// pay
	bool IsPayFirstDouble;
	UINT32 PayReward;                                       ///> ֧�����
	///////////////////////////////////////////////////////////////////////////////////

	// gold click 1
	UINT32 GoldClickFreeLimit;
	UINT32 GoldClickFreeTime;
	UINT32 GoldClickBaseCount;
	Sequence<UINT32, 2> GoldClickConsume;
	std::vector<Sequence<UINT32, 2>> GoldClickCount;
	///////////////////////////////////////////////////////////////////////////////////
	///> ս������
	UINT32 LeagueBattleLevelSealType;						// ������Ҫ�Ľ������
	UINT32 LeagueBattleNeedLevel;							// ������Ҫ����ҵȼ�
	bool   LeagueBattleIsOpen;								// �����Ƿ���
	std::vector<Sequence<UINT32, 3> > LeagueBattleOpenDate;				// �����������ں�ʱ��
	UINT32 LeagueMatchSign;									// ��ʾ�ӻ�򿪵����������3������24��ر��淨��������1��3������24��رձ�����			
	UINT32 LeagueRacePointLastDay;							// ����������ʼ���㣬��������8����24��00������1.4-11��
	UINT32 LeagueEleminationAfterDay;						// �����������������뱾����̭����ʱ��
	UINT32 LeagueCrossRacePointAfterDay;					// ������̭��������������������ʱ��
	UINT32 LeagueCrossRacePointLastDay;						// �������������ʱ��
	UINT32 LeagueCrossEleminationAfterDay;					// ���������������������̭����ʱ��
	UINT32 LeagueSeasonEndAfterDay;							// ��̭��������ú������ر�
	std::vector<UINT32> LeagueBattlePointRaceWeekDay;		// �������ܼ�
	std::vector<UINT32> LeagueBattleCrossPointRaceWeekDay;	// ����������ܼ�
	Sequence<UINT32, 2> LeagueBattlePointRaceTime;			// ������ÿ��ʱ��
	Sequence<UINT32, 4> LeagueBattleEliminationTime;		// ��̭��ÿ��ʱ��
	UINT32 LeagueBattleEleMapTime;							// �����������������ɶ���ͼ
	Sequence<UINT32, 2> LeagueBattleRaceReward;				// ������ǰ���ٳ�������
	UINT32 LeagueClearOpen;									// ��ǲ��������������ݿ���
	UINT32 LeagueClearAfterCrossRacePointDay;				// ��ǲ����������ݾ��뱾����̭��ʱ��

	UINT32 LeagueTeamRoleNum;								// ����ս����Ҫ������
	UINT32 LeagueTeamNameLenMin;							// ����ս��������̳���
	UINT32 LeagueTeamNameLenMax;							// ����ս�����������
	UINT32 LeagueInitPoint;									// ���������ʼ����
	UINT32 LeagueRankJoinCrosssPointRace;					// ������������ǰ���������Խ�����������
	UINT32 LeagueBattleReadyTime;							// ����������׼��ʱ��
	UINT32 LeagueBattleOneFightTime;						// ����ÿС��ս����ʱ��
	UINT32 LeagueBattleReadyTimeForEighth;					// ������̭��׼��ʱ��
	UINT32 LeagueBattleOneFightTimeForEighth;				// ������̭��ÿС��ս��ʱ��
	UINT32 LeagueBattleFightAfterTime;						// ����ÿС��ս����ʱ��
	UINT32 LeagueBattleSceneId;								// ����ս������ID
	UINT32 LeagueBattleRecordNum;							// ��ս��¼���

	UINT32 SkyTeamRoleNum;									// ���ս����Ҫ������
	UINT32 SkyTeamNameLenMin;								// ���ս��������̳���
	UINT32 SkyTeamNameLenMax;								// ���ս�����������
	UINT32 SkyTeamInitPoint;								// ���ս�ӳ�ʼ����
	UINT32 SkyCraftSceneId;									// �������������ID
	UINT32 SkyCraftRecordNum;								// ���ս�Ӷ�ս��¼����
	UINT32 SkyCraftDailyNum;								// ���������������ÿ�ճ���

	// �������
	std::vector<UINT32> LeagueApplyNoticeTime;				// ���������ڼ书��ʱ��
	std::vector<UINT32> LeagueApplyEndNoticeTime;			// �������һ�칫��ʱ��
	std::vector<UINT32> LeagueRaceNoticeTime;				// ������ÿ�չ���
	std::vector<UINT32> LeagueEliNoticeTime;				// ��̭��ÿ�չ���
	///////////////////////////////////////////////////////////////////////////////////

	///> Ӣ��ս��
	std::vector<UINT32> HeroBattleLootRate;
	UINT32 HeroBattleReviveTime;
	UINT32 HeroBattleOverTime;
	float HeroBattleSecond2OnePoint;
	UINT32 HeroBattleTimeLimitIfNoOccupant;
	UINT32 HeroBattleChooseHeroTime;
	std::vector<UINT32> HeroBattleReward;
	std::vector<Sequence<UINT32,2>> HeroBattleSpecialTime;
	UINT32 HeroBattleSpecialNum;
	std::vector<UINT32> HeroBattleSpecialReward;
	UINT32 HeroBattleFreeHeroCount;
	Sequence<UINT32, 3> HeroBattleBigReward;
	std::vector<UINT32> HeroBattleBigRewardRate;
	UINT32 HeroBattleMatchWorldTime;
	UINT32 HeroBattleELOK;
	UINT32 HeroBattleOriginELOPoint;
	std::vector<UINT32> HeroBattleELOTeam;
	UINT32 HeroBattleCanChooseSame;
	///////////////////////////////////////////////////////////////////////////////////
	UINT32 ReportHeroBattleMvpDelNum;		// mvp�ٱ��������͸���
	float ReportHeroBattleInvalidKda;		// ���ִ��ڶ��پٱ���Ч
	UINT32 ReportHeroBattleLeastNum;		// ʧ�ܷ���Ҿٱ��ٱ�����������͵���Ҿٱ����ӵĸ���
	UINT32 ReportHeroCampTotalKillNum;		// ������ɱ�����ڵ��ڶ��٣����Լ���ɱ��������Ϊ0�����
	UINT32 ReportHeroCampTotalAddNum;		// ������ɱ�����ڵ��ڶ��٣����Լ���ɱ��������Ϊ0�����, �ٱ��������ӵĸ���

	///> Ӣ��Ͽ��
	UINT32 MobaBattleChooseHeroTime;
	float MobaDeadExpRange;
	double MobaExpPer3Seconds;
	std::vector<Sequence<UINT32, 2>> MobaUpgradeAttack;
	std::vector<Sequence<UINT32, 2>> MobaUpgradeDefense;
	Sequence<UINT32, 2> MobaSignaTimelLimit;
	UINT32 MobaGameRecordMaxNum;
	UINT32 EscapeTimeRate;
	UINT32 EscapeMinTime;
	UINT32 MobaEnemyId;
	std::vector<UINT32> MobaBattleReward;
	///////////////////////////////////////////////////////////////////////////////////
	///> ս��ϵͳ
	UINT32 PartnerNum;
	UINT32 PartnerNeedFriendDegree;
	UINT32 PartnerNeedDragonCoin;
	UINT32 PartnerLeaveTime;
	UINT32 PartnerLivenessRecordNum;
	UINT32 PartnerShopRecordNum;
	UINT32 PartnerShopId;

	///> ʦͽ
	UINT32 MentorOpenSealLevel;
	UINT32 MentorNormalCompleteNum;
	UINT32 MentorPerfectCompleteNum;
	UINT32 MentorPunishDay;
	UINT32 MentorPeriodDay;
	UINT32 MentorBreakConfirmTime;
	UINT32 MentorNormalCompleteDay;
	UINT32 MentorForceCompleteDay;
	///////////////////////////////////////////////////////////////////////////////////

	///> �˿�֮����������
	std::vector<Sequence<UINT32, 2> > PokerTournamentSignUpCost; 
	///> �˿�֮��ÿ������
	UINT32 PokerTournamentPlayNum;
	///> �˿�֮����������
	UINT32 PokerTournamentSignUpNum;
	///> �˿�֮������	
	UINT32	PokerTournamentWaiting;
	UINT32	PokerTournamentRoundWaiting;
	UINT32	PokerTournamentRounding;
	UINT32	PokerTournamentRound;
	UINT32	PokerTournamentChange;
	UINT32	PokerTournamentChangeAdd;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 HorseWaitSceneID;
	UINT32 HorseSceneID;
	UINT32 HorseLevelLimit;
	UINT32 HorseWaitTime;
	UINT32 HorseGroupCount;
	UINT32 HorseStartAnimTime;
	UINT32 HorseFinalWall;
	UINT32 HorseCollisionWallTime;
	UINT32 HorseTimeRefresh;
	UINT32 HorseLaps;
	UINT32 HorseID;
	UINT32 HorsePlayerNum;
	UINT32 HorseWholeTime;
	///////////////////////////////////////////////////////////////////////////////////

	// ��԰����
	UINT32 PlantSpriteCheckTimes;
	// С��ÿ�ճ��ִ���
	UINT32 PlantSpriteMaxTimes;
	// С����ȴʱ��
	UINT32 PlantSpriteCD;
	// С������ʱ���
	vector<UINT32>    PlantSpriteAppearTimeSection;
	// ��԰��־�����
	UINT32 GardenLogMax;
	// ˢ��ʱ��
	UINT32 RefreshGardenTime;
	// ����������
	UINT32 GardenBanquetGuests;
	// ÿ����ȡ��ά���Ĵ���
	UINT32 GardenBanquetAwardTimesTSWK;
	// ��԰Ĭ�����
	UINT32 GardeLandDefault;
	// ֲ��ɺ�״̬�µ�buffid
	UINT32 PlantGrowDroughtBuff;
	// ֲ��溦״̬�µ�buff
	UINT32 	PlantGrowPestBuff;
	// ֲ��ή��״̬�µ�buff
	UINT32 	PlantGrowSluggish;
	// ֲ���������buff 
	UINT32 	PlantGrowNormalBuff;
	// ֲ��������buff
	UINT32 	PlantGrowMatureBuff;
	vector<Sequence<UINT32, 2>> BreakFarmlandLevel;
	Sequence<float, 4>  HomeFeastDes1;
	Sequence<float, 4>	HomeFeastDes2;
	Sequence<float, 4>	HomeFeastDes3;
	Sequence<float, 4>	HomeFeastDes4;
	Sequence<float, 4>  HomeFeastEndDes1;
	Sequence<float, 4>	HomeFeastEndDes2;
	Sequence<float, 4>	HomeFeastEndDes3;
	Sequence<float, 4>	HomeFeastEndDes4;
	// ����λ��
	vector<Sequence<UINT32, 2>> BreakNewFarmlandCost;
	float FishingCameraDesdir;
	// ����λ��
	vector<Sequence<float, 3>> PlantSeedPosition;
	vector<Sequence<float, 3>> GuildPlantSeedPosition;
	// �����԰Ĭ�����
	vector<Sequence<UINT32, 2>> BreakFarmlandLevel_Guild;
	// ÿ��͵���ܵĴ���
	UINT32 GardenStealCount;
	// ��ֲȡ���Ƿ񷵻�����
	UINT32 PlantReturn;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 GuildTerritoryOpenWeek;
	UINT32 GuildTerritoryRoleLvl;
	UINT32 GuildTerritoryGuildLvl;
	UINT32 GuildTerritoryReadyTime;
	Sequence<UINT32, 2> GuildTerritoryCost;
	std::map<UINT32, Sequence<UINT32, 3> > GuildTerritoryOpenTime;
	std::vector<Sequence<UINT32, 3> > GuildTerritoryDecTime;
	std::vector<Sequence<UINT32, 2> > GuildTerritoryAucTime;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 GuildInheritExp;
	UINT32 GuildInheritTime;
	UINT32 GuildInheritTimes;
	UINT32 GuildInheritCon;
	UINT32 GuildInheritFirend;
	UINT32 GuildInheritCDTime;
	UINT32 GuildRecruitTime;

	UINT32 GuildInheritTeaLimit;
	UINT32 GuildInheritStuLimit;
	Sequence<float, 4> InheritPos;
	Sequence<float, 4> TeacherInheritPos;
	UINT32 TeacherInheritTime;

	UINT32 GuildInheritRoleLvlLow;
	UINT32 GuildInheritRoleLvlHig;
	std::vector<Sequence<UINT32, 2> > GuildInheritRoleLvlGap;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 WatchCommendTimeSpan;
	UINT32 WatchCommendTotalCount;
	UINT32 WatchPVPScore;
	///////////////////////////////////////////////////////////////////////////////////

	// sprite
	UINT32 SpriteEvolutionMinLevel;
	UINT32 SpriteAwakeMinLevel;
	UINT32 SpriteAwakeItemID;
	vector<UINT32> SpriteAwakeConsume;
	vector<UINT32> SpritePositionLevel;
	vector<Sequence<UINT32,2>> SpriteFoodExp;
	double SpriteExpRatio;
	std::vector<Sequence<UINT32, 4>> SpriteTrainCost;
	UINT32 SpriteTrainNoToChooseMaxNum;
	Sequence<UINT32,2> SpriteDrawGoodRange;
	Sequence<UINT32,2> SpriteGoldDrawGoodRange;
	UINT32 SpriteMaxNum;
	Sequence<UINT32,2> SpriteMinGuaranteeCount;
	std::vector<Sequence<UINT32, 2>> SpriteZero;

	// sprite draw
	Sequence<UINT32, 2> SpriteDrawCost;
	Sequence<UINT32, 2> SpriteGoldDrawCost;

	// sprite shop
	Sequence<UINT32, 2> SpriteShopGoldOneCost;
	Sequence<UINT32, 2> SpriteShopGoldTenCost;
	Sequence<UINT32, 2> SpriteShopOneCost;
	Sequence<UINT32, 2> SpriteShopTenCost;
	std::vector<Sequence<UINT32, 2>> SpriteShopGoldOneGain;
	std::vector<Sequence<UINT32, 2>> SpriteShopGoldOneGift;
	std::vector<Sequence<UINT32, 2>> SpriteShopGoldTenGain;
	std::vector<Sequence<UINT32, 2>> SpriteShopGoldTenGift;
	std::vector<Sequence<UINT32, 2>> SpriteShopOneGain;
	std::vector<Sequence<UINT32, 2>> SpriteShopOneGift;
	std::vector<Sequence<UINT32, 2>> SpriteShopTenGain;
	std::vector<Sequence<UINT32, 2>> SpriteShopTenGift;

	///////////////////////////////////////////////////////////////////////////////////

	UINT32 InvFightTimeOver;
	UINT32 InvFightFromCount;
	UINT32 InvFightFightTime;
	UINT32 InvFightRoleLevel;
	UINT32 InvFightMapID;
	///////////////////////////////////////////////////////////////////////////////////

	std::map<UINT32,UINT32> NpcFeelingSeal2Level;
	UINT32 NpcFeelingGiveCount;
	UINT32 NpcFeelingGiveLeftAddLimit;
	UINT32 NpcFeelingTopCount;
	UINT32 NpcFeelingExchangePro;
	UINT32 NpcFeelingExchangeCount;
	UINT32 NpcFeelingReturnPro;
	UINT32 NpcFeelingReturnCount;
	UINT32 NpcFeelingReturnDelay;
	UINT32 NpcFeelingAddExtraExpRange;
	UINT32 NpcFeelingAddExtraExpWeight;
	UINT32 NpcFeelingBackFlowWeight;
	UINT32 NpcFeelingSmallBackFlowWeight;
	UINT32 NpcFeelingBuyCount;
	std::vector<UINT32> NpcFeelingBuyCost;
	UINT32 NpcFeelingTriggerFavorCount;
	UINT32 NpcFeelingTriggerFavorPro;

	///////////////////////////////////////////////////////////////////////////////////

	UINT32 SurviveFightRoleCount;
	UINT32 SurviveFightMapID;
	UINT32 SurviveReadyTime;
	//UINT32 SurviveFightTime;
	std::vector<UINT32> SurviveAddPoint;	
	UINT32 SurviveRewardTopRank;
	UINT32 SurviveRewardTopCount;
	std::vector<Sequence<UINT32,2>> SurviveRewardItems;
	///////////////////////////////////////////////////////////////////////////////////

	///> PVP �����ӳ�
	std::vector<Sequence<UINT32,2>> PVPDayRewards;
	std::vector<Sequence<UINT32,2>> PVPWeekRewards;
	std::vector<Sequence<UINT32,2>> PVPWinRewards;
	Sequence<UINT32,3> PVPMatchScore;
	Sequence<int, 2> PVPLeaderBuff;
	UINT32 PVPQuitFailQuitCount;
	UINT32 PVPRoleNum;
	int PVPGameCount;
	UINT32 PVPGameTimeDown;
	UINT32 PVPGameTimeConti;
	int PVPWeekReqCount;
	int PVPDayReqCount;
	UINT32 PVPMatchWaitTime;
	UINT32 PVPDieReviveTime;
	int PVPLeaderKillCount;
	UINT32 PVPMatchWorldTime;
	UINT32 PVPLoadWaitTime;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 GMFOneFightTime;
	UINT32 GMFFightAfterTime;
	Sequence<int, 2> GMFBuff;
	UINT32 GMFKickCoolDown;
	UINT32 GMFReadyTime;
	UINT32 GMFInspireCoolDown;
	UINT32 GPROneFightTime;
	UINT32 GPRFightAfterTime;
	UINT32 GPRReviveTime;
	std::unordered_set<INT32> GMFInspireCountConf;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 ArenaStarSaveDBLimit;
	UINT32 ArenaStarRankLimit;
	std::vector<Sequence<int,3>> ArenaStarReward;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 RiskOpenAddDiceNum;
	UINT32 RiskRecoverDiceMaxNum;
	UINT32 RiskDiceMaxNum;
	Sequence<UINT32, 2> RiskRecoverDiceNum;
	std::vector<Sequence<UINT32, 2>> RiskBoxTime;
	std::vector<Sequence<UINT32, 4>> RiskBoxAccelerate;
	Sequence<UINT32, 2> RiskReBattleCost;
	///////////////////////////////////////////////////////////////////////////////////

	int DEGroupRoleCount;
	std::set<UINT32> DERankRewardDay;
	Sequence<UINT32, 2> DETongGuanReward;
	///////////////////////////////////////////////////////////////////////////////////

	std::vector<UINT32> BossRushCost;
	///////////////////////////////////////////////////////////////////////////////////

	UINT32 ManualRollTime;
	UINT32 HardDragonDropDiamondBox;
	vector<UINT32> DragonResetWeekDay;
	vector<UINT32> HardDragonResetWeekDay;
	vector<UINT32> SmallDragonResetWeekDay;
	vector<Sequence<int, 2>> TeamTowerSceneID;
	vector<Sequence<UINT32,3> > SweepTowerCost;
	vector<Sequence<UINT32,2> > SweepTowerTime;
	vector<Sequence<UINT32,2> > RefreshSweepCost;
	vector<Sequence<UINT32,2> > RefreshSweepRandValue;
	vector<Sequence<UINT32, 2>> GoddessTrialRewards;

	std::string StopMark;
	int ExpFindBackNeedLevel;
	int ExpFindBackParam;
	int ItemFindBackNeedLevel;

	int BattleAssistLevel;
	UINT32 LevelSealFragmentID;
	UINT32 OpenIBShopRedPointLv;
	UINT32 IBShopGiftTextLen;
	UINT32 IBShopHistMaxSize;
	UINT32 IBShopLevel;
	UINT32 IBShopDegree;
	UINT32 IBHistPay;
	UINT32 IBDayMaxBuyCount;
	UINT32 IBEveryTimeMaxBuy;
	UINT32 IBOverMaxUnRecv;
	UINT32 WorldLevelCount; // ����ȼ�ǰ����
	UINT32 DailyTaskAskHelpNum;		// �ճ����������������
	UINT32 DailyTaskDonateNum;		// �����״���
	UINT32 StageCatchUpExpMinLevel;
	UINT32 GragonCoinExchangeGold;
	vector<UINT32> ActivityChestExp; 
	vector<UINT32> ActivityChestExpWeekly;
	Sequence<UINT32, 3> RefreshIBShopTime;

	UINT32 AudioMsgMax;
	UINT32 GuildPrestigeDle;
	UINT32 GuildWageKeepDay;
	std::map<UINT32, UINT32> GuildCampDayID; //����Ӫ�ػid 
	Sequence<UINT32, 2> GivePlatFriendGift;
	std::string GivePlatFriendGiftMailTitle;
	std::string GivePlatFriendGiftMailContent;

	UINT32 PandoraPushDay;									// �˶������ͼ��
	UINT32 LaddyPushDay;									// �����������ͼ��
	std::vector<Sequence<UINT32,2>> QuestionnaireLevel;		// �����ʾ�
	std::vector<UINT32> QuestionnaireVersion;				// �汾��

	UINT32 RoleDeclarationLenMax;							// ����������󳤶�
	std::vector<UINT32>	PersonalCareerPVP;					// pvp ϵͳID
	std::unordered_set<UINT32> CareerLevelFilter;			// ������Ҫ��ʾ�ĵȼ�
	std::unordered_set<UINT32> TrophySceneList;				// ��ý����ĳ���ID
	// ���ñ���
	UINT32 ShareGiftProb;				// ÿ�ֻ�ñ������
	UINT32 ShareGiftCountDay;			// ÿ����������
	///> �ع�
	UINT32 BackFlowNoLoginDayTime;
	UINT32 BackFlowActivityDayTime;
	UINT32 BackFlowStartAgainMinDayTime;

	std::vector<Sequence<UINT32, 3>> BackFlowTreasure;
	UINT32 BackFlowPayAlieenCount;
	UINT32 BackFlowShopGoodCount;
	UINT32 BackFlowShopFreshCount;
	UINT32 BackFlowShopDuration;
	UINT32 BackFlowJudgeDayTime;
	UINT32 BackFlowJudgeMaxDayTime;
	UINT32 BackFlowActivitySceneID;
	UINT32 BackFlowNestFinishCountLimit;

	std::vector<Sequence<UINT32,3>> MilitaryRank;			// ���а�����������ľ���
	UINT32 TopRankMilitaryExploit;							// ������ͷ����ʼ����
	UINT32 MilitaryRankShowRange;							// ���ͻ�����ʾ���������
	UINT32 MilitarySameOpponentRound;						// ����ϵͳ���پ���������ͬ���ֲ����ӷ֣���ˢ���ƣ�

	//΢��С����
	std::string GuildCampTaskDesc;//�����ð����������;
	UINT32 GuildSignWeekCount;//����ǩ���ܴ���;

	//�һ�ȯ
	UINT32 FindBackTicketExchangeDragon;

	Sequence<UINT32, 2> RenameItemCost;		///> ��������
	Sequence<UINT32, 2> ReGuildNameItemCost; ///> �Ĺ���������
	Sequence<UINT32, 2> ReDragonGuileNameItemCost;	///> С�ֶӸ�������

	///> �����ף��
	UINT32 ArgentaLastTime;
	UINT32 ArgentaWithinDays;
	UINT32 ArgentaActivityPoint;
	UINT32 ArgentaNestCount;
	UINT32 ArgentaEndToStartLimit;
	UINT32 PveGeraldGiftOpen;
	UINT32 PveGeraldGiftAssistRequire;
	UINT32 PveGeraldGiftAssistRewardOpen;
	std::vector<Sequence<UINT32,5>>  PveGeraldGiftReward;
	Sequence<UINT32,2> TarjaItem;
	UINT32 TarjaTime;
	UINT32 TarjaUseItem;
	UINT32 TarjaSwitch;

	UINT32 LevelMailLevel;
	UINT32 LevelMailID;	

	// ����Ӫ���ɶ�
	std::vector<Sequence<float,4>> GuildCampPartyNPC;
	float RandomItemDistance;
	std::vector<Sequence<UINT32,2>> GuildCampPartySpriteTimeStap;

	std::unordered_map<UINT32,Sequence<float,5>> GuildCampRandomEnemy;
	Sequence<UINT32,2> GuildCampPartyNPCDropID;

	std::unordered_map<UINT32,Sequence<float,4>>  GuildCampPartyBonus;
	UINT32 GuildCampPartyContinuedTime;
	UINT32 GuildCampPartyPreNoticeTime;
	UINT32 GuildCampPartySpriteExistTime;
	UINT32 GuildCampPartyCountDownNoticeTime;
	float GuildCampRandomEnemyFrequency;
	UINT32 GuildCampPartyContinuedTime2;
	UINT32 GuildCollecLotteryWaitTime;
	std::vector<Sequence<UINT32, 2>> GuildCampSummonSpiritCost;

	UINT32 LoginActivityOpenDay;
	UINT32 LoginActivityLevel;

	UINT32 GrowthFundBuyDays;									//������x��ر�
	UINT32 GrowthFundExpireMailDays;							//
	UINT32 GrowthFundExpireNoticeDays;
	std::vector<UINT32> GrowthFundExpireNoticeTime;				//


	std::map<UINT32, float> _provoke_skill_ids;
	std::vector<UINT32> _send_msg_skill_ids;
	float _AINavGap;
	///> ����
	std::vector<UINT32> ArtifactLevels;
	std::vector<Sequence<UINT32, 2>> RecastAttrRate;

	///> ȫ��PK
	bool CustomBattleOpen;
	int CustomBattleRankLimit;

	int CustomBattleRandomCount;
	int CustomBattleRandomMax;
	int CustomBattleReqCD;
	int CustomBattleCreateCD;

	int CustomBattleNameLimit1;
	int CustomBattleNameMin;
	int CustomBattleExpireTime;
	int CustomBattleMatchExpireTime;
	int CustomBattleMatchExpandTime;
	int CustomBattleMatchExpandMax;
	bool IsOpenCustomBattleCross;
	bool IsCustomBattleOnlyMatchCross;

	int CustomBattleWorldUpdate;
	int CustomBattleWorldUpdateLimit;

	int CustomBattleMsUpdate;
	int CustomBattleMsUpdateLimit;
	int CustomBattleMsRoleUpdate;
	int CustomBattleMsRoleUpdateLimit;

	int CustomBattleMatcherUpdate;
	int CustomBattleMatcherLimit;
	int CustomBattleMatcherSystemLimit;

	int CustomBattleMatcherSystemTime;


	int CustomBattleMatcherCount;
	int CustomBattleMatcherCD;
	int CustomBattleMatcherPoint;

	bool CustomBattleSystemJoin;
	bool CustomBattleCreate;

	int TransOtherItemId;
	int TransOtherItemIdFirst;
	bool IsTransOpen;
	std::set<UINT32> TransSceneType;

	std::vector<Sequence<UINT32, 2>> CanLevelTransSkill;

	UINT32 ChannelMaskDays;

	///> ��ɱͨ������
	UINT32 AssistDamageTime;			///> �˺������������Чʱ��
	std::vector<UINT32> MultiKillTime;	///> ��ɱʱ����

	std::string SkillEmblemChangeProfMailTitle;			///> ��������תְ�ʼ�����
	std::string SkillEmblemChangeProfMailContent;		///> ��������תְ�ʼ�����

	std::unordered_set<UINT32> SkillEmblemDisableSceneTypes;
	std::unordered_set<UINT32> ArtifactDisableSceneTypes;
	std::vector<Sequence<UINT32, 5>> HeroBattleBanTime; ///> ���ٱ�����-��Ӧʱ��

	UINT32 PVEMorphLevelSealType;


	UINT32 WeeklyTaskRefreshCount;
	std::vector<UINT32> WeeklyTaskRefreshCost;
	std::map<UINT32, UINT32> WeeklyTaskRefreshProb;
	UINT32 WeeklyTaskAskHelpNum;
	UINT32 WeeklyTaskDonateNum;		// �����״���
	UINT32 WeeklyTaskHighQualityNum;	// ˢ�¼��αس���ɫ����
	UINT32 WeeklyTaskHighQuality;
	UINT32 WeekyFreeRefreshNum;
	UINT32 GetWeeklyTaskRefreshCost(UINT32 count);
	UINT32 RandomWeeklyTaskQuality();

	UINT32 DailyTaskMaxRefreshTaskTimes;
	UINT32 DailyTaskMaxBuyRefreshCount;
	std::vector<UINT32> DailyTaskMaxBuyRefreshCost;
	UINT32 DailyTaskMaxHelpCount;
	UINT32 GetDailyTaskRefreshCost(UINT32 count);


	UINT32 NormalWeddingSceneId;
	UINT32 LuxuryWeddingSceneId;
	UINT32 NormalWeddingInviteNum;
	UINT32 LuxuryWeddingInviteNum;
	UINT32 WeddingPrepareTime;
	UINT32 WeddingRunningTime;

	UINT32 WeddingHappynessInterval;
	UINT32 WeddingHappynessOneTime;
	UINT32 WeddingMaxHappyness;
	UINT32 WeddingFlowCD;
	UINT32 WeddingFireworksCD;
	UINT32 WeddingFlowerAddHp;
	UINT32 WeddingFireworksAddHp;
	Sequence<UINT32, 2> WeddingFlowerCost;
	Sequence<UINT32, 2> WeddingFireworksCost;
	std::vector<Sequence<UINT32, 2>> WeddingFlowerReward;
	std::vector<Sequence<UINT32, 2>> WeddingFireworksReward;
	UINT32 WeddingFlowerDailyMax;
	UINT32 WeddingFireworksDailyMax;
	UINT32 WeddingDoodadDailyMax;
	std::string WeddingPatrolAITree;
	std::vector<Sequence<float, 4>> WeddingPatrolPath;
	UINT32 WeddingPatrolTime;
	UINT32 NormalWeddingPetId;
	UINT32 LuxuryWeddingPetId;
	Sequence<float, 4> WeddingVowsHusBandPos;
	Sequence<float, 4> WeddingVowsWifePos;
	Sequence<float, 4> WeddingCarHusBandPos;
	Sequence<float, 4> WeddingCarWifePos;
	std::vector<UINT32> NormalWeddingNpcIDs;
	std::vector<UINT32> LuxuryWeddingNpcIDs;
	std::string WeddingCutscenePath;
	std::string WeddingCarCutScene;

	std::string WeddingWife;
	std::string WeddingHusband;

	UINT32 CombineServerLevel;
	std::vector<Sequence<UINT32, 2>> CombineServerReward;
	UINT32 CombineServerDayLimit;
	float CombineServerExpRate;


	// ƽ̨����
	std::unordered_set<UINT32> FirstPassShareSceneList;
	std::vector<Sequence<UINT32,2>>	FirstPassShareReward;
	UINT32	WeekShareNum;
	std::vector<Sequence<UINT32,2>>	WeekShareReward;

	std::vector<Sequence<UINT32,2>>	FlowerActivityReward;

	UINT32 BigMeleeReadyScene;
	std::vector<UINT32> BigMeleeFightScene;
	UINT32 BigMeleeRestTime;
	UINT32 BigMeleeFightTime;
	UINT32 BigMeleeGroupOne;
	UINT32 BigMeleeBigGroup;
	UINT32 BigMeleeUseGroup;
	UINT32 BigMeleeMaxGames;
	UINT32 BigMeleeSvrGroup;
	UINT32 BigMeleeAnimTime;
	UINT32 BigMeleeReviveTime;
	UINT32 BigMeleeReqRankTime;
	UINT32 BigMeleeSyncTime;
	UINT32 BigMeleePickUpItemPoint;
	std::vector<UINT32> BigMeleePreHourNoticeList;
	std::vector<Sequence<float, 3>> BigMeleeResetPos;
	std::vector<Sequence<UINT32, 3>> BigMeleeKillerLvPoint;
	std::vector<UINT32> BigMeleeJoinAward;
	UINT32 BigMeleeJoinTime;
	UINT32 BigMeleeNeedPlayLevel;
	UINT32 BigMeleeDestoryDay;
	UINT32 BigMeleeSysSwitch;
	UINT32 BigMeleeCutScenes;
	UINT32 BigMeleeMSCutSceneRoles;

	UINT32 AllDisconnectKickTime;

	Sequence<UINT32, 2> DuckTickets;
	std::vector<UINT32> ThemeActivityScene;
	Sequence<UINT32, 2> DayDuckTickets;
	UINT32 BioHellRankId;
	std::vector<UINT32> BioHellSceneStage;

	Sequence<UINT32, 2> MarriageCost;
	Sequence<UINT32, 2> MarriageSplendidCost;
	Sequence<UINT32, 2> MarriageDivorceCost;
	UINT32 MarriageFriendDegreeNeed;
	UINT32 MarriageFreeDivorceDay;

	UINT32 ApplyMarriageDistance;
	UINT32 MarriageRing;
	UINT32 MarriageSplendidRing;
	Sequence<UINT32, 2> NormalMarriageBuff;
	Sequence<UINT32, 2> SplendidMarriageBuff;

	std::vector<Sequence<UINT32, 2>> BattleFieldBuff;
	std::vector<Sequence<UINT32, 2>> BattleFieldLevel; //45=54|55=64|65=74|75=84|85=94|95=104
	UINT32 BattleFieldMaxPlayer;//ÿ��8���
	UINT32 BattleFieldMatchTime;//ÿ120�����һ��ƥ��
	UINT32 BattleFieldReadyScene;//�ȴ�����
	UINT32 BattleFieldCreateScene;//�������������
	std::vector<UINT32> BattleFieldFightScene;//ս������
	std::vector<UINT32> BattleFieldPreNoticeList;//���ս�������Ԥ������;
	UINT32 BattleFieldCutScene;

	UINT32 DragonGuildWeekTaskCount;
	UINT32 DragonGuildWeekReciveCount;
	UINT32 DragonGuildCreateVipReq;
	UINT32 DragonGuildApplyMax;
	std::string DragonGuildGroupShareTitle;
	std::string DragonGuildGroupShareContent;
	Sequence<UINT32, 2> DragonGuildEnterLimit;
	UINT32 DragonGuildShopID;
	UINT32 DragonGuildShopRecordNum;
	UINT32 DragonGuildRecruitTime;
	std::string	DragonGuildDesignation;
	UINT32 DragonGuildActLeaDay;
	Sequence<UINT32, 3> DragonGuildTaskRefreshStartDate;
	UINT32 DragonGuildTaskRefreshCycleTime;
	UINT32 DragonGuildTaskNeedPartnerCount;
	UINT32 DragonGuildLivenessRecordNum;
	UINT32 DragonGuildSyncDataMaxSize;

	UINT32 BattleFieldFightTime;
	UINT32 BattleFieldAnimTime;
	UINT32 BattleFieldReviveTime;
	UINT32 BattleFieldSyncTime;
	std::vector<Sequence<UINT32, 3>> BattleFieldKillerLvPoint;
	
	UINT32 GuildActRank;

	Sequence<UINT32, 2> PeerBox;

	std::vector<Sequence<UINT32, 3>> LotteryDrawItems;//ת�̻����
	std::map<UINT32, Sequence<UINT32, 2>> LotteryDrawConsume;//ת�̻����Ʒ

	//////////////////������Ϸ����
	std::vector<UINT32> ChildOnceGameRestTimeList;	//δ�����˵���-��Ϣ����ʱ����б���С�������У�����λΪ��
	UINT32	ChildOnceGameForceExitTime;	//δ�����˵���-ǿ������ʱ��㣬��λΪ��
	UINT32	ChildOnceGameForceRestTime;	//δ�����˵���-ǿ��������Ϣʱ������λΪ��
	std::vector<UINT32>	AdultOnceGameRestTimeList;	//�����˵���-��Ϣ����ʱ����б���С�������У�����λΪ��
	UINT32	AdultOnceGameForceExitTime;	//�����˵���-ǿ������ʱ��㣬��λΪ��
	UINT32	AdultOnceGameForceRestTime;	//�����˵���-ǿ��������Ϣʱ������λΪ��
	std::string OnceGameRemindMsg;
	std::string OnceGameForceExitMsg;
	std::string TotalGameRemindMsg;
	std::string TotalGameForceExitMsg;
	std::string LoginRemindMsg;
	std::vector<UINT32> ForceExitSceneType;
	bool HgIsIpen;

	/////////////////
	std::unordered_set<int> TransformBuffsChangeOutlook;
	std::unordered_set<UINT32> PvpBiasSceneTypes;

	UINT32 NewZoneChargeBackTimeStartTime;
	UINT32 NewZoneChargeBackTimeEndTime;
	UINT32 NewZoneChargeBackRewardEndTime;
	std::map<UINT32, UINT32> NewZoneChargeBackRange;
	UINT32 NewZoneOpenTime;
	UINT32 NewZoneChargeBackMax;
	UINT32 NewZoneChargeBackRate;

	bool BriefShop;

	std::map<UINT32, UINT32> DonateItemQualityContribute;
	UINT32 GetDonateByQuality(UINT32 quality);
};

inline GlobalConfig& GetGlobalConfig()
{
	return GlobalConfig::GetInstance();
}

#endif
