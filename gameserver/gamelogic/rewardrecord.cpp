#include "pch.h"
#include "time.h"
#include "rewardrecord.h"
#include "pb/project.pb.h"
#include "unit/role.h"
#include "utility/uidhelper.h"
#include "util/gametime.h"
#include "reward/ptcg2c_rewardchangedntf.h"

SRewardInfo::SRewardInfo()
{
	dwType = 0;
	dwSubType = 0;
	isget = false;
	uniqueday = 0;
	dwState = eCanNotGet;
	qwUniqueId = UIDHelper::CreateUID(UID_Reward);
	dwTimeStamp = (UINT32)time(NULL);
}

void SRewardInfo::RewardInfoToKKSG(KKSG::RewardInfo* pInfo) const
{
	pInfo->set_uniqueid(qwUniqueId);
	pInfo->set_type(dwType);
	pInfo->set_subtype(dwSubType);
	pInfo->set_state(isget ? eHasGet : dwState);
	pInfo->set_timestamp(dwTimeStamp);
	pInfo->set_uniqueday(uniqueday);
	pInfo->set_isget(isget);
	for (auto i = vecParam.begin(); i != vecParam.end(); ++i)
	{
		pInfo->add_param(i->c_str());
	}
	for (auto j = vecItem.begin(); j != vecItem.end(); ++j)
	{
		KKSG::ItemBrief* pItem = pInfo->add_item();
		pItem->set_itemid(j->itemID);
		pItem->set_itemcount(j->itemCount);
		pItem->set_isbind(j->isbind);
	}
}

void SRewardInfo::KKSGToRewardInfo(const KKSG::RewardInfo* pInfo)
{
	qwUniqueId = pInfo->uniqueid();
	dwType = pInfo->type();
	dwSubType = pInfo->subtype();
	dwState = pInfo->state();
	dwTimeStamp = pInfo->timestamp();
	uniqueday = pInfo->uniqueday();
	isget = pInfo->isget();
	for (int i = 0; i < pInfo->param_size(); ++i)
	{
		vecParam.push_back(pInfo->param(i));
	}
	for (int j = 0; j < pInfo->item_size(); ++j)
	{
		ItemDesc stItem;
		stItem.ToThis(pInfo->item(j));
		vecItem.push_back(stItem);
	}
}

void SRewardInfo::Reset()
{
	dwTimeStamp = 0;
	dwState = eCanNotGet;
	vecParam.clear();
	vecItem.clear();
}

void SRewardInfo::SetDay(const SRewardInfo& info)
{
	isget = info.isget;
	uniqueday = info.uniqueday;
}

bool SRewardInfo::ResetDay()
{
	UINT32 tempunique = GameTime::GetTodayUniqueTag();
	if (tempunique != uniqueday)
	{
		uniqueday = tempunique;
		isget = false;
		return true;
	}
	return false;
}

bool SRewardInfo::Update(int nHour, int nMin, bool& isdayreset)
{
	time_t dwNowTime = time(NULL);
	struct tm* pstTime = localtime(&dwNowTime);
	///> 计算当天应该更新的时间
	pstTime->tm_hour = nHour;
	pstTime->tm_min = nMin;
	UINT32 dwUpdateTime = (UINT32)mktime(pstTime);

	isdayreset = ResetDay();

	if (dwUpdateTime <= (UINT32)dwNowTime)
	{
		if (dwTimeStamp <= dwUpdateTime)
		{
			Reset();
			return true;
		}
	}
	else
	{
		if ((dwTimeStamp + 24*3600) <= dwUpdateTime)
		{
			Reset();
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////
CRewardRecord::CRewardRecord(Role* pRole)
:m_pRole(pRole)
{
	m_nextdayreward = 0;
}

CRewardRecord::~CRewardRecord()
{

}

bool CRewardRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_rewardrecord())
	{
		const KKSG::RewardRecord* pRecord = &poRoleAllInfo->rewardrecord();
		for (int i = 0; i < pRecord->rewardinfo_size(); ++i)
		{
			SRewardInfo stInfo;
			stInfo.KKSGToRewardInfo(&pRecord->rewardinfo(i));
			m_mapRewardInfo[stInfo.qwUniqueId] = stInfo;
		}
		m_isgiven.FromString(pRecord->given());
		m_istaken.FromString(pRecord->taken());
		m_nextdayreward = pRecord->nextdayreward();
		for (int i = 0; i < pRecord->onlinereward_size(); ++i)
		{
			m_onlinereward.insert(pRecord->onlinereward(i));
		}
	}
	return true;
}

void CRewardRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_bIsModify.TestAndReset())
	{
		poRoleAllInfo->clear_rewardrecord();
		KKSG::RewardRecord* pRecord = poRoleAllInfo->mutable_rewardrecord();
		for (auto i = m_mapRewardInfo.begin(); i != m_mapRewardInfo.end(); ++i)
		{
			KKSG::RewardInfo* pInfo = pRecord->add_rewardinfo();
			i->second.RewardInfoToKKSG(pInfo);
		}
		pRecord->set_given(m_isgiven.ToString());
		pRecord->set_taken(m_istaken.ToString());
		pRecord->set_nextdayreward(m_nextdayreward);
		for (auto i = m_onlinereward.begin(); i != m_onlinereward.end(); ++i)
		{
			pRecord->add_onlinereward(*i);
		}

		roChanged.insert(pRecord);
	}
}

