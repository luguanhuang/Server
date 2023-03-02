#include "pch.h"
#include "tloggerbattle.h"
#include "scene/scene.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "appid.h"
#include "gamelogic/pkrecord.h"
#include "gamelogic/skilllevel.h"
#include "table/XSkillEffectMgr.h"
#include "gamelogic/towerrecord.h"
#include "loghelper.h"
#include "scene/scenestatistics.h"
#include "gamelogic/itemconfig.h"
#include "gamelogic/XLevelSpawnMgr.h"
#include "skill/XSkillCore.h"
#include "skill/XSkillMgr.h"
#include "entity/XRole.h"
#include "component/XSkillComponent.h"
#include "utility/tlogger.h"

std::map<std::string, std::string> TSecBattleFlow::m_specaildefault;
void TSecBattleFlow::Send(const std::vector<std::string>& fields, const std::map<std::string, int>& handles)
{
	//SSDebug<<"battle flow table:"<<GetTableName()<<END;

	LogHelper log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	for (auto i = fields.begin(); i != fields.end(); ++i)
	{
		auto j = m_data.find(*i);
		if (j == m_data.end())
		{
			// default
			auto k = m_specaildefault.find(*i);
			if (k == m_specaildefault.end())
			{
				log.AddKeyValue((*i).c_str(), "0");
			}
			else
			{
				log.AddKeyValue((*i).c_str(), k->second);
			}

			//SSDebug<<"no field:"<<*i<<" table:"<<GetTableName()<<END;
		}
		else
		{
			auto k = handles.find(*i);
			if (k != handles.end())
			{
				if (1 == k->second)
				{
					log.AddKeyValue((*i).c_str(), j->second);
				}
				else if (2 == k->second)
				{
					log.AddKeyValueTime((*i).c_str(), convert<UINT32>(j->second));
				}
				//SSDebug<<"field:"<<*i<<" value:"<<j->second<<END;
			}
		}
	}
	log.Send();
}

void TSecBattleFlow::FillCommon(Scene* scene)
{
	m_data["GameSvrId"] = ToString(m_poRole->GetServerId());
	m_data["dtEventTime"] = ToString(time(NULL));
	m_data["GameAppID"] = ToString(GSConfig::Instance()->GetAppType()==KKSG::GAME_APP_QQ?MobileQQ_App_ID:WeChat_App_ID);
	m_data["OpenID"] = m_poRole->GetAccount();
	m_data["PlatID"] = ToString(m_poRole->GetClientConf().platid());
	m_data["AreaID"] = ToString(GSConfig::Instance()->GetAppType());
	m_data["ZoneID"] = ToString(m_poRole->GetServerId());
	m_data["BattleID"] = ToString(scene->GetSceneID());
}

