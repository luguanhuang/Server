#include "pch.h"
#include "activitymgr.h"
#include "activityrecord.h"
#include "itemconfig.h"
#include "item.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "util/timespecificmgr.h"
#include "activity/ptcg2c_activityrolenotify.h"
#include "define/systemiddef.h"
#include "util/gametime.h"
#include "config/itemdropconfig.h"
#include "payv2Record.h"
#include "payv2Mgr.h"
#include "scene/sceneteam.h"
#include "gamelogic/levelsealMgr.h"

INSTANCE_SINGLETON(CActivityMgr);

UINT32 CActivityMgr::activityChestNum = 5;

CActivityMgr::CActivityMgr()
{
	m_handler = INVALID_TIMESPECIFIC;
	Reset();
}

CActivityMgr::~CActivityMgr()
{

}

void CActivityMgr::Reset()
{
	//m_dwActivityValueScale = 0;
	m_oTable.Clear();
	m_oChestTable.Clear();
	m_systemid2index.clear();
}

bool CActivityMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_handler =  CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CActivityMgr::OnTimer, this));
	//Test();

	return true;
}

void CActivityMgr::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_handler);
}

bool CActivityMgr::LoadFile()
{
	if (!m_oTable.LoadFile("table/Activity.txt"))
	{
		SSWarn<<"load file table/Activity.txt failed"<<END;
		return false;
	}
	if (!m_oChestTable.LoadFile("table/ActivityChest.txt"))
	{
		SSWarn<<"load file table/ActivityChest.txt failed"<<END;
		return false;
	}
	activityChestNum =  GetGlobalConfig().ActivityChestExp.size();
	for(UINT32 i = 0; i < m_oTable.Table.size(); i++)
	{
		m_systemid2index[m_oTable.Table[i]->id] = i+1;
	}
	/*for (auto i = m_oChestTable.Table.begin(); i != m_oChestTable.Table.end(); ++i)
	{
	if (ACTIVITY_NORMALCHEST_COUNT == (*i)->chest[0])
	{
	m_dwActivityValueScale = (*i)->chest[1];
	break;
	}
	}*/
	return true;
}

bool CActivityMgr::Reload()
{
	Reset();
	return LoadFile();
}

void CActivityMgr::UpdateRecord(CActivityRecord* pRecord, Role* pRole)
{
	if (pRecord->Update())
	{
		UpdateActiviyId(pRecord, pRole);
	}
}

bool CActivityMgr::OnTimer()
{
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* role = *i;
		role->HintNotify(ActivityRewardID, true);
		CheckFinishPayMember(role);
		//SendActivityInfo(role, true);
	}
	return true;
}

/*void CActivityMgr::SendActivityInfo(Role* pRole, bool bIsPackAll)
{
if (NULL == pRole)
{
return;
}
CActivityReocrd* pRecord = pRole->Get<CActivityReocrd>();
if (NULL == pRecord)
{
return;
}
UpdateRecord(pRecord, pRole);

PtcG2C_ActivityRoleNotify stNotify;
KKSG::ActivityRecord* pRecoreSend = stNotify.m_Data.mutable_activityrecord();
if (bIsPackAll)
{
pRecord->RecordToKKSG(pRecoreSend);	
}
else
{
pRecord->RecordChangedToKKSG(pRecoreSend);
}
for (int i = 0; i < pRecoreSend->activityid_size(); ++i)
{
UINT32 dwConfigCount = GetConfigFinishCount(pRecoreSend->activityid(i));
if (UINT32(-1) == dwConfigCount)
{
dwConfigCount = 0;
}
pRecoreSend->add_needfinishcount(dwConfigCount);
}
pRole->Send(stNotify);
}*/

void CActivityMgr::UpdateActiviyId(CActivityRecord* pRecord, Role* pRole)
{
	if (NULL == pRole || NULL == pRecord)
	{
		return;
	}
	pRecord->Update();

	std::vector<UINT32> random_activityid;
	random_activityid.clear();

	UINT32 weekDay = XCommon::GetWeekNum();
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
		if (NULL != (*i) && pRole->Get<CRoleSystem>()->IsSystemOpened((*i)->id) && (UINT32)pRole->GetLevel() >= (*i)-> level[0] && (UINT32)pRole->GetLevel() <= (*i)->level[1])
		{
			for(auto j = (*i)->weekday.begin(); j != (*i)->weekday.end(); j++)
			{
				if((*j) == weekDay)
				{
					if ((*i)->random == 1)
					{
						random_activityid.push_back((*i)->sortid);
						continue;
					}
					pRecord->PushActivityId((*i)->sortid);
					break;
				}
			}
		}
	}

	// 随机的任务
	if (random_activityid.size() > 0)
	{
		// 因为会有其他情况触发重新加载，所以需要先清理掉已有的随机任务
		for (auto it = random_activityid.begin(); it != random_activityid.end(); ++it)
		{
			if (pRecord->IsGetRandomActivityId(*it))
			{
				LogDebug("exist random actid : %d\n",*it);
				return;
			}
		}

		UINT32 random_id = XRandom::randInt(0,random_activityid.size());
		LogDebug("random_id : %d and active_size is %d \n",random_id,random_activityid.size());
		pRecord->PushActivityId(random_activityid[random_id]);

		LogDebug("this random actid : %d\n",random_activityid[random_id]);
	}
}

