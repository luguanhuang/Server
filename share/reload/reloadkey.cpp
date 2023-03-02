#include "pch.h"
#include "reloadkey.h"

GSReloadKey::GSReloadKey()
{
#define Add(x) m_reloadKey.insert(x)

	Add("BuffList");
	Add("BuffListPVP");
	Add("SkillList");

	Add("SceneView");
	Add("SceneList");
	Add("DNExpedition");
	Add("RandomSceneList");
	Add("PkParam");
	Add("GuildBossConfig");
	//Add("LevelDir");

	Add("XEntityPresentation");
	Add("XEntityStatistics");

	Add("PkProfession");
	Add("PlayerLevel");
	Add("Vip");
	Add("SystemList");
	Add("TaskListNew");
	Add("AtkDefMod");
	Add("DefMod");

	Add("ItemList");
	Add("ChestList");
	Add("DoodadList");
	Add("EquipList");
	Add("CircleDraw");
	Add("FashionList");
	Add("ItemCompose");

	Add("RandomAttributes");
	Add("ForgeAttributes");
	Add("ForgeTransAttr");

	Add("AttributeEmblem");
	Add("SkillEmblem");

	Add("ItemBack");
	Add("DropList");
	Add("FashionCompose");

	Add("Shop");
	Add("IBShop");

	Add("Notice");
	Add("LevelSealType");
	Add("GlobalConfig");
	Add("WorldLevelExpBuff");
	Add("MailContent");
	Add("PayAileen");
	Add("MultiActivityList");
	Add("GuildConfig");
	Add("PandoraHeart");
	Add("AITree");
	Add("AIConfig");
	Add("ChannelCloseSys");
	Add("FestivalSceneList");
	Add("StageRank");
	Add("StageAssist");
	Add("ShareRandomGift");
	// herobattle
	Add("HeroBattle");
	Add("WeekEnd4v4");

	Add("DragonGroupMgr");

	// enchant
	Add("EnchantEquip");
	// µÇÂ¼»î¶¯
	Add("LoginActivity");
	Add("SkyCraft");

	Add("GuildCampParty");
	// effect
	Add("EffectTable");
	// artifact
	Add("Artifact");

	Add("AbyssParty");
	Add("Operating");

	Add("PeriodDesi");
	Add("Tarja");
	// pet
	Add("PetInfo");
	// military
	Add("MilitaryRankByExploit");
	// moba
	Add("Moba");
	// dragon help
	Add("dragonhelp");

	Add("Designation");
	Add("FirstPass");
	Add("Mayhem");
	Add("Sprite");
	Add("Live");

	Add("ResWar");
	Add("SkyCity");
	Add("Dragon");
	Add("SuperActivity");
	Add("bossrush");
    Add("GoalAwards");

	Add("BackFlowBuy");
	Add("Ach");

	Add("Qa");

	Add("AttrMod");

	Add("NestList");

	Add("Partner");

	Add("BfReward");
	Add("helldoor");
	// npc feeling
	Add("npcfeeling");
	// doodad item skill
	Add("doodaditemskill");
	// dragonguild
	Add("DragonGuild");
	Add("SceneTrans");
	Add("SceneLeisure");
	// jade
	Add("Jade");
	// fuse
	Add("Fuse");
	// PersonalCareerQuest
	Add("PersonalCareer");

#undef Add
}

GSReloadKey::~GSReloadKey()
{

}
