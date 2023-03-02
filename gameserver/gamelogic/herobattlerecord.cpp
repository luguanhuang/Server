#include "pch.h"
#include "herobattlerecord.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "herobattlemgr.h"
#include "globalconfig.h"
#include "bagtransition.h"
#include "config/itemdropconfig.h"
#include "unit/role.h"
#include "utility/UtilMath.h"
#include "ranklist/rpcg2m_updatemsranklist.h"
#include "utility/tlogger.h"
#include "gamelogic/military_rank_record.h"
#include "reportdatamgr.h"
#include "scene/sceneherobattle.h"
#include "scene/scenemorphinfo.h"
#include "payconfig.h"
#include "payv2Mgr.h"
#include "common/ptcg2c_gserrornotify.h"
#include "scene/scenemobabattle.h"

#define MAX_GAME_RECORD 10

CHeroBattleRecord::CHeroBattleRecord(Role* pRole):m_pRole(pRole)
{
	m_winThisWeek = 0;
	m_todaySpCount = 0;
	m_lastUpdateTime = 0;

	m_totalNum = 0;
	m_winNum = 0;
	m_loseNum = 0;

	m_bigRewardCount = 0;

	m_weekprize = 0;

	m_eloPoint = GetGlobalConfig().HeroBattleOriginELOPoint;

	m_continueWinNum = 0;
	m_maxKillNum = 0;
	
	m_isModify.Set();
}

CHeroBattleRecord::~CHeroBattleRecord()
{

}

bool CHeroBattleRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (!poRoleAllInfo->has_herobattle())
	{
		return true;
	}

	const KKSG::HeroBattleRecord& record = poRoleAllInfo->herobattle();
	for (int i = 0; i < record.havehero_size(); ++i)
	{
		m_hero.insert(record.havehero(i));
	}

	m_winThisWeek = record.winthisweek();
	m_todaySpCount = record.todayspcount();
	m_lastUpdateTime = record.lastupdatetime();

	m_totalNum = record.totalnum();
	m_winNum = record.winnum();
	m_loseNum = record.losenum();

	m_bigRewardCount = record.bigrewardcount();
	m_weekprize = record.weekprize();

	m_eloPoint = record.elopoint();

	if (UtilMath::Isdoublezero(m_eloPoint))
	{
		m_eloPoint = GetGlobalConfig().HeroBattleOriginELOPoint;
	}
	
	for (int i = 0; i < record.freeweekhero_size(); ++i)
	{
		if (HeroBattleMgr::Instance()->GetHeroConf(record.freeweekhero(i)) == NULL)
		{
			LogWarn("heroid:%u is not in OverWatch.txt", record.freeweekhero(i));
			continue;
		}
		m_freeWeekHero.push_back(record.freeweekhero(i));
	}

	for (int i = 0; i < record.gamerecord_size(); ++i)
	{
		m_gameRecord.push_back(record.gamerecord(i));
	}

	for (int i = 0; i < record.experiencehero_size() && i < record.experienceherotime_size(); ++i)
	{
		m_experienceHero[record.experiencehero(i)] = record.experienceherotime(i);
	}

	m_dayTime = record.daytime();
	m_daytimes = record.daytimes();

	m_continueWinNum = record.continuewinnum();
	m_maxKillNum = record.maxkillnum();

	return true;
}

void CHeroBattleRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	Update();

	if (!m_isModify.TestAndReset())
	{
		return;
	}
	KKSG::HeroBattleRecord* record = poRoleAllInfo->mutable_herobattle();
	record->Clear();

	for (auto i = m_hero.begin(); i != m_hero.end(); ++i)
	{
		record->add_havehero(*i);
	}

	record->set_winthisweek(m_winThisWeek);
	record->set_todayspcount(m_todaySpCount);
	record->set_lastupdatetime(m_lastUpdateTime);

	record->set_totalnum(m_totalNum);
	record->set_winnum(m_winNum);	
	record->set_losenum(m_loseNum);

	record->set_bigrewardcount(m_bigRewardCount);
	record->set_weekprize(m_weekprize);
	record->set_elopoint(m_eloPoint);	

	for (UINT32 i = 0; i < m_freeWeekHero.size(); ++i)
	{
		record->add_freeweekhero(m_freeWeekHero[i]);
	}

	for (auto i = m_gameRecord.begin(); i != m_gameRecord.end(); ++i)
	{
		KKSG::HeroBattleOneGame* game = record->add_gamerecord();
		game->CopyFrom(*i);
	}

	for (auto i = m_experienceHero.begin(); i != m_experienceHero.end(); ++i)
	{
		record->add_experiencehero(i->first);
		record->add_experienceherotime(i->second);
	}

	record->set_daytime(m_dayTime);
	record->set_daytimes(m_daytimes);

	record->set_continuewinnum(m_continueWinNum);
	record->set_maxkillnum(m_maxKillNum);

	roChanged.insert(record);
}

