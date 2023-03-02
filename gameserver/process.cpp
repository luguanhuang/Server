#include "pch.h"
#include "timeutil.h"
#include "process.h"
#include "ptcregister.h"
#include "dblink.h"
#include "config.h"
#include "appid.h"
#include "protostatistics.h"
#include "timer.h"
#include "unit/rolemanager.h"
#include "unit/enemymanager.h"
#include "unit/selectrolestatemgr.h"
#include "scene/scenemanager.h"
#include "scene/sceneconfig.h"
#include "scene/grid.h"
#include "buff/XBuffTemplateManager.h"
#include "utility/XActionCurve.h"
#include "util/XRandom.h"
#include "table/SkillReader.h"
#include "table/CurveReader.h"
#include "table/SkillList.h"
#include "table/XSkillEffectMgr.h"
#include "table/ProfessionMgr.h"
#include "table/PlayerLevelManager.h"
#include "table/CombatParamMgr.h"
#include "table/TaskMgr.h"
#include "table/TableCmd.h"
#include "table/OpenSystemMgr.h"
#include "table/ChannelCloseSys.h"
#include "table/PowerPointMgr.h"
#include "physics/XPhysics.h"
#include "gamelogic/XCombat.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/AchivementMgr.h"
#include "gamelogic/XLevelSpawnMgr.h"
#include "gamelogic/XLevelScriptMgr.h"
#include "table/globalconfig.h"
#include "gamelogic/LotteryMgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "gamelogic/rolesummarymgr.h"
#include "gamelogic/emblemmgr.h"
#include "gamelogic/jademgr.h"
#include "gamelogic/XSpawnInfoGenerator.h"
#include "gamelogic/checkinmgr.h"
#include "util/timespecificmgr.h"
#include "gamelogic/activitymgr.h"
#include "gamelogic/takecostmgr.h"
#include "gamelogic/fashionmgr.h"
#include "gamelogic/randombossmgr.h"
#include "gamelogic/rewardmgr.h"
#include "gamelogic/shop.h"
#include "table/expeditionconfigmgr.h"
#include "gamelogic/buyexchangemgr.h"
#include "gamelogic/stagerankmgr.h"
#include "gamelogic/gmhandler.h"
#include "gamelogic/chatmgr.h"
#include "gamelogic/AchivementActivityManager.h"
#include "gamelogic/noticemgr.h"
#include "gamelogic/arenarewardmgr.h"
#include "gamelogic/arenamgr.h"
#include "guild/guildmgr.h"
#include "guild/guildconfig.h"
#include "guild/guildcardmgr.h"
#include "buff/XBuffRegenerate.h"
#include "guild/guildboss.h"
#include "gamelogic/onlinereward.h"
#include "gamelogic/FatigueMgr.h"
#include "scene/positiondebugmgr.h"
#include "gamelogic/RandomTaskConfig.h"
#include "gamelogic/teaminvitemgr.h"
#include "gamelogic/pkmgr.h"
#include "gamelogic/randomnamemgr.h"
#include "ai/aiconfig.h"
#include "ai/aitreemgr.h"
#include "scene/waypointmgr.h"
#include "unit/dummyrolemanager.h"
#include "gamelogic/fightgroup.h"
#include "unit/petconfigmgr.h"
#include "gamelogic/robotgenerator.h"
#include "gamelogic/robotconfig.h"
#include "findback/findbackconfig.h"
#include "utility/tlogmgr.h"
#include "unit/accountsessionmgr.h"
#include "forbid/forbidwordmgr.h"
#include "tlogwriter.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "gamelogic/enhanceequipmgr.h"
#include "scene/attributepk.h"
#include "idip/idipcmdproc.h"
#include "gamelogic/pvpmgr.h"
#include "tsssdk/TssHandler.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/dragonconfig.h"
#include "gamelogic/MultActivityMgr.h"
#include "gamelogic/bossrushmgr.h"
#include "live/livemanager.h"
#include "live/liveconfig.h"
#include "scene/leavescenecontrol.h"
#include "db/mysqlmgr.h"
#include "scene/sceneswitch.h"
#include "apollovoice/ApolloVoiceMgr.h"
#include "gamelogic/payv2Mgr.h"
#include "network/mslink.h"
#include "network/gatelink.h"
#include "unit/rolemanager.h"
#include "reconnetmgr.h"
#include "ai/aionlinedebugmgr.h"
#include "gamelogic/titleconfig.h"
#include "gamelogic/firstpassconfig.h"
#include "gamelogic/firstpassmgr.h"
#include "gamelogic/flower.h"
#include "gamelogic/spriteconfig.h"
#include "gamelogic/atlasconfigmgr.h"
#include "gamelogic/ibshopmgr.h"
#include "scene/DragonExpConfig.h"
#include "gamelogic/stageassist.h"
#include "mail/mailconfig.h"
#include "gamelogic/superriskconfig.h"
#include "gamelogic/teammgr.h"
#include "config/itemdropconfig.h"
#include "table/PushTableConfig.h"
#include "table/InspireConfig.h"
#include "spactivity/spactivitymgr.h"
#include "test/testmgr.h"
#include "spactivity/spactivityconfig.h"
#include "skycitycfg/skycityconfig.h"
#include "gamelogic/halliconmgr.h"
#include "gamelogic/payconfig.h"
#include "event/eventmgr.h"
#include "gamelogic/pandoraconfig.h"
#include "reswar/reswarpvemgr.h"
#include "gamelogic/worldlevelmgr.h"
#include "guild/guildinherit.h"
#include "httptask/httptaskmgr.h"
#include "gamelogic/reloadmgr.h"
#include "gamelogic/teamcostconfig.h"
#include "utility/UtilMath.h"
#include "gamelogic/antiaddictionmgr.h"
#include "gamelogic/enchantmgr.h"
#include "gamelogic/horsemgr.h"
#include "tablemgr/partnertablemgr.h"
#include "photo/photo_manager.h"
#include "gamelogic/herobattlemgr.h"
#include "servermgr/servermgr.h"
#include "gamelogic/reportdatamgr.h"
#include "gamelogic/randomattrmgr.h"
#include "gamelogic/popwindows.h"
#include "gamelogic/changeprofession.h"
#include "gamelogic/holidaystage.h"
#include "gamelogic/LeagueMatchParamMgr.h"
#include "gamelogic/loginactivity.h"
#include "profile/gsprofiler.h"
#include "gamelogic/military_rank_manager.h"
#include "script/scriptmgr.h"
#include "tablemgr/skycrafttablemgr.h"
#include "gamelogic/artifactmgr.h"
#include "gamelogic/effectconfig.h"
#include "gamelogic/artifactconfig.h"
#include "guild/guildcampparty.h"
#include "gamelogic/operatingactivity.h"
#include "gamelogic/abysspartymgr.h"
#include "gamelogic/perioddesi.h"
#include "gamelogic/tarjamgr.h"
#include "table/ohmygodmgr.h"
#include "gamelogic/weekend4v4mgr.h"
#include "gamelogic/mobabattlemgr.h"
#include "tablemgr/backflowtablemgr.h"
#include "gamelogic/nestconfig.h"
#include "gamelogic/dragonhelpconfig.h"
#include "table/mayhemconfig.h"
#include "gamelogic/tajiehelpconfig.h"
#include "servermgr/servercrossdatamgr.h"
#include "gamelogic/PersonalCareerQuest.h"
#include "skill/XBulletMgr.h"
#include "tablemgr/marriagetablemgr.h"
#include "gamelogic/dragongroupmgr.h"
#include "table/backflowbuyconfig.h"
#include "dragonguild/dragonguildmgr.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "gamelogic/attrmodmgr.h"
#include "tablemgr/prerogativemgr.h"
#include "gamelogic/goalaward.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "gamelogic/battlefieldgsmgr.h"
#include "gamelogic/helldoordropmgr.h"
#include "singleton.h"
#include "gamelogic/fusemgr.h"
#include "gamelogic/competedragonconfig.h"
#include "table/doodaditemskillconfig.h"
#include "gamelogic/payconsumemgr.h"
#include "scene/scenetransconfig.h"
#include "scene/sceneleisureconfig.h"

