#include "pch.h"
#include "pay/rpcm2g_giftibreqgsgoods.h"
#include "ibshop/rpcm2g_giftibitemreq.h"
#include "pay/ibgiftmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "util/gametime.h"
#include "pay/ibgiftmgr.h"
#include "role/rolemanager.h"
#include "mail/mailconfig.h"
#include "mail/maildata.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"



// generate by ProtoGen at date: 2017/5/5 13:37:55

RPC_CLIENT_IMPLEMETION(RpcM2G_GiftIbReqGSGoods, GiftIbReqGSGoodsArg, GiftIbReqGSGoodsRes)

void RpcM2G_GiftIbReqGSGoods::OnReply(const GiftIbReqGSGoodsArg &roArg, const GiftIbReqGSGoodsRes &roRes, const CUserData &roUserData)
{
	const KKSG::PayGiftIbItem* pItem = &(roArg.item());
	KKSG::PayGiftIbItem* pVerify =  IBGiftMgr::Instance()->GetIbItem(pItem->billno());
	if (pVerify!=NULL)
	{
		if (roRes.error() == KKSG::ERR_SUCCESS)
		{
			if (pItem->billno()==pVerify->billno())
			{
				pVerify->set_status(PayGiftIbStatus_Success);
				IBGiftMgr::Instance()->DelToGiftIBItem(pItem);
				IBGiftMgr::Instance()->Save(MYSQL_OP_UPDATE,*pVerify);
				IBGiftMgr::stIBGiftHistItem histItem;
				const std::string szToName = CRoleSummaryMgr::Instance()->GetRoleName(pItem->toroleid());
				const std::string szFromName =  CRoleSummaryMgr::Instance()->GetRoleName(pItem->fromroleid());
				histItem.name = szFromName;
				histItem.item.set_itemid(pItem->itemid());
				histItem.item.set_itemcount(pItem->count());
				histItem.nTime = GameTime::GetTime();
				IBGiftMgr::Instance()->AddHistItem(pItem->toroleid(),IBGiftMgr::eIBGIFTTARGET,histItem);
				LogInfo("RpcM2G_GiftIbReqGSGoods::OnReply =fromroleid=%llu toroleid=%llu billno=%s status=%d itemid=%d count=%d ts=%d text=%s",pVerify->fromroleid(),pVerify->toroleid(),pVerify->billno().c_str(),pVerify->status(),pVerify->itemid(),pVerify->count(),pVerify->ts(),pVerify->text().c_str());
				std::string mailtitle = "";
				std::string mailcontent = "";
				const MailConf* pMailConf = MailConfig::Instance()->GetMailConf(MailConf_GIFTIB_THANKS);
				if(pMailConf)
				{
					mailtitle = XCommon::ReplaceStr(pMailConf->m_title,"%s",szToName);
					mailcontent = pMailConf->m_content;
				}
				std::vector<ItemDesc> rewards;
				MailData maildata(mailtitle, mailcontent, rewards, MAIL_EXPIRE_TIME);
				CMailMgr::Instance()->SendMail(pItem->fromroleid(), maildata);	
			}
		}
	}

	UINT32 nRpcID =roArg.rpcid();
	CUserData oUser;
	GiftIbReqGoodsRes res;
	res.set_error(roRes.error());
	oUser.m_dwUserData = roRes.error();
	oUser.m_pUserPtr = (void*)&res;
	CRpc::ReplyDelayRpc(nRpcID, oUser);
}

void RpcM2G_GiftIbReqGSGoods::OnTimeout(const GiftIbReqGSGoodsArg &roArg, const CUserData &roUserData)
{
	CUserData oUserData;
	oUserData.m_dwUserData = ERR_TIMEOUT;
	oUserData.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUserData);
}
