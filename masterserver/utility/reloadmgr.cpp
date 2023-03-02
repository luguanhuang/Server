#include "pch.h"
#include "reloadmgr.h"
#include "util.h"
#include "config.h"
#include "common/rpcm2d_dbreload.h"
#include "common/rpcm2n_nsreload.h"
#include "common/rpcm2g_gsreload.h"
#include "network/dblink.h"
#include "network/controllink.h"
#include "network/gslink.h"
#include "scene/gsmanager.h"
#include "table/PlayerLevelManager.h"
#include "table/ItemConfigMgr.h"
#include "notice/noticemgr.h"
#include "table/guildauctconfig.h"
#include "levelseal/levelsealMgr.h"
#include "scene/sceneconfig.h"
#include "table/expeditionconfigmgr.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "config/itemdropconfig.h"
#include "table/auctionconfig.h"
#include "table/globalconfig.h"
#include "table/OpenSystemMgr.h"
#include "mail/mailconfig.h"
#include "table/MultActivityMgr.h"
#include "worldboss/worldbossmgr.h"
#include "guild/guildboss.h"
#include "table/guildConfig.h"
#include "guild/guildarena.h"
#include "garden/garden_config.h"
#include "tablemgr/skycrafttablemgr.h"
#include "custombattle/custombattleconfig.h"
#include "custombattle/custombattlemgr.h"
#include "table/PetConfigMgr.h"
#include "event/activitymailmgr.h"
#include "military/military_manager.h"
#include "firstpass/firstpassconfig.h"
#include "mobabattle/mobabattlemgr.h"
#include "herobattle/herobattlemgr.h"
#include "mayhem/mayhemmgr.h"
#include "table/SpriteConfigMgr.h"
#include "live/liveconfig.h"
#include "reswar/reswarpvemgr.h"
#include "skycitycfg/skycityconfig.h"
#include "qa/qamgr.h"
#include "tablemgr/partnertablemgr.h"
#include "goalawards/goalawardsmgr.h"
#include "survive/survivemgr.h"
#include "table/ConsumeRebateMgr.h"
#include "guildcastle/guildcastlemgr.h"
#include "tablemgr/dragonguildtablemgr.h"

INSTANCE_SINGLETON(CReloadMgr)


bool ReloadData::OnReloadReply(UINT32 delayid, const KKSG::GMToolCommandRes& res)
{
	replyData.push_back(res);
	if(replyData.size() >= dataNum)
	{
		CUserData data;
		data.m_pUserPtr = (void*)this;
		CRpc::ReplyDelayRpc(delayid, data);
		return true;
	}
	return false;
}

CReloadMgr::CReloadMgr()
{	
}

CReloadMgr::~CReloadMgr()
{
}

template<typename T>
bool ReloadFunWapper()
{
	return T::Instance()->LoadFile();
}

template<>
bool ReloadFunWapper<GlobalConfig>()
{
	return GetGlobalConfig().LoadFile();
}

