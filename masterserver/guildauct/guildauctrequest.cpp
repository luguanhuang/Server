#include "pch.h"
#include "guildauctrequest.h"
#include "table/guildauctconfig.h"
#include "role/role.h"
#include "guild/guildmgr.h"
#include "guildauctmgr.h"
#include "table/globalconfig.h"
#include "notice/noticemgr.h"
#include "guildauctbase.h"
#include "table/ItemConfigMgr.h"
#include "role/rolemanager.h"
#include "define/maildef.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "auction/rpcm2g_guildauctfailback.h"
#include "network/gslink.h"
#include "common/ptcm2c_mserrornotify.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
GuildAuctRequest::GuildAuctRequest()
{

}

GuildAuctRequest::GuildAuctRequest(CRole* role)
{
	m_pRole = role;
}

int GuildAuctRequest::HandleActList(int acttype, KKSG::GuildAuctReqRes& roRes)
{
	UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID());
	if(0 == guildID)
	{
		return KKSG::ERR_SUCCESS;
	}
	
	GuildAuctMgr::Instance()->FillActTypeList(guildID, acttype, roRes, m_pRole->GetID());	

	for(int i = 0; i < roRes.saleitems_size(); ++i)
	{
		SSDebug << __FUNCTION__ << " UID = " << roRes.saleitems(i).uid() << " itemid = " << roRes.saleitems(i).itemid() << " auctprice = " << roRes.saleitems(i).curauctprice() << " maxprice = " << roRes.saleitems(i).maxprice() << END;
	}
	SSDebug << __FUNCTION__ << " profit = " << roRes.profit() << END;

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleItemList(int itemtype, KKSG::GuildAuctReqRes& roRes)
{
	GuildAuctMgr::Instance()->FillItemTypeList(itemtype, roRes, m_pRole->GetID());

	for(int i = 0; i < roRes.saleitems_size(); ++i)
	{
		SSDebug << __FUNCTION__ << " UID = " << roRes.saleitems(i).uid() << " itemid = " << roRes.saleitems(i).itemid() << " auctprice = " << roRes.saleitems(i).curauctprice() << " maxprice = " << roRes.saleitems(i).maxprice() << END;
	}

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleBuyAuctPre(UINT64 UID, UINT32 curAuctPrice, UINT32& price, KKSG::GuildAuctReqRes& roRes) 
{
	auto data = GuildAuctMgr::Instance()->GetGuildAuctData(UID);

	if(NULL == data || data->IsSaled())
	{
		return KKSG::ERR_AUCT_ITEMSALED;
	}

	if(data->GetAuctPrice() != curAuctPrice)
	{
		roRes.set_curauctprice(data->GetAuctPrice());
		return KKSG::ERR_AUCT_PRICECHAGE;
	}

	price = data->GetAuctUpPrice();
	if(price >= data->GetMaxPrice())
	{
		return KKSG::ERR_FAILED;//超过一口价
	}

	if(data->IsLocked())
	{
		return KKSG::ERR_AUCT_ITEM_LOCK;//被其它玩家锁定
	}

	data->SetLock();	

	SSDebug << __FUNCTION__ << " lockuid = " << UID << END;

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleBuyAuctAft(UINT64 UID, KKSG::GuildAuctReqRes& roRes)
{
	auto data = GuildAuctMgr::Instance()->GetGuildAuctData(UID);

	if(NULL == data)
	{
		SSError << " find err UID = " << UID << END;
		return KKSG::ERR_FAILED;
	}

	//ClearLock(UID);

	if(data->HaveAucted())
	{
		AuctFailNotify(data);
	}
	
	data->SetAuctData(m_pRole->GetID(), data->GetAuctUpPrice());

	GuildAuctMgr::Instance()->TLogBuy(m_pRole->GetID(), data);

	roRes.set_curauctprice(data->GetAuctPrice());
	time_t now = GameTime::GetTime();
	if (data->GetSaleLeftTime((UINT32)now) < GetGlobalConfig().GuildAuctItemDelayTime)
	{
		GuildAuctMgr::Instance()->DelayItemTime(data->GetGuildID(), data->GetActType());		
	}

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleBuyNowPre(UINT64 UID, UINT32& price)
{
	auto data = GuildAuctMgr::Instance()->GetGuildAuctData(UID);

	if(NULL == data || data->IsSaled())
	{
		return KKSG::ERR_AUCT_ITEMSALED;// 已出售
	}

	if(data->GetSaleType() == GAST_GUILD_AUCT_ING)
	{
		UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID());
		if(data->GetGuildID() != guildID)
		{
			return KKSG::ERR_FAILED;
		}
	}

	if(data->IsLocked())
	{
		return KKSG::ERR_AUCT_ITEM_LOCK;//被其它玩家锁定
	}

	data->SetLock();	

	SSDebug << __FUNCTION__ << " lockuid = " << UID << END;

	price = data->GetMaxPrice();

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleBuyNowAft(UINT64 UID)
{
	auto data = GuildAuctMgr::Instance()->GetGuildAuctData(UID);

	if(NULL == data)
	{
		SSError << " find err UID = " << UID << END;
		return KKSG::ERR_FAILED;
	}

	//ClearLock(UID);

	if(data->HaveAucted())
	{
		AuctFailNotify(data);

		data->SetAuctData(0, data->GetAuctPrice());//auct role ---> 0
	}

	time_t now = GameTime::GetTime();

	if(data->GetSaleType() == GAST_GUILD_AUCT_ING)
	{
		BuyNowGuildNotice(data);	
		data->SetSaleTypeData(GAST_GUILD_BUY_NOW_UNPROF, now);
	}
	else
	{
		data->SetSaleTypeData(GAST_WORLD_BUY_NOW, now);
	}

	GuildAuctMgr::Instance()->TLogBuy(m_pRole->GetID(), data);

	AuctSuccessMail(m_pRole->GetID(), data->GetItemID(), data->GetItemCount(), true);
	GuildAuctMgr::Instance()->TLogGiveItem(m_pRole->GetID(), data);

	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleAuctGuildHistory(KKSG::GuildAuctReqRes& roRes)
{
	UINT64 guildID = CGuildMgr::Instance()->GetGuildIdByRoleId(m_pRole->GetID());
	if(0 == guildID)
	{
		return KKSG::ERR_SUCCESS;
	}

	GuildAuctMgr::Instance()->FillGuildSaleHistory(roRes, guildID);

	for(int i = 0; i < roRes.salehistorys_size(); ++i)
	{
		SSDebug << __FUNCTION__ << " UID = " << roRes.salehistorys(i).itemid() << " price = " << roRes.salehistorys(i).saleprice() << END;
	}
	
	return KKSG::ERR_SUCCESS;
}

int GuildAuctRequest::HandleAuctWorldHistory(KKSG::GuildAuctReqRes& roRes)
{

	GuildAuctMgr::Instance()->FillWorldSaleHistory(roRes);

	for(int i = 0; i < roRes.salehistorys_size(); ++i)
	{
		SSDebug << __FUNCTION__ << " UID = " << roRes.salehistorys(i).itemid() << " price = " << roRes.salehistorys(i).saleprice() << END;
	}

	return KKSG::ERR_SUCCESS;
}

void GuildAuctRequest::BuyNowGuildNotice(GABaseData* baseData)
{
	if(NULL == baseData)
	{
		return ;
	}
	NoticeMgr::Instance()->NoticeGuildAuctBuyNow(baseData->GetGuildID(), 
		m_pRole->GetName(), baseData->GetMaxPrice(), CItemConfig::Instance()->GetItemName(baseData->GetItemID()));	
}

void GuildAuctRequest::AuctFailNotify(GABaseData* baseData)
{
	if(NULL == baseData)
	{
		return ;
	}
	UINT64 roleID = baseData->GetAuctRoleID();
	if(0 == roleID)
	{
		return;
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
	if(pRole)
	{
		PtcM2C_MSErrorNotify ptc;
		ptc.m_Data.set_errorno(KKSG::ERR_AUCT_AUCTOVER);
		pRole->Send(ptc);

		RpcM2G_GuildAuctFailBack* rpc = RpcM2G_GuildAuctFailBack::CreateRpc();
		rpc->m_oArg.set_roleid(roleID);
		rpc->m_oArg.set_itemid(baseData->GetItemID());
		rpc->m_oArg.set_price(baseData->GetAuctPrice());
		pRole->SendMsgToGS(*rpc);
	}
	else
	{
		// 邮件通知
		AuctFailMailBack(roleID, baseData->GetItemID(),baseData->GetAuctPrice());
	}
}

void GuildAuctRequest::AuctFailMailBack(UINT64 roleID, UINT32 itemID,UINT32 price)
{
	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_GuildAuct_Fail);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent, "%s", CItemConfig::Instance()->GetItemName(itemID));
	}
	std::vector<ItemDesc> rewards;
	rewards.push_back(ItemDesc(DRAGON_COIN, price));

	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_GuildAuctFailBack);
	CMailMgr::Instance()->SendMail(roleID, maildata);	

}
void GuildAuctRequest::AuctSuccessMail(UINT64 roleID, UINT32 itemID, UINT32 itemCount, bool isBuyNow)
{
	std::string mailtitle = "null";
	std::string mailcontent;
	int MailConfID = MailConf_GuildAuct_BuyAuct_Success;
	if(isBuyNow)
	{
		MailConfID = MailConf_GuildAuct_BuyNow_Success;
	}
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConfID);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent, "%s", CItemConfig::Instance()->GetItemName(itemID));
	}
	std::vector<ItemDesc> rewards;
	rewards.push_back(ItemDesc(itemID, itemCount));
	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_GuildAuct_Give);
	CMailMgr::Instance()->SendMail(roleID, maildata);
}

