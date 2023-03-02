#include "pch.h"
#include "process.h"
#include "config.h"
#include "appid.h"
#include "ptcregister.h"
#include "protostatistics.h"
#include "db/mysqlmgr.h"
#include "role/rolemanager.h"
#include "idip/idipproc.h"
#include "scene/gsmanager.h"
#include "scene/scenemanager.h"
#include "scene/sceneconfig.h"
#include "role/rolesummarymgr.h"
#include "table/PlayerLevelManager.h"
#include "table/ItemConfigMgr.h"
#include "mail/mailmgr.h"
#include "guild/guildmgr.h"
#include "team/teammgr.h"
#include "notice/noticemgr.h"
#include "chat/chatmgr.h"
#include "live/liveconfig.h"
#include "live/livemanager.h"
#include "global/GlobalConfigDBBuild.h"
#include "levelseal/levelsealMgr.h"
#include "friend/friendmgr.h"
#include "table/globalconfig.h"
#include "friend/friendconfig.h"
#include "scene/scenecreate.h"
#include "guild/guildboss.h"
#include "table/ProfessionMgr.h"
#include "table/guildConfig.h"
#include "tsssdk/MsTssHandler.h"
#include "table/MultActivityMgr.h"
#include "worldboss/worldbossmgr.h"
#include "tlogwriter.h"
#include "table/expeditionconfigmgr.h"
#include "team/teaminvitemgr.h"
#include "team/teammatchmgr.h"
#include "pvp/pvpmgr.h"
#include "ranklist/ranklistmgr.h"
#include "util/timespecificmgr.h"
#include "team/robotsummarymgr.h"
#include "social/socialmgr.h"
#include "social/flower.h"
#include "utility/reloadmgr.h"
#include "firstpass/firstpassmgr.h"
#include "pk/pkmgr.h"
#include "pk/pkopenmgr.h"
#include "firstpass/firstpassconfig.h"
#include "account/punishmgr.h"
#include "table/StringTableMgr.h"
#include "guild/guildmgr.h"
#include "worldboss/worldbossline.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/DragonExpRankMgr.h"
#include "push/pushmgr.h"
#include "mail/mailconfig.h"
#include "arena/arenamgr.h"
#include "auction/auctionmgr.h"
#include "loghelper/tlogr.h"
#include "table/guildSkillConfig.h"
#include "table/auctionconfig.h"
#include "guild/guildcheckin.h"
#include "guild/guildcardmgr.h"
#include "gmf/guildmatchfightmgr.h"
#include "guild/guildarena.h"
#include "event/eventmgr.h"
#include "event/activitymailmgr.h"
#include "guild/guildladder.h"
#include "../share/skycitycfg/skycityconfig.h"
#include "skycity/skycitymgr.h"
#include "garden/garden_manager.h"
#include "garden/garden_config.h"
#include "qa/qamgr.h"
#include "test/testmgr.h"
#include "httptask/httptaskmgr.h"
#include "table/OpenSystemMgr.h"
#include "account/accountkick.h"
#include "reswar/reswarmgrdef.h"
#include "platforminfo/platformfriendmgr.h"
#include "reswar/reswarpvemgr.h"
#include "reswar/guildbuffmgr.h"
#include "reswar/reswarmgr.h"
#include "guildauct/guildauctmgr.h"
#include "table/guildauctconfig.h"
#include "config/itemdropconfig.h"
#include "cross/crosszonestatus.h"
#include "guild/guildinherit.h"
#include "idip/platnotice.h"
#include "forbid/forbidwordmgr.h"
#include "chat/fmmgr.h"
#include "nestweek/nestweekmgr.h"
#include "partner/partnermgr.h"
#include "tablemgr/partnertablemgr.h"
#include "mentorship/mentormgr.h"
#include "mentorship/mentorrelationmgr.h"
#include "mentorship/mentorconfig.h"
#include "invfight/invfightmgr.h"
#include "guild/guildterritorybattle.h"
#include "pokertournament/cardmatchmgr.h"
#include "herobattle/herobattlemgr.h"
#include "guildcastle/guildcastlemgr.h"
#include "pay/paymgr.h"
#include "pay/SnsSigCheck.h"
#include "leagueteam/leagueteammgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "loghelper/reportdatamgr.h"
#include "leaguebattle/leaguebattlematchmgr.h"
#include "tablemgr/leaguematchtablemgr.h"
#include "push/pushsubscribemgr.h"
#include "other/serverinfomgr.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "shm/shmmsmgr.h"
#include "leagueteam/crossleagueteammgr.h"
#include "role/crossrolesummarymgr.h"
#include "profile/msprofiler.h"
#include "leaguebattle/leaguecrosselimgr.h"
#include "other/openserverdatemgr.h"
#include "push/pushconfig.h"
#include "military/military_manager.h"
#include "common/rpcm2d_mssavedbend.h"
#include "network/dblink.h"
#include "script/scriptmgr.h"
#include "idip/accountbanmgr.h"
#include "invfight/invfightcrossmgr.h"
#include "table/SpriteConfigMgr.h"
#include "table/PetConfigMgr.h"
#include "pay/ibgiftmgr.h"
#include "arenastar/arenastarmgr.h"
#include "skyteam/skyteammgr.h"
#include "skycraft/skycraftmatchmgr.h"
#include "skycraft/skycraftelimgr.h"
#include "tablemgr/skycrafttablemgr.h"
#include "custombattle/custombattlemgr.h"
#include "custombattle/custombattlemsmatcher.h"
#include "custombattle/custombattlerolemgr.h"
#include "custombattle/custombattleconfig.h"
#include "match/matchmgr.h"
#include "report/reportmgr.h"
#include "team/tarjateammgr.h"
#include "mobabattle/mobabattlemgr.h"
#include "spactivity/spactivitymgr.h"
#include "tablemgr/backflowtablemgr.h"
#include "mobabattle/mobarecordmgr.h"
#include "mayhem/mayhemmgr.h"
#include "network/rslink.h"
#include "marriage/weddingmgr.h"
#include "groupchat/groupchatmgr.h"
#include "tablemgr/marriagetablemgr.h"
#include "marriage/marriagemgr.h"
#include "marriage/marriagerelationmgr.h"
#include "battlefield/battlefieldmgr.h"
#include "dragonguild/dragonguildmgr.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "goalawards/goalawardsmgr.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "survive/survivemgr.h"
#include "tablemgr/prerogativemgr.h"
#include "competedragon/competedragonconfig.h"
#include "task/roletaskmgr.h"
#include "table/ConsumeRebateMgr.h"
#include "pay/payconsumemgr.h"
#include "role/activerolerecord.h"

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
		ss << "log/masterserver_proto_" << buffer << ".html";
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