void CActivityMgr::OnFinishActivityId(CActivityRecord* pRecord, Role* pRole, UINT32 dwActivityId,bool israndom)
{
	UINT32 randomvalue = 0;
	if (israndom)
	{
		randomvalue = 1;
	}
	ActivityTable::RowData* pRowData = NULL; //m_oTable.GetByid(dwActivityId);
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); ++it)
	{
		if ((*it)->id == dwActivityId && (*it)->random == randomvalue)
		{
			pRowData = *it;
		}
	}
	if (NULL == pRowData || NULL == pRecord || NULL == pRole)
	{
		return;
	}
	UpdateRecord(pRecord, pRole);
	UINT32 dwCount = pRecord->GetFinishCount(pRowData->sortid);
	if (dwCount >= pRowData->count)
	{
		return;
	}
	bool isComplete = dwCount + 1 == pRowData->count ? true : false;

	pRecord->AddActivityIdCount(pRowData->sortid, pRowData->value, isComplete);
	if (isComplete) //活跃度增加
	{
		LogInfo("role[%llu] finish actid: %u", pRole->GetID(), pRowData->sortid);
		HintNotify(pRole);
	}
}

/*UINT32 CActivityMgr::GetChestIndex(CActivityReocrd* pRecord, UINT32 dwIndex)
{
if (dwIndex >= eChestDummy)
{
return INVALID_ACTIVITYCHESTID;
}
if (NULL == pRecord)
{
return INVALID_ACTIVITYCHESTID;
}
return pRecord->GetChestIndex(dwIndex);
}

bool CActivityMgr::IsFinishActivity(CActivityReocrd* pRecord, UINT32 dwActivityId)
{
if (NULL == pRecord)
{
return false;
}
UINT32 dwConfigCount = GetConfigFinishCount(dwActivityId);
UINT32 dwCount = pRecord->GetFinishCount(dwActivityId);
if (dwCount < dwConfigCount)
{
return false;
}
return true;
}*/

bool CActivityMgr::GetChestItem(UINT32 dwAllValue, UINT32 dwLevel, UINT32 dwIndex, std::vector<ItemDesc>& vecItem)
{
	std::vector<UINT32> vecDropId = GetChestDropId(dwIndex, dwLevel, dwAllValue);
	if (vecDropId.empty())
	{
		return false;
	}
	for (auto i = vecDropId.begin(); i != vecDropId.end(); ++i)
	{
		const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(*i, dwLevel);
		if (NULL == pDrop)
		{
			continue;
		}
		ItemDesc stItem(*pDrop);
		vecItem.push_back(stItem);
	}
	return true;
}

std::vector<UINT32> CActivityMgr::GetChestDropId(UINT32 dwIndex, UINT32 dwLevel, UINT32 dwAllValue)
{
	/*UINT32 dwCoef = 0;
	if (eSpecialChestMask == dwIndex)
	{
	dwIndex = 0;
	}
	else
	{
	dwCoef = UINT32(dwIndex/ACTIVITY_NORMALCHEST_COUNT);
	dwIndex = (dwIndex % ACTIVITY_NORMALCHEST_COUNT) + 1;
	}*/
	if(GetGlobalConfig().ActivityChestExp.size() < dwIndex + 1)
	{
		LogWarn("ActivityChestExp' size in GlobalConfig is too small");
		return std::vector<UINT32>();
	}
	if(dwAllValue < GetGlobalConfig().ActivityChestExp[dwIndex])
	{
		return std::vector<UINT32>();
	}
	UINT32 levelseal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;

	for (auto i = m_oChestTable.Table.begin(); i != m_oChestTable.Table.end(); ++i)
	{
		if (dwIndex + 1 != (*i)->chest)
		{
			continue;
		}
		if (dwLevel >= (*i)->level[0] && dwLevel <= (*i)->level[1])
		{
			return (*i)->dropid;
		}
		/*if (0 == dwIndex || dwAllValue >= ((dwCoef * m_dwActivityValueScale)+ (*i)->chest[1]))
		{
		if (dwLevel >= (*i)->level[0] && dwLevel <= (*i)->level[1])
		{
		return (*i)->dropid;
		}	
		}*/
	}
	return std::vector<UINT32>();
}

UINT32 CActivityMgr::GetConfigFinishCount(UINT32 dwActivityId)
{
	ActivityTable::RowData* pRowData = NULL;
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); ++it)
	{
		if ((*it)->sortid == dwActivityId )
		{
			pRowData = *it;
		}
	}

	if (NULL == pRowData)
	{
		return (UINT32)(-1);
	}
	return pRowData->count;
}