void GuildAuctRequest::AuctProfitMail(UINT64 roleID, UINT64 guildID, int actType, UINT32 price)
{
	SSDebug << __FUNCTION__ << " role = " << CRoleSummaryMgr::Instance()->GetRoleName(roleID) << " guild = " << guildID 
		<< " acttype = " << actType << " price = " << price << END; 

	UINT32 ConfProfitMax = GuildAuctMgr::Instance()->GetGuildAuctProfitMaxByActType(actType);
	UINT32 HaveProfit = GuildAuctMgr::Instance()->GetRoleProfit(roleID,actType);
	if(HaveProfit >= ConfProfitMax)
	{
		AuctProfitFailMail(roleID,actType);
		return ;
	}

	UINT32 myPrice = price;
	if(myPrice > ConfProfitMax - HaveProfit)
	{
		myPrice = ConfProfitMax - HaveProfit;
	}

	GuildAuctMgr::Instance()->AddRoleProfit(roleID, actType,myPrice);

	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_GuildAuct_Profit_Normal);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent, "%s", GuildAuctConfig::Instance()->GetActName(actType));
		XCommon::ReplaceOneStr(mailcontent, "%d", ToString(ConfProfitMax));
	}
	std::vector<ItemDesc> rewards;
	rewards.push_back(ItemDesc(DRAGON_COIN, myPrice));
	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	maildata.SetReason(ItemFlow_GuildAuctProfit);
	CMailMgr::Instance()->SendMail(roleID, maildata);

	GuildAuctMgr::Instance()->TLogMakeProfit(guildID, roleID, actType, myPrice);

	EventMgr::Instance()->AddEventOnlyOnMS(roleID, MENTOR_EVENT_OFFSET + KKSG::MentorTask_GuildAuctBenefit, 0, 1);
}


