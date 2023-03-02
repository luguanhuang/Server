#include "pch.h"
#include "globalconfig.h"
#include "table/GlobalTable.h"
#include "util/XCommon.h"
#include "util/gametime.h"
#include "checkutil.h"

struct GlobalKeyValue
{
	std::string operator[] (const std::string& key)
	{
		auto iter = GetGlobalConfig().m_mapKeyValue.find(key);
		if (iter == GetGlobalConfig().m_mapKeyValue.end())
		{
			LogError("key [%s] not in GlobalConfig.txt", key.c_str());
			CHECK_COND_RETURN(false, "");
		}
		LogInfo("Load GlobalConfig, key [%s]", key.c_str());
		return iter->second;
	}
};

static GlobalKeyValue KeyValue;

template <typename T>
std::vector<T> ConvertToVector(const std::string& s, char sep)
{
	std::vector<T> result;
	std::vector<std::string> vec = Split(s, sep);
	for (unsigned i = 0; i < vec.size(); ++i)
	{
		result.push_back(SafeConvert<T>(vec[i]));
	}

	return result;
}

template <typename T, int N>
Sequence<T, N> ConvertToSequence(const std::string& s, char sep)
{
	Sequence<T, N> result;
	std::vector<std::string> vec = Split(s, sep);
	CHECK_COND_RETURN(vec.size() == N, result);
	for (unsigned i = 0; i < vec.size(); ++i)
	{
		result[i] = SafeConvert<T>(vec[i]);
	}

	return result;
}


bool GlobalConfig::InitFunc()
{
	m_key2reload["Team"] = std::bind(&GlobalConfig::InitTeam, this);
	m_key2reload["Pk"] = std::bind(&GlobalConfig::InitPk, this);
	m_key2reload["PVP"] = std::bind(&GlobalConfig::InitPvp, this);
	m_key2reload["GMF"] = std::bind(&GlobalConfig::InitGmf, this);
	m_key2reload["Castle"] = std::bind(&GlobalConfig::InitCastle, this);
	m_key2reload["InvFight"] = std::bind(&GlobalConfig::InitInvFight, this);
	m_key2reload["DEBossRush"] = std::bind(&GlobalConfig::InitDEBossRush, this);
	m_key2reload["Friend"] = std::bind(&GlobalConfig::InitFriend, this);
	m_key2reload["Mail"] = std::bind(&GlobalConfig::InitMail, this);
	m_key2reload["Arena"] = std::bind(&GlobalConfig::InitArena, this);
	m_key2reload["Combat"] = std::bind(&GlobalConfig::InitCombat, this);
	m_key2reload["Common"] = std::bind(&GlobalConfig::InitCommon, this);
	m_key2reload["Boss"] = std::bind(&GlobalConfig::InitBoss, this);
	m_key2reload["SceneView"] = std::bind(&GlobalConfig::InitSceneView, this);
	m_key2reload["GoldClick"] = std::bind(&GlobalConfig::InitGoldClick, this);
	m_key2reload["Item"] = std::bind(&GlobalConfig::InitItem, this);
	m_key2reload["Auct"] = std::bind(&GlobalConfig::InitAuct, this);
	m_key2reload["GuildAuct"] = std::bind(&GlobalConfig::InitGuildAuct, this);
	m_key2reload["Garden"] = std::bind(&GlobalConfig::InitGarden, this);
	m_key2reload["Poker"] = std::bind(&GlobalConfig::InitPoker, this);
	m_key2reload["Career"] = std::bind(&GlobalConfig::InitPersonalCareer, this);
	m_key2reload["GuildInherit"] = std::bind(&GlobalConfig::InitGuildInherit, this); // 公会传承
	m_key2reload["GuildTerr"] = std::bind(&GlobalConfig::InitGuildTerr, this);  // 公会领地
	m_key2reload["GuildLadder"] = std::bind(&GlobalConfig::InitGuildLadder, this);  // 公会领地
	m_key2reload["Flower"] = std::bind(&GlobalConfig::InitFlower, this);  // 鲜花
	m_key2reload["Mentor"] = std::bind(&GlobalConfig::InitMentor, this);  // 师徒
	m_key2reload["ArenaStar"] = std::bind(&GlobalConfig::InitArenaStar, this); // 竞技名人堂
	m_key2reload["Platform"] = std::bind(&GlobalConfig::InitPlatForm, this);	// 平台相关
	m_key2reload["Partner"] = std::bind(&GlobalConfig::InitPartner, this);
	m_key2reload["AI"] = std::bind(&GlobalConfig::InitAI, this);
	m_key2reload["SkyCraft"] = std::bind(&GlobalConfig::InitSkyCraft, this);
	m_key2reload["CustomBattle"] = std::bind(&GlobalConfig::InitCustomBattle, this);

	m_key2reload["Military"] = std::bind(&GlobalConfig::InitMilitaryRank, this); // 军阶
	m_key2reload["BigMelee"] = std::bind(&GlobalConfig::InitBigMelee, this); 
	m_key2reload["BattleField"] = std::bind(&GlobalConfig::InitBattleField, this); 
	m_key2reload["Task"] = std::bind(&GlobalConfig::InitTask, this);
	m_key2reload["NpcFeeling"] = std::bind(&GlobalConfig::InitNpcFeeling, this);
	m_key2reload["Survive"] = std::bind(&GlobalConfig::InitSurvive, this);
	m_key2reload["HG"]	= std::bind(&GlobalConfig::InitHG, this);
	m_key2reload["LuckyTurntable"] = std::bind(&GlobalConfig::InitLuckyTable, this);

	return true;
}

bool GlobalConfig::Reload(const std::string& param)
{
	std::map<std::string, ReloadGlobalConfigCallBack>::iterator it = m_key2reload.find(param);
	if (it == m_key2reload.end())
	{
		SSWarn<<"can't find param:"<<param<<END;
		return false;
	}

	InitKeyValue();

	return (it->second)();
}