bool CReloadMgr::Init()
{
	//ms
	RegisterReloadHandler("msconfig", ReloadServer_NONE, std::bind(&MSConfig::Reload, MSConfig::Instance()));
	RegisterReloadHandler("version", ReloadServer_NONE, std::bind(&MSConfig::HotLoadVersionFile, MSConfig::Instance()));

	RegisterReloadHandler("GuildAuctReward", ReloadServer_NONE, ReloadFunWapper<GuildAuctConfig>);
	RegisterReloadHandler("AuctSupplement", ReloadServer_NONE, ReloadFunWapper<AuctionConfig>);
	RegisterReloadHandler("Boss", ReloadServer_NONE, ReloadFunWapper<WorldBossMgr>);
	RegisterReloadHandler("CustomBattle", ReloadServer_NONE, ReloadFunWapper<CCustomBattleConfig>);
	RegisterReloadHandler("CustomBattleGM", ReloadServer_NONE, std::bind(&CReloadMgr::CustomBattleLoadGM, this));

	RegisterReloadHandler("Garden", ReloadServer_NONE, ReloadFunWapper<GardenConfig>);

	// ms 上的军衔奖励
	RegisterReloadHandler("MilitaryRankReward", ReloadServer_NONE, ReloadFunWapper<MilitaryManager>);

	RegisterReloadHandler("ActivityMail", ReloadServer_NONE, ReloadFunWapper<ActivityMailMgr>);
	RegisterReloadHandler("BMMaxPlayers", ReloadServer_NONE, ReloadFunWapper<MayhemMsMgr>);

	//绝地求生
	RegisterReloadHandler("survive", ReloadServer_NONE, ReloadFunWapper<CSurviveMgr>);

	RegisterReloadHandler("ClearGuildArenaPoint1", ReloadServer_NONE, std::bind(&CReloadMgr::ClearGuildArenaPoint1, this));
	RegisterReloadHandler("ClearGuildArenaPoint2", ReloadServer_NONE, std::bind(&CReloadMgr::ClearGuildArenaPoint2, this));
	RegisterReloadHandler("ClearGuildArenaPoint3", ReloadServer_NONE, std::bind(&CReloadMgr::ClearGuildArenaPoint3, this));
	RegisterReloadHandler("ClearGuildArenaPoint4", ReloadServer_NONE, std::bind(&CReloadMgr::ClearGuildArenaPoint4, this));

	//新服狂欢消费返利
	RegisterReloadHandler("ConsumeRebate", ReloadServer_NONE, ReloadFunWapper<CConsumeRebateConfig>);

	//ns
	RegisterReloadHandler("usewhitelist", ReloadServer_NS);
	RegisterReloadHandler("whitelist", ReloadServer_NS);
	RegisterReloadHandler("queuing", ReloadServer_NS);
	
	//db
	RegisterReloadHandler("dbloginlimit", ReloadServer_DB);
	RegisterReloadHandler("maxregisternum", ReloadServer_DB);

	// common
	RegisterReloadHandler("SceneList", ReloadServer_GS | ReloadServer_NS, ReloadFunWapper<CSceneConfig>);
	RegisterReloadHandler("GlobalConfig", ReloadServer_GS | ReloadServer_NS, ReloadFunWapper<GlobalConfig>);


	//gs
	// buff and skill
	RegisterReloadHandler("BuffList", ReloadServer_GS);
	RegisterReloadHandler("BuffListPVP", ReloadServer_GS);
	RegisterReloadHandler("SkillList", ReloadServer_GS);

	// scene
	RegisterReloadHandler("SceneView", ReloadServer_GS);
	RegisterReloadHandler("DNExpedition", ReloadServer_GS, ReloadFunWapper<ExpeditionConfigMgr>);
	RegisterReloadHandler("RandomSceneList", ReloadServer_GS, ReloadFunWapper<ExpeditionConfigMgr>);
	RegisterReloadHandler("PkParam", ReloadServer_GS);

	// enemy
	RegisterReloadHandler("XEntityPresentation", ReloadServer_GS, ReloadFunWapper<XEntityInfoLibrary>);
	RegisterReloadHandler("XEntityStatistics", ReloadServer_GS, ReloadFunWapper<XEntityInfoLibrary>);
	RegisterReloadHandler("GuildBossConfig", ReloadServer_GS, ReloadFunWapper<CGuildBossMgr>);

	// role
	RegisterReloadHandler("PkProfession", ReloadServer_GS);
	RegisterReloadHandler("PlayerLevel", ReloadServer_GS, ReloadFunWapper<PlayerLevelManager>);
	RegisterReloadHandler("Vip", ReloadServer_GS);
	RegisterReloadHandler("SystemList", ReloadServer_GS, ReloadFunWapper<OpenSystemMgr>);
	RegisterReloadHandler("TaskListNew", ReloadServer_GS);
	RegisterReloadHandler("AtkDefMod", ReloadServer_GS);
	RegisterReloadHandler("DefMod", ReloadServer_GS);

	// item
	RegisterReloadHandler("ItemList", ReloadServer_GS, ReloadFunWapper<CItemConfig>);
	RegisterReloadHandler("DoodadList", ReloadServer_GS);
	RegisterReloadHandler("EquipList", ReloadServer_GS);
	RegisterReloadHandler("DropList", ReloadServer_GS, ReloadFunWapper<ItemDropConfig>);
	RegisterReloadHandler("ChestList", ReloadServer_GS);
	RegisterReloadHandler("CircleDraw", ReloadServer_GS);
	RegisterReloadHandler("ItemCompose", ReloadServer_GS);
	RegisterReloadHandler("ItemBack", ReloadServer_GS);

	// emblem
	RegisterReloadHandler("AttributeEmblem", ReloadServer_GS);
	RegisterReloadHandler("SkillEmblem", ReloadServer_GS);

	// fashion
	RegisterReloadHandler("FashionList", ReloadServer_GS);
	RegisterReloadHandler("FashionCompose", ReloadServer_GS);

	// shop
	RegisterReloadHandler("Shop", ReloadServer_GS);
	RegisterReloadHandler("IBShop", ReloadServer_GS);

	// random attribute
	RegisterReloadHandler("RandomAttributes", ReloadServer_GS);
	RegisterReloadHandler("ForgeAttributes", ReloadServer_GS);
	RegisterReloadHandler("ForgeTransAttr", ReloadServer_GS);

	// other
	RegisterReloadHandler("Notice", ReloadServer_GS, ReloadFunWapper<NoticeMgr>);
	RegisterReloadHandler("LevelSealType", ReloadServer_GS, ReloadFunWapper<CLevelSealMgr>);
	RegisterReloadHandler("WorldLevelExpBuff", ReloadServer_GS);
	RegisterReloadHandler("MailContent", ReloadServer_GS, ReloadFunWapper<MailConfig>);
	RegisterReloadHandler("PayAileen", ReloadServer_GS);
	RegisterReloadHandler("MultiActivityList", ReloadServer_GS, ReloadFunWapper<MultActivityMgr>);
	RegisterReloadHandler("PandoraHeart", ReloadServer_GS);
	RegisterReloadHandler("FestivalSceneList", ReloadServer_GS);
	RegisterReloadHandler("StageRank", ReloadServer_GS);
	RegisterReloadHandler("StageAssist", ReloadServer_GS);
	RegisterReloadHandler("SkyCraft", ReloadServer_GS, ReloadFunWapper<SkyCraftTableMgr>);

	// channel
	RegisterReloadHandler("ChannelCloseSys", ReloadServer_GS);
	RegisterReloadHandler("AITree", ReloadServer_GS);
	RegisterReloadHandler("AIConfig", ReloadServer_GS);


	// sharegift
	RegisterReloadHandler("ShareRandomGift", ReloadServer_GS);

	// herobattle
	RegisterReloadHandler("HeroBattle", ReloadServer_GS, ReloadFunWapper<HeroBattleMgr>);
    // weekend4v4
    RegisterReloadHandler("WeekEnd4v4", ReloadServer_GS);
	// enchant
	RegisterReloadHandler("EnchantEquip", ReloadServer_GS);
	// spactivity
	RegisterReloadHandler("SpActivity_Argenta", ReloadServer_GS);
	// effect
	RegisterReloadHandler("EffectTable", ReloadServer_GS);
	// artifact
	RegisterReloadHandler("Artifact", ReloadServer_GS);

	// loginactivity
	RegisterReloadHandler("LoginActivity", ReloadServer_GS);


	RegisterReloadHandler("AbyssParty", ReloadServer_GS);
	RegisterReloadHandler("Operating", ReloadServer_GS);
	RegisterReloadHandler("PeriodDesi", ReloadServer_GS);
	RegisterReloadHandler("Tarja", ReloadServer_GS);
	RegisterReloadHandler("Designation", ReloadServer_GS);
	RegisterReloadHandler("Ach", ReloadServer_GS);
	RegisterReloadHandler("Mayhem", ReloadServer_GS);	// pet
	RegisterReloadHandler("PetInfo", ReloadServer_GS, ReloadFunWapper<CPetConfigMgr>);

	// team
	RegisterReloadHandler("dragonhelp", ReloadServer_GS);

	// first pass 
	RegisterReloadHandler("FirstPass", ReloadServer_GS, ReloadFunWapper<CFirstPassConfig>);
	
	// moba
	RegisterReloadHandler("Moba", ReloadServer_NONE, ReloadFunWapper<MobaBattleMgr>);

	// pet
	RegisterReloadHandler("Sprite", ReloadServer_GS, ReloadFunWapper<CSpriteConfigMgr>);

	// live
	RegisterReloadHandler("Live", ReloadServer_GS, ReloadFunWapper<LiveConfig>);

	// reswar
	RegisterReloadHandler("ResWar", ReloadServer_GS, ReloadFunWapper<ResWarAwardMgr>);

	// skycity
	RegisterReloadHandler("SkyCity", ReloadServer_GS, ReloadFunWapper<SkyCityConfig>);

    // GoalAwards
    RegisterReloadHandler("GoalAwards", ReloadServer_GS, ReloadFunWapper<GoalAwardsMgr>);

	// dragonconfig
	RegisterReloadHandler("Dragon", ReloadServer_GS);
	// spactivity
	RegisterReloadHandler("SuperActivity", ReloadServer_GS);
	// bossrush
	RegisterReloadHandler("bossrush", ReloadServer_GS);

	RegisterReloadHandler("BackFlowBuy", ReloadServer_GS);
	RegisterReloadHandler("Qa", ReloadServer_GS,ReloadFunWapper<QAMgr>);

	RegisterReloadHandler("AttrMod", ReloadServer_GS);

	RegisterReloadHandler("NestList", ReloadServer_GS);

	RegisterReloadHandler("Partner", ReloadServer_GS, ReloadFunWapper<PartnerTableMgr>);

	RegisterReloadHandler("BfReward", ReloadServer_GS);

	RegisterReloadHandler("helldoor", ReloadServer_GS);

	// npc feeling
	RegisterReloadHandler("npcfeeling", ReloadServer_GS);
	// doodad item skill
	RegisterReloadHandler("doodaditemskill", ReloadServer_GS);

	//龙本战队
	RegisterReloadHandler("DragonGuild", ReloadServer_GS, ReloadFunWapper<DragonGuildTableMgr>);

	RegisterReloadHandler("guildcastle", ReloadServer_NONE, ReloadFunWapper<GuildCastleMgr>);

	RegisterReloadHandler("SceneTrans", ReloadServer_GS);
	RegisterReloadHandler("SceneLeisure", ReloadServer_GS);
	// jade
	RegisterReloadHandler("Jade", ReloadServer_GS);
	// fuse
	RegisterReloadHandler("Fuse", ReloadServer_GS);

	// 工会营地活动
	RegisterReloadHandler("GuildCampParty", ReloadServer_GS);
	// 职业生涯奖杯
	RegisterReloadHandler("PersonalCareer", ReloadServer_GS);

	return true;
}

