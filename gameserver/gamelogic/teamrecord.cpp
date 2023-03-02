#include "pch.h"
#include "teamrecord.h"
#include "unit/role.h"
#include "define/teamdef.h"
#include "payv2Record.h"
#include <ctime>
#include "util/XCommon.h"
#include "levelsealMgr.h"
#include "expeditionconfigmgr.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "mail/mailsender.h"
#include "util/gametime.h"
#include "define/tlogenum.h"
#include "globalconfig.h"
#include "stagemgr.h"
#include "stagerankmgr.h"
#include "common/ptcg2c_gserrornotify.h"
#include "gamelogic/dragonhelpconfig.h"
#include "designationMgr.h"
#include "servermgr/servercrossdatamgr.h"

void STeamCountInfo::DayZero()
{
	m_extraAddCount = 0;
	m_finishCountToday = 0;
	m_buyCountToday = 0;
}

void STeamCountInfo::WeekZero()
{
	m_helpCount = 0;
}

//int STeamCountInfo::GetGiveCountToday(int teamType)
//{
//	switch(teamType)
//	{
//	case EXP_TEAM_SCENE :
//		return  GetGlobalConfig().GetInt("ExpTeamDayCount", 0);
//	case GUILD_TEAM_SCENE :
//		return  GetGlobalConfig().GetInt("GuildTeamDayCount", 0);
//	case NEST_TEAM_SCENE : 
//		return  GetGlobalConfig().GetInt("NestTeamDayCount", 0);
//	case ABYSS_TEAM_SCENE:
//		return  GetGlobalConfig().GetInt("AbyssTeamDayCount", 0);
//	case TEAM_TOWER:
//		return  GetGlobalConfig().GetInt("TowerTeamDayCount", 0);
//	case GODDESS_TEAM_SCENE:
//		return  GetGlobalConfig().GetInt("GoddessTrialCount", 0);
//	case ENDLESSABYSS_SCENE:
//		return  GetGlobalConfig().GetInt("EndlessabyssCount", 0);
//	case GUILDCAMP_TEAM_SCENE:
//		return  GetGlobalConfig().GetInt("GuildCampDayCount", 0);
//	default:
//		{
//			SSError << "eroor sTeam type = " << teamType << END;
//		}
//		break;
//	}
//	return 0;
//}

CTeamRecord::CTeamRecord(Role* role)
{
	m_pRole = role;

	m_GoddessGetRewardToday = 0;
	m_weekNestRewardCount = 0;
	m_diamondCostCount = 0;
	m_useTicketCount = 0;
	m_lastDayUpTime = UINT32(time(NULL));
	m_lastWeekUpTime = UINT32(time(NULL));
	m_wantDragonHelp = false;
}

CTeamRecord::~CTeamRecord()
{

}

bool CTeamRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if(poRoleAllInfo && poRoleAllInfo->has_teamdbinfo())
	{
		const KKSG::TeamRecord& data = poRoleAllInfo->teamdbinfo();

		m_lastDayUpTime = data.lastdayuptime();
		m_lastWeekUpTime = data.lastweekuptime();

		m_TeamCountInfo.clear();
		for(int j = 0; j < data.teamcountins_size(); ++j)
		{
			const KKSG::TeamCountInfo& info = data.teamcountins(j);
			auto& sin = m_TeamCountInfo[info.teamtype()];
			sin.m_finishCountToday = info.finishcounttoday();
			sin.m_buyCountToday = info.buycounttoday();
			sin.m_extraAddCount = info.extraaddcount();
			sin.m_helpCount = info.helpcount();
		}	

		m_GoddessGetRewardToday = data.goddessgetrewardtoday();
		m_weekNestRewardCount = data.weeknestrewardcount();
		m_diamondCostCount = data.diamondcostcount();
		m_useTicketCount = data.useticketcount();

		m_teamcost = data.teamcost();

		for(int i = 0; i < data.dragonhelpfetchedrew_size(); ++i)
		{
			m_dragonHelpFetchedReward.insert(data.dragonhelpfetchedrew(i));
		}
		m_wantDragonHelp = data.wantdragonhelp();
	}

	Update();

	return true;
}

void CTeamRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if(m_IsModified.TestAndReset())
	{
		KKSG::TeamRecord& data = *poRoleAllInfo->mutable_teamdbinfo();
		roChanged.insert(&data);
		data.Clear();

		data.set_lastdayuptime(m_lastDayUpTime);
		data.set_lastweekuptime(m_lastWeekUpTime);

		for(auto it = m_TeamCountInfo.begin(); it != m_TeamCountInfo.end(); ++it)
		{
			KKSG::TeamCountInfo *info = data.add_teamcountins();
			info->set_teamtype(it->first);
			info->set_finishcounttoday((it->second).m_finishCountToday);
			info->set_buycounttoday((it->second).m_buyCountToday);
			info->set_extraaddcount((it->second).m_extraAddCount);
			info->set_helpcount((it->second).m_helpCount);
		}

		data.set_goddessgetrewardtoday(m_GoddessGetRewardToday);
		data.set_weeknestrewardcount(m_weekNestRewardCount);
		data.set_diamondcostcount(m_diamondCostCount);
		data.set_useticketcount(m_useTicketCount);

		*data.mutable_teamcost() = m_teamcost;

		for(auto it = m_dragonHelpFetchedReward.begin(); it != m_dragonHelpFetchedReward.end(); ++it)
		{
			data.add_dragonhelpfetchedrew(*it);
		}
		data.set_wantdragonhelp(m_wantDragonHelp);
	}
}

void CTeamRecord::Update()
{
	time_t now = GameTime::GetTime();//time(NULL);
	if(!XCommon::IsDaySame(m_lastDayUpTime, now))
	{
		DayZero();

		m_lastDayUpTime = UINT32(now);

		m_IsModified.Set();
	}
	if(!XCommon::IsWeekSame(m_lastWeekUpTime, now))
	{
		WeekZero();

		m_lastWeekUpTime = UINT32(now);

		m_IsModified.Set();
	}
}

int CTeamRecord::GetTeamFinishCountToday( int teamType )
{
	int finishCount = 0;
	finishCount += m_TeamCountInfo[teamType].m_finishCountToday;
	if(ABYSS_TEAM_SCENE == teamType && m_pRole->Get<CPayV2Record>()->GetAbyssCount() > 0)
	{
		finishCount += m_pRole->Get<CPayV2Record>()->GetUsedAbyssCount();
	}
	return finishCount;
}

int CTeamRecord::GetTeamBuyCountToday( int teamType )
{
	return m_TeamCountInfo[teamType].m_buyCountToday;
}

int CTeamRecord::GetTeamLeftCountToday( int teamType )
{
	int leftCount = 0;
	leftCount += GetTeamMaxCountToday(teamType);
	leftCount -= GetTeamFinishCountToday(teamType);
	if(leftCount < 0)
	{
		leftCount = 0;
	}
	return leftCount;
}

int CTeamRecord::GetTeamMaxCountToday( int teamType )
{
	int maxCount = 0;
	//maxCount += STeamCountInfo::GetGiveCountToday(teamType);
	maxCount += ExpeditionConfigMgr::Instance()->GetType2Count(teamType);
	maxCount += m_TeamCountInfo[teamType].m_extraAddCount;	
	if(ABYSS_TEAM_SCENE == teamType)
	{
		maxCount += m_pRole->Get<CPayV2Record>()->GetAbyssCount();
	}
	return maxCount;
}

void CTeamRecord::AddTeamFinishCountToday( int teamType )
{
	SSDebug << __FUNCTION__ << " " << teamType << END;
	if(ABYSS_TEAM_SCENE == teamType && m_pRole->Get<CPayV2Record>()->GetRemainAbyssCount() > 0)
	{
		m_pRole->Get<CPayV2Record>()->UpdateAbyssCount();
	}
	else
	{
		m_TeamCountInfo[teamType].m_finishCountToday++;
	}
	m_pRole->OnAddTeamCount(teamType, 1);

	m_IsModified.Set();
}

void CTeamRecord::AddTeamBuyCountToday( int teamType )
{
	m_TeamCountInfo[teamType].m_buyCountToday ++;
	m_IsModified.Set();
}

void CTeamRecord::AddTeamExtraAddCount( int teamType )
{
	m_TeamCountInfo[teamType].m_extraAddCount ++;
	m_IsModified.Set();
}

void CTeamRecord::AddGoddessRewardToday()
{
	m_GoddessGetRewardToday++;

	m_IsModified.Set();
}

int CTeamRecord::GetGoddessRewardToday()
{
	return m_GoddessGetRewardToday;
}

int CTeamRecord::GetLeftGoddessRewardToday()
{
	int goddessReward = GetGoddessRewardToday();
	int joinIn = GetTeamFinishCountToday(GODDESS_TEAM_SCENE);
	int canGet = joinIn /  GetGlobalConfig().GetInt("GoddessTrialNeedJoinTimes", 1);
	if(canGet < goddessReward)
	{
		SSError << "get GoddessReward bigger than he can get" << END;
		return 0;
	}
	return canGet - goddessReward;
}