bool GlobalConfig::LoadFile()
{
	InitFunc();

	if(!InitKeyValue())
	{
		return false;
	}

	std::vector<std::string> vec;
	vec.clear();
	InitBigMelee();
	InitTeam();
	if(!InitPk()) return false;
	if(!InitNpcFeeling()) return false;
	if(!InitSurvive()) return false;
	InitPvp();
	InitGmf();
	InitCastle();
	InitInvFight();
	InitCustomBattle();
	InitBattleField();

	if (!InitDEBossRush())
	{
		return false;
	}

	// friend
	InitFriend();
	//////////////////////////////////////////////////////////////////////////////////////////////////
	// arena
	InitArena();
	///////////////////////////////////////////////////////////////////////////////////
	// mail
	InitMail();
	///////////////////////////////////////////////////////////////////////////////////
	// auction
	InitAuct();
	// GuildAuct
	InitGuildAuct();
	// Garden
	InitGarden();
	// Poker
	InitPoker();
	// PersonalCarrer	
	InitPersonalCareer();
	// AI
	InitAI();
	//ArenaStar
	InitArenaStar();
	// military
	InitMilitaryRank();
	// skycraft
	InitSkyCraft();
	//party
	InitGuildCampParty();
	//wedding	
	InitWedding();
	InitTask();

	// 平台分享
	InitPlatFormShare();
	///////////////////////////////////////////////////////////////////////////////////

	// guild boss
	GuildBossScenePlayer = SafeConvert<UINT32>(KeyValue["GuildBossScenePlayer"]); 
	GuildBossJoinMailTitle = KeyValue["GuildBossJoinMailTitle"];
	GuildBossJoinMailContent = KeyValue["GuildBossJoinMailContent"];
	GuildBossFinishMailTitle = KeyValue["GuildBossFinishMailTitle"];
	GuildBossFinishMailContent = KeyValue["GuildBossFinishMailContent"];
	GuildBossFirstkillMailTitle = KeyValue["GuildBossFirstkillMailTitle"];
	GuildBossFirstkillMailContent = KeyValue["GuildBossFirstkillMailContent"];
	GuildBossPresidentMailTitle = KeyValue["GuildBossPresidentMailTitle"];
	GuildBossPresidentMailContent = KeyValue["GuildBossPresidentMailContent"];
	GuildBossOfficialMailTitle = KeyValue["GuildBossOfficialMailTitle"];
	GuildBossOfficialMailContent = KeyValue["GuildBossOfficialMailContent"];
	///////////////////////////////////////////////////////////////////////////////////

	// guild arena
	GuildArenaBattleWinTitle = KeyValue["GuildArenaBattleWinTitle"];
	GuildArenaBattleWinContent = KeyValue["GuildArenaBattleWinContent"];
	GuildArenaBattleLoseTitle = KeyValue["GuildArenaBattleLoseTitle"];
	GuildArenaBattleLoseContent = KeyValue["GuildArenaBattleLoseContent"];
	GuildArenaBattleChampionTitle = KeyValue["GuildArenaBattleChampionTitle"];
	GuildArenaBattleChampionContent = KeyValue["GuildArenaBattleChampionContent"];
	GuildArenaBattleSecondTitle = KeyValue["GuildArenaBattleSecondTitle"];
	GuildArenaBattleSecondContent = KeyValue["GuildArenaBattleSecondContent"];
	GuildArenaBattleChampionLeaderTitle = KeyValue["GuildArenaBattleChampionLeaderTitle"];
	GuildArenaBattleChampionLeaderContent = KeyValue["GuildArenaBattleChampionLeaderContent"];
	GuildArenaBattleSecondLeaderTitle = KeyValue["GuildArenaBattleSecondLeaderTitle"];
	GuildArenaBattleSecondLeaderContent = KeyValue["GuildArenaBattleSecondLeaderContent"];
	GuildArenaBeforeWar = ConvertToSequence<UINT32, 3>(KeyValue["GuildArenaBeforeWar"], '=');
	vec.clear();
	vec = Split(KeyValue["GuildArenaSelectedTime"], '|');
	GuildArenaTimeVec.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaTimeVec.push_back(ConvertToSequence<UINT32, 15>(vec[i], '='));
	}
	GuildArenaDelayTime = SafeConvert<UINT32>(KeyValue["GuildArenaDelayTime"]); 
	GuildArenaOriginal = SafeConvert<UINT32>(KeyValue["Guild_Arena_Original"]);
	GuildArenaSignupLevel = SafeConvert<UINT32>(KeyValue["Guild_Arena_Signup_Level"]);
	vec.clear();
	vec = Split(KeyValue["Guild_Arena_Signup_Time"], '|');
	GuildArenaSignupTime.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaSignupTime.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["Guild_Arena_Single"], '|');
	GuildArenaSingle.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaSingle.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["Guild_Arena_Double"], '|');
	GuildArenaDouble.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaDouble.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	GuildArenaFinal = ConvertToSequence<UINT32, 5>(KeyValue["Guild_Arena_Final"], '=');
	GuildArenaFinalLamp = ConvertToSequence<UINT32, 4>(KeyValue["Guild_Arena_Final_Lamp"], '=');
	GuildArenaFinalNotice = ConvertToSequence<UINT32, 4>(KeyValue["Guild_Arena_Final_Notice"], '=');
	vec.clear();
	vec = Split(KeyValue["Guild_Arena_Signup_Notice"], '|');
	GuildArenaApply.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaApply.push_back(ConvertToSequence<UINT32, 5>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["Guild_Arena_Single_Notice"], '|');
	GuildArenaSingleNotice.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaSingleNotice.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["Guild_Arena_Double_Notice"], '|');
	GuildArenaDoubleNotice.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildArenaDoubleNotice.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	// sky city
	SkyCityFirstHalfTime=SafeConvert<UINT32>(KeyValue["SkyCityFirstHalfTime"]);
	SkyCityHalfTime=SafeConvert<UINT32>(KeyValue["SkyCityHalfTime"]);
	SkyCitySpaceTime=SafeConvert<UINT32>(KeyValue["SkyCitySpaceTime"]);
	SkyCitySecondHalfTime=SafeConvert<UINT32>(KeyValue["SkyCitySecondHalfTime"]);
	SkyCityGames=SafeConvert<UINT32>(KeyValue["SkyCityGames"]);
	SkyCityFloor=SafeConvert<UINT32>(KeyValue["SkyCityFloor"]);
	SkyCityWaitingRoomTime = SafeConvert<UINT32>(KeyValue["SkyCityWaitingRoomTime"]);
	SkyCityChangeSceneTime = SafeConvert<UINT32>(KeyValue["SkyCityChangeSceneTime"]);
	SkyCityEnterLv = SafeConvert<UINT32>(KeyValue["SkyCityEnterLv"]);
	vec.clear();
	vec = Split(KeyValue["SkyCityRestPos"], '|');
	SkyCityRestPos.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		SkyCityRestPos.push_back(SafeConvert<float>(vec[i]));
	}
	if(6 != SkyCityRestPos.size())
	{
		SSError << " SkyCityRestPos config err size = " << SkyCityRestPos.size() << END;
		return false;
	}
	vec.clear();
	vec = Split(KeyValue["SkyCityFightSceneID"], '|');
	SkyCityFightSceneID.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		SkyCityFightSceneID.push_back(SafeConvert<UINT32>(vec[i]));
	}


	SkyCityLastResultDelay = SafeConvert<UINT32>(KeyValue["SkyCityLastResultDelay"]);
	SkyCityLookupId = SafeConvert<UINT32>(KeyValue["SkyCityLookupId"]);
	SkyCity2FloorGroup = SafeConvert<UINT32>(KeyValue["SkyCity2FloorGroup"]);
	SkyArenaStartAnimTime = SafeConvert<int>(KeyValue["SkyArenaStartAnimTime"]);

	///////////////////////////////////////////////////////////////////////////////////

	// res war
	ResWarWaitingTime=SafeConvert<UINT32>(KeyValue["ResWarWaitingTime"]);
	ResWarPVPSceneID=SafeConvert<UINT32>(KeyValue["ResWarPVPSceneID"]);
	ResWarPVESceneID=SafeConvert<UINT32>(KeyValue["ResWarPVESceneID"]);
	ResWarExploreTime=SafeConvert<UINT32>(KeyValue["ResWarExploreTime"]);
	ResWarOpenGroupWeek=SafeConvert<UINT32>(KeyValue["ResWarOpenGroupWeek"]);
	ResWarPVPTime=SafeConvert<UINT32>(KeyValue["ResWarPVPTime"]);
	ResWarDNEID= SafeConvert<UINT32>(KeyValue["ResWarDNEID"]);
	ResWarFightRate=SafeConvert<float>(KeyValue["ResWarFightRate"]);
	CHECK_COND_RETURN(ResWarFightRate > 0.001f, false);
	ResWarMatchTime=SafeConvert<UINT32>(KeyValue["ResWarMatchTime"]);
	ResWarGuildLv=SafeConvert<UINT32>(KeyValue["ResWarGuildLv"]);
	ResWarNoticeTime=SafeConvert<UINT32>(KeyValue["ResWarNoticeTime"]);
	ResWarPVEMineSize = SafeConvert<UINT32>(KeyValue["ResWarPVEMineSize"]);
	ResWarDestoryTime  = SafeConvert<UINT32>(KeyValue["ResWarDestoryTime"]);
	ResWarLevelLimit   = SafeConvert<UINT32>(KeyValue["ResWarLevelLimit"]);
	ResWarEnemyAttack   =   SafeConvert<UINT32>(KeyValue["ResWarEnemyAttack"]);
	ResWarSwitch = SafeConvert<UINT32>(KeyValue["ResWarSwitch"]);
	ResWarPullGroup = SafeConvert<UINT32>(KeyValue["ResWarPullGroup"]);
	///////////////////////////////////////////////////////////////////////////////////

	// guild
	MaxGetGuildCheckInBonusNum = atoi(KeyValue["MaxGetGuildCheckInBonusNum"].c_str());
	GuildBonusAskTimeSpan = atoi(KeyValue["GuildBonusAskTimeSpan"].c_str());
	GuildBonusNoticeGetAllTime = atoi(KeyValue["GuildBonusNoticeGetAllTime"].c_str());
	vec.clear();
	vec = Split(KeyValue["GuildBonusOnLineNumAddPercent"], "|");
	GuildBonusOnLineNumAddPercent.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildBonusOnLineNumAddPercent.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["GuildCheckInBonusTime"], "|");
	GuildCheckInBonusTime.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildCheckInBonusTime.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	GuildApplyMax = SafeConvert<UINT32>(KeyValue["GuildApplyMax"]);

	LeaveGuildCount = SafeConvert<UINT32>(KeyValue["LeaveGuildCount"]);
	LeaveGuildCD = SafeConvert<UINT32>(KeyValue["LeaveGuildCD"]);

	///////////////////////////////////////////////////////
	InitGuildLadder(); // 公会天梯
	////////////////////////////////////////

	vec.clear();
	vec = Split(KeyValue["GuildLadderWinNotice"], '|');
	GuildLadderWinNotice.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		GuildLadderWinNotice.insert(SafeConvert<UINT32>(vec[i]));
	}

	GuildRankRewardTime = SafeConvert<UINT32>(KeyValue["GuildRankRewardTime"]);
	GuildRankRewardKeepTime = SafeConvert<UINT32>(KeyValue["GuildRankRewardKeepTime"]);
	DelGuildExp = SafeConvert<UINT32>(KeyValue["DelGuildExp"]);
	DelGuildPre = SafeConvert<UINT32>(KeyValue["DelGuildPre"]);
	///////////////////////////////////////////////////////////////////////////////////

	// card
	InitCard();
	///////////////////////////////////////////////////////////////////////////////////

	// combat
	InitCombat();
	///////////////////////////////////////////////////////////////////////////////////

	// common
	InitCommon();
	///////////////////////////////////////////////////////////////////////////////////

	// guild boss/ world boss
	InitBoss();
	///////////////////////////////////////////////////////////////////////////////////

	// item
	if (!InitItem())
	{
		return false;
	}
	///////////////////////////////////////////////////////////////////////////////////

	// pet
	IsOpenPairPet = GetInt("IsOpenPairPet", 1) > 0;

	vec.clear();
	vec = Split(KeyValue["HungryExpPercent"], '|');
	HungryExpPercent.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		HungryExpPercent.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["PetSeatBuy"], '|');
	PetSeatBuy.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PetSeatBuy.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["PetExpTransferCost"], '|');
	PetExpTransferCost.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PetExpTransferCost.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	// T show
	TShowFreeVoteNum = SafeConvert<int>(KeyValue["TShowFreeVoteNum"]);
	TShowVoteCostNum = ConvertToSequence<int, 2>(KeyValue["TShowVoteCost"], '=');
	vec.clear();
	vec = Split(KeyValue["TShowFirstVoteReward"], '|');
	TShowFirstVoteReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		TShowFirstVoteReward.push_back(ConvertToSequence<int, 2>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	// flower
	vec.clear();
	vec = Split(KeyValue["Flower2Charm"], "|");
	Flower2Charm.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Flower2Charm.push_back(ConvertToSequence<int, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["GetFlowerTime"], '=');
	GetFlowerTime.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GetFlowerTime.push_back(SafeConvert<UINT32>(vec[i]));
	}
	GetFlowerNum = atoi(KeyValue["GetFlowerNum"].c_str());
	vec.clear();
	vec = Split(KeyValue["BuyFlowerCost"], "|");
	FlowerCostNum.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		FlowerCostNum.push_back(ConvertToSequence<int, 3>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	// scene view
	InitSceneView();
	///////////////////////////////////////////////////////////////////////////////////

	// QQ/WX
	InitPlatForm();

	// pay
	IsPayFirstDouble = SafeConvert<UINT32>(KeyValue["IsPayFirstDouble"])==1?true:false;
	PayReward = SafeConvert<UINT32>(KeyValue["PayReward"]);
	///////////////////////////////////////////////////////////////////////////////////

	// gold click
	InitGoldClick();

	///////////////////////////////////////////////////////////////////////////////////
	///> 赛程相关
	LeagueBattleLevelSealType =  GetInt("LeagueBattleLevelSealType", 3);
	LeagueBattleNeedLevel = GetInt("LeagueBattleNeedLevel", 40);
	UINT32 temp = GetInt("LeagueBattleIsOpen", 1);
	LeagueBattleIsOpen = (temp == 1) ? true : false;

	// 开启日期
	vec.clear();
	vec = Split(KeyValue["LeagueBattleOpenDate"], '|');
	LeagueBattleOpenDate.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		LeagueBattleOpenDate.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	LeagueMatchSign = GetInt("LeagueMatchSign", 3);
	LeagueRacePointLastDay = GetInt("LeagueRacePointLastDay", 8);
	LeagueEleminationAfterDay = GetInt("LeagueEleminationAfterDay", 1);
	LeagueCrossRacePointAfterDay = GetInt("LeagueCrossRacePointAfterDay", 3);
	LeagueCrossRacePointLastDay = GetInt("LeagueCrossRacePointLastDay", 7);
	LeagueCrossEleminationAfterDay = GetInt("LeagueCrossEleminationAfterDay", 1);
	LeagueSeasonEndAfterDay = GetInt("LeagueSeasonEndAfterDay", 2);
	LeagueBattlePointRaceWeekDay = ConvertToVector<UINT32>(KeyValue["LeagueBattlePointRaceWeekDay"], '|');
	LeagueBattleCrossPointRaceWeekDay = ConvertToVector<UINT32>(KeyValue["LeagueBattleCrossPointRaceWeekDay"], '|');
	LeagueBattlePointRaceTime = ConvertToSequence<UINT32, 2>(KeyValue["LeagueBattlePointRaceTime"], '=');
	LeagueBattleEliminationTime = ConvertToSequence<UINT32, 4>(KeyValue["LeagueBattleEliminationTime"], '=');
	LeagueBattleEleMapTime = GetInt("LeagueBattleEleMapTime", 1800);
	LeagueBattleRaceReward = ConvertToSequence<UINT32, 2>(KeyValue["LeagueBattleRaceReward"], '=');
	LeagueClearOpen = GetInt("LeagueClearOpen", 0);
	LeagueClearAfterCrossRacePointDay = GetInt("LeagueClearAfterCrossRacePointDay", 1);

	LeagueTeamRoleNum = GetInt("LeagueTeamRoleNum", 4);
	LeagueTeamNameLenMin = GetInt("LeagueTeamNameLenMin", 2);
	LeagueTeamNameLenMax = GetInt("LeagueTeamNameLenMax", 6);
	LeagueInitPoint = GetInt("LeagueInitPoint", 1500);
	LeagueRankJoinCrosssPointRace = GetInt("LeagueRankJoinCrosssPointRace", 8);
	LeagueBattleReadyTime = GetInt("LeagueBattleReadyTime", 30);
	LeagueBattleOneFightTime = GetInt("LeagueBattleOneFightTime", 90);
	LeagueBattleReadyTimeForEighth = GetInt("LeagueBattleReadyTimeForEighth", 60);
	LeagueBattleOneFightTimeForEighth = GetInt("LeagueBattleOneFightTimeForEighth", 120);
	LeagueBattleFightAfterTime = GetInt("LeagueBattleFightAfterTime", 5);
	LeagueBattleSceneId = GetInt("LeagueBattleSceneId", 60);
	LeagueBattleRecordNum = GetInt("LeagueBattleRecordNum", 10);

	vec.clear();
	vec = Split(KeyValue["LeagueApplyNoticeTime"], '|');
	LeagueApplyNoticeTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		LeagueApplyNoticeTime.push_back(SafeConvert<UINT32>(vec[i]));
	}	
	vec.clear();
	vec = Split(KeyValue["LeagueApplyEndNoticeTime"], '|');
	LeagueApplyEndNoticeTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		LeagueApplyEndNoticeTime.push_back(SafeConvert<UINT32>(vec[i]));
	}	
	vec.clear();
	vec = Split(KeyValue["LeagueRaceNoticeTime"], '|');
	LeagueRaceNoticeTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		LeagueRaceNoticeTime.push_back(SafeConvert<UINT32>(vec[i]));
	}	
	vec.clear();
	vec = Split(KeyValue["LeagueEliNoticeTime"], '|');
	LeagueEliNoticeTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		LeagueEliNoticeTime.push_back(SafeConvert<UINT32>(vec[i]));
	}
	///////////////////////////////////////////////////////////////////////////////////

	// hero battle
	vec.clear();
	vec = Split(KeyValue["HeroBattleLootRate"], '|');
	HeroBattleLootRate.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroBattleLootRate.push_back(SafeConvert<UINT32>(vec[i]));
	}
	CHECK_COND_RETURN(HeroBattleLootRate.size() != 0, false);
	HeroBattleReviveTime = SafeConvert<UINT32>(KeyValue["HeroBattleReviveTime"]);
	HeroBattleOverTime = SafeConvert<UINT32>(KeyValue["HeroBattleOverTime"]);
	HeroBattleSecond2OnePoint = SafeConvert<float>(KeyValue["HeroBattleSecond2OnePoint"]);
	HeroBattleTimeLimitIfNoOccupant = SafeConvert<UINT32>(KeyValue["HeroBattleTimeLimitIfNoOccupant"]);

	HeroBattleChooseHeroTime = SafeConvert<UINT32>(KeyValue["HeroBattleChooseHeroTime"]);

	vec.clear();
	vec = Split(KeyValue["HeroBattleReward"], '|');
	HeroBattleReward.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroBattleReward.push_back(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["HeroBattleSpecialTime"], '|');
	HeroBattleSpecialTime.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroBattleSpecialTime.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	HeroBattleSpecialNum = SafeConvert<UINT32>(KeyValue["HeroBattleSpecialNum"]);
	vec.clear();
	vec = Split(KeyValue["HeroBattleSpecialReward"], '|');
	HeroBattleSpecialReward.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroBattleSpecialReward.push_back(SafeConvert<UINT32>(vec[i]));
	}

	HeroBattleFreeHeroCount = SafeConvert<UINT32>(KeyValue["HeroBattleFreeHeroCount"]);
	HeroBattleBigReward = ConvertToSequence<UINT32, 3>(KeyValue["HeroBattleBigReward"], '=');

	vec.clear();
	vec = Split(KeyValue["HeroBattleBigRewardRate"], '|');
	HeroBattleBigRewardRate.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroBattleBigRewardRate.push_back(SafeConvert<UINT32>(vec[i]));
	}

	HeroBattleMatchWorldTime = SafeConvert<UINT32>(KeyValue["HeroBattleMatchWorldTime"]);

	HeroBattleELOK = SafeConvert<UINT32>(KeyValue["HeroBattleELOK"]);
	HeroBattleOriginELOPoint = SafeConvert<UINT32>(KeyValue["HeroBattleOriginELOPoint"]);

	vec.clear();
	vec = Split(KeyValue["HeroBattleELOTeam"], '|');
	HeroBattleELOTeam.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		HeroBattleELOTeam.push_back(SafeConvert<UINT32>(vec[i]));
	}
	HeroBattleCanChooseSame = SafeConvert<UINT32>(KeyValue["HeroBattleCanChooseSame"]);
	///////////////////////////////////////////////////////////////////////////////////
	ReportHeroBattleMvpDelNum = GetInt("ReportHeroBattleMvpDelNum", 3);
	ReportHeroBattleInvalidKda = SafeConvert<float>(KeyValue["ReportHeroBattleInvalidKda"]);
	ReportHeroBattleLeastNum = GetInt("ReportHeroBattleLeastNum", 1);
	ReportHeroCampTotalKillNum = GetInt("ReportHeroCampTotalKillNum", 2);
	ReportHeroCampTotalAddNum = GetInt("ReportHeroCampTotalAddNum", 2);

	ReportHeroBattleMvpDelNum = GetInt("ReportHeroBattleMvpDelNum", 3);
	ReportHeroBattleInvalidKda = SafeConvert<float>(KeyValue["ReportHeroBattleInvalidKda"]);
	ReportHeroBattleLeastNum = GetInt("ReportHeroBattleLeastNum", 1);
	ReportHeroCampTotalKillNum = GetInt("ReportHeroCampTotalKillNum", 2);
	ReportHeroCampTotalAddNum = GetInt("ReportHeroCampTotalAddNum", 2);

	MobaBattleChooseHeroTime = SafeConvert<UINT32>(KeyValue["MobaBattleChooseHeroTime"]);
	MobaDeadExpRange = SafeConvert<float>(KeyValue["MobaDeadExpRange"]);
	MobaExpPer3Seconds = SafeConvert<double>(KeyValue["MobaExpPer3Seconds"]);
	MobaEnemyId = GetInt("MobaEnemyId", 35014);

	vec.clear();
	vec = Split(KeyValue["MobaBattleReward"], '|');
	MobaBattleReward.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		MobaBattleReward.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["MobaUpgradeAttack"], '|');
	MobaUpgradeAttack.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		MobaUpgradeAttack.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["MobaUpgradeDefense"], '|');
	MobaUpgradeDefense.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		MobaUpgradeDefense.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	MobaSignaTimelLimit = ConvertToSequence<UINT32, 2>(KeyValue["MobaSignaTimelLimit"], '=');

	MobaGameRecordMaxNum = SafeConvert<UINT32>(KeyValue["MobaGameRecordMaxNum"]);

	EscapeTimeRate = SafeConvert<UINT32>(KeyValue["EscapeTimeRate"]);
	EscapeMinTime = SafeConvert<UINT32>(KeyValue["EscapeMinTime"]);
	///////////////////////////////////////////////////////////////////////////////////
	InitPartner();
	///////////////////////////////////////////////////////////////////////////////////

	HorseWaitSceneID = SafeConvert<UINT32>(KeyValue["HorseWaitSceneID"]);
	HorseSceneID = SafeConvert<UINT32>(KeyValue["HorseSceneID"]);
	HorseLevelLimit = SafeConvert<UINT32>(KeyValue["HorseLevelLimit"]);
	HorseWaitTime  = SafeConvert<UINT32>(KeyValue["HorseWaitTime"]);
	HorseGroupCount = SafeConvert<UINT32>(KeyValue["HorseGroupCount"]);
	HorseStartAnimTime =  SafeConvert<UINT32>(KeyValue["HorseStartAnimTime"]);
	HorseFinalWall = SafeConvert<UINT32>(KeyValue["HorseFinalWall"]);
	HorseCollisionWallTime = SafeConvert<UINT32>(KeyValue["HorseCollisionWallTime"]);
	HorseTimeRefresh = SafeConvert<UINT32>(KeyValue["HorseTimeRefresh"]);
	HorseLaps = SafeConvert<UINT32>(KeyValue["HorseLaps"]);
	HorseID = SafeConvert<UINT32>(KeyValue["HorseID"]);
	HorsePlayerNum = SafeConvert<UINT32>(KeyValue["HorsePlayerNum"]);
	HorseWholeTime = SafeConvert<UINT32>(KeyValue["HorseWholeTime"]);

	///////////////////////////////////////////////////////////////////////////////////
	InitGuildTerr();  // 公会领地配置
	///////////////////////////////////////////////////////////////////////////////////
	InitGuildInherit(); // 公会传承
	GuildRecruitTime = SafeConvert<UINT32>(KeyValue["GuildRecruitTime"]);

	InheritPos = ConvertToSequence<float, 4>(KeyValue["InheritPos"], '=');
	TeacherInheritPos = ConvertToSequence<float, 4>(KeyValue["TeacherInheritPos"], '=');
	TeacherInheritTime = SafeConvert<UINT32>(KeyValue["TeacherInheritTime"]);

	vec.clear();
	vec = Split(KeyValue["GuildInheritRoleLvlGap"], '|');
	GuildInheritRoleLvlGap.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		GuildInheritRoleLvlGap.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	WatchCommendTimeSpan = SafeConvert<UINT32>(KeyValue["WatchCommendTimeSpan"]);
	WatchCommendTotalCount= SafeConvert<UINT32>(KeyValue["WatchCommendTotalCount"]);
	WatchPVPScore = SafeConvert<UINT32>(KeyValue["Spectate_PVP_show_baseline"]);
	///////////////////////////////////////////////////////////////////////////////////

	// sprite
	SpriteEvolutionMinLevel = SafeConvert<UINT32>(KeyValue["SpriteEvolutionMinLevel"]);
	SpriteAwakeMinLevel = SafeConvert<UINT32>(KeyValue["SpriteAwakeMinLevel"]);
	SpriteAwakeItemID = SafeConvert<UINT32>(KeyValue["SpriteAwakeItemID"]);
	vec.clear();
	vec = Split(KeyValue["SpriteAwakeConsume"], '|');
	SpriteAwakeConsume.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		SpriteAwakeConsume.push_back(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["SpritePositionLevel"], '|');
	SpritePositionLevel.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		SpritePositionLevel.push_back(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["SpriteFoodExp"], '|');
	SpriteFoodExp.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		SpriteFoodExp.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}
	SpriteExpRatio = SafeConvert<double>(KeyValue["SpriteExpRatio"]);
	vec.clear();
	vec = Split(KeyValue["SpriteTrainCost"], '|');
	SpriteTrainCost.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		SpriteTrainCost.push_back(ConvertToSequence<UINT32, 4>(vec[i], '='));
	}
	SpriteTrainNoToChooseMaxNum = SafeConvert<UINT32>(KeyValue["SpriteTrainNoToChooseMaxNum"]);

	SpriteDrawGoodRange = ConvertToSequence<UINT32, 2>(KeyValue["SpriteDrawGoodRange"], '=');
	SpriteGoldDrawGoodRange = ConvertToSequence<UINT32, 2>(KeyValue["SpriteGoldDrawGoodRange"], '=');
	SpriteMaxNum = SafeConvert<UINT32>(KeyValue["SpriteMaxNum"]);
	SpriteMinGuaranteeCount = ConvertToSequence<UINT32, 2>(KeyValue["SpriteMinGuaranteeCount"], '=');

	vec.clear();
	vec = Split(KeyValue["SpriteZero"], '|');
	SpriteZero.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		SpriteZero.push_back(ConvertToSequence<UINT32, 2>(*i, '='));
	}

	// sprite draw
	SpriteDrawCost = ConvertToSequence<UINT32, 2>(KeyValue["SpriteDrawCost"], '=');
	SpriteGoldDrawCost = ConvertToSequence<UINT32, 2>(KeyValue["SpriteGoldDrawCost"], '=');

	// sprite shop
	SpriteShopGoldOneCost = ConvertToSequence<UINT32, 2>(KeyValue["SpriteShopGoldOneCost"], '=');
	SpriteShopGoldTenCost = ConvertToSequence<UINT32, 2>(KeyValue["SpriteShopGoldTenCost"], '=');
	SpriteShopOneCost = ConvertToSequence<UINT32, 2>(KeyValue["SpriteShopOneCost"], '=');
	SpriteShopTenCost = ConvertToSequence<UINT32, 2>(KeyValue["SpriteShopTenCost"], '=');

	vec.clear();
	vec = Split(KeyValue["SpriteShopGoldOneGain"], '|');
	SpriteShopGoldOneGain.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopGoldOneGain.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopGoldOneGift"], '|');
	SpriteShopGoldOneGift.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopGoldOneGift.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopGoldTenGain"], '|');
	SpriteShopGoldTenGain.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopGoldTenGain.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopGoldTenGift"], '|');
	SpriteShopGoldTenGift.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopGoldTenGift.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopOneGain"], '|');
	SpriteShopOneGain.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopOneGain.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopOneGift"], '|');
	SpriteShopOneGift.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopOneGift.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopTenGain"], '|');
	SpriteShopTenGain.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopTenGain.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SpriteShopTenGift"], '|');
	SpriteShopTenGift.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SpriteShopTenGift.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	RiskOpenAddDiceNum = SafeConvert<UINT32>(KeyValue["RiskOpenAddDiceNum"]);
	RiskRecoverDiceMaxNum = SafeConvert<UINT32>(KeyValue["RiskRecoverDiceMaxNum"]);
	RiskDiceMaxNum = SafeConvert<UINT32>(KeyValue["RiskDiceMaxNum"]);
	RiskRecoverDiceNum = ConvertToSequence<UINT32, 2>(KeyValue["RiskRecoverDiceNum"], '=');
	RiskReBattleCost = ConvertToSequence<UINT32, 2>(KeyValue["RiskReBattle"], '=');
	vec.clear();
	vec = Split(KeyValue["RiskBoxTime"], '|');
	RiskBoxTime.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		RiskBoxTime.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["RiskBoxAccelerate"], '|');
	RiskBoxAccelerate.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		RiskBoxAccelerate.push_back(ConvertToSequence<UINT32, 4>(vec[i], '='));
	}
	///////////////////////////////////////////////////////////////////////////////////

	ManualRollTime = SafeConvert<UINT32>(KeyValue["ManualRollTime"]);
	HardDragonDropDiamondBox = SafeConvert<UINT32>(KeyValue["HardDragonDropDiamondBox"]);
	vec.clear();
	vec = Split(KeyValue["DragonResetWeekDay"], '|');
	DragonResetWeekDay.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		DragonResetWeekDay.push_back(atoi(vec[i].c_str()));
	}
	vec.clear();
	vec = Split(KeyValue["HardDragonResetWeekDay"], '|');
	HardDragonResetWeekDay.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		HardDragonResetWeekDay.push_back(atoi(vec[i].c_str()));
	}
	vec.clear();
	vec = Split(KeyValue["SmallDragonResetWeekDay"], '|');
	SmallDragonResetWeekDay.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		SmallDragonResetWeekDay.push_back(atoi(vec[i].c_str()));
	}
	vec.clear();
	vec = Split(KeyValue["TeamTowerSceneID"], '|');
	TeamTowerSceneID.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		TeamTowerSceneID.push_back(ConvertToSequence<int, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SweepTowerCost"], '|');
	SweepTowerCost.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		SweepTowerCost.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SweepTowerTime"], '|');
	SweepTowerTime.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		SweepTowerTime.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["RefreshSweepCost"], '|');
	RefreshSweepCost.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		RefreshSweepCost.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["RefreshSweepRand"], '|');
	RefreshSweepRandValue.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		RefreshSweepRandValue.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["GoddessTrialRewards"], '|');
	GoddessTrialRewards.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		GoddessTrialRewards.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	StopMark = KeyValue["StopMark"];
	ExpFindBackNeedLevel = SafeConvert<int>(KeyValue["ExpFindBackNeedLevel"]);
	ExpFindBackParam = SafeConvert<int>(KeyValue["ExpFindBackParam"]);
	ItemFindBackNeedLevel = SafeConvert<int>(KeyValue["ItemFindBackNeedLevel"]);

	BattleAssistLevel = SafeConvert<int>(KeyValue["BattleAssistLevel"]);
	LevelSealFragmentID = SafeConvert<UINT32>(KeyValue["LevelSealFragmentID"]);
	OpenIBShopRedPointLv = SafeConvert<UINT32>(KeyValue["OpenIBShopRedPointLv"]);
	WorldLevelCount = SafeConvert<UINT32>(KeyValue["WorldLevelCount"]);
	DailyTaskAskHelpNum = GetInt("DailyTaskAskHelpNum", 3);	
	DailyTaskDonateNum = GetInt("DailyTaskDonateNum", 10);
	GragonCoinExchangeGold = SafeConvert<UINT32>(KeyValue["GragonCoinExchangeGold"]);
	StageCatchUpExpMinLevel = SafeConvert<UINT32>(KeyValue["StageCatchUpExpMinLevel"]);

	vec.clear();
	vec = Split(KeyValue["ActivityChestExp"], '|');
	ActivityChestExp.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		ActivityChestExp.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["ActivityChestExpWeekly"], '|');
	ActivityChestExpWeekly.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		ActivityChestExpWeekly.push_back(SafeConvert<UINT32>(vec[i]));
		ActivityChestExp.push_back(SafeConvert<UINT32>(vec[i]));
	}

	RefreshIBShopTime = ConvertToSequence<UINT32, 3>(KeyValue["RefreshIBShopTime"], '=');
	IBShopGiftTextLen = SafeConvert<UINT32>(KeyValue["IBShopGiftTextLen"]);
	IBShopHistMaxSize = SafeConvert<UINT32>(KeyValue["IBShopHistMaxSize"]);
	IBShopLevel = SafeConvert<UINT32>(KeyValue["IBShopLevel"]);
	IBShopDegree = SafeConvert<UINT32>(KeyValue["IBShopDegree"]);
	IBHistPay =  SafeConvert<UINT32>(KeyValue["IBHistPay"]);
	IBDayMaxBuyCount = SafeConvert<UINT32>(KeyValue["IBDayMaxBuyCount"]);
	IBEveryTimeMaxBuy = SafeConvert<UINT32>(KeyValue["IBEveryTimeMaxBuy"]);
	IBOverMaxUnRecv = SafeConvert<UINT32>(KeyValue["IBOverMaxUnRecv"]);


	StopMark = KeyValue["StopMark"];

	///////////////////////////////////////////////////////////////////////////////////
	AudioMsgMax = SafeConvert<UINT32>(KeyValue["AudioMsgMax"]);
	GuildPrestigeDle = SafeConvert<UINT32>(KeyValue["GuildPrestigeDle"]);
	GuildWageKeepDay = SafeConvert<UINT32>(KeyValue["GuildWageKeepDay"]);
	vec.clear();
	vec = Split(KeyValue["GuildCampDayID"], '|');
	GuildCampDayID.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32, 2> temp = ConvertToSequence<UINT32, 2>(vec[i], '=');
		GuildCampDayID[temp.seq[0]] = temp.seq[1];
	}
	GivePlatFriendGift = ConvertToSequence<UINT32, 2>(KeyValue["SendPlatFriendGift"], '=');
	GivePlatFriendGiftMailTitle = KeyValue["GivePlatFriendGiftMailTitle"];
	GivePlatFriendGiftMailContent = KeyValue["GivePlatFriendGiftMailContent"];

	///////////////////////////////////////////////////////////////////////////////////
	PandoraPushDay =  SafeConvert<UINT32>(KeyValue["PandoraPushDay"]);
	LaddyPushDay = GetInt("LaddyPushDay", 90);

	vec.clear();
	vec = Split(KeyValue["QuestionnaireLevel"], '|');
	QuestionnaireLevel.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		QuestionnaireLevel.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["QuestionnaireVersion"], '|');
	QuestionnaireVersion.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		QuestionnaireVersion.push_back(SafeConvert<UINT32>(vec[i]));
	}
	CHECK_COND_RETURN(QuestionnaireVersion.size() == QuestionnaireLevel.size(), false);
	///////////////////////////////////////////////////////////////////////////////////

	vec = Split(KeyValue["DrawGoodRange"], '|');
	CHECK_COND_RETURN(vec.size() == 2, false);

	InitFlower();

	InitMentor();

	ShareGiftProb = convert<UINT32>(KeyValue["ShareGiftProb"]);
	ShareGiftCountDay = convert<UINT32>(KeyValue["ShareGiftCountDay"]);

	BackFlowNoLoginDayTime = SafeConvert<UINT32>(KeyValue["BackFlowNoLoginDayTime"]);
	BackFlowActivityDayTime = SafeConvert<UINT32>(KeyValue["BackFlowActivityDayTime"]);
	BackFlowStartAgainMinDayTime = SafeConvert<UINT32>(KeyValue["BackFlowStartAgainMinDayTime"]);
	vec.clear();
	vec = Split(KeyValue["BackFlowTreasure"], '|');
	BackFlowTreasure.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		BackFlowTreasure.push_back(ConvertToSequence<UINT32, 3>(*i, '='));
	}
	BackFlowPayAlieenCount = SafeConvert<UINT32>(KeyValue["BackFlowPayAlieenCount"]);
	BackFlowShopGoodCount = SafeConvert<UINT32>(KeyValue["BackFlowShopGoodCount"]);
	BackFlowShopFreshCount = SafeConvert<UINT32>(KeyValue["BackFlowShopFreshCount"]);
	BackFlowShopDuration = SafeConvert<UINT32>(KeyValue["BackFlowShopDuration"]);
	BackFlowJudgeDayTime = SafeConvert<UINT32>(KeyValue["BackFlowJudgeDayTime"]);
	BackFlowJudgeMaxDayTime = SafeConvert<UINT32>(KeyValue["BackFlowJudgeMaxDayTime"]);
	BackFlowActivitySceneID = SafeConvert<UINT32>(KeyValue["BackFlowActivitySceneID"]);
	BackFlowNestFinishCountLimit = SafeConvert<UINT32>(KeyValue["BackFlowNestFinishCountLimit"]);

	GuildCampTaskDesc = KeyValue["WxGuildCampTask"];
	GuildSignWeekCount = SafeConvert<UINT32>(KeyValue["GuildSignWeekCount"]);

	FindBackTicketExchangeDragon = SafeConvert<UINT32>(KeyValue["FindBackTicketExchangeDragon"]);

	LevelMailLevel = GetInt("LevelMailLevel", 30);
	LevelMailID = GetInt("LevelMailID", 0);

	RenameItemCost = ConvertToSequence<UINT32, 2>(KeyValue["RenameItemCost"], '='); 
	ReGuildNameItemCost = ConvertToSequence<UINT32, 2>(KeyValue["ReGuildNameItemCost"], '='); 
	ReDragonGuileNameItemCost = ConvertToSequence<UINT32, 2>(KeyValue["ReDragonGuildNameItemCost"], '=');

	ArgentaLastTime = SafeConvert<UINT32>(KeyValue["ArgentaLastTime"]);
	ArgentaWithinDays = SafeConvert<UINT32>(KeyValue["ArgentaWithinDays"]);
	ArgentaActivityPoint = SafeConvert<UINT32>(KeyValue["ArgentaActivityPoint"]);
	ArgentaNestCount = SafeConvert<UINT32>(KeyValue["ArgentaNestCount"]);
	ArgentaEndToStartLimit = SafeConvert<UINT32>(KeyValue["ArgentaEndToStartLimit"]);

	PveGeraldGiftOpen = SafeConvert<UINT32>(KeyValue["PveGeraldGiftOpen"]);
	PveGeraldGiftAssistRequire =  SafeConvert<UINT32>(KeyValue["PveGeraldGiftAssistRequire"]);
	PveGeraldGiftAssistRewardOpen =  SafeConvert<UINT32>(KeyValue["PveGeraldGiftAssistRewardOpen"]);
	vec.clear();
	vec = Split(KeyValue["PveGeraldGiftReward"], '|');
	PveGeraldGiftReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
		PveGeraldGiftReward.push_back(ConvertToSequence<UINT32, 5>(vec[i], '='));

	TarjaItem =  ConvertToSequence<UINT32, 2>(KeyValue["TarjaItem"], '=');
	TarjaTime =  SafeConvert<UINT32>(KeyValue["TarjaTime"]);
	TarjaUseItem = SafeConvert<UINT32>(KeyValue["TarjaUseItem"]);
	TarjaSwitch = SafeConvert<UINT32>(KeyValue["TarjaSwitch"]);

	LoginActivityOpenDay = SafeConvert<UINT32>(KeyValue["LoginActivityOpenDay"]);
	LoginActivityLevel = SafeConvert<UINT32>(KeyValue["LoginActivityLevel"]);

	GrowthFundBuyDays =  SafeConvert<UINT32>(KeyValue["GrowthFundBuyDays"]);
	GrowthFundExpireMailDays = SafeConvert<UINT32>(KeyValue["GrowthFundExpireMailDays"]);
	GrowthFundExpireNoticeDays = SafeConvert<UINT32>(KeyValue["GrowthFundExpireNoticeDays"]);
	vec.clear();
	vec = Split(KeyValue["GrowthFundExpireNoticeTime"], '|');
	GrowthFundExpireNoticeTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		GrowthFundExpireNoticeTime.push_back(SafeConvert<UINT32>(vec[i]));
	}	

	ArtifactLevels = ConvertToVector<UINT32>(KeyValue["ArtifactLevels"], '|');

	vec.clear();
	vec = Split(KeyValue["RecastAttrRate"], '|');
	RecastAttrRate.clear();
	UINT32 sum = 0;
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		RecastAttrRate.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
		sum += RecastAttrRate[i][0];
	}
	if (sum != 100)
	{
		LogWarn("RecastAttrRate first sum must be 100, now is [%u]", sum);
		assert(false);	
	}

	AssistDamageTime = GetInt("AssistDamageTime", 15);
	vec.clear();
	vec = Split(KeyValue["MultiKillTime"], '|');
	MultiKillTime.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		MultiKillTime.push_back(SafeConvert<UINT32>(vec[i]));
	}
	assert(MultiKillTime.size() != 0);

	SkillEmblemChangeProfMailTitle = KeyValue["SkillEmblemChangeProfMailTitle"];
	SkillEmblemChangeProfMailContent = KeyValue["SkillEmblemChangeProfMailContent"];

	vec.clear();
	vec = Split(KeyValue["SkillEmblemDisableSceneTypes"], '|');
	SkillEmblemDisableSceneTypes.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		SkillEmblemDisableSceneTypes.insert(SafeConvert<UINT32>(vec[i]));
	}	

	vec.clear();
	vec = Split(KeyValue["ArtifactDisableSceneTypes"], '|');
	ArtifactDisableSceneTypes.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		ArtifactDisableSceneTypes.insert(SafeConvert<UINT32>(vec[i]));
	}	

	vec.clear();
	vec = Split(KeyValue["HeroBattleBanTime"], '|');
	HeroBattleBanTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		HeroBattleBanTime.push_back(ConvertToSequence<UINT32, 5>(vec[i], '='));
	}

	PVEMorphLevelSealType = SafeConvert<UINT32>(KeyValue["PVEMorphLevelSealType"].c_str());
	ChannelMaskDays = SafeConvert<UINT32>(KeyValue["ChannelMaskDays"].c_str());

	vec.clear();
	vec = Split(KeyValue["CanLevelTransSkill"], '|');
	CanLevelTransSkill.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		CanLevelTransSkill.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	AllDisconnectKickTime = GetInt("AllDisconnectKickTime", 300);



	DuckTickets = ConvertToSequence<UINT32, 2>(KeyValue["DuckTickets"], '=');
	vec.clear();
	vec = Split(KeyValue["ThemeActivityScene"], '|');
	ThemeActivityScene.clear();
	for(UINT32 i = 0; i < (UINT32)vec.size(); i++)
	{
		ThemeActivityScene.push_back(SafeConvert<UINT32>(vec[i]));
	}
	DayDuckTickets = ConvertToSequence<UINT32, 2>(KeyValue["DayDuckTickets"], '=');
	BioHellRankId = SafeConvert<UINT32>(KeyValue["BioHellRankId"].c_str());

	vec.clear();
	vec = Split(KeyValue["BioHellSceneStage"], '|');
	BioHellSceneStage.clear();
	for(UINT32 i = 0; i < (UINT32)vec.size(); i++)
	{
		BioHellSceneStage.push_back(SafeConvert<UINT32>(vec[i]));
	}

	MarriageCost = ConvertToSequence<UINT32, 2>(KeyValue["MarriageCost"], '=');
	MarriageSplendidCost = ConvertToSequence<UINT32, 2>(KeyValue["MarriageSplendidCost"], '=');
	MarriageFriendDegreeNeed = SafeConvert<UINT32>(KeyValue["MarriageFriendDegree"].c_str());
	MarriageDivorceCost = ConvertToSequence<UINT32, 2>(KeyValue["MarriageDivorceCost"], '=');
	MarriageFreeDivorceDay = SafeConvert<UINT32>(KeyValue["MarriageFreeDivorceDay"].c_str());

	ApplyMarriageDistance = SafeConvert<UINT32>(KeyValue["ApplyMarriageDistance"].c_str());
	MarriageRing = SafeConvert<UINT32>(KeyValue["MarriageRing"].c_str());
	MarriageSplendidRing = SafeConvert<UINT32>(KeyValue["MarriageSplendidRing"].c_str());
	NormalMarriageBuff = ConvertToSequence<UINT32, 2>(KeyValue["PartnerSkill"], '=');
	SplendidMarriageBuff = ConvertToSequence<UINT32, 2>(KeyValue["PartnerSkill2"], '=');

	TransOtherItemId = GetInt("TransOtherItemId", 5837);
	TransOtherItemIdFirst = GetInt("TransOtherItemIdFirst", 3050);
	IsTransOpen = GetInt("IsTransOpen", 1) > 0;

	vec.clear();
	vec = Split(KeyValue["TransSceneType"], '|');
	TransSceneType.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		UINT32 type = convert<UINT32>(*i);
		TransSceneType.insert(type);
	}

	CombineServerLevel = GetInt("CombineServerLevel", 10);
	CombineServerDayLimit = GetInt("CombineServerDayLimit", 20);
	vec.clear();
	vec = Split(KeyValue["CombineServerReward"], '|');
	CombineServerReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		CombineServerReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	CombineServerExpRate = GetFloat("CombineServerExpRate", 1.0f);

	TransformBuffsChangeOutlook.clear();
	vec = Split(KeyValue["TransformBuffsChangeOutlook"], '|');
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		TransformBuffsChangeOutlook.insert(convert<int>(*i));
	}

	PvpBiasSceneTypes.clear();
	vec = Split(KeyValue["PvpProfessionBiasSceneTypes"], '|');
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		PvpBiasSceneTypes.insert(convert<int>(*i));
	}

	DragonGuildWeekTaskCount = SafeConvert<UINT32>(KeyValue["DragonGuildTaskCount"].c_str());
	DragonGuildWeekReciveCount = SafeConvert<UINT32>(KeyValue["DragonGuildReceiveCount"].c_str());
	DragonGuildCreateVipReq = SafeConvert<UINT32>(KeyValue["DragonGuildCreateVipReq"]);
	DragonGuildApplyMax = SafeConvert<UINT32>(KeyValue["DragonGuildApplyMax"]);
	DragonGuildGroupShareTitle = KeyValue["DragonGuildApplyMax"];
	DragonGuildGroupShareContent = KeyValue["DragonGuildApplyMax"];
	DragonGuildEnterLimit = ConvertToSequence<UINT32, 2>(KeyValue["DragonGuildEnterLimit"], '=');
	DragonGuildShopID = SafeConvert<UINT32>(KeyValue["DragonGuildShopID"]);
	DragonGuildShopRecordNum = SafeConvert<UINT32>(KeyValue["DragonGuildShopRecordNum"]);
	DragonGuildRecruitTime = SafeConvert<UINT32>(KeyValue["DragonGuildRecruitTime"]);
	DragonGuildDesignation = KeyValue["DragonGuildDesignation"];
	DragonGuildActLeaDay = SafeConvert<UINT32>(KeyValue["DragonGuildActLeaDay"]);
	DragonGuildTaskRefreshStartDate = ConvertToSequence<UINT32, 3>(KeyValue["DragonGuildRefreshTaskStartDate"], '=');
	DragonGuildTaskRefreshCycleTime = SafeConvert<UINT32>(KeyValue["DragonGuildRefreshTaskCycleTime"]);
	DragonGuildTaskNeedPartnerCount = SafeConvert<UINT32>(KeyValue["DragonGuildTaskNeedPartnerCount"]);
	DragonGuildLivenessRecordNum = SafeConvert<UINT32>(KeyValue["DragonGuildLivenessRecordNum"]);
	DragonGuildSyncDataMaxSize = SafeConvert<UINT32>(KeyValue["DragonGuildSyncDataMaxSize"]);

	TransOtherItemId = GetInt("TransOtherItemId", 5837);
	IsTransOpen = GetInt("IsTransOpen", 1) > 0;

	vec.clear();
	vec = Split(KeyValue["TransSceneType"], '|');
	TransSceneType.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		UINT32 type = convert<UINT32>(*i);
		TransSceneType.insert(type);
	}

	//转盘活动
	InitLuckyTable();

	PeerBox = ConvertToSequence<UINT32, 2>(KeyValue["PeerBox"], '=');

	InitHG();

	vec.clear();
	vec = Split(KeyValue["NewZoneChargeBackTime"], '|');
	std::vector<Sequence<UINT32, 3>> backTime;
	for (size_t i = 0; i < vec.size(); i++)
	{
		backTime.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}
	if (backTime.size() != 2)
	{
		LogError("NewZoneChargeBackTime size != 2");
		return false;
	}
	NewZoneChargeBackTimeStartTime = GameTime::MakeTime(backTime[0][0], backTime[0][1], backTime[0][2]);
	NewZoneChargeBackTimeEndTime = GameTime::MakeTime(backTime[1][0], backTime[1][1], backTime[1][2]);
	Sequence<UINT32, 3> rewardEndTime = ConvertToSequence<UINT32, 3>(KeyValue["NewZoneChargeBackRewardEndTime"], '=');
	NewZoneChargeBackRewardEndTime = GameTime::MakeTime(rewardEndTime[0], rewardEndTime[1], rewardEndTime[2]);

	vec.clear();
	vec = Split(KeyValue["NewZoneChargeBackRange"], '|');
	std::vector<Sequence<UINT32, 2>> backRange;
	for (size_t i = 0; i < vec.size(); i++)
	{
		backRange.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	NewZoneChargeBackRange.clear();
	for (auto it = backRange.begin(); it != backRange.end(); ++it)
	{
		auto& r = *it;
		NewZoneChargeBackRange[r[0]] = r[1];
	}
	Sequence<UINT32, 3> NewZoneOpenTimeSeq = ConvertToSequence<UINT32, 3>(KeyValue["NewZoneOpenTime"], '=');
	NewZoneOpenTime = GameTime::MakeTime(NewZoneOpenTimeSeq[0], NewZoneOpenTimeSeq[1], NewZoneOpenTimeSeq[2]);

	NewZoneChargeBackMax = GetInt("NewZoneChargeBackMax", 100000);
	NewZoneChargeBackRate = GetInt("NewZoneChargeBackRate", 100);


	vec.clear();
	vec = Split(KeyValue["DonateItemQualityContribute"], '|');
	std::vector<Sequence<UINT32, 2>> ditem;
	for (size_t i = 0; i < vec.size(); i++)
	{
		ditem.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	DonateItemQualityContribute.clear();
	for (auto it = ditem.begin(); it != ditem.end(); ++it)
	{
		DonateItemQualityContribute[(*it)[0]] =  (*it)[1];
	}

	BriefShop = GetInt("BriefShop", 1) > 0;

	return true;
}

const std::string& GlobalConfig::GetConfig(const char *Key)
{
	return m_mapKeyValue[Key];
}

int GlobalConfig::GetInt(const char *Key, int default_)
{
	auto i = m_mapKeyValue.find(Key);
	if (i == m_mapKeyValue.end())
	{
		return default_;
	}

	return SafeConvert<int>(i->second);
}

float GlobalConfig::GetFloat(const char *Key, float default_)
{
	auto i = m_mapKeyValue.find(Key);
	if (i == m_mapKeyValue.end())
	{
		return default_;
	}

	return SafeConvert<float>(i->second);
}

bool GlobalConfig::InitKeyValue()
{
	const char *filename = "table/GlobalConfig.txt";
	GlobalTable oTable;
	if (!oTable.LoadFile(filename))
	{
		LogWarn("Load %s failed!", filename);
		return false;
	}

	m_mapKeyValue.clear();
	for (unsigned int i = 0; i < oTable.Table.size(); ++i)
	{
		auto &row = *oTable.Table[i];
		if (m_mapKeyValue.find(row.Name) != m_mapKeyValue.end())
		{
			LogWarn("key [%s] is duplicated key", row.Name.c_str());
			CHECK_COND_RETURN(false, false);
			return false;
		}
		m_mapKeyValue[row.Name] = row.Value;
	}
	return true;
}

bool GlobalConfig::InitBigMelee()
{
	std::vector<std::string> vec;
	vec.clear();
	BigMeleeReadyScene	= SafeConvert<UINT32>(KeyValue["BigMeleeReadyScene"].c_str());
	vec.clear();
	BigMeleeFightScene = ConvertToVector<UINT32>(KeyValue["BigMeleeFightScene"], '|');
	BigMeleeRestTime	= SafeConvert<UINT32>(KeyValue["BigMeleeRestTime"].c_str());
	BigMeleeFightTime	= SafeConvert<UINT32>(KeyValue["BigMeleeFightTime"].c_str());
	BigMeleeGroupOne    = SafeConvert<UINT32>(KeyValue["BigMeleeGroupOne"].c_str());
	BigMeleeBigGroup    = SafeConvert<UINT32>(KeyValue["BigMeleeBigGroup"].c_str());
	BigMeleeUseGroup    = SafeConvert<UINT32>(KeyValue["BigMeleeUseGroup"].c_str());
	BigMeleeMaxGames    = SafeConvert<UINT32>(KeyValue["BigMeleeMaxGames"].c_str());
	BigMeleeSvrGroup    = SafeConvert<UINT32>(KeyValue["BigMeleeSvrGroup"].c_str());
	BigMeleeAnimTime    = SafeConvert<UINT32>(KeyValue["BigMeleeAnimTime"].c_str());
	BigMeleeReviveTime  = SafeConvert<UINT32>(KeyValue["BigMeleeReviveTime"].c_str());
	BigMeleeReqRankTime = SafeConvert<UINT32>(KeyValue["BigMeleeReqRankTime"].c_str());
	BigMeleeSyncTime    = SafeConvert<UINT32>(KeyValue["BigMeleeSyncTime"].c_str());
	BigMeleePickUpItemPoint = SafeConvert<UINT32>(KeyValue["BigMeleePickUpItemPoint"].c_str());
	BigMeleeJoinTime    = SafeConvert<UINT32>(KeyValue["BigMeleeJoinTime"].c_str());
	vec.clear();
	vec = Split(KeyValue["BigMeleeResetPos"], '|');
	BigMeleeResetPos.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BigMeleeResetPos.push_back(ConvertToSequence<float, 3>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["BigMeleeKilerLvPoint"], '|');
	BigMeleeKillerLvPoint.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BigMeleeKillerLvPoint.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	BigMeleeJoinAward = ConvertToVector<UINT32>(KeyValue["BigMeleeJoinAward"], '|');
	BigMeleeDestoryDay = SafeConvert<UINT32>(KeyValue["BigMeleeDestoryDay"]);
	BigMeleeSysSwitch  = SafeConvert<UINT32>(KeyValue["BigMeleeSysSwitch"]);
	BigMeleeNeedPlayLevel	= SafeConvert<UINT32>(KeyValue["BigMeleeNeedPlayLevel"].c_str());

	vec.clear();
	vec = Split(KeyValue["BigMeleePreHourNoticeList"], '|');
	BigMeleePreHourNoticeList.clear();
	for(UINT32 i = 0; i < (UINT32)vec.size(); i++)
	{
		BigMeleePreHourNoticeList.push_back(SafeConvert<UINT32>(vec[i]));
	}

	BigMeleeCutScenes  = SafeConvert<UINT32>(KeyValue["BigMeleeCutScenes"]);
	BigMeleeMSCutSceneRoles = SafeConvert<UINT32>(KeyValue["BigMeleeMSCutSceneRoles"]);

	return true;
}

bool GlobalConfig::InitTeam()
{
	std::vector<std::string> vec;
	TeamMatchDelay = SafeConvert<UINT32>(KeyValue["TeamMatchDelay"]);
	TeamFastMatchConfirmT = SafeConvert<UINT32>(KeyValue["TeamFastMatchConfirmT"]);
	TeamFastMatchBuff = SafeConvert<UINT32>(KeyValue["TeamFastMatchBuff"]);
	TeamVoteTime = SafeConvert<UINT32>(KeyValue["TeamVoteTime"]);
	TeamInviteConfirmTime = SafeConvert<UINT32>(KeyValue["TeamInviteConfirmTime"]);
	TeamInvUnfConfirmTime = SafeConvert<UINT32>(KeyValue["TeamInvUnfConfirmTime"]);
	TeamUnfRefuseForNowT = SafeConvert<UINT32>(KeyValue["TUnfRefuseForNowT"]);
	TeamInviteHistoryCount = SafeConvert<int>(KeyValue["TeamInviteHistoryCount"]);
	TeamInvRefuseHistoryTime = SafeConvert<UINT32>(KeyValue["TInvRefuseTime"]);
	TeamInvIgnoreHistoryTime = SafeConvert<UINT32>(KeyValue["TInvIgnoreTime"]);
	TeamKickLeaderNoticeTime = SafeConvert<UINT32>(KeyValue["TeamKickLeaderNoticeTime"]);
	TeamGuildBuff = ConvertToSequence<UINT32, 2>(KeyValue["TeamGuildBuff"], '=');
	TeamInviteFriendCount = SafeConvert<int>(KeyValue["TeamInviteFriendCount"]);
	TeamInviteGuildCount = SafeConvert<int>(KeyValue["TeamInviteGuildCount"]);
	TeamInviteRecCount = SafeConvert<int>(KeyValue["TeamInviteRecCount"]);
	TeamInviteHistoryTime = SafeConvert<UINT32>(KeyValue["TeamInviteHistoryTime"]);
	TeamCostMailLimit = SafeConvert<UINT32>(KeyValue["TeamCostMailLimit"]);
	TeamDiamondCostLimit = SafeConvert<UINT32>(KeyValue["TeamDiamondCostLimit"]); 
	TeamDiamondRevenue = SafeConvert<UINT32>(KeyValue["TeamDiamondRevenue"]);
	TeamLevelSealGap = GetInt("TeamLevelSealGap", 0);
	TeamCostWorldLevel = GetInt("TeamCostWorldLevel", 25);

	vec.clear();
	vec = Split(KeyValue["BuyNestCountCost"], '|');
	BuyNestCountCost.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		BuyNestCountCost.push_back(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["BuyAbyssCountCost"], '|');
	BuyAbyssCountCost.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		BuyAbyssCountCost.push_back(SafeConvert<UINT32>(vec[i]));
	}

	TeamDiamondCostCount = SafeConvert<UINT32>(KeyValue["TeamDiamondCostCount"]);
	TeamUseTicketCount = SafeConvert<UINT32>(KeyValue["TeamUseTicketCount"]);

	TeamKingBackRewardCountLimit = SafeConvert<UINT32>(KeyValue["TeamKingBackRewardCountLimit"]);
	TeamKingBackRewards.clear();
	vec = Split(KeyValue["TeamKingBackRewards"], '|');
	for(size_t i = 0; i < vec.size(); ++i)
	{
		TeamKingBackRewards.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}

	TeamGrayOpen = false;//灰度默认关掉
	TeamGrayServerIDs.clear();
	vec.clear();	
	vec = Split(KeyValue["TeamGrayServer"], '|');
	for(size_t i = 0; i < vec.size(); ++i)
	{
		if(0 == i)//表示开关配置
		{
			if(vec[i] != "0")
			{
				TeamGrayOpen = true;
			}
		}
		else
		{
			TeamGrayServerIDs.insert(SafeConvert<UINT32>(vec[i]));
		}
	}
	SSInfo << " team gray open = " << TeamGrayOpen << END;
	for(auto it = TeamGrayServerIDs.begin(); it != TeamGrayServerIDs.end(); ++it)
	{
		SSInfo << " team gray server = " << (*it) << END;
	}

	vec.clear();
	vec = Split(KeyValue["NestDropItemsOnUseTicket"], '|');
	NestDropItemsOnUseTicket.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		NestDropItemsOnUseTicket.insert(SafeConvert<UINT32>(vec[i]));
	}
	return true;
}

