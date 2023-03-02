#include "pch.h"
#include "weekend4v4mgr.h"
#include "util/gametime.h"
#include "util/XCommon.h"
#include "util/timespecificmgr.h"
#include "pb/project.pb.h"
#include "weekend4v4record.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "MultActivityMgr.h"
#include "bagtransition.h"
#include "activity/ptcg2c_halliconsntf.h"
#include "table/OpenSystemMgr.h"

INSTANCE_SINGLETON(WeekEnd4v4Mgr);

WeekEnd4v4Mgr::WeekEnd4v4Mgr(){}

WeekEnd4v4Mgr::~WeekEnd4v4Mgr(){}

bool WeekEnd4v4Mgr::Init()
{
    if(!LoadFile())
    {
        return false;
    }

    TimeSpecific time(0, 0, 5);
    m_dayChange = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&WeekEnd4v4Mgr::UpdateOnDayPass, this));

	return true;
}

bool WeekEnd4v4Mgr::CheckFile()
{
    WeekEnd4v4List tmp;
    if(!tmp.LoadFile("table/WeekEnd4v4List.txt"))
    {
        SSWarn<<"load table/WeekEnd4v4List.txt failed"<<END;
        return false;
    }
    return true;
}

void WeekEnd4v4Mgr::ClearFile()
{
    m_weekend4v4List.Clear();
    m_weekendSort.clear();
}

bool WeekEnd4v4Mgr::LoadFile()
{
    if (!CheckFile())
    {
        return false;
    }

    ClearFile();

    if(!m_weekend4v4List.LoadFile("table/WeekEnd4v4List.txt"))
    {
        SSWarn<<"load table/WeekEnd4v4List.txt failed"<<END;
        return false;
    }

    UINT32 activitySize = m_weekend4v4List.Table.size();
    std::vector<WEEKEND4V4_TYPEID> tmpweekendSort(activitySize, WEEKEND4V4_TYPEID_NONE);
    for (UINT32 i = 0; i < activitySize; i++)
    {
        WeekEnd4v4List::RowData *pRowData = m_weekend4v4List.Table[i];
        if(!pRowData) return false;
        if(pRowData->ID <= WEEKEND4V4_TYPEID_NONE || pRowData->ID >= WEEKEND4V4_TYPEID_MAX)
        {
            LogError("table/WeekEnd4v4List.txt: enum has not this ID: %d\n", pRowData->ID);
            return false;
        }
        if(pRowData->Index > activitySize) 
        {
            LogError("table/WeekEnd4v4List.txt: Index Error.\n");
            return false;
        }
        if(pRowData->Index > 0)
        {
            tmpweekendSort[pRowData->Index-1] = (WEEKEND4V4_TYPEID)pRowData->ID;
        }
    }

    for (UINT32 i = 0; i < tmpweekendSort.size(); i++)
    {
        if(WEEKEND4V4_TYPEID_NONE != tmpweekendSort[i])
        {
            m_weekendSort.push_back(tmpweekendSort[i]);
        }
    }

    setActivityID();
    return true;
}

void WeekEnd4v4Mgr::Uninit()
{
    m_weekend4v4List.Clear();
    m_weekendSort.clear();
}

void WeekEnd4v4Mgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{

}

bool WeekEnd4v4Mgr::UpdateOnDayPass()
{
    UINT32 weekday = GameTime::GetWeekDay(false);
    if(1 == weekday)
    {
        setActivityID();
    }

    return true;
}

void WeekEnd4v4Mgr::setActivityID()
{
    m_indexWeekEnd = GetIndexWeekEnd(GameTime::GetTime());
    m_thisActivityID = GetActivityID(m_indexWeekEnd);
    m_nextActivityID = GetActivityID(m_indexWeekEnd+1);
}

UINT32 WeekEnd4v4Mgr::GetIndexWeekEnd(time_t nowtime)
{
    static const time_t baseCircle = XCommon::MakeTimeNew(2017, 6, 5);
    if(nowtime < baseCircle) 
    {
        SSError<<"WeekEnd4v4: System Time is Error."<<END;
        return (UINT32)-1;
    }
    return static_cast<UINT32>((nowtime - baseCircle) / (7*24*3600));
}