//////////////////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> TSecRoundStartFlow::m_field2handler;
std::vector<std::string> TSecRoundStartFlow::m_fields;
bool TSecRoundStartFlow::Init()
{
	// fields
	m_fields.push_back("GameSvrId");
	m_fields.push_back("dtEventTime");
	m_fields.push_back("GameAppID");
	m_fields.push_back("OpenID");
	m_fields.push_back("PlatID");
	m_fields.push_back("AreaID");
	m_fields.push_back("ZoneID");
	m_fields.push_back("BattleID");
	m_fields.push_back("ClientStartTime");
	m_fields.push_back("UserName");
	m_fields.push_back("PicUrl");
	m_fields.push_back("SvrUserMoney1");
	m_fields.push_back("SvrUserMoney2");
	m_fields.push_back("SvrUserMoney3");
	m_fields.push_back("SvrUserMoney4");
	m_fields.push_back("SvrUserMoney5");
	m_fields.push_back("SvrUserMoney6");
	m_fields.push_back("SvrUserKEY1");
	m_fields.push_back("SvrRoundRankPoint");
	m_fields.push_back("SvrRoundRank1");
	m_fields.push_back("SvrRoundRank2");

	m_fields.push_back("SvrMapid");
	m_fields.push_back("SvrMapidBattlePoint");
	m_fields.push_back("SvrRoundType");
	m_fields.push_back("SvrRoundMode");
	m_fields.push_back("SvrEndlessFlag1");
	m_fields.push_back("SvrEndlessFlag2");
	m_fields.push_back("SvrRoleID");
	m_fields.push_back("SvrRoleType");
	m_fields.push_back("SvrRoleLevel");
	m_fields.push_back("SvrRoleBattlePoint");
	m_fields.push_back("SvrRoleCharm");

	m_fields.push_back("SvrRoleAtk");
	m_fields.push_back("SvrRoleMAtk");
	m_fields.push_back("SvrRoleHP");
	m_fields.push_back("SvrRoleMP");
	m_fields.push_back("SvrRoleDef");
	m_fields.push_back("SvrRoleMDef");
	m_fields.push_back("SvrRoleinf1");
	m_fields.push_back("SvrRoleinf2");
	m_fields.push_back("SvrRoleinf3");

	m_fields.push_back("SvrRoleSkillType1");
	m_fields.push_back("SvrRoleSkillCD1");
	m_fields.push_back("SvrRoleSkillLv1");
	m_fields.push_back("SvrRoleSkillType2");
	m_fields.push_back("SvrRoleSkillCD2");
	m_fields.push_back("SvrRoleSkillLv2");
	m_fields.push_back("SvrRoleSkillType3");
	m_fields.push_back("SvrRoleSkillCD3");
	m_fields.push_back("SvrRoleSkillLv3");
	m_fields.push_back("SvrRoleSkillType4");
	m_fields.push_back("SvrRoleSkillCD4");
	m_fields.push_back("SvrRoleSkillLv4");
	m_fields.push_back("SvrRoleSkillType5");
	m_fields.push_back("SvrRoleSkillCD5");
	m_fields.push_back("SvrRoleSkillLv5");
	m_fields.push_back("SvrRoleSkillType6");
	m_fields.push_back("SvrRoleSkillCD6");
	m_fields.push_back("SvrRoleSkillLv6");
	m_fields.push_back("SvrRoleSkillType7");
	m_fields.push_back("SvrRoleSkillCD7");
	m_fields.push_back("SvrRoleSkillLv7");
	m_fields.push_back("SvrRoleSkillType8");
	m_fields.push_back("SvrRoleSkillCD8");
	m_fields.push_back("SvrRoleSkillLv8");
	m_fields.push_back("SvrRoleSkillType9");
	m_fields.push_back("SvrRoleSkillCD9");
	m_fields.push_back("SvrRoleSkillLv9");

	m_fields.push_back("Player1OpenID");
	m_fields.push_back("Player1Side");
	m_fields.push_back("Player1Type");
	m_fields.push_back("Player1BattlePoint");
	m_fields.push_back("Player2OpenID");
	m_fields.push_back("Player2Side");
	m_fields.push_back("Player2Type");
	m_fields.push_back("Player2BattlePoint");
	m_fields.push_back("Player3OpenID");
	m_fields.push_back("Player3Side");
	m_fields.push_back("Player3Type");
	m_fields.push_back("Player3BattlePoint");
	m_fields.push_back("Player4OpenID");
	m_fields.push_back("Player4Side");
	m_fields.push_back("Player4Type");
	m_fields.push_back("Player4BattlePoint");
	m_fields.push_back("Player5OpenID");
	m_fields.push_back("Player5Side");
	m_fields.push_back("Player5Type");
	m_fields.push_back("Player5BattlePoint");
	m_fields.push_back("Player6OpenID");
	m_fields.push_back("Player6Side");
	m_fields.push_back("Player6Type");
	m_fields.push_back("Player6BattlePoint");
	m_fields.push_back("Player7OpenID");
	m_fields.push_back("Player7Side");
	m_fields.push_back("Player7Type");
	m_fields.push_back("Player7BattlePoint");

	m_fields.push_back("SvrMonsterCount");
	m_fields.push_back("SvrBossType");
	m_fields.push_back("SvrMonsterAtkMAX");
	m_fields.push_back("SvrMonsterSkillMAX");
	m_fields.push_back("SvrMonsterHpMax");
	m_fields.push_back("SvrMonsterHpMin");
	m_fields.push_back("SvrBossAtkMAX");
	m_fields.push_back("SvrBossSkillMAX");
	m_fields.push_back("SvrBossHpMax");
	m_fields.push_back("SvrBossHpMin");


	m_specaildefault["ClientStartTime"] = "0-0-0 0:0:0";

	// handler
	for (auto i = m_fields.begin(); i != m_fields.end(); ++i)
	{
		if (*i == "dtEventTime")
		{
			m_field2handler[*i] = 2;
		}
		else
		{
			m_field2handler[*i] = 1;
		}
	}
	return true;
}

bool TSecRoundStartFlow::FillPkRank(const std::string& rank)
{
	m_data["SvrRoundRank1"]	= rank;
	return true;
}

bool TSecRoundStartFlow::FillAttr()
{
	m_data["SvrRoleAtk"] = ToString((int)m_poRole->GetAttr(TOTAL_PhysicalAtkMod));			
	m_data["SvrRoleMAtk"] = ToString((int)m_poRole->GetAttr(TOTAL_MagicAtkMod));			
	m_data["SvrRoleHP"]	 = ToString((int)m_poRole->GetAttr(TOTAL_MaxHP));			
	m_data["SvrRoleMP"]	 = ToString((int)m_poRole->GetAttr(TOTAL_MaxMP));		    
	m_data["SvrRoleDef"] = ToString((int)m_poRole->GetAttr(TOTAL_PhysicalDefMod));		
	m_data["SvrRoleMDef"]  = ToString((int)m_poRole->GetAttr(TOTAL_MagicDefMod));			

	///> ×îÖÕÉËº¦£¬ÖÂÃüÒ»»÷£¬ÖÂÃüÒ»»÷ÉËº¦£¬ÖÂÃüÒ»»÷µÖ¿¹£¬Ó²Ö±£¬Ó²Ö±µÖ¿¹£¬Ñ£ÔÎ£¬Ñ£ÔÎµÖ¿¹
	m_data["SvrRoleinf1"] = ToString((int)m_poRole->GetAttr(TOTAL_FinalDamage)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_Critical)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_CritDamage)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_CritResist)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_Paralyze)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_ParaResist)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_Stun)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_StunResist));
	///> »ð¹¥»÷Á¦£¬Ë®¹¥»÷£¬¹â¹¥»÷£¬°µ¹¥»÷£¬»ðµÖ¿¹£¬Ë®µÖ¿¹£¬¹âµÖ¿¹£¬°µµÖ¿¹
	m_data["SvrRoleinf2"] = ToString((int)m_poRole->GetAttr(TOTAL_FireAtk)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_WaterAtk)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_LightAtk)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_DarkAtk)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_FireDef)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_WaterDef)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_LightDef)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_DarkDef));
	///> Á¦Á¿£¬Ãô½Ý£¬ÖÇÁ¦£¬ÌåÖÊ
	m_data["SvrRoleinf3"] = ToString((int)m_poRole->GetAttr(TOTAL_Strength)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_Agility)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_Intelligence)) + "," +
							ToString((int)m_poRole->GetAttr(TOTAL_Vitality));
	return true;
}