bool GlobalConfig::PkInSameLevelRange(UINT32 lev1, UINT32 lev2)
{
	for(size_t i = 0; i < PkMatchLevelRange.size(); ++i)
	{
		UINT32 count = 0;
		if(PkMatchLevelRange[i][0] <= lev1 && lev1 <= PkMatchLevelRange[i][1])
		{
			++count;
		}
		if(PkMatchLevelRange[i][0] <= lev2 && lev2 <= PkMatchLevelRange[i][1])
		{
			++count;
		}
		if(2 == count)
		{
			return true;
		}
		if(1 == count)
		{
			return false;
		}
	}
	return true;
}

UINT32 GlobalConfig::PkGetResetLeftTime(UINT32 lastResetTime, UINT32 now)
{
	if(0 == PkWeekResetNum)//配置错误
	{
		return 1;
	}
	if(0 == lastResetTime)
	{
		SSError << " first time " << END;
		return 1;
	}
	UINT32 NWeekTime = PkWeekResetNum * (XCommon::GetOneDayTime() * 7);
	UINT32 nextResetTime = 0;
	if(lastResetTime < PkWeekResetBaseTime)
	{
		nextResetTime = PkWeekResetBaseTime;
	}
	else
	{
		nextResetTime = PkWeekResetBaseTime + (lastResetTime - PkWeekResetBaseTime) / NWeekTime * NWeekTime + NWeekTime;//取整
	}
	//const std::string timeStr = TimeUtil::GetDateTimeStr(nextResetTime);
	//SSWarn << " next reset time = " << TimeUtil::GetDateTimeStr(lastResetTime) 
	//	<< " next reset time = " << timeStr << END;
	if(nextResetTime > now)
	{
		return nextResetTime - now;
	}
	return 0;

	//加偏移
	//lastResetTime += PkWeekOffset;
	//now += PkWeekOffset;

	//取Begin时间
	//lastResetTime = GameTime::GetWeekBeginTime(lastResetTime, false);

	//多长时间重置一次
	//UINT32 NWeekTime = PkWeekResetNum * (XCommon::GetOneDayTime() * 7);

	//if(lastResetTime + NWeekTime > now)
	//{
	//	return lastResetTime + NWeekTime - now;
	//}

	//return 0;
}

