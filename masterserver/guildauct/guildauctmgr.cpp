#include "pch.h"
#include "guildauctmgr.h"
#include "define/itemdef.h"
#include "table/guildauctconfig.h"
#include "guildauctdbhandler.h"
#include "mail/mailconfig.h"
#include "define/maildef.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "guildauctrequest.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "util/XCommon.h"
#include "common/ptcm2c_hintnotifyms.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/systemiddef.h"
#include "notice/noticemgr.h"
#include "loghelper/tlogr.h"
#include "util/gametime.h"
#include "auction/ptcm2c_guildauctitemtimefresh.h"
#include "guild/guildmgr.h"
#include "config.h"

const int GuildAuctMaxCount = 100000;
const int GuildAuctClientSendCount = 200;

INSTANCE_SINGLETON(GuildAuctMgr)

	GuildAuctMgr::GuildAuctMgr()
{
	m_lastUpProfitDataTime = GameTime::GetTime();
}

GuildAuctMgr::~GuildAuctMgr()
{

}

bool GuildAuctMgr::Init()
{
	m_handler = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);	
	m_UIDIndex = 0;

	if(!GADBHandler::Read())
	{
		return false;
	}

	// 加载活动类型与最大分红数
	auto it		= GetGlobalConfig().GuildAuctProfitMax.begin();
	auto it_end = GetGlobalConfig().GuildAuctProfitMax.end();
	for (; it != it_end; ++it)
	{
		type_profit_max_.insert(std::make_pair(it->seq[0],it->seq[1]));
	}

	// 工会拍卖额外系数
	auto it_rate	 = GetGlobalConfig().GuildAuctGuildItemExRate.begin();
	auto it_rate_end = GetGlobalConfig().GuildAuctGuildItemExRate.end();
	for (; it_rate != it_rate_end; ++it_rate)
	{
		type_profit_rate_.insert(std::make_pair((UINT32)it_rate->seq[0],it_rate->seq[1]));
	}

	return true;
}

void GuildAuctMgr::Uninit()
{
	if(m_handler)
	{
		CTimerMgr::Instance()->KillTimer(m_handler);
		m_handler = 0;
	}

	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		delete (it->second);
	}
	m_allData.clear();
}

void GuildAuctMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	CheckSaleState();
	CheckProfitData();
}

void GuildAuctMgr::AddGuildAuctData(UINT64 guildID, int actType, int rank)
{
	std::vector<UINT64> tmpRole;
	AddGuildAuctData(guildID, actType, rank, tmpRole);
}
void GuildAuctMgr::AddGuildAuctData(UINT64 guildID, int actType, int rank, std::vector<UINT64>& roles)
{
	SSInfo << __FUNCTION__ << " guildid = " << guildID << " act = " << actType << " rank = " << rank << " roles = " << roles.size() << END;
	auto pGuild = CGuildMgr::Instance()->GetGuild(guildID);
	if(NULL == pGuild)
	{
		SSInfo << __FUNCTION__ << " find not guild = " << guildID << END;
		return ;
	}
	UINT32 roleCount = 0;	
	if(GuildAuctConfig::Instance()->GetProfitType(actType) == GAPROFIT_TYPE_JOIN)
	{
		roleCount = roles.size();
	}
	else
	{
		roleCount = (UINT32)pGuild->GetMemberCount();
	}
	std::vector<ItemDesc> items;
	auto conf = GuildAuctConfig::Instance()->GetItems(actType, rank, items, roleCount);
	if(NULL == conf)
	{
		SSError << " find guild auction reward failed acttype = " << actType << " rank = " << rank << END;
		return ;
	}

	time_t now = GameTime::GetTime();
	for(size_t i = 0; i < items.size(); ++i)
	{
		UINT32 itemCount = items[i].itemCount;
		for(size_t j = 0; j < itemCount; ++j)
		{
			UINT64 UID = NewUID();
			GABaseData* baseData = new GABaseData(UID, items[i].itemID, 1, actType, guildID, now, roles);	
			if(NULL == baseData)
			{
				continue;
			}
			AddUniOne(baseData);
			TLogAddItem(baseData);
			GADBHandler::Add(baseData);
		}
	}

	if(m_allData.size() >= GuildAuctMaxCount)
	{
		SSWarn << __FUNCTION__ << " allCount = " << m_allData.size() << END;
	}

	if(pGuild)
	{
		const auto& members = pGuild->GetGuildMember();	
		for (auto iter = members.begin(); iter != members.end(); ++iter)
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(iter->first);
			if(pRole)
			{
				HintNotify(pRole);					
			}
		}
		/*
		for(size_t i = 0; i < members.size(); ++i)
		{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(members[i].qwRoleID);
		if(pRole)
		{
		HintNotify(pRole);					
		}
		}
		*/
	}

	// 领地争夺战不需要发通知
	if (actType != GA_ACT_TERRITORY && actType != GA_ACT_TERRITORY_DAY)
	{
		NoticeMgr::Instance()->NoticeGuildAuctActEnd(guildID, conf->ActName, rank);
	}
}