void CHeroBattleRecord::Update()
{
	time_t nowTime = GameTime::GetTime();
	if (!XCommon::IsDaySame(m_lastUpdateTime, nowTime))
	{
		if (!XCommon::IsWeekSame(m_lastUpdateTime, nowTime))
		{
			m_winThisWeek = 0;
			m_weekprize = 0;
			HeroBattleMgr::Instance()->GetNewRandomFreeHero(m_freeWeekHero, m_pRole);
		}
		if (!XCommon::IsMonthSame(m_lastUpdateTime, nowTime))
		{
			m_totalNum = m_winNum = m_loseNum = 0;
			m_eloPoint = GetGlobalConfig().HeroBattleOriginELOPoint;

		}
		m_lastUpdateTime = nowTime;
		m_todaySpCount = 0;
		m_bigRewardCount = 0;
		m_daytimes = 0;

		m_isModify.Set();
	}
	for (auto i = m_experienceHero.begin(); i != m_experienceHero.end(); )
	{
		if (i->second < nowTime)
		{
			m_experienceHero.erase(i++);
			m_isModify.Set();
			continue;
		}
		++i;
	}
}

UINT32 CHeroBattleRecord::GetRandomHero(const std::vector<UINT32>& freeHero, const std::map<UINT32, UINT32>& experienceHero, const std::set<UINT32>& exceptHero)
{
	Update();

	std::vector<UINT32> allhero;
	for (auto i = m_hero.begin(); i != m_hero.end(); ++i)
	{
		if (HeroBattleMgr::Instance()->GetHeroConf(*i) == NULL)
		{
			LogWarn("heroid:%u is not in OverWatch.txt", *i);
			continue;
		}
		if (exceptHero.find(*i) == exceptHero.end())
		{
			allhero.push_back(*i);
		}
	}

	for (auto i = freeHero.begin(); i != freeHero.end(); ++i)
	{
		if (HeroBattleMgr::Instance()->GetHeroConf(*i) == NULL)
		{
			LogWarn("heroid:%u is not in OverWatch.txt", *i);
			continue;
		}
		if (exceptHero.find(*i) == exceptHero.end())
		{
			allhero.push_back(*i);
		}
	}

	for (auto i = experienceHero.begin(); i != experienceHero.end(); ++i)
	{
		if (HeroBattleMgr::Instance()->GetHeroConf(i->first) == NULL)
		{
			LogWarn("heroid:%u is not in OverWatch.txt", i->first);
			continue;
		}
		if (exceptHero.find(i->first) == exceptHero.end())
		{
			allhero.push_back(i->first);
		}
	}

	if (allhero.empty())
	{
		LogError("allhero is empty");
		return 1;
	}

	UINT32 random = XRandom::randInt(0, allhero.size());
	return allhero[random];
}

bool CHeroBattleRecord::IsHaveHero(UINT32 heroid)
{
	Update();	

	return m_hero.find(heroid) != m_hero.end();
}

KKSG::ErrorCode CHeroBattleRecord::BuyHero(UINT32 heroid)
{
	Update();
	
	OverWatchConf* conf = HeroBattleMgr::Instance()->GetHeroConf(heroid);
	if (conf == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}
	if (m_hero.find(heroid) != m_hero.end())
	{
		return KKSG::ERR_UNKNOWN;
	}

	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_HeroBattle, ItemFlow_HeroBattle_BuyHero);
	if (!take.TakeItem(conf->Price[0], conf->Price[1]))
	{
		take.RollBack();
		return KKSG::ERR_HERO_LACKMONEY;
	}
	take.NotifyClient();

	m_hero.insert(heroid);
	m_experienceHero.erase(heroid);

	THeroBattleBuyFlow oLog(m_pRole);
	oLog.m_CostID = conf->Price[0];
	oLog.m_CostNum = conf->Price[1];
	oLog.m_HeroID = heroid;
	oLog.Do();

	LogInfo("role [%llu] buy hero [%u]", m_pRole->GetID(), heroid);

	m_isModify.Set();
	return KKSG::ERR_SUCCESS;
}

