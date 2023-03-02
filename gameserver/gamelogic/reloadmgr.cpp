#include "pch.h"
#include "reloadmgr.h"
#include "buff/XBuffTemplateManager.h"
#include "table/XSkillEffectMgr.h"
#include "table/expeditionconfigmgr.h"
#include "scene/attributepk.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "table/PlayerLevelManager.h"
#include "table/OpenSystemMgr.h"
#include "table/TaskMgr.h"
#include "itemconfig.h"
#include "shop.h"
#include "gamelogic/noticemgr.h"
#include "ibshopmgr.h"
#include "payv2Mgr.h"
#include "gamelogic/fashionmgr.h"
#include "gamelogic/XLevelSpawnMgr.h"
#include "ai/aitreemgr.h"
#include "levelsealMgr.h"
#include "config/itemdropconfig.h"
#include "gamelogic/worldlevelmgr.h"
#include "mail/mailconfig.h"
#include "gamelogic/pkmgr.h"
#include "gamelogic/payconfig.h"
#include "pandoraconfig.h"
#include "gamelogic/MultActivityMgr.h"
#include "guild/guildboss.h"
#include "findback/findbackconfig.h"
#include "guild/guildconfig.h"
#include "table/SkillReader.h"
#include "ai/aitreemgr.h"
#include "table/PowerPointMgr.h"
#include "table/ChannelCloseSys.h"
#include "LotteryMgr.h"
#include "herobattlemgr.h"
#include "gamelogic/holidaystage.h"
#include "gamelogic/stagerankmgr.h"
#include "enchantmgr.h"
#include "spactivity/spactivitymgr.h"
#include "gamelogic/stageassist.h"
#include "spactivity/spactivitydef.h"
#include "gamelogic/loginactivity.h"
#include "tablemgr/skycrafttablemgr.h"
#include "guild/guildcampparty.h"
#include "effectconfig.h"
#include "artifactconfig.h"
#include "abysspartymgr.h"
#include "operatingactivity.h"
#include "gamelogic/perioddesi.h"
#include "gamelogic/tarjamgr.h"
#include "randomattrmgr.h"
#include "unit/petconfigmgr.h"
#include "gamelogic/military_rank_manager.h"
#include "mobabattlemgr.h"
#include "dragonhelpconfig.h"
#include "gamelogic/designationMgr.h"
#include "weekend4v4mgr.h"
#include "gamelogic/firstpassconfig.h"
#include "ai/aiconfig.h"
#include "table/mayhemconfig.h"
#include "spriteconfig.h"
#include "live/liveconfig.h"
#include "reload/reloadkey.h"
#include "reswar/reswarpvemgr.h"
#include "skycitycfg/skycityconfig.h"
#include "dragongroupmgr.h"
#include "gamelogic/dragonconfig.h"
#include "spactivity/spactivityconfig.h"
#include "gamelogic/bossrushmgr.h"
#include "table/backflowbuyconfig.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/attrmodmgr.h"
#include "gamelogic/nestconfig.h"
#include "tablemgr/partnertablemgr.h"
#include "gamelogic/battlefieldgsmgr.h"
#include "gamelogic/helldoordropmgr.h"
#include "npcfeeling/npcfeelingmgr.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "gamelogic/achievev2Mgr.h"
#include "goalaward.h"
#include "table/doodaditemskillconfig.h"
#include "scene/scenetransconfig.h"
#include "scene/sceneleisureconfig.h"
#include "jademgr.h"
#include "fusemgr.h"
#include "gamelogic/PersonalCareerQuest.h"

INSTANCE_SINGLETON(ReloadMgr)

ReloadMgr::ReloadMgr()
{

}

ReloadMgr::~ReloadMgr()
{

}

template<typename T>
bool ReloadFun()
{
	return T::Instance()->LoadFile();
}


template<>
bool ReloadFun<GlobalConfig>()
{
	return GetGlobalConfig().LoadFile();
}

