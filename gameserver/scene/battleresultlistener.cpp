#include "pch.h"
#include "battleresultlistener.h"
#include "unit/role.h"
#include "unit/combatstatistics.h"
#include "scene.h"
#include "unit/dummyrole.h"
#include "network/mslink.h"
#include "gamelogic/military_rank_record.h"
#include "idutil.h"
#include "servermgr/servermgr.h"

void NormalBattleResultListener::NotifyClient(Role *pRole)
{
	pRole->Send(BattlePtc);
	pRole->SetFlag(RSF_ISGiveReward, true);
	pRole->BattlePtc.m_Data = BattlePtc.m_Data;
}

KKSG::PkResult* NormalBattleResultListener::GetPkResult()
{
	return pRoleResult->mutable_pkresult();
}

KKSG::InvFightBattleResult* NormalBattleResultListener::GetInvFightResult()
{
	return pRoleResult->mutable_invfightresult();
}

KKSG::CustomBattleResult* NormalBattleResultListener::GetCustomBattleResult()
{
	return pRoleResult->mutable_custombattle();
}


KKSG::BigMeleeBattleResult * NormalBattleResultListener::GetBigMeleeBattleResult()
{
	return pRoleResult->mutable_bigmelee();
}


KKSG::BattleFieldBattleResult *  NormalBattleResultListener::GetBattleFieldResult()
{
	return pRoleResult->mutable_battlefield();
}



void NormalBattleResultListener::SetBossRush(UINT32 currentmax, UINT32 lastmax)
{
	BattlePtc.m_Data.mutable_specialstage()->mutable_bossrushresult()->set_currentmax(currentmax);
	BattlePtc.m_Data.mutable_specialstage()->mutable_bossrushresult()->set_lastmax(lastmax);
}

void NormalBattleResultListener::SetArenaup(UINT32 arenaup, bool ismissed)
{
	BattlePtc.m_Data.mutable_specialstage()->set_arenaup(arenaup);
	BattlePtc.m_Data.mutable_specialstage()->set_arenamissed(ismissed);
	pRoleResult->set_stars(RANK_SSS);
}

void NormalBattleResultListener::SetGoblinResult(int rank, bool bGetGuildBonus)
{
	KKSG::GuildGoblinResult* result = pRoleResult->mutable_guildgoblinresult();
	result->set_getguildbonus(bGetGuildBonus);
	result->set_currank(rank);
}

void NormalBattleResultListener::SetTowerResult(int towerFloor, bool isNewRecord)
{
	KKSG::TowerResult* result = pRoleResult->mutable_towerresult();
	result->set_towerfloor(towerFloor);
	result->set_isnewrecord(isNewRecord);
}

void NormalBattleResultListener::SetStatisticsInfo(const CombatStatistics &stat)
{
	pRoleResult->set_damage(stat.GetTotalDamage());
	pRoleResult->set_maxcombo(stat.GetMaxCombo());
	pRoleResult->set_deathcount(stat.GetDeathCount());
	pRoleResult->set_killcount(stat.GetKillCount());
	pRoleResult->set_killcontinuemax(stat.GetKillContinueCountMax());
	pRoleResult->set_treat(stat.GetTotalTreat());
	pRoleResult->set_assitnum(stat.GetAssistNum());
	pRoleResult->set_behitdamage(stat.GetBeHitDamage());
	pRoleResult->set_multikillcountmax(stat.GetMultiKillCountMax());

	KKSG::BattleStatisticsData* data = watchptc.m_Data.add_data();
	data->set_roleid(pRoleResult->roleid());
	data->set_name(pRoleResult->rolename());
	data->set_type(pRoleResult->isleader());
	data->set_profession(pRoleResult->profession());
	data->set_damageall(stat.GetTotalDamage());
	data->set_deadcount(stat.GetDeathCount());
	data->set_killcount(stat.GetKillCount());
	data->set_killcontinue(stat.GetKillContinueCountMax());
	data->set_treatcount(0);
	data->set_combomax(stat.GetMaxCombo());
	data->set_assitnum(stat.GetAssistNum());
}

