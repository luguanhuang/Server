#include "pch.h"
#include "MultActivityMgr.h"
#include "unit/unit.h"
#include "unit/role.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "util/XCommon.h"
#include <time.h>
#include "pb/project.pb.h"
#include "table/OpenSystemMgr.h"
#include "activity/ptcg2c_mulactivitystatechange.h"
#include "unit/rolemanager.h"
#include "noticemgr.h"
#include "QAMgr.h"
#include "QARecord.h"
#include "horsemgr.h"
#include "stagemgr.h"
#include "util/gametime.h"
#include "guild/guildcampparty.h"
#include "weekend4v4record.h"
#include "halliconmgr.h"
#include "gamelogic/battlefieldgsmgr.h"

INSTANCE_SINGLETON(MultActivityMgr)

MultActivityMgr::MultActivityMgr()
{
}

MultActivityMgr::~MultActivityMgr()
{

}

bool MultActivityMgr::Init()
{
	if (!MultActivityMgrBase::Init())
	{
		return false;
	}
	// 开放等级
	for (auto it = m_activitys.begin(); it != m_activitys.end(); ++it)
	{
		MultiActivityList::RowData* config = m_tableList.GetByID(it->second.id);
		if (!config)
		{
			continue;
		}
		it->second.playerLevel = OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(config->SystemID);
	}
	return true;
}

void MultActivityMgr::Uninit()
{
	MultActivityMgrBase::Uninit();
}

void MultActivityMgr::OnActivityStateChange(UINT32 id, KKSG::MulActivityTimeState lastState, KKSG::MulActivityTimeState curState)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}

	if (id == MULT_ACTIVITY_GUILD_QA)
	{
		// 公会答题推送可以开启
		if (lastState == KKSG::MULACTIVITY_BEfOREOPEN && curState == KKSG::MULACTIVITY_RUNNING)
		{
			std::unordered_map<UINT64, CGuildSimple>& guildSimpleMap = GuildSimpleMgr::Instance()->GetAllGuild();
			for (auto iter = guildSimpleMap.begin(); iter != guildSimpleMap.end(); ++iter)
			{
				NoticeMgr::Instance()->GuildQALeftTimeNotice(iter->second.GetGuildId(), 1);
			}
		}
	}
	else if (id == MULT_ACTIVITY_MULTI_QA)
	{
		///>多人答题弹icon
		if (lastState == KKSG::MULACTIVITY_BEfOREOPEN && curState == KKSG::MULACTIVITY_RUNNING)
		{
			for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
			{
				Role* pRole = *i;
				if(pRole) QAMgr::Instance()->TriggerQA(pRole, QATYPE_MULTI);
			}
		}
	}else if (id == MULT_ACTIVITY_HORSE)
	{
		if (lastState == KKSG::MULACTIVITY_BEfOREOPEN && curState == KKSG::MULACTIVITY_RUNNING)
		{
			LogDebug("MULT_ACTIVITY_HORSE");
			HorseRideMgr::Instance()->Open();
		}
    }else if (id == MULT_ACTIVITY_WEEKEND4V4)
    {
        if (lastState != KKSG::MULACTIVITY_RUNNING && curState == KKSG::MULACTIVITY_RUNNING)
        {
            LogDebug("MULT_ACTIVITY_WEEKEND4V4 start");
            WeekEnd4v4Mgr::Instance()->HallIconAllNtf(KKSG::HICONS_BEGIN);
        }else if (lastState == KKSG::MULACTIVITY_RUNNING && curState != KKSG::MULACTIVITY_RUNNING)
        {
            LogDebug("MULT_ACTIVITY_WEEKEND4V4 end");
            WeekEnd4v4Mgr::Instance()->HallIconAllNtf(KKSG::HICONS_END);
        }
    } 
	else if (id == MULT_ACTIVITY_GUILDCAMP_PARTY)
	{
		// 公会营地派对开启
		if (lastState == KKSG::MULACTIVITY_BEfOREOPEN && curState == KKSG::MULACTIVITY_RUNNING)
		{
			CGuildCampPartyMgr::Instance()->OpenActivities();
		}
	}else if (id == MULT_ACTIVITY_BATTLEFIELD)
	{
		if (curState == KKSG::MULACTIVITY_RUNNING)
		{
			BattleFieldGSMgr::Instance()->Start();
		}
	}
}


