#include "pch.h"
#include <time.h>
#include "stagemgr.h"
#include "scene/sceneconfig.h"
#include "pb/project.pb.h"
#include "foreach.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "findback/findbackrecord.h"
#include "team.h"
#include "globalconfig.h"
#include "stageassist.h"
#include "buff/XBuffTemplateManager.h"
#include "designationMgr.h"
#include "achievev2Mgr.h"
#include "payv2Record.h"
#include "event/eventmgr.h"
#include "holidaystage.h"
#include "table/AbyssPartyListTable.h"
#include "table/expeditionconfigmgr.h"
#include "absparty/ptcg2c_abspartyntf.h"
#include "perioddesi.h"
#include "util/gametime.h"
#include "team/ptcg2m_tarjainfontf.h"
#include "tablemgr/backflowtablemgr.h"
#include "config.h"
#include "scene/scenemortuary.h"
#include "gamelogic/PersonalCareerQuest.h"
#include "define/tlogenum.h"
#include "gamelogic/bagtransition.h"
#include "mail/mailsender.h"
#include "mail/mailconfig.h"
#include "gamelogic/notice.h"
#include "scene/DragonExpConfig.h"
#include "stage/ptcg2c_updatestageinfontf.h"

StageMgr::StageMgr(Role* role)
{
	m_pRole = role;
	m_StageDay = 0;
	m_lastWeekUpTime = time_t(NULL);
	m_bossrushmax = 0;
	m_BRJoinCounttoday = 0;
	m_BRRefreshCountToday = 0;
	m_brupday = 0;
	m_nAbyssMailTime = 0;
	m_helperWinCount = 0;
	m_helperWeekWinCount = 0;
	m_nTarjaAward = 0;
	m_nTarjaTime = 0;
	m_nKidHelperCount = 0;
	//trophy_data_.clear();
	m_deSpecialFlag = false;
}

StageMgr::~StageMgr()
{

}

void StageMgr::FirstInit(UINT32 roleNum)
{
	m_StageDay = 0;
	m_IsModified.Set();

	if (GSConfig::Instance()->IsBackFlowServer() && roleNum == 1)
	{
		BackFlowServerConfig::RowData* pData = BackFlowTableMgr::Instance()->GetServerConfig(m_pRole->GetProfession(), GSConfig::Instance()->GetBackFlowLevel());
		if (pData == NULL)
		{
			return ;
		}

		std::vector<UINT32> stageProcess;
		stageProcess.push_back(pData->MainStageProcess);
		stageProcess.push_back(pData->AbyssStageProcess);
		stageProcess.push_back(pData->NestStageProcess);

		for (auto itS = stageProcess.begin(); itS != stageProcess.end(); ++itS)
		{
			std::vector<UINT32> stageIds;
			std::vector<UINT32> chapterIds;
			SceneConfig::Instance()->GetStageUntil(*itS, stageIds, chapterIds);
			// 关卡星级、关卡宝箱
			for (auto it = stageIds.begin(); it != stageIds.end(); ++it)
			{
				UINT32 stageId = *it;
				SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(stageId);
				if (pSceneInfo == NULL)
				{
					continue;
				}
				SetRank(stageId, RANK_SSS);
				/*
				if (pSceneInfo->m_pConf->SceneChest)
				{
				SetSceneChestOpened(stageId);
				}
				*/
			}
			/*
			// 章节宝箱
			for (auto it = chapterIds.begin(); it != chapterIds.end(); ++it)
			{
			UINT32 chapterId = *it;
			UINT32 chestNum = SceneConfig::Instance()->GetChapterChestNum(chapterId);
			UINT32 starNum = GetChapterStarNum(chapterId);
			for (UINT32 i = 0; i < chestNum; ++i)
			{
			ChapterChestInfo* pChestInfo = SceneConfig::Instance()->GetChapterChestInfo(chapterId, i);
			if (pChestInfo == NULL)
			{
			continue;
			}
			if (starNum >= pChestInfo->startNeed)
			{
			SetFetchedChapterChest(chapterId, i);	
			}
			}
			}
			*/
		}
	}
}

void StageMgr::FinishBackFlowStage(BackFlowServerConfig::RowData* pData)
{
	std::vector<UINT32> stageProcess;
	stageProcess.push_back(pData->MainStageProcess);
	stageProcess.push_back(pData->AbyssStageProcess);
	stageProcess.push_back(pData->NestStageProcess);

	bool has = false;
	for (auto itS = stageProcess.begin(); itS != stageProcess.end(); ++itS)
	{
		std::vector<UINT32> stageIds;
		std::vector<UINT32> chapterIds;
		SceneConfig::Instance()->GetStageUntil(*itS, stageIds, chapterIds);
		// 关卡星级、关卡宝箱
		for (auto it = stageIds.begin(); it != stageIds.end(); ++it)
		{
			UINT32 stageId = *it;
			if (IsPassed(stageId))
			{
				continue;
			}
			SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(stageId);
			if (pSceneInfo == NULL)
			{
				continue;
			}
			SetRank(stageId, RANK_SSS);
			has = true;
		}
	}

	if (has)
	{
		UpdateStageInfo2Client();
	}
}

void StageMgr::UpdateStageInfo2Client()
{
	PtcG2C_UpdateStageInfoNtf ntf;
	FillStageInfo(ntf.m_Data.mutable_stages());
	m_pRole->Send(ntf);
}

