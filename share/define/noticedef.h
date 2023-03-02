#ifndef __NOTICEDEF_H__
#define __NOTICEDEF_H__

enum CNoticeType
{
	GuildWarNotice                  = 10001,
	EmblemNotice	                = 10002,
	JadeNotice		                = 10003,
	EnhanceNotice1                  = 10022,
	EnhanceNotice2                  = 10023,
	EnhanceNotice3                  = 10024,
	GuildCardNotice1                = 10005,
	GuildCardNotice2                = 10006,
	FlowerNotice1                   = 10007,
	LotteryNoticeA					= 10008,
	LotteryNoticeS                  = 10009,
	LotteryNoticeL					= 10010,
	FashionRankNotice1              = 10020,
	HellHoundHardNotice             = 10021,
	GuildBonusNotice                = 10025,
	FirstGuildBonusNotice           = 10026,
	LuckestGuildBonusNotice         = 10027,
	FashionTimeoutNotice			= 10028,
	TshowWalkNotice					= 10029,
	TshowTop3SelfNotice				= 10030,
	TshowTop3Notice					= 10031,
	TakeWorldQANotice				= 10032, 
	FlowerNotice_2                  = 10040,
	FlowerNotice_3                  = 10041,
	FlowerNotice_4                  = 10042,
	FlowerNotice_5                  = 10043,

	RollResultTeamNotice			= 10045,
	RollGiveUpSelfNotice			= 10046,
	RollGiveUpTeamNotice			= 10047,
	RollFinalResultSelfNotice		= 10048,
	RollFinalResultTeamNotice		= 10049,
	
	AddFriendNotice					= 10050,

	FirstPassStage                  = 10060,

	SpriteSkillCastSolo				= 10070,
	SpriteSkillCastTeam				= 10071,

	ChangeNameNotice1                = 10072,
	ChangeNameNotice2                = 10073,

	ChangeProNotice1                = 10074,
	ChangeProNotice2                = 10075,

	SendGuildBonusNotice1			= 10080,
	SendGuildBonusNotice2			= 10081,

	FlowerValentinesDayNoticeFirst	= 10090,
	FlowerValentinesDayNoticeOther	= 10091,
	Flower520DayNoticeFirst			= 10092,
	Flower520DayNoticeOther			= 10093,

	GuildCampPartyPre5				= 10094,
	GuildCampPartySysStart			= 10095,
	GuildCampPartyGuildStart		= 10096,
	GuildCampPartySpriteAppear		= 10097,
	GuildCampPartySpriteDisAppear	= 10098,
	GuildCampPartyExchangeItem  	= 10099,
	GuildCampPartyEnd  				= 10100,
	GuildCampPartySummonSpirit		= 10101,

	ReviveSelf						= 10201,
	ReviveOther						= 10202,

	ServerLevelSealNotice           = 20004,
	GuildSystemNotice               = 20005,

	GuildCardBeginNotice			= 20006,
	GuildCardRoundNotice			= 20007,
	GuildCardEndNotice				= 20008,
	GuildCardFirstNotice			= 20009,
	GuildCardBeginSoonNotice		= 20010,
	
	TitleGuildNotice				= 25000,
	TitleFirstGetMax				= 25001,
	TitleNextGetMax					= 25002,

	PvpBeginNotice                  = 30001,
	PvpEndNotice                    = 30002,
	GuildLevelUp                    = 30005,
	GuildStudySkillUp               = 30006,
	GuildDareFinished               = 30007,
	GuildArenaInit                  = 30008,
	GuildArenaReady                 = 30009,
	GuildArenaStart                 = 30010,
	GuildBossRankNotice             = 30011,
	GuildBossFirstKill              = 30012,
	GuildBossLastKill               = 30013,
	GuildBossDps                    = 30014,
	GuildArenaYouTurn               = 30015,
	GuildBossInspire				= 30016,
	GuildArenaKickYou				= 30017,
	FlowerDesignationNotice			= 40001,		///>获得昨日榜鲜花称号
	FlowerDesignationLostNotice	    = 40002,		///>失去昨日榜鲜花称号


	///>50001	距离军团语音派对时间已到，可以开启通知
	///>50002	公会官员已经开启了公会答题通知
	GuildQALeftTime_Arrive			= 50001,
	GuildQALeftTime_QAOpen			= 50002,
	TakeGuildQANotice						= 50003, 

	WorldBossFirst					= 60001,
	WorldBossSecond					= 60002,
	WorldBossAll					= 60003,

