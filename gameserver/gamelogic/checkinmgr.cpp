#include "pch.h"
#include "checkinmgr.h"
#include "time.h"
#include "item.h"
#include "unit/role.h"
#include "checkinrecord.h"
#include "checkin/ptcg2c_checkininfonotify.h"
#include "bagtransition.h"
#include "util/XRandom.h"
#include "globalconfig.h"
#include "takecostmgr.h"
#include "unit/rolemanager.h"
#include "util/timespecificmgr.h"
#include "guild/guildmgr.h"
#include "util/gametime.h"
#include "payv2Record.h"
#include "qqviprecord.h"

INSTANCE_SINGLETON(CCheckinMgr);

CCheckinMgr::CCheckinMgr()
{
	Reset();
}

CCheckinMgr::~CCheckinMgr()
{

}

bool CCheckinMgr::Init()
{
	if (!LoadFile())
	{
		return false;
	}
	TimeSpecific time(HOUR_ONE_DAY_BEGIN, 0, 0);
	m_timer = CTimeSpecificMgr::Instance()->RegisterEvent(time, std::bind(&CCheckinMgr::OnTimer, this));
	//Test();

	return true;
}

void CCheckinMgr::Uninit()
{
	CTimeSpecificMgr::Instance()->UnRegisterEvent(m_timer);
}

void CCheckinMgr::Reset()
{
	m_dwCurrentMonth = (UINT32)(-1);
	m_oTable.Clear();
	m_oNotify.Clear();
}

bool CCheckinMgr::LoadFile()
{
	if (!m_oTable.LoadFile("table/checkin.txt"))
	{
		SSWarn<<"load file table/checkin.txt failed"<<END;
		return false;
	}
	UpdateCheckinItems();
	return true;
}

bool CCheckinMgr::Reload()
{
	Reset();
	return LoadFile();
}

bool CCheckinMgr::OnTimer()
{
	for (auto i = RoleManager::Instance()->Begin(); i != RoleManager::Instance()->End(); ++i)
	{
		Role* role = *i;
		SendCheckinInfo(role);
	}
	return true;
}

void CCheckinMgr::SendCheckinInfo(Role* pRole)
{
	if (NULL == pRole)
	{
		return;
	}
	CCheckinRecord *pRecord = pRole->Get<CCheckinRecord>();
	if (NULL == pRecord)
	{
		return;
	}
	pRecord->Update();
	UpdateCheckinItems();
	
	m_oNotify.set_daycheckinfo(pRecord->GetCheckInfo());
	m_oNotify.set_daymakeup(pRecord->GetCheckinCount());
	time_t dwNowTime = time(NULL) - SECONDS_OFFSET;
	tm* stNowTime = localtime(&dwNowTime);
	CanCheckinInfo stInfo = GetCanCheckinInfo(pRole, dwNowTime);
	m_oNotify.set_daycancheck(stInfo.dwCanCheckDay);
	m_oNotify.set_startday(stInfo.dwStartDay);
	m_oNotify.set_isoddmonth(IsOddMonth());

	PtcG2C_CheckinInfoNotify stNotify;
	stNotify.m_Data.CopyFrom(m_oNotify);
	pRole->Send(stNotify);

	m_oNotify.clear_daycheckinfo();
	m_oNotify.clear_daymakeup();
	m_oNotify.clear_daycancheck();
}

bool CCheckinMgr::IsCanCheckin(CCheckinRecord* pRecord, UINT32 dwMonthDay, UINT32 dwStartDay)
{
	if (NULL == pRecord)
	{
		return false;
	}
	if (dwMonthDay <= pRecord->CheckinDays() + dwStartDay - 1)
	{
		return false;
	}
	return true;
}

