#include "pch.h"
#include "liverecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "unit/role.h"
#include "table/globalconfig.h"
#include <time.h>
#include "gamelogic/bagtransition.h"
#include "foreach.h"
#include "activity/rpcc2g_gettoweractivitytop.h"
#include "livemanager.h"
#include "liveinfo.h"

CLiveRecord::CLiveRecord(Role* role)
{
	m_pRole = role;
	m_liveVisibleSetting = true;
	m_mostCommendLive = new KKSG::OneLiveRecordInfo();
	m_mostViewLive = new KKSG::OneLiveRecordInfo();
	m_totalCommend = 0;
	m_totalView = 0;
}

CLiveRecord::~CLiveRecord()
{
	delete m_mostViewLive;
	delete m_mostCommendLive;
	foreach(i in m_recentLives)
	{
		delete *i;
	}
	m_recentLives.clear();
}

bool CLiveRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_liverecord())
	{
		const KKSG::LiveRecord* record = &poRoleAllInfo->liverecord();
		m_totalCommend = record->mytotalcommendednum();
		m_totalView = record->mytotalviewednum();
		if (record->has_mostviewedrecord())
		{
			m_mostViewLive->CopyFrom(record->mostviewedrecord());
		}
		if (record->has_mostcommendedrecord())
		{
			m_mostCommendLive->CopyFrom(record->mostcommendedrecord());
		}
		for (size_t i = 0; i < record->recentrecords_size(); ++i)
		{
			KKSG::OneLiveRecordInfo* one = new KKSG::OneLiveRecordInfo();
			one->CopyFrom(record->recentrecords(i));
			m_recentLives.push_back(one);
		}
		if (record->has_livevisible())
		{
			m_liveVisibleSetting = record->livevisible();
		}
		
	}
	return true;
}

void CLiveRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_switch.TestAndReset())
	{
		KKSG::LiveRecord* record = poRoleAllInfo->mutable_liverecord();
		record->Clear();
		record->set_mytotalviewednum(m_totalView);
		record->set_mytotalcommendednum(m_totalCommend);
		record->set_livevisible(m_liveVisibleSetting);
		if (m_mostCommendLive->liveid())
		{
			auto info = record->mutable_mostcommendedrecord();
			info->CopyFrom(*m_mostCommendLive);
		}
		if (m_mostViewLive->liveid())
		{
			auto info = record->mutable_mostviewedrecord();
			info->CopyFrom(*m_mostViewLive);
		}
		foreach(i in m_recentLives)
		{
			auto info = record->add_recentrecords();
			info->CopyFrom(**i);
		}

		roChanged.insert(record);
	}
}

void CLiveRecord::Change()
{
	m_switch.Set();
}


void CLiveRecord::DebugReset()
{
	int now = (int)time(0);

	//Update();
}

void CLiveRecord::UpdateRecord(LiveInfo* info)
{
	if (!info)
	{
		return;
	}

	KKSG::OneLiveRecordInfo* addInfo = new KKSG::OneLiveRecordInfo();
	bool success = info->Convert2DBKKSG_OneLiveRecord(addInfo);
	if (!success)
	{
		delete addInfo;
		return;
	}
	m_recentLives.push_front(addInfo);
	if (m_recentLives.size() > 3)
	{
		auto pInfo = m_recentLives.back();
		delete pInfo;
		m_recentLives.pop_back();
	}

	if (info->m_watchNum > m_mostViewLive->watchnum() || !m_mostViewLive->liveid())
	{
		m_mostViewLive->Clear();
		info->Convert2DBKKSG_OneLiveRecord(m_mostViewLive);
	}
	
	if (info->m_commendNum > m_mostCommendLive->commendnum() || !m_mostCommendLive->liveid())
	{
		m_mostCommendLive->Clear();
		info->Convert2DBKKSG_OneLiveRecord(m_mostCommendLive);
	}

	m_switch.Set();
}

UINT32 CLiveRecord::GetMyRecord2Client(KKSG::GetMyWatchRecordRes& roRes)
{
	roRes.set_mycommendednum(m_totalCommend);
	roRes.set_mywatchednum(m_totalView);
	if (m_mostCommendLive->liveid())
	{
		auto info = roRes.mutable_mymostcommendedrecord();
		info->CopyFrom(*m_mostCommendLive);
	}

	if (m_mostViewLive->liveid())
	{
		auto info = roRes.mutable_mymostwatchedrecord();
		info->CopyFrom(*m_mostViewLive);
	}
	foreach(i in m_recentLives)
	{
		auto info = roRes.add_myrecentrecords();
		info->CopyFrom(**i);
	}
	roRes.set_visiblesetting(m_liveVisibleSetting);

	return KKSG::ERR_SUCCESS;
}

UINT32 CLiveRecord::ChangeVisible(bool visible)
{
	m_liveVisibleSetting = visible;
	Change();

	return KKSG::ERR_SUCCESS;
}

void CLiveRecord::GetMaxCommendViewNum(UINT32& maxCommadNum, UINT32& maxWatchNum)
{
    maxCommadNum = 0;
    maxWatchNum = 0;
    if(m_mostCommendLive)maxCommadNum = m_mostCommendLive->commendnum();
    if(m_mostViewLive)maxWatchNum = m_mostViewLive->watchnum();
}