void CReloadMgr::Uninit()
{
}

ReloadErrorCode CReloadMgr::ReloadNew(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid)
{
	// 特殊处理
	if (type != 0)
	{
		if (type == 4)
		{
			GetGlobalConfig().Reload(strParam);
		}
		UINT32 count = _ReloadGs(type, strParam, strErrMsg, delayid);
		if (count == 0)
		{
			return Reload_Failed;
		}
		ReloadData data(count);
		m_reloadDataMap.insert(std::make_pair(delayid, data));
		return Reload_DelayAndSucceed;
	}

	auto it = m_handlers.find(strParam);
	if (it == m_handlers.end())
	{
		strErrMsg = "[MsReload] Invalid reload file: " + strParam;
		LogWarn("%s", strErrMsg.c_str());	
		return Reload_Failed;
	}

	// 执行MS上的处理
	bool result = true;
	if (it->second.func)
	{
		result = it->second.func();
		if(result)
		{
			strErrMsg = "[MsReload] Reload " + strParam + " file succeed\n";
			LogInfo("%s", strErrMsg.c_str());
		}
		else
		{
			strErrMsg = "[MsReload] Reload " + strParam + " file failed\n";
			LogWarn("%s", strErrMsg.c_str());
		}
	}

	UINT32 sum = 0;
	bool hasFail = false;
	for (UINT32 i = 0; i < 32; ++i)
	{
		UINT32 mask = 1 << i;
		if (mask & it->second.serverType)
		{
			UINT32 count = 0;
			switch (mask)
			{
			case ReloadServer_GS:
				count = _ReloadGs(type, strParam, strErrMsg, delayid);
				break;
			case ReloadServer_DB:
				count = _ReloadDb(type, strParam, strErrMsg, delayid);
				break;
			case ReloadServer_NS:
				count = _ReloadNs(type, strParam, strErrMsg, delayid);
				break;
			default:
				LogInfo("invalid server type[%u]", mask);
				continue;
			}
			if (count == 0)
			{
				hasFail = true;
			}
			sum += count;
		}
	}

	if (sum)
	{
		ReloadData data(sum);
		m_reloadDataMap.insert(std::make_pair(delayid, data));
	}

	if (hasFail)
	{
		if (sum)
		{
			return Reload_DelayAndFailed;
		}
		return Reload_Failed;
	}
	else
	{
		if (sum)
		{
			return Reload_DelayAndSucceed;
		}
		return Reload_Succeed;
	}
}