bool StageMgr::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if(poRoleAllInfo && poRoleAllInfo->has_stages())
	{
		const KKSG::StageInfo &stages = poRoleAllInfo->stages();

		m_StageDay = stages.day();
		m_lastWeekUpTime = stages.lastweekuptime();

		int n = stages.sceneid_size();
		for (int i = 0; i < n; ++i)
		{
			m_StagesRank[stages.sceneid(i)] =  stages.rank(i);
		}

		n = stages.countscenegroupid_size();
		for (int i = 0; i < n; ++i)
		{
			StageCountInfo info;
			info.count = stages.count(i);
			info.buycount = stages.buycount(i);
			m_StagesCount[stages.countscenegroupid(i)] = info;
		}

		n = stages.cdscenegroupid_size();
		for (int i = 0; i < n; ++i)
		{
			m_StagesCoolDown[stages.cdscenegroupid(i)] = stages.cooldown(i);
		}

		for (int j = 0; j < stages.chestopenedscene_size(); ++j)
		{
			m_ChestOpenedScene.push_back(stages.chestopenedscene(j));
		}

		n = stages.chapterchest_size();
		m_ChapterChest.resize(n);
		for (int i = 0; i < n; ++i)
		{
			m_ChapterChest[i] = stages.chapterchest(i);
		}


		m_helperWinCount = stages.helperwincount();
		m_helperWeekWinCount = stages.helperweekwincount();

		m_DEStageProgress.clear();
		for(int i = 0; i < stages.stageprogress_size(); ++i)
		{
			const KKSG::DEStageProgress& sp = stages.stageprogress(i);
			CDEStageProgress pro(sp.sceneid());
			pro.Load(sp);
			m_DEStageProgress.insert(std::make_pair(pro.GetSceneID(), pro));
		}

		m_stageassist.clear();
		for (int i = 0; i < stages.stageassist_size(); ++i)
		{
			const KKSG::StageAssistOne& one = stages.stageassist(i);
			m_stageassist[one.stageid()] = one.point();
		}

		m_mapAbyss.clear();
		const KKSG::AbsPartyInfo  *pAbs = &stages.absparty();
		for (int i = 0; i < pAbs->aby_size(); ++i)
		{
			const KKSG::AbsPartyBase &base = pAbs->aby(i);
			m_mapAbyss[base.type()] =  base.diff();
		}

		m_nAbyssMailTime = pAbs->abyssmailtime();

		m_BRJoinCounttoday = stages.brjoincounttoday();
		m_BRRefreshCountToday = stages.brrefreshcounttoday();
		const KKSG::BRRankState& br = stages.brrankstate();
		m_BRState.confid = br.confid();
		m_BRState.brbid1 = br.brbid1();
		m_BRState.brbid2 = br.brbid2();
		m_BRState.rank = br.rank();
		m_bossrushmax = stages.bossrushmax();
		m_brupday = stages.brupday();

		m_HolidayId = stages.holidayid();
		m_HolidayTimes = stages.holidaytimes();
		m_nKidHelperCount = stages.kidhelpercount();

		m_nTarjaTime = stages.tarjatime();
		m_nTarjaAward = stages.tarjaaward();

		if (stages.has_trophydata())
		{
			trophy_.total_score = stages.trophydata().total_score();
			trophy_.honour_rank = stages.trophydata().honour_rank();
			for (UINT32 index = 0; index < (UINT32)stages.trophydata().trophydata_size(); ++index)
			{
				const KKSG::StageTrophyData&  trophy_data =    stages.trophydata().trophydata(index);
				StageTrophyDataDetail data_detail;
				data_detail.scene_id = trophy_data.scene_id();
				data_detail.help_count = trophy_data.help_count();
				data_detail.hight_damage = trophy_data.hight_damage();
				data_detail.hight_treat = trophy_data.hight_treat();
				data_detail.no_deathpass_count = trophy_data.no_deathpass_count();
				data_detail.pass_count = trophy_data.pass_count();
				data_detail.quickly_pass_time = trophy_data.quickly_pass_time();

				for (UINT32 i = 0; i < (UINT32)trophy_data.get_trophy_detail_size(); ++i)
				{
					std::vector<TrophyTypeDetail> detail_list;
					detail_list.clear();
					for (UINT32 count = 0; count < (UINT32) trophy_data.get_trophy_detail(i).detail_size(); ++count )
					{
						TrophyTypeDetail detail;
						detail.trophy_order = trophy_data.get_trophy_detail(i).detail(count).tropy_order();
						detail.trophy_time = trophy_data.get_trophy_detail(i).detail(count).trophy_time();
						detail_list.push_back(detail);
					}
					data_detail.trophy_detail.insert(std::make_pair(trophy_data.get_trophy_detail(i).trophy_id(),detail_list));
				}

				trophy_.trophy_data.push_back(data_detail);
			}
		}
		m_mapDne.clear();
		for (size_t i = 0; i< stages.dnes_size(); i++)
		{
			const KKSG::DneRecord  &record =  stages.dnes(i);
			m_mapDne[record.dneid()] = record.count();
		}

		m_deSpecialFlag = stages.despecialflag();
	}

	CheckSpecial();

	Update();

	return true;
}

void StageMgr::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_IsModified.TestAndReset())
	{
		poRoleAllInfo->clear_stages();
		KKSG::StageInfo *pStages = poRoleAllInfo->mutable_stages();
		roChanged.insert(pStages);
		FillStageInfo(pStages);
	}
}

