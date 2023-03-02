#include "pch.h"
#include "activityrecord.h"
#include "pb/project.pb.h"
#include "time.h"
#include "util/XRandom.h"
#include "util/XCommon.h"
#include "activitymgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "unit/role.h"
#include "guild/ptcg2m_synaddguildactivity.h"
#include "mslink.h"
#include "gamelogic/teamcostconfig.h"
#include "gamelogic/rolepartner.h"
#include "partner/ptcg2m_addpartnerlivenessg2m.h"
#include "util/gametime.h"
#include "gamelogic/teamrecord.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "utility/tlogger.h"
#include "gamelogic/bagtransition.h"

CActivityRecord::CActivityRecord(Role* role):m_pRole(role)
{
	m_dwChestGetInfo = 0;
	Reset();
}

void CActivityRecord::Reset()
{
	m_dwAllValue = 0;
	m_dwDoubleId = 0;
	std::vector<UINT32> recived_id;
	// 日活跃的清空，周活跃的保留
	UINT32 day_index = GetGlobalConfig().ActivityChestExp.size() - GetGlobalConfig().ActivityChestExpWeekly.size();
	for (UINT32 index = 0; index < day_index; ++index )
	{
		if(m_dwChestGetInfo & (1 << index))
		{
			m_dwChestGetInfo = m_dwChestGetInfo & (~(1 << index));
		}
	}
	m_guildLadderTime = 0;
	m_mapFinishCount.clear();
}

bool CActivityRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_activityrecord())
	{
		const KKSG::ActivityRecord* pRecord = &poRoleAllInfo->activityrecord();
		m_dwAllValue = pRecord->activityallvalue();
		m_dwDoubleId = pRecord->doubleactivityid();
		m_dwChestGetInfo = pRecord->chestgetinfo();
		m_dwLastUpdateTime = (time_t)pRecord->lastupdatetime();
		m_dwWeekValue = pRecord->activityweekvalue();
		m_guildLadderTime = pRecord->guildladdertime();
		for (int i = 0; i < pRecord->activityid_size(); ++i)
		{
			UINT32 new_activity_id = CActivityMgr::Instance()->ChangeActivityID(pRecord->activityid(i));
			m_mapFinishCount[new_activity_id] = pRecord->finishcount(i);
			//m_mapFinishCount[pRecord->activityid(i)] = pRecord->finishcount(i);
		}
	}
	return true;
}

void CActivityRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_bIsModify.TestAndReset())
	{
		KKSG::ActivityRecord* pRecord = poRoleAllInfo->mutable_activityrecord();
		pRecord->Clear();
		RecordToKKSG(pRecord);

		roChanged.insert(pRecord);
	}
}

void CActivityRecord::RecordToKKSG(KKSG::ActivityRecord* pRecord)
{
	pRecord->set_activityallvalue(m_dwAllValue);
	pRecord->set_doubleactivityid(m_dwDoubleId);
	pRecord->set_chestgetinfo(m_dwChestGetInfo);
	pRecord->set_lastupdatetime((UINT64)m_dwLastUpdateTime);
	pRecord->set_activityweekvalue(m_dwWeekValue);
	pRecord->set_guildladdertime(m_guildLadderTime);

	std::map<UINT32, std::pair<UINT32, UINT32>> indexMap;
	for (auto i = m_mapFinishCount.begin(); i != m_mapFinishCount.end(); i++)
	{
		//indexMap[CActivityMgr::Instance()->GetIndexID(i->first)] = std::make_pair(i->first, i->second);	
		pRecord->add_activityid(i->first);
		pRecord->add_finishcount(i->second);
	}
	/*for (auto i = indexMap.begin(); i != indexMap.end(); i++)
	{
	pRecord->add_activityid(i->second.first);
	pRecord->add_finishcount(i->second.second);
	}*/
}