bool CCheckinMgr::OnCheckin(Role* pRole, CCheckinRecord* pRecord, UINT32 dwMonth, UINT32 dwMonthDay, UINT32& dwBonus, UINT32 dwStartDay)
{
	if (NULL == pRole || NULL == pRecord)
	{
		return false;
	}
	if (!pRecord->IsDayChecked(dwMonthDay - 1))
	{
		///> 当天签到
		pRecord->SetCheckinInfo(dwMonthDay - 1);
	}
	else
	{
		///> 扣物品
		if (!TakeCostManager::Instance()->TakeCost("SignCost", pRole, pRecord->GetCheckinCount(), ItemFlow_Checkin, ItemFlow_Checkin_Day))
		{
			return false;
		}
		///> 补签
		UINT32 dwCheckDay = pRecord->GetFirstUnCheckinDay(dwStartDay);
		pRecord->SetCheckinInfo(dwCheckDay, true);
	}
	
	///> 给签到物品
	ItemDesc stItem;
	if (!GetItem(dwMonth, pRecord->CheckinDays(), dwBonus, stItem))
	{
		return false;
	}
	stItem.itemCount = pRole->Get<CPayV2Record>()->IsCheckinDoubleDays(pRecord->CheckinDays())?2*stItem.itemCount*dwBonus:stItem.itemCount*dwBonus;
	std::vector<ItemDesc> item;
	std::vector<ItemDesc> itemx;
	GetQQVipItem(pRole, itemx);
	GetStartUpItem(pRole, itemx);
	for (auto it = itemx.begin(); it != itemx.end(); ++it)
	{
		ItemDesc& temp = *it;
		if (temp.itemID == stItem.itemID)
		{
			stItem.itemCount += temp.itemCount;
		}
		else
		{
			item.push_back(temp);
		}
	}
	item.push_back(stItem);
	BagGiveItemTransition stGiveTransition(pRole);
	stGiveTransition.SetReason(ItemFlow_Checkin, ItemFlow_Checkin_Day);
	for (UINT32 i = 0; i < item.size(); ++ i)
	{
		stGiveTransition.GiveItem(item[i].itemID, item[i].itemCount);
	}
	stGiveTransition.NotifyClient();

	return true;
}

CanCheckinInfo CCheckinMgr::GetCanCheckinInfo(Role* pRole, time_t dwNowTime)
{
	tm* stNowTime = localtime(&dwNowTime);
	UINT32 dwNowMonth = stNowTime->tm_mon;
	UINT32 dwNowDay = stNowTime->tm_mday;

	time_t dwCreateTime = pRole->GetCreateTime() > SECONDS_OFFSET ? pRole->GetCreateTime() - SECONDS_OFFSET : 0;
	tm* stCreateTime = localtime(&dwCreateTime);
	UINT32 dwCreateMonth = stCreateTime->tm_mon;
	UINT32 dwCreateDay = stCreateTime->tm_mday;

	CanCheckinInfo stInfo;
	if (dwCreateMonth != dwNowMonth)
	{
		stInfo.dwCanCheckDay = dwNowDay;
		stInfo.dwStartDay = 1;
	}
	else
	{
		stInfo.dwCanCheckDay = (dwCreateDay < dwNowDay) ? (dwNowDay - dwCreateDay + 1) : 1; 
		stInfo.dwStartDay = dwCreateDay;
	}
	return stInfo;
}

void CCheckinMgr::UpdateCheckinItems()
{
	time_t dwNowTime = time(NULL) - SECONDS_OFFSET;
	tm* stNowTime = localtime(&dwNowTime);
	if (m_dwCurrentMonth == (UINT32)stNowTime->tm_mon)
	{
		return;
	}
	m_dwCurrentMonth = stNowTime->tm_mon;
	m_oNotify.Clear();

	UINT32 dwStart = (0 == (stNowTime->tm_mon % 2)) ? 0 : 31;
	UINT32 dwAllDays = GetMonthDay(1900 + stNowTime->tm_year, stNowTime->tm_mon + 1);
	if (dwStart + 31 > m_oTable.Table.size())
	{
		return;
	}
	for (UINT32 i = dwStart; i < dwStart + dwAllDays; ++i)
	{
		m_oNotify.add_itemid(m_oTable.Table[i]->reward);
		m_oNotify.add_itemcount(m_oTable.Table[i]->count);
	}
}

UINT32 CCheckinMgr::GetMonthDay(UINT32 dwYear, UINT32 dwMonth)
{
	switch(dwMonth)  
	{  
	case 1:return 31;  
	case 3:return 31;  
	case 4:return 30;  
	case 5:return 31;  
	case 6:return 30;
	case 7:return 31;
	case 8:return 31;
	case 9:return 30;  
	case 10:return 31;  
	case 11:return 30;  
	case 12:return 31; 
	}  
	if((0 == dwYear%4 && 0 != dwYear%100) || 0 == dwYear%400)  
	{
		return 29;
	}
	else
	{
		return 28;
	}
}