void StageMgr::FillStageInfo(KKSG::StageInfo* pStages)
{
	pStages->Clear();
	pStages->set_day(m_StageDay);
	pStages->set_lastweekuptime((UINT32)m_lastWeekUpTime);

	foreach(i in m_StagesRank)
	{
		pStages->add_sceneid(i->first);
		pStages->add_rank(i->second);
	}

	foreach(i in m_StagesCount)
	{
		pStages->add_countscenegroupid(i->first);
		pStages->add_count(i->second.count);
		pStages->add_buycount(i->second.buycount);
	}

	foreach(i in m_StagesCoolDown)
	{
		if (i->second != 0)
		{
			pStages->add_cdscenegroupid(i->first);
			pStages->add_cooldown((int)i->second);
		}
	}

	foreach(i in m_ChapterChest)
	{
		pStages->add_chapterchest(*i);
	}

	foreach(i in m_ChestOpenedScene)
	{
		pStages->add_chestopenedscene(*i);
	}


	pStages->set_helperwincount(m_helperWinCount);
	pStages->set_helperweekwincount(m_helperWeekWinCount);

	for(auto it = m_DEStageProgress.begin(); it != m_DEStageProgress.end(); ++it)
	{
		KKSG::DEStageProgress* sp = pStages->add_stageprogress();
		(it->second).Save(*sp);
	}

	// stage assist
	for (auto i = m_stageassist.begin(); i != m_stageassist.end(); ++i)
	{
		KKSG::StageAssistOne* one = pStages->add_stageassist();
		one->set_stageid(i->first);
		one->set_point(i->second);
	}

	// abs
	KKSG::AbsPartyInfo* pAbsParty =  pStages->mutable_absparty();
	for (auto iter = m_mapAbyss.begin(); iter!= m_mapAbyss.end(); iter++)
	{
		KKSG::AbsPartyBase *pBase = pAbsParty->add_aby();
		pBase->set_type(iter->first);
		pBase->set_diff(iter->second);
	}
	pAbsParty->set_abyssmailtime(m_nAbyssMailTime);

	pStages->set_brupday(m_brupday);
	pStages->set_bossrushmax(m_bossrushmax);
	pStages->set_brjoincounttoday(m_BRJoinCounttoday);
	pStages->set_brrefreshcounttoday(m_BRRefreshCountToday);
	pStages->set_holidayid(m_HolidayId);
	pStages->set_holidaytimes(m_HolidayTimes);
	pStages->set_kidhelpercount(m_nKidHelperCount);
	pStages->set_tarjatime(m_nTarjaTime);
	pStages->set_tarjaaward(m_nTarjaAward);

	KKSG::BRRankState* br = pStages->mutable_brrankstate();
	br->set_confid(m_BRState.confid);
	br->set_brbid1(m_BRState.brbid1);
	br->set_brbid2(m_BRState.brbid2);
	br->set_rank(m_BRState.rank);

	pStages->mutable_trophydata()->set_total_score(trophy_.total_score);
	pStages->mutable_trophydata()->set_honour_rank(trophy_.honour_rank);
	for (auto data_it = trophy_.trophy_data.begin(); data_it != trophy_.trophy_data.end(); ++data_it)
	{
		KKSG::StageTrophyData* trophy_data = pStages->mutable_trophydata()->add_trophydata();
		trophy_data->set_scene_id(data_it->scene_id);
		trophy_data->set_help_count(data_it->help_count);
		trophy_data->set_hight_damage(data_it->hight_damage);
		trophy_data->set_hight_treat(data_it->hight_treat);
		trophy_data->set_no_deathpass_count(data_it->no_deathpass_count);
		trophy_data->set_quickly_pass_time(data_it->quickly_pass_time);
		trophy_data->set_pass_count(data_it->pass_count);
		for (auto detail_it = data_it->trophy_detail.begin(); detail_it != data_it->trophy_detail.end(); ++detail_it)
		{
			KKSG::TrophyGetTypeDetail* type_detail =  trophy_data->add_get_trophy_detail();
			type_detail->set_trophy_id(detail_it->first);
			for (auto trophy_order_it =  detail_it->second.begin(); trophy_order_it !=  detail_it->second.end(); ++trophy_order_it)
			{
				KKSG::TrophyDetail* detail =   type_detail->add_detail();
				detail->set_tropy_order(trophy_order_it->trophy_order);
				detail->set_trophy_time(trophy_order_it->trophy_time);
			}
		}
	}

	for (auto iter = m_mapDne.begin(); iter!=m_mapDne.end(); iter++)
	{
		KKSG::DneRecord* record =  pStages->add_dnes();
		record->set_dneid(iter->first);
		record->set_count(iter->second);
	}

	pStages->set_despecialflag(m_deSpecialFlag);
}


void StageMgr::SetRank(UINT32 sceneID, int rank)
{
	m_StagesRank[sceneID] = rank;
	m_IsModified.Set();
}

void StageMgr::SetAbyss(UINT32 nType,UINT32 nDiff)
{
	if(nDiff > m_mapAbyss[nType])
	{
		LogDebug("setrole=%llu aby type=%u diff=%u",m_pRole->GetID(),nType,nDiff);
		m_mapAbyss[nType] = nDiff;
		m_IsModified.Set();
	}
}

void StageMgr::SetAbyssMailTime(UINT32 nTime)
{
	m_nAbyssMailTime = nTime;
	m_IsModified.Set();
}

UINT32 StageMgr::GetAbyssMailTime()
{
	return m_nAbyssMailTime;
}


void StageMgr::SendAbyssInfo()
{
	PtcG2C_AbsPartyNtf ntf;
	for (auto iter = m_mapAbyss.begin(); iter!= m_mapAbyss.end(); iter++)
	{
		KKSG::AbsPartyBase *pBase = ntf.m_Data.add_aby();
		pBase->set_type(iter->first);
		pBase->set_diff(iter->second);
	}
	m_pRole->Send(ntf);
}

void StageMgr::OnFinishAbyss(UINT32 nAbyssID)
{
	if (nAbyssID == 0)
		return;
	const AbyssPartyListTable::RowData* pRowData = 	ExpeditionConfigMgr::Instance()->GetAbssParty(nAbyssID);
	if (pRowData== NULL)
		return;
	SetAbyss(pRowData->AbyssPartyId,pRowData->Index+1);
	SendAbyssInfo();
}



int StageMgr::GetRank(UINT32 sceneID)
{
	auto i = m_StagesRank.find(sceneID);
	if (i == m_StagesRank.end())
	{
		return INVALID_STAGERANK;
	}
	else
	{
		return i->second;
	}
}

bool StageMgr::IsPassed(UINT32 sceneID)
{
	int rank = GetRank(sceneID);
	if(RANK_EMPTY == rank || INVALID_STAGERANK == rank)//未通关
	{
		return false;
	}
	return true;
}