void CTeamRecord::TestPrint()
{
	SSInfo << __FUNCTION__ << END;
	for(auto it = m_TeamCountInfo.begin(); it != m_TeamCountInfo.end(); ++it)
	{
		SSInfo << (it->first) << " " << (it->second).m_finishCountToday 
			<< " " << (it->second).m_extraAddCount << " " << (it->second).m_buyCountToday << END;
	}
	for(auto it = m_TeamCountInfo.begin(); it != m_TeamCountInfo.end(); ++it)
	{
		SSInfo << "help type = " << it->first << " count = " << (it->second).m_helpCount << END;
	}
	for(auto it = m_dragonHelpFetchedReward.begin(); it != m_dragonHelpFetchedReward.end(); ++it)
	{
		SSInfo << " dhr fetched = " << (*it) << END;
	}
}

void CTeamRecord::DayZero()
{
	for(auto it = m_TeamCountInfo.begin(); it != m_TeamCountInfo.end(); ++it)
	{
		if(it->first == TEAM_TYPE_COUPLE)
		{
			continue;
		}
		(it->second).DayZero();
	}

	m_GoddessGetRewardToday = 0;
	m_useTicketCount = 0;
}

void CTeamRecord::WeekZero()
{
	for(auto it = m_TeamCountInfo.begin(); it != m_TeamCountInfo.end(); ++it)
	{
		if(it->first == TEAM_TYPE_COUPLE)
		{
			(it->second).DayZero();
		}
		(it->second).WeekZero();
	}
	m_weekNestRewardCount = 0;
	m_diamondCostCount = 0;
	ClearDragonHelpFetchedReward();
}


void CTeamRecord::MarkTeamCostInfo(UINT32 expid, UINT32 index)
{
	m_teamcost.set_expid(expid);
	m_teamcost.set_costindex(index);

	m_IsModified.Set();
	SSInfo<<"mark team cost "<<this<<" expid:"<<expid<<" index"<<index<<END;
}

void CTeamRecord::RecoverTeamCostInfo(TeamCostRecoverReason reason/* = TCRR_NORMAL*/)
{
	if (IsInValidCostInfo())
	{
		return;
	}
	UINT32 expid = m_teamcost.expid();
	UINT32 costindex = m_teamcost.costindex();
	const Sequence<UINT32, 3>* ret = ExpeditionConfigMgr::Instance()->GetTeamCost(expid, costindex);
	ResetTeamCostInfo(reason);

	if (NULL == ret)
	{
		SSWarn<<"recover team cost, invalid "<<this<<" expid:"<<expid<<" costindex:"<<costindex<<END;
		return;
	}

	std::vector<ItemDesc> items;
	items.push_back(ItemDesc(ret->seq[1], ret->seq[2]));

	// send mail
	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_TeamCostBack);
	MailSender sender;
	sender.SetContent(Mail_System, NULL == conf ? "" : conf->m_title,  NULL == conf ? "" : conf->m_content);
	sender.SetReason(ItemFlow_TeamCostBack);
	sender.SetItems(items);
	sender.Send(m_pRole->GetID());

	SSInfo<<"recover team cost, item:"<<ret->seq[1]<<" count:"<<ret->seq[2]<<" "<<this<<END;
}

void CTeamRecord::ResetTeamCostInfo(TeamCostRecoverReason reason)
{
	m_teamcost.set_costindex(TeamCostInvalidID);
	m_teamcost.set_expid(0);

	m_IsModified.Set();

	SSInfo<<"reset team cost "<<this<<END;

	int err = 0;
	switch(reason)
	{
	case TCRR_NORMAL:
		{
			err = KKSG::ERR_TEAM_COST_CANCEL;
		}
		break;
	case TCRR_WIN:
		{
			err = KKSG::ERR_TEAM_COST_WIN;
		}
		break;
	default:
		break;
	}
	if(err)
	{
		PtcG2C_GSErrorNotify ptc;
		ptc.m_Data.set_errorno(err);
		ptc.m_Data.set_istip(true);
		m_pRole->Send(ptc);
	}
}

bool CTeamRecord::GetTeamCostInfo(UINT32& expid, UINT32& index)
{
	if (IsInValidCostInfo())
	{
		return false;
	}
	expid = m_teamcost.expid();	
	index = m_teamcost.costindex();
	return true;
}

bool CTeamRecord::IsInValidCostInfo()
{
	UINT32 expid = m_teamcost.expid();
	UINT32 costindex = m_teamcost.costindex();
	if (0 == expid || (UINT32)(-1) == costindex)
	{
		return true;
	}
	return false;
}

