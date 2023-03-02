#include "pch.h"
#include "weekend4v4record.h"
#include "unit/role.h"
#include "util/gametime.h"
#include "weekend4v4mgr.h"

WeekEnd4v4Record::WeekEnd4v4Record(Role* role)
    :m_pRole(role), m_ismodify(), m_indexWeekEnd(0), 
    m_activityID(WEEKEND4V4_TYPEID_NONE), m_count(0)
{
}

WeekEnd4v4Record::~WeekEnd4v4Record()
{
}


bool WeekEnd4v4Record::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
    if(!SetIndexWeekEnd()) return false;
    const KKSG::WeekEnd4v4Data& info = poRoleAllInfo->weekend4v4data();
    //if(info.indexweekend() != m_indexWeekEnd ||(WEEKEND4V4_TYPEID)info.activityid() != m_activityID)
    if(info.indexweekend() != m_indexWeekEnd)
    {
        ResetCount();
        return true;
    }
    m_count = info.count();
	return true;
}

void WeekEnd4v4Record::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
    ResetRecord();
	if (!m_ismodify.TestAndReset())
	{
		return;
	}
    KKSG::WeekEnd4v4Data& info = *poRoleAllInfo->mutable_weekend4v4data();
    info.set_indexweekend(m_indexWeekEnd);
    info.set_activityid(m_activityID);
    info.set_count(m_count);
	roChanged.insert(&info);
}

void WeekEnd4v4Record::ResetRecord()
{
    if(!IsIndexWeekEndChange())return;
    SetIndexWeekEnd();
    ResetCount();
}

bool WeekEnd4v4Record::SetIndexWeekEnd()
{
    m_indexWeekEnd = WeekEnd4v4Mgr::Instance()->GetIndexWeekEnd(GameTime::GetTime());
    if((UINT32)-1 == m_indexWeekEnd) return false;
    m_activityID = WeekEnd4v4Mgr::Instance()->GetThisActivityID();
    return true;
}

bool WeekEnd4v4Record::IsIndexWeekEndChange()
{
    UINT32 tmpIndexWeekEnd = WeekEnd4v4Mgr::Instance()->GetIndexWeekEnd(GameTime::GetTime());
    if((UINT32)-1 == tmpIndexWeekEnd) return false;
    if(tmpIndexWeekEnd == m_indexWeekEnd) return false;
    return true;
}

bool WeekEnd4v4Record::CountCheckRewards(WEEKEND4V4_TYPEID type)
{
    UINT32 maxcount = WeekEnd4v4Mgr::Instance()->GetMaxCount(type);
    return m_count <= maxcount;
}

bool WeekEnd4v4Record::CountCheckHallIconsOpen(WEEKEND4V4_TYPEID type)
{
    UINT32 maxcount = WeekEnd4v4Mgr::Instance()->GetMaxCount(type);
    return m_count < maxcount;
}

bool WeekEnd4v4Record::CountCheckHallIconsClose(WEEKEND4V4_TYPEID type)
{
    UINT32 maxcount = WeekEnd4v4Mgr::Instance()->GetMaxCount(type);
    return m_count == maxcount;
}

bool WeekEnd4v4Record::IncCount(WEEKEND4V4_TYPEID type)
{
    ++m_count;
    if(CountCheckHallIconsClose(type))
    {
        WeekEnd4v4Mgr::Instance()->HallIconRoleNtf(m_pRole, KKSG::HICONS_END);
    }
    m_ismodify.Set();
    return true;
}

UINT32 WeekEnd4v4Record::GetCount()
{
    ResetRecord();
    return m_count;
}

void WeekEnd4v4Record::ResetCount()
{
    m_count = 0;
    m_ismodify.Set();
}