bool ReloadMgr::Init()
{
	GetFun("BuffList") = ReloadFun<XBuffTemplateManager>;
	GetFun("BuffListPVP") = ReloadFun<XBuffTemplateManager>;
	GetFun("SkillList") = ReloadFun<XSkillEffectMgr>;

	GetFun("SceneView") = ReloadFun<GlobalConfig>;
	GetFun("SceneList") = ReloadFun<SceneConfig>;
	GetFun("DNExpedition") = ReloadFun<ExpeditionConfigMgr>;
	GetFun("RandomSceneList") = ReloadFun<ExpeditionConfigMgr>;
	GetFun("PkParam") = ReloadFun<PkMgr>;
	GetFun("GuildBossConfig") = ReloadFun<GuildBossMgr>;
	//GetFun("LevelDir") = ReloadFun<XLevelSpawnMgr>;

	GetFun("XEntityPresentation") = ReloadFun<XEntityInfoLibrary>;
	GetFun("XEntityStatistics") = ReloadFun<XEntityInfoLibrary>;
	
	GetFun("PkProfession") = ReloadFun<AttributePkMgr>;
	GetFun("PlayerLevel") = ReloadFun<PlayerLevelManager>;
	GetFun("Vip") = ReloadFun<PayV2Mgr>;
	GetFun("SystemList") = ReloadFun<OpenSystemMgr>;
	GetFun("TaskListNew") = ReloadFun<TaskMgr>;
	GetFun("AtkDefMod") = ReloadFun<PowerPointMgr>;
	GetFun("DefMod") = ReloadFun<PowerPointMgr>;

	GetFun("ItemList") = ReloadFun<ItemConfig>;
	GetFun("ChestList") = ReloadFun<ItemConfig>;
	GetFun("DoodadList") = ReloadFun<ItemConfig>;
	GetFun("EquipList") = ReloadFun<ItemConfig>;
	GetFun("CircleDraw") = ReloadFun<ItemConfig>;
	GetFun("FashionList") = ReloadFun<ItemConfig>;
	GetFun("ItemCompose") = ReloadFun<ItemConfig>;

	GetFun("RandomAttributes") = ReloadFun<RandomAttrMgr>;
	GetFun("ForgeAttributes") = ReloadFun<RandomAttrMgr>;
	GetFun("ForgeTransAttr") = ReloadFun<RandomAttrMgr>;

	GetFun("AttributeEmblem") = ReloadFun<CEmblemMgr>;
	GetFun("SkillEmblem") = ReloadFun<CEmblemMgr>;

	GetFun("ItemBack") = ReloadFun<FindBackConfig>;
	GetFun("DropList") = ReloadFun<ItemDropConfig>;
	GetFun("FashionCompose") = ReloadFun<FashionMgr>;

	GetFun("Shop") = ReloadFun<ShopMgr>;
	GetFun("IBShop") = ReloadFun<IBShopMgr>;

	GetFun("Notice") = ReloadFun<NoticeMgr>;
	GetFun("LevelSealType") = ReloadFun<CLevelSealMgr>;
	GetFun("GlobalConfig") = ReloadFun<GlobalConfig>;
	GetFun("WorldLevelExpBuff") = ReloadFun<WorldLevelMgr>;
	GetFun("MailContent") = ReloadFun<MailConfig>;
	GetFun("PayAileen") = ReloadFun<CPayConfig>;
	GetFun("MultiActivityList") = ReloadFun<MultActivityMgr>;
	GetFun("GuildConfig") = ReloadFun<GuildConfig>;
	GetFun("PandoraHeart") = ReloadFun<PandoraConfig>;
	GetFun("AITree") = ReloadFun<AITreeMgr>;
	GetFun("AIConfig") = ReloadFun<AIConfig>;
	GetFun("ChannelCloseSys") = ReloadFun<ChannelCloseSysMgr>;
	GetFun("FestivalSceneList") = ReloadFun<CHolidayStage>;
	GetFun("StageRank") = ReloadFun<StageRankMgr>;
	GetFun("StageAssist") = ReloadFun<StageAssistMgr>;
	GetFun("ShareRandomGift") = ReloadFun<LotteryMgr>;
	// herobattle
	GetFun("HeroBattle") = ReloadFun<HeroBattleMgr>;
    GetFun("WeekEnd4v4") = ReloadFun<WeekEnd4v4Mgr>;

	GetFun("DragonGroupMgr") = ReloadFun<DragonGroupMgr>;

	// enchant
	GetFun("EnchantEquip") = ReloadFun<EnchantMgr>;
	// 登录活动
	GetFun("LoginActivity") = ReloadFun<LoginActivityMgr>;
	GetFun("SkyCraft") = ReloadFun<SkyCraftTableMgr>;

	GetFun("GuildCampParty") = ReloadFun<CGuildCampPartyMgr>;
	// effect
	GetFun("EffectTable") = ReloadFun<EffectConfig>;
	// artifact
	GetFun("Artifact") = ReloadFun<ArtifactConfig>;

	GetFun("AbyssParty") = ReloadFun<AbyssPartyMgr>;
	GetFun("Operating") = ReloadFun<OperatingActivityMgr>;

	GetFun("PeriodDesi") = ReloadFun<PeriodDesiMgr>;
	GetFun("Tarja") = ReloadFun<TarjaMgr>;
	// pet
	GetFun("PetInfo") = ReloadFun<PetConfigMgr>;
	// military
	GetFun("MilitaryRankByExploit") = ReloadFun<MilitaryRankManager>;
	// moba 
	GetFun("Moba") = ReloadFun<MobaBattleMgr>;
	// dragon help
	GetFun("dragonhelp") = ReloadFun<DragonHelpConfig>;
	
	GetFun("Designation") =  ReloadFun<DesignationMgr>;
	GetFun("FirstPass") =  ReloadFun<FirstPassConfig>;
	GetFun("Mayhem") = ReloadFun<MayhemConfigMgr>;
	// sprite
	GetFun("Sprite") = ReloadFun<SpriteConfig>;
	// live 
	GetFun("Live") = ReloadFun<LiveConfig>;

	// reswar
	GetFun("ResWar") = ReloadFun<ResWarAwardMgr>;
	// skycity
	GetFun("SkyCity") = ReloadFun<SkyCityConfig>;
    //GoalAwards
    GetFun("GoalAwards") = ReloadFun<GoalAwardsConfig>;
	// dragon
	GetFun("Dragon") = ReloadFun<DragonConfig>;
	// superactivity
	GetFun("SuperActivity") = ReloadFun<SpActivityConfig>;
	// bossrush
	GetFun("bossrush") = ReloadFun<BossRushMgr>;

	GetFun("BackFlowBuy") = ReloadFun<BackFlowBuyConfig>;

	GetFun("Qa") = ReloadFun<QAMgr>;

	GetFun("AttrMod") = ReloadFun<AttrModMgr>;

	GetFun("NestList") = ReloadFun<NestConfig>;

	GetFun("Partner") = ReloadFun<PartnerTableMgr>;

	GetFun("Ach") = ReloadFun<AchieveV2Mgr>;
	GetFun("BfReward") = ReloadFun<BattleFieldGSMgr>;
	GetFun("helldoor") = ReloadFun<HellDoorDropMgr>;
	// npc feeling
	GetFun("npcfeeling") = ReloadFun<CNpcFeelingMgr>;
	// doodaditemskill
	GetFun("doodaditemskill") = ReloadFun<CDoodadItemSkillConfig>;

	//龙本战队任务，成就
	GetFun("DragonGuild") = ReloadFun<DragonGuildTableMgr>;
	// jade
	GetFun("Jade") = ReloadFun<CJadeMgr>;

	GetFun("SceneTrans") = ReloadFun<SceneTransConfig>;

	GetFun("SceneLeisure") = ReloadFun<SceneLeisureConfig>;

	GetFun("Fuse") = ReloadFun<FuseMgr>;
	// 个人职业生涯
	GetFun("PersonalCareer") = ReloadFun<PersonalCareerQuest>;

	if (m_reloadMap.size() != GSReloadKey::GetInstance().size())
	{
		LogError("reloadMap size [%u] != GSReloadKey size [%u]", m_reloadMap.size(), GSReloadKey::GetInstance().size());
		return false;
	}

	return true;
}