bool CCheckinMgr::GetItem(UINT32 dwMonth, UINT32 dwCheckDay, UINT32& dwBonus, ItemDesc& stItem)
{
	if (0 == dwCheckDay)
	{
		return false;
	}
	dwCheckDay = (0 == dwMonth % 2) ? dwCheckDay - 1 : dwCheckDay + 30; 
	if (dwCheckDay >= m_oTable.Table.size())
	{
		return false;
	}
	CheckinTable::RowData* pRowData = m_oTable.Table[dwCheckDay];
	if (NULL == pRowData)
	{
		return false;
	}
	stItem.itemID = pRowData->reward;
	stItem.itemCount = pRowData->count;
	UINT32 dwRandom = (UINT32)XRandom::randInt(1, 101);
	for (auto i = pRowData->bonus.begin(); i != pRowData->bonus.end(); ++i)
	{
		if (dwRandom <= (*i).seq[0])
		{
			dwBonus = (*i).seq[1];
			break;
		}
	}
	return true;
}
bool CCheckinMgr::GetQQVipItem(Role* pRole, std::vector<ItemDesc>& stItem)
{
	if (pRole->Get<QQVipRecord>()->IsSVip())
	{
		for (size_t i = 0; i < GetGlobalConfig().QQSVipSignIn.size(); ++i)
		{
			ItemDesc item;
			item.itemID = GetGlobalConfig().QQSVipSignIn[i][0];
			item.itemCount = GetGlobalConfig().QQSVipSignIn[i][1];
			stItem.push_back(item);
		}
	}
	else if (pRole->Get<QQVipRecord>()->IsVip())
	{
		for (size_t i = 0; i < GetGlobalConfig().QQVipSignIn.size(); ++i)
		{
			ItemDesc item;
			item.itemID = GetGlobalConfig().QQVipSignIn[i][0];
			item.itemCount = GetGlobalConfig().QQVipSignIn[i][1];
			stItem.push_back(item);
		}
	}
	else
	{
		return false;
	}
	return true;
}


bool CCheckinMgr::GetStartUpItem(Role* pRole, std::vector<ItemDesc>& stItem)
{
	if (pRole->GetStartUpType() == KKSG::StartUp_QQ)
	{
		for (size_t i = 0; i < GetGlobalConfig().QQGameCenterSingnIn.size(); ++i)
		{
			ItemDesc item;
			item.itemID = GetGlobalConfig().QQGameCenterSingnIn[i][0];
			item.itemCount = GetGlobalConfig().QQGameCenterSingnIn[i][1];
			stItem.push_back(item);
		}
	}
	else if (pRole->GetStartUpType() == KKSG::StartUp_WX)
	{
		for (size_t i = 0; i < GetGlobalConfig().WXGameCenterSingnIn.size(); ++i)
		{
			ItemDesc item;
			item.itemID = GetGlobalConfig().WXGameCenterSingnIn[i][0];
			item.itemCount = GetGlobalConfig().WXGameCenterSingnIn[i][1];
			stItem.push_back(item);
		}
	}
	else
	{
		return false;
	}
	return true;
}
void CCheckinMgr::Test()
{
	for (int i = 1; i <= 12; ++i)
	{
		SSWarn<<"month:"<<i<<END;
		CCheckinRecord stRecord;
		for (int j = 0; j <= (int)GetMonthDay(2015, i); ++j)
		{
			SSWarn<<"day: "<<j<<END;
			if (!IsCanCheckin(&stRecord, j, 1))
			{
				SSWarn<<"has checked in before"<<END;
				continue;
			}
			ItemDesc stItem;
			UINT32 dwBonus = 0;
			GetItem(i, j, dwBonus, stItem);
			SSWarn<<"item id:"<<stItem.itemID<<" item count:"<<stItem.itemCount<<" bonus:"<<dwBonus<<END;
			stRecord.SetCheckinInfo(j);
			if (!IsCanCheckin(&stRecord, j, 1))
			{
				SSWarn<<"checked in"<<END;
			}
		}
	}
}

bool CCheckinMgr::IsOddMonth()
{
	time_t dwNowTime = time(NULL) - SECONDS_OFFSET;
	tm* stNowTime = localtime(&dwNowTime);
	return 0==(stNowTime->tm_mon % 2)?true:false;
}