class PrintProtoStatisticsTimer : public ITimer
{
public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		if(!ProtoStatistics::Instance()->IsEnable()) return;

		time_t tNow = time(NULL);
		tm* pt = localtime(&tNow);
		char buffer[64] = {0}; 
		strftime(buffer, 64, "%Y-%m-%d-%H-%M", pt);

		std::stringstream ss;
		ss << "log/gameserver" << GSConfig::Instance()->GetLine() << "_proto_" << buffer << ".html";
		FILE *fp = fopen(ss.str().c_str(), "w");
		if(fp == NULL)
		{
			LogError("Create file %s failed", ss.str().c_str());
			return;
		}

		std::string info = ProtoStatistics::Instance()->HtmlInfo();
		fwrite(info.c_str(), info.size(), 1, fp);
		fclose(fp);

		ProtoStatistics::Instance()->Reset();
	}
};


PrintProtoStatisticsTimer PrintTimer;

static bool IsProcessRunning = true;

static volatile bool IsSignalINT2Received = false;

static bool IsSignalINT2Processed = false;

bool Process_Setup(const char *configfile)
{
	UtilMath::Init();

	SINGLETON_CREATE_INIT_ARG1(GSConfig, configfile);
	SINGLETON_CREATE_INIT(CAppConfig);
	
	SINGLETON_CREATE_INIT(TssHandler);

	GetGlobalConfig().LoadFile();

	SINGLETON_CREATE_INIT_ARG1(CScriptMgr, "script/pre_load_gs.txt");
	SINGLETON_CREATE_INIT(CMysqlMgr);
	SINGLETON_CREATE_INIT(CTimeSpecificMgr);
	SINGLETON_CREATE_INIT(XEntityInfoLibrary);
	SINGLETON_CREATE_INIT(AITreeMgr);
	SINGLETON_CREATE_INIT(RobotGenerator);
	SINGLETON_CREATE_INIT(RobotConfig);
	SINGLETON_CREATE_INIT(RoleManager);
	SINGLETON_CREATE_INIT(EnemyManager);
	SINGLETON_CREATE_INIT(DummyRoleManager);
	SINGLETON_CREATE_INIT(WayPointMgr)
	SINGLETON_CREATE_INIT(InspireConfig);
	SINGLETON_CREATE_INIT(SceneLeisureConfig);
	SINGLETON_CREATE_INIT(SceneTransConfig);
	SINGLETON_CREATE_INIT(SceneConfig);
	SINGLETON_CREATE_INIT(XLevelScriptMgr);
	SINGLETON_CREATE_INIT(XLevelSpawnMgr);
	SINGLETON_CREATE_INIT(SceneManager);
	SINGLETON_CREATE_INIT(XBuffTemplateManager);
	SINGLETON_CREATE_INIT(XActionCurveMgr);
	SINGLETON_CREATE_INIT(XSkillEffectMgr);
	SINGLETON_CREATE_INIT(ProfessionMgr);
	SINGLETON_CREATE_INIT(PlayerLevelManager);
	SINGLETON_CREATE_INIT(SelectRoleStateMgr);
	SINGLETON_CREATE_INIT(XPhysics);
	SINGLETON_CREATE_INIT(TableCmd);
	SINGLETON_CREATE_INIT(ItemConfig);
	SINGLETON_CREATE_INIT(ItemDropConfig);
	SINGLETON_CREATE_INIT(AchivementActivityManager);
	SINGLETON_CREATE_INIT(AchivementMgr);
	SINGLETON_CREATE_INIT(CombatParamMgr);
	SINGLETON_CREATE_INIT(TaskMgr);
	SINGLETON_CREATE_INIT(PowerPointMgr);
	SINGLETON_CREATE_INIT(OpenSystemMgr);
	SINGLETON_CREATE_INIT(ChannelCloseSysMgr);
	SINGLETON_CREATE_INIT(MultActivityMgr);
	SINGLETON_CREATE_INIT(LotteryMgr);
	SINGLETON_CREATE_INIT(EnhanceEquipMgr);
	SINGLETON_CREATE_INIT(CArenaMgr);
	SINGLETON_CREATE_INIT(RoleSummaryMgr);
	SINGLETON_CREATE_INIT(CEmblemMgr);
	SINGLETON_CREATE_INIT(CJadeMgr);
	SINGLETON_CREATE_INIT(XSpawnInfoGenerator);
	SINGLETON_CREATE_INIT(CCheckinMgr);
	SINGLETON_CREATE_INIT(CActivityMgr);
	SINGLETON_CREATE_INIT(TakeCostManager);
	SINGLETON_CREATE_INIT(FashionMgr);
	SINGLETON_CREATE_INIT(RandomBossMgr);
	SINGLETON_CREATE_INIT(CRewardMgr);
	SINGLETON_CREATE_INIT(ShopMgr);
	SINGLETON_CREATE_INIT(TeamCopyMgr);
	SINGLETON_CREATE_INIT(ExpeditionConfigMgr);
	SINGLETON_CREATE_INIT(BuyExchangeMgr);
	SINGLETON_CREATE_INIT(StageRankMgr);
	SINGLETON_CREATE_INIT(GMHandler);
	SINGLETON_CREATE_INIT(ChatMgr);
	SINGLETON_CREATE_INIT(NoticeMgr);
	SINGLETON_CREATE_INIT(ArenaRewardMgr);
	SINGLETON_CREATE_INIT(GuildSimpleMgr);
	SINGLETON_CREATE_INIT(GuildConfig);
	SINGLETON_CREATE_INIT(GuildCardMgr);
    SINGLETON_CREATE_INIT(GuildBossMgr);
	SINGLETON_CREATE_INIT(OnlineRewardMgr);
	SINGLETON_CREATE_INIT(FatigueMgr);
	SINGLETON_CREATE_INIT(PositionDebugMgr);
	SINGLETON_CREATE_INIT(RandomTaskConfig);
	SINGLETON_CREATE_INIT(TeamInviteMgr);
	SINGLETON_CREATE_INIT(PkMgr);
	SINGLETON_CREATE_INIT(RandomNameMgr);
	SINGLETON_CREATE_INIT(AIConfig);
	SINGLETON_CREATE_INIT(FlowerConfig);
	SINGLETON_CREATE_INIT(FightGroupMgr);
	SINGLETON_CREATE_INIT(PetConfigMgr);
	SINGLETON_CREATE_INIT(SkyCityConfig);
	SINGLETON_CREATE_INIT(FindBackConfig);
	SINGLETON_CREATE_INIT(TLogMgr);
	SINGLETON_CREATE_INIT(TLogWriter);
	SINGLETON_CREATE_INIT(TTag);
	SINGLETON_CREATE_INIT(AccountSessionManager);
	SINGLETON_CREATE_INIT(ForbidWordMgr);
	SINGLETON_CREATE_INIT(DesignationMgr);
	SINGLETON_CREATE_INIT(TowerConfig);
	SINGLETON_CREATE_INIT(CLevelSealMgr);
	SINGLETON_CREATE_INIT(AchieveV2Mgr);
	SINGLETON_CREATE_INIT(AttributePkMgr);
	SINGLETON_CREATE_INIT(CIdipCmdProcess);
	SINGLETON_CREATE_INIT(PvpMgr);
	SINGLETON_CREATE_INIT(QAMgr);
	SINGLETON_CREATE_INIT(DragonConfig);
	SINGLETON_CREATE_INIT(BossRushMgr);
	SINGLETON_CREATE_INIT(LiveManager);
	SINGLETON_CREATE_INIT(LiveConfig);
	SINGLETON_CREATE_INIT(CLeaveSceneControl);
	SINGLETON_CREATE_INIT( ApolloVoiceMgr );
	SINGLETON_CREATE_INIT(PayV2Mgr);
	SINGLETON_CREATE_INIT(AIOnlineDebugMgr);
	SINGLETON_CREATE_INIT(TitleConfig);
	SINGLETON_CREATE_INIT(FirstPassConfig);
	SINGLETON_CREATE_INIT(FirstPassMgr);
	SINGLETON_CREATE_INIT(SpriteConfig);
	SINGLETON_CREATE_INIT(CAtlasConfigMagr);
	SINGLETON_CREATE_INIT(IBShopMgr);
	SINGLETON_CREATE_INIT(DragonExpConfig);
	SINGLETON_CREATE_INIT(StageAssistMgr);
	SINGLETON_CREATE_INIT(MailConfig);
	SINGLETON_CREATE_INIT(SuperRiskConfig);
	SINGLETON_CREATE_INIT(PushTableConfig);	
	SINGLETON_CREATE_INIT(CTestMgr);
	SINGLETON_CREATE_INIT(SpActivityConfig);
	SINGLETON_CREATE_INIT(SpActivityMgr);	
	SINGLETON_CREATE_INIT(HallIconMgr);
	SINGLETON_CREATE_INIT(CPayConfig);
	SINGLETON_CREATE_INIT(PandoraConfig);
	SINGLETON_CREATE_INIT(EventMgr);
	SINGLETON_CREATE_INIT(ResWarAwardMgr);
	SINGLETON_CREATE_INIT(WorldLevelMgr);
	SINGLETON_CREATE_INIT(CGuildInherit);
	SINGLETON_CREATE_INIT(HttpTaskMgr);
	SINGLETON_CREATE_INIT(ReloadMgr);
	SINGLETON_CREATE_INIT(TeamCostConfig);
	SINGLETON_CREATE_INIT(CAntiAddictionMgr);
	SINGLETON_CREATE_INIT(EnchantMgr);
	SINGLETON_CREATE_INIT(HorseRideMgr);
	SINGLETON_CREATE_INIT(PartnerTableMgr);
	SINGLETON_CREATE_INIT(PhotoManager);
	SINGLETON_CREATE_INIT(HeroBattleMgr);
	SINGLETON_CREATE_INIT(ServerMgr);
	SINGLETON_CREATE_INIT(ReportDataMgr);
	SINGLETON_CREATE_INIT(RandomAttrMgr);
	SINGLETON_CREATE_INIT(PopWindows);
	SINGLETON_CREATE_INIT(ChangeProfessionMgr);
	SINGLETON_CREATE_INIT(CHolidayStage);
	SINGLETON_CREATE_INIT(LeagueMatchParamMgr);
	SINGLETON_CREATE_INIT(LoginActivityMgr);
	SINGLETON_CREATE_INIT(CGsProfiler);
	SINGLETON_CREATE_INIT(MilitaryRankManager);
	SINGLETON_CREATE_INIT(SkyCraftTableMgr);	
	SINGLETON_CREATE_INIT(EffectConfig);
	SINGLETON_CREATE_INIT(ArtifactMgr);
	SINGLETON_CREATE_INIT(ArtifactConfig);
	SINGLETON_CREATE_INIT(CGuildCampPartyMgr);
	SINGLETON_CREATE_INIT(OperatingActivityMgr);
	SINGLETON_CREATE_INIT(AbyssPartyMgr);
	SINGLETON_CREATE_INIT(PeriodDesiMgr);
	SINGLETON_CREATE_INIT(TarjaMgr);
	SINGLETON_CREATE_INIT(OhMyGodMgr);
	SINGLETON_CREATE_INIT(BackFlowTableMgr);
    SINGLETON_CREATE_INIT(WeekEnd4v4Mgr);
    SINGLETON_CREATE_INIT(DragonGroupMgr);
	SINGLETON_CREATE_INIT(MobaBattleMgr);
	SINGLETON_CREATE_INIT(NestConfig);
	SINGLETON_CREATE_INIT(DragonHelpConfig);
	SINGLETON_CREATE_INIT(MayhemConfigMgr);
	SINGLETON_CREATE_INIT(TajieHelpConfig);
	SINGLETON_CREATE_INIT(ServerCrossDataMgr);
	SINGLETON_CREATE_INIT(MarriageTableMgr);
	SINGLETON_CREATE_INIT(BackFlowBuyConfig);
	SINGLETON_CREATE_INIT(DragonGuildTableMgr);
	SINGLETON_CREATE_INIT(DragonGuildSimpleMgr);
	SINGLETON_CREATE_INIT(AttrModMgr);
    SINGLETON_CREATE_INIT(GoalAwardsConfig);
	SINGLETON_CREATE_INIT(BattleFieldGSMgr);
	SINGLETON_CREATE_INIT(HellDoorDropMgr);

	SINGLETON_CREATE_INIT(PersonalCareerQuest);	
	SINGLETON_CREATE_INIT(XBulletMgr);	
	SINGLETON_CREATE_INIT(CPreprogativeConfig);
	SINGLETON_CREATE_INIT(CNpcFeelingMgr);
	SINGLETON_CREATE_INIT(CDoodadItemSkillConfig);
	SINGLETON_CREATE_INIT(FuseMgr);
	SINGLETON_CREATE_INIT(CompeteDragonConfig);
	SINGLETON_CREATE_INIT(CPayConsumeMgr);
	//the last one
	SINGLETON_DESTORY_UNINIT(CMysqlMgr)

	CProtocolRegister::Regiter();
	XCombat::Setup();

	ProtoStatistics::Instance()->SetEnable(false);
	CTimerMgr::Instance()->SetTimer(&PrintTimer, 0, 60 * 60 * 1000, -1, __FILE__, __LINE__);

	return true;
}