class QueryRouterTimer : public ITimer
{
public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount)
	{
		RSLink::Instance()->QueryRouterFromWorld();
	}
};


PrintProtoStatisticsTimer PrintTimer;
QueryRouterTimer RouterTimer;

static bool IsProcessRunning = true;


bool Process_Setup(const char *configfile)
{
	SINGLETON_CREATE_INIT_ARG1(MSConfig, configfile);
	SINGLETON_CREATE_INIT(CAppConfig);

	GetGlobalConfig().LoadFile();

	SINGLETON_CREATE_INIT_ARG1(CScriptMgr, "script/pre_load_ms.txt");
	SINGLETON_CREATE_INIT(CMysqlMgr);
	SINGLETON_CREATE_INIT(CActiveRoleRecord);
	SINGLETON_CREATE_INIT(BackFlowTableMgr);
	SINGLETON_CREATE_INIT(COpenServerDateMgr);
	SINGLETON_CREATE_INIT(ShmMsMgr);
	SINGLETON_CREATE_INIT(TLogMgr);
	SINGLETON_CREATE_INIT(TLogWriter);
	SINGLETON_CREATE_INIT(CGlobalConfigDBBuild);
	SINGLETON_CREATE_INIT(CTimeSpecificMgr);
	SINGLETON_CREATE_INIT(CRoleManager);
	SINGLETON_CREATE_INIT(CFirstPassConfig);	
	SINGLETON_CREATE_INIT(PushMgr);
	SINGLETON_CREATE_INIT(CRoleSummaryMgr);
	SINGLETON_CREATE_INIT(MailConfig);
	SINGLETON_CREATE_INIT(CMailMgr);
	SINGLETON_CREATE_INIT(CPunishMgr);
	SINGLETON_CREATE_INIT(ArenaMgr);
	SINGLETON_CREATE_INIT(CGuildConfig);
	SINGLETON_CREATE_INIT(GuildCardMgr);
	SINGLETON_CREATE_INIT(CGuildMgr);
	SINGLETON_CREATE_INIT(CRankListMgr);
	SINGLETON_CREATE_INIT(CIdipProcess);
	SINGLETON_CREATE_INIT(CGsManager);
	SINGLETON_CREATE_INIT(CSceneConfig);
	SINGLETON_CREATE_INIT(CSceneManager);
	SINGLETON_CREATE_INIT(XEntityInfoLibrary);
	SINGLETON_CREATE_INIT(PlayerLevelManager);
	SINGLETON_CREATE_INIT(CItemConfig);
	SINGLETON_CREATE_INIT(ItemDropConfig);
	SINGLETON_CREATE_INIT(RobotSummaryMgr);
	SINGLETON_CREATE_INIT(TeamMgr);
	SINGLETON_CREATE_INIT(NoticeMgr);
	SINGLETON_CREATE_INIT(FriendConfig);
	SINGLETON_CREATE_INIT(FriendMgr);
	SINGLETON_CREATE_INIT(CChatMgr);
	SINGLETON_CREATE_INIT(LiveManager);
	SINGLETON_CREATE_INIT(LiveConfig);
	SINGLETON_CREATE_INIT(CSceneCreator);
	SINGLETON_CREATE_INIT(CLevelSealMgr);
	SINGLETON_CREATE_INIT(CGuildBossMgr);
	SINGLETON_CREATE_INIT(CProfessionMgr);
	SINGLETON_CREATE_INIT(CGuildBossListener);
	SINGLETON_CREATE_INIT(CTssHandler);
	SINGLETON_CREATE_INIT(MultActivityMgr);
	SINGLETON_CREATE_INIT(WorldBossMgr);
	SINGLETON_CREATE_INIT(ExpeditionConfigMgr);
	SINGLETON_CREATE_INIT(TeamInviteMgr);
	SINGLETON_CREATE_INIT(TeamMatchMgr);
	SINGLETON_CREATE_INIT(PvpMgr);
	SINGLETON_CREATE_INIT(SocialMgr);
	SINGLETON_CREATE_INIT(FlowerConfig);
	SINGLETON_CREATE_INIT(CReloadMgr);
	SINGLETON_CREATE_INIT(CFirstPassMgr);
	SINGLETON_CREATE_INIT(PkMgr);
	SINGLETON_CREATE_INIT(PkOpenMgr);
	SINGLETON_CREATE_INIT(StringTableMgr);
	SINGLETON_CREATE_INIT(WorldBossLineMgr);
	SINGLETON_CREATE_INIT(DragonExpRankMgr);
	SINGLETON_CREATE_INIT(AuctionMgr);
	SINGLETON_CREATE_INIT(CGuildSkillConfig);
	SINGLETON_CREATE_INIT(AuctionConfig);
	SINGLETON_CREATE_INIT(GuildCheckinMgr);
	SINGLETON_CREATE_INIT(GuildMatchFightMgr);
	SINGLETON_CREATE_INIT(CGuildWarMgr);
	SINGLETON_CREATE_INIT(EventMgr);
	SINGLETON_CREATE_INIT(CGuildLadder);
	SINGLETON_CREATE_INIT(SkyCityConfig);
	SINGLETON_CREATE_INIT(SkyCityMgr);
	SINGLETON_CREATE_INIT(GardenManager);
	SINGLETON_CREATE_INIT(GardenConfig);
	SINGLETON_CREATE_INIT(QAMgr);
	SINGLETON_CREATE_INIT(CTestMgr);
	SINGLETON_CREATE_INIT(HttpTaskMgr);
	SINGLETON_CREATE_INIT(OpenSystemMgr);
	SINGLETON_CREATE_INIT(CAccountKicker);
	SINGLETON_CREATE_INIT(ResWarAwardMgr);
	SINGLETON_CREATE_INIT(ResWarMgr);
	SINGLETON_CREATE_INIT(GuildBuffMgr);
	SINGLETON_CREATE_INIT(CPlatformFriendMgr);
	SINGLETON_CREATE_INIT(CGuildArenaIntegralMgr);
	SINGLETON_CREATE_INIT(GuildAuctMgr);
	SINGLETON_CREATE_INIT(GuildAuctConfig)
	SINGLETON_CREATE_INIT(CGuildInherit)
	SINGLETON_CREATE_INIT(CrossZoneStatus);
	SINGLETON_CREATE_INIT(CPlatNoticeMgr);
	SINGLETON_CREATE_INIT(ForbidWordMgr);
	SINGLETON_CREATE_INIT(FMMgr);
	SINGLETON_CREATE_INIT(NestWeekMgr);
	SINGLETON_CREATE_INIT(PartnerTableMgr);
	SINGLETON_CREATE_INIT(MarriageTableMgr);
	SINGLETON_CREATE_INIT(PartnerMgr);
	SINGLETON_CREATE_INIT(MentorMgr);
	SINGLETON_CREATE_INIT(MentorRelationMgr);
	SINGLETON_CREATE_INIT(MentorConfig);
	SINGLETON_CREATE_INIT(InvFightMgr);
	SINGLETON_CREATE_INIT(InvFightCrossMgr);
	SINGLETON_CREATE_INIT(CGuildTerritoryBattle);
	SINGLETON_CREATE_INIT(CardMatchMgr);
	SINGLETON_CREATE_INIT(HeroBattleMgr);
	SINGLETON_CREATE_INIT(GuildCastleMgr);
	SINGLETON_CREATE_INIT(CSnsSigCheck);
	SINGLETON_CREATE_INIT(CPayMgr);
	SINGLETON_CREATE_INIT(LeagueTeamMgr);
	SINGLETON_CREATE_INIT(LeagueSeasonMgr);
	SINGLETON_CREATE_INIT(LeagueEliminationMgr);
	SINGLETON_CREATE_INIT(CReportDataMgr);
	SINGLETON_CREATE_INIT(LeagueBattleMatchMgr);
	SINGLETON_CREATE_INIT(LeagueMatchTableMgr);
	SINGLETON_CREATE_INIT(CPushSubscribeMgr);
	SINGLETON_CREATE_INIT(ZServerInfoMgr);
	SINGLETON_CREATE_INIT(CrossLeagueTeamMgr);
	SINGLETON_CREATE_INIT(CrossRoleSummaryMgr);
	SINGLETON_CREATE_INIT(LeagueCrossEliMgr);
	SINGLETON_CREATE_INIT(AccountBanMsMgr);
	SINGLETON_CREATE_INIT(CSpriteConfigMgr);
	SINGLETON_CREATE_INIT(CPetConfigMgr);
	
	SINGLETON_CREATE_INIT(MilitaryManager);
	SINGLETON_CREATE_INIT(IBGiftMgr);
	SINGLETON_CREATE_INIT(ArenaStarMgr);

	SINGLETON_CREATE_INIT(CMsProfiler);
	SINGLETON_CREATE_INIT(CPushConfigMgr);
	SINGLETON_CREATE_INIT(SkyCraftTableMgr);
	SINGLETON_CREATE_INIT(SkyTeamMgr);
	SINGLETON_CREATE_INIT(SkyCraftMatchMgr);
	SINGLETON_CREATE_INIT(SkyCraftEliMgr);

	SINGLETON_CREATE_INIT(CCustomBattleConfig);
	SINGLETON_CREATE_INIT(CustomBattleMgr);
	SINGLETON_CREATE_INIT(CustomBattleMsMatcher);
	SINGLETON_CREATE_INIT(CustomBattleRoleMgr);

	SINGLETON_CREATE_INIT(MatchMgr);
	SINGLETON_CREATE_INIT(ReportMgr);
	SINGLETON_CREATE_INIT(TarjaTeamMgr);
	SINGLETON_CREATE_INIT(ActivityMailMgr);
	SINGLETON_CREATE_INIT(MobaBattleMgr);
	SINGLETON_CREATE_INIT(SpActivityMgr);
	SINGLETON_CREATE_INIT(MobaRecordMgr);
	SINGLETON_CREATE_INIT(MayhemMsMgr);
	SINGLETON_CREATE_INIT(WeddingMgr);

    SINGLETON_CREATE_INIT(GroupChatMgr);
    SINGLETON_CREATE_INIT(GoalAwardsMgr);
    SINGLETON_CREATE_INIT(MarriageMgr);
    SINGLETON_CREATE_INIT(MarriageRelationMgr);
	SINGLETON_CREATE_INIT(BattleFieldMgr);
	SINGLETON_CREATE_INIT(CNpcFeelingMgr);
	SINGLETON_CREATE_INIT(CSurviveMgr);
	SINGLETON_CREATE_INIT(CPreprogativeConfig);
	SINGLETON_CREATE_INIT(RoleTaskMgr);
	SINGLETON_CREATE_INIT(CompeteDragonConfig);

	SINGLETON_CREATE_INIT(DragonGuildTableMgr);
	SINGLETON_CREATE_INIT(DragonGuildMgr);
	SINGLETON_CREATE_INIT(CConsumeRebateConfig);
	SINGLETON_CREATE_INIT(CPayConsumeMgr);

	CProtocolRegister::Regiter();

	//the last one
	SINGLETON_DESTORY_UNINIT(CActiveRoleRecord);
	// by huangds. 2018/5/16
	//SINGLETON_DESTORY_UNINIT(CMysqlMgr);

	ProtoStatistics::Instance()->SetEnable(false);
	CTimerMgr::Instance()->SetTimer(&PrintTimer, 0, 60 * 60 * 1000, -1, __FILE__, __LINE__);
	CTimerMgr::Instance()->SetTimer(&RouterTimer, 0, 10 * 1000, -1, __FILE__, __LINE__);

	// level and power point rank
	CRoleSummaryMgr::Instance()->UpdateRankList();
	CPunishMgr::Instance()->UpdatePunishRankData();
	return true;
}