void GuildAuctMgr::DelGuildAuctData(UINT64 UID)
{
	auto data = GetGuildAuctData(UID);
	if(NULL == data)
	{
		return ;
	}
	DelUniOne(data);

	GADBHandler::Del(UID);

	delete data;
}

GABaseData* GuildAuctMgr::GetGuildAuctData(UINT64 UID)
{
	auto it = m_allData.find(UID);
	if(it != m_allData.end())
	{
		return (it->second);
	}
	return NULL;
}

void GuildAuctMgr::AddUniOne(GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	UINT64 UID = data->GetUID();
	if(m_allData.find(UID) != m_allData.end())
	{
		SSError << " uid repeat uid = " << UID << " guildid = " << data->GetGuildID() << " acttype = " << data->GetActType() << END;
		return ;
	}

	m_allData[data->GetUID()] = data;
}

void GuildAuctMgr::DelUniOne(GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	m_allData.erase(data->GetUID());
}

void GuildAuctMgr::UpdateUID(UINT64 UID)
{
	if(m_UIDIndex < (UID&0x00000000ffffffff))
	{
		m_UIDIndex = (UID&0x00000000ffffffff);
	}
}

UINT64 GuildAuctMgr::NewUID()
{
	++m_UIDIndex;
	UINT64 servergroupid = (UINT64)MSConfig::Instance()->GetServerID();
	UINT64 UID  = ((servergroupid << 32) | m_UIDIndex);
	return UID;
}

void GuildAuctMgr::CheckSaleState()
{
	time_t now = GameTime::GetTime();
	std::vector<UINT64> waitDel;//待删除
	GAGuildActProfit waitProfit;//待分红

	GuildAuctRequest req;

	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		UINT64 UID = it->first;
		auto data = it->second;
		if(data->IsLocked())
		{
			continue;
		}
		switch(data->GetSaleType())
		{
		case GAST_GUILD_AUCT_ING:
			{
				if(0 == data->GetSaleLeftTime(now))
				{
					if(data->HaveAucted())//有人竞拍
					{
						req.AuctSuccessMail(data->GetAuctRoleID(), data->GetItemID(), data->GetItemCount(), false);//给物品
						data->SetSaleTypeData(GAST_GUILD_BUY_AUCT, now);
						TLogGiveItem(data->GetAuctRoleID(), data);
						waitProfit.AddGABaseData(data);

						// 保存一边检查是否需要给世界拍卖行额外物品
						CheckAuctItem(data);
					}
					else
					{
						waitProfit.AddGABaseData(data);
						if(GuildAuctConfig::Instance()->IsActFlow(data->GetActType()))
						{
							data->SetSaleTypeData(GAST_WORLD_AUCT_ING, now);
							TLogAddItem(data);
						}
						else
						{
							waitDel.push_back(UID);
						}
					}
				}
			}
			break;
		case GAST_GUILD_BUY_NOW_UNPROF:
			{
				if(0 == data->GetSaleLeftTime(now))
				{
					data->SetSaleTypeData(GAST_GUILD_BUY_NOW_PROFIT, now);
					waitProfit.AddGABaseData(data);

					// 保存一边检查是否需要给世界拍卖行额外物品
					CheckAuctItem(data);					
				}
			}
			break;
		case GAST_WORLD_AUCT_ING:
			{
				if(0 == data->GetSaleLeftTime(now))
				{
					if(data->HaveAucted())
					{
						req.AuctSuccessMail(data->GetAuctRoleID(), data->GetItemID(), data->GetItemCount(), false);
						data->SetSaleTypeData(GAST_WORLD_BUY_AUCT, now);
						TLogGiveItem(data->GetAuctRoleID(), data);
					}
					else
					{
						waitDel.push_back(UID);//直接删除
					}
				}
			}
			break;
		default:
			{
				if(data->NeedDel(now))
				{
					waitDel.push_back(UID);
				}
			}
			break;
		}

	}

	waitProfit.MakeProfit();

	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		DelGuildAuctData(waitDel[i]);
	}

	AuctionToWroldEx();
}