void CTeamRecord::UpdateCostInfo()
{
	UINT32 uniqueday = GameTime::GetTodayUnique();
	if (uniqueday != m_teamcost.updateday())
	{
		m_teamcost.set_dragoncount(0);
		m_teamcost.set_updateday(uniqueday);
		m_teamcost.set_getgiftvalue(0);

		m_IsModified.Set();
	}
}

UINT32 CTeamRecord::GetTeamCostCount()
{
	UpdateCostInfo();
	return m_teamcost.dragoncount();
}

void CTeamRecord::AddTeamCostCount()
{
	UpdateCostInfo();
	UINT32 count = m_teamcost.dragoncount();
	m_teamcost.set_dragoncount(count + 1);

	m_IsModified.Set();
}

UINT32 CTeamRecord::GetTeamCostGiftValue()
{
	return m_teamcost.getgiftvalue();
}

void CTeamRecord::SetTeamCostGift(UINT32 value)
{
	m_IsModified.Set();
	m_teamcost.set_getgiftvalue(value);
}

UINT32 CTeamRecord::GetWNRewardLeftCount()
{
	UINT32 maxCount = GetWNRewardMaxCount();
	if(maxCount > m_weekNestRewardCount)
	{
		return maxCount - m_weekNestRewardCount;
	}
	return 0;
}

UINT32 CTeamRecord::GetWNRewardMaxCount()
{
	if(GSConfig::Instance()->IsCrossGS())
	{
		return ExpeditionConfigMgr::Instance()->GetNestWeekCount(ServerCrossDataMgr::Instance()->GetSealTypeByRoleID(m_pRole->GetID()));
	}
	else
	{
		return ExpeditionConfigMgr::Instance()->GetNestWeekCount(CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType);
	}
}

void CTeamRecord::AddWNRewardCount()
{
	++m_weekNestRewardCount;	
	m_IsModified.Set();
}

void CTeamRecord::DebugResetTeamCount()
{
	DayZero();
	WeekZero();
	m_IsModified.Set(); 
}

UINT32 CTeamRecord::GetDiamondCostCountLeft()
{
	if(GetGlobalConfig().TeamDiamondCostCount > m_diamondCostCount)
	{
		return GetGlobalConfig().TeamDiamondCostCount - m_diamondCostCount;
	}
	return 0;
}

void CTeamRecord::AddDiamondCostCount()
{
	++m_diamondCostCount;

	m_IsModified.Set();	
}

UINT32 CTeamRecord::GetUseTicketLeftCount()
{
	UINT32 maxCountToday = GetGlobalConfig().TeamUseTicketCount;
	if(maxCountToday > m_useTicketCount)
	{
		return maxCountToday - m_useTicketCount;
	}
	return 0;
}

void CTeamRecord::AddUseTicketCount()
{
	m_useTicketCount ++;
	m_IsModified.Set();
}

int CTeamRecord::GetStarProgressByType(UINT32 type)
{
	int expid = 0;
	std::map<UINT32, int> starexp;
	ExpeditionConfigMgr::Instance()->GetAllStarExpByType(type, starexp);
	for(auto it = starexp.begin(); it != starexp.end(); ++it)
	{
		UINT32 sceneID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(it->second);
		expid = it->second;
		if(!m_pRole->Get<StageMgr>()->IsPassed(sceneID))
		{
			break;
		}
	}
	return expid;//如果全部通关则是最后一个
}

void CTeamRecord::AddHelpCount(int type)
{
	m_TeamCountInfo[type].m_helpCount++;
	m_IsModified.Set();

	if(DRAGON_TEAM_SCENE == type)//龙本
	{
		DragonHelpConfig::Instance()->CheckRoleHint(m_pRole);
		DesignationMgr::Instance()->DargonAssistDesignation(m_pRole,m_TeamCountInfo[type].m_helpCount);
	}
}

void CTeamRecord::AddDragonHelpFetchedReward(int id)
{
	m_dragonHelpFetchedReward.insert(id);
	m_IsModified.Set();
}

bool CTeamRecord::HaveDragonHelpFetchedReward(int id)
{
	return (m_dragonHelpFetchedReward.find(id) != m_dragonHelpFetchedReward.end());
}

void CTeamRecord::ClearDragonHelpFetchedReward()
{
	m_dragonHelpFetchedReward.clear();
	m_IsModified.Set();
}

UINT32 CTeamRecord::GetHelpCount(int type)
{
	return m_TeamCountInfo[type].m_helpCount;
}

void CTeamRecord::SetWantDragonHelp(bool want)
{
	m_wantDragonHelp = want;
	m_IsModified.Set();
}

bool CTeamRecord::IsWantHelp(int type)
{
	if(DRAGON_TEAM_SCENE == type)
	{
		return IsWantDragonHelp();
	}
	return false;
}