UINT32 StageMgr::GetStageCount(UINT32 nGroupId)
{
	auto iter = m_StagesCount.find(nGroupId);
	if (iter != m_StagesCount.end())
	{
		return iter->second.count;
	}
	return 0;
}

bool StageMgr::CanPlayCount(UINT32 nScenID)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(nScenID);
	if (!pSceneInfo)
	{
		return false;	
	}
	return CanPlayCount(pSceneInfo);
}

bool StageMgr::CanPlayCount(SceneInfo *pSceneInfo)
{
	Update();
	int count = pSceneInfo->m_pConf->DayLimit;
	if (count <= 0)
	{
		return true;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);
	auto i = m_StagesCount.find(groupid);
	if (i != m_StagesCount.end())
	{
		UINT32 buycount = i->second.buycount;
		if (pSceneInfo->m_pConf->type == KKSG::SCENE_ABYSSS)
		{
			buycount = 0;
		}
		if (i->second.count >= (UINT32)count + buycount)
		{
			return false;
		}
	}
	return true;
}

UINT32 StageMgr::CanPlayCoolDown(SceneInfo* pSceneInfo)
{
	int cooldown = pSceneInfo->m_pConf->CoolDown;
	if (cooldown <= 0)
	{
		return 0;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);
	auto i = m_StagesCoolDown.find(groupid);
	if (i == m_StagesCoolDown.end())
	{
		return 0;
	}
	if (i->second > time(NULL))
	{
		return (UINT32)(i->second - time(NULL));
	}
	i->second = 0;
	m_IsModified.Set();
	return 0;
}

void StageMgr::DayZero()
{
	//m_StagesCount.clear();
	for (auto it = m_StagesCount.begin(); it != m_StagesCount.end(); )
	{
		if (!DragonExpConfig::Instance()->IsDESceneGroup(it->first))
		{
			m_StagesCount.erase(it++);
		}
		else
		{
			it++;
		}
	}
	m_nTarjaAward = 0;

	m_IsModified.Set();
}

void StageMgr::WeekZero()
{
	for (auto it = m_StagesCount.begin(); it != m_StagesCount.end(); )
	{
		if (DragonExpConfig::Instance()->IsDESceneGroup(it->first))
		{
			m_StagesCount.erase(it++);
		}
		else
		{
			it++;
		}
	}
	HandleHelperWeekWin(m_helperWeekWinCount);
	m_helperWeekWinCount = 0;

	m_IsModified.Set();
}

void StageMgr::CheckSpecial()
{
	if (!m_deSpecialFlag)//龙之远征一次性flag
	{
		m_deSpecialFlag = true;
		m_IsModified.Set();

		UINT32 groupID = DragonExpConfig::Instance()->GetSceneGroupID();
		m_StagesCount.erase(groupID);
	}
}

void StageMgr::Update()
{
	time_t now = GameTime::GetTime();//time(NULL);

	int today = XCommon::GetTodayUnique();
	if (m_StageDay != today)
	{
		m_StageDay = today;
		DayZero();
		m_IsModified.Set();
	}

	//为了处理 BOSSRUSH 跨天
	if(m_brupday != today)
	{
		if(m_pRole->GetCurrScene() && m_pRole->GetCurrScene()->GetSceneType() == KKSG::SCENE_BOSSRUSH)
		{
			//SSInfo << " bossrush next day delay roleid = " << m_pRole->GetID() << END;
		}
		else
		{
			SSInfo << " bossrush next day clear roleid = " << m_pRole->GetID() << END;
			m_brupday = today;

			m_BRJoinCounttoday = 0;
			m_BRRefreshCountToday = 0;
			m_BRState.DayZero();
		}
	}

	if(!XCommon::IsWeekSame(m_lastWeekUpTime, now))
	{
		m_lastWeekUpTime = now;
		WeekZero();
		m_IsModified.Set();
	}
}

bool StageMgr::ResetPlayCount(UINT32 groupid)
{
	Update();
	auto i = m_StagesCount.find(groupid);
	if (i == m_StagesCount.end())
	{
		return false;
	}

	i->second.count = 0;
	i->second.buycount += 1;

	m_IsModified.Set();

	return true;
}

UINT32 StageMgr::GetBuyCount(UINT32 groupid)
{
	Update();
	auto i = m_StagesCount.find(groupid);
	if (i == m_StagesCount.end())
	{
		return 0;
	}
	return i->second.buycount;
}

void StageMgr::AddScenePlayCount(UINT32 sceneID)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(sceneID);
	if (!pSceneInfo)
	{
		return ;	
	}
	AddScenePlayCount(pSceneInfo);
}

void StageMgr::AddScenePlayCount(SceneInfo *pSceneInfo)
{
	Update();
	if (pSceneInfo->m_pConf->DayLimit <= 0)
	{
		return;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);
	auto i = m_StagesCount.find(groupid);
	if (i != m_StagesCount.end())
	{
		i->second.count += 1;
	}
	else
	{
		StageCountInfo info;
		info.count = 1;
		info.buycount = 0;
		m_StagesCount[groupid] = info;
	}
	m_IsModified.Set();

	m_pRole->OnAddScenePlayCount((KKSG::SceneType)pSceneInfo->m_pConf->type, 1);
}

void StageMgr::AddSceneBuyCount(SceneInfo *pSceneInfo, UINT32 count)
{
	Update();
	if (pSceneInfo->m_pConf->DayLimit <= 0)
	{
		return;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);
	auto i = m_StagesCount.find(groupid);
	if (i != m_StagesCount.end())
	{
		i->second.buycount += count;
	}
	else
	{
		StageCountInfo info;
		info.count = 0;
		info.buycount = 1;
		m_StagesCount[groupid] = info;
	}
	m_IsModified.Set();
}


UINT32 StageMgr::GetCanPlayCount(UINT32 nSceneID)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(nSceneID);
	if (!pSceneInfo)
	{
		return 0;	
	}
	return GetCanPlayCount(pSceneInfo);
}