	WorldBoss25						= 60100,
	WorldBoss50						= 60101,
	WorldBoss75						= 60102,

	WorldBossGuild25				= 60103,
	WorldBossGuild50				= 60104,
	WorldBossGuild75				= 60105,

	WorldBossGuild					= 60106,

	WorldBossLeft_1Min              = 60004,
	WorldBossLeft_2Min              = 60005,
	WorldBossLeft_5Min              = 60006,
	WorldBossLeft_10Min             = 60007,

	GuildBossLeft_10Min             = 60008,
	GuildBossLeft_5Min              = 60009,
	GuildBossLeft_2Min              = 60010,
	GuildBossLeft_1Min              = 60011,

	GuildBossLvlup                  = 60012,

	LiveNotice_Nest                 = 100020,
	LiveNotice_Dragon               = 100021,
	LiveNotice_1V1                  = 100022,
	LiveNotice_ProtectCaptain       = 100023,
	LiveNotice_GuildBattle          = 100024,
	LiveNotice_HeroBattle           = 100025,
	LiveNotice_LeagueBattle         = 100026,
	LiveNotice_2V2                  = 100027,
	LiveNotice_CustomPk             = 100028,

	TrophyRankNotice				= 100103,

	GuildRecruit                    = 110011,
	DragonGuileRecruitNotice		= 110031,

	BioHellFirstComplete			= 120001,