void NormalBattleResultListener::AddItem(int itemID, int itemCount, bool isbind, int AddReson)
{
	if (AddReson == FirstStar)
	{
		KKSG::ItemBrief *pItemBrief = pRoleResult->add_starreward();
		pItemBrief->set_itemid(itemID);
		pItemBrief->set_itemcount(itemCount);
		pItemBrief->set_isbind(isbind);
	}
	else if (AddReson == NormalStageReward)
	{
		if (itemID == GOLD)
		{
			pRoleResult->set_money(itemCount + pRoleResult->money());
		}
		else if (itemID == EXP)
		{
			pRoleResult->set_exp(itemCount + pRoleResult->exp());
		}
		else
		{
			KKSG::ItemBrief *pItemBrief = pRoleResult->add_items();
			pItemBrief->set_itemid(itemID);
			pItemBrief->set_itemcount(itemCount);
			pItemBrief->set_isbind(isbind);
		}
	}
	else if(GuildReward == AddReson)
	{
		KKSG::ItemBrief* pItemBrief = pRoleResult->add_guildreward();
		pItemBrief->set_itemid(itemID);
		pItemBrief->set_itemcount(itemCount);
		pItemBrief->set_isbind(isbind);
	}
	else if (GuildDragonReward == AddReson)
	{
		SetGuildDargon(itemCount);
	}
	else if (TeamCostReward == AddReson)
	{
		KKSG::ItemBrief* item = pRoleResult->mutable_teamcostreward();
		item->set_itemid(itemID);
		item->set_itemcount(itemCount);
		item->set_isbind(isbind);
	}
}

void NormalBattleResultListener::SetSceneRank(int Rank, UINT32 firststar)
{
	pRoleResult->set_stars(Rank);
	pRoleResult->set_firststars(firststar);

	KKSG::BattleStarData* data = watchptc.m_Data.add_star();
	data->set_roleid(pRoleResult->roleid());
	data->set_star(Rank);
}

void NormalBattleResultListener::SetInitStatus(UINT64 nRoleID, UINT64 gid, bool IsLeader,const std::string &szName,UINT32 nLv,UINT32 nExp,UINT32 nPro,UINT32 nServerID)
{
	pRoleResult = BattlePtc.m_Data.add_rolereward();

	pRoleResult->set_money(0);
	pRoleResult->set_exp(0);
	pRoleResult->set_stars(0);
	pRoleResult->set_firststars(0);
	pRoleResult->set_damage(0);
	pRoleResult->set_rolename(szName);
	pRoleResult->set_startlevel(nLv);
	pRoleResult->set_startexp(nExp);
	pRoleResult->set_roleid(nRoleID);
	pRoleResult->set_gid(gid);
	pRoleResult->set_profession(nPro);
	pRoleResult->set_isleader(IsLeader);
	pRoleResult->set_ishelper(false);
	pRoleResult->set_serverid(nServerID);
}


void NormalBattleResultListener::SetInitStatus(Role *pRole, UINT64 gid, bool IsLeader)
{
	pRoleResult = BattlePtc.m_Data.add_rolereward();

	pRoleResult->set_money(0);
	pRoleResult->set_exp(0);
	pRoleResult->set_stars(0);
	pRoleResult->set_firststars(0);
	pRoleResult->set_damage(0);
	pRoleResult->set_rolename(pRole->GetName());
	pRoleResult->set_startlevel(pRole->GetLevel());
	pRoleResult->set_startexp((UINT32)pRole->GetExp());
	pRoleResult->set_roleid(pRole->GetID());
	pRoleResult->set_gid(gid);
	pRoleResult->set_profession(pRole->GetProfession());
	pRoleResult->set_military_rank(pRole->Get<MilitaryRankRecord>()->GetMilitaryRank());
	pRoleResult->set_isleader(IsLeader);
	pRoleResult->set_ishelper(false);
	pRoleResult->set_serverid(pRole->GetServerId());
}