UINT32 StageMgr::GetCanPlayCount(SceneInfo *pSceneInfo)
{
	Update();
	int count = pSceneInfo->m_pConf->DayLimit;
	if (count <= 0)
	{
		return 0;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo); 
	auto i = m_StagesCount.find(groupid);	
	if (i != m_StagesCount.end())
	{
		UINT32 buycount = i->second.buycount;
		if (pSceneInfo->m_pConf->type == KKSG::SCENE_ABYSSS)
		{
			buycount = 0;
		}
		count = count + buycount - i->second.count;
	}
	return (count <= 0) ? 0 : (UINT32)count;
}

UINT32 StageMgr::GetPlayedCount(SceneInfo *pSceneInfo)
{
	if (NULL == pSceneInfo)
	{
		return 0;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo); 
	auto it = m_StagesCount.find(groupid);	
	if (it != m_StagesCount.end())
	{
		return it->second.count;
	}
	return 0;
}

UINT32 StageMgr::GetPlayedCount(UINT32 sceneID)
{
	SceneInfo *pSceneInfo = SceneConfig::Instance()->FindSceneInfo(sceneID);
	if (!pSceneInfo)
	{
		return 0;	
	}
	return GetPlayedCount(pSceneInfo);
}

UINT32 StageMgr::GetSceneBuyCount(SceneInfo *pSceneInfo)
{
	Update();
	if (pSceneInfo->m_pConf->DayLimit <= 0)
	{
		return 0;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);
	auto i = m_StagesCount.find(groupid);
	if (i == m_StagesCount.end())
	{
		return 0;
	}
	return i->second.buycount;
}

void StageMgr::AddSceneCoolDown(SceneInfo *pSceneInfo)
{
	int cooldown = pSceneInfo->m_pConf->CoolDown;
	if (cooldown <= 0)
	{
		return;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);	
	m_StagesCoolDown[groupid] = (int)time(NULL) + cooldown;

	m_IsModified.Set();
}

UINT32 StageMgr::GetSceneCoolDown(SceneInfo *pSceneInfo)
{
	int cooldown = pSceneInfo->m_pConf->CoolDown;
	if (cooldown <= 0)
	{
		return 0;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);
	auto i = m_StagesCoolDown.find(groupid);
	if (i == m_StagesCoolDown.end())
	{
		return 0;
	}
	if (i->second > time(NULL))
	{
		return (UINT32)(i->second - time(NULL));
	}
	i->second = 0;
	m_IsModified.Set();
	return 0;
}

void StageMgr::ClearSceneCoolDown(SceneInfo* pSceneInfo)
{
	int cooldown = pSceneInfo->m_pConf->CoolDown;
	if (cooldown <= 0)
	{
		return;
	}
	int groupid = SceneConfig::Instance()->GetSceneGroupID(pSceneInfo);	
	m_StagesCoolDown[groupid] = 0;

	m_IsModified.Set();
}

void StageMgr::ClearSceneCoolDown()
{
	m_StagesCoolDown.clear();
	m_IsModified.Set();
	return;
}

void StageMgr::ForeachSceneCount(SceneCountCallback callback, void *ptr, std::set<UINT32>& groupids)
{
	Update();
	foreach(i in m_StagesCount)
	{
		bool istrue = true;
		if (!groupids.empty())
		{
			auto j = groupids.find(i->first);
			if (j == groupids.end())
			{
				istrue = false;
			}
		}
		if (istrue)
		{
			callback(ptr, i->first, i->second.count, i->second.buycount);
		}
	}
}

void StageMgr::ForeachSceneCoolDown(SceneCoolDownCallback callback, void *ptr, std::set<UINT32>& groupids)
{
	time_t nowtime = time(NULL);
	foreach(i in m_StagesCoolDown)
	{
		bool istrue = true;
		if (!groupids.empty())
		{
			auto j = groupids.find(i->first);
			if (j == groupids.end())
			{
				istrue = false;
			}
		}
		if (istrue)
		{
			callback(ptr, i->first, nowtime >= i->second ? 0 : (UINT32)(i->second - nowtime));
		}		
	}
}

bool StageMgr::IsFetchedChapterChest(int chapter, int chestID)
{
	UINT32 n = m_ChapterChest.size();
	UINT32 mask = (((UINT32)chapter) << 8);
	for (UINT32 i = 0; i < n; ++i)
	{
		if (mask == (m_ChapterChest[i] & 0xFFFFFF00) )
		{
			return (m_ChapterChest[i] & (1 << chestID)) != 0;
		}
	}

	return false;
}

void StageMgr::SetFetchedChapterChest(int chapter, int chestID)
{
	UINT32 n = m_ChapterChest.size();
	UINT32 mask = (((UINT32)chapter) << 8);
	for (UINT32 i = 0; i < n; ++i)
	{
		if (mask == (m_ChapterChest[i] & 0xFFFFFF00) )
		{
			m_ChapterChest[i] |= (1 << chestID);
			m_IsModified.Set();
			return;
		}
	}

	m_ChapterChest.push_back(mask | (1 << chestID));
	m_IsModified.Set();
}

bool StageMgr::IsSceneChestOpened(UINT32 sceneID)
{
	return std::find(m_ChestOpenedScene.begin(), m_ChestOpenedScene.end(), sceneID) != m_ChestOpenedScene.end();
}

void StageMgr::SetSceneChestOpened(UINT32 sceneID)
{
	m_ChestOpenedScene.push_back(sceneID);
	m_IsModified.Set();
}

void StageMgr::AddBRJoinCount()
{
	Update();
	m_BRJoinCounttoday++;

	m_IsModified.Set();
}

int StageMgr::GetBRJoinCount()
{
	Update();

	return m_BRJoinCounttoday;
}

void StageMgr::AddBRRefreshCount()
{
	Update();

	if(m_pRole->Get<CPayV2Record>()->GetRemainBossRushCount() > 0)
	{
		m_pRole->Get<CPayV2Record>()->UpdateBossRushCount();
	}
	else
	{
		m_BRRefreshCountToday ++;
	}

	m_IsModified.Set();
}