/*void CActivityReocrd::RecordChangedToKKSG(KKSG::ActivityRecord* pRecord)
{
pRecord->set_activityallvalue(m_dwAllValue);
pRecord->set_doubleactivityid(m_dwDoubleId);
pRecord->set_chestgetinfo(m_dwChestGetInfo);
pRecord->set_recordtime(m_dwRecordTime);
for (auto i = m_setChangedCount.begin(); i != m_setChangedCount.end(); ++i)
{
auto j = m_mapFinishCount.find(*i);
if (j != m_mapFinishCount.end())
{
pRecord->add_activityid(j->first);
pRecord->add_finishcount(j->second);
}
}
m_setChangedCount.clear();
}*/

bool CActivityRecord::Update()
{
	time_t nowTime = GameTime::GetTime();
	if (!XCommon::IsDaySame(m_dwLastUpdateTime, nowTime))
	{
		Reset();		
		if (!XCommon::IsWeekSame(m_dwLastUpdateTime, nowTime))
		{
			m_dwWeekValue = 0;
			m_dwChestGetInfo = 0;
		}

		m_dwLastUpdateTime = nowTime;

		m_bIsModify.Set();

		return true;
	}
	return false;
}

void CActivityRecord::PushActivityId(UINT32 dwActivityId)
{
	auto i = m_mapFinishCount.find(dwActivityId);
	if (i == m_mapFinishCount.end())
	{
		m_mapFinishCount[dwActivityId] = 0;
		m_bIsModify.Set();
	}
}

void CActivityRecord::AddActivityIdCount(UINT32 dwActivityId, UINT32 dwValue, bool isComplete)
{
	auto i = m_mapFinishCount.find(dwActivityId);
	if (i != m_mapFinishCount.end())
	{
		i->second++;
		if(isComplete)
		{
			m_dwAllValue += dwValue;
			m_dwWeekValue += dwValue;
			_OnAvtivityAdd(dwActivityId, dwValue);
			m_pRole->OnAddActivityPoint(dwValue);

			EventMgr::Instance()->AddEvent(m_pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_DailyActive , m_dwAllValue, 1, GameTime::GetTime());
		}
		m_bIsModify.Set();
	}
}

/*void CActivityReocrd::UpdateDoubleId()
{
int nRandom = XRandom::randInt(1, m_mapFinishCount.size() + 1);
int j = 1;
for (auto i = m_mapFinishCount.begin(); i != m_mapFinishCount.end(); ++i)
{
if (nRandom <= j)
{
m_dwDoubleId = i->first;
m_setChangedCount.insert(m_dwDoubleId);
m_bIsModify.Set();
return;
}
++j;
}

}

UINT32 CActivityReocrd::GetDoubleId()
{
return m_dwDoubleId;
}

UINT32 CActivityReocrd::GetChestIndex(UINT32 dwIndex)
{
if (eSpecialChest == dwIndex)
{
dwIndex = eSpecialChestMask;
}
else
{
UINT32 i = 0;
while (ACTIVITY_SINGLECHEST_MASK == (ACTIVITY_SINGLECHEST_MASK & (m_dwChestGetInfo >> ACTIVITY_NORMALCHEST_COUNT*i)))
{
++i;
}
dwIndex = ACTIVITY_NORMALCHEST_COUNT*i + dwIndex- 1;
if (dwIndex > eChestMaxMask)
{
dwIndex = eChestMaxMask;
}
}
if ((UINT32)(1 << dwIndex) == (m_dwChestGetInfo & (1 << dwIndex)))
{
return INVALID_ACTIVITYCHESTID;
}
return dwIndex;
}*/

UINT32 CActivityRecord::GetChestGetInfo()
{
	return m_dwChestGetInfo;
}

void CActivityRecord::SetChestGetInfo(UINT32 dwIndex)
{
	m_dwChestGetInfo |= (1 << dwIndex);
	m_bIsModify.Set();
}

