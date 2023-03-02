#include "pch.h"
#include "auctionbase.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "table/globalconfig.h"

AuctionItem::AuctionItem(UINT64 uid, UINT64 roleid, AuctionItemState state, UINT32 perprice, time_t reqsaletime, time_t onsaletime, const KKSG::Item& itemdata)
	:m_uid(uid),m_roleid(roleid),m_state(state),m_perprice(perprice),m_reqsaletime(reqsaletime),m_onsaletime(onsaletime),m_item(itemdata)
{
	m_outsaletime = (time_t)0;
	m_lockEndTime = (time_t)0;
	SetRoleAccount(m_roleid);
	SSDebug << __FUNCTION__ << END;
}

AuctionItem::AuctionItem(UINT64 uid, UINT64 roleid, const std::string& roleaccount, AuctionItemState state, UINT32 perprice, time_t reqsaletime, time_t onsaletime, UINT32 outsaletime, const KKSG::Item& itemdata)
	:m_uid(uid),m_roleid(roleid),m_roleaccount(roleaccount),m_state(state),m_perprice(perprice),m_reqsaletime(reqsaletime),m_onsaletime(onsaletime),m_outsaletime(outsaletime),m_item(itemdata)
{
	m_lockEndTime = (time_t)0;
	SSDebug << __FUNCTION__ << END;
}

AuctionItem::~AuctionItem()
{
	SSDebug << __FUNCTION__ << END;
}

bool AuctionItem::IsTreasuer()
{
	int randAttr = 0;
	if(m_item.has_randattr()) randAttr += m_item.randattr().attrs_size();
	if(m_item.has_forge()) randAttr += m_item.forge().attrs_size();
	if(randAttr >= 3)
	{
		return true;//装备
	}
	if(m_item.emblemthirdslot() >= 1)
	{
		return true;//纹章
	}
	return false;
}

void AuctionItem::FillSaleItem(KKSG::AuctionSaleData& data)
{
	data.set_uid(m_uid);
	data.set_perprice(m_perprice);
	if(m_state == AIS_OUT_SALE)
	{
		data.set_duelefttime(0);
	}
	else
	{
		UINT32 duelefttime = GetDueLeftTime(GameTime::GetTime());
		if(0 == duelefttime)
		{
			duelefttime = 1;
		}
		data.set_duelefttime(duelefttime);
	}
	data.mutable_itemdata()->CopyFrom(m_item);
}

UINT32 AuctionItem::GetDueLeftTime(time_t now)
{
	UINT32 OnSaleContinueTime = GetGlobalConfig().AuctOnSaleTime; 

	if(m_reqsaletime + OnSaleContinueTime > now)
	{
		return UINT32(m_reqsaletime + OnSaleContinueTime - now);
	}

	return 0;
}

void AuctionItem::TestPrint()
{
	SSDebug << " sale item " << END;
	SSDebug << " auctuid = " << m_uid << " roleid = " << m_roleid << " roleaccount = " << m_roleaccount << " preprice " 
		<< m_perprice << " reqsaleT = " << m_reqsaletime << " onsaleT = " << m_onsaletime << " outsaleT = " << m_outsaletime << END;
	SSDebug << " itemid = " << GetItemID() << " itemcount = " << GetItemCount() << " state = " << m_state << " duelefttime = " << GetDueLeftTime(GameTime::GetTime()) 
		<< " recoverlefttime = " << GetRecoverLeftTime(GameTime::GetTime()) << END;
}

void AuctionItem::SetRoleAccount(UINT64 roleid)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if(pRole)
	{
		m_roleaccount = pRole->GetAccount();
	}
	else
	{
		// 系统补的货就没有role信息，所以不算错误
		SSInfo << " find role null roleid = " << roleid << END;
	}
}

UINT32 AuctionItem::GetRecoverLeftTime(time_t now)
{
	if(m_outsaletime + GetGlobalConfig().AuctRecoverTime > now)
	{
		return UINT32(m_outsaletime + GetGlobalConfig().AuctRecoverTime - now);
	}
	return 0;
}

void AuctionItem::ReduceItemCount(UINT32 rcount)
{
	UINT32 oldcount = m_item.itemcount();
	UINT32 left = (oldcount < rcount)? 0 : oldcount - rcount;
	m_item.set_itemcount(left);
}

void AuctionItem::AddItemCount(UINT32 rcount)
{
	UINT32 oldcount = m_item.itemcount();
	UINT32 left =  oldcount + rcount;
	m_item.set_itemcount(left);
}

void AuctionItem::SetLock()
{
	SSDebug << __FUNCTION__ << END;
	m_lockEndTime = GameTime::GetTime() + AuctLockContiTime;
}

void AuctionItem::ClearLock()
{
	SSDebug << __FUNCTION__ << END;
	m_lockEndTime = time_t(0);
}

bool AuctionItem::IsLocked(time_t now)
{
	return now < m_lockEndTime;
}

void AuctionItem::FillDBData(KKSG::AuctDBItemDale& data)
{
	data.set_auctuid(m_uid);
	data.set_roleid(m_roleid);
	data.set_roleaccount(m_roleaccount);
	data.set_state(m_state);
	data.set_perprice(m_perprice);
	data.set_reqsaletime((UINT32)m_reqsaletime);
	data.set_onsaletime((UINT32)m_onsaletime);
	data.set_outsaletime((UINT32)m_outsaletime);
	std::string tmp; 
	m_item.SerializeToString(&tmp);
	data.set_itemdata(tmp.c_str(), tmp.size());
}

RoleSaleItem::RoleSaleItem(UINT64 roleid)
	:m_roleid(roleid)
{

}

void RoleSaleItem::AddAuctionItem(AuctionItem* pSaleItem)
{
	m_saleitems.insert(pSaleItem);
}

void RoleSaleItem::DelAuctionItem(AuctionItem* pSaleItem)
{
	m_saleitems.erase(pSaleItem);
}

void RoleSaleItem::FillSaleItem(KKSG::AuctionAllReqRes& roRes)
{
	for(auto it = m_saleitems.begin(); it != m_saleitems.end(); ++it)
	{
		AuctionItem* pSaleData = (*it);
		if(pSaleData)
		{
			pSaleData->FillSaleItem(*roRes.add_saledata());
		}
	}
}


int RoleSaleItem::GetValidTimeCount()
{
	time_t now = GameTime::GetTime();
	int count = 0;
	auto it = m_saleitems.begin();
	auto it_end = m_saleitems.end();
	for (; it != it_end; ++it)
	{
		if ( (*it)->GetDueLeftTime(now) > 0)
		{
			count += 1;
		}
	}
	return count; 
}

void RoleSaleItem::TestPrint()
{
	for(auto it = m_saleitems.begin(); it != m_saleitems.end(); ++it)
	{
		(*it)->TestPrint();
	}
}