std::string GlobalConfig::GetCoverName(const std::string &name,UINT32 nType)
{
	std::string szTmp = "";
	if (nType == KKSG::MarriagePos_Wife)
	{
		szTmp = name;
		szTmp += WeddingWife;
	}else if (nType == KKSG::MarriagePos_Husband)
	{
		szTmp = name;
		szTmp += WeddingHusband;
	}
	return szTmp;
}

bool GlobalConfig::InitPk()
{
	PkTime = ConvertToSequence<UINT32, 5>(KeyValue["PkTime"], '=');
	PkDayReward = ConvertToSequence<UINT32, 3>(KeyValue["PkDayReward"], '|');
	PkSceneId = SafeConvert<int>(KeyValue["PkSceneId"]);
	PkPrepareTime = SafeConvert<int>(KeyValue["PkPrepareTime"]);
	PkUpdateTime = SafeConvert<int>(KeyValue["PkUpdateTime"]);
	PkTwoUpdateTime = GetInt("PkTwoUpdateTime", 150);
	PkMatchDelay = SafeConvert<UINT32>(KeyValue["PkMatchDelay"]);
	PkAddRobotPoint = ConvertToSequence<UINT32,2>(KeyValue["PkAddRobot"],'=');
	PkMatchToWorldTime = SafeConvert<UINT32>(KeyValue["PkMatchToWorldTime"]);
	PkWeekOffset = 4 * 3600;
	PkWeekResetNum = SafeConvert<UINT32>(KeyValue["PkWeekResetNum"]);
	PkBasePoint = SafeConvert<int>(KeyValue["PkBasePoint"]);

	PkResetPoint.clear();
	std::vector<std::string> vec;
	vec = Split(KeyValue["PkResetPoint"], "|");
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PkResetPoint.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}

	PkLoadWaitTime = SafeConvert<UINT32>(KeyValue["PkLoadWaitTime"]);
	PkTwoMatchToWorldTime = SafeConvert<UINT32>(KeyValue["PkTwoMatchToWorldTime"]);
	PkRoleMatchPairDayLimit = SafeConvert<UINT32>(KeyValue["PkRoleMatchPairDayLimit"]);
	PkRoleMatchPairWeekLimit = SafeConvert<UINT32>(KeyValue["PkRoleMatchPairWeekLimit"]);
	std::string tstr = KeyValue["PkWeekResetBaseTime"];
	SSInfo << " PkWeekResetBaseTime Config = " << tstr << END;
	time_t t = XCommon::ParseTime(tstr);
	if(0 == t)
	{
		t = XCommon::ParseTimeBackSlash(tstr);
	}
	if(0 == t)
	{
		return false;
	}
	PkWeekResetBaseTime = UINT32(t);
	SSInfo << " PkWeekResetBaseTime sec = " << PkWeekResetBaseTime << END;

	PkMatchLevelRange.clear();
	vec = Split(KeyValue["PkMatchPointRange"], '|');
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PkMatchLevelRange.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}

	return true;
}

bool GlobalConfig::InitPvp()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["PVPDayRewards"], "|");
	PVPDayRewards.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PVPDayRewards.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["PVPWeekRewards"], "|");
	PVPWeekRewards.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PVPWeekRewards.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["PVPWinRewards"], "|");
	PVPWinRewards.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PVPWinRewards.push_back(ConvertToSequence<UINT32,2>(vec[i], '='));
	}
	std::string pvpMS = KeyValue["PVPMatchScore"];
	if(!pvpMS.empty())
	{
		PVPMatchScore = ConvertToSequence<UINT32,3>(pvpMS,'|');
	}
	PVPLeaderBuff = ConvertToSequence<int,2>(KeyValue["PVPLeaderBuff"], '|');
	PVPQuitFailQuitCount = SafeConvert<UINT32>(KeyValue["PVPQuitFailQuitCount"]);
	PVPMatchWaitTime = SafeConvert<UINT32>(KeyValue["PVPMatchWaitTime"]);
	PVPMatchWorldTime = SafeConvert<UINT32>(KeyValue["PvpMatchWorldTime"]);
	PVPGameCount = SafeConvert<int>(KeyValue["PVPGameCount"]);
	PVPGameTimeDown = SafeConvert<UINT32>(KeyValue["PVPTimeDown"]);
	PVPGameTimeConti = SafeConvert<UINT32>(KeyValue["PVPGameTime"]);
	PVPWeekReqCount = SafeConvert<int>(KeyValue["PVPWeekReqCount"]);
	PVPDayReqCount = SafeConvert<int>(KeyValue["PVPDayReqCount"]);
	PVPDieReviveTime = SafeConvert<UINT32>(KeyValue["PVPDieReviveTime"]);
	PVPLeaderKillCount = SafeConvert<int>(KeyValue["PVPLeaderKillCount"]);
	PVPRoleNum = GetInt("PVPRoleNum", 4);
	PVPLoadWaitTime = SafeConvert<UINT32>(KeyValue["PVPLoadWaitTime"]);
	return true;
}

