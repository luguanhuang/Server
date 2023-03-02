#ifndef __GOALAWARDS_MGR_H__
#define __GOALAWARDS_MGR_H__

#include "pb/project.pb.h"
#include "table/GoalAwards.h"
#include "util/uniquequeue.h"
#include "goalawarddef/goalawardsdef.h"

enum GOAL_AWARD_TYPE
{
    GOAL_AWARD_TYPE_NONE,
    GOAL_AWARD_TYPE_SET_UP,
    GOAL_AWARD_TYPE_SET_DOWN,
    GOAL_AWARD_TYPE_ADD_UP,
    GOAL_AWARD_TYPE_GK_UP,
    GOAL_AWARD_TYPE_GK_DOWN,
    GOAL_AWARD_TYPE_GK_PASS,
    GOAL_AWARD_TYPE_GK_ADD_UP
};

struct GoalAwardInfo
{
    UINT32 mGoalAwardsID;
    UINT32 mDoneIndex;
    UINT32 mGottenAwardsIndex;
    std::map<UINT32, double> mScore;

    GoalAwardInfo()
        : mGoalAwardsID(0), mDoneIndex(0), mGottenAwardsIndex(0)
    {
    }
};

class CRole;
class GoalAwardsMgr : public ITimer
{
    GoalAwardsMgr();
    ~GoalAwardsMgr();
    DECLARE_SINGLETON(GoalAwardsMgr);
public:
    bool Init();
    void Uninit();

    bool CheckFile();
    bool LoadFile();
    void ClearFile();

    virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

public:
    GoalAwards::RowData* GetTableRow(UINT32 idGoalAwards, UINT32 indexAwards);
private:
    GoalAwards m_GoalAwardsTable;
    std::map<UINT32, std::vector<UINT32>> m_GoalAwardsTableIndex;

public:
    void RpcGoalAwardsGetList(CRole* pRole, const KKSG::GoalAwardsGetList_C2M &roArg, KKSG::GoalAwardsGetList_M2C &roRes);
    void RpcGoalAwardsGetAwards(CRole* pRole, const KKSG::GoalAwardsGetAwards_C2M &roArg, KKSG::GoalAwardsGetAwards_M2C &roRes);
    void PtcGoalAwardsRedPoint(UINT64 roleid);
private:
    std::unordered_map<UINT64, std::map<UINT32, GoalAwardInfo>> m_RoleData;

public:
    UINT32 GetTableRowType(const GoalAwardInfo& info);
    bool CheckHasAwards(UINT64 roleid, const GoalAwardInfo& info);
    GOAL_AWARD_TYPE GetGoalAwardType(UINT32 idGoalAwards);
    void FreshValue(UINT64 roleid, UINT32 idGoalAwards, double value, UINT32 gkid);
    void GMClearDB(UINT64 roleid);
    void SaveToDB(){}

private:
    void FreshGoalAwardsInfo(GoalAwardInfo& info);
    std::map<UINT32, GoalAwardInfo>& InitRoleData(UINT64 roleid);
    void _FreshValue(std::map<UINT32, GoalAwardInfo>& roledate, UINT64 roleid, UINT32 idGoalAwards, double value, UINT32 gkid);
    void _FreshValueList(std::map<UINT32, GoalAwardInfo>& roledate, UINT64 roleid, UINT32 idStart, UINT32 idEnd, double value, UINT32 gkid);
public:
    bool QueryDBGoalAwardsInfo();
    bool SaveDBGoalAwardsInfo(UINT64 roleid);

    void DoTLogGoalAwards(UINT64 roleid, UINT32 awardid, UINT32 awardindex, UINT32 awardtype, UINT32 op);

private:
    UniqueQueue<UINT64> m_isChangedGoalAwardsInfo;
    UINT32 m_saveToDBCount;
    HTIMER m_timeHandler;
};

#endif
