#ifndef __GOALAWARDS_CONFIG_H__
#define __GOALAWARDS_CONFIG_H__

#include "goalawarddef/goalawardsdef.h"

class Role;
class GoalAwardsConfig
{
    GoalAwardsConfig();
    ~GoalAwardsConfig();
    DECLARE_SINGLETON(GoalAwardsConfig);
public:
    bool Init();
    void Uninit();

    bool CheckFile();
    bool LoadFile();
    void ClearFile();
public:
    void SetValue(UINT64 roleid, GOAL_AWARD_ID idGoalAwards, double value, UINT32 gkid);
    void TeamFriend(std::vector<Role*> &roRoles, UINT32 gkid, UINT32 stype);
    void OnLevelUp(Role* pRole);
};

#endif