UINT32 CActivityMgr::GetIndexID(UINT32 systemid)
{
	if(m_systemid2index.find(systemid) == m_systemid2index.end())
	{
		return 0;
	}
	return m_systemid2index[systemid];
}

void CActivityMgr::HintNotify(Role* pRole)
{
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	UpdateRecord(pRecord, pRole);
	UINT32 allValue = pRecord->GetActivityAllValue();
	UINT32 getChestInfo = pRecord->GetChestGetInfo();
	bool flag = false;
	for(UINT32 i = 0; i < GetGlobalConfig().ActivityChestExp.size(); i++)
	{
		if(allValue < GetGlobalConfig().ActivityChestExp[i]) break;
		if((getChestInfo & (1 << i)) == 0)
		{
			flag = true;
			break;
		}
	}
	if(flag)
	{
		pRole->HintNotify(ActivityRewardID, false);
	}
}

void CActivityMgr::OnRoleFirstEnterScene(Role* pRole)
{
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	UpdateRecord(pRecord, pRole);

	CheckFinishPayMember(pRole);
}

void CActivityMgr::CheckFinishPayMember(Role* pRole)
{
	CPayV2Record* pPayRecord = pRole->Get<CPayV2Record>();
	if(pPayRecord->GetExpireTimeByID(pay_member_3) > 0)
	{
		CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
		OnFinishActivityId(pRecord, pRole, eActivityPayMember);
	}
}

std::vector<ItemDesc> CActivityMgr::GetActivityCoin(UINT32 activityid)
{
	std::vector<ItemDesc> items;
	ActivityTable::RowData* pRowData = NULL; 
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); ++it)
	{
		if ((*it)->sortid == activityid )
		{
			pRowData = *it;
		}
	}
	if (NULL != pRowData)
	{
		for (auto i = pRowData->item.begin(); i != pRowData->item.end(); ++i)
		{
			ItemDesc item;
			item.itemID = i->seq[0];
			item.itemCount = i->seq[1];
			items.push_back(item);
		}
	}
	return items;
}

bool CActivityMgr::CheckPlayerGuileRelation(Role* role,Scene* scene)
{
	if (role->getGuildId() > 0)
	{
		// 有没有工会成员？
		auto sene_team = scene->GetSceneTeam();
		if (sene_team != NULL)
		{
			auto all_memberids =sene_team->GetAllMemberIDs();
			for (auto it = all_memberids.begin(); it != all_memberids.end(); ++it)
			{
				if (*it == role->GetRoleID())
				{
					continue;
				}

				Role *other_role = RoleManager::Instance()->FindByRoleID(*it);
				if (other_role == NULL)
				{
					continue;
				}
				if (role->getGuildId() == other_role->getGuildId())
				{
					return  true;
				}
			}
		}
	}	

	return false;
}

UINT32 CActivityMgr::ChangeActivityID(UINT32 old_activity_id,UINT32 random )
{
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); ++it)
	{
		if ((*it)->id == old_activity_id && (*it)->random == random)
		{
			return (*it)->sortid;
		}
	}
	return old_activity_id;
}

bool CActivityMgr::CheckRandomActivityID(UINT32 activity_id)
{
	for (auto it = m_oTable.Table.begin(); it != m_oTable.Table.end(); ++it)
	{
		if ((*it)->id == activity_id)
		{
			if ((*it)->random == 1)
			{
				return true;
			}
		}
	}

	return false;
}

void CActivityMgr::Test()
{
	// init record
	/*CActivityReocrd oRecord;
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
	oRecord.PushActivityId((*i)->id);
	}
	// add activity
	for (auto i = m_oTable.Table.begin(); i != m_oTable.Table.end(); ++i)
	{
	for (int j = 0; j < 100; ++j)
	{
	OnFinishActivityId(&oRecord, NULL, (*i)->id);
	if (IsFinishActivity(&oRecord, (*i)->id))
	{
	SSWarn<<"has finish this activity, activity id:"<<(*i)->id<<END;
	}
	SSWarn<<"all value:"<<oRecord.GetActivityAllValue()<<END;
	}
	}
	// double id
	for (int i = 0; i < 100; ++i)
	{
	oRecord.UpdateDoubleId();
	SSWarn<<"double id:"<<oRecord.GetDoubleId()<<END;
	}

	// test chest
	for (int i = 0; i < 100; ++i)
	{
	UINT32 dwIndex = GetChestIndex(&oRecord, i%4);
	SSWarn<<"i:"<<i<<" get chest index:"<<dwIndex<<END;
	if ((UINT32)(-1) == dwIndex)
	{
	continue;
	}
	for (int j = 0; j < 100; j += 3)
	{
	SSWarn<<"level:"<<j<<END;
	std::vector<ItemDesc> vecItem;
	GetChestItem(oRecord.GetActivityAllValue(), j, dwIndex, vecItem);
	for (auto k = vecItem.begin(); k != vecItem.end(); ++k)
	{
	SSWarn<<"item id:"<<k->ItemID<<" item count:"<<k->ItemCount<<END;
	}
	}
	oRecord.SetChestGetInfo(dwIndex);
	}*/

}