#include "pch.h"
#include "checkinrecord.h"
#include "pb/project.pb.h"
#include "time.h"
#include "unit/role.h"
#include "util/XCommon.h"

CCheckinRecord::CCheckinRecord()
{
	Reset();
}

void CCheckinRecord::Reset()
{
	m_dwCheckinInfo = 0;
	m_dwCheckinTime = 0;
	m_dwCheckinCount = 0;
}

bool CCheckinRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_checkinrecord())
	{
		m_dwCheckinInfo = poRoleAllInfo->checkinrecord().checkininfo();
		m_dwCheckinTime = poRoleAllInfo->checkinrecord().checkintime();
		m_dwCheckinCount = poRoleAllInfo->checkinrecord().checkincount();
	}
	return true;
}

void CCheckinRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_bIsModify.TestAndReset())
	{
		KKSG::CheckinRecord* pCheckinRecord = poRoleAllInfo->mutable_checkinrecord();
		pCheckinRecord->Clear();
		pCheckinRecord->set_checkininfo(m_dwCheckinInfo);
		pCheckinRecord->set_checkintime(m_dwCheckinTime);
		pCheckinRecord->set_checkincount(m_dwCheckinCount);

		roChanged.insert(pCheckinRecord);
	}
}

void CCheckinRecord::Update()
{
	UINT32 nowday = XCommon::GetMonthUnique();	
	if (nowday != m_dwCheckinTime)
	{
		Reset();
		m_dwCheckinTime = nowday;
		m_bIsModify.Set();
	}
}

UINT32 CCheckinRecord::GetCheckInfo()
{
	return m_dwCheckinInfo;
}

UINT32 CCheckinRecord::CheckinDays()
{
	UINT32 i = 0;
	UINT32 dwTempCheckinInfo = m_dwCheckinInfo;
	while(0 != dwTempCheckinInfo)
	{
		++i;
		dwTempCheckinInfo = dwTempCheckinInfo & (dwTempCheckinInfo -1);
	}
	return i;
}

UINT32 CCheckinRecord::GetCheckinCount()
{
	return m_dwCheckinCount;
}

UINT32 CCheckinRecord::GetFirstUnCheckinDay(UINT32 dwStartDay)
{
	UINT32 dwTempCheckinInfo = m_dwCheckinInfo >> (dwStartDay - 1);
	UINT32 i = 0;
	while(0x01 & (dwTempCheckinInfo >> i))
	{
		++i;
	}
	return i + dwStartDay - 1;
}

void CCheckinRecord::SetCheckinInfo(UINT32 dwCheckinDay, bool IsCheckLast)
{
	m_dwCheckinInfo |= 1 << dwCheckinDay;
	if (IsCheckLast)
	{
		++m_dwCheckinCount;
	}
	m_bIsModify.Set();
	return;
}

bool CCheckinRecord::IsDayChecked(UINT32 dwCheckinDay)
{
	if ((1 << dwCheckinDay) & m_dwCheckinInfo)
	{
		return true;
	}
	return false;
}
