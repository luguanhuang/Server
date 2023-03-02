#include "pch.h"
#include "guildauctbase.h"
#include "table/globalconfig.h"
#include "table/guildauctconfig.h"
#include "guildauctdbhandler.h"
#include "util/XCommon.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "guildauctrequest.h"
#include "util/gametime.h"
#include <cmath>

GABaseData::GABaseData(UINT64 UID, UINT32 itemID, UINT32 itemCount, int actType, UINT64 guildID, time_t now, std::vector<UINT64>& joinRoles)
{
	m_DBData.set_uid(UID);
	m_DBData.set_itemid(itemID);
	m_DBData.set_itemcount(itemCount);
	m_DBData.set_guildid(guildID);
	m_DBData.set_acttype(actType);
	for(size_t i = 0; i < joinRoles.size(); ++i)
	{
		m_DBData.add_joinroles(joinRoles[i]);
	}

	SetSaleTypeData(GAST_GUILD_AUCT_ING, now, true);
	InitDefault();
	SSDebug << __FUNCTION__ << " " << m_DBData.uid() << END;
}

GABaseData::GABaseData(const KKSG::GAItemDBData& data)
{
	m_DBData = data;
	InitDefault();
	SSDebug << __FUNCTION__ << " " << m_DBData.uid() << END;
}

GABaseData::~GABaseData()
{
	SSDebug << __FUNCTION__ << " " << m_DBData.uid() << END;
}

void GABaseData::SetSaleTypeData(int saleType, time_t now, bool init/* = flase*/)
{
	SSDebug << __FUNCTION__ << " saletype = " << saleType << " now = " << now << END;
	m_DBData.set_saletype(saleType);
	auto conf = GuildAuctConfig::Instance()->GetItemConf(m_DBData.itemid());
	if(NULL == conf)
	{
		SSError << " find guild auction item config failed itemid = " << m_DBData.itemid() << END;
	}
	switch(saleType)
	{
	case GAST_GUILD_AUCT_ING:
		{
			if(conf)
			{
				m_DBData.set_auctprice(conf->GuildBasePrice);
				m_DBData.set_maxprice(conf->GuildMaxPrice);
			}
			else
			{
				m_DBData.set_auctprice(30);
				m_DBData.set_maxprice(100);
			}
			m_DBData.set_gwtime(UINT32(now));
		}
		break;
	case GAST_WORLD_AUCT_ING:
		{
			if(conf)
			{
				m_DBData.set_auctprice(conf->WorldBasePrice);
				m_DBData.set_maxprice(conf->WorldMaxPrice);
			}
			else
			{
				m_DBData.set_auctprice(30);
				m_DBData.set_maxprice(100);
			}
			m_DBData.set_gwtime(UINT32(now));
		}
		break;
	case GAST_GUILD_BUY_NOW_UNPROF:
	case GAST_GUILD_BUY_AUCT:
	case GAST_WORLD_BUY_NOW:
	case GAST_WORLD_BUY_AUCT:
		{
			m_DBData.set_saledtime(UINT32(now));
		}
		break;
	case GAST_GUILD_BUY_NOW_PROFIT:
		{
			;
		}
		break;
	default:
		{
			SSError << " set unknow saletype = " << saleType << " UID = " << GetUID() << END;
		}
		break;
	}

	if(!init)
	{
		GADBHandler::Update(this);
	}
}

void GABaseData::FillClientOnSaleData(KKSG::GASaleItem& data, UINT64 roleID)
{
	data.set_uid(m_DBData.uid());
	data.set_acttype(m_DBData.acttype());
	data.set_itemid(m_DBData.itemid());
	data.set_auctroleid(m_DBData.auctroleid());
	data.set_curauctprice(m_DBData.auctprice());
	data.set_maxprice(m_DBData.maxprice());
	data.set_lefttime(GetSaleLeftTime((UINT32)GameTime::GetTime()));
	data.set_display(m_DBData.saletype() == GAST_GUILD_AUCT_ING  || m_DBData.saletype() == GAST_WORLD_AUCT_ING ? true:false);
}