bool GlobalConfig::InitGmf()
{
	GMFOneFightTime = SafeConvert<UINT32>(KeyValue["GuildArenaLimitTime"]); 
	GMFFightAfterTime = 3;
	GMFBuff = ConvertToSequence<int,2>(KeyValue["GMFBuff"], '|');
	GMFKickCoolDown = SafeConvert<UINT32>(KeyValue["GMFKickCoolDown"]);
	GMFReadyTime = SafeConvert<UINT32>(KeyValue["GMFReadyTime"]);
	GMFInspireCoolDown = SafeConvert<UINT32>(KeyValue["GMFInspireCoolDown"]);
	GPROneFightTime = SafeConvert<UINT32>(KeyValue["GPROneFightTime"]); 
	GPRFightAfterTime = SafeConvert<UINT32>(KeyValue["GPRFightAfterTime"]);
	GPRReviveTime = SafeConvert<UINT32>(KeyValue["GPRReviveTime"]);
	std::vector<INT32> GMFVecTmp = ConvertToVector<INT32>(KeyValue["GuildArenaInspireCount"], '|');
	GMFInspireCountConf.clear();
	for(size_t i = 0; i < GMFVecTmp.size(); ++i)
	{
		GMFInspireCountConf.insert(GMFVecTmp[i]);
	}
	return true;
}

bool GlobalConfig::InitCastle()
{
	std::vector<std::string> vec;
	GuildCastleReadyTime = SafeConvert<UINT32>(KeyValue["GuildCastleReadyTime"]);
	GuildCastleFightTime = SafeConvert<UINT32>(KeyValue["GuildCastleFightTime"]);
	GuildCastleBattleRoleMax = SafeConvert<UINT32>(KeyValue["GuildCastleBattleRoleMax"]);
	GuildCastleAddPointInterval = SafeConvert<UINT32>(KeyValue["GuildCastleAddPointInterval"]);
	GuildCastleAddPointCount = SafeConvert<UINT32>(KeyValue["GuildCastleAddPointCount"]);
	GuildCastleEndPoint = SafeConvert<UINT32>(KeyValue["GuildCastleEndPoint"]);
	GuildCastleJvDianOccupyTime = SafeConvert<UINT32>(KeyValue["GuildCastleJvDianOccupyTime"]);
	GuildCastleJvDianPos.clear();
	GuildCastleJvDianPos.push_back(ConvertToSequence<float,3>(KeyValue["GuildTerritoryUpPos"],'|'));
	GuildCastleJvDianPos.push_back(ConvertToSequence<float,3>(KeyValue["GuildTerritoryMidPos"],'|'));
	GuildCastleJvDianPos.push_back(ConvertToSequence<float,3>(KeyValue["GuildTerritoryBtmPos"],'|'));
	GuildCastleJvDianRadius.clear();
	GuildCastleJvDianRadius.push_back(SafeConvert<float>(KeyValue["TerritoryBattleUpRadius"]));
	GuildCastleJvDianRadius.push_back(SafeConvert<float>(KeyValue["TerritoryBattleMidRadius"]));
	GuildCastleJvDianRadius.push_back(SafeConvert<float>(KeyValue["TerritoryBattleBtmRadius"]));
	vec.clear();
	vec = Split(KeyValue["GuildCastleScenes"], '|');
	GuildCastleScenes.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		UINT32 mapID = SafeConvert<UINT32>(vec[i]);
		GuildCastleScenes.push_back(mapID);
	}
	vec.clear();
	vec = Split(KeyValue["GuildCastleMulPoint"], '|');
	GuildCastleMulPoint.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildCastleMulPoint.push_back(SafeConvert<UINT32>(vec[i]));
	}
	GuildCastleFeatsRules =  ConvertToVector<UINT32>(KeyValue["GuildCastleFeatsRules"], '|');
	return true;
}

bool GlobalConfig::InitCustomBattle()
{
	CustomBattleReqCD = GetInt("CustomBattleReqCD", 3);
	CustomBattleCreateCD = GetInt("CustomBattleCreateCD", 30);
	CustomBattleOpen = GetInt("CustomBattleOpen", 1) > 0;
	CustomBattleRankLimit = GetInt("CustomBattleRankLimit", 200);

	CustomBattleRandomCount = GetInt("CustomBattleRandomCount", 20);
	CustomBattleRandomMax = GetInt("CustomBattleRandomMax", 5000);

	CustomBattleNameLimit1 = GetInt("CustomBattleNameLimit1", 100);
	CustomBattleNameMin = GetInt("CustomBattleNameMin", 2);

	CustomBattleExpireTime = GetInt("CustomBattleExpireTime", 3*24*3600);
	CustomBattleMatchExpireTime = GetInt("CustomBattleMatchExpireTime", 60);
	CustomBattleMatchExpandTime = GetInt("CustomBattleMatchExpandTime", 6);
	CustomBattleMatchExpandMax = GetInt("CustomBattleMatchExpandMax", 4);
	IsOpenCustomBattleCross = GetInt("IsOpenCustomBattleCross", 1) > 0;
	IsCustomBattleOnlyMatchCross = GetInt("IsCustomBattleOnlyMatchCross", 0) > 0;

	CustomBattleWorldUpdate = GetInt("CustomBattleWorldUpdate", 1000);
	CustomBattleWorldUpdateLimit = GetInt("CustomBattleWorldUpdateLimit", 100);

	CustomBattleMsUpdate = GetInt("CustomBattleMsUpdate", 1000);
	CustomBattleMsUpdateLimit = GetInt("CustomBattleMsUpdateLimit", 100);
	CustomBattleMsRoleUpdate = GetInt("CustomBattleMsRoleUpdate", 1000);
	CustomBattleMsRoleUpdateLimit = GetInt("CustomBattleMsRoleUpdate", 100);

	CustomBattleMatcherUpdate = GetInt("CustomBattleMatcherUpdate", 1000);
	CustomBattleMatcherLimit = GetInt("CustomBattleMatcherLimit", 500);;
	CustomBattleMatcherSystemLimit = GetInt("CustomBattleMatcherSystemLimit", 500);

	CustomBattleMatcherSystemTime = GetInt("CustomBattleMatcherSystemTime", 10002300);

	CustomBattleMatcherCount = GetInt("CustomBattleMatcherCount", 3);
	CustomBattleMatcherCD = GetInt("CustomBattleMatcherCD", 3600);
	CustomBattleMatcherPoint = GetInt("CustomBattleMatcherPoint", 1700);

	CustomBattleSystemJoin = GetInt("CustomBattleSystemJoin", 1) > 0; 
	CustomBattleCreate = GetInt("CustomBattleCreate", 1) > 0;

	return true;
}

bool GlobalConfig::InitInvFight()
{
	InvFightTimeOver = SafeConvert<UINT32>(KeyValue["InvFightTimeOver"]);
	InvFightFromCount = SafeConvert<UINT32>(KeyValue["InvFightFromCount"]);
	InvFightFightTime = SafeConvert<UINT32>(KeyValue["InvFightFightTime"]);
	InvFightRoleLevel = SafeConvert<UINT32>(KeyValue["InvFightRoleLevel"]);
	InvFightMapID = SafeConvert<UINT32>(KeyValue["InvFightMapID"]);
	return true;
}

bool GlobalConfig::InitDEBossRush()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["DERankRewardTime"], "|");
	DERankRewardDay.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		DERankRewardDay.insert(SafeConvert<UINT32>(vec[i]));
	}
	DEGroupRoleCount = SafeConvert<int>(KeyValue["DEGroupRoleCount"]);
	DETongGuanReward = ConvertToSequence<UINT32,2>(KeyValue["DETongGuanReward"], '=');

	vec.clear();
	vec = Split(KeyValue["BossRushRefreshCost"], '|');
	BossRushCost.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		BossRushCost.push_back(SafeConvert<UINT32>(vec[i]));
	}
	if(8 != (int)BossRushCost.size())//个数固定
	{
		return false;
	}
	return true;
}

bool GlobalConfig::InitFriend()
{
	GiftSendMinDegree = SafeConvert<UINT32>(KeyValue["FriendSystemSendGiftMinDegree"]);
	GiftSendMaxTimes = SafeConvert<UINT32>(KeyValue["FriendSystemSendGifMaxTimes"]);
	GiftReceiveMaxTimes = SafeConvert<UINT32>(KeyValue["FriendSystemAcceptGifMaxTimes"]);
	FriendMaxDegree= SafeConvert<UINT32>(KeyValue["FriendSystemMaxFriendlyEvaluation"]);
	FriendMaxCount = SafeConvert<UINT32>(KeyValue["FriendMaxCount"]);
	ApplyMaxCount = SafeConvert<UINT32>(KeyValue["ApplyMaxCount"]);
	BlackMaxCount = SafeConvert<UINT32>(KeyValue["BlackMaxCount"]);
	FriendSystemRefreshAddListCD = SafeConvert<UINT32>(KeyValue["FriendSystemRefreshAddListCD"]);

	FriendExpireTime = GetInt("FriendExpireTime", 15*24*3600);
	FriendExpireLevel = GetInt("FriendExpireLevel", 20);

	FriendUpdateCount = GetInt("FriendUpdateCount", 100);

	return true;
}

bool GlobalConfig::InitMail()
{
	SendGold_MailTitle = KeyValue["SendGoldMailTitle"];
	SendGold_MailContent = KeyValue["SendGoldMailContent"];
	SendDiamond_MailTitle = KeyValue["SendDiamondMailTitle"];
	SendDiamond_MailContent = KeyValue["SendDiamondMailContent"];
	SendFlower_MailTitle = KeyValue["SendFlowerMailTitle"];
	SendFlower_MailContent = KeyValue["SendFlowerMailContent"];
	FlowerWeekRank_MailTitle = KeyValue["FlowerWeekRankMailTitle"];
	FlowerWeekRank_MailContent = KeyValue["FlowerWeekRankMailContent"];
	FlowerValentinesDayRank_MailTitle = KeyValue["FlowerValentinesDayRank_MailTitle"];
	FlowerValentinesDayRank_MailContent = KeyValue["FlowerValentinesDayRank_MailContent"];
	Flower520DayRank_MailTitle = KeyValue["Flower520DayRank_MailTitle"];
	Flower520DayRank_MailContent = KeyValue["Flower520DayRank_MailContent"];
	FlowerActivityDayRank_MailTitle = KeyValue["FlowerActivityDayRank_MailTitle"];
	FlowerActivityDayRank_MailContent = KeyValue["FlowerActivityDayRank_MailContent"];
	MentorTask_MailMasterTitle = KeyValue["MentorTask_RewardMasterTitle"];
	MentorTask_MailMasterContent = KeyValue["MentorTask_RewardMasterContent"];
	MentorTask_MailStudentTitle = KeyValue["MentorTask_RewardStudentTitle"];
	MentorTask_MailStudentContent = KeyValue["MentorTask_RewardStudentContent"];
	Mentor_CompleteRewardTitle = KeyValue["Mentor_CompleteRewardTitle"];
	Mentor_CompleteRewardContent = KeyValue["Mentor_CompleteRewardContent"];
	MentorBreakTitle = KeyValue["MentorBreakTitle"];
	MentorBreakContent = KeyValue["MentorBreakContent"];
	DesignationFlowerMailTitle = KeyValue["DesignationFlowerMailTitle"];
	DesignationSendFlowerMailContent = KeyValue["DesignationSendFlowerMailContent"];
	GuildArenaPointRaceWinTitle = KeyValue["GuildArenaPointRaceWinTitle"];
	GuildArenaPointRaceWinContent = KeyValue["GuildArenaPointRaceWinContent"];
	GuildArenaPointRaceLoseTitle = KeyValue["GuildArenaPointRaceLoseTitle"];
	GuildArenaPointRaceLoseContent = KeyValue["GuildArenaPointRaceLoseContent"];
	GuildCampRankMailTitle = KeyValue["GuildCampRankMailTitle"];
	GuildCampRankMailContent = KeyValue["GuildCampRankMailContent"];

	m_dwRoleMailCacheMaxNum = GetInt("RoleMailCacheMaxNum", 50000);

	return true;
}

bool GlobalConfig::InitArena()
{
	ArenaNpcCount = SafeConvert<int>(KeyValue["ArenaNpcCount"]);
	ArenaRankBegin = SafeConvert<int>(KeyValue["ArenaRankBegin"]);
	ArenaScale = SafeConvert<int>(KeyValue["ArenaScale"]);

	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["ArenaFighters"], "|");
	ArenaFighters.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		ArenaFighters.push_back(ConvertToSequence<int, 2>(vec[i], '='));
	}

	return true;
}

bool GlobalConfig::InitCard()
{
	CardMatchWaiting = SafeConvert<UINT32>(KeyValue["CardMatchWaiting"]);
	CardMatchRoundWaiting = SafeConvert<UINT32>(KeyValue["CardMatchRoundWaiting"]);;
	CardMatchRounding = SafeConvert<UINT32>(KeyValue["CardMatchRounding"]);;
	CardMatchRound = SafeConvert<UINT32>(KeyValue["CardMatchRound"]);;
	CardMatchChange = SafeConvert<UINT32>(KeyValue["CardMatchChange"]);;
	CardMatchChangeAdd = SafeConvert<UINT32>(KeyValue["CardMatchChangeAdd"]);;

	return true;
}