void MultActivityMgr::OnActivitysChange(UINT32 opencount, std::vector<UINT32>& changeIds)
{
	if (GSConfig::Instance()->IsCrossGS())
	{
		return;
	}


	auto itend = RoleManager::Instance()->End();
	for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
	{
		Role* pRole = (*it);	
		if(pRole && pRole->GetCurrScene() && !pRole->IsInBattle())
		{
			PtcG2C_MulActivityStateChange masc;
			masc.m_Data.set_opencount(opencount);
			for(size_t i = 0; i < changeIds.size(); ++i)
				FillOneMulActivityInfo(NULL, changeIds[i], masc.m_Data.add_changeinfo(), NULL);
			pRole->Send(masc);
		}
	}
}

bool MultActivityMgr::CanJoinMulitActivity(Role* pRole, int id)
{
	if(NULL == pRole)
	{
		return false;
	}

	MultActivityConf* conf = GetActivityConfig(id);
	if(NULL == conf)
	{
		SSError << " can not find multi activity id = " << id << END;
		return false;
	}

	if(pRole->GetLevel() < (int)conf->playerLevel)
	{
		return false;
	}
	if(conf->guildLevel > 0)
	{
		CGuildSimple* guild = GuildSimpleMgr::Instance()->GetGuildByRoleId(pRole->GetID());
		if(NULL == guild || guild->GetGuildLvl() < (int)conf->guildLevel)
		{
			return false;
		}
	}

	KKSG::MulActivityTimeState state = GetOpenTimeState(id);
	if(KKSG::MULACTIVITY_RUNNING != state)
	{
		return false;
	}

	int count = GetJoinCount(pRole, id);
	if(count >= conf->maxCount)
	{
		return false;
	}

	return true;
}

int MultActivityMgr::GetJoinCount(Role* pRole, int id)
{
	if(NULL == pRole)
	{
		return 0;	
	}

	QARecord* record = pRole->Get<QARecord>();
	if( MULT_ACTIVITY_GUILD_QA == id )
	{
		return record->GetQACount(QATYPE_GUILD);
	}
	if( MULT_ACTIVITY_MULTI_QA == id )
	{
		return record->GetQACount(QATYPE_MULTI);
	}else if(MULT_ACTIVITY_HORSE == id)
	{
		StageMgr* pStage = pRole->Get<StageMgr>();
		return  pStage->GetStageCount(GetGlobalConfig().HorseSceneID);
    }
	return 0; 
}

void MultActivityMgr::FillAllMulActivityInfo(Role* pRole, int amid, KKSG::MulActivityRes &roRes, const KKSG::GetMulActOpenInfoRes* res)
{
	if (amid == 0)
	{
		for (auto it = m_activitys.begin(); it != m_activitys.end(); ++it)
		{
			FillOneMulActivityInfo(pRole, it->first, roRes.add_actinfo(), res);
		}
	}
	else
	{
		FillOneMulActivityInfo(pRole, amid, roRes.add_actinfo(), res);
	}
} 


void MultActivityMgr::GmTestCalcWeek()
{
	auto  iter = m_activitys.find(MULT_ACTIVITY_MULTI_QA);
	if (iter!=m_activitys.end())
	{
		MultActivityConf &conf = iter->second;
		std::vector<int>  vecOpenDays;
		if (conf.openDays[0])
		{
			for (int i = 0; i < 7; i++)
			{
				vecOpenDays.push_back(i+1);
			}
		}else
		{
			for (size_t i = 0; i < conf.openDays.size(); i++)
			{
				vecOpenDays.push_back(conf.openDays[i]);
			}
		}

		UINT32 nWeek  = GameTime::GetWeekUnique(GameTime::GetTime());
		UINT32 nDay   = GameTime::GetWeekDay();
		UINT32 nTotal = 0;
		for (size_t j = 0; j < vecOpenDays.size(); j++)
		{
			if (nDay<=vecOpenDays[j])
			{
				nTotal++;
			}
		}
		nTotal += nWeek*vecOpenDays.size();
		UINT32 nIdx = nTotal%7;
		LogDebug("idx=%u", nIdx);
	}
}

