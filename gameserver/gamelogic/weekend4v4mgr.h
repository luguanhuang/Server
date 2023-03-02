#ifndef __WEEKEND4V4_H__
#define __WEEKEND4V4_H__

#include "table/WeekEnd4v4List.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"

class Role;

enum WEEKEND4V4_TYPEID
{
    WEEKEND4V4_TYPEID_NONE = 0,             //无
    WEEKEND4V4_TYPEID_MONSTERFIGHT = 1,     //怪物大乱斗
    WEEKEND4V4_TYPEID_GHOSTACTION = 2,      //幽灵行动
    WEEKEND4V4_TYPEID_LIVECHALLENGE = 3,    //生存大挑战
    WEEKEND4V4_TYPEID_CRAZYBOMB = 4,        //疯狂炸弹人
    WEEKEND4V4_TYPEID_HORSERACING = 5,      //团队赛马
    WEEKEND4V4_TYPEID_DUCK = 6,             //钩鸭子
    WEEKEND4V4_TYPEID_MAX
};



class WeekEnd4v4Mgr : public ITimer
{
	WeekEnd4v4Mgr();
	~WeekEnd4v4Mgr();
	DECLARE_SINGLETON(WeekEnd4v4Mgr);
public:
    virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
    bool UpdateOnDayPass();

	bool Init();
	void Uninit();

    bool CheckFile();
    void ClearFile();
    bool LoadFile();

    UINT32 GetIndexWeekEnd(time_t nowtime);

    WEEKEND4V4_TYPEID GetThisActivityID();
    UINT32 GetThisWeekEndSceneID();

    // pRole为NULL表示只检查system级别的条件
    bool CheckSystemOpen(Role* pRole = NULL);

    void FillWeekEnd4v4Info(Role* pRole, KKSG::WeekEnd4v4GetInfoRes& roRes);

    UINT32 GetWeekEndSceneID(WEEKEND4V4_TYPEID type);
    UINT32 GetMaxCount(WEEKEND4V4_TYPEID type);
    UINT32 GetReviveSeconds(WEEKEND4V4_TYPEID type);
    UINT32 GetScoreRatio(WEEKEND4V4_TYPEID type);
    UINT32 GetHorseRacingRankPoint(UINT32 rank);
    UINT32 GetFullTime(WEEKEND4V4_TYPEID type);
    UINT32 GetGoalScore(WEEKEND4V4_TYPEID type);
    bool AddItem(Role* pRole, WEEKEND4V4_TYPEID wktypeid, bool win);

    bool GMChangeActivitySort(UINT32 newWeekID);

    void HallIconAllNtf(UINT32 nState);
    void HallIconRoleNtf(Role* pRole, UINT32 nState);

    void setActivityID();
private:
    WEEKEND4V4_TYPEID GetActivityID(UINT32 indexWeekEnd);

    WeekEnd4v4List m_weekend4v4List;
    std::vector<WEEKEND4V4_TYPEID> m_weekendSort;
    UINT32 m_indexWeekEnd;
    WEEKEND4V4_TYPEID m_thisActivityID;
    WEEKEND4V4_TYPEID m_nextActivityID;

    UINT32 m_dayChange;
};

#endif