bool GlobalConfig::InitCombat()
{
	StunTime = SafeConvert<float>(KeyValue["StunTime"]);
	TurnSpeed = SafeConvert<double>(KeyValue["TurnSpeed"]);
	DashSpeed = SafeConvert<double>(KeyValue["DashSpeed"]);
	WalkSpeed = SafeConvert<double>(KeyValue["WalkSpeed"]);
	RunSpeed = SafeConvert<double>(KeyValue["RunSpeed"]);
	AutoTurnSpeed = SafeConvert<double>(KeyValue["AutoTurnSpeed"]);
	AttackSpeed = SafeConvert<double>(KeyValue["AttackSpeed"]);
	SkillCD = SafeConvert<double>(KeyValue["SkillCD"]);
	ElemDefLimit = SafeConvert<double>(KeyValue["ElemDefLimit"]);
	ElemAtkLimit = SafeConvert<double>(KeyValue["ElemAtkLimit"]);
	GeneralCombatParam = SafeConvert<double>(KeyValue["GeneralCombatParam"]);
	CritDamageBase = SafeConvert<double>(KeyValue["CritDamageBase"]);
	FinalDamageLimit = SafeConvert<double>(KeyValue["FinalDamageLimit"]);
	StunResistLimit = SafeConvert<double>(KeyValue["StunResistLimit"]);
	StunLimit = SafeConvert<double>(KeyValue["StunLimit"]);
	ParaResistLimit = SafeConvert<double>(KeyValue["ParaResistLimit"]);
	ParalyzeLimit = SafeConvert<double>(KeyValue["ParalyzeLimit"]);
	CritResistLimit = SafeConvert<double>(KeyValue["CritResistLimit"]);
	CriticalLimit = SafeConvert<double>(KeyValue["CriticalLimit"]);
	MagicAvoidenceLimit = SafeConvert<double>(KeyValue["MagicAvoidenceLimit"]);
	PhycialAvoidenceLimit = SafeConvert<double>(KeyValue["PhycialAvoidenceLimit"]);
	AttrChangeDamageLimit = SafeConvert<double>(KeyValue["AttrChangeDamageLimit"]);

	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["CDChangeLimit"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	CDChangeLowerBound = SafeConvert<float>(vec[0]);
	CDChangeUpperBound = SafeConvert<float>(vec[1]);

	vec = Split(KeyValue["AttackSpeedLimit"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	AttackSpeedLowerBound = SafeConvert<double>(vec[0]);
	AttackSpeedUpperBound = SafeConvert<double>(vec[1]);

	vec = Split(KeyValue["BuffChangeCastDamageLimit"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	BuffCastDamageChangeLowerBound = SafeConvert<double>(vec[0]) - 1.0l;
	BuffCastDamageChangeUpperBound = SafeConvert<double>(vec[1]) - 1.0l;

	vec = Split(KeyValue["BuffChangeReceivedDamageLimit"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	BuffReceivedDamageChangeLowerBound = SafeConvert<double>(vec[0]) - 1.0l;
	BuffReceivedDamageChangeUpperBound = SafeConvert<double>(vec[1]) - 1.0l;

	vec = Split(KeyValue["MpCostChangeLimit"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	MpCostChangeLowerBound = SafeConvert<double>(vec[0]) - 1.0l;
	MpCostChangeUpperBound = SafeConvert<double>(vec[1]) - 1.0l;

	vec = Split(KeyValue["CritDamageLimit"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	CritDamageLowerBound = SafeConvert<double>(vec[0]);
	CritDamageUpperBound = SafeConvert<double>(vec[1]);

	vec = Split(KeyValue["DamageRandomRange"], '=');
	CHECK_COND_RETURN(vec.size() == 2, false);
	DamageRandomLowerBound = SafeConvert<double>(vec[0]);
	DamageRandomUpperBound = SafeConvert<double>(vec[1]);

	Hit_PresentStraight = SafeConvert<float>(KeyValue["PresentStraight"]);
	Hit_HardStraight = SafeConvert<float>(KeyValue["HardStraight"]);
	Hit_Offset = SafeConvert<float>(KeyValue["Offset"]);
	Hit_Height = SafeConvert<float>(KeyValue["Height"]);

	InitEnmityValue = SafeConvert<float>(KeyValue["InitEnmityValue"]);
	ReviveBuff = ConvertToSequence<int, 2>(KeyValue["ReviveBuff"], '=');

	vec.clear();
	vec = Split(KeyValue["ComboBuff"], '|');
	ComboBuff.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		ComboBuff.push_back(ConvertToSequence<int, 3>(vec[i], '='));
	}
	ComboInterval = (UINT32)(SafeConvert<float>(KeyValue["ComboInterval"]) * 1000);

	vec.clear();
	vec = Split(KeyValue["EntitySummonGroupLimit"], '|');
	EntitySummonGroupLimit.clear();
	EntitySummonGroupLimit.push_back(0);
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		EntitySummonGroupLimit.push_back(SafeConvert<int>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["PowPointAdjust"], '|');
	PowPointAdjust.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		PowPointAdjust.push_back(SafeConvert<UINT32>(vec[i]));
	}
	PowPointBaseValue = SafeConvert<float>(KeyValue["PowPointBaseValue"]);
	PowPointMaxPoint =  SafeConvert<float>(KeyValue["PowPointMaxPoint"]);

	BuffMinAuraInterval = SafeConvert<float>(KeyValue["BuffMinAuraInterval"]);
	CHECK_COND_RETURN(BuffMinAuraInterval > 0.001f, false);
	BuffMinRegenerateInterval = SafeConvert<float>(KeyValue["BuffMinRegenerateInterval"]);
	CHECK_COND_RETURN(BuffMinRegenerateInterval > 0.001f, false);

	SkillReviveHpPer = GetFloat("SkillReviveHpPer", 0.1f);
	return true;
}

bool GlobalConfig::InitCommon()
{
	InitialReturnSceneID = SafeConvert<int>(KeyValue["InitialReturnSceneID"]);
	MaxFatigue = SafeConvert<int>(KeyValue["MaxFatigue"]);
	MaxRecoverFatigue = SafeConvert<int>(KeyValue["MaxRecoverFatigue"]);
	FatigueRegeneration = SafeConvert<int>(KeyValue["FatigueRegeneration"]);
	RoleSaveInterval = SafeConvert<int>(KeyValue["RoleSaveInterval"]);
	RoleSummarySaveInterval = SafeConvert<int>(KeyValue["RoleSummarySaveInterval"]);
	MaxRole = GetInt("MaxRole", 4);

	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["MpRecovery"], '|');
	MpRecovery.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		MpRecovery.push_back(SafeConvert<int>(*i));
	}
	InitialSceneID = SafeConvert<int>(KeyValue["InitialSceneID"]);
	BraveReturnServerSceneID = GetInt("BraveReturnServerSceneID", InitialSceneID);
	MailMax = SafeConvert<UINT32>(KeyValue["MailMax"]);
	ArenaSceneID = SafeConvert<int>(KeyValue["ArenaSceneID"]);
	SkillPointPPTWeight = SafeConvert<int>(KeyValue["SkillPointPPTWeight"]);
	DailyFlowerCount = SafeConvert<int>(KeyValue["DailyFlowerCount"]);

	SkillPageNewOpen = GetInt("SkillPageNewOpen", 30);
	ProfessionChangeCount = GetInt("ProfessionChangeCount", 100);
	ProfessionChangeLevel = GetInt("ProfessionChangeLevel", 50);
	ProfessionChangeTime = GetInt("ProfessionChangeTime", 10);
	ProfessionChangeSkill = (GetInt("ProfessionChangeSkill", 1) > 0);
	ProfessionChangeUseItem = ConvertToSequence<UINT32, 2>(KeyValue["ProfessionChangeUseItem"], '=');

	vec.clear();
	vec = Split(KeyValue["ProfessionChangeShop"], '|');
	ProfessionChangeShop.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		ProfessionChangeShop.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["LevelChangeShop"], '|');
	LevelChangeShop.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		LevelChangeShop.push_back(SafeConvert<UINT32>(vec[i]));
	}

	DinnerTime = ConvertToSequence<int, 4>(KeyValue["DinnerTime"], '|');
	SupperTime = ConvertToSequence<int, 4>(KeyValue["SupperTime"], '|');

	vec.clear();
	vec = Split(KeyValue["DinnerReward"], '|');
	DinnerReward.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		DinnerReward.push_back(ConvertToSequence<int, 2>(*i, '='));
	}
	vec.clear();
	vec = Split(KeyValue["SupperReward"], '|');
	SupperReward.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		SupperReward.push_back(ConvertToSequence<int, 2>(*i, '='));
	}

	vec.clear();
	vec = Split(KeyValue["LoginReward"], "|");
	LoginRewards.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32,3> seq = ConvertToSequence<UINT32, 3>(vec[i], '=');
		LoginRewards.push_back(seq);
	}
	LoginRewardOpenLevel = SafeConvert<INT32>(KeyValue["LoginRewardOpenLevel"]);

	RoleNameLenMin = SafeConvert<UINT32>(KeyValue["RoleNameLenMin"]);
	RoleNameLenMax = SafeConvert<UINT32>(KeyValue["RoleNameLenMax"]);

	SecondPromoteSecneID = GetInt("SecondPromoteSecneID", 22);

	vec.clear();
	vec = Split(KeyValue["SkillSlotUnlockLevel"], '|');
	SkillSlotUnlockLevel.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		SkillSlotUnlockLevel.push_back(SafeConvert<UINT32>(vec[i]));
	}

	LevelSealExpRate = SafeConvert<float>(KeyValue["LevelSealExpRate"]);

	RolesummaryExpireTime = GetInt("RoleSummaryExpireTime", 7*24*3600);

	return true;	
}

bool GlobalConfig::InitBoss()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["WorldBossGuildConsume"], '|');
	WorldBossGuildConsume.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		WorldBossGuildConsume.push_back(SafeConvert<UINT32>(*i));
	}

	vec.clear();
	vec = Split(KeyValue["WorldBossGuildAddAttr"], '|');
	WorldBossGuildAddAttr.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		WorldBossGuildAddAttr.push_back(ConvertToSequence<int, 2>(*i, '='));
	}

	vec.clear();
	vec = Split(KeyValue["WorldBossAddAttr"], '|');
	WorldBossAddAttr.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		WorldBossAddAttr.push_back(ConvertToSequence<int, 2>(*i, '='));
	}

	vec.clear();
	vec = Split(KeyValue["WorldBossKillReward"], '|');
	WorldBossKillReward.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		WorldBossKillReward.push_back(ConvertToSequence<int, 2>(*i, '='));
	}

	vec.clear();
	vec = Split(KeyValue["GuildBossAddAttr"], '|');
	GuildBossAddAttr.clear();
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		GuildBossAddAttr.push_back(ConvertToSequence<int, 2>(*i, '='));
	}
	WorldBossPoint = ConvertToSequence<int, 4>(KeyValue["WorldBossPoint"], '|');
	GuildBossPoint = ConvertToSequence<int, 4>(KeyValue["GuildBossPoint"], '|');

	WorldBossSceneLimit = SafeConvert<int>(KeyValue["WorldBossSceneLimit"]);
	WorldBossGameEndCountDown = SafeConvert<int>(KeyValue["WorldBossGameEndCountDown"]);

	WorldBossRoleLimit = GetInt("WorldBossRoleLimit", 4000);
	WorldBossPlayTime = GetInt("WorldBossPlayTime", 240);
	WorldBossPlayCount = GetInt("WorldBossPlayCount", 1);
	WorldBossGuildAttrCount = GetInt("WorldBossGuildAttrCount", 10);
	WorldBossAttrCount = GetInt("WorldBossAttrCount", 10);
	WorldBossCountDown = GetInt("WorldBossCountDown", 6);

	WorldBossTimeEnd = GetInt("WorldBossTimeEnd", 240);
	WorldBossSyncTime = GetInt("WorldBossSyncTime", 1000);

	return true;	
}

bool GlobalConfig::InitSceneView()
{
	ViewGridCount = SafeConvert<int>(KeyValue["ViewGridCount"]);
	CHECK_COND_RETURN(ViewGridCount <= 20, false);
	ViewGridLen = SafeConvert<int>(KeyValue["ViewGridLen"]);
	ViewGridFriendDegree = SafeConvert<UINT32>(KeyValue["ViewGridFriendDegree"]);
	ViewGridSec = SafeConvert<int>(KeyValue["ViewGridSec"]);
	CHECK_COND_RETURN(ViewGridSec >= 2, false);
	ViewGridCanBeSee = SafeConvert<UINT32>(KeyValue["ViewGridCanBeSee"]);
	ViewGridOnceLimit = (UINT32)GetInt("ViewGridOnceLimit", 5);
	ViewGridTryLimit = (UINT32)GetInt("ViewGridTryLimit", 100);
	SceneWaitTimeout = (UINT32)GetInt("SceneWaitTimeout", 5);

	SceneGroupUpdate = (UINT32)GetInt("SceneGroupUpdate", 500);

	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["ViewGridScene"], '|');
	ViewGridScene.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		ViewGridScene.insert(SafeConvert<int>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["PVPOne"], '|');
	PVPOne.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		PVPOne.insert(SafeConvert<int>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["CanEnterScene"], '|');
	CanEnterScene.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		CanEnterScene.insert(SafeConvert<int>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["DragonSceneCanReviveCD"], '|');
	DragonSceneCanReviveCD.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		DragonSceneCanReviveCD.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["DragonSceneReviveExcept"], "|");
	DragonSceneReviveExcept.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		DragonSceneReviveExcept.insert(SafeConvert<int>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["ViewGroupScene"], '|');
	ViewGroupScene.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		ViewGroupScene.insert(SafeConvert<int>(vec[i]));
	}
	ViewGroupLen = GetInt("ViewGroupLen", 3);

	return true;
}

bool GlobalConfig::InitGoldClick()
{
	GoldClickFreeLimit = SafeConvert<UINT32>(KeyValue["GoldClickFreeLimit"]);
	GoldClickFreeTime = SafeConvert<UINT32>(KeyValue["GoldClickFreeTime"]);
	GoldClickBaseCount = SafeConvert<UINT32>(KeyValue["GoldClickBaseCount"]);
	GoldClickConsume = ConvertToSequence<UINT32, 2>(KeyValue["GoldClickConsume"], '=');

	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["GoldClickCount"], '|');
	GoldClickCount.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GoldClickCount.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	return true;
}

bool GlobalConfig::InitItem()
{
	AttributeEmblemSlotLevelLimit = ConvertToVector<int>(KeyValue["AttributeEmblemSlotLevelLimit"], '|');
	SkillEmblemSlotLevelLimit = ConvertToVector<int>(KeyValue["SkillEmblemSlotLevelLimit"], '|');
	ExtraSkillEmblemSlotLevelLimit = ConvertToVector<int>(KeyValue["ExtraSkillEmblemSlotLevelLimit"], '|');
	ExtraSkillEmblemSlotDragonCoin = ConvertToVector<int>(KeyValue["ExtraSkillEmblemSlotDragonCoin"], '|');

	if (ExtraSkillEmblemSlotLevelLimit.size() != ExtraSkillEmblemSlotDragonCoin.size())
	{
		LogError("ExtraSkillEmblemSlotLevelLimit.size() != ExtraSkillEmblemSlotDragonCoin.size()");
		return false;
	}

	JadeMosaicLevel = ConvertToVector<int>(KeyValue["JadeMosaicLevel"], '|');
	JadeLevelUpCost = ConvertToVector<UINT32>(KeyValue["JadeLevelUpCost"], '|');

	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["DecomposeRandom"], '|');
	if (2 == vec.size())
	{
		DecomposeRandom = SafeConvert<int>(vec[0]);
		DecomposeParam = SafeConvert<float>(vec[1]);
	}
	vec.clear();
	vec = Split(KeyValue["EnhanceTransRate"], '|');
	EnhanceTransRate.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		EnhanceTransRate.push_back(ConvertToSequence<float, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["SmeltCorlorRange"], '|');
	SmeltCorlorRange.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		SmeltCorlorRange.push_back(SafeConvert<UINT32>(vec[i]));
	}

	ForgeSmeltTransRate = SafeConvert<UINT32>(KeyValue["ForgeSmeltTransRate"]);

	vec.clear();
	vec = Split(KeyValue["EquipAttackSmeltExchanged"], '|');
	EquipAttackSmeltExchanged.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		EquipAttackSmeltExchanged.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["EquipDefenseSmeltExchanged"], '|');
	EquipDefenseSmeltExchanged.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		EquipDefenseSmeltExchanged.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["EmblemSmeltExchanged"], '|');
	EmblemSmeltExchanged.clear();
	for (UINT32 i = 0; i < vec.size(); ++i)
	{
		EmblemSmeltExchanged.push_back(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["ChatItemCost"], '|');
	blueBirdPrice = 0; 
	if(vec.size() > 0)
	{
		Sequence<UINT32, 2> seq = ConvertToSequence<UINT32, 2>(vec[0], '=');
		blueBirdPrice = seq[1];
	}	

	vec.clear();
	vec = Split(KeyValue["EnhanceNotice1"], '|');
	EnhanceNotice1.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		EnhanceNotice1.insert(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["EnhanceNotice2"], '|');
	EnhanceNotice2.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		EnhanceNotice2.insert(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["EnhanceNotice3"], '|');
	EnhanceNotice3.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		EnhanceNotice3.insert(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["ExchangeRate"], '|');
	ExchangeRate.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		ExchangeRate.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}
	return true;
}

bool GlobalConfig::InitAuct()
{
	std::vector<std::string> vec;
	vec.clear();
	// auction
	AuctOnSaleTime = (SafeConvert<UINT32>(KeyValue["AuctOnSaleDay"]))*(UINT32)XCommon::GetOneDayTime();
	vec.clear();
	vec = Split(KeyValue["AuctSaleDelayMin"], "=");
	if(2 == (int)vec.size())
	{
		AuctSaleDelayMin = SafeConvert<UINT32>(vec[0]) * 60;
		AuctSaleDelayMax = SafeConvert<UINT32>(vec[1]) * 60;
	}
	AuctRecoverTime = 30 * 24 * 3600;//(SafeConvert<UINT32>(KeyValue["AuctRecoverT"]))*(UINT32)XCommon::GetOneDayTime();
	AuctCartItemAllCount = SafeConvert<UINT32>(KeyValue["AuctCartItemAllCount"]);
	AuctCartItemTreasurePercent = SafeConvert<UINT32>(KeyValue["AuctCartItemTreasurePercent"]);
	AuctCartItemTreasureCount = AuctCartItemTreasurePercent * AuctCartItemAllCount / 100;
	AuctCartItemNormalCount = AuctCartItemAllCount - AuctCartItemTreasureCount;
	AuctWeekTradeLimit = SafeConvert<UINT32>(KeyValue["AuctWeekTradeLimit"]);
	AuctTradeRecordTime = SafeConvert<UINT32>(KeyValue["AuctTradeRecordDay"]) * (UINT32)XCommon::GetOneDayTime();
	AuctAVGPricePreHigh = SafeConvert<UINT32>(KeyValue["AuctAVGPricePreHigh"]);
	AuctAVGPricePreLow = SafeConvert<UINT32>(KeyValue["AuctAVGPricePreLow"]);
	AuctFreeRefreshCount = SafeConvert<UINT32>(KeyValue["AuctFreeRefreshCount"]);
	AuctAutoRefreshTime = SafeConvert<UINT32>(KeyValue["AuctAutoRefreshTime"]) * 60;
	AuctFreeRefreshTime = SafeConvert<UINT32>(KeyValue["AuctFreeRefreshTime"]) * 60;

	AuctRevenueRate = SafeConvert<UINT32>(KeyValue["AuctRevenueRate"]);
	AuctRefreshPayCount = SafeConvert<UINT32>(KeyValue["AuctRefreshPay"]);
	AuctNormalItemCoolDownDay = SafeConvert<UINT32>(KeyValue["AuctNormalItemCoolDownDay"]);
	AuctTreasureItemCoolDownDay = SafeConvert<UINT32>(KeyValue["AuctTreasureItemCoolDownDay"]);
	AuctSupplementTime = ConvertToSequence<UINT32, 2>(KeyValue["AuctSupplementTime"], '=');
	AuctSupplementFrequency = SafeConvert<UINT32>(KeyValue["AuctSupplementFrequency"]);
	vec.clear();
	vec = Split(KeyValue["AuctRefreshTimeSegment"], '|');
	AuctRefreshTimeSegment.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32,4> seq = ConvertToSequence<UINT32,4>(vec[i],'=');
		AuctRefreshTimeSegment.push_back(seq);
	}

	vec.clear();
	vec = Split(KeyValue["AuctItemCoolDownDay"], '|');
	AuctItemCoolDownDay.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32,3> seq = ConvertToSequence<UINT32,3>(vec[i],'=');
		AuctItemCoolDownDay.push_back(seq);
	}	

	vec.clear();
	vec = Split(KeyValue["AuctItemSpecialPush"], '|');
	AuctItemSpecialPush.clear();
	for (UINT32 i = 0; i < vec.size(); i++)
	{
		AuctItemSpecialPush.insert(SafeConvert<UINT32>(vec[i]));
	}

	AverageConsume7Day = SafeConvert<UINT32>(KeyValue["AverageConsume7Day"]);

	return true;
}