void NormalBattleResultListener::SetInitStatus(const KKSG::PvpRoleInfo& info)
{
	pRoleResult = BattlePtc.m_Data.add_rolereward();

	pRoleResult->set_money(0);
	pRoleResult->set_exp(0);
	pRoleResult->set_stars(0);
	pRoleResult->set_firststars(0);
	pRoleResult->set_damage(0);
	pRoleResult->set_rolename(info.name());
	pRoleResult->set_startlevel(info.level());
	pRoleResult->set_startexp(0);
	pRoleResult->set_roleid(info.roleid());
	pRoleResult->set_gid(0);
	pRoleResult->set_profession(info.profession());
	pRoleResult->set_military_rank(info.military_rank());
	pRoleResult->set_isleader(false);
	pRoleResult->set_ishelper(false);
	pRoleResult->set_serverid(GetServerIDByRoleID(info.roleid()));	
}

void NormalBattleResultListener::SetInitStatus(DummyRole *pRobot)
{
	pRoleResult = BattlePtc.m_Data.add_rolereward();

	pRoleResult->set_money(0);
	pRoleResult->set_exp(0);
	pRoleResult->set_stars(0);
	pRoleResult->set_firststars(0);
	pRoleResult->set_damage(0);
	pRoleResult->set_rolename(pRobot->GetName());
	pRoleResult->set_startlevel(pRobot->GetLevel());
	pRoleResult->set_startexp(pRobot->GetExp());
	pRoleResult->set_roleid(pRobot->GetID());
	pRoleResult->set_gid(0);
	pRoleResult->set_profession(pRobot->GetProfession());
	pRoleResult->set_isleader(false);
	pRoleResult->set_ishelper(false);
	pRoleResult->set_serverid(0);
}

void NormalBattleResultListener::SetDummyRoleEndState(DummyRole *pRobot)
{
	pRoleResult->set_endlevel(pRobot->GetLevel());
	pRoleResult->set_endexp(pRobot->GetExp());
}

void NormalBattleResultListener::SetRoleEndState(Role *pRole)
{
	pRoleResult->set_endlevel(pRole->GetLevel());
	pRoleResult->set_endexp((UINT32)pRole->GetExp());
}

NormalBattleResultListener::NormalBattleResultListener(Scene *pScene, bool win)
{
	BattlePtc.m_Data.mutable_stageinfo()->set_isstagefailed(!win);
	BattlePtc.m_Data.mutable_stageinfo()->set_stageid(pScene->GetSceneInfo()->m_pConf->id);
	
	BattlePtc.m_Data.mutable_stageinfo()->set_stagetype(pScene->GetSceneInfo()->m_pConf->type);
	BattlePtc.m_Data.mutable_stageinfo()->set_timespan(pScene->GetTimeSpan());
	BattlePtc.m_Data.mutable_stageinfo()->set_abyssid(pScene->GetAbyssID());
	pRoleResult = NULL;

	watchptc.m_Data.set_scenetype(pScene->GetSceneType());

	msptc.m_Data.set_scenename(pScene->GetSceneInfo()->m_pConf->Comment);
}

void NormalBattleResultListener::SetTimeSpan(UINT32 timespan)
{
	BattlePtc.m_Data.mutable_stageinfo()->set_timespan(timespan);
}

void NormalBattleResultListener::SetScore(int score, int vipscore)
{
	pRoleResult->set_score(score);
	pRoleResult->set_vipscore(vipscore);
}

void NormalBattleResultListener::SetVipLevel(int viplevel)
{
	pRoleResult->set_vipscore(viplevel);
}

void NormalBattleResultListener::SetEnd(bool bEnd)
{
	BattlePtc.m_Data.mutable_stageinfo()->set_end(bEnd);
}

void NormalBattleResultListener::ModifyExp(Role* role)
{
	pRoleResult->set_exp((UINT32)role->GetRealExp());
}

KKSG::PVPResult* NormalBattleResultListener::GetPvpResult()
{
	return pRoleResult->mutable_pvpresult();
}

KKSG::HorseAward* NormalBattleResultListener::GetHorseResult()
{
	return pRoleResult->mutable_horse();
}

KKSG::SkyCityFinalInfo* NormalBattleResultListener::GetSkyCityResult()
{
	return pRoleResult->mutable_skycity();
}


KKSG::HeroBattleResult* NormalBattleResultListener::GetHeroBattleResult()
{
	return pRoleResult->mutable_heroresult();
}

KKSG::MobaBattleResult* NormalBattleResultListener::GetMobaBattleResult()
{
	return BattlePtc.m_Data.mutable_stageinfo()->mutable_mobabattle();
}