void GABaseData::FillClientSaleHistoryData(KKSG::GASaleHistory& data, bool guild/* = false*/)
{
	data.set_acttype(m_DBData.acttype());
	data.set_saletime(m_DBData.saledtime());
	data.set_itemid(m_DBData.itemid());
	int saleType = GetSaleType();
	if(guild)
	{
		if(GAST_GUILD_BUY_NOW_UNPROF == saleType || GAST_GUILD_BUY_NOW_PROFIT == saleType)
		{
			data.set_auctresult(KKSG::GA_RESULT_BUY_NOW);
			data.set_saleprice(m_DBData.maxprice());
		}
		else if(GAST_GUILD_BUY_AUCT == saleType)
		{
			data.set_auctresult(KKSG::GA_RESULT_BUY_AUCT);
			data.set_saleprice(m_DBData.auctprice());
		}
		else 
		{
			data.set_saletime(m_DBData.gwtime());
			data.set_auctresult(KKSG::GA_RESULT_TO_WORLD);
		}
	}
	else
	{
		if(GAST_WORLD_BUY_NOW == saleType)
		{
			data.set_auctresult(KKSG::GA_RESULT_BUY_NOW);
			data.set_saleprice(m_DBData.maxprice());
		}
		else if(GAST_WORLD_BUY_AUCT == saleType)
		{
			data.set_auctresult(KKSG::GA_RESULT_BUY_AUCT);
			data.set_saleprice(m_DBData.auctprice());
		}
		else
		{
			data.set_auctresult(KKSG::GA_RESULT_TO_WORLD);
		}
	}
}

void GABaseData::FillDBData(KKSG::GAItemDBData& data)
{
	data = m_DBData;
}

void GABaseData::GetJoinRoles(std::unordered_set<UINT64>& joinRoles)
{
	for(int i = 0; i < m_DBData.joinroles_size(); ++i)
	{
		joinRoles.insert(m_DBData.joinroles(i));
	}
}

UINT32 GABaseData::GetJoinRoleCount()
{
	return UINT32(m_DBData.joinroles_size());
}

UINT32 GABaseData::GetSaleLeftTime(UINT32 now)
{
	UINT32 ConfTime = 0;
	UINT32 CSTime = 0;
	if(GetSaleType() == GAST_GUILD_AUCT_ING || GetSaleType() == GAST_GUILD_BUY_NOW_UNPROF)
	{
		ConfTime = GetGlobalConfig().GuildAuctGuildSaleTime;
		CSTime = m_DBData.gwtime();
	}
	else if(GetSaleType() == GAST_WORLD_AUCT_ING)
	{
		ConfTime = GetGlobalConfig().GuildAuctWorldSaleTime;
		CSTime = m_DBData.gwtime();
	}

	if(!ConfTime || !CSTime)
	{
		return 10;
	}
	
	if(CSTime + ConfTime > now)
	{
		return (CSTime + ConfTime - now);
	}
	return 0;
}

void GABaseData::SetLock()
{
	m_lockTime = GameTime::GetTime() + GAItemLockTime;
}

bool GABaseData::IsLocked()
{
	return (GameTime::GetTime() < m_lockTime);
}

void GABaseData::ClearLock()
{
	m_lockTime = GameTime::GetTime();
}

void GABaseData::TestPrint()
{
	SSDebug << __FUNCTION__ << " : " << END;
	SSDebug << " UID = " << GetUID() << " " << (GetUID() & 0x00000000ffffffff) << END;
	SSDebug << " itemid = " << GetItemID() << " itemcount = " << GetItemCount() << " saletype = " << GetSaleType() << END;
	SSDebug << " auctrole = " << GetAuctRoleID() << " auctprice = " << GetAuctPrice() << " maxprice = " << GetMaxPrice() << END;
	SSDebug << " guildid = " << GetGuildID() << " acttype = " << GetActType() << " joinrolesize = " << m_DBData.joinroles_size() << " islocked = " << IsLocked() << END;
}

void GABaseData::SetAuctData(UINT64 roleID, UINT32 price)
{
	SSDebug << __FUNCTION__ << " roleid = " << roleID << " price = " << price << END;
	m_DBData.set_auctroleid(roleID); 
	m_DBData.set_auctprice(price); 

	GADBHandler::Update(this);
}

UINT32 GABaseData::GetAuctUpPrice()
{
	return (GetAuctPrice() + GetAuctPrice() * GetGlobalConfig().GuildAuctUpRate / 100);
}

void GABaseData::InitDefault()
{
	ClearLock();
}

GAPAIR_KEY GABaseData::GetPairKey()
{
	return GAPAIR_KEY(GetGuildID(), GetActType()); 
}

bool GABaseData::IsSaled()
{
	return (GetSaleType() != GAST_GUILD_AUCT_ING && GetSaleType() != GAST_WORLD_AUCT_ING); 
}

bool GABaseData::NeedDel(time_t now)
{
	if(IsSaled() && GetSaleType() != GAST_GUILD_BUY_NOW_UNPROF)
	{
		return !XCommon::IsDaySame(time_t(m_DBData.saledtime()), now);
	}
	return false;
}