void ReloadMgr::Uninit()
{

}

bool ReloadMgr::ReloadTable(UINT32 type, const std::string& cmd, std::string& strErrMsg)
{
	bool result = false;
	if (1 == type)
	{
		// skill
		std::string path = "SkillPackage/" + cmd;
		SkillReader::HotPatchSkill(path.c_str());
		result = true;
	}
	else if (2 == type)
	{
		// AI
		std::string path = "gsconf/table/AITree/" + cmd;
		AITreeMgr::Instance()->LoadOneAITree(path.c_str());
		result = true;
	}
	else if (3 == type)
	{
		// Level
		UINT32 sceneid = convert<UINT32>(cmd.c_str());
		SceneInfo* info = SceneConfig::Instance()->FindSceneInfo(sceneid);
		if (NULL != info)
		{
			XLevelSpawnMgr::Instance()->AddSceneStaticWaveInfo(sceneid, info->m_strSpawnConfig);
			result = true;
		}
	}
	else if (4 == type)
	{
		result = GetGlobalConfig().Reload(cmd);
	}
	else if (5 == type)
	{
		UINT32 args = convert<UINT32>(cmd);
		result = SpActivityMgr::Instance()->LoadFile(args);
	}
	else
	{
		auto iter = m_reloadMap.find(cmd);
		if(iter == m_reloadMap.end())
		{
			LogWarn("Invalid reload file: %s", cmd.c_str());
			strErrMsg = "Invalid reload file: " + cmd;
			return false;
		}
		result = iter->second();
	}

	if(result)
	{
		strErrMsg = "Reload " + cmd + " file succeed";
	}
	else
	{
		strErrMsg = "Reload " + cmd + " file failed";
	}

	return result;
}

ReloadMgr::Reload_Func& ReloadMgr::GetFun(const std::string& key)
{
	if (!GSReloadKey::GetInstance().IsIn(key))
	{
		LogError("key [%s] not in GSReloadKey's set", key.c_str());
		assert(false);
	}
	return m_reloadMap[key];
}