void CHeroBattleRecord::FillHeroBattleInfo(KKSG::GetHeroBattleInfoRes& res)
{
	Update();

	for (auto i = m_hero.begin(); i != m_hero.end(); ++i)
	{
		res.add_havehero(*i);
	}

	for (UINT32 i = 0; i < m_freeWeekHero.size(); ++i)
	{
		res.add_weekhero(m_freeWeekHero[i]);
	}

	UINT32 nowTime = (UINT32)GameTime::GetTime();
	for (auto i = m_experienceHero.begin(); i != m_experienceHero.end(); ++i)
	{
		res.add_experiencehero(i->first);
		res.add_experienceherolefttime(i->second - nowTime);
	}

	res.set_winthisweek(m_winThisWeek);
	res.set_totalnum(m_totalNum);
	res.set_winnum(m_winNum);
	res.set_losenum(m_loseNum);
	res.set_todaygetspcount(m_todaySpCount);
	res.set_weekprize(m_weekprize);
	res.set_cangetprize(CanGetNextWeekPrize());
}

void CHeroBattleRecord::FillHeroBattleGameRecord(KKSG::GetHeroBattleGameRecordRes& res)
{
	Update();

	for (auto i = m_gameRecord.begin(); i != m_gameRecord.end(); ++i)
	{
		KKSG::HeroBattleOneGame* game = res.add_games();
		game->CopyFrom(*i);
	}
}

UINT32 CHeroBattleRecord::GetTodaySpCount()
{
	Update();

	return m_todaySpCount;
}

void CHeroBattleRecord::AddTodaySpCount()
{
	Update();

	m_todaySpCount++;

	m_isModify.Set();
}

UINT32 CHeroBattleRecord::GetTodayBigRewardRate()
{
	Update();

	if (GetGlobalConfig().HeroBattleBigRewardRate.empty())
	{
		return 0;
	}
	UINT32 pos = std::min(m_bigRewardCount, (UINT32)GetGlobalConfig().HeroBattleBigRewardRate.size() - 1);

	return GetGlobalConfig().HeroBattleBigRewardRate[pos];
}

void CHeroBattleRecord::AddTodayBigRewardCount()
{
	Update();

	m_bigRewardCount++;

	m_isModify.Set();
}

UINT32 CHeroBattleRecord::GetWeekPrize()
{
	Update();

	return m_weekprize;
}

bool CHeroBattleRecord::CanGetNextWeekPrize()
{
	Update();

	HeroWeekRewardConf* conf = HeroBattleMgr::Instance()->GetHeroWeekRewardConf(m_weekprize + 1);
	if (conf == NULL)
	{
		return false;
	}
	return m_winThisWeek >= conf->winnum;
}

double CHeroBattleRecord::GetELOPoint()
{
	Update();

	return m_eloPoint;
}

void CHeroBattleRecord::SetELOPoint(double point)
{
	Update();

	m_eloPoint = point;

	UpdateMSRankList();

	m_isModify.Set();
}

void CHeroBattleRecord::UpdateMSRankList()
{
	Update();

	if (m_totalNum <= 0) ///> 没有打过英雄战场
	{
		return;
	}
	RpcG2M_UpdateMSRankList* rpc = RpcG2M_UpdateMSRankList::CreateRpc();
	rpc->m_oArg.set_ranktype(KKSG::HeroBattleRank);
	KKSG::RankData* data = rpc->m_oArg.mutable_info();
	data->set_roleid(m_pRole->GetID());
	data->set_time((UINT32)GameTime::GetTime());
	KKSG::HeroRankData* herodata = data->mutable_heroinfo();
	herodata->set_elopoint(m_eloPoint);
	herodata->set_totalnum(m_totalNum);
	herodata->set_winnum(m_winNum);
	herodata->set_continuewinnum(m_continueWinNum);
	herodata->set_maxkillnum(m_maxKillNum);

	m_pRole->SendToMS(*rpc);
}