bool CRewardRecord::Init(UINT32 dwType, UINT32 dwSubType)
{
	if (eCanCover != dwSubType)
	{
		return false;
	}
	SRewardInfo* pInfo = GetRewardInfobyId((UINT64)dwType);
	if (NULL == pInfo)
	{
		SRewardInfo stInfo;
		stInfo.dwType = dwType;
		stInfo.dwSubType = dwSubType;
		stInfo.qwUniqueId = (UINT64)dwType;
		stInfo.dwState = eCanNotGet;
		stInfo.dwTimeStamp = 0;
		m_mapRewardInfo[stInfo.qwUniqueId] = stInfo;
		m_bIsModify.Set();
		return true;
	}
	return false;
}

void CRewardRecord::Push(SRewardInfo& stRewardInfo, bool bIsNotfyClient)
{
	if (stRewardInfo.dwSubType != eCanCover)
	{
		return;
	}
	///> 如果是每日重置类型可覆盖类型，type设为uniqueid
	stRewardInfo.qwUniqueId = (UINT64)stRewardInfo.dwType;
	SRewardInfo* pInfo = GetRewardInfobyId(stRewardInfo.qwUniqueId);
	if (NULL == pInfo)
	{
		m_mapRewardInfo[stRewardInfo.qwUniqueId] = stRewardInfo;
	}
	else
	{
		stRewardInfo.SetDay(*pInfo);
		*pInfo = stRewardInfo;			
	}
	if (bIsNotfyClient)
	{
		PtcG2C_RewardChangedNtf stNtf;
		KKSG::RewardInfo* pInfo = stNtf.m_Data.add_addedrewardinfo();
		stRewardInfo.RewardInfoToKKSG(pInfo);
		m_pRole->Send(stNtf);
	}
	m_bIsModify.Set();
}

bool CRewardRecord::Pop(UINT64 qwUniqueId)
{
	SRewardInfo* pInfo = GetRewardInfobyId(qwUniqueId);
	if (NULL == pInfo)
	{
		return false;
	}
	if (eCanGet != pInfo->dwState || eCanCover != pInfo->dwSubType)
	{
		return false;
	}
	PtcG2C_RewardChangedNtf stNtf;

	pInfo->dwState = eHasGet;
	pInfo->isget = true;
	pInfo->vecItem.clear();
	pInfo->vecParam.clear();
	KKSG::RewardInfo* pSendInfo = stNtf.m_Data.add_addedrewardinfo();
	pInfo->RewardInfoToKKSG(pSendInfo);

	m_pRole->Send(stNtf);

	m_bIsModify.Set();
	return true;
}

bool CRewardRecord::Update(UINT32 dwType, int nHour, int nMin)
{
	SRewardInfo* pInfo = GetRewardInfobyId(dwType);
	if (NULL == pInfo)
	{
		return false;
	}
	bool isdayreset = false;
	if (!pInfo->Update(nHour, nMin, isdayreset))
	{
		return false;
	}
	if (isdayreset)
	{
		m_bIsModify.Set();
	}

	PtcG2C_RewardChangedNtf stNtf;
	KKSG::RewardInfo* pSendInfo = stNtf.m_Data.add_addedrewardinfo();
	pInfo->RewardInfoToKKSG(pSendInfo);
	m_pRole->Send(stNtf);

	m_bIsModify.Set();
	return true;
}

SRewardInfo* CRewardRecord::GetRewardInfobyId(UINT64 qwUniqueId)
{
	auto i = m_mapRewardInfo.find(qwUniqueId);
	if (i == m_mapRewardInfo.end())
	{
		return NULL;
	}
	return &(i->second);
}

void CRewardRecord::SendAllRewardInfo()
{
	PtcG2C_RewardChangedNtf stNtf;
	for (auto i = m_mapRewardInfo.begin(); i != m_mapRewardInfo.end(); ++i)
	{
		KKSG::RewardInfo* pInfo = stNtf.m_Data.add_addedrewardinfo();
		i->second.RewardInfoToKKSG(pInfo);
	}
	m_pRole->Send(stNtf);
}

void CRewardRecord::SetGiven(UINT64 id)
{
	m_isgiven.Set((UINT32)id, true);
	m_bIsModify.Set();
}

void CRewardRecord::SetTaken(UINT64 id)
{
	m_istaken.Set((UINT32)id, true);
	m_bIsModify.Set();
}

bool CRewardRecord::IsGiven(UINT64 id) const
{
	return m_isgiven.check((UINT32)id);
}

bool CRewardRecord::IsTaken(UINT64 id) const
{
	return m_istaken.check((UINT32)id);	
}

bool CRewardRecord::IsTakenOnline(UINT32 index)
{
	auto i = m_onlinereward.find(index);
	return i != m_onlinereward.end();
}

void CRewardRecord::SetNextDay()
{
	m_nextdayreward = eHasGet;
	m_bIsModify.Set();
}

void CRewardRecord::SetOnline(UINT32 index)
{
	m_onlinereward.insert(index);
	m_bIsModify.Set();
}