WEEKEND4V4_TYPEID WeekEnd4v4Mgr::GetActivityID(UINT32 indexWeekEnd)
{
    if(((UINT32)-1) == indexWeekEnd) return WEEKEND4V4_TYPEID_NONE;
    if(m_weekendSort.empty()) return WEEKEND4V4_TYPEID_NONE;
    return m_weekendSort[indexWeekEnd % m_weekendSort.size()];
}

WEEKEND4V4_TYPEID WeekEnd4v4Mgr::GetThisActivityID()
{
    return m_thisActivityID;
}

UINT32 WeekEnd4v4Mgr::GetThisWeekEndSceneID()
{
    return GetWeekEndSceneID(m_thisActivityID);
}

bool WeekEnd4v4Mgr::CheckSystemOpen(Role* pRole)
{
    if(OpenSystemMgr::Instance()->IsClosedSystem(SYS_WEEKEND4V4))
    {
        return false;
    }
    if(!MultActivityMgr::Instance()->IsActivityOnDay(MULT_ACTIVITY_WEEKEND4V4, GameTime::GetWeekDay(false)))
    {
        return false;
    }
    if(!MultActivityMgr::Instance()->IsActivityTimeOpen(MULT_ACTIVITY_WEEKEND4V4))
    {
        return false;
    }
    static int openlevel = OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(SYS_WEEKEND4V4);
    if (NULL != pRole && pRole->GetLevel() < openlevel)
    {
        return false;	
    }
    return true;
}

void WeekEnd4v4Mgr::FillWeekEnd4v4Info(Role* pRole, KKSG::WeekEnd4v4GetInfoRes& roRes)
{
    roRes.set_errorcode(KKSG::ERR_SUCCESS);
    roRes.set_thisactivityid(m_thisActivityID);
    roRes.set_nextactivityid(m_nextActivityID);
    WeekEnd4v4Record* record = pRole->Get<WeekEnd4v4Record>();
    roRes.set_joincount(record->GetCount());
}

UINT32 WeekEnd4v4Mgr::GetWeekEndSceneID(WEEKEND4V4_TYPEID type)
{
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return 0;
    }
    return m_weekend4v4List.Table[type-1]->SceneID;
}

UINT32 WeekEnd4v4Mgr::GetMaxCount(WEEKEND4V4_TYPEID type)
{
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return 0;
    }
    return m_weekend4v4List.Table[type-1]->RewardTimes;
}

UINT32 WeekEnd4v4Mgr::GetReviveSeconds(WEEKEND4V4_TYPEID type)
{
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return 0;
    }
    return m_weekend4v4List.Table[type-1]->ReviveSeconds;
}

UINT32 WeekEnd4v4Mgr::GetScoreRatio(WEEKEND4V4_TYPEID type)
{
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return 0;
    }
    return m_weekend4v4List.Table[type-1]->Ratio;
}

UINT32 WeekEnd4v4Mgr::GetHorseRacingRankPoint(UINT32 rank)
{
    vector<Sequence<uint, 2>>& tmp = m_weekend4v4List.Table[WEEKEND4V4_TYPEID_HORSERACING-1]->RankPoint;
    if(rank >= tmp.size())
    {
        return 0;
    }
    return tmp[rank][1];
}

UINT32 WeekEnd4v4Mgr::GetFullTime(WEEKEND4V4_TYPEID type)
{
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return 0;
    }
    return m_weekend4v4List.Table[type-1]->MaxTime;
}

UINT32 WeekEnd4v4Mgr::GetGoalScore(WEEKEND4V4_TYPEID type)
{
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return 0;
    }
    return m_weekend4v4List.Table[type-1]->GoalScore;
}