UINT32 CReloadMgr::_ReloadNs(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid)
{
	if(!ControlLink::Instance()->IsConnected())
	{
		string errMsg = "[NsReload] Ns is disconnected\n";
		LogWarn("%s", errMsg.c_str());
		strErrMsg += errMsg;
		return 0;
	}

	RpcM2N_NsReload *rpc = RpcM2N_NsReload::CreateRpc();
	rpc->m_oArg.set_cmd(strParam);
	rpc->m_oArg.set_delayid(delayid);	
	ControlLink::Instance()->SendTo(*rpc);
	return 1;
}

UINT32 CReloadMgr::_ReloadDb(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid)
{
	if(!DBLink::Instance()->IsConnected())
	{
		string errMsg = "[DbReload] Db is disconnected\n";
		LogWarn("%s", errMsg.c_str());
		strErrMsg += errMsg;
		return 0;
	}

	RpcM2D_DbReload *rpc = RpcM2D_DbReload::CreateRpc();
	rpc->m_oArg.set_cmd(strParam);
	rpc->m_oArg.set_delayid(delayid);
	DBLink::Instance()->SendTo(*rpc);
	return 1;
}

UINT32 CReloadMgr::_ReloadGs(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid)
{
	UINT32 count = 0;
	for(auto iter = CGsManager::Instance()->Begin(); iter != CGsManager::Instance()->End(); iter++)
	{
		RpcM2G_GsReload *rpc = RpcM2G_GsReload::CreateRpc();
		rpc->m_oArg.set_cmd(strParam);
		rpc->m_oArg.set_type(type);
		rpc->m_oArg.set_delayid(delayid);
		GSLink::Instance()->SendToLine(iter->first, *rpc);
		count++;
	}
	if(count == 0)
	{
		std::string ss = "[GsReload] Gs is disconnected\n";
		LogWarn("%s", ss.c_str());
		strErrMsg += ss;
	}

	return count;
}