static bool g_MsSaveDBEnd = true;
void SetMsSaveDBEnd(bool isEnd)
{
	g_MsSaveDBEnd = isEnd; 
}

void Wait()
{
#ifdef DNA_LIVE_ENV
	TimeUtil::Sleep(200);
#endif
}

void Process_Cleanup()
{
	CRoleSummaryMgr::Instance()->SetEnd(); Wait();
	CRoleSummaryMgr::Instance()->OnTimer(0, 0); Wait();
	CRankListMgr::Instance()->SaveAllToDB(); Wait();
	CChatMgr::Instance()->SaveAllToDB(); Wait();
	CGuildMgr::Instance()->SaveToDB(); Wait();
	SocialMgr::Instance()->Save2Db(); Wait();
	FriendMgr::Instance()->OnTimer(0, 0); Wait();
	GardenManager::Instance()->SaveAllToDB(); Wait();
	MentorMgr::Instance()->Save2Db(); Wait();
	CPlatformFriendMgr::Instance()->Save2DB(); Wait();
	CrossRoleSummaryMgr::Instance()->SaveAll2DB(); Wait();
	LeagueEliminationMgr::Instance()->SaveAll2DB(); Wait();
	LeagueTeamMgr::Instance()->SaveAll2DB(); Wait();
	PartnerMgr::Instance()->SaveAll2DB(); Wait();
	ArenaStarMgr::Instance()->SaveAll2DB(); Wait();
	SkyTeamMgr::Instance()->Save2DB(); Wait();
	SkyCraftEliMgr::Instance()->SaveToDB(); Wait();
	ReportMgr::Instance()->SaveToDB(); Wait();
	MobaRecordMgr::Instance()->SaveAllToDB(); Wait();
	MarriageMgr::Instance()->Save2Db(); Wait();
    GoalAwardsMgr::Instance()->SaveToDB(); Wait();
	RoleTaskMgr::Instance()->SaveToDB(-1); Wait();

	CustomBattleMgr::Instance()->SetEnd(); 
	CustomBattleMgr::Instance()->OnTimer(0, 0); Wait();

	CustomBattleRoleMgr::Instance()->SetEnd(); 
	CustomBattleRoleMgr::Instance()->OnTimer(0, 0); Wait();

	DragonGuildMgr::Instance()->SaveAll2DB(); Wait();

	RpcM2D_MsSaveDBEnd* rpc = RpcM2D_MsSaveDBEnd::CreateRpc();
	DBLink::Instance()->SendTo(*rpc);

	while (g_MsSaveDBEnd == false)
	{
		MSConfig::Instance()->ProcessNetMessage();
		TimeUtil::Sleep(500);
	}
	LogInfo("MsSaveDBEnd");
	SINGLETON_DESTORY_UNINIT(CPayConsumeMgr);
	SINGLETON_DESTORY_UNINIT(DragonGuildMgr);
	SINGLETON_DESTORY_UNINIT(DragonGuildTableMgr);
	SINGLETON_DESTORY_UNINIT(CConsumeRebateConfig);
	SINGLETON_DESTORY_UNINIT(CompeteDragonConfig);
	SINGLETON_DESTORY_UNINIT(RoleTaskMgr);
	SINGLETON_DESTORY_UNINIT(CSurviveMgr);
	SINGLETON_DESTORY_UNINIT(CNpcFeelingMgr);
	SINGLETON_DESTORY_UNINIT(CPreprogativeConfig);
	SINGLETON_DESTORY_UNINIT(BattleFieldMgr);
    SINGLETON_DESTORY_UNINIT(MarriageRelationMgr);
    SINGLETON_DESTORY_UNINIT(MarriageMgr);
    SINGLETON_DESTORY_UNINIT(GoalAwardsMgr);
    SINGLETON_DESTORY_UNINIT(GroupChatMgr);
	SINGLETON_DESTORY_UNINIT(MobaRecordMgr);
	SINGLETON_DESTORY_UNINIT(WeddingMgr);
	SINGLETON_DESTORY_UNINIT(MayhemMsMgr);	SINGLETON_DESTORY_UNINIT(SpActivityMgr);
	SINGLETON_DESTORY_UNINIT(MobaBattleMgr);
	SINGLETON_DESTORY_UNINIT(ActivityMailMgr);
	SINGLETON_DESTORY_UNINIT(TarjaTeamMgr);
	SINGLETON_DESTORY_UNINIT(ReportMgr);
	SINGLETON_DESTORY_UNINIT(MatchMgr);
	SINGLETON_DESTORY_UNINIT(CustomBattleRoleMgr);
	SINGLETON_DESTORY_UNINIT(CustomBattleMsMatcher);
	SINGLETON_DESTORY_UNINIT(CustomBattleMgr);
	SINGLETON_DESTORY_UNINIT(CCustomBattleConfig);
	SINGLETON_DESTORY_UNINIT(SkyCraftEliMgr);
	SINGLETON_DESTORY_UNINIT(SkyCraftMatchMgr);
	SINGLETON_DESTORY_UNINIT(SkyTeamMgr);
	SINGLETON_DESTORY_UNINIT(SkyCraftTableMgr);
	SINGLETON_DESTORY_UNINIT(CPushConfigMgr);
	SINGLETON_DESTORY_UNINIT(CMsProfiler);
	SINGLETON_DESTORY_UNINIT(AccountBanMsMgr);
	SINGLETON_DESTORY_UNINIT(ArenaStarMgr);
	SINGLETON_DESTORY_UNINIT(IBGiftMgr);
	SINGLETON_DESTORY_UNINIT(MilitaryManager);
	SINGLETON_DESTORY_UNINIT(LeagueCrossEliMgr);
	SINGLETON_DESTORY_UNINIT(CrossRoleSummaryMgr);
	SINGLETON_DESTORY_UNINIT(CrossLeagueTeamMgr);
	SINGLETON_DESTORY_UNINIT(ZServerInfoMgr);
	SINGLETON_DESTORY_UNINIT(LeagueMatchTableMgr);
	SINGLETON_DESTORY_UNINIT(CPushSubscribeMgr);
	SINGLETON_DESTORY_UNINIT(CReportDataMgr);
	SINGLETON_DESTORY_UNINIT(LeagueEliminationMgr);
	SINGLETON_DESTORY_UNINIT(LeagueBattleMatchMgr);
	SINGLETON_DESTORY_UNINIT(LeagueSeasonMgr);
	SINGLETON_DESTORY_UNINIT(LeagueTeamMgr);
	SINGLETON_DESTORY_UNINIT(CardMatchMgr);	
	SINGLETON_DESTORY_UNINIT(CPayMgr);
	SINGLETON_DESTORY_UNINIT(CSnsSigCheck);
	SINGLETON_DESTORY_UNINIT(GuildCastleMgr);
	SINGLETON_DESTORY_UNINIT(HeroBattleMgr);
	SINGLETON_DESTORY_UNINIT(CGuildTerritoryBattle);
	SINGLETON_DESTORY_UNINIT(InvFightCrossMgr);
	SINGLETON_DESTORY_UNINIT(InvFightMgr);
	SINGLETON_DESTORY_UNINIT(MentorConfig);
	SINGLETON_DESTORY_UNINIT(MentorRelationMgr);
	SINGLETON_DESTORY_UNINIT(MentorMgr);
	SINGLETON_DESTORY_UNINIT(NestWeekMgr);
	SINGLETON_DESTORY_UNINIT(PartnerMgr);
	SINGLETON_DESTORY_UNINIT(MarriageTableMgr);
	SINGLETON_DESTORY_UNINIT(PartnerTableMgr);
	SINGLETON_DESTORY_UNINIT(FMMgr);
	SINGLETON_DESTORY_UNINIT(ForbidWordMgr);
	SINGLETON_DESTORY_UNINIT(CPlatNoticeMgr);
	SINGLETON_DESTORY_UNINIT(CrossZoneStatus);
	SINGLETON_DESTORY_UNINIT(CGuildInherit);
	SINGLETON_DESTORY_UNINIT(GuildAuctConfig);
	SINGLETON_DESTORY_UNINIT(GuildAuctMgr);
	SINGLETON_DESTORY_UNINIT(CGuildArenaIntegralMgr);
	SINGLETON_DESTORY_UNINIT(CPlatformFriendMgr);
	SINGLETON_DESTORY_UNINIT(GuildBuffMgr);
	SINGLETON_DESTORY_UNINIT(ResWarMgr);
	SINGLETON_DESTORY_UNINIT(ResWarAwardMgr);
	SINGLETON_DESTORY_UNINIT(GardenManager);
	SINGLETON_DESTORY_UNINIT(GardenConfig);
	SINGLETON_DESTORY_UNINIT(CAccountKicker);
	SINGLETON_DESTORY_UNINIT(OpenSystemMgr);
	SINGLETON_DESTORY_UNINIT(HttpTaskMgr);
	SINGLETON_DESTORY_UNINIT(CTestMgr);
	SINGLETON_DESTORY_UNINIT(QAMgr);
	SINGLETON_DESTORY_UNINIT(CGuildLadder);
	SINGLETON_DESTORY_UNINIT(EventMgr);
	SINGLETON_DESTORY_UNINIT(CGuildWarMgr);
	SINGLETON_DESTORY_UNINIT(GuildMatchFightMgr);
	SINGLETON_DESTORY_UNINIT(AuctionConfig);
	SINGLETON_DESTORY_UNINIT(GuildCheckinMgr);
	SINGLETON_DESTORY_UNINIT(CGuildSkillConfig);
	SINGLETON_DESTORY_UNINIT(AuctionMgr);
	SINGLETON_DESTORY_UNINIT(DragonExpRankMgr);
	SINGLETON_DESTORY_UNINIT(WorldBossLineMgr);
	SINGLETON_DESTORY_UNINIT(StringTableMgr);
	SINGLETON_DESTORY_UNINIT(CReloadMgr);
	SINGLETON_DESTORY_UNINIT(PkOpenMgr);
	SINGLETON_DESTORY_UNINIT(PkMgr);
	SINGLETON_DESTORY_UNINIT(CFirstPassMgr);
	SINGLETON_DESTORY_UNINIT(FlowerConfig);
	SINGLETON_DESTORY_UNINIT(SocialMgr);
	SINGLETON_DESTORY_UNINIT(PvpMgr);
	SINGLETON_DESTORY_UNINIT(TeamMatchMgr);
	SINGLETON_DESTORY_UNINIT(TeamInviteMgr);
	SINGLETON_DESTORY_UNINIT(ExpeditionConfigMgr);
	SINGLETON_DESTORY_UNINIT(WorldBossMgr);
	SINGLETON_DESTORY_UNINIT(CTssHandler);
	SINGLETON_DESTORY_UNINIT(MultActivityMgr);
	SINGLETON_DESTORY_UNINIT(CGuildBossListener);
	SINGLETON_DESTORY_UNINIT(CProfessionMgr);
	SINGLETON_DESTORY_UNINIT(LiveConfig);
	SINGLETON_DESTORY_UNINIT(LiveManager);
	SINGLETON_DESTORY_UNINIT(CGuildBossMgr);
	SINGLETON_DESTORY_UNINIT(CLevelSealMgr);
	SINGLETON_DESTORY_UNINIT(CGlobalConfigDBBuild);
	SINGLETON_DESTORY_UNINIT(CSceneCreator);
	SINGLETON_DESTORY_UNINIT(CChatMgr);
	SINGLETON_DESTORY_UNINIT(FriendConfig);
	SINGLETON_DESTORY_UNINIT(FriendMgr);
	SINGLETON_DESTORY_UNINIT(NoticeMgr);
	SINGLETON_DESTORY_UNINIT(TeamMgr);
	SINGLETON_DESTORY_UNINIT(RobotSummaryMgr);
	SINGLETON_DESTORY_UNINIT(CRoleManager);
	SINGLETON_DESTORY_UNINIT(SkyCityMgr);
	SINGLETON_DESTORY_UNINIT(SkyCityConfig);
	SINGLETON_DESTORY_UNINIT(CRankListMgr);
	SINGLETON_DESTORY_UNINIT(CGuildMgr);
	SINGLETON_DESTORY_UNINIT(GuildCardMgr);
	SINGLETON_DESTORY_UNINIT(CGuildConfig);
	SINGLETON_DESTORY_UNINIT(ArenaMgr);
	SINGLETON_DESTORY_UNINIT(CPunishMgr);
	SINGLETON_DESTORY_UNINIT(CMailMgr);
	SINGLETON_DESTORY_UNINIT(MailConfig);
	SINGLETON_DESTORY_UNINIT(CRoleSummaryMgr);
	SINGLETON_DESTORY_UNINIT(PushMgr);
	SINGLETON_DESTORY_UNINIT(CFirstPassConfig);
	SINGLETON_DESTORY_UNINIT(CGsManager);
	SINGLETON_DESTORY_UNINIT(CSceneConfig);
	SINGLETON_DESTORY_UNINIT(CSceneManager);
	SINGLETON_DESTORY_UNINIT(CIdipProcess);
	SINGLETON_DESTORY_UNINIT(PlayerLevelManager);
	SINGLETON_DESTORY_UNINIT(XEntityInfoLibrary);
	SINGLETON_DESTORY_UNINIT(ItemDropConfig);
	SINGLETON_DESTORY_UNINIT(CItemConfig);
	SINGLETON_DESTORY_UNINIT(TLogWriter);
	SINGLETON_DESTORY_UNINIT(TLogMgr);
	SINGLETON_DESTORY_UNINIT(ShmMsMgr);
	SINGLETON_DESTORY_UNINIT(COpenServerDateMgr);
	SINGLETON_DESTORY_UNINIT(CScriptMgr);
	SINGLETON_DESTORY_UNINIT(BackFlowTableMgr);
	SINGLETON_DESTORY_UNINIT(CAppConfig);	
	SINGLETON_DESTORY_UNINIT(MSConfig);	
	
	// by huangds, 2018/5/16
	SINGLETON_DESTORY_UNINIT(CMysqlMgr);
}

bool Process_Update()
{
	MSConfig::Instance()->ProcessNetMessage();

	CTssHandler::Instance()->on_timer();
	PkMgr::Instance()->Update();
	CGuildMgr::Instance()->Update();
	CMailMgr::Instance()->Update();
	EventMgr::Instance()->Update();
	HttpTaskMgr::Instance()->Update();
	DragonGuildMgr::Instance()->Update();
	return IsProcessRunning;
}

void Process_Stop()
{
	IsProcessRunning = false;
}