bool GlobalConfig::InitGuildAuct()
{
	std::vector<std::string> vec;
	vec.clear();
	///////////////////////////////////////////////////////////////////////////////////

	// guild auction
	GuildActLeaDay = SafeConvert<UINT32>(KeyValue["GuildActLeaDay"]);
	GuildActDelMax = SafeConvert<UINT32>(KeyValue["GuildActDelMax"]);
	GuildActDailyTitle = KeyValue["GuildActDailyTitle"];
	GuildActDailyCon = KeyValue["GuildActDailyCon"];
	GuildActWeekTitle = KeyValue["GuildActWeekTitle"];
	GuildActWeekCon = KeyValue["GuildActWeekCon"];
	GuildActDelTitle = KeyValue["GuildActDelTitle"];
	GuildActDelCon = KeyValue["GuildActDelCon"];
	GuildActLeaDownTitle = KeyValue["GuildActLeaDownTitle"];
	GuildActLeaDownCon = KeyValue["GuildActLeaDownCon"];
	GuildActLeaUpTitle = KeyValue["GuildActLeaUpTitle"];
	GuildActLeaUpCon = KeyValue["GuildActLeaUpCon"];
	vec.clear();
	vec = Split(KeyValue["GuildActLvl"], '|');
	GuildActLvl.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		GuildActLvl.push_back(SafeConvert<UINT32>(vec[i]));
	}

	GuildAuctGuildSaleTime = SafeConvert<UINT32>(KeyValue["GuildAuctGuildSaleTime"]);
	GuildAuctWorldSaleTime = SafeConvert<UINT32>(KeyValue["GuildAuctWorldSaleTime"]);
	GuildAuctUpRate = SafeConvert<UINT32>(KeyValue["GuildAuctUpRate"]);
	GuildAuctGuildDue = SafeConvert<UINT32>(KeyValue["GuildAuctGuildDue"]);
	GuildAuctWorldDue = SafeConvert<UINT32>(KeyValue["GuildAuctWorldDue"]);
	GuildAuctGuildItemPublicityTime = SafeConvert<UINT32>(KeyValue["GuildAuctGuildItemPublicityTime"]);
	GuildAuctItemDelayTime  = SafeConvert<UINT32>(KeyValue["GuildAuctItemDelayTime"]); 
	vec.clear();
	vec = Split(KeyValue["GuildAuctProfitMax"], '|');
	GuildAuctProfitMax.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32,2> seq = ConvertToSequence<UINT32,2>(vec[i],'=');
		GuildAuctProfitMax.push_back(seq);
	}
	vec.clear();
	vec = Split(KeyValue["GuildAuctGuildItemExRate"], '|');
	GuildAuctGuildItemExRate.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<float,2> seq = ConvertToSequence<float,2>(vec[i],'=');
		GuildAuctGuildItemExRate.push_back(seq);
	}
	GuildActRank = SafeConvert<UINT32>(KeyValue["GuildActRank"]);	

	return true;
}
bool GlobalConfig::InitGarden()
{
	std::vector<std::string> vec;
	vec.clear();
	PlantSpriteMaxTimes =SafeConvert<UINT32>(KeyValue["PlantSpriteMaxTimes"]);
	PlantSpriteCD=SafeConvert<UINT32>(KeyValue["PlantSpriteCD"]);
	GardenLogMax=SafeConvert<UINT32>(KeyValue["GardenLogMax"]);
	PlantSpriteCheckTimes =SafeConvert<UINT32>(KeyValue["PlantSpriteCheckTimes"]);

	vec.clear();
	vec = Split(KeyValue["PlantSpriteAppearTimeSection"], '|');
	PlantSpriteAppearTimeSection.clear();
	for(UINT32 i = 0; i < vec.size(); i++)
	{
		PlantSpriteAppearTimeSection.push_back(SafeConvert<UINT32>(vec[i]));
	}
	RefreshGardenTime =SafeConvert<UINT32>(KeyValue["RefreshGardenTime"]);
	GardenBanquetGuests =SafeConvert<UINT32>(KeyValue["GardenBanquetGuests"]);
	GardeLandDefault = SafeConvert<UINT32>(KeyValue["GardeLandDefault"]);
	GardenBanquetAwardTimesTSWK =  SafeConvert<UINT32>(KeyValue["GardenBanquetAwardTimesTSWK"]);
	// 植物干旱状态下的buffid
	PlantGrowDroughtBuff = SafeConvert<UINT32>(KeyValue["PlantGrowDroughtBuff"]);
	// 植物虫害状态下的buff
	PlantGrowPestBuff = SafeConvert<UINT32>(KeyValue["PlantGrowPestBuff"]); 
	// 植物萎靡状态下的buff
	PlantGrowSluggish = SafeConvert<UINT32>(KeyValue["PlantGrowSluggish"]);
	// 植物变正常的buff 
	PlantGrowNormalBuff = SafeConvert<UINT32>(KeyValue["PlantGrowNormalBuff"]);
	// 植物变变成熟的buff
	PlantGrowMatureBuff = SafeConvert<UINT32>(KeyValue["PlantGrowMatureBuff"]);
	FishingCameraDesdir = SafeConvert<float>(KeyValue["FishingCameraDesdir"]);

	HomeFeastDes1 = ConvertToSequence<float, 4>(KeyValue["HomeFeastDes1"], '|'); 
	HomeFeastDes2 = ConvertToSequence<float, 4>(KeyValue["HomeFeastDes2"], '|'); 
	HomeFeastDes3 = ConvertToSequence<float, 4>(KeyValue["HomeFeastDes3"], '|'); 
	HomeFeastDes4 = ConvertToSequence<float, 4>(KeyValue["HomeFeastDes4"], '|'); 

	HomeFeastEndDes1 = ConvertToSequence<float, 4>(KeyValue["HomeFeastEndDes1"], '|');  
	HomeFeastEndDes2 = ConvertToSequence<float, 4>(KeyValue["HomeFeastEndDes2"], '|');  
	HomeFeastEndDes3 = ConvertToSequence<float, 4>(KeyValue["HomeFeastEndDes3"], '|');  
	HomeFeastEndDes4 = ConvertToSequence<float, 4>(KeyValue["HomeFeastEndDes4"], '|');  

	vec.clear();
	vec = Split(KeyValue["BreakNewFarmlandCost"], '|');
	BreakNewFarmlandCost.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BreakNewFarmlandCost.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["BreakFarmlandLevel"], '|');
	BreakFarmlandLevel.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BreakFarmlandLevel.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["PlantSeedPosition"], '|');
	PlantSeedPosition.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		PlantSeedPosition.push_back(ConvertToSequence<float, 3>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["GuildPlantSeedPosition"], '|');
	GuildPlantSeedPosition.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		GuildPlantSeedPosition.push_back(ConvertToSequence<float, 3>(vec[i], '='));
	}	
	GardenStealCount = SafeConvert<UINT32>(KeyValue["GardenStealCount"]);
	vec.clear();
	vec = Split(KeyValue["BreakFarmlandLevel_Guild"], '|');
	BreakFarmlandLevel_Guild.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BreakFarmlandLevel_Guild.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	PlantReturn = SafeConvert<UINT32>(KeyValue["PlantReturn"]);

	return true;
}

bool GlobalConfig::InitPoker()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["PokerTournamentSignUpCost"], '|');
	PokerTournamentSignUpCost.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		PokerTournamentSignUpCost.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	PokerTournamentPlayNum = SafeConvert<UINT32>(KeyValue["PokerTournamentPlayNum"]);
	PokerTournamentSignUpNum = SafeConvert<UINT32>(KeyValue["PokerTournamentSignUpNum"]);

	PokerTournamentWaiting		= SafeConvert<UINT32>(KeyValue["PokerTournamentWaiting"]);
	PokerTournamentRoundWaiting = SafeConvert<UINT32>(KeyValue["PokerTournamentRoundWaiting"]);
	PokerTournamentRounding		= SafeConvert<UINT32>(KeyValue["PokerTournamentRounding"]); 
	PokerTournamentRound		= SafeConvert<UINT32>(KeyValue["PokerTournamentRound"]);
	PokerTournamentChange		= SafeConvert<UINT32>(KeyValue["PokerTournamentChange"]);
	PokerTournamentChangeAdd	= SafeConvert<UINT32>(KeyValue["PokerTournamentChangeAdd"]);

	return true;
}

bool GlobalConfig::InitPersonalCareer()
{
	std::vector<std::string> vec;
	vec.clear();
	RoleDeclarationLenMax =  SafeConvert<UINT32>(KeyValue["RoleDeclarationLenMax"]);
	vec.clear();
	vec = Split(KeyValue["PersonalCareerPVP"], '|');
	PersonalCareerPVP.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		PersonalCareerPVP.push_back(SafeConvert<UINT32>(vec[i]));
	}
	vec.clear();
	vec = Split(KeyValue["CareerLevelFilter"], '|');
	CareerLevelFilter.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		CareerLevelFilter.insert(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["TrophySceneList"], '|');
	CareerLevelFilter.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{	
		TrophySceneList.insert(SafeConvert<UINT32>(vec[i]));
	}

	return true;
};

bool GlobalConfig::InitGuildTerr()
{
	GuildTerritoryOpenWeek = SafeConvert<UINT32>(KeyValue["GuildTerritoryOpenWeek"]);
	GuildTerritoryRoleLvl = SafeConvert<UINT32>(KeyValue["GuildTerritoryRoleLvl"]);
	GuildTerritoryGuildLvl = SafeConvert<UINT32>(KeyValue["GuildTerritoryGuildLvl"]);
	GuildTerritoryReadyTime = SafeConvert<UINT32>(KeyValue["GuildTerritoryReadyTime"]);
	GuildTerritoryCost = ConvertToSequence<UINT32, 2>(KeyValue["GuildTerritoryCost"], '=');

	std::vector<std::string> vec;

	vec.clear();
	vec = Split(KeyValue["GuildTerritoryOpenTime"], '|');
	GuildTerritoryOpenTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32, 3> temp = ConvertToSequence<UINT32, 3>(vec[i], '='); 
		GuildTerritoryOpenTime[temp.seq[0]] = temp;
	}

	vec.clear();
	vec = Split(KeyValue["GuildTerritoryDecTime"], '|');
	GuildTerritoryDecTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		GuildTerritoryDecTime.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}
	vec.clear();

	vec = Split(KeyValue["GuildTerritoryAucTime"], '|');
	GuildTerritoryAucTime.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		GuildTerritoryAucTime.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	return true;
}

bool GlobalConfig::InitGuildInherit()
{
	GuildInheritRoleLvlLow = SafeConvert<UINT32>(KeyValue["GuildInheritRoleLvlLow"]);
	GuildInheritRoleLvlHig = SafeConvert<UINT32>(KeyValue["GuildInheritRoleLvlHig"]);
	GuildInheritExp = SafeConvert<UINT32>(KeyValue["GuildInheritExp"]);
	GuildInheritTime = SafeConvert<UINT32>(KeyValue["GuildInheritTime"]);
	GuildInheritTimes = SafeConvert<UINT32>(KeyValue["GuildInheritTimes"]);
	GuildInheritTeaLimit = SafeConvert<UINT32>(KeyValue["GuildInheritTeaLimit"]);
	GuildInheritStuLimit = SafeConvert<UINT32>(KeyValue["GuildInheritStuLimit"]);
	GuildInheritCon = SafeConvert<UINT32>(KeyValue["GuildInheritCon"]);
	GuildInheritFirend = SafeConvert<UINT32>(KeyValue["GuildInheritFirend"]);
	GuildInheritCDTime = SafeConvert<UINT32>(KeyValue["GuildInheritCDTime"]);

	return true;
}

bool GlobalConfig::InitGuildLadder()
{

	GuildLadderNum = SafeConvert<UINT32>(KeyValue["GuildLadderNum"]);
	GuildLadderRewardTimes = SafeConvert<UINT32>(KeyValue["GuildLadderRewardTimes"]);
	GuildLadderTitle = KeyValue["GuildLadderTitle"];
	GuildLadderCon = KeyValue["GuildLadderCon"];
	GuildLadderRoleTitle = KeyValue["GuildLadderRoleTitle"];
	GuildLadderRoleCon = KeyValue["GuildLadderRoleCon"];

	std::vector<std::string> vec;

	vec.clear();
	vec = Split(KeyValue["GuildLadderReward"], "|");
	GuildLadderReward.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildLadderReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["GuildLadderTime"], "|");
	GuildLadderTime.clear();
	for(size_t i = 0; i < vec.size(); ++i)
	{
		GuildLadderTime.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	return true;
}


bool GlobalConfig::InitMilitaryRank()
{

	std::vector<std::string> vec;
	vec.clear();	
	vec = Split(KeyValue["MilitaryRank"], '|');
	MilitaryRank.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		MilitaryRank.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}	

	TopRankMilitaryExploit = SafeConvert<UINT32>(KeyValue["TopRankMilitaryExploit"]);
	MilitaryRankShowRange  = SafeConvert<UINT32>(KeyValue["MilitaryRankShowRange"]);
	MilitarySameOpponentRound = SafeConvert<UINT32>(KeyValue["MilitarySameOpponentRound"]);

	return true;
}

bool GlobalConfig::InitFlower()
{
	FlowerRainTime = SafeConvert<UINT32>(KeyValue["FlowerRainTime"]);

	return true;
}

bool GlobalConfig::InitPlatForm()
{
	std::vector<std::string> vec;

	QQVipHintDay = ConvertToVector<INT32>(KeyValue["QQVipHintDay"], '|');
	vec.clear();
	vec = Split(KeyValue["QQVipGift"], '|');
	QQVipGift.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		QQVipGift.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}
	vec.clear();
	vec = Split(KeyValue["QQSVipGift"], '|');
	QQSVipGift.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		QQSVipGift.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["QQVipSignIn"], '|');
	QQVipSignIn.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		QQVipSignIn.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["QQSVipSignIn"], '|');
	QQSVipSignIn.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		QQSVipSignIn.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["QQGameCenterSingnIn"], '|');
	QQGameCenterSingnIn.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		QQGameCenterSingnIn.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["WXGameCenterSingnIn"], '|');
	WXGameCenterSingnIn.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		WXGameCenterSingnIn.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	QQVipLevelReward = ConvertToSequence<UINT32, 2>(KeyValue["QQVipLevelReward"], '=');
	QQSVipLevelReward = ConvertToSequence<UINT32, 2>(KeyValue["QQSVipLevelReward"], '=');
	QQGameCenterLevelReward = ConvertToSequence<UINT32, 2>(KeyValue["QQGameCenterLevelReward"], '=');
	WXGameCenterLevelReward = ConvertToSequence<UINT32, 2>(KeyValue["WXGameCenterLevelReward"], '=');

	EGameHintDay = ConvertToVector<INT32>(KeyValue["EGameHintDay"], '|');
	XinYueHintDay = ConvertToVector<INT32>(KeyValue["XinYueHintDay"], '|');

	OpenCgiUrl = KeyValue["OpenCgiUrl"];

	vec.clear();
	vec = Split(KeyValue["OpenCGIChest"], '|');
	OpenCGIChest.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		OpenCGIChest.push_back(ConvertToSequence<UINT32, 4>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["OpenWebChatChest"], '|');
	OpenWebChatChest.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		OpenWebChatChest.push_back(ConvertToSequence<UINT32, 4>(vec[i], '='));
	}

	return true;
}

bool GlobalConfig::InitPartner()
{
	PartnerNum = GetInt("PartnerNum", 4);
	PartnerNeedFriendDegree = GetInt("PartnerNeedFriendDegree", 10);
	PartnerNeedDragonCoin = GetInt("PartnerNeedDragonCoin", 200);
	PartnerLeaveTime = GetInt("PartnerLeaveTime", 14400);
	PartnerLivenessRecordNum = GetInt("PartnerLivenessRecordNum", 10);
	PartnerShopRecordNum = GetInt("PartnerShopRecordNum", 10);
	PartnerShopId = GetInt("PartnerShopId", 20);
	return true;
}

bool GlobalConfig::InitAI()
{
	_provoke_skill_ids.clear();
	{
		const std::string& provokeSkill = GetGlobalConfig().GetConfig("ProvokeSkill");
		const std::vector<string>& skillargs = Split(provokeSkill, '|');

		for (UINT32 i=0; i<skillargs.size(); i++)
		{
			const std::vector<string>& args = Split(skillargs[i], "=");
			_provoke_skill_ids[XCommon::XHash(args[0].c_str())] = convert<float>(args[1]); 
		}
	}

	_send_msg_skill_ids.clear();
	{
		const std::string& provokeSkill =  GetGlobalConfig().GetConfig("SendMsgSkill");
		const std::vector<string>& skillnames = Split(provokeSkill, '|');

		for (UINT32 i=0; i<skillnames.size(); i++)
		{
			_send_msg_skill_ids.push_back(XCommon::XHash(skillnames[i].c_str()));
		}
	}
	_AINavGap = convert<float>(GetGlobalConfig().GetConfig("AINavGap"));
	return true;
}

bool GlobalConfig::InitMentor()
{
	MentorOpenSealLevel = SafeConvert<int>(KeyValue["MentorOpenSealLevel"]);
	MentorNormalCompleteNum = SafeConvert<int>(KeyValue["Mentor_NormalComplete"]);
	MentorPerfectCompleteNum = SafeConvert<int>(KeyValue["Mentor_PerfectComplete"]);
	MentorPunishDay = SafeConvert<int>(KeyValue["MentorPunishTime"]);
	MentorPeriodDay = SafeConvert<int>(KeyValue["MentorPeriodTime"]);
	MentorBreakConfirmTime = SafeConvert<int>(KeyValue["MentorBreakConfirmTime"]);
	MentorNormalCompleteDay = SafeConvert<int>(KeyValue["Mentor_NormalCompleteDay"]);
	MentorForceCompleteDay = SafeConvert<int>(KeyValue["Mentor_ForceCompleteDay"]);

	return true;
}

bool GlobalConfig::InitArenaStar()
{
	ArenaStarSaveDBLimit = SafeConvert<UINT32>(KeyValue["ArenaStarSaveDBLimit"]);
	ArenaStarRankLimit = SafeConvert<UINT32>(KeyValue["ArenaStarRankLimit"]);
	ArenaStarReward.clear();
	std::vector<std::string> vec = Split(KeyValue["ArenaStarReward"], '|');
	for(size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<int,3> seq = ConvertToSequence<int,3>(vec[i], '=');
		ArenaStarReward.push_back(seq);
	}
	return true;
}

bool GlobalConfig::InitSkyCraft()
{
	SkyTeamRoleNum = SafeConvert<int>(KeyValue["SkyTeamRoleNum"]);
	SkyTeamNameLenMin = SafeConvert<int>(KeyValue["SkyTeamNameLenMin"]);
	SkyTeamNameLenMax = SafeConvert<int>(KeyValue["SkyTeamNameLenMax"]);
	SkyTeamInitPoint = SafeConvert<int>(KeyValue["SkyTeamInitPoint"]);
	SkyCraftSceneId = SafeConvert<int>(KeyValue["SkyCraftSceneId"]);
	SkyCraftRecordNum = SafeConvert<int>(KeyValue["SkyCraftRecordNum"]);
	SkyCraftDailyNum = GetInt("SkyCraftDailyNum", 7);
	return true;
}