void CReloadMgr::OnReloadDelayReply(UINT32 delayid, const KKSG::GMToolCommandRes& res)
{
	auto iter = m_reloadDataMap.find(delayid);
	if(iter == m_reloadDataMap.end())
	{
		LogError("can't find delayid:%u in reloadDataMap", delayid);
		assert(false);
		return;
	}
	if(iter->second.OnReloadReply(delayid, res))
	{
		m_reloadDataMap.erase(iter);
	}
}

bool CReloadMgr::CustomBattleLoadGM()
{
	CustomBattleMgr::Instance()->LoadGM();
	return true;
}

bool CReloadMgr::ClearGuildArenaPoint1()
{
	CGuildArenaIntegralMgr::Instance()->ClearScoreByErrorReason(1);
	return true;
}

bool CReloadMgr::ClearGuildArenaPoint2()
{
	CGuildArenaIntegralMgr::Instance()->ClearScoreByErrorReason(2);
	return true;
}

bool CReloadMgr::ClearGuildArenaPoint3()
{
	CGuildArenaIntegralMgr::Instance()->ClearScoreByErrorReason(3);
	return true;
}

bool CReloadMgr::ClearGuildArenaPoint4()
{
	CGuildArenaIntegralMgr::Instance()->ClearScoreByErrorReason(4);
	return true;
}

void CReloadMgr::RegisterReloadHandler(const std::string& name, UINT32 type, ReloadFunc func)
{
	ReloadHandler handler;
	handler.serverType = type;
	handler.func = func;
	m_handlers[name] = handler;
}

bool ReloadNoUse()
{
	return true;
}
