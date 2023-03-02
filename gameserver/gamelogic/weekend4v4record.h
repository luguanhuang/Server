#ifndef __WEEKEND4V4RECORD_H__
#define __WEEKEND4V4RECORD_H__

#include "unit/roleserilizeinterface.h"
#include <map>
#include "weekend4v4mgr.h"
class Role;

class WeekEnd4v4Record : public RoleSerializeInterface
{
public:
	WeekEnd4v4Record(Role* role);
	virtual ~WeekEnd4v4Record();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

    void ResetRecord();

    bool SetIndexWeekEnd();
    bool IsIndexWeekEndChange();

    bool CountCheckRewards(WEEKEND4V4_TYPEID type);
    bool CountCheckHallIconsOpen(WEEKEND4V4_TYPEID type);
    bool CountCheckHallIconsClose(WEEKEND4V4_TYPEID type);

    bool IncCount(WEEKEND4V4_TYPEID type);
    UINT32 GetCount();
    void ResetCount();
public:
	Role* m_pRole;
	Switch m_ismodify;

    UINT32 m_indexWeekEnd;
    WEEKEND4V4_TYPEID m_activityID;
    UINT32 m_count;
};

#endif