bool GABaseData::IsAuctSaled()
{
	return (GetSaleType() == GAST_GUILD_BUY_AUCT || GetSaleType() == GAST_WORLD_BUY_AUCT);
}

bool GABaseData::IsWorldSaled()
{
	return (GetSaleType() == GAST_WORLD_BUY_AUCT || GetSaleType() == GAST_WORLD_BUY_NOW);
}

GAGuildActType::GAGuildActType(GAPAIR_KEY gakey)
{
	m_key = gakey;
	m_allPrice = 0.0;
}

void GAGuildActType::AddBaseData(GABaseData* baseData)
{
	if(NULL == baseData)
	{
		return ;
	}
	UINT32 tmpprice = 0;
	UINT32 due = 0;
	if(baseData->IsSaled())
	{
		if(baseData->IsAuctSaled())
		{
			tmpprice = baseData->GetAuctPrice();
		}
		else
		{
			tmpprice = baseData->GetMaxPrice();
		}
		due = GetGlobalConfig().GuildAuctGuildDue;
	}
	else
	{
		if(baseData->HaveAucted())
		{
			due = GetGlobalConfig().GuildAuctGuildDue;
		}
		else
		{
			due = GetGlobalConfig().GuildAuctWorldDue;
		}
		tmpprice = baseData->GetAuctPrice();
	}
	if(due > 100)
	{
		due = 100;
	}
	
	m_allPrice += (1.0 * tmpprice * (100.0 - due) / 100.0);

	if(m_joinRoles.empty() && baseData->GetItemCount() > 0)
	{
		baseData->GetJoinRoles(m_joinRoles);
	}
}

UINT32 GAGuildActType::GetRoleProfit(UINT64 roleID)
{
	int profitType = GuildAuctConfig::Instance()->GetProfitType(m_key.second);
	if(profitType == GAPROFIT_TYPE_JOIN)
	{
		if(0.0 == m_allPrice)
		{
			return 0;
		}
		if(m_joinRoles.find(roleID) == m_joinRoles.end())//不是参与者
		{
			return (UINT32)(-1);
		}
	}
	return GetProfit();
}

UINT32 GAGuildActType::GetProfit()
{
	UINT32 rolecount = 0;
	int profitType = GuildAuctConfig::Instance()->GetProfitType(m_key.second);
	if(profitType == GAPROFIT_TYPE_JOIN)
	{
		rolecount = m_joinRoles.size();
	}
	else
	{
		auto pGuild = CGuildMgr::Instance()->GetGuild(m_key.first);
		if(pGuild)
		{
			rolecount = pGuild->GetMemberCount();
		}
	}
	if(0 == rolecount)
	{
		return 0;
	}
	return UINT32(std::ceil(m_allPrice/rolecount));
}

void GAGuildActType::MakeProfit()
{
	int profitType = GuildAuctConfig::Instance()->GetProfitType(m_key.second);
	if(profitType == GAPROFIT_TYPE_ALL)
	{
		auto pGuild = CGuildMgr::Instance()->GetGuild(m_key.first);
		if(pGuild)
		{
			const std::map<UINT64, GuildMember>& members = pGuild->GetGuildMember(); 
			for (auto iter = members.begin(); iter != members.end(); ++iter)
			{
				m_joinRoles.insert(iter->first);
			}
			/*
			const auto& memebers = pGuild->GetGuildMember();
			for(size_t i = 0; i < memebers.size(); ++i)
			{
				m_joinRoles.insert(memebers[i].qwRoleID);
			}
			*/
		}
	}

	UINT32 profit = GetProfit();
	GuildAuctRequest req;
	for(auto it = m_joinRoles.begin(); it != m_joinRoles.end(); ++it)
	{
		req.AuctProfitMail(*it, m_key.first, m_key.second, profit);
	}
}

void GAGuildActProfit::AddGABaseData(GABaseData* baseData)
{
	if(NULL == baseData)
	{
		return ;
	}
	auto gakey = baseData->GetPairKey();
	auto it = m_waitProfit.find(gakey);
	if(it == m_waitProfit.end())
	{
		GAGuildActType tmp(gakey);
		m_waitProfit.insert(std::make_pair(gakey, tmp));
		it = m_waitProfit.find(gakey);
	}

	if(it != m_waitProfit.end())
	{
		(it->second).AddBaseData(baseData);
	}
}

void GAGuildActProfit::MakeProfit()
{
	for(auto it = m_waitProfit.begin(); it != m_waitProfit.end(); ++it)
	{
		(it->second).MakeProfit();
	}
}