bool GlobalConfig::InitGuildCampParty()
{
	// 工会营地派对
	std::vector<std::string> vec;	
	vec.clear();
	vec = Split(KeyValue["GuildCampPartyNPC"], '|');
	GuildCampPartyNPC.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		GuildCampPartyNPC.push_back(ConvertToSequence<float, 4>(vec[i], '='));
	}

	//GuildCampPartyFirstStageTime = SafeConvert<int>(KeyValue["GuildCampPartyFirstStageTime"]);
	//GuildCampPartySecondStageTime = SafeConvert<int>(KeyValue["GuildCampPartySecondStageTime"]);

	RandomItemDistance =  SafeConvert<float>(KeyValue["RandomItemDistance"]);


	vec.clear();
	vec = Split(KeyValue["GuildCampPartySpriteTimeStap"], '|');
	GuildCampPartySpriteTimeStap.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		Sequence<UINT32,2> temp = ConvertToSequence<UINT32, 2>(vec[i], '=');
		GuildCampPartySpriteTimeStap.push_back(temp);
	}

	vec.clear();
	vec = Split(KeyValue["GuildCampPartyBonus"], '|');
	GuildCampPartyBonus.clear();
	for (UINT32 i = 0; i < (UINT32)vec.size(); ++i)
	{
		Sequence<float,4> temp_seq = ConvertToSequence<float, 4>(vec[i], '=');			
		GuildCampPartyBonus.insert(std::make_pair(i,temp_seq));
	}

	GuildCampRandomEnemyFrequency = SafeConvert<float>(KeyValue["GuildCampRandomEnemyFrequency"]);
	//vec.clear();
	//vec = Split(KeyValue["GuildCampRandomEnemy"], '|');
	//GuildCampRandomEnemy.clear();
	//for (size_t i = 0; i < vec.size(); ++i)
	//{
	//	Sequence<float,4> temp_seq = ConvertToSequence<float, 4>(vec[i], '=');			
	//	GuildCampRandomEnemy.push_back(temp_seq);
	//}

	GuildCampPartyNPCDropID = ConvertToSequence<UINT32, 2>(KeyValue["GuildCampPartyNPCDropID"], '=');

	GuildCampPartyContinuedTime =  SafeConvert<UINT32>(KeyValue["GuildCampPartyContinuedTime"]);

	GuildCampPartyPreNoticeTime = SafeConvert<UINT32>(KeyValue["GuildCampPartyPreNoticeTime"]);

	GuildCampPartySpriteExistTime = SafeConvert<UINT32>(KeyValue["GuildCampPartySpriteExistTime"]);
	GuildCampPartyCountDownNoticeTime  = SafeConvert<UINT32>(KeyValue["GuildCampPartyCountDownNoticeTime"]);
	// npc 存在时间
	GuildCampPartyContinuedTime2 = SafeConvert<UINT32>(KeyValue["GuildCampPartyContinuedTime2"]);
	// 获取奖励时间
	GuildCollecLotteryWaitTime = SafeConvert<UINT32>(KeyValue["GuildCollecLotteryWaitTime"]);

	vec.clear();
	vec = Split(KeyValue["GuildCampSummonSpiritCost"], '|');
	GuildCampSummonSpiritCost.clear();
	for (UINT32 i = 0; i < (UINT32)vec.size(); ++i)
	{
		Sequence<UINT32, 2> temp_seq = ConvertToSequence<UINT32, 2>(vec[i], '=');			
		GuildCampSummonSpiritCost.push_back(temp_seq);
	}

	return true;
}

bool GlobalConfig::InitBattleField()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["BattleFieldLevel"], '|');
	BattleFieldLevel.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BattleFieldLevel.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	BattleFieldBuff.clear();
	vec = Split(KeyValue["BattleFieldBuff"], '|');	
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BattleFieldBuff.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	BattleFieldMaxPlayer =  SafeConvert<UINT32>(KeyValue["BattleFieldMaxPlayer"]);//每组8玩家
	BattleFieldMatchTime =  SafeConvert<UINT32>(KeyValue["BattleFieldMatchTime"]);//每120秒进行一次匹配
	BattleFieldReadyScene=  SafeConvert<UINT32>(KeyValue["BattleFieldReadyScene"]);//等待场景
	BattleFieldCreateScene = SafeConvert<UINT32>(KeyValue["BattleFieldCreateScene"]);//一次创建场景数量
	BattleFieldFightScene=  ConvertToVector<UINT32>(KeyValue["BattleFieldFightScene"], '|');//战斗场景
	BattleFieldCutScene = SafeConvert<UINT32>(KeyValue["BattleFieldCutScene"]);		//切场景数
	BattleFieldFightTime = SafeConvert<UINT32>(KeyValue["BattleFieldFightTime"]);	
	BattleFieldAnimTime = SafeConvert<UINT32>(KeyValue["BattleFieldAnimTime"]);	

	vec.clear();
	vec = Split(KeyValue["BattleFieldPreNoticeList"], '|');
	BattleFieldPreNoticeList.clear();
	for(UINT32 i = 0; i < (UINT32)vec.size(); i++)
	{
		BattleFieldPreNoticeList.push_back(SafeConvert<UINT32>(vec[i]));
	}

	BattleFieldReviveTime = SafeConvert<UINT32>(KeyValue["BattleFieldReviveTime"]);	
	vec.clear();
	vec = Split(KeyValue["BattleFieldKillerLvPoint"], '|');
	BattleFieldKillerLvPoint.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		BattleFieldKillerLvPoint.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	BattleFieldSyncTime = SafeConvert<UINT32>(KeyValue["BattleFieldSyncTime"]);	

	return true;
}

bool GlobalConfig::InitTask()
{
	WeeklyTaskRefreshCount = GetInt("WeeklyTaskRefreshCount", 5);
	WeeklyTaskRefreshCost = ConvertToVector<UINT32>(KeyValue["WeeklyTaskRefreshCost"], '|');

	std::vector<string> vec;
	std::vector<Sequence<UINT32, 2> > tempProb;

	vec.clear();
	vec = Split(KeyValue["WeeklyTaskRefreshProb"], '|');
	for (size_t i = 0; i < vec.size(); ++i)
	{
		tempProb.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	WeeklyTaskRefreshProb.clear();
	WeeklyTaskHighQuality = 0;
	UINT32 sumRate = 0;
	for (size_t i = 0; i < tempProb.size(); ++i)
	{
		Sequence<UINT32, 2>& one = tempProb[i];
		sumRate += one[1];
		WeeklyTaskRefreshProb[sumRate] = one[0];
		if (WeeklyTaskHighQuality < one[0])
		{
			WeeklyTaskHighQuality = one[0];
		}
	}

	WeeklyTaskAskHelpNum = GetInt("WeeklyTaskAskHelpNum", 3);
	WeeklyTaskDonateNum = GetInt("WeeklyTaskDonateNum", 10);
	WeeklyTaskHighQualityNum = GetInt("WeeklyTaskHighQualityNum", 3);
	WeekyFreeRefreshNum = GetInt("WeekyFreeRefreshNum", 3);

	DailyTaskMaxRefreshTaskTimes = GetInt("MaxRefreshTaskTimes", 3);
	DailyTaskMaxBuyRefreshCount = GetInt("MaxBuyRefreshCount", 10);
	DailyTaskMaxBuyRefreshCost = ConvertToVector<UINT32>(KeyValue["BuyRefreshCost"], '|');
	DailyTaskMaxHelpCount = GetInt("DailyTaskMaxHelpCount", 10);

	return true;
}

UINT32 GlobalConfig::GetWeeklyTaskRefreshCost(UINT32 count)
{
	if (WeeklyTaskRefreshCost.empty())
	{
		return 0;
	}
	if (count >= WeeklyTaskRefreshCost.size())
	{
		return *WeeklyTaskRefreshCost.rbegin();
	}
	return WeeklyTaskRefreshCost[count];
}

UINT32 GlobalConfig::GetDailyTaskRefreshCost(UINT32 count)
{
	if (DailyTaskMaxBuyRefreshCost.empty())
	{
		return 0;
	}
	if (count >= DailyTaskMaxBuyRefreshCost.size())
	{
		return *DailyTaskMaxBuyRefreshCost.rbegin();
	}
	return DailyTaskMaxBuyRefreshCost[count];
}

UINT32 GlobalConfig::RandomWeeklyTaskQuality()
{
	if (WeeklyTaskRefreshProb.empty())
	{
		return 0;
	}
	if (WeeklyTaskRefreshProb.size() == 1)
	{
		return WeeklyTaskRefreshProb.begin()->second;
	}
	UINT32 value = XRandom::randInt(0, WeeklyTaskRefreshProb.rbegin()->first);
	auto it = WeeklyTaskRefreshProb.upper_bound(value);
	return it->second;
}


bool GlobalConfig::InitWedding()
{
	std::vector<std::string> vec;
	NormalWeddingSceneId = GetInt("NormalWeddingSceneId", 72);
	LuxuryWeddingSceneId = GetInt("LuxuryWeddingSceneId", 73);
	NormalWeddingInviteNum = GetInt("NormalWeddingInviteNum", 5);
	LuxuryWeddingInviteNum = GetInt("LuxuryWeddingInviteNum", 10);
	WeddingPrepareTime = GetInt("WeddingPrepareTime", 600);
	WeddingRunningTime = GetInt("WeddingRunningTime", 1200);

	WeddingHappynessInterval = GetInt("WeddingHappynessInterval", 1);
	WeddingHappynessOneTime = GetInt("WeddingHappynessOneTime", 1);
	WeddingMaxHappyness = GetInt("WeddingMaxHappyness", 100);
	WeddingFlowCD = GetInt("WeddingFlowCD", 10);
	WeddingFireworksCD = GetInt("WeddingFireworksCD", 10);
	WeddingFlowerAddHp = GetInt("WeddingFlowerAddHp", 10);
	WeddingFireworksAddHp = GetInt("WeddingFireworksAddHp", 10);

	WeddingFlowerDailyMax = GetInt("WeddingFlowerDailyMax", 10);
	WeddingFireworksDailyMax = GetInt("WeddingFireworksDailyMax", 10);
	WeddingDoodadDailyMax = GetInt("WeddingDoodadDailyMax", 10);

	WeddingFlowerCost = ConvertToSequence<UINT32, 2>(KeyValue["WeddingFlowerCost"], '=');
	WeddingFireworksCost = ConvertToSequence<UINT32, 2>(KeyValue["WeddingFireworksCost"], '=');

	vec.clear();
	vec = Split(KeyValue["WeddingFlowerReward"], '|');
	WeddingFlowerReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		WeddingFlowerReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["WeddingFireworksReward"], '|');
	WeddingFireworksReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		WeddingFireworksReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	WeddingPatrolAITree = KeyValue["WeddingPatrolAITree"];
	vec.clear();
	vec = Split(KeyValue["WeddingPatrolPath"], '|');
	WeddingPatrolPath.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		WeddingPatrolPath.push_back(ConvertToSequence<float, 4>(vec[i], '='));
	}
	WeddingPatrolTime = GetInt("WeddingPatrolTime", 25);
	NormalWeddingPetId = GetInt("NormalWeddingPetId", 144);
	LuxuryWeddingPetId = GetInt("LuxuryWeddingPetId", 146);
	WeddingVowsHusBandPos = ConvertToSequence<float, 4>(KeyValue["WeddingVowsHusBandPos"], '=');
	WeddingVowsWifePos = ConvertToSequence<float, 4>(KeyValue["WeddingVowsWifePos"], '=');
	WeddingCarHusBandPos = ConvertToSequence<float, 4>(KeyValue["WeddingCarHusBandPos"], '=');
	WeddingCarWifePos = ConvertToSequence<float, 4>(KeyValue["WeddingCarWifePos"], '=');
	NormalWeddingNpcIDs = ConvertToVector<UINT32>(KeyValue["NormalWeddingNpcIDs"], '|');
	LuxuryWeddingNpcIDs = ConvertToVector<UINT32>(KeyValue["LuxuryWeddingNpcIDs"], '|');
	WeddingCutscenePath = KeyValue["WeddingCutscenePath"];
	WeddingCarCutScene = KeyValue["WeddingCarCutScene"];


	WeddingWife = KeyValue["WeddingWife"];
	WeddingHusband =  KeyValue["WeddingHusband"];
	return true;
}

bool GlobalConfig::InitPlatFormShare()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["FirstPassShareSceneList"], '|');
	FirstPassShareSceneList.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		FirstPassShareSceneList.insert(SafeConvert<UINT32>(vec[i]));
	}

	vec.clear();
	vec = Split(KeyValue["FirstPassShareReward"], '|');
	FirstPassShareReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		FirstPassShareReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	WeekShareNum = SafeConvert<UINT32>(KeyValue["WeekShareNum"]);

	vec.clear();
	vec = Split(KeyValue["WeekShareReward"], '|');
	WeekShareReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		WeekShareReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["FlowerActivityAwardList"], '|');
	FlowerActivityReward.clear();
	for (size_t i = 0; i < vec.size(); ++i)
	{
		FlowerActivityReward.push_back(ConvertToSequence<UINT32, 2>(vec[i], '='));
	}

	return true;

}

bool GlobalConfig::InitNpcFeeling()
{
	NpcFeelingGiveCount = SafeConvert<UINT32>(KeyValue["NpcFeelingGiveCount"]);
	NpcFeelingGiveLeftAddLimit = SafeConvert<UINT32>(KeyValue["NpcFeelingGiveLeftAddLimit"]);

	NpcFeelingSeal2Level.clear();
	std::vector<std::string> strVec = Split(KeyValue["NpcFeelingSeal2Level"], '|');
	for(size_t i = 0; i < strVec.size(); ++i)
	{
		auto seq = ConvertToSequence<UINT32,2>(strVec[i], '=');
		NpcFeelingSeal2Level[seq[0]] = seq[1];
	}

	NpcFeelingTopCount = SafeConvert<UINT32>(KeyValue["NpcFeelingTopCount"]);

	NpcFeelingExchangePro = SafeConvert<UINT32>(KeyValue["NpcFeelingExchangePro"]);
	NpcFeelingExchangeCount = SafeConvert<UINT32>(KeyValue["NpcFeelingExchangeCount"]);

	NpcFeelingReturnPro = SafeConvert<UINT32>(KeyValue["NpcFeelingReturnPro"]);
	NpcFeelingReturnCount = SafeConvert<UINT32>(KeyValue["NpcFeelingReturnCount"]);
	NpcFeelingReturnDelay = SafeConvert<UINT32>(KeyValue["NpcFeelingReturnDelay"]);

	NpcFeelingAddExtraExpRange = SafeConvert<UINT32>(KeyValue["NpcFeelingAddExtraExpRange"]);
	NpcFeelingAddExtraExpWeight = SafeConvert<UINT32>(KeyValue["NpcFeelingAddExtraExpWeight"]);
	NpcFeelingBackFlowWeight = SafeConvert<UINT32>(KeyValue["NpcFeelingBackFlowWeight"]);
	NpcFeelingSmallBackFlowWeight = SafeConvert<UINT32>(KeyValue["NpcFeelingSmallBackFlowWeight"]);

	NpcFeelingBuyCount = SafeConvert<UINT32>(KeyValue["NpcFeelingBuyCount"]);
	NpcFeelingBuyCost.clear();
	NpcFeelingBuyCost = ConvertToVector<UINT32>(KeyValue["NpcFeelingBuyCost"], '|');

	NpcFeelingTriggerFavorCount = SafeConvert<UINT32>(KeyValue["NpcFeelingTriggerFavorCount"]);
	NpcFeelingTriggerFavorPro = SafeConvert<UINT32>(KeyValue["NpcFeelingTriggerFavorPro"]);

	return true;
}

bool GlobalConfig::InitSurvive()
{
	SurviveFightRoleCount = SafeConvert<UINT32>(KeyValue["SurviveFightRoleCount"]);
	SurviveFightMapID = SafeConvert<UINT32>(KeyValue["SurviveFightMapID"]); 
	SurviveReadyTime = SafeConvert<UINT32>(KeyValue["SurviveReadyTime"]);
	//SurviveFightTime = 100;

	SurviveAddPoint.clear();
	SurviveAddPoint = ConvertToVector<UINT32>(KeyValue["SurviveAddPoint"], '|');

	SurviveRewardTopRank = SafeConvert<UINT32>(KeyValue["SurviveRewardTopRank"]);
	SurviveRewardTopCount = SafeConvert<UINT32>(KeyValue["SurviveRewardTopCount"]);

	SurviveRewardItems.clear();
	std::vector<std::string> vec = Split(KeyValue["SurviveRewardItems"], '|');
	for(size_t i = 0; i < vec.size(); ++i)
	{
		auto seq = ConvertToSequence<UINT32,2>(vec[i], '=');
		SurviveRewardItems.push_back(seq);
	}

	return true;
}
bool GlobalConfig::InitHG()
{
	ChildOnceGameRestTimeList = ConvertToVector<UINT32>(KeyValue["ChildOnceGameRestTimeList"], '|');
	ChildOnceGameForceExitTime = SafeConvert<UINT32>(KeyValue["ChildOnceGameForceExitTime"]);
	ChildOnceGameForceRestTime = SafeConvert<UINT32>(KeyValue["ChildOnceGameForceRestTime"]);
	AdultOnceGameRestTimeList = ConvertToVector<UINT32>(KeyValue["AdultOnceGameRestTimeList"], '|');
	AdultOnceGameForceExitTime = SafeConvert<UINT32>(KeyValue["AdultOnceGameForceExitTime"]);
	AdultOnceGameForceRestTime = SafeConvert<UINT32>(KeyValue["AdultOnceGameForceRestTime"]);
	OnceGameRemindMsg = KeyValue["OnceGameRemindMsg"];
	OnceGameForceExitMsg = KeyValue["OnceGameForceExitMsg"];
	TotalGameRemindMsg = KeyValue["TotalGameRemindMsg"];
	TotalGameForceExitMsg = KeyValue["TotalGameForceExitMsg"];
	LoginRemindMsg = KeyValue["LoginRemindMsg"];
	ForceExitSceneType = ConvertToVector<UINT32>(KeyValue["ForceExitSceneType"], '|');
	HgIsIpen = SafeConvert<UINT32>(KeyValue["HgIsOpen"])==1?true:false;
	return true;
}

bool GlobalConfig::InitLuckyTable()
{
	std::vector<std::string> vec;
	vec.clear();
	vec = Split(KeyValue["LotteryDrawItems"], '|');
	LotteryDrawItems.clear();
	for (size_t i = 0; i < vec.size(); i++)
	{
		LotteryDrawItems.push_back(ConvertToSequence<UINT32, 3>(vec[i], '='));
	}

	vec.clear();
	vec = Split(KeyValue["LotteryDrawConsume"], '|');
	LotteryDrawConsume.clear();
	for (size_t i = 0; i < vec.size(); i++)
	{
		Sequence<UINT32, 3> tmp = ConvertToSequence<UINT32, 3>(vec[i], '=');
		Sequence<UINT32, 2> consumeitem;
		consumeitem.seq[0] = tmp.seq[1];
		consumeitem.seq[1] = tmp.seq[2];
		LotteryDrawConsume.insert(std::make_pair(tmp.seq[0], consumeitem));
	}
	return true;
}

UINT32 GlobalConfig::GetDonateByQuality(UINT32 quality)
{
	auto it = DonateItemQualityContribute.find(quality);
	return it == DonateItemQualityContribute.end() ? 0 : it->second;
}