void CHeroBattleRecord::AddGameRecord(const KKSG::HeroBattleOneGame& oneGame, UINT32 killNum)
{
	Update();

	UINT32 exploit = 0;
	++m_totalNum;
	++m_daytimes;
	m_maxKillNum = std::max(m_maxKillNum, killNum);

	if (oneGame.over() == KKSG::HeroBattleOver_Win)
	{
		++m_winNum;
		++m_winThisWeek;
		++m_continueWinNum;

		std::vector<UINT64> opponents;
		for (int i = 0; i < oneGame.team1_size(); ++i)
		{
			if (oneGame.team1(i).roleid() == m_pRole->GetID())
			{
				opponents.clear();
				break;
			}
			opponents.push_back(oneGame.team1(i).roleid());
		}
		if (opponents.empty())
		{
			for (int i = 0; i < oneGame.team2_size(); ++i)
			{
				opponents.push_back(oneGame.team2(i).roleid());
			}
		}

		bool isHave = false;
		UINT32 num = 0;
		for (auto i = m_gameRecord.rbegin(); i != m_gameRecord.rend(); ++i)
		{
			if (num >= GetGlobalConfig().MilitarySameOpponentRound)
			{
				break;
			}
			if ((*i).over() != KKSG::HeroBattleOver_Win)
			{
				continue;
			}
			std::set<UINT64> team1, team2;
			for (int j = 0; j < (*i).team1_size(); ++j)
			{
				team1.insert((*i).team1(j).roleid());
			}
			for (int j = 0; j < (*i).team2_size(); ++j)
			{
				team2.insert((*i).team2(j).roleid());
			}

			std::set<UINT64>* pTeam;
			if (team1.find(m_pRole->GetID()) == team1.end())
			{
				pTeam = &team1;
			}
			else
			{
				pTeam = &team2;
			}
			for (UINT32 j = 0; j < opponents.size(); ++j)
			{
				if (pTeam->find(opponents[j]) != pTeam->end())
				{
					isHave = true;
					break;
				}
			}
			if (isHave)
			{
				break;
			}
			++num;
		}
		if (!isHave)
		{
			exploit = m_pRole->Get<MilitaryRankRecord>()->AddBattleRecord(KKSG::SCENE_HEROBATTLE);
		}
		else
		{
			PtcG2C_GSErrorNotify oPtc;
			oPtc.m_Data.set_errorno(KKSG::ERR_MILITARY_TRIGGER_CHECK);
			oPtc.m_Data.set_istip(true);
			m_pRole->Send(oPtc);
		}
	}
	else if (oneGame.over() == KKSG::HeroBattleOver_Lose)
	{
		++m_loseNum;
		m_continueWinNum = 0;

		exploit = m_pRole->Get<MilitaryRankRecord>()->ReduceBattleRecord(KKSG::SCENE_HEROBATTLE);
	}

	m_gameRecord.push_back(oneGame);
	m_gameRecord.back().set_exploit(exploit);
	while (m_gameRecord.size() > MAX_GAME_RECORD)
	{
		m_gameRecord.erase(m_gameRecord.begin());
	}

	UpdateMSRankList();

	m_isModify.Set();
}

void CHeroBattleRecord::RunAway(UINT32 killNum)
{
	Update();

	++m_totalNum;
	++m_daytimes;
	m_maxKillNum = std::max(m_maxKillNum, killNum);
	++m_loseNum;
	m_continueWinNum = 0;

	m_pRole->Get<MilitaryRankRecord>()->ReduceBattleRecord(KKSG::SCENE_HEROBATTLE);

	UpdateMSRankList();

	m_isModify.Set();
}

KKSG::ErrorCode CHeroBattleRecord::GetWeekNextReward()
{
	Update();

	HeroWeekRewardConf* conf = HeroBattleMgr::Instance()->GetHeroWeekRewardConf(m_weekprize + 1);
	if (!CanGetNextWeekPrize() || conf == NULL)
	{
		return KKSG::ERR_HEROBATTLE_CANTGETPRIZE;
	}

	++m_weekprize;

	BagGiveItemTransition give(m_pRole);
	give.SetReason(ItemFlow_HeroBattle, ItemFlow_HeroBattle_WeekReward);

	for (UINT32 i = 0; i < conf->reward.size(); ++i)
	{
		UINT32 dropid = conf->reward[i];
		const ItemDesc* desc = ItemDropConfig::Instance()->GetRandomDrop(dropid, m_pRole->GetLevel());
		if (desc == NULL)
		{
			continue;
		}
		give.GiveItem(*desc);
	}

	give.NotifyClient();

	m_isModify.Set();
	LogInfo("role [%llu] get week reward [%u]", m_pRole->GetID(), m_weekprize);

	return KKSG::ERR_SUCCESS;
}