void Process_Cleanup()
{
	XCombat::Cleanup();

	SINGLETON_DESTORY_UNINIT(CPayConsumeMgr);
	SINGLETON_DESTORY_UNINIT(CompeteDragonConfig);
	SINGLETON_DESTORY_UNINIT(FuseMgr);
	SINGLETON_DESTORY_UNINIT(CDoodadItemSkillConfig);
	SINGLETON_DESTORY_UNINIT(CNpcFeelingMgr);
	SINGLETON_DESTORY_UNINIT(CPreprogativeConfig);
	SINGLETON_DESTORY_UNINIT(XBulletMgr);
	SINGLETON_DESTORY_UNINIT(BackFlowBuyConfig);
	SINGLETON_DESTORY_UNINIT(AttrModMgr);
	SINGLETON_DESTORY_UNINIT(TajieHelpConfig);

	SINGLETON_DESTORY_UNINIT(HellDoorDropMgr);
	SINGLETON_DESTORY_UNINIT(BattleFieldGSMgr);
	SINGLETON_DESTORY_UNINIT(GoalAwardsConfig);
	SINGLETON_DESTORY_UNINIT(DragonGuildSimpleMgr);
	SINGLETON_DESTORY_UNINIT(DragonGuildTableMgr);
	SINGLETON_DESTORY_UNINIT(BackFlowBuyConfig);	
	SINGLETON_DESTORY_UNINIT(MarriageTableMgr);
	SINGLETON_DESTORY_UNINIT(ServerCrossDataMgr);
	SINGLETON_DESTORY_UNINIT(TajieHelpConfig);
	SINGLETON_DESTORY_UNINIT(MayhemConfigMgr);
	SINGLETON_DESTORY_UNINIT(DragonHelpConfig);
	SINGLETON_DESTORY_UNINIT(NestConfig);
	SINGLETON_DESTORY_UNINIT(MobaBattleMgr);
    SINGLETON_DESTORY_UNINIT(DragonGroupMgr);
	SINGLETON_DESTORY_UNINIT(WeekEnd4v4Mgr);
	SINGLETON_DESTORY_UNINIT(BackFlowTableMgr);	
	SINGLETON_DESTORY_UNINIT(OhMyGodMgr);
	SINGLETON_DESTORY_UNINIT(TarjaMgr);
	SINGLETON_DESTORY_UNINIT(PeriodDesiMgr);
	SINGLETON_DESTORY_UNINIT(AbyssPartyMgr);
	SINGLETON_DESTORY_UNINIT(OperatingActivityMgr);	
	SINGLETON_DESTORY_UNINIT(CGuildCampPartyMgr);
	SINGLETON_DESTORY_UNINIT(ArtifactConfig);
	SINGLETON_DESTORY_UNINIT(ArtifactMgr);
	SINGLETON_DESTORY_UNINIT(EffectConfig);	
	SINGLETON_DESTORY_UNINIT(SkyCraftTableMgr);
	SINGLETON_DESTORY_UNINIT(MilitaryRankManager);
	SINGLETON_DESTORY_UNINIT(CGsProfiler);
	SINGLETON_DESTORY_UNINIT(LoginActivityMgr);
	SINGLETON_DESTORY_UNINIT(LeagueMatchParamMgr);
	SINGLETON_DESTORY_UNINIT(ChangeProfessionMgr);
	SINGLETON_DESTORY_UNINIT(CHolidayStage);
	SINGLETON_DESTORY_UNINIT(PopWindows);
	SINGLETON_DESTORY_UNINIT(RandomAttrMgr);
	SINGLETON_DESTORY_UNINIT(ReportDataMgr);
	SINGLETON_DESTORY_UNINIT(ServerMgr);
	SINGLETON_DESTORY_UNINIT(HeroBattleMgr);
	SINGLETON_DESTORY_UNINIT(PhotoManager);
	SINGLETON_DESTORY_UNINIT(PartnerTableMgr);
	SINGLETON_DESTORY_UNINIT(HorseRideMgr);
	SINGLETON_DESTORY_UNINIT(EnchantMgr);
	SINGLETON_DESTORY_UNINIT(CAntiAddictionMgr);
	SINGLETON_DESTORY_UNINIT(TeamCostConfig);
	SINGLETON_DESTORY_UNINIT(ReloadMgr);
	SINGLETON_DESTORY_UNINIT(HttpTaskMgr);
	SINGLETON_DESTORY_UNINIT(CGuildInherit);
	SINGLETON_DESTORY_UNINIT(WorldLevelMgr);
	SINGLETON_DESTORY_UNINIT(ResWarAwardMgr);
	SINGLETON_DESTORY_UNINIT(EventMgr);
	SINGLETON_DESTORY_UNINIT(PandoraConfig);
	SINGLETON_DESTORY_UNINIT(CPayConfig);
	SINGLETON_DESTORY_UNINIT(HallIconMgr);
	SINGLETON_DESTORY_UNINIT(SpActivityMgr);
	SINGLETON_DESTORY_UNINIT(SpActivityConfig);
	SINGLETON_DESTORY_UNINIT(CTestMgr);
	SINGLETON_DESTORY_UNINIT(PushTableConfig);
	SINGLETON_DESTORY_UNINIT(SuperRiskConfig);
	SINGLETON_DESTORY_UNINIT(MailConfig);
	SINGLETON_DESTORY_UNINIT(StageAssistMgr);
	SINGLETON_DESTORY_UNINIT(DragonExpConfig);
	SINGLETON_DESTORY_UNINIT(IBShopMgr);
	SINGLETON_DESTORY_UNINIT(CAtlasConfigMagr);
	SINGLETON_DESTORY_UNINIT(SpriteConfig);
	SINGLETON_DESTORY_UNINIT(FirstPassMgr);
	SINGLETON_DESTORY_UNINIT(FirstPassConfig);
	SINGLETON_DESTORY_UNINIT(TitleConfig);
	SINGLETON_DESTORY_UNINIT(AIOnlineDebugMgr);
	SINGLETON_DESTORY_UNINIT(PayV2Mgr);
	SINGLETON_DESTORY_UNINIT(ApolloVoiceMgr);
	SINGLETON_DESTORY_UNINIT(CLeaveSceneControl);
	SINGLETON_DESTORY_UNINIT(LiveConfig);
	SINGLETON_DESTORY_UNINIT(LiveManager);
	SINGLETON_DESTORY_UNINIT(BossRushMgr);
	SINGLETON_DESTORY_UNINIT(DragonConfig);
	SINGLETON_DESTORY_UNINIT(QAMgr);
	SINGLETON_DESTORY_UNINIT(PvpMgr);	
	SINGLETON_DESTORY_UNINIT(CIdipCmdProcess);
	SINGLETON_DESTORY_UNINIT(AttributePkMgr);
	SINGLETON_DESTORY_UNINIT(AchieveV2Mgr);
	SINGLETON_DESTORY_UNINIT(CLevelSealMgr);	
	SINGLETON_DESTORY_UNINIT(TowerConfig);
	SINGLETON_DESTORY_UNINIT(DesignationMgr);
	SINGLETON_DESTORY_UNINIT(ForbidWordMgr);
	SINGLETON_DESTORY_UNINIT(AccountSessionManager);
	SINGLETON_DESTORY_UNINIT(TTag);
	SINGLETON_DESTORY_UNINIT(TLogWriter);
	SINGLETON_DESTORY_UNINIT(TLogMgr);	
	SINGLETON_DESTORY_UNINIT(FindBackConfig);
	SINGLETON_DESTORY_UNINIT(PetConfigMgr);
	SINGLETON_DESTORY_UNINIT(PetConfigMgr);	
	SINGLETON_DESTORY_UNINIT(FightGroupMgr);		
	SINGLETON_DESTORY_UNINIT(SkyCityConfig);
	SINGLETON_DESTORY_UNINIT(FlowerConfig);
	SINGLETON_DESTORY_UNINIT(AIConfig);
	SINGLETON_DESTORY_UNINIT(RandomNameMgr);
	SINGLETON_DESTORY_UNINIT(PkMgr);
	SINGLETON_DESTORY_UNINIT(RandomTaskConfig);
	SINGLETON_DESTORY_UNINIT(PositionDebugMgr);
	SINGLETON_DESTORY_UNINIT(OnlineRewardMgr);
	SINGLETON_DESTORY_UNINIT(GuildBossMgr);
	SINGLETON_DESTORY_UNINIT(GuildCardMgr);
	SINGLETON_DESTORY_UNINIT(GuildConfig);
	SINGLETON_DESTORY_UNINIT(GuildSimpleMgr);
	SINGLETON_DESTORY_UNINIT(ArenaRewardMgr);
	SINGLETON_DESTORY_UNINIT(AchivementActivityManager);
	SINGLETON_DESTORY_UNINIT(NoticeMgr);
	SINGLETON_DESTORY_UNINIT(ChatMgr);
	SINGLETON_DESTORY_UNINIT(GMHandler);
	SINGLETON_DESTORY_UNINIT(StageRankMgr);
	SINGLETON_DESTORY_UNINIT(BuyExchangeMgr);
	SINGLETON_DESTORY_UNINIT(ExpeditionConfigMgr);
	SINGLETON_DESTORY_UNINIT(TeamCopyMgr);
	SINGLETON_DESTORY_UNINIT(TeamInviteMgr);
	SINGLETON_DESTORY_UNINIT(ShopMgr);
	SINGLETON_DESTORY_UNINIT(CRewardMgr);
	SINGLETON_DESTORY_UNINIT(RandomBossMgr);
	SINGLETON_DESTORY_UNINIT(FashionMgr);
	SINGLETON_DESTORY_UNINIT(TakeCostManager);
	SINGLETON_DESTORY_UNINIT(CActivityMgr);
	SINGLETON_DESTORY_UNINIT(CCheckinMgr);
	SINGLETON_DESTORY_UNINIT(XSpawnInfoGenerator);
	SINGLETON_DESTORY_UNINIT(CJadeMgr);
	SINGLETON_DESTORY_UNINIT(CEmblemMgr);
	SINGLETON_DESTORY_UNINIT(CArenaMgr);
	SINGLETON_DESTORY_UNINIT(RoleSummaryMgr);
	SINGLETON_DESTORY_UNINIT(EnhanceEquipMgr);
	SINGLETON_DESTORY_UNINIT(LotteryMgr);
	SINGLETON_DESTORY_UNINIT(ChannelCloseSysMgr);
	SINGLETON_DESTORY_UNINIT(OpenSystemMgr);
	SINGLETON_DESTORY_UNINIT(MultActivityMgr);
	SINGLETON_DESTORY_UNINIT(PowerPointMgr);
	SINGLETON_DESTORY_UNINIT(TaskMgr);
	SINGLETON_DESTORY_UNINIT(CombatParamMgr);
	SINGLETON_DESTORY_UNINIT(AchivementMgr);
	SINGLETON_DESTORY_UNINIT(ItemDropConfig);
	SINGLETON_DESTORY_UNINIT(ItemConfig);
	SINGLETON_DESTORY_UNINIT(TableCmd);
	SINGLETON_DESTORY_UNINIT(XPhysics);
	SINGLETON_DESTORY_UNINIT(SelectRoleStateMgr);
	SINGLETON_DESTORY_UNINIT(PlayerLevelManager);
	SINGLETON_DESTORY_UNINIT(ProfessionMgr);
	SINGLETON_DESTORY_UNINIT(XActionCurveMgr);
	SINGLETON_DESTORY_UNINIT(XBuffTemplateManager);
	SINGLETON_DESTORY_UNINIT(SceneManager);
	SINGLETON_DESTORY_UNINIT(XLevelSpawnMgr);
	SINGLETON_DESTORY_UNINIT(XLevelScriptMgr);
	SINGLETON_DESTORY_UNINIT(SceneConfig);
	SINGLETON_DESTORY_UNINIT(SceneTransConfig);
	SINGLETON_DESTORY_UNINIT(SceneLeisureConfig);
	SINGLETON_DESTORY_UNINIT(InspireConfig);
	SINGLETON_DESTORY_UNINIT(WayPointMgr)
	SINGLETON_DESTORY_UNINIT(DummyRoleManager);
	SINGLETON_DESTORY_UNINIT(EnemyManager);
	SINGLETON_DESTORY_UNINIT(RoleManager);
	SINGLETON_DESTORY_UNINIT(RobotConfig);
	SINGLETON_DESTORY_UNINIT(RobotGenerator);
	SINGLETON_DESTORY_UNINIT(AITreeMgr);
	SINGLETON_DESTORY_UNINIT(XSkillEffectMgr);
	SINGLETON_DESTORY_UNINIT(XEntityInfoLibrary);
	SINGLETON_DESTORY_UNINIT(CTimeSpecificMgr);
	SINGLETON_DESTORY_UNINIT(TssHandler);	
	
	SINGLETON_DESTORY_UNINIT(CScriptMgr);	
	SINGLETON_DESTORY_UNINIT(CAppConfig);
	SINGLETON_DESTORY_UNINIT(GSConfig);

	LogInfo("server stoped");
}