bool WeekEnd4v4Mgr::AddItem(Role* pRole, WEEKEND4V4_TYPEID type, bool win)
{
    if(NULL == pRole)
    {
        return false;
    }
    if(!pRole->Get<WeekEnd4v4Record>()->CountCheckRewards(type))
    {
        return false;
    }
    if(type <= WEEKEND4V4_TYPEID_NONE || type >= WEEKEND4V4_TYPEID_MAX)
    {
        return false;
    }
    std::vector<Sequence<UINT32, 2>>* pItems = NULL;
    if(win)
    {
        pItems = &(m_weekend4v4List.Table[type-1]->DropItems);
    }else{
        pItems = &(m_weekend4v4List.Table[type-1]->LoseDrop);
    }
    if(NULL == pItems)
    {
        return false;
    }
    BagGiveItemTransition give(pRole);
    switch(type)
    {
    case WEEKEND4V4_TYPEID_MONSTERFIGHT:
        give.SetReason(ItemFlow_WeekEnd4v4, ItemFlow_WeekEnd4v4_MONSTERFIGHT);
        break;
    case WEEKEND4V4_TYPEID_GHOSTACTION:
        give.SetReason(ItemFlow_WeekEnd4v4, ItemFlow_WeekEnd4v4_GHOSTACTION);
        break;
    case WEEKEND4V4_TYPEID_LIVECHALLENGE:
        give.SetReason(ItemFlow_WeekEnd4v4, ItemFlow_WeekEnd4v4_LIVECHALLENGE);
        break;
    case WEEKEND4V4_TYPEID_CRAZYBOMB:
        give.SetReason(ItemFlow_WeekEnd4v4, ItemFlow_WeekEnd4v4_CRAZYBOMB);
        break;
    case WEEKEND4V4_TYPEID_DUCK:
        give.SetReason(ItemFlow_WeekEnd4v4, ItemFlow_WeekEnd4v4_DUCK);
        break;
    case WEEKEND4V4_TYPEID_HORSERACING:
    default:
        give.SetReason(ItemFlow_WeekEnd4v4, ItemFlow_WeekEnd4v4_HORSERACING);
        break;
    }
    for(auto itor = pItems->begin(); itor != pItems->end(); ++itor)
    {
        give.GiveItem((*itor)[0], (*itor)[1]);
    }
    give.NotifyClient();
    return true;
}

bool WeekEnd4v4Mgr::GMChangeActivitySort(UINT32 newWeekID)
{
    setActivityID();
    if(((UINT32)-1) == m_indexWeekEnd) return false;

    if(newWeekID<=WEEKEND4V4_TYPEID_NONE || newWeekID>=WEEKEND4V4_TYPEID_MAX)
    {
        return false;
    }
    if(m_weekendSort.empty()) return false;
    UINT32 tmpIndex = (UINT32)-1;
    for(UINT32 i = 0; i<m_weekendSort.size(); ++i)
    {
        if(m_weekendSort[i] == newWeekID)
        {
            tmpIndex = i;
            break;
        }
    }
    if((UINT32)(-1) == tmpIndex)
    {
        return false;
    }

    std::vector<WEEKEND4V4_TYPEID> tmpVec(m_weekendSort);
    for(UINT32 i = 0; i<m_weekendSort.size(); ++i)
    {
        m_weekendSort[(m_indexWeekEnd+i) % m_weekendSort.size()] = tmpVec[(tmpIndex+i) % m_weekendSort.size()];
    }

    setActivityID();

    for(UINT32 i = 0; i<m_weekendSort.size(); ++i)
    {
        LogDebug("GMChangeActivitySort %d\n", m_weekendSort[i]);
    }

    return true;
}

void WeekEnd4v4Mgr::HallIconAllNtf(UINT32 nState)
{
    auto itend = RoleManager::Instance()->End();
    for(auto it = RoleManager::Instance()->Begin(); it != itend; ++it)
    {
        HallIconRoleNtf(*it,nState);
    }
}

void WeekEnd4v4Mgr::HallIconRoleNtf(Role* pRole, UINT32 nState)
{
    if(!pRole)
    {
        return;
    }
    if(KKSG::HICONS_BEGIN == nState && !CheckSystemOpen(pRole))
    {
        return;
    }
    if(KKSG::HICONS_BEGIN == nState && !pRole->Get<WeekEnd4v4Record>()->CountCheckHallIconsOpen(m_thisActivityID))
    {
        return;
    }
    PtcG2C_HallIconSNtf ptc;
    ptc.m_Data.set_systemid(SYS_WEEKEND4V4);
    ptc.m_Data.set_state((KKSG::HallIconState)nState);
    pRole->Send(ptc);
}