int StageMgr::GetBRRefreshCount(bool self/* = false*/)
{
	Update();

	int count = m_BRRefreshCountToday;
	if(m_pRole->Get<CPayV2Record>()->GetBossRushCount() > 0 && !self)
	{
		count += m_pRole->Get<CPayV2Record>()->GetUsedBossRushCount();
	}
	return count;
}

void StageMgr::ResetBRRefreshCount()
{
	Update();

	m_BRRefreshCountToday = 0;
	m_pRole->Get<CPayV2Record>()->ClearBossRushCount();
	m_IsModified.Set();
}


void StageMgr::TestClear()
{
	/*
	m_StagesRank.clear();
	m_StagesCount.clear();
	m_StagesCoolDown.clear();
	*/
	m_StagesCount.clear();
	m_BRRefreshCountToday = 0;
	m_BRJoinCounttoday = 0;

	m_IsModified.Set();
}

void StageMgr::DebugResetStageRank(UINT32 sceneID)
{
	m_mapAbyss.clear();
	m_StagesRank.erase(sceneID);
	m_IsModified.Set();
}

void StageMgr::ClearBRRoleState()
{
	m_BRState.DayZero();
	m_IsModified.Set();
}

BRRoleState* StageMgr::GetBRRoleState()
{
	Update();
	return &m_BRState;
}

void StageMgr::AddHelperWinCount()
{
	Update();
	m_helperWinCount ++;
	m_helperWeekWinCount ++;
	{
		DesignationMgr::Instance()->AssistDesignation(m_pRole,m_helperWinCount);
		AchieveV2Mgr::Instance()->OnAssistFight(m_pRole,m_helperWinCount);
		PeriodDesiMgr::Instance()->OnTriger(m_pRole,186);
	}

	m_IsModified.Set();
}

void StageMgr::SetBossHpProgress(UINT32 sceneID, UINT32 bossid, int bosshppercent)
{
	SSDebug << __FUNCTION__ << " sceneid = " << sceneID << " bossid = " << bossid << " bosshppercent = " << bosshppercent << END;
	auto it = m_DEStageProgress.find(sceneID);
	if(it == m_DEStageProgress.end())
	{
		CDEStageProgress pro(sceneID);
		pro.SetBossHpPercent(bossid, bosshppercent);
		m_DEStageProgress.insert(std::make_pair(pro.GetSceneID(), pro));
	}
	else
	{
		(it->second).SetBossHpPercent(bossid, bosshppercent);
	}
	m_IsModified.Set();
}

int StageMgr::GetBossHpProgress(UINT32 sceneID, UINT32 bossid)
{
	Update();
	auto it = m_DEStageProgress.find(sceneID);
	if(it != m_DEStageProgress.end())
	{
		return (it->second).GetBossHpPercent(bossid);
	}

	return DEDefaultHpPercent;
}

int StageMgr::GetBossAVGHpProgress(UINT32 sceneID)
{
	Update();
	auto it = m_DEStageProgress.find(sceneID);
	if(it != m_DEStageProgress.end())
	{
		return (it->second).GetAVGBossHpPercent();
	}

	return DEDefaultHpPercent;
}

void StageMgr::ClearProgress(UINT32 sceneID /*= 0*/)
{
	if(sceneID)
	{
		m_DEStageProgress.erase(sceneID);
	}
	else
	{
		m_DEStageProgress.clear();
	}
	m_IsModified.Set();
}


bool StageMgr::AddStageAssist(UINT32 stageid, int point)
{
	const std::pair<std::unordered_map<UINT32, int>::iterator, bool>& ret = m_stageassist.insert(std::make_pair(stageid, point));
	if (ret.second)
	{
		m_IsModified.Set();
	}
	return ret.second;
}

int StageMgr::GetStageAssistPoint()
{
	int point = 0;
	for (auto i = m_stageassist.begin(); i != m_stageassist.end(); ++i)
	{
		point += i->second;
	}
	return point;
}

void StageMgr::OnEnterScene(Scene* scene)
{
	if (m_pRole->GetLevel() >=  GetGlobalConfig().BattleAssistLevel)
	{
		return;
	}
	BuffDesc buff = StageAssistMgr::Instance()->GetAssistBuff(m_pRole, scene->GetSceneType());
	if (0 != buff.buffID)
	{
		XAddBuffActionArgs args;
		args.singleBuff.buffID = buff.buffID;
		args.singleBuff.BuffLevel = buff.BuffLevel;
		args.Caster = m_pRole->GetID();
		args.bClientOnly = !scene->IsSyncScene();
		m_pRole->GetXObject()->DispatchAction(&args);

		m_pRole->AddReviveBuff(buff);
	}
}

std::string StageMgr::DumpStageAssistPoint()
{
	std::stringstream ss;
	int point = 0;
	for (auto i = m_stageassist.begin(); i != m_stageassist.end(); ++i)
	{
		ss<<i->first<<":"<<i->second<<", ";
		point += i->second;
	}
	ss<<"point:"<<point;
	return ss.str();

}

void StageMgr::HandleHelperWeekWin(int helperWinCount)
{
	if (helperWinCount==0)
	{
		return;
	}
	EventMgr::Instance()->AddEvent(m_pRole->GetID(), DESIGNATION_COM_TYPE_WEEK_ASSIST, helperWinCount);
}

int StageMgr::HolidayCheck(UINT32 sceneId)
{
	if (!CHolidayStage::Instance()->IsHolidaySecneId(sceneId))
	{
		return KKSG::ERR_SUCCESS;
	}

	UINT32 holidaySceneId = CHolidayStage::Instance()->GetHolidaySceneId();
	if (sceneId != holidaySceneId)
	{
		return KKSG::ERR_UNKNOWN;
	}

	UINT32 holidayId = CHolidayStage::Instance()->GetHolidayId(); 
	if (m_HolidayId != holidayId)
	{
		m_HolidayId = holidayId;
		m_HolidayTimes = 0;
		m_IsModified.Set();
	}

	if (m_HolidayTimes >= CHolidayStage::Instance()->GetLimitTimes())
	{
		return KKSG::ERR_TEAM_TOWER_DAYCOUNT;
	}
	return KKSG::ERR_SUCCESS;
}