bool TSecRoundStartFlow::FillSkill()
{
	RoleSkillSet* record = m_poRole->Get<RoleSkillSet>();
	std::string SvrRoleSkillType = "SvrRoleSkillType";
	std::string SvrRoleSkillCD = "SvrRoleSkillCD";
	std::string SvrRoleSkillLv = "SvrRoleSkillLv";
	for (int i = 1; i <= 9; ++i)
	{
		skillInfo* info = record->GetSkillBySlot(i - 1); 
		if (NULL != info)
		{
			m_data[SvrRoleSkillType + ToString(i)] = ToString(info->skillhash);
			m_data[SvrRoleSkillLv + ToString(i)] = ToString(info->skilllevel);				

			int cd = 0;
			XSkillComponent* xskill = m_poRole->GetXRole()->SkillComponent();
			if (NULL != xskill && NULL != xskill->SkillManager())
			{
				XSkillCore* pCore = xskill->SkillManager()->GetSkill(info->skillhash);
				if (NULL != pCore)
				{
					cd = pCore->GetCoolDown();
				}
			}
			m_data[SvrRoleSkillCD + ToString(i)] = ToString(cd);				
		}
		else
		{
			m_data[SvrRoleSkillType + ToString(i)] = "0";
			m_data[SvrRoleSkillLv + ToString(i)] = "0";				
			m_data[SvrRoleSkillCD + ToString(i)] = "0";
		}
	}
	return true;
}

bool TSecRoundStartFlow::FillPlayers(UINT32 fightgroup, const std::map<UINT64, PlayerInfo>& players)
{
	if (players.size() > 8)
	{
		SSWarn<<"players out of range:"<<players.size()<<END;
		return  false;
	}

	std::string Player = "Player";
	std::string OpenID = "OpenID";
	std::string Side = "Side";
	std::string Type = "Type";
	std::string BattlePoint = "BattlePoint";

	UINT32 count = 1;
	for (auto i = players.begin(); i != players.end(); ++i)
	{
		std::string PlayerOpenID = Player + ToString(count) + OpenID;
		std::string PlayerSide = Player + ToString(count) + Side;
		std::string PlayerType = Player + ToString(count) + Type;
		std::string PlayerBattlePoint = Player + ToString(count) + BattlePoint;
		const PlayerInfo& info = i->second;
		if (info.roleid == m_poRole->GetID())
		{
			continue;
		}
		if (count == 8)
		{
			break;
		}
		m_data[PlayerOpenID] = info.PlayerOpenID;
		m_data[PlayerSide] = (fightgroup == info.PlayerSide) ? "2" : "1";
		m_data[PlayerType] = ToString(info.PlayerType);
		m_data[PlayerBattlePoint] = ToString(info.PlayerBattlePoint);
		++count;
	}
	return true;
}

bool TSecRoundStartFlow::FillMonster(Scene* scene)
{
	XLevelStatistic* statistic = XLevelSpawnMgr::Instance()->GetStatistic(scene->GetSceneTemplateID());
	if (NULL != statistic)
	{
		m_data["SvrMonsterCount"] = ToString(statistic->SvrMonsterCount);
		m_data["SvrBossType"] = ToString(statistic->SvrBossType);
		m_data["SvrMonsterAtkMAX"] = ToString(statistic->SvrMonsterAtkMAX);
		m_data["SvrMonsterSkillMAX"] = ToString(statistic->SvrMonsterSkillMAX);
		m_data["SvrMonsterHpMax"] = ToString(statistic->SvrMonsterHpMax);
		m_data["SvrMonsterHpMin"] = ToString(statistic->SvrMonsterHpMin);
		m_data["SvrBossAtkMAX"] = ToString(statistic->SvrBossAtkMAX);
		m_data["SvrBossSkillMAX"] = ToString(statistic->SvrBossSkillMAX);
		m_data["SvrBossHpMax"] = ToString(statistic->SvrBossHpMax);
		m_data["SvrBossHpMin"] = ToString(statistic->SvrBossHpMin);
	}
	else
	{
		m_data["SvrMonsterCount"] = "0";
		m_data["SvrBossType"] = "0";
		m_data["SvrMonsterAtkMAX"] = "0";
		m_data["SvrMonsterSkillMAX"] = "0";
		m_data["SvrMonsterHpMax"] = "0";
		m_data["SvrMonsterHpMin"] = "0";
		m_data["SvrBossAtkMAX"] = "0";
		m_data["SvrBossSkillMAX"] = "0";
		m_data["SvrBossHpMax"] = "0";
		m_data["SvrBossHpMin"] = "0";
	}
	return true;
}

const int Item_Goddess = 88;
const int Item_Dragon = 85;