	GuildArenaBattleOne             = 200001,
	GuildArenaBattleTwo             = 200002,
	GuildArenaBattleFin             = 200003,
	GuildArenaBattleWin             = 200004,
	GuildArenaBattleLos             = 200005,
	GuildArenaBattleCom             = 200006,
	GuildArenaWiner                 = 200007,
	GuildArenaWinerTwo              = 200008,
	GuildArenaWinerThree            = 200009,
	GuildArenaBefore3               = 200010,
	GuildArenaBefore2               = 200011,
	GuildArenaBefore1               = 200012,
	GuildArenaPushOn                = 200013,
	GuildArenaPushOut               = 200014,
	GuildActivityLeader             = 200015,
	GuildArenaApply                 = 200016,
	GuildArenaIntegralReady         = 200017,
	GuildArenaEnterScene            = 200018,
	GuildArenaIntegralWin           = 200019,
	GuildArenaIntegralLos           = 200020,
	GuildArenaInspire               = 200025,
	GuildIntegralApplyEnd           = 200026,
	GuildIntegralMiss               = 200027,
	GuildLadderReady                = 200100,
	GuildLadderBegin                = 200101,
	GuildLadderWin                  = 200102,
	GuildLadderWinTimes             = 200103,
	GuildLadderTen                  = 200104,
	GuildLadderFive                 = 200105,
	GuildLadderEnd                  = 200106,
	GuildArenaEnterSceneOne         = 200107,
	GuildArenaEnterSceneTwo         = 200108,
	GuildArenaEnterSceneThree       = 200109,
	GuildArenaKickNotice            = 200110,
	GuildArenaOneFightWin           = 200111,
	GuildArenaOneFightLose          = 200112,
	GuildRankEnd                    = 200150,
	GuildRankReward                 = 200151,
	SkyCityOpen_1MIN				= 200152,				
	SkyCityOpen_2MIN				= 200153,				
	SkyCityOpen_3MIN				= 200154,		
	SkyCityOpen_7Floor				= 200155,		
	SkyCityOpen_MatchFail			= 200156,		
	ApolloVice_Notice				= 200157,
	GuildAuctBuyNow                 = 200160,
	GuildAddMember                  = 200161,
	GuildAuctActEnd                 = 200162,
	GuildResWarEnemyUnKnown			= 200163,
	GuildResWarStealGuildRes		= 200164,
	GuildResWarAddEnemyTime			= 200165,
	GuildResWarAddSelfRes			= 200166,
	GuildResWarClearDeBuff			= 200167,
	GuildResWarSubSelfTime			= 200168,
	GuildResWarAddResValue			= 200169,
	GuildResWarSignNotice			= 200170,
	GuildResWarStart				= 200171,
	GuildResWarSignAward			= 200173,
	GPROneGameWin                   = 200174,
	GPROneGameLose                  = 200175,
	GPRAllGameWin                   = 200176,
	GPRAllGameLose                  = 200177,
	SkyCityWinGuild					= 200191,
	GuildResWarEnemyUnKnownChanel	= 200192,
	GuildResWarStealGuildResChanel  = 200193,
	GuildResWarAddEnemyTimeChanel	= 200194,
	GuildResWarAddSelfResChanel		= 200195,
	GuildResWarClearDeBuffChanel	= 200196,
	GuildResWarSubSelfTimeChanel	= 200197,
	GuildResWarCard					= 200199,
	GuildJoinLastBattle				= 200201,
	GuildInheritEnd                 = 200202,
	GuildInheritReqEnd              = 200203,
	GuildInheritReqerEnd            = 200204,
	ResWarOpen5Min					= 200205,
	ResWarFinal5Min					= 200206,
	PokerTournamentBeginNotice		= 200207,
	PokerTournamentRoundNotice		= 200208,
	PokerTournamentEndNotice		= 200209,
	PokerTournamentFirstNotice		= 200210,
	PokerTournamentBeginSoonNotice	= 200211,
	PandoraNoticeA					= 200212,
	PandoraNoticeS					= 200213,
	PandoraNoticeL					= 200214,
	GuildTerrBegin                  = 200215,
	GuildTerrWarning                = 200216,
	GuildTerrDeclareOne             = 200217,
	GuildTerrDeclareTwo             = 200218,
	GuildTerrDeclareThree           = 200219,
	GuildTerrCanDeclareOne          = 200220,
	GuildTerrCanDeclareTwo          = 200221,
	GuildTerrCanDeclareThree        = 200222,
	GuildTerrWarningDeclare         = 200223,
	GuildTerrWarningDeclareTwo      = 200224,
	GuildTerrWarningNoDeclare       = 200225,
	GuildTerrNotDeclareOne          = 200226,
	GuildTerrNotDeclareTwo          = 200227,
	GuildTerrNotDeclareThree        = 200228,
	GuildTerrEnterSceneOne          = 200229,
	GuildTerrEnterSceneTwo          = 200230,
	GuildTerrEnterSceneThree        = 200231,
	GuildTerrEnterScene             = 200232,
	GuildTerrEnd                    = 200234,
	GuildTerrReward                 = 200235,
	GuildTerrRewardOne              = 200236,
	GuildTerrRewardTwo              = 200237,
	GuildTerrRewardThree            = 200238,
	GuildTerrOtherReward            = 200239,
	GuildTerrAllianceOne            = 200246,
	GuildTerrAllianceTwo            = 200247,
	GuildTerrAllianceThree          = 200248,
	GuildTerrDelAlliance            = 200249,
	GuildTerrContinueKill_4         = 200319,
	GuildTerrContinueKill_6			= 200320,
	GuildTerrContinueKill_8			= 200321,
	GuildTerrOccupy_Up				= 200332,
	GuildTerrOccupy_Mid				= 200333,
	GuildTerrOccupy_Down			= 200334,
	PartnerMakeNotice				= 200250,
	PartnerLeaveNotice				= 200251,
	PartnerDissolveNotice			= 200252,
	PartnerOtherLeaveNotice			= 200253,
	PartnerLevelUpNotice			= 200254,
	GuildTerrMulPointOne            = 200240,
	GuildTerrMulPointTwo		    = 200241,
	GuildTerrMulPointThree          = 200242,
	GuildTerrWinGuildOne            = 200243,
	GuildTerrWinGuildTwo            = 200244,
	GuildTerrWinGuildThree			= 200245,
	HorseRaceCountTime				= 200256,
	GuildTerrDefGuildOne			= 200257,
	GuildTerrDefGuildTwo			= 200258,
	GuildTerrDefGuildThree			= 200259,
	TeamKickLeaderBefore            = 200260,
	TeamKickLeaderAfter				= 200261,
	NoticeType_LeagueStartApply		= 200262,
	NoticeType_LeagueEndApply		= 200263,
	NoticeType_LeagueRace			= 200264,
	NoticeType_LeagueRace30Min	    = 200265,
	NoticeType_LeagueRace10Min	    = 200266,
	NoticeType_LeagueRace3Min	    = 200267,
	NoticeType_LeagueRace2Min	    = 200268,
	NoticeType_LeagueRace1Min	    = 200269,
	NoticeType_LeagueRaceStart	    = 200270,
	NoticeType_LeagueRaceEnd	    = 200271,
	NoticeType_LeaguePreEle			= 200272,
	NoticeType_LeagueEli			= 200273,
	NoticeType_LeagueFirstEli30Min  = 200274,
	NoticeType_LeagueFirstEli10Min  = 200275,
	NoticeType_LeagueFirstEli3Min   = 200276,
	NoticeType_LeagueFirstEli2Min   = 200277,
	NoticeType_LeagueFirstEli1Min   = 200278,
	NoticeType_LeagueFirstEliStart  = 200279,
	NoticeType_LeagueSecondEli3Min  = 200280,
	NoticeType_LeagueSecondEli2Min  = 200281,
	NoticeType_LeagueSecondEli1Min  = 200282,
	NoticeType_LeagueSecondEliStart = 200283,
	NoticeType_LeagueThirdEli3Min   = 200284,
	NoticeType_LeagueThirdEli2Min   = 200285,
	NoticeType_LeagueThirdEli1Min   = 200286,
	NoticeType_LeagueThirdEliStart  = 200287,
	NoticeType_LeagueEliChamption   = 200288,
	NoticeType_LeagueEliEnd			= 200289,