void MultActivityMgr::FillOneMulActivityInfo(Role* pRole, int confid, KKSG::MulActivitInfo* info, const KKSG::GetMulActOpenInfoRes* res)
{
	if(NULL == info)
	{
		return ;
	}
	info->set_id(confid);
	info->set_dayjoincount(GetJoinCount(pRole, confid));
	UINT32 leftTime = 0;
	MultActivityTime openTime;
	KKSG::MulActivityTimeState state = GetOpenTimeState(confid, leftTime, openTime);
	info->set_openstate(state);
	info->set_lefttime(leftTime);
	info->set_beginmin(openTime.startSec / 60);
	info->set_endmin(openTime.endSec / 60);

	// 二次开启
	if (state != KKSG::MULACTIVITY_RUNNING)
	{
		info->set_real_open_state(KKSG::ActOpenState_NotOpen);
		return;
	}
	info->set_real_open_state(KKSG::ActOpenState_Running);

	if (pRole == NULL)
	{
		return;
	}

	UINT32 now = GameTime::GetTime();
	if (confid == MULT_ACTIVITY_GUILD_QA)
	{
		QARecord* qa = pRole->Get<QARecord>();
		UINT32 lastOpenTime = QAMgr::Instance()->GetGuildLastOpenTime(pRole->getGuildId());
		UINT32 startTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILD_QA);
		UINT32 prepareTime = startTime + QAMgr::Instance()->GetPrepareTime(QATYPE_GUILD);
		UINT32 endTime = prepareTime + 10 * 30; // TODO hard code!!!
		if (lastOpenTime < startTime)
		{
			info->set_real_open_state(KKSG::ActOpenState_NotOpen);
		}
		else if (now < prepareTime)
		{
			info->set_lefttime(prepareTime - now);
			info->set_real_open_state(KKSG::ActOpenState_Prepare);
		}
		else if (now < endTime)
		{
			info->set_lefttime(endTime - now);
			info->set_real_open_state(KKSG::ActOpenState_Running);
		}
		else 
		{
			info->set_real_open_state(KKSG::ActOpenState_Over);
		}
		info->set_is_playing(qa->IsPlayingQA());
	}
	else if (confid == MULT_ACTIVITY_MULTI_QA)
	{
		QARecord* qa = pRole->Get<QARecord>();
		UINT32 startTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_MULTI_QA);
		UINT32 prepareTime = startTime + QAMgr::Instance()->GetPrepareTime(QATYPE_MULTI);
		UINT32 endTime = prepareTime + 10 * 30; // TODO hard code!!!
		if (now < prepareTime)
		{
			info->set_lefttime(prepareTime - now);
			info->set_real_open_state(KKSG::ActOpenState_Prepare);
		}
		else if (now < endTime)
		{
			info->set_lefttime(endTime - now);
			info->set_real_open_state(KKSG::ActOpenState_Running);
		}
		else 
		{
			info->set_real_open_state(KKSG::ActOpenState_Over);
		}
		info->set_is_playing(qa->IsPlayingQA());
	}
	// 红包
	else if (confid == MULT_ACTIVITY_GUILD_BONUS)
	{
		if (res == NULL)
		{
			info->set_real_open_state(KKSG::ActOpenState_NotOpen);
			return;
		}
		UINT32 lastOpenTime = res->last_bonus_time();
		UINT32 startTime = MultActivityMgr::Instance()->GetOpenTime(MULT_ACTIVITY_GUILD_BONUS);
		if (lastOpenTime < startTime)
		{
			info->set_real_open_state(KKSG::ActOpenState_NotOpen);
		}
		else if (res->is_all_check_bonus_taked())
		{
			info->set_openstate(KKSG::MULACTIVITY_END);
			info->set_real_open_state(KKSG::ActOpenState_Over);
		}
	}
	else if (confid == MULT_ACTIVITY_CARDMATCH)
	{
		if (NULL != res )
		{
			if (KKSG::MULACTIVITY_END == res->guildcard_openstate())
			{
				info->set_openstate(KKSG::MULACTIVITY_END);
			}
		}
		if (res == NULL || res->guildcardopen() == false)
		{
			info->set_real_open_state(KKSG::ActOpenState_NotOpen);
		}
	}
}