bool TSecRoundStartFlow::FillBegin(Scene* scene)
{
	FillCommon(scene);
	m_data["ClientStartTime"] = "0";

	m_data["UserName"] = ProcessRoleName(m_poRole->GetName());

	m_data["PicUrl"] = m_poRole->GetRoleAllInfo().idiprecord().picurl();

	m_data["SvrUserMoney1"] = ToString(m_poRole->Get<Bag>()->CountVirtualItem(GOLD));
	m_data["SvrUserMoney2"]	= ToString(m_poRole->Get<Bag>()->CountVirtualItem(DIAMOND));	
	m_data["SvrUserMoney3"]	= ToString(m_poRole->Get<Bag>()->CountVirtualItem(FATIGUE));		
	m_data["SvrUserMoney4"]	= ToString(m_poRole->Get<Bag>()->CountVirtualItem(DRAGON_COIN));		
	m_data["SvrUserMoney5"]	= ToString(m_poRole->Get<Bag>()->CountVirtualItem(ARENA_COIN));		
	m_data["SvrUserMoney6"]	= ToString(m_poRole->Get<Bag>()->CountItemInBag(Item_Goddess)); ///> Å®ÉñÑÛÀá	
	m_data["SvrUserKEY1"]	= ToString(m_poRole->Get<Bag>()->CountItemInBag(Item_Dragon));  ///> Áú±¾ÃÅÆ±
	m_data["SvrRoundRankPoint"] = ToString(m_poRole->Get<CPkRecord>()->GetPoint());
	m_data["SvrRoundRank1"] = "0";
	m_data["SvrRoundRank2"] = "0";

	m_data["SvrMapid"] = ToString(scene->GetSceneTemplateID());
	m_data["SvrMapidBattlePoint"] = ToString(scene->GetSceneInfo()->m_pConf->RecommendPower);
	m_data["SvrRoundType"] = ToString(scene->GetSceneType());

	m_data["SvrRoundMode"] = "0";
	const RoleTowerInfo* towerinfo = m_poRole->Get<CTowerRecord>()->GetTowerInfo(DEFAULT_HARDLEVEL);
	m_data["SvrEndlessFlag1"] = ToString(NULL == towerinfo ? 0 : towerinfo->reachTopFloor);
	m_data["SvrEndlessFlag2"] = "0"; 

	m_data["SvrRoleID"] = ToString(m_poRole->GetID());				
	m_data["SvrRoleType"] = ToString(m_poRole->GetProfession());			
	m_data["SvrRoleLevel"]= ToString(m_poRole->GetLevel());
	m_data["SvrRoleBattlePoint"] = ToString((int)m_poRole->GetAttr(TOTAL_POWERPOINT));
	m_data["SvrRoleCharm"] = "0";

	FillAttr();
	
	FillSkill();

	FillMonster(scene);

	m_data["Player1OpenID"] = "0";
	m_data["Player1Side"] = "0";
	m_data["Player1Type"] = "0";
	m_data["Player1BattlePoint"] = "0";
	m_data["Player2OpenID"] = "0";
	m_data["Player2Side"] = "0";
	m_data["Player2Type"] = "0";
	m_data["Player2BattlePoint"] = "0";
	m_data["Player3OpenID"] = "0";
	m_data["Player3Side"] = "0";
	m_data["Player3Type"] = "0";
	m_data["Player3BattlePoint"] = "0";
	m_data["Player4OpenID"] = "0";
	m_data["Player4Side"] = "0";
	m_data["Player4Type"] = "0";
	m_data["Player4BattlePoint"] = "0";
	m_data["Player5OpenID"] = "0";
	m_data["Player5Side"] = "0";
	m_data["Player5Type"] = "0";
	m_data["Player5BattlePoint"] = "0";
	m_data["Player6OpenID"] = "0";
	m_data["Player6Side"] = "0";
	m_data["Player6Type"] = "0";
	m_data["Player6BattlePoint"] = "0";
	m_data["Player7OpenID"] = "0";
	m_data["Player7Side"] = "0";
	m_data["Player7Type"] = "0";
	m_data["Player7BattlePoint"] = "0";

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> TSecRoundEndFlow::m_field2handler;
std::vector<std::string> TSecRoundEndFlow::m_fields;
bool TSecRoundEndFlow::Init()
{
	// fields
	m_fields.push_back("GameSvrId");
	m_fields.push_back("dtEventTime");
	m_fields.push_back("GameAppID");
	m_fields.push_back("OpenID");
	m_fields.push_back("PlatID");
	m_fields.push_back("AreaID");
	m_fields.push_back("ZoneID");
	m_fields.push_back("BattleID");
	m_fields.push_back("ClientEndTime");
	m_fields.push_back("ClientVersion");
	m_fields.push_back("UserIP");
	m_fields.push_back("Result");

	m_fields.push_back("RoleAtk");
	m_fields.push_back("RoleMAtk");
	m_fields.push_back("RoleHP");
	m_fields.push_back("RoleMP");
	m_fields.push_back("RoleDef");
	m_fields.push_back("RoleMDef");
	m_fields.push_back("Roleinf1");
	m_fields.push_back("Roleinf2");
	m_fields.push_back("Roleinf3");

	m_fields.push_back("RoundEndType");
	m_fields.push_back("RoundTimeUse");
	m_fields.push_back("RoundRate");
	m_fields.push_back("EndLessFloor");
	m_fields.push_back("RoundNScore");
	m_fields.push_back("RoundDScore");
	m_fields.push_back("RoundExp");
	m_fields.push_back("Roundmoney");
	m_fields.push_back("RoundDB");
	m_fields.push_back("RoundHB");
	m_fields.push_back("RoundGB");
	m_fields.push_back("RoundGT");
	m_fields.push_back("UserMoney1");
	m_fields.push_back("UserMoney2");
	m_fields.push_back("UserMoney3");
	m_fields.push_back("UserMoney4");
	m_fields.push_back("UserMoney5");
	m_fields.push_back("UserMoney6");
	m_fields.push_back("UserKEY1");
	m_fields.push_back("RoundRankup");
	m_fields.push_back("RoundRankPoint");
	m_fields.push_back("RoundRank1");
	m_fields.push_back("RoundRank2");
	m_fields.push_back("DropItemType1");
	m_fields.push_back("DropItemUuid1");
	m_fields.push_back("DropItemType2");
	m_fields.push_back("DropItemTypeCount");

	m_fields.push_back("MoveTotal");
	m_fields.push_back("ButtonClickCount1");
	m_fields.push_back("ButtonClickCount2");
	m_fields.push_back("ButtonClickCount3");
	m_fields.push_back("ButtonClickCount4");
	m_fields.push_back("ButtonClickCount5");
	m_fields.push_back("ButtonClickCount6");
	m_fields.push_back("ButtonClickCount7");
	m_fields.push_back("ButtonClickCount8");
	m_fields.push_back("ButtonClickCount9");
	m_fields.push_back("ButtonClickCount10");
	m_fields.push_back("ButtonClickCountQTE");

	m_fields.push_back("PlayerDpsCount");
	m_fields.push_back("PlayerAtkMissTotal");
	m_fields.push_back("PlayerCritCount");
	m_fields.push_back("PlayerDamageMax");
	m_fields.push_back("PlayerDamageMin");
	m_fields.push_back("PlayerDpsTotal");

	m_fields.push_back("PlayerATKMax");
	m_fields.push_back("PlayerATKMin");
	m_fields.push_back("PlayerCritATKMax");
	m_fields.push_back("PlayerCritATKMin");
	m_fields.push_back("PlayerAtkTag");
	m_fields.push_back("PlayerAtkCount");
	m_fields.push_back("PlayerAtkTotal");

	m_fields.push_back("PlayerSkillCount1");
	m_fields.push_back("PlayerSkillCD1");
	m_fields.push_back("PlayerSkillHitCount1");
	m_fields.push_back("PlayerSkillMax1");
	m_fields.push_back("PlayerSkillMin1");
	m_fields.push_back("PlayerCritSkillMax1");
	m_fields.push_back("PlayerCritSkillMin1");
	m_fields.push_back("PlayerSkillTag1");
	m_fields.push_back("PlayerSkillDPS1");
	m_fields.push_back("PlayerSkillCount2");
	m_fields.push_back("PlayerSkillCD2");
	m_fields.push_back("PlayerSkillHitCount2");
	m_fields.push_back("PlayerSkillMax2");
	m_fields.push_back("PlayerSkillMin2");
	m_fields.push_back("PlayerCritSkillMax2");
	m_fields.push_back("PlayerCritSkillMin2");
	m_fields.push_back("PlayerSkillTag2");
	m_fields.push_back("PlayerSkillDPS2");
	m_fields.push_back("PlayerSkillCount3");
	m_fields.push_back("PlayerSkillCD3");
	m_fields.push_back("PlayerSkillHitCount3");
	m_fields.push_back("PlayerSkillMax3");
	m_fields.push_back("PlayerSkillMin3");
	m_fields.push_back("PlayerCritSkillMax3");
	m_fields.push_back("PlayerCritSkillMin3");
	m_fields.push_back("PlayerSkillTag3");
	m_fields.push_back("PlayerSkillDPS3");
	m_fields.push_back("PlayerSkillCount4");
	m_fields.push_back("PlayerSkillCD4");
	m_fields.push_back("PlayerSkillHitCount4");
	m_fields.push_back("PlayerSkillMax4");
	m_fields.push_back("PlayerSkillMin4");
	m_fields.push_back("PlayerCritSkillMax4");
	m_fields.push_back("PlayerCritSkillMin4");
	m_fields.push_back("PlayerSkillTag4");
	m_fields.push_back("PlayerSkillDPS4");
	m_fields.push_back("PlayerSkillCount5");
	m_fields.push_back("PlayerSkillCD5");
	m_fields.push_back("PlayerSkillHitCount5");
	m_fields.push_back("PlayerSkillMax5");
	m_fields.push_back("PlayerSkillMin5");
	m_fields.push_back("PlayerCritSkillMax5");
	m_fields.push_back("PlayerCritSkillMin5");
	m_fields.push_back("PlayerSkillTag5");
	m_fields.push_back("PlayerSkillDPS5");
	m_fields.push_back("PlayerSkillCount6");
	m_fields.push_back("PlayerSkillCD6");
	m_fields.push_back("PlayerSkillHitCount6");
	m_fields.push_back("PlayerSkillMax6");
	m_fields.push_back("PlayerSkillMin6");
	m_fields.push_back("PlayerCritSkillMax6");
	m_fields.push_back("PlayerCritSkillMin6");
	m_fields.push_back("PlayerSkillTag6");
	m_fields.push_back("PlayerSkillDPS6");
	m_fields.push_back("PlayerSkillCount7");
	m_fields.push_back("PlayerSkillCD7");
	m_fields.push_back("PlayerSkillHitCount7");
	m_fields.push_back("PlayerSkillMax7");
	m_fields.push_back("PlayerSkillMin7");
	m_fields.push_back("PlayerCritSkillMax7");
	m_fields.push_back("PlayerCritSkillMin7");
	m_fields.push_back("PlayerSkillTag7");
	m_fields.push_back("PlayerSkillDPS7");
	m_fields.push_back("PlayerSkillCount8");
	m_fields.push_back("PlayerSkillCD8");
	m_fields.push_back("PlayerSkillHitCount8");
	m_fields.push_back("PlayerSkillMax8");
	m_fields.push_back("PlayerSkillMin8");
	m_fields.push_back("PlayerCritSkillMax8");
	m_fields.push_back("PlayerCritSkillMin8");
	m_fields.push_back("PlayerSkillTag8");
	m_fields.push_back("PlayerSkillDPS8");
	m_fields.push_back("PlayerSkillCount9");
	m_fields.push_back("PlayerSkillCD9");
	m_fields.push_back("PlayerSkillHitCount9");
	m_fields.push_back("PlayerSkillMax9");
	m_fields.push_back("PlayerSkillMin9");
	m_fields.push_back("PlayerCritSkillMax9");
	m_fields.push_back("PlayerCritSkillMin9");
	m_fields.push_back("PlayerSkillTag9");
	m_fields.push_back("PlayerSkillDPS9");

	m_fields.push_back("PlayerSkillCount10");
	m_fields.push_back("PlayerSkillCD10");
	m_fields.push_back("PlayerSkill10TimeMax");
	m_fields.push_back("PlayerSkill10TimeTotal");

	m_fields.push_back("PlayerQTECount");
	m_fields.push_back("PlayerQTEHitCount");
	m_fields.push_back("PlayerQTEMax");
	m_fields.push_back("PlayerQTEMin");
	m_fields.push_back("PlayerCritQTEMax");
	m_fields.push_back("PlayerCritQTEMin");
	m_fields.push_back("PlayerQTETag");
	m_fields.push_back("PlayerQTEDps");

	m_fields.push_back("PlayerInitHP");
	m_fields.push_back("PlayerHealHPCount");
	m_fields.push_back("PlayerHealHPMax");
	m_fields.push_back("PlayerHealHPMin");
	m_fields.push_back("PlayerHealHPTotal");
	m_fields.push_back("PlayerDamageHPCount");
	m_fields.push_back("PlayerDamageHPMax");
	m_fields.push_back("PlayerDamageHPMin");
	m_fields.push_back("PlayerDamageHPTotal");
	m_fields.push_back("PlayerEndHP");
	m_fields.push_back("PlayerDeadCount");
	m_fields.push_back("PlayerReliveCount");

	m_fields.push_back("PlayerInitMP");
	m_fields.push_back("PlayerHealMPCount");
	m_fields.push_back("PlayerHealMPMax");
	m_fields.push_back("PlayerHealMPMin");
	m_fields.push_back("PlayerHealMPTotal");
	m_fields.push_back("PlayerDamageMPCount");
	m_fields.push_back("PlayerDamageMPMax");
	m_fields.push_back("PlayerDamageMPMin");
	m_fields.push_back("PlayerDamageMPTotal");
	m_fields.push_back("PlayerEndMP");

	m_fields.push_back("MonsterCount");
	m_fields.push_back("MonsterEndCount");
	m_fields.push_back("MonsterCount1");
	m_fields.push_back("MonsterCount2");
	m_fields.push_back("MonsterCount3");
	m_fields.push_back("BossCount");
	m_fields.push_back("BossKillCount");

	m_fields.push_back("BossInitHPMax");
	m_fields.push_back("BossInitHPMin");
	m_fields.push_back("BossDamageMax");
	m_fields.push_back("BossDamageMin");
	m_fields.push_back("BossDamageTotal");
	m_fields.push_back("BossInitHPTotal");

	m_fields.push_back("MonsterInitHPMax");
	m_fields.push_back("MonsterInitHPMin");
	m_fields.push_back("MonsterDamageMax");
	m_fields.push_back("MonsterDamageMin");
	m_fields.push_back("MonsterDamageTotal");
	m_fields.push_back("MonsterInitHPTotal");

	m_fields.push_back("BossAttackCount");
	m_fields.push_back("BossUseSkillCount");
	m_fields.push_back("BossMissCount");
	m_fields.push_back("BossTimeTotal");
	m_fields.push_back("BossMoveTotal");
	m_fields.push_back("BossAttackMax");
	m_fields.push_back("BossAttackMin");
	m_fields.push_back("BossAttackTotal");
	m_fields.push_back("BossCallCount");
	m_fields.push_back("BossCallTotal");

	m_fields.push_back("MonsterAttackCount");
	m_fields.push_back("MonsterSkillCount");
	m_fields.push_back("MonsterMissCount");
	m_fields.push_back("MonsterTimeTotal");
	m_fields.push_back("MonsterMoveTotal");
	m_fields.push_back("MonsterAttackMax");
	m_fields.push_back("MonsterAttackMin");
	m_fields.push_back("MonsterAttackTotal");

	// handler
	for (auto i = m_fields.begin(); i != m_fields.end(); ++i)
	{
		if (*i == "dtEventTime")
		{
			m_field2handler[*i] = 2;
		}
		else
		{
			m_field2handler[*i] = 1;
		}
	}

	// special default
	m_specaildefault["ClientEndTime"] = "0-0-0 0:0:0";

	m_specaildefault["PlayerSkillCD1"] = "9999999";
	m_specaildefault["PlayerSkillCD2"] = "9999999";
	m_specaildefault["PlayerSkillCD3"] = "9999999";
	m_specaildefault["PlayerSkillCD4"] = "9999999";
	m_specaildefault["PlayerSkillCD5"] = "9999999";
	m_specaildefault["PlayerSkillCD6"] = "9999999";
	m_specaildefault["PlayerSkillCD7"] = "9999999";
	m_specaildefault["PlayerSkillCD8"] = "9999999";
	m_specaildefault["PlayerSkillCD9"] = "9999999";
	m_specaildefault["PlayerSkillCD10"] = "9999999";

	return true;
}

bool TSecRoundEndFlow::FillBegin(Scene* scene)
{
	FillCommon(scene);

	// init
	m_data["RoundRankPoint"] = "0";
	m_data["RoundRankup"] = "0";		
	m_data["RoundRank1"] = "0";			
	m_data["RoundRank2"] = "0";		

	// end
	m_data["RoundExp"] = "0";			
	m_data["Roundmoney"] = "0";		
	m_data["RoundDB"] = "0";			
	m_data["RoundHB"] = "0";			
	m_data["RoundGB"] = "0";		
	m_data["RoundGT"] = "0";	

	m_data["DropItemType1"] = "0";	
	m_data["DropItemUuid1"] = "0";
	m_data["DropItemType2"] = "0";
	m_data["DropItemTypeCount"]	 = "0";	


	return true;
}

bool TSecRoundEndFlow::FillEnd(Scene* scene)
{
	m_data["ClientVersion"] = m_poRole->GetClientConf().clientversion();
	m_data["UserIP"] = m_poRole->GetClientConf().ip();

	bool ischeat = (NULL == scene->Statistics()) ? false : scene->Statistics()->IsCheat();
	m_data["Result"] = ischeat ? "1" : "0";
	m_data["RoundEndType"] = ToString(m_endtype);

	StageRankInfo* rankinfo = scene->GetStageRankInfo(m_poRole->GetID());
	if (NULL != rankinfo)
	{
		m_data["RoundRate"] = ToString(XCommon::GetOneCount(rankinfo->rank));
		const RoleTowerInfo* towerinfo = NULL;
		if (scene->GetSceneType() == KKSG::SCENE_TOWER)
		{
			towerinfo = m_poRole->Get<CTowerRecord>()->GetTowerInfo(DEFAULT_HARDLEVEL);
		}
		m_data["EndLessFloor"] = ToString(NULL == towerinfo ? 0 : towerinfo->reachTopFloor);
		m_data["RoundNScore"] = ToString(rankinfo->score);		
		m_data["RoundDScore"] = ToString(rankinfo->score);	
	}

	m_data["UserMoney1"] = ToString(m_poRole->Get<Bag>()->CountVirtualItem(GOLD));
	m_data["UserMoney2"] = ToString(m_poRole->Get<Bag>()->CountVirtualItem(DIAMOND));	
	m_data["UserMoney3"] = ToString(m_poRole->Get<Bag>()->CountVirtualItem(FATIGUE));		
	m_data["UserMoney4"] = ToString(m_poRole->Get<Bag>()->CountVirtualItem(DRAGON_COIN));		
	m_data["UserMoney5"] = ToString(m_poRole->Get<Bag>()->CountVirtualItem(ARENA_COIN));		
	m_data["UserMoney6"] = ToString(m_poRole->Get<Bag>()->CountItemInBag(Item_Goddess)); ///> Å®ÉñÑÛÀá	
	m_data["UserKEY1"] = ToString(m_poRole->Get<Bag>()->CountItemInBag(Item_Dragon));  ///> Áú±¾ÃÅÆ±

	if (scene->GetSceneType() == KKSG::SCENE_PK)
	{
		m_data["RoundRankPoint"] = ToString(m_poRole->Get<CPkRecord>()->GetPoint());
	}

	if (NULL != scene && NULL != scene->Statistics())
	{
		RoleBattleRecord* record = scene->Statistics()->GetRecord(m_poRole->GetID());
		if (NULL != record)
		{
			const std::map<UINT32, UINT32>& items = record->itemid2count;
			m_data["RoundExp"] = items.end() == items.find(EXP) ? "0" : ToString(items.find(EXP)->second);
			m_data["Roundmoney"] = items.end() == items.find(GOLD) ? "0" : ToString(items.find(GOLD)->second);
			m_data["RoundDB"] = items.end() == items.find(DRAGON_COIN) ? "0" : ToString(items.find(DRAGON_COIN)->second);
			m_data["RoundHB"] = items.end() == items.find(ARENA_COIN) ? "0" : ToString(items.find(ARENA_COIN)->second);			
			m_data["RoundGB"] = items.end() == items.find(GUILD_CONTRIBUTE) ? "0" : ToString(items.find(GUILD_CONTRIBUTE)->second);		
			m_data["RoundGT"] = items.end() == items.find(Item_Goddess) ? "0" : ToString(items.find(Item_Goddess)->second);
			
			std::string tempequip;
			std::string tempitem;
			std::string tempitemcount;
			for (auto i = items.begin(); i != items.end(); ++i)
			{
				EquipConf* conf = ItemConfig::Instance()->GetEquipConf(i->first);
				if (NULL == conf)
				{
					tempitem += (ToString(i->first) + ",");
					tempitemcount += (ToString(i->second) + ",");
				}
				else
				{
					tempequip += (ToString(i->first) + ",");
				}
			}
			m_data["DropItemType1"]	= "" == tempequip ? "0" : tempequip;
			m_data["DropItemUuid1"] = "0";			
			m_data["DropItemType2"]	= "" == tempitem ? "0" : tempitem;
			m_data["DropItemTypeCount"] = "" == tempitemcount ? "0" : tempitemcount;
		}
	}
	return true;
}

bool TSecRoundEndFlow::FillPk(const std::string& change, const std::string& rank)
{
	m_data["RoundRankup"] = change;		
	m_data["RoundRank1"] = rank;			
	return true;
}

bool TSecRoundEndFlow::FillTimeSpan(UINT32 time)
{
	m_data["RoundTimeUse"] = ToString(time*1000);		
	return true;
}

bool TSecRoundEndFlow::FillMonsterKill(const MonsterInfo& info)
{
	m_data["MonsterCount"] = ToString(info.MonsterCount);
	m_data["MonsterEndCount"] = ToString(info.MonsterEndCount);
	m_data["MonsterCount1"] = ToString(info.MonsterCount1);
	m_data["MonsterCount2"] = ToString(info.MonsterCount2);
	m_data["MonsterCount3"] = ToString(info.MonsterCount3);
	m_data["BossCount"] = ToString(info.BossCount);
	m_data["BossKillCount"] = ToString(info.BossKillCount);
	return true;
}

bool TSecRoundEndFlow::FillPlayerDeath(int death, int revive)
{
	m_data["PlayerDeadCount"] = ToString(death);
	m_data["PlayerReliveCount"] = ToString(revive);
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::map<std::string, int> TSecRoundEndCount::m_field2handler;
std::vector<std::string> TSecRoundEndCount::m_fields;
bool TSecRoundEndCount::Init()
{
	// fields
	m_fields.push_back("GameSvrId");
	m_fields.push_back("dtEventTime");
	m_fields.push_back("GameAppID");
	m_fields.push_back("OpenID");
	m_fields.push_back("PlatID");
	m_fields.push_back("AreaID");
	m_fields.push_back("ZoneID");
	m_fields.push_back("BattleID");

	m_fields.push_back("Skill1Use");
	m_fields.push_back("Skill1Count");
	m_fields.push_back("Skill1EffectMin");
	m_fields.push_back("Skill1EffectMax");
	m_fields.push_back("Skill1TimeMin");
	m_fields.push_back("Skill1TimeMax");
	m_fields.push_back("Skill1TimeTotal");

	m_fields.push_back("Skill2Use");
	m_fields.push_back("Skill2Count");
	m_fields.push_back("Skill2EffectMin");
	m_fields.push_back("Skill2EffectMax");
	m_fields.push_back("Skill2TimeMin");
	m_fields.push_back("Skill2TimeMax");
	m_fields.push_back("Skill2TimeTotal");

	m_fields.push_back("Skill3Use");
	m_fields.push_back("Skill3Count");
	m_fields.push_back("Skill3EffectMin");
	m_fields.push_back("Skill3EffectMax");
	m_fields.push_back("Skill3TimeMin");
	m_fields.push_back("Skill3TimeMax");
	m_fields.push_back("Skill3TimeTotal");

	m_fields.push_back("Skill4Use");
	m_fields.push_back("Skill4Count");
	m_fields.push_back("Skill4EffectMin");
	m_fields.push_back("Skill4EffectMax");
	m_fields.push_back("Skill4TimeMin");
	m_fields.push_back("Skill4TimeMax");
	m_fields.push_back("Skill4TimeTotal");

	m_fields.push_back("Skill5Use");
	m_fields.push_back("Skill5Count");
	m_fields.push_back("Skill5EffectMin");
	m_fields.push_back("Skill5EffectMax");
	m_fields.push_back("Skill5TimeMin");
	m_fields.push_back("Skill5TimeMax");
	m_fields.push_back("Skill5TimeTotal");

	m_fields.push_back("Skill6Use");
	m_fields.push_back("Skill6Count");
	m_fields.push_back("Skill6EffectMin");
	m_fields.push_back("Skill6EffectMax");
	m_fields.push_back("Skill6TimeMin");
	m_fields.push_back("Skill6TimeMax");
	m_fields.push_back("Skill6TimeTotal");

	m_fields.push_back("Skill7Use");
	m_fields.push_back("Skill7Count");
	m_fields.push_back("Skill7TimeMin");
	m_fields.push_back("Skill7TimeMax");
	m_fields.push_back("Skill7TimeTotal");

	m_fields.push_back("Skill8Use");
	m_fields.push_back("Skill8Count");
	m_fields.push_back("Skill8CountMax");
	m_fields.push_back("Skill8TimeMin");
	m_fields.push_back("Skill8TimeMax");
	m_fields.push_back("Skill8TimeTotal");

	// handler
	for (auto i = m_fields.begin(); i != m_fields.end(); ++i)
	{
		if (*i == "dtEventTime")
		{
			m_field2handler[*i] = 2;
		}
		else
		{
			m_field2handler[*i] = 1;
		}
	}
	return true;
}

bool TSecRoundEndCount::FillBegin(Scene* scene)
{
	FillCommon(scene);

	return true;
}