#include "pch.h"
#include <time.h>
#include "buyrecord.h"
#include "pb/project.pb.h"
#include "payv2Record.h"
#include "unit/role.h"
#include "payv2Mgr.h"
#include "define/itemdef.h"
#include "foreach.h"
#include "util/XCommon.h"
#include "table/backflowbuyconfig.h"
#include "pay/ptcg2c_updatebuygoldandfatigueinfo.h"
#include "common/ptcg2c_hintnotify.h"
#include "define/systemiddef.h"

void BackFlowBuyInfo::Load(const KKSG::BackFlowBuy& info)
{
	buyindex = info.index();
	buyday = info.day();
	buycount = info.count();
}

void BackFlowBuyInfo::Save(KKSG::BackFlowBuy* info)
{
	info->set_index(buyindex);
	info->set_day(buyday);
	info->set_count(buycount);
}

UINT32 BackFlowBuyInfo::GetTodayIndex(UINT32 day)
{
	if (day != buyday)
	{
		///> 今天没买, index + 1
		buycount = 0;
		return buyindex + 1;
	}
	else
	{
		///> 今天买了
		return buyindex;
	}
}

void BackFlowBuyInfo::DoBuy(UINT32 index, UINT32 day)
{
	if (index != buyindex)
	{
		buycount = 1;
		buyday = day;
		buyindex = index;
	}
	else
	{
		++buycount;
	}
	modify.Set();
}

void BackFlowBuyInfo::Reset()
{
	buyindex = 0;
	buyday = 0;
	buycount = 0;
	islock = false;

	modify.Set();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
BuyRecord::BuyRecord(Role *pRole)
{
	m_pRole = pRole;
	m_BuyDay = 0;
	m_BuyGoldCount = 0;
	m_BuyDragonCoinCount = 0;
}

BuyRecord::~BuyRecord()
{

}

bool BuyRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_buyinfo())
	{
		const KKSG::BuyGoldFatInfo &buyInfo = poRoleAllInfo->buyinfo();
		m_BuyDay = buyInfo.day();
		m_BuyGoldCount = buyInfo.buygoldcount();
		m_BuyDragonCoinCount = buyInfo.buydragoncount();

		BuyFatigueSt st;
		int n = buyInfo.buyfatiguecount_size()/2;
		for (int i = 0; i < n; ++i)
		{
			st.fagitueID = buyInfo.buyfatiguecount(i*2);
			st.buyCount = buyInfo.buyfatiguecount(i*2+1);
			m_BuyFatgiueCount.push_back(st);
		}
		
		m_backflow.Load(buyInfo.backflow());	
	}

	return true;
}


void BuyRecord::SaveTo(KKSG::BuyGoldFatInfo *pBuyInfo)
{
	pBuyInfo->set_day(m_BuyDay);
	pBuyInfo->set_buygoldcount(m_BuyGoldCount);
	pBuyInfo->set_buydragoncount(m_BuyDragonCoinCount);

	pBuyInfo->clear_buyfatiguecount();
	for (UINT32 i = 0; i < m_BuyFatgiueCount.size(); ++i)
	{
		pBuyInfo->add_buyfatiguecount(m_BuyFatgiueCount[i].fagitueID);
		pBuyInfo->add_buyfatiguecount(m_BuyFatgiueCount[i].buyCount);
	}

	m_backflow.Save(pBuyInfo->mutable_backflow());
}


void BuyRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_isModified.TestAndReset() || m_backflow.modify.TestAndReset())
	{
		KKSG::BuyGoldFatInfo *pBuyInfo = poRoleAllInfo->mutable_buyinfo();
		SaveTo(pBuyInfo);
		roChanged.insert(pBuyInfo);
	}
}

int BuyRecord::GetBuyGoldCount()
{
	CheckDay();

	return m_BuyGoldCount;
}

void BuyRecord::IncBuyGoldCount()
{
	++m_BuyGoldCount;
	m_isModified.Set();
}

int BuyRecord::GetBuyFatigueCount(int fatigueID)
{
	CheckDay();

	foreach (i in m_BuyFatgiueCount)
	{
		if (fatigueID == i->fagitueID)
		{
			return i->buyCount;
		}
	}

	return 0;
}

void BuyRecord::IncBuyFagitueCount(int fatigueID)
{
	bool found = false;
	foreach (i in m_BuyFatgiueCount)
	{
		if (fatigueID == i->fagitueID)
		{
			i->buyCount += 1;
			found = true;
			break;
		}
	}

	if (found == false)
	{
		BuyFatigueSt buy;
		buy.fagitueID = fatigueID;
		buy.buyCount = 1;
		m_BuyFatgiueCount.push_back(buy);
	}

	m_isModified.Set();
}

void BuyRecord::CheckDay()
{
	int day = XCommon::GetDayBeginTime(time(0));
	if (m_BuyDay != day)
	{
		m_BuyDay = day;
		m_BuyGoldCount = 0;
		m_BuyDragonCoinCount = 0;
		m_BuyFatgiueCount.clear();
		m_isModified.Set();
	}
}

bool BuyRecord::CanBuyGold()
{
	CPayV2Record *pCPayV2Record = m_pRole->Get<CPayV2Record>();
	VIPTable::RowData *pRow = PayV2Mgr::Instance()->GetVipConf(pCPayV2Record->GetVipLevel());
	if (pRow == NULL)
	{
		return false;
	}

	return GetBuyGoldCount() < pRow->BuyGoldTimes;
}

bool BuyRecord::CanBuyFatigue(int fatigueID)
{
	if (fatigueID != FATIGUE)
	{
		return false;
	}

	CPayV2Record *pCPayV2Record = m_pRole->Get<CPayV2Record>();
	VIPTable::RowData *pRow = PayV2Mgr::Instance()->GetVipConf(pCPayV2Record->GetVipLevel());
	if (pRow == NULL)
	{
		return false;
	}

	return GetBuyFatigueCount(fatigueID) < pRow->BuyFatigueTimes;
}

void BuyRecord::SendToClient()
{
	PtcG2C_UpdateBuyGoldAndFatigueInfo oPtc;
	SaveTo(&oPtc.m_Data);
	m_pRole->Send(oPtc);
}

void BuyRecord::LoginCheck()
{
	CheckDay();
	SendToClient();
}

int BuyRecord::GetBuyDragonCoinCount()
{
	CheckDay();
	return m_BuyDragonCoinCount;
}

void BuyRecord::IncBuyDragonCoinCount(UINT32 count)
{
	m_BuyDragonCoinCount += count;
	m_isModified.Set();
}

bool BuyRecord::CanBuyDragon(UINT32 count)
{
	CPayV2Record *pCPayV2Record = m_pRole->Get<CPayV2Record>();
	VIPTable::RowData *pRow = PayV2Mgr::Instance()->GetVipConf(pCPayV2Record->GetVipLevel());
	if (pRow == NULL)
	{
		return false;
	}
	return (GetBuyDragonCoinCount() + count) <= pRow->BuyDragonCoinTimes;
}


void BuyRecord::HintBackFlow()
{
	if (m_pRole->IsBackFlowRole())
	{
		UINT32 day = XCommon::GetTodayUnique();
		UINT32 index = m_backflow.GetTodayIndex(day);
		UINT32 countlimit = BackFlowBuyConfig::Instance()->GetCanBuyCount(index);
		if (countlimit > m_backflow.buycount)
		{
			PtcG2C_HintNotify ntf;
			ntf.m_Data.add_systemid(SYS_BACKFLOW_BUY);
			ntf.m_Data.set_isremove(false);
			m_pRole->Send(ntf);
		}
	}
}