KKSG::MobaBattleRoleResult* NormalBattleResultListener::GetMobaBattleRoleResult()
{
	return pRoleResult->mutable_mobabattle();
}

KKSG::WeekEnd4v4TeamResult* NormalBattleResultListener::GetWeekEnd4v4TeamResult()
{
    return BattlePtc.m_Data.mutable_stageinfo()->mutable_weekend4v4tmresult();
}

KKSG::WeekEnd4v4BattleRoleData* NormalBattleResultListener::GetWeekEnd4v4RoleResult()
{
    return pRoleResult->mutable_weekend4v4roledata();
}

void NormalBattleResultListener::SetIsFinalResult(bool final)
{
	BattlePtc.m_Data.set_isfinalresult(final);
}

void NormalBattleResultListener::SetBox(RoleBoxRewardInfo* boxinfo)
{
	if (NULL != boxinfo)
	{
		for (auto i = boxinfo->boxs.begin(); i != boxinfo->boxs.end(); ++i)
		{
			KKSG::BattleRewardChest* box = pRoleResult->add_box();
			box->set_chesttype(i->type);
			box->set_itemid(i->item.itemID);
			box->set_itemcount(i->item.itemCount);
			box->set_isbind(i->item.isbind);
		}
	}
}

void NormalBattleResultListener::SetWatchPvp(UINT64 winuid, UINT64 mvp)
{
	watchptc.m_Data.set_winuid(winuid);
	watchptc.m_Data.set_mvp(mvp);
}

void NormalBattleResultListener::SetWatchInfo(Scene* scene)
{
	BattlePtc.m_Data.mutable_watchinfo()->set_wathccount(scene->GetTotalWatchNum());
	BattlePtc.m_Data.mutable_watchinfo()->set_likecount(scene->GetTotalCommendNum());
}

void NormalBattleResultListener::SetGuildExp(UINT32 exp)
{
	pRoleResult->set_guildexp(exp);
}

void NormalBattleResultListener::SetResWar(UINT32 nRes)
{
	pRoleResult->set_reswar(nRes);
}

void NormalBattleResultListener::SetGuildCon(UINT32 con)
{
	pRoleResult->set_guildcon(con);
}

void NormalBattleResultListener::SetGuildDargon(UINT32 dargon)
{
	pRoleResult->set_guilddargon(dargon);
}

void NormalBattleResultListener::NotifyWatcher(Scene* scene)
{
	watchptc.m_Data.mutable_watchinfo()->set_wathccount(scene->GetTotalWatchNum());
	watchptc.m_Data.mutable_watchinfo()->set_likecount(scene->GetTotalCommendNum());
	scene->BroadcastWatcher(watchptc);
}

void NormalBattleResultListener::SetIsHelper(bool helper)
{
	if(pRoleResult)
	{
		pRoleResult->set_ishelper(helper);
	}
}

void NormalBattleResultListener::SetIsSelectBoxExcept(bool istrue)
{
	if(pRoleResult)
	{
		pRoleResult->set_isboxexcept(istrue);
	}
}

KKSG::DragonExpResult* NormalBattleResultListener::GetDEResult()
{
	return pRoleResult->mutable_deresult();
}

void NormalBattleResultListener::SetExpSeal(bool isexpseal)
{
	pRoleResult->set_isexpseal(isexpseal);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
KKSG::GuildExpAndConData* NormalBattleResultListener::GetGuildInfo()
{
	return &msptc.m_Data;
}

void NormalBattleResultListener::NotifyMS(std::vector<Role*>& roles)
{
	//¼æÈÝ¿ç·þGS

	if (GSConfig::Instance()->IsCrossGS())
	{
		std::unordered_set<UINT32> sids;
		for (size_t i = 0; i < roles.size(); ++i)
		{
			UINT32 serverID = ServerMgr::Instance()->GetFromServerIdByRoleId(roles[i]->GetID());
			if (sids.find(serverID) != sids.end())
			{
				continue;
			}
			sids.insert(serverID);
			ServerMgr::Instance()->SendByRoleId(roles[i]->GetID(), msptc);
		}
	}
	else
	{
		MSLink::Instance()->SendTo(msptc);
	}
}