KKSG::ErrorCode CHeroBattleRecord::UseExperienceHeroTicket(UINT32 itemid)
{
	Update();

	HeroBattleExperienceConf* conf = HeroBattleMgr::Instance()->GetExperienceHeroConf(itemid);
	if (conf == NULL)
	{
		LogWarn("itemid [%u] is not in HeroBattleExperience.txt", itemid);
		return KKSG::ERR_UNKNOWN;
	}
	if (m_experienceHero.find(conf->HeroID) != m_experienceHero.end())
	{
		return KKSG::ERR_HERO_EXPERIENCE_HAVE;
	}

	BagTakeItemTransition take(m_pRole);
	take.SetReason(ItemFlow_HeroBattle, ItemFlow_HeroBattle_HeroTicket);
	if (!take.TakeItem(itemid, 1))
	{
		take.RollBack();
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}
	take.NotifyClient();

	if (m_hero.find(conf->HeroID) != m_hero.end())
	{
		BagGiveItemTransition give(m_pRole);
		give.SetReason(ItemFlow_HeroBattle, ItemFlow_HeroBattle_HeroTicket_GiveReward);
		for (auto i = conf->Reward.begin(); i != conf->Reward.end(); ++i)
		{
			give.GiveItem((*i)[0], (*i)[1]);
		}
		give.NotifyClient();
	}
	else
	{
		m_experienceHero[conf->HeroID] = (UINT32)GameTime::GetTime() + conf->LastTime * 3600;
		if (m_pRole->GetCurrScene() != NULL)
		{
			if (m_pRole->GetCurrScene()->GetHeroBattleHandler())
			{
				SceneHeroBattle* battle = m_pRole->GetCurrScene()->GetHeroBattleHandler();
				battle->AddExperienceHero(m_pRole->GetID(), conf->HeroID, m_experienceHero[conf->HeroID]);
			}
			else if (m_pRole->GetCurrScene()->GetMobaBattleHandler())
			{
				SceneMobaBattle* moba = m_pRole->GetCurrScene()->GetMobaBattleHandler();
				moba->AddExperienceHero(m_pRole->GetID(), conf->HeroID, m_experienceHero[conf->HeroID]);
			}
			else if (m_pRole->GetCurrScene()->GetSceneMorphInfo()->CanMorph())
			{
				m_pRole->GetCurrScene()->GetSceneMorphInfo()->AddExperienceHero(m_pRole->GetID(), conf->HeroID, m_experienceHero[conf->HeroID]);
			}	
		}
	}

	m_isModify.Set();
	return KKSG::ERR_SUCCESS;
}

void CHeroBattleRecord::PrintHeroBattleRoundTLog(const THeroBattleRoundVar& var, const std::string& battleTag)
{
	Update();

	THeroBattleRoundFlow oLog(m_pRole);
	oLog.SetTransTag(battleTag);

	oLog.m_Elo = var.m_elo;
	oLog.m_NewElo = var.m_newElo;
	oLog.m_Result = var.m_result;
	oLog.m_KillNum = var.m_killNum;
	oLog.m_DeadNum = var.m_deadNum;
	oLog.m_IsMvp = var.m_IsMvp;
	for (auto i = m_hero.begin(); i != m_hero.end(); ++i)
	{
		if (i == m_hero.begin())
		{
			oLog.m_strHaveHero = ToString(*i);
		}
		else
		{
			oLog.m_strHaveHero += "#";
			oLog.m_strHaveHero += ToString(*i);
		}
	}
	for (auto i = m_freeWeekHero.begin(); i != m_freeWeekHero.end(); ++i)
	{
		if (i == m_freeWeekHero.begin())
		{
			oLog.m_strFreeWeekHero = ToString(*i);
		}
		else
		{
			oLog.m_strFreeWeekHero += "#";
			oLog.m_strFreeWeekHero += ToString(*i);
		}
	}
	oLog.Do();

	ReportDataMgr::Instance()->HeroBattleResult(m_pRole, var.m_result, var.m_killNum, var.m_deadNum);
}

void CHeroBattleRecord::OnBuyPayMember(int ID)
{
	Update();

	if (ID != pay_member_1)
	{
		return;
	}
	PayMemberTable::RowData* pRow = CPayConfig::Instance()->GetPayMemberByID(ID);
	if (pRow == NULL)
	{
		return;
	}
	if (pRow->HeroBattleFree > 0
	&& m_freeWeekHero.size() < GetGlobalConfig().HeroBattleFreeHeroCount + pRow->HeroBattleFree)
	{
		UINT32 count = GetGlobalConfig().HeroBattleFreeHeroCount + pRow->HeroBattleFree - m_freeWeekHero.size();
		HeroBattleMgr::Instance()->AddSomeRandomFreeHero(m_freeWeekHero, count);

		m_isModify.Set();
	}
}