void StageMgr::DoAfterHolidayEnd(UINT32 sceneId)
{
	UINT32 holidaySceneId = CHolidayStage::Instance()->GetHolidaySceneId();
	if (sceneId == holidaySceneId)
	{
		m_HolidayTimes++;
		m_IsModified.Set();
	}
}

UINT32 StageMgr::GetHolidayTimes(UINT32 holidayId)
{
	if (m_HolidayId != holidayId)
	{
		m_HolidayId = holidayId;
		m_HolidayTimes = 0;
		m_IsModified.Set();
	}
	return m_HolidayTimes;
}


UINT32 StageMgr::GetAbyssDiff(UINT32 nType)
{
	auto iter = m_mapAbyss.find(nType);
	if (iter!=m_mapAbyss.end())
	{
		return iter->second;
	}
	return 0;
}

void StageMgr::AddTarjaTime()
{
	UINT32 nTime =	(UINT32)GameTime::GetTime();
	if(nTime>m_nTarjaTime)
	{
		m_nTarjaTime = nTime + GetGlobalConfig().TarjaTime;
	}else
	{
		m_nTarjaTime += GetGlobalConfig().TarjaTime;
	}
	m_IsModified.Set();
	SyncTarjaMs();
}


void StageMgr::SyncTarjaMs()
{
	if (GetGlobalConfig().TarjaSwitch==0)
		return;
	UINT32 nTime = (UINT32)GameTime::GetTime();
	if (m_nTarjaTime>nTime)
	{
		PtcG2M_TarjaInfoNtf ntf;
		ntf.m_Data.set_role(m_pRole->GetID());
		ntf.m_Data.set_time(m_nTarjaTime);
		m_pRole->SendToMS(ntf);
	}
}

UINT32 StageMgr::GetChapterStarNum(UINT32 chapterId)
{
	std::vector<UINT32> *pSceneIDS = SceneConfig::Instance()->GetChapterScene(chapterId);
	if (pSceneIDS == NULL)
	{
		return 0;
	}

	int star = 0;
	for (auto i = pSceneIDS->begin(); i != pSceneIDS->end(); ++i)
	{
		int t = GetRank(*i);
		switch (t)
		{
		case RANK_S:
			star += 1;
			break;
		case RANK_SS_1:
		case RANK_SS_2:
			star += 2;
			break;
		case RANK_SSS:
			star += 3;
			break;
		default:
			break;
		}
	}

	return star;
}

void StageMgr::SetTrophy(Scene *scene)
{
	if (scene == NULL)
	{
		return;
	}

	UINT32 scene_id = scene->GetSceneTemplateID();
	auto it = GetGlobalConfig().TrophySceneList.find(scene_id);
	auto it_end = GetGlobalConfig().TrophySceneList.end();
	if (it == it_end)
	{
		return;
	}

	StageTrophyDataDetail   info;
	info.scene_id = scene_id;
	// 1.通关次数
	info.pass_count = 1 ;

	// 2.通关最快时间
	UINT32 dwCurrTime = TimeUtil::GetTime();
	UINT32 quickly_pass_time = ((dwCurrTime >= (UINT32)m_pRole->GetEnterSceneTime() && m_pRole->GetEnterSceneTime()) ? dwCurrTime - (UINT32)m_pRole->GetEnterSceneTime() : 0);
	info.quickly_pass_time = quickly_pass_time;

	// 3.最高伤害
	// 4.最高治疗
	CombatStatistics & statiis = m_pRole->Statistics();
	info.hight_damage = (UINT64)statiis.GetTotalDamage();
	info.hight_treat = (UINT64)statiis.GetTotalTreat();

	// 5.助战次数	
	if (scene->IsHelper(m_pRole->GetID()))
	{
		info.help_count = 1;
	}
	// 6.不死通关次数
	UINT32	death_count = scene->GetMortuary()?scene->GetMortuary()->GetDeathCount(m_pRole->GetID()):0;
	if (death_count == 0)
	{
		info.no_deathpass_count = 1;
	}

	bool exist = false;
	for (auto it = trophy_.trophy_data.begin(); it !=  trophy_.trophy_data.end(); ++it)
	{
		auto& data = (*it);
		if (data.scene_id == scene_id)
		{
			data.pass_count += info.pass_count; 
			data.help_count += info.help_count;
			if (data.hight_damage < info.hight_damage)
			{
				data.hight_damage = info.hight_damage;
			}
			if (data.hight_treat < info.hight_treat)
			{
				data.hight_treat = info.hight_treat;
			}
			data.no_deathpass_count += info.no_deathpass_count;

			if ( data.quickly_pass_time > info.quickly_pass_time)
			{
				data.quickly_pass_time = info.quickly_pass_time;
			}

			exist = true;
			break;
		}
	}

	if (!exist)
	{
		trophy_.trophy_data.push_back(info);
	}

	UINT32 old_trophy_rank = trophy_.honour_rank;
	std::vector<ItemDesc> items;
	PersonalCareerQuest::Instance()->GetTrophyRank(scene_id,trophy_,items);

	if (trophy_.honour_rank != old_trophy_rank)
	{
		Notice notice(TrophyRankNotice);
		notice.Replace("$N",ToString(trophy_.honour_rank));
		notice.SendSelf(m_pRole);
	}

	if (items.size() > 0)
	{		
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_TrophyRankAward);
		std::string mailtitle;
		std::string mailcontent;
		if (conf)
		{
			mailtitle = conf->m_title;
			mailcontent = MailConfig::Instance()->ReplaceString("%d",ToString(trophy_.honour_rank) , conf->m_content);
		}
		MailSender sender;
		sender.SetReason(ItemFlow_Personal_Honour_Award);
		sender.SetContent(Mail_System, mailtitle,  mailcontent);
		sender.SetItems(items);
		sender.Send(m_pRole->GetID());
	}

	m_IsModified.Set();
}