void GuildAuctMgr::FillActTypeList(UINT64 guildID, int actType, KKSG::GuildAuctReqRes& roRes, UINT64 roleID)
{
	GAPAIR_KEY gakey(guildID, actType);
	GAGuildActType profit(gakey);
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		auto data = it->second;
		if(data->GetGuildID() != guildID)
		{
			continue;
		}
		if(0 != actType && data->GetActType() != actType)
		{
			continue;
		}

		if (0 != actType && data->GetPairKey() == gakey && (data->GetSaleType() == GAST_GUILD_AUCT_ING || data->GetSaleType() == GAST_GUILD_BUY_NOW_UNPROF))
		{
			profit.AddBaseData(data);
		}

		if (data->GetSaleType() != GAST_GUILD_AUCT_ING && data->GetSaleType() != GAST_GUILD_BUY_NOW_UNPROF)
		{
			continue;
		}
		if (roRes.saleitems_size() >= GuildAuctClientSendCount && data->GetAuctRoleID() != roleID)
		{
			continue;
		}

		data->FillClientOnSaleData(*roRes.add_saleitems(), roleID);
	}

	if(0 != actType)
	{
		roRes.set_profit(profit.GetRoleProfit(roleID));
	}
}

void GuildAuctMgr::FillItemTypeList(int itemType, KKSG::GuildAuctReqRes& roRes, UINT64 roleID)
{
	std::multimap<UINT32,GABaseData*,std::greater<UINT32>> price_items;
	for(auto it =  m_allData.begin(); it != m_allData.end(); ++it)
	{
		auto data = it->second;
		if(data->GetSaleType() != GAST_WORLD_AUCT_ING)
		{
			continue;
		}
		if(0 != itemType && GuildAuctConfig::Instance()->GetItemType(data->GetItemID()) != itemType)
		{
			continue;
		}
		price_items.insert(std::make_pair(data->GetAuctPrice(),data));	
	}

	auto it = price_items.begin();
	auto it_end = price_items.end();
	for (; it != it_end; ++it)
	{
		auto data = it->second;
		if (roRes.saleitems_size() >= GuildAuctClientSendCount && data->GetAuctRoleID() != roleID)
		{
			break;
		}

		data->FillClientOnSaleData(*roRes.add_saleitems(), roleID);
	}
}

void GuildAuctMgr::FillGuildSaleHistory(KKSG::GuildAuctReqRes& roRes, UINT64 guildID)
{
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		auto data = (it->second);
		if(data->GetGuildID() != guildID)
		{
			continue;
		}
		if(data->GetSaleType() == GAST_GUILD_AUCT_ING)
		{
			continue;
		}
		if(roRes.salehistorys_size() >= GuildAuctClientSendCount)
		{
			continue;
		}

		data->FillClientSaleHistoryData(*roRes.add_salehistorys(), true);
	}
}

void GuildAuctMgr::FillWorldSaleHistory(KKSG::GuildAuctReqRes& roRes)
{
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		auto data = (it->second);
		if(!data->IsWorldSaled())
		{
			continue;
		}
		if(roRes.salehistorys_size() >= GuildAuctClientSendCount)
		{
			continue;
		}

		data->FillClientSaleHistoryData(*roRes.add_salehistorys());
	}
}

void GuildAuctMgr::TestClear()
{
	SSDebug << __FUNCTION__ << " size = " << m_allData.size() << END;
	std::vector<UINT64> waitDel;
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		waitDel.push_back(it->first);
	}
	for(size_t i = 0; i < waitDel.size(); ++i)
	{
		DelGuildAuctData(waitDel[i]);
	}
}

void GuildAuctMgr::TestPrint()
{
	SSDebug << __FUNCTION__ << " size = " << m_allData.size() << END;
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		(it->second)->TestPrint();
	}

	//for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	//{
	//	std::string sst;
	//	KKSG::GASaleItem tmp;
	//	(it->second)->FillClientOnSaleData(tmp);
	//	tmp.SerializeToString(&sst);
	//	SSDebug << " client size = " << sst.size() << END;
	//}

	//for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	//{
	//	std::string sst;
	//	KKSG::GASaleHistory tmp;
	//	(it->second)->FillClientSaleHistoryData(tmp);
	//	tmp.SerializeToString(&sst);
	//	SSDebug << " history size = " << sst.size() << END;
	//}
}

