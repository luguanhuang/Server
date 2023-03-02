#include "pch.h"
#include <time.h>
#include "designationRecord.h"
#include "pb/project.pb.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "QARecord.h"
#include "util/gametime.h"

QARecord::QARecord(Role* role)
:m_pRole(role)
,m_curQAType(0)
,m_lastEndTime(0)
,m_lastResetTime(0)
{

}

QARecord::~QARecord()
{

}


bool QARecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	const KKSG::SQARecord& info = poRoleAllInfo->qarecord();
	m_curQAType = info.cur_qa_type();
	m_lastEndTime = info.last_end_time();
	m_lastResetTime = info.last_reset_time();

	for (INT32 i = 0; i < info.trigger_time_size(); i++)
	{
		UINT32 key = (UINT32)info.trigger_time(i).key();
		UINT32 value = (UINT32)info.trigger_time(i).value();
		m_triggerTime[key] = value;
	}

	for (INT32 i = 0; i < info.used_count_size(); i++)
	{
		UINT32 key = (UINT32)info.used_count(i).key();
		UINT32 value = (UINT32)info.used_count(i).value();
		m_usedCount[key] = value;
	}

	OnDayPass();	
	return true;
}

void QARecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (!m_ismodify.TestAndReset())
	{
		return;
	}

	KKSG::SQARecord& info = *poRoleAllInfo->mutable_qarecord();
	info.set_cur_qa_type(m_curQAType);
	info.set_last_end_time(m_lastEndTime);
	info.set_last_reset_time(m_lastResetTime);

	info.clear_trigger_time();
	info.clear_used_count();
	for (auto it = m_triggerTime.begin(); it != m_triggerTime.end(); ++it)
	{
		KKSG::MapKeyValue* pData = info.add_trigger_time();
		pData->set_key(it->first);
		pData->set_value(it->second);
	}
	for (auto it = m_usedCount.begin(); it != m_usedCount.end(); ++it)
	{
		KKSG::MapKeyValue* pData = info.add_used_count();
		pData->set_key(it->first);
		pData->set_value(it->second);
	}

	roChanged.insert(&info);
}

void QARecord::OnDayPass(bool force)
{
	UINT32 now = GameTime::GetTime();
	if (XCommon::IsDaySame(m_lastResetTime, now) && !force)
	{
		return;
	}
	m_curQAType = 0;
	m_lastEndTime = 0;
	m_triggerTime.clear();
	m_usedCount.clear();

	m_lastResetTime = now;
	m_ismodify.Set();
}

void QARecord::AddQACount( UINT32 qaType )
{
	m_triggerTime[qaType] = GameTime::GetTime();
	m_usedCount[qaType]++;
	m_ismodify.Set();
}

UINT32 QARecord::GetQACount(UINT32 type)
{
	auto it = m_usedCount.find(type);
	if (it == m_usedCount.end())
	{
		return 0;
	}
	return it->second;
}

void QARecord::SetTrigger(UINT32 type)
{
	m_triggerTime[type] = GameTime::GetTime();
	m_ismodify.Set();
}

UINT32 QARecord::GetTriggerTime(UINT32 type)
{
	auto it = m_triggerTime.find(type);
	if (it == m_triggerTime.end())
	{
		return 0;
	}
	return it->second;
}

void QARecord::SetCurQA(UINT32 type, UINT32 endTime)
{
	m_curQAType = type;
	m_lastEndTime = endTime;
	m_ismodify.Set();
}

UINT32 QARecord::GetCurType()
{
	return m_curQAType;
}

bool QARecord::IsPlayingQA()
{
	UINT32 now = GameTime::GetTime();
	if (m_lastEndTime < now)
	{
		return false;
	}
	return true;
}
