#include "pch.h"
#include "ibshoprecord.h"
#include "pb/project.pb.h"
#include "util/XCommon.h"
#include "ibshopmgr.h"
#include "table/globalconfig.h"
#include "payv2Mgr.h"

IBShopRecord::IBShopRecord(Role* role)
{
	m_pRole			= role;
	m_nLastTime		= 0;
	m_nVipLv		= 0;
	m_bLimitHot	= false;			//显示红点标志
	m_nPayDegree   = 0;
	m_nPullTime = 0;
	m_nDayTime  = 0;
}

IBShopRecord::~IBShopRecord()
{
	m_pRole = NULL;
}

bool IBShopRecord::Load(const KKSG::RoleAllInfo *poRoleAllInfo)
{
	if (poRoleAllInfo->has_ibshopitems())
	{
		m_mapOrder.clear();
		const KKSG::IBShopAllRecord* pRecord = &poRoleAllInfo->ibshopitems();
		m_nLastTime = pRecord->nlasttime();
		m_nVipLv    = pRecord->nviplv();
		m_bLimitHot = pRecord->blimithot();
		m_nPayDegree = pRecord->paydegree();
		m_nDayTime = pRecord->daytime();
		for (int i= 0; i < pRecord->allibshopitems_size(); i++)
		{
			const KKSG::IBShopOneRecord *pItemOne				= &pRecord->allibshopitems(i);
			m_mapIBShopItem[pItemOne->ngoodsid()].nGoodsID		= pItemOne->ngoodsid();
			m_mapIBShopItem[pItemOne->ngoodsid()].nActivity			= pItemOne->activity();
			m_mapIBShopItem[pItemOne->ngoodsid()].nHasBuyCount	= pItemOne->nitemcount();
			m_mapIBShopItem[pItemOne->ngoodsid()].nLimitEndTime	= pItemOne->nupdatetime();
			m_mapIBShopItem[pItemOne->ngoodsid()].nActivtiyTime = pItemOne->activitytime();
		}
		for (int i= 0; i < pRecord->orders_size(); i++)
		{
			const KKSG::IBGiftOrder *pOrder = &pRecord->orders(i);
			if (pOrder)
				AddOrder(pOrder->orderid(),pOrder->time());
		}

	}
	return true;
}

void IBShopRecord::Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged)
{
	if (m_IsModify.TestAndReset())
	{
		KKSG::IBShopAllRecord* pRecord = poRoleAllInfo->mutable_ibshopitems();
		pRecord->Clear();
		pRecord->set_nlasttime(m_nLastTime);
		pRecord->set_nviplv(m_nVipLv);
		pRecord->set_blimithot(m_bLimitHot);
		pRecord->set_paydegree(m_nPayDegree);
		pRecord->set_daytime(m_nDayTime);
		for (auto iter = m_mapIBShopItem.begin(); iter != m_mapIBShopItem.end(); iter++)
		{
			KKSG::IBShopOneRecord* pOne = pRecord->add_allibshopitems();
			pOne->set_ngoodsid(m_mapIBShopItem[iter->first].nGoodsID);
			pOne->set_nitemcount(m_mapIBShopItem[iter->first].nHasBuyCount);
			pOne->set_activity(m_mapIBShopItem[iter->first].nActivity);
			pOne->set_activitytime(m_mapIBShopItem[iter->first].nActivtiyTime);
			pOne->set_nupdatetime(m_mapIBShopItem[iter->first].nLimitEndTime);
		}
		for (auto iter = m_mapOrder.begin(); iter!= m_mapOrder.end(); iter++)
		{
			KKSG::IBGiftOrder *pOrder = pRecord->add_orders();
			pOrder->set_orderid(iter->first);
			pOrder->set_time(iter->second);
		}
		roChanged.insert(pRecord);
	}
}

void IBShopRecord::SetSaveFlag()
{
	m_IsModify.Set();
}

void IBShopRecord::AddOrder(const std::string &order,UINT32 nTime,bool bSave)
{
	m_mapOrder[order] = nTime;
	if (bSave)
		SetSaveFlag();
}

void IBShopRecord::DelOrder(const std::string &order,bool bSave)
{
	auto iter = m_mapOrder.find(order);
	if (iter!=m_mapOrder.end())
		m_mapOrder.erase(iter);
	if (bSave)
		SetSaveFlag();
}

void IBShopRecord::ClearOrder()
{
	m_mapOrder.clear();
	SetSaveFlag();
}

bool IBShopRecord::HasOrder(const std::string &order)
{
	auto iter = m_mapOrder.find(order);
	if (iter!=m_mapOrder.end())
		return true;
	return false;
}

void IBShopRecord::SetPayDegree(UINT32 nDegree)
{
	m_nPayDegree = nDegree;
	SetSaveFlag();
}

void IBShopRecord::AddPayDegree(const UINT32 nDegree)
{
	UINT32 nTotal = m_nPayDegree + nDegree;
	SetPayDegree(nTotal);
	LogInfo("addpaydegree roleid=%llu name=%s degree=%u total=%u",m_pRole->GetID(),m_pRole->GetName().c_str(),nDegree,nTotal);
	PayV2Mgr::Instance()->CalcVipLevel(m_pRole, nDegree*10, KKSG::PAY_PARAM_NONE);
}

const UINT32 IBShopRecord::GetPayDegree()
{
	return m_nPayDegree;
}