	NoticeType_LeagueCrossRace			= 200290,
	NoticeType_LeagueCrossRace30Min		= 200291,
	NoticeType_LeagueCrossRace10Min		= 200292,
	NoticeType_LeagueCrossRace3Min		= 200293,
	NoticeType_LeagueCrossRace2Min	    = 200294,
	NoticeType_LeagueCrossRace1Min	    = 200295,
	NoticeType_LeagueCrossRaceStart	    = 200296,
	NoticeType_LeagueCrossRaceEnd	    = 200297,
	NoticeType_LeagueCrossPreEle		= 200298,

	NoticeType_LeagueCrossEli			 = 200405,
	NoticeType_LeagueFirstCrossEli30Min  = 200299,
	NoticeType_LeagueFirstCrossEli10Min  = 200300,
	NoticeType_LeagueFirstCrossEli3Min   = 200301,
	NoticeType_LeagueFirstCrossEli2Min   = 200302,
	NoticeType_LeagueFirstCrossEli1Min   = 200303,
	NoticeType_LeagueFirstCrossEliStart  = 200304,
	NoticeType_LeagueSecondCrossEli3Min  = 200305,
	NoticeType_LeagueSecondCrossEli2Min  = 200306,
	NoticeType_LeagueSecondCrossEli1Min  = 200307,
	NoticeType_LeagueSecondCrossEliStart = 200308,
	NoticeType_LeagueThirdCrossEli3Min   = 200309,
	NoticeType_LeagueThirdCrossEli2Min   = 200310,
	NoticeType_LeagueThirdCrossEli1Min   = 200311,
	NoticeType_LeagueThirdCrossEliStart  = 200312,
	NoticeType_LeagueCrossEliChamption   = 200313,
	NoticeType_LeagueCrossEliEnd		 = 200314,

	HeroBattleBigRewardNotice		= 200315,
	HeroBattleGameNotice1			= 200400,
	HeroBattleGameNotice2			= 200401,
	HeroBattleGameNotice3			= 200402,
	HeroBattleGameNotice4			= 200403,
	HeroBattleGameNotice5			= 200404,

	NoticeGuildTerrBatWinOne        = 200326,
	NoticeGuildTerrBatWinTwo        = 200327,
	NoticeGuildTerrBatWinThree      = 200328,
	NoticeGuildTerrBatSavOne        = 200323,
	NoticeGuildTerrBatSavTwo        = 200324,
	NoticeGuildTerrBatSavThree      = 200325,
	NoticeGuildTerrWinBOne          = 200329,
	NoticeGuildTerrWinBTwo          = 200330,

	NoticeWeekNestWin				= 200500,

	NoticeGrowthFundNotice			= 200600,
	NoticeMilitaryRankAward			= 200800,

	MayHemHourNotice				= 210000,
	MayHemMinuteNotice				= 210001,

	MayHemApproachtNotice			= 210002,
	MayHemPreStartNotice			= 210003,
	MayHemStartNotice				= 210004,
	MayHemRoundResultNotice			= 210005,
	MayHemEndNotice					= 210006,

	WeddingNpcEnterNotice			= 210015,
	WeddingFireworksNotice			= 210017,
	WeddingFlowerNotice				= 210018,
	WeddingCarNotice				= 210019,
	WeddingNotice					= 210020,

	TransOtherNotice1				= 210050,
	TransOtherNotice2				= 210051,
	TransOtherNotice				= 210052,

	BattleFieldPreNotice			= 210100,
	BattleFieldStartNotice			= 210101,

};

enum NoticeTeamType
{
	LevelUp = 20001,
	ItemGet = 20002,
	GuildIn = 20003,
};

#endif