void StageMgr::GetTrophyData(KKSG::PersonalCareerRes &roRes)
{
	auto data = roRes.mutable_trophy_data();
	data->set_honour_rank(trophy_.honour_rank);
	data->set_total_score(trophy_.total_score);
	for (auto it = trophy_.trophy_data.begin(); it != trophy_.trophy_data.end(); ++it)
	{
		KKSG::StageTrophyData* trophy_data = data->add_trophydata();
		trophy_data->set_scene_id(it->scene_id);
		trophy_data->set_help_count(it->help_count);
		trophy_data->set_hight_damage(it->hight_damage);
		trophy_data->set_hight_treat(it->hight_treat);
		trophy_data->set_no_deathpass_count(it->no_deathpass_count);
		trophy_data->set_quickly_pass_time(it->quickly_pass_time);
		trophy_data->set_pass_count(it->pass_count);
		for (auto detail_it = it->trophy_detail.begin(); detail_it != it->trophy_detail.end(); ++detail_it)
		{
			KKSG::TrophyGetTypeDetail* type_detail = trophy_data->add_get_trophy_detail();
			type_detail->set_trophy_id(detail_it->first);
			for (auto exist_it = detail_it->second.begin();exist_it != detail_it->second.end(); ++exist_it)
			{
				KKSG::TrophyDetail* detail =   type_detail->add_detail();
				detail->set_tropy_order(exist_it->trophy_order);
				detail->set_trophy_time(exist_it->trophy_time);
			}
		}
	}
}

void StageMgr::OnAddDne(UINT32 nDneID)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}

	m_mapDne[nDneID]++;
	m_IsModified.Set();
	DesignationMgr::Instance()->OnEqualBiger(m_pRole, DESIGNATION_COM_TYPE_DNE,nDneID,m_mapDne[nDneID],GameTime::GetTime(),true);
}

UINT32 StageMgr::GetDneCount(UINT32 nDneID)
{
    auto itor = m_mapDne.find(nDneID);
    if(m_mapDne.end() == itor)
    {
        return 0;
    }
    return itor->second;
}

void StageMgr::GetTrophyOrderBySceneID(const UINT32 scene_id,UINT32& trophy_id,UINT32& trophy_order)
{
	trophy_id = 0;
	trophy_order = 0;

	for (auto it = trophy_.trophy_data.begin(); it != trophy_.trophy_data.end(); ++it)
	{
		if (it->scene_id == scene_id)
		{			
			for (auto tropy_it = it->trophy_detail.begin(); tropy_it != it->trophy_detail.end(); ++tropy_it)
			{				
				for (auto trophy_order_it = tropy_it->second.begin(); trophy_order_it != tropy_it->second.end(); ++trophy_order_it)
				{
					if (trophy_order < trophy_order_it->trophy_order)
					{
						trophy_order = trophy_order_it->trophy_order;
						trophy_id = tropy_it->first;
					}
				}		
			}
		}
	}
	return ;
}

void StageMgr::DebugTest(UINT32 trophy_id,UINT32 trophy_order,UINT32 score)
{
	if (trophy_id == 0 && trophy_order == 0 &&score == 0)
	{
		trophy_.honour_rank = 1;
		trophy_.total_score = 0;
		trophy_.trophy_data.clear();
		m_IsModified.Set();

		return;
	}

	for (auto it = trophy_.trophy_data.begin(); it != trophy_.trophy_data.end(); ++it)
	{
		for (auto exit_it = it->trophy_detail.begin(); exit_it != it->trophy_detail.end(); ++exit_it)
		{
			if (exit_it->first == trophy_id )
			{
				for ( auto order_it = exit_it->second.begin(); order_it != exit_it->second.end(); ++order_it )
				{
					if (order_it->trophy_order == trophy_order)
					{
						return;
					}
				}				
			}
		}
	}

	StageTrophyDataDetail   info;
	TrophyTypeDetail detail;
	if (score > 0 )
	{
		trophy_.total_score = score;
	}
	else
	{
		detail.trophy_order = trophy_order;
		detail.trophy_time = (UINT32)GameTime::GetTime();
		std::vector<TrophyTypeDetail> detail_list;
		detail_list.push_back(detail);
		info.trophy_detail.insert(std::make_pair(trophy_id,detail_list));
	}
	trophy_.trophy_data.push_back(info);

	UINT32 old_trophy_rank = trophy_.honour_rank;
	std::vector<ItemDesc> items;
	PersonalCareerQuest::Instance()->GetTrophyRank(0,trophy_,items);

	if (trophy_.honour_rank != old_trophy_rank)
	{
		Notice notice(TrophyRankNotice);
		notice.Replace("$N",ToString(trophy_.honour_rank));
		notice.SendSelf(m_pRole->GetID());
	}

	if (items.size() > 0)
	{		
		const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_TrophyRankAward);
		std::string mailtitle;
		std::string mailcontent;
		if (conf)
		{
			mailtitle = conf->m_title;
			mailcontent = MailConfig::Instance()->ReplaceString("%d",ToString(trophy_.honour_rank) , conf->m_content);
		}
		MailSender sender;
		sender.SetReason(ItemFlow_Personal_Honour_Award);
		sender.SetContent(Mail_System, mailtitle,  mailcontent);
		sender.SetItems(items);
		sender.Send(m_pRole->GetID());
	}


	m_IsModified.Set();

}

void StageMgr::InfoPrint()
{
	Update();
	for (auto it = m_StagesCount.begin(); it != m_StagesCount.end(); ++it)
	{
		SSInfo << " group = " << it->first << " cout = " << it->second.count << END;
	}
}