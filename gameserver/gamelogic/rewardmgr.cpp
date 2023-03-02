#include "pch.h"
#include "rewardmgr.h"
#include "unit/role.h"
#include "rewardrecord.h"
#include "unit/rolemanager.h"
#include "item.h"
#include "reward/ptcg2d_queryreward.h"
#include "reward/ptcg2d_addreward.h"
#include "reward/ptcg2d_removereward.h"
#include "reward/ptcg2c_rewardchangedntf.h"
#include "dblink.h"
#include "bagtransition.h"
#include "time.h"

COnceRewardMgr::COnceRewardMgr()
{

}

COnceRewardMgr::~COnceRewardMgr()
{

}

void COnceRewardMgr::ClearReward(UINT64 qwRoleId)
{
	auto i = m_mapRole2Rewards.find(qwRoleId);
	if (i != m_mapRole2Rewards.end())
	{
		m_mapRole2Rewards.erase(i);
	}
}

void COnceRewardMgr::PushReward(UINT64 qwRoleId, const SRewardInfo& stRewardInfo, bool bIsSaveToDb)
{
	m_mapRole2Rewards[qwRoleId][stRewardInfo.qwUniqueId] = stRewardInfo;
	if (bIsSaveToDb)
	{
		PtcG2D_AddReward stPtc;
		stPtc.m_Data.set_roleid(qwRoleId);
		stPtc.m_Data.set_uniqueid(stRewardInfo.qwUniqueId);

		KKSG::RewardInfo RewardInfo;
		stRewardInfo.RewardInfoToKKSG(&RewardInfo);
		*stPtc.m_Data.mutable_rewardinfo() = RewardInfo.SerializeAsString();

		DBLink::Instance()->Send(stPtc);

		AddToClient(qwRoleId, stRewardInfo);
	}
}

bool COnceRewardMgr::PopReward(UINT64 qwRoleId, UINT64 qwUniqueId)
{
	auto i = m_mapRole2Rewards.find(qwRoleId);
	if (i == m_mapRole2Rewards.end())
	{
		return false;
	}
	auto j = i->second.find(qwUniqueId);
	if (j == i->second.end())
	{
		return false;
	}
	std::vector<ItemDesc> vecItem = j->second.vecItem;
	i->second.erase(j);

	PtcG2D_RemoveReward stPtc;
	stPtc.m_Data.set_roleid(qwRoleId);
	stPtc.m_Data.set_uniqueid(qwUniqueId);
	DBLink::Instance()->Send(stPtc);

	Role* pRole = RoleManager::Instance()->FindByRoleID(qwRoleId);
	if (NULL == pRole)
	{
		return false;
	}
	RemoveToClient(pRole, qwUniqueId);
	///> 给奖励
	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_Reward, ItemFlow_Reward_System);
	for (auto k = vecItem.begin(); k != vecItem.end(); ++k)
	{
		stTransition.GiveItem(*k);
	}
	stTransition.NotifyClient();
	return true;
}

void COnceRewardMgr::RemoveToClient(Role* pRole, UINT64 qwUniqueId)
{
	if (NULL == pRole)
	{
		return;
	}
	PtcG2C_RewardChangedNtf stNtf;
	stNtf.m_Data.add_removedrewarduniqueid(qwUniqueId);
	pRole->Send(stNtf);
}

void COnceRewardMgr::AddToClient(UINT64 qwRoleId, const SRewardInfo& stRewardInfo)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(qwRoleId);
	if (NULL == pRole)
	{
		return;
	}
	PtcG2C_RewardChangedNtf stNtf;
	KKSG::RewardInfo* pSendInfo = stNtf.m_Data.add_addedrewardinfo();
	stRewardInfo.RewardInfoToKKSG(pSendInfo);	
	pRole->Send(stNtf);
}

void COnceRewardMgr::SendAllToClient(UINT64 qwRoleId)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(qwRoleId);
	if (NULL == pRole)
	{
		return;
	}
	auto i = m_mapRole2Rewards.find(qwRoleId);
	if (i == m_mapRole2Rewards.end())
	{
		return;
	}
	PtcG2C_RewardChangedNtf stNtf;
	for (auto j = i->second.begin(); j != i->second.end(); ++j)
	{
		KKSG::RewardInfo* pSendInfo = stNtf.m_Data.add_addedrewardinfo();
		(j->second).RewardInfoToKKSG(pSendInfo);
	}
	pRole->Send(stNtf);
}

/////////////////////////////////////////////////////////////////////////////////
INSTANCE_SINGLETON(CRewardMgr);

CRewardMgr::CRewardMgr()
:m_oDefaultHanlder(0)
{

}

CRewardMgr::~CRewardMgr()
{
	for (auto i = m_mapRewardHandler.begin(); i != m_mapRewardHandler.end(); ++i)
	{
		if (NULL != i->second)
		{
			delete i->second;
			i->second = NULL;
		}
	}
	m_mapRewardHandler.clear();
}

bool CRewardMgr::Init()
{
	if (!m_oTable.LoadFile("table/SystemReward.txt"))
	{
		SSWarn<<"load file table/SystemReward.txt failed"<<END;
		return false;
	}

	m_mapRewardHandler[KKSG::RewardDinner] = new CDinnerRewardHandler(KKSG::RewardDinner);
	m_mapRewardHandler[KKSG::RewardSupper] = new CSupperRewardHandler(KKSG::RewardSupper);

	return true;
}