void GuildAuctRequest::AuctProfitFailMail(UINT64 roleID,UINT32 act_type)
{	
	std::string mailtitle = "null";
	std::string mailcontent;
	const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(Mailconf_GuildAuct_Profit_Overflow);
	if(pMailConf)
	{
		mailtitle = pMailConf->m_title;
		mailcontent = pMailConf->m_content;
		XCommon::ReplaceOneStr(mailcontent, "%s", GuildAuctConfig::Instance()->GetActName(act_type));
	}
	std::vector<ItemDesc> rewards;
	MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(roleID, maildata);
}

void GuildAuctRequest::ClearLock(UINT64 UID)
{
	SSDebug << __FUNCTION__ << " clear lockuid = " << UID << END;
	auto data = GuildAuctMgr::Instance()->GetGuildAuctData(UID);

	if(NULL == data)
	{
		SSError << " find err UID = " << UID << END;
		return ;
	}

	data->ClearLock();
}

void GuildAuctRequest::HandleCommonData(const KKSG::GuildAuctReqArg& roArg, KKSG::GuildAuctReqRes& roRes)
{
	//处理 请求拍卖和一口价购买 无论成功还是失败都返回当前分红
	if(roArg.reqtype() == KKSG::GART_BUY_AUCT || roArg.reqtype() == KKSG::GART_BUY_NOW)
	{
		if(roArg.acttype() != 0)
		{
			HandleActList(roArg.acttype(), roRes);				
		}
	}
}