bool CActivityRecord::IsRecived(UINT32 dwIndex)
{
	if(m_dwChestGetInfo & (1 << dwIndex)) return true;
	return false;
}

UINT32 CActivityRecord::GetFinishCount(UINT32 dwActivityId)
{	
	auto i = m_mapFinishCount.find(dwActivityId);
	if (i == m_mapFinishCount.end())
	{
		return INVALID_ACTIVITYCOUNT;
	}
	return i->second;
}

UINT32 CActivityRecord::GetActivityAllValue()
{
	CActivityMgr::Instance()->UpdateRecord(this, m_pRole);
	return m_dwAllValue;
}

UINT32 CActivityRecord::GetWeekValue()
{
	CActivityMgr::Instance()->UpdateRecord(this, m_pRole);
	return m_dwWeekValue;
}

void CActivityRecord::_AddGuildActivity(UINT32 addNum)
{
	PtcG2M_SynAddGuildActivity msg;
	msg.m_Data.set_roleid(m_pRole->GetID());
	msg.m_Data.set_guildactivitynum(addNum);
	m_pRole->SendToMS(msg);
}

void CActivityRecord::DoAddGuild()
{
	if (m_dwWeekValue > 0)
	{
		_AddGuildActivity(m_dwWeekValue);
	}
}

void CActivityRecord::_TeamCostGift()
{
	m_pRole->Get<CTeamRecord>()->UpdateCostInfo();
	UINT32 ret = TeamCostConfig::Instance()->GiveReward(m_pRole, m_dwAllValue, m_pRole->Get<CTeamRecord>()->GetTeamCostGiftValue());
	if (0 != ret)
	{
		m_pRole->Get<CTeamRecord>()->SetTeamCostGift(ret);
	}
}

void CActivityRecord::_OnAvtivityAdd(UINT32 activityId, UINT32 value)
{
	_AddGuildActivity(value);
	_TeamCostGift();
	_ActivityCoin(activityId);

	// 活跃度同步MS
	PtcG2M_AddPartnerLivenessG2M ntf;
	ntf.m_Data.set_roleid(m_pRole->GetID());
	ntf.m_Data.set_actid(activityId);
	ntf.m_Data.set_value(value);
	m_pRole->SendToMS(ntf);

	TActiveDegreeFlow oLog(m_pRole);
	oLog.m_ActivityId = activityId;
	oLog.m_Value = value;
	oLog.m_TotalValue = m_dwAllValue;
	oLog.Do();
}


void CActivityRecord::_ActivityCoin(UINT32 activityid)
{
	if (m_pRole->IsBackFlowRole())
	{
		std::vector<ItemDesc> items = CActivityMgr::Instance()->GetActivityCoin(activityid);
		if (!items.empty())
		{
			BagGiveItemTransition give(m_pRole);
			give.SetReason(ItemFlow_Activity, ItemFlow_Activity_Coin);
			for (auto i = items.begin(); i != items.end(); ++i)
			{
				give.GiveItem(i->itemID, i->itemCount);
			}	
			give.NotifyClient();
		}
	}
}


bool CActivityRecord::CheckHaveRandomActivityId(UINT32 activity_id)
{
	UINT32 new_activity_id =   CActivityMgr::Instance()->ChangeActivityID(activity_id,1);
	for (auto it = m_mapFinishCount.begin(); it != m_mapFinishCount.end(); ++it)
	{
		if (it->first == new_activity_id)
		{
			return true;
		}
	}

	return false;
}


bool CActivityRecord::IsGetRandomActivityId(UINT32 activity_id)
{
	auto it = m_mapFinishCount.find(activity_id);
	if (it != m_mapFinishCount.end())
	{
		return true;
	}
	return false;
}


void  CActivityRecord::TestAddVaule(UINT32 value)
{
	m_dwAllValue += value;
	m_dwWeekValue += value;
	m_pRole->OnAddActivityPoint(value);

	m_bIsModify.Set();
}