void GuildAuctMgr::AddGuildAuctDBData(KKSG::GAItemDBData& itemDBData)
{
	GABaseData* baseData = new GABaseData(itemDBData);
	if(NULL == baseData)
	{
		return ;
	}

	UpdateUID(itemDBData.uid());
	AddUniOne(baseData);
}

void GuildAuctMgr::AddRoleProfit(UINT64 roleID, UINT32 act_type,UINT32 price)
{
	auto it = m_profitData.find(roleID);
	if (it != m_profitData.end())
	{
		it->second[act_type] += price;
	}
	else
	{
		std::unordered_map<UINT32,UINT32> type_price;
		type_price.insert(std::make_pair(act_type,price));
		m_profitData[roleID] = type_price;
	}
}

UINT32 GuildAuctMgr::GetRoleProfit(UINT64 roleID,UINT32 act_type)
{
	auto it = m_profitData.find(roleID);
	if(it != m_profitData.end())
	{
		auto type_it = (it->second).find(act_type);
		if (type_it != (it->second).end())
		{
			return it->second[act_type];
		}
		return  0;
	}

	return 0;
}

void GuildAuctMgr::CheckProfitData()
{
	time_t now = GameTime::GetTime();
	if(!XCommon::IsDaySame(m_lastUpProfitDataTime, now))
	{
		m_lastUpProfitDataTime = now;
		m_profitData.clear();
	}
}

bool GuildAuctMgr::HaveGuildSale(UINT64 guildID)
{
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		auto data = it->second;
		if(data->GetGuildID() == guildID && data->GetSaleType() == GAST_GUILD_AUCT_ING)
		{
			return true;
		}
	}
	return false;
}

void GuildAuctMgr::OnRoleLogin(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	UINT64 gid = CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID());
	if(HaveGuildSale(gid))
	{
		HintNotify(pRole);
	}
}

void GuildAuctMgr::HintNotify(CRole* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	PtcM2C_HintNotifyMS ptc;
	ptc.m_Data.set_isremove(false);
	ptc.m_Data.add_systemid(SYS_AUCTION);
	pRole->Send(ptc);
	SSDebug << __FUNCTION__ << " role = " << pRole->GetID() << END;
}

void GuildAuctMgr::TLogAddItem(GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	TGuildAuctionFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.SetData(data);
	if(data->GetSaleType() == GAST_GUILD_AUCT_ING)//在公会拍卖
	{
		oLog.m_iOperType = TX_GuildAuctionStart;
	}
	else if(data->GetSaleType() == GAST_WORLD_AUCT_ING)//流拍到世界
	{
		oLog.m_iOperType = TX_WorldAuctionStart;
	}
	oLog.Do();
}

void GuildAuctMgr::TLogBuy(UINT64 roleID, GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	TGuildAuctionFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.SetData(data);
	if(data->GetSaleType() == GAST_GUILD_AUCT_ING)//在公会中竞拍
	{
		oLog.m_iOperType = TX_GuildBidding;
	}
	else if(data->GetSaleType() == GAST_WORLD_AUCT_ING)//在世界中竞拍
	{
		oLog.m_iOperType = TX_WorldBidding;
	}
	else if(data->GetSaleType() == GAST_GUILD_BUY_NOW_UNPROF)//在公会中一口价
	{
		oLog.m_iOperType = TX_GuildBuyNow;
	}
	else if(data->GetSaleType() == GAST_WORLD_BUY_NOW)//在世界一口价
	{
		oLog.m_iOperType = TX_WorldBuyNow;
	}
	oLog.m_uRoleID = roleID;
	oLog.Do();
}

void GuildAuctMgr::TLogMakeProfit(UINT64 guildID, UINT64 roleID, int actType, UINT32 price)
{
	TGuildAuctionFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.m_uRoleID = roleID;
	oLog.m_uGuildID = guildID;
	oLog.m_iActType = actType;
	oLog.m_iOperType = TX_MakeProfit;
	oLog.m_uProfitNum = price;
	oLog.Do();
	//price 分红最终得到的龙币
}

