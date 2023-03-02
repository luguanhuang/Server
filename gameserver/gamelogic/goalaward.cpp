#include "pch.h"
#include "goalaward.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "network/mslink.h"
#include "goalawards/ptcg2m_goalawardsserverevent.h"
#include "goalawards/ptcg2m_goalawardsserverteam.h"

INSTANCE_SINGLETON(GoalAwardsConfig);

GoalAwardsConfig::GoalAwardsConfig(){}

GoalAwardsConfig::~GoalAwardsConfig(){}

bool GoalAwardsConfig::Init()
{
    return true;
}

void GoalAwardsConfig::Uninit()
{
}

bool GoalAwardsConfig::CheckFile()
{
    return true;
}

bool GoalAwardsConfig::LoadFile()
{
    return true;
}

void GoalAwardsConfig::ClearFile()
{
}

void GoalAwardsConfig::SetValue(UINT64 roleid, GOAL_AWARD_ID idGoalAwards, double value, UINT32 gkid)
{
    PtcG2M_GoalAwardsServerEvent ptc;
    ptc.m_Data.set_roleid(roleid);
    ptc.m_Data.set_goalawardsid(idGoalAwards);
    ptc.m_Data.set_num(value);
    ptc.m_Data.set_gkid(gkid);
    MSLink::Instance()->SendTo(roleid, ptc);
}

void GoalAwardsConfig::TeamFriend(std::vector<Role*> &roRoles, UINT32 gkid, UINT32 stype)
{
    PtcG2M_GoalAwardsServerTeam ptc;
    for(auto itor = roRoles.begin(); itor != roRoles.end(); ++itor)
    {
        if(NULL != *itor)
        {
            ptc.m_Data.add_teamidlist((*itor)->GetRoleID());
        }
    }
    ptc.m_Data.set_gkid(gkid);
    ptc.m_Data.set_goaltype(stype);
    for(auto itor = roRoles.begin(); itor != roRoles.end(); ++itor)
    {
        if(NULL != *itor)
        {
            ptc.m_Data.set_roleid((*itor)->GetRoleID());
            MSLink::Instance()->SendTo((*itor)->GetRoleID(), ptc);
        }
    }
}

void GoalAwardsConfig::OnLevelUp(Role* pRole)
{
    SetValue(pRole->GetRoleID(), GOAL_AWARD_ID_1, 0.f, 0);
}