static float deltaTime = 0;
static int frameCount = 0;

int FrameCount()
{
	return frameCount;
}

void Process_ReceiveSignalINT2();

bool Process_Update()
{
	Process_ReceiveSignalINT2();

	float delatT = TimeUtil::GetDeltaTime();

	GSConfig::Instance()->ProcessNetMessage();

	deltaTime += delatT;

	float FRAME_TIME = GSConfig::Instance()->GetFrameTime();

	if (deltaTime >= FRAME_TIME && !IsSignalINT2Received)
	{
		//logical frame count added here
		frameCount++;

		//sequence is very IMPORTANT !!!
		deltaTime = (delatT < FRAME_TIME) ? deltaTime - FRAME_TIME : 0;
		//update
		XBulletMgr::Instance()->Update((delatT < FRAME_TIME) ? FRAME_TIME : delatT);
		RoleManager::Instance()->Update((delatT < FRAME_TIME) ? FRAME_TIME : delatT);
		EnemyManager::Instance()->Update((delatT < FRAME_TIME) ? FRAME_TIME : delatT);
		DummyRoleManager::Instance()->Update((delatT < FRAME_TIME) ? FRAME_TIME : delatT);
		SceneManager::Instance()->Update((delatT < FRAME_TIME) ? FRAME_TIME : delatT);
		ApolloVoiceMgr::Instance()->Update();
	}
	TssHandler::Instance()->on_timer();
	HttpTaskMgr::Instance()->Update();
	return IsProcessRunning;
}