void GuildAuctMgr::TLogGiveItem(UINT64 roleID, GABaseData* data)
{
	if(NULL == data)
	{
		return ;
	}
	TGuildAuctionFlow oLog(TLogMgr::Instance()->GetTagString());
	oLog.SetData(data);
	oLog.m_uRoleID = roleID;
	if(data->GetSaleType() == GAST_GUILD_BUY_AUCT)//公会竞拍给物品
	{
		oLog.m_iOperType = TX_GuildBiddingDone;
		oLog.Do();
	}
	else if(data->GetSaleType() == GAST_WORLD_BUY_AUCT)//世界竞拍给物品
	{
		oLog.m_iOperType = TX_WorldBiddingDone;
		oLog.Do();
	}
	else if(data->GetSaleType() == GAST_GUILD_BUY_NOW_UNPROF)//公会一口价给物品
	{
		//一口价之前已经有记录，不需要在给物品的时候再记录;
	}
	else if(data->GetSaleType() == GAST_WORLD_BUY_NOW)//世界一口价给物品
	{
	}
}

UINT32 GuildAuctMgr::GetGuildAuctProfitMaxByActType(UINT32 act_type)
{
	auto profit_it = type_profit_max_.find(act_type);
	if (profit_it != type_profit_max_.end())
	{
		return profit_it->second;
	}

	return 0;
}

bool GuildAuctMgr::CheckItemAttachType(UINT32 act_type,UINT32 item_id)
{
	const auto & type_sitems = GuildAuctConfig::Instance()->GetCertainItems();
	auto it		=  type_sitems.find(act_type);
	auto it_end =  type_sitems.end();
	if (it != it_end)
	{
		auto it_item = it->second.begin();
		auto it_item_end = it->second.end();
		for ( ; it_item != it_item_end; ++it_item)
		{
			if (*it_item == item_id)
			{
				return true;
			}
		}
	}
	return false;
}


void  GuildAuctMgr::CheckAuctItem(GABaseData* data)
{
	auto it_act = item_auct_.find(data->GetActType());
	if (it_act != item_auct_.end())
	{
		auto it_item	 = it_act->second.find(data->GetItemID());
		auto it_item_end = it_act->second.end();
		if (it_item != it_item_end)
		{
			it_item->second.itemCount += data->GetItemCount();
		}
		else
		{
			ItemDesc item(data->GetItemID(),data->GetItemCount());
			it_act->second.insert(std::make_pair(data->GetItemID(),item));
		}
	}
	else
	{
		ItemDesc item(data->GetItemID(),data->GetItemCount());
		std::map<UINT32,ItemDesc> items;
		items.insert(std::make_pair(data->GetItemID(),item));
		item_auct_.insert(std::make_pair(data->GetActType(),items));
	}	
}
// 竞拍或者一口价物品活动结束时的给世界拍卖行增加的物品
void  GuildAuctMgr::AuctionToWroldEx()
{
	auto it = item_auct_.begin();
	auto it_end = item_auct_.end();
	for (; it != it_end; ++it)
	{
		auto it_act  = type_profit_rate_.find(it->first);
		if (it_act == type_profit_rate_.end())
		{
			continue;
		}	
		auto item_it = it->second.begin();
		auto item_it_end = it->second.end();
		for (; item_it != item_it_end; ++ item_it)
		{
			if (CheckItemAttachType(it->first,item_it->first))
			{
				UINT32  count = (UINT32) (item_it->second.itemCount * it_act->second) + 1;
				std::vector<UINT64> roles;
				time_t now = GameTime::GetTime();
				for (UINT32 index = 0; index < count ; ++index)
				{					
					UINT64 UID = NewUID();
					GABaseData* baseData = new GABaseData(UID, item_it->second.itemID, 1, it->first, 0, now, roles);	
					if(NULL == baseData)
					{
						continue;
					}
					baseData->SetSaleTypeData(GAST_WORLD_AUCT_ING, now);
					AddUniOne(baseData);
					TLogAddItem(baseData);
					GADBHandler::Add(baseData);
				}
			}
		}
	}

	item_auct_.clear();
}

// 延长商品时间
void GuildAuctMgr::DelayItemTime(UINT64 guildID, int actType)
{
	bool add = false;
	for(auto it = m_allData.begin(); it != m_allData.end(); ++it)
	{
		auto data = it->second;
		if(data->GetGuildID() == guildID && data->GetActType() == actType)
		{
			data->AddGwTime(GetGlobalConfig().GuildAuctItemDelayTime);
			add = true;
		}
	}

	if (add)
	{
		Guild * guild = CGuildMgr::Instance()->GetGuild(guildID);
		if (NULL != guild)
		{
			PtcM2C_GuildAuctItemTimeFresh notice;
			notice.m_Data.set_auct_type(actType);
			guild->BroadCast(notice);
		}
	}
}