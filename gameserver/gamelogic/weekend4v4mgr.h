#ifndef __WEEKEND4V4_H__
#define __WEEKEND4V4_H__

#include "table/WeekEnd4v4List.h"
#include "pb/project.pb.h"
#include "pb/enum.pb.h"

class Role;

enum WEEKEND4V4_TYPEID
{
    WEEKEND4V4_TYPEID_NONE = 0,             //��
    WEEKEND4V4_TYPEID_MONSTERFIGHT = 1,     //������Ҷ�
    WEEKEND4V4_TYPEID_GHOSTACTION = 2,      //�����ж�
    WEEKEND4V4_TYPEID_LIVECHALLENGE = 3,    //�������ս
    WEEKEND4V4_TYPEID_CRAZYBOMB = 4,        //���ը����
    WEEKEND4V4_TYPEID_HORSERACING = 5,      //�Ŷ�����
    WEEKEND4V4_TYPEID_DUCK = 6,             //��Ѽ��
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

    // pRoleΪNULL��ʾֻ���system���������
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