class StopServer: public ITimer
{
public:
	StopServer()
	{
		index = 0;
		timer = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	}
	~StopServer()
	{
		CTimerMgr::Instance()->KillTimer(timer);
	}

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		int from = index;
		int i = 0;
		for (; index < (int)m_roleIDs.size() && i < 200; ++i, ++index)
		{
			Role* role = RoleManager::Instance()->FindByRoleID(m_roleIDs[index]);
			if (role)
			{
				role->SaveDataOnGsShutdown();

				//RoleManager::Instance()->RemoveBySession(role->GetSessionID());
			}
			else
				LogWarn("stop server save role data, missing roleid:%llu", m_roleIDs[index]);
		}
		if (index == m_roleIDs.size())
		{
			IsProcessRunning = false;
			delete this;
		}
		LogInfo("stop server save role data, from [%u to %u)", from, index);
	}

	void InitRoles()
	{
		m_roleIDs.clear();
		std::list<Role*>::iterator from = RoleManager::Instance()->Begin();
		std::list<Role*>::iterator end = RoleManager::Instance()->End();
		for (; from != end; ++from)
		{
			m_roleIDs.push_back((*from)->GetID());
		}
		LogInfo("stop server save role data, total size: %u", m_roleIDs.size());
	}

public:

	static int index;
	HTIMER timer;
	std::vector<UINT64> m_roleIDs;
};

int StopServer::index = 0;

void Process_Stop()
{
	IsSignalINT2Received  = true;
}

void Process_ReceiveSignalINT2()
{
	if (!IsSignalINT2Received)
	{
		return;
	}
	if (IsSignalINT2Processed)
	{
		return;
	}

	IsSignalINT2Processed = true;

	ReconnectManager::Instance()->UnInit();

	MSLink::Instance()->CloseMsLink();

	GatetLink::Instance()->CloseAllLink();

	StopServer* stop = new StopServer();
	stop->InitRoles();
}