void CRewardMgr::Uninit()
{
}

void CRewardMgr::OnRoleLogin(Role* pRole)
{
	CRewardRecord* pRecord = pRole->Get<CRewardRecord>();
	if (NULL == pRecord)
	{
		return;
	}
	for (auto i = m_mapRewardHandler.begin(); i != m_mapRewardHandler.end(); ++i)
	{
		if (NULL == i->second)
		{
			continue;
		}
		///> 初始化
		pRecord->Init(i->first, GetSubType(i->first));
		PushReward(pRole, i->first, false);
	}
	///> 上线通知玩家所有系统奖励信息
	pRecord->SendAllRewardInfo();
	
	///> 查询一次性奖励信息
	//PtcG2D_QueryReward oPtc;
	//oPtc.m_Data.set_roleid(pRole->GetID());
	//DBLink::Instance()->Send(oPtc);
}

//void CRewardMgr::ReceiveFromDb(UINT64 qwRoleId, const SRewardInfo& stRewardInfo)
//{
//	m_oOnceRewardMgr.PushReward(qwRoleId, stRewardInfo);
//}

//bool CRewardMgr::RemoveFromDb(UINT64 qwRoleId, UINT64 qwUniqueId)
//{
//	return m_oOnceRewardMgr.PopReward(qwRoleId, qwUniqueId);
//}

//void CRewardMgr::ClearOnceReward(UINT64 qwRoleId)
//{
//	m_oOnceRewardMgr.ClearReward(qwRoleId);
//}

//void CRewardMgr::SendOnceReward(UINT64 qwRoleId)
//{
//	m_oOnceRewardMgr.SendAllToClient(qwRoleId);
//}

void CRewardMgr::PushReward(UINT64 qwRoleId, UINT32 dwType, bool bIsNotifyClient)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(qwRoleId);
	if (NULL == pRole)
	{
		return;
	}
	PushReward(pRole, dwType, bIsNotifyClient);
}

void CRewardMgr::PushReward(Role* pRole, UINT32 dwType, bool bIsNotifyClient)
{
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL"<<END;
		return;
	}
	CRewardRecord* pRecord = pRole->Get<CRewardRecord>();
	if (NULL == pRecord)
	{
		SSWarn<<"pRewardRecord is NULL, role id:"<<pRole->GetID()<<END;
		return;
	}
	CRewardHandler* pHandler = GetHandler(dwType);
	if (NULL == pHandler)
	{
		pHandler = &m_oDefaultHanlder;
	}
	///> 判断记录是否需要更新
	if (!pHandler->Update(pRecord, dwType))
	{
		return;
	}
	///> 更新记录
	SRewardInfo stRewardInfo;
	stRewardInfo.dwType = dwType;
	stRewardInfo.dwSubType = GetSubType(dwType);
	if (!pHandler->GetReward(pRole, stRewardInfo))
	{
		return;
	}
	stRewardInfo.dwState = eCanGet;
	if (eCanCover == stRewardInfo.dwSubType)
	{
		pRecord->Push(stRewardInfo, bIsNotifyClient);
	}
	else if (eCanNotCover )
	{
		m_oOnceRewardMgr.PushReward(pRole->GetID(), stRewardInfo, true);
	}
	return;
}

//void CRewardMgr::PushOnceReward(UINT64 qwRoleId, UINT32 dwType, const std::vector<std::string>& vecParam
//	, const std::vector<ItemDesc>& vecItem)
//{
//	SRewardInfo stRewardInfo;
//	stRewardInfo.dwType = dwType;
//	stRewardInfo.dwSubType = GetSubType(dwType);
//	if (eCanNotCover != stRewardInfo.dwSubType)
//	{
//		SSWarn<<"push reward type:"<<stRewardInfo.dwType<<END;
//		return;
//	}
//	stRewardInfo.vecParam = vecParam;
//	stRewardInfo.vecItem = vecItem;
//	stRewardInfo.dwState = eCanGet;
//	m_oOnceRewardMgr.PushReward(qwRoleId, stRewardInfo, true);
//}

CRewardHandler* CRewardMgr::GetHandler(UINT32 dwType)
{
	auto i = m_mapRewardHandler.find(dwType);
	if (i == m_mapRewardHandler.end())
	{
		return NULL;
	}
	return i->second;
}

UINT32 CRewardMgr::GetSubType(UINT32 dwType)
{
	auto i = m_oTable.GetByType(dwType);
	if (NULL == i)
	{
		SSWarn<<"can not find sub type, type:"<<dwType<<END;
		return 0;
	}
	return i->SubType;
}

std::vector<ItemDesc> CRewardMgr::GetDefaultReward(UINT32 dwType)
{
	std::vector<ItemDesc> vecItem;
	auto i = m_oTable.GetByType(dwType);
	if (NULL == i)
	{
		return vecItem;
	}
	for (auto j = i->Reward.begin(); j != i->Reward.end(); ++j)
	{
		vecItem.push_back(ItemDesc((int)j->seq[0], (int)j->seq[1]));
	}
	return vecItem;
}

void CRewardMgr::Dispatch(UINT32 type)
{
	auto i = m_mapRewardHandler.find(type);
	if (i == m_mapRewardHandler.end())
	{
		return;
	}
	i->second->OnTimer(type);
}
