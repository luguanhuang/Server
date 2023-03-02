#include "pch.h"
#include "goalawards/ptcg2m_goalawardsserverteam.h"
#include "goalawards/goalawardsmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "platforminfo/platformfriendmgr.h"
#include "friend/friendop.h"
// generate by ProtoGen at date: 2017/9/13 10:43:48

void PtcG2M_GoalAwardsServerTeam::Process(UINT32 dwConnID)
{
    UINT64 roleid = m_Data.roleid();
    CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
    if(NULL == pRoleSummary) return;
    FriendOp roleop(roleid);

    bool hasFriends = false;
    bool hasWXQQFriends = false;
    for(int i = 0; i < m_Data.teamidlist().size(); ++i)
    {
        UINT64 teamerid = m_Data.teamidlist(i);
        if(roleid == teamerid) continue;
        
        if(roleop.IsFriend(teamerid)) hasFriends = true;

        CRoleSummary* pTmpSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(teamerid);
        if(NULL == pTmpSummary) continue;
        bool wxqq = CPlatformFriendMgr::Instance()->IsPlatFriend(pRoleSummary->GetAccount(), pTmpSummary->GetAccount());
        if(wxqq) hasWXQQFriends = true;

        if(hasFriends && hasWXQQFriends)
        {
            break;
        }
    }
    if(hasFriends)
    {
        GoalAwardsMgr::Instance()->FreshValue(roleid, GOAL_AWARD_ID_601, 1.f, m_Data.gkid());
    }
    if(hasWXQQFriends)
    {
        GoalAwardsMgr::Instance()->FreshValue(roleid, GOAL_AWARD_ID_603, 1.f, m_Data.gkid());
    }
}
