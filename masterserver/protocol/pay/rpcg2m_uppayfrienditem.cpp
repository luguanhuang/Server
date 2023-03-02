#include "pch.h"
#include "pay/rpcg2m_uppayfrienditem.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "httptask/paybuygoods.h"
#include "httptask/httptaskmgr.h"
#include "role/rolesummarymgr.h"
#include "loghelper/tlogr.h"
#include "util/gametime.h"
#include "pay/paymgr.h"
#include "pay/ibgiftmgr.h"
#include "friend/friendmgr.h"
#include "friend/friend.h"

// generate by ProtoGen at date: 2017/3/14 10:36:09

RPC_SERVER_IMPLEMETION(RpcG2M_UpPayFriendItem, UpPayFriendItemArg, UpPayFriendItemRes)

void RpcG2M_UpPayFriendItem::OnCall(const UpPayFriendItemArg &roArg, UpPayFriendItemRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.fromroleid());
	if (NULL == role)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn << "role is NULL and roleid is :" << roArg.fromroleid() <<END;
		return;
	}

	LogInfo("roleid=%llu,toroleid=%llu,itemid=%u,count=%u,goodsid=%s\n", roArg.fromroleid(), roArg.toroleid(), roArg.itemid(), 
		roArg.count(), roArg.goodsid().c_str());
	CRoleSummary* pSum =  CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.toroleid());
	if (pSum == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_IBBUY);
		SSWarn << "invalid toroleid" << roArg.toroleid() <<END;
		return;
	}

	Friend* info = FriendMgr::Instance()->GetFriendInfo(role->GetID());
	if (info == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_IBGIFT_FRIEND);
		return;
	}
	UINT32 nDegree = info->GetDegree(roArg.toroleid());
	if (GetGlobalConfig().IBShopDegree>0&& nDegree < GetGlobalConfig().IBShopDegree)
	{
		roRes.set_errorcode(KKSG::ERR_IBGIFT_FRIEND);
		return;
	}

	if (IBGiftMgr::Instance()->GetUnRecvGift(roArg.toroleid())>GetGlobalConfig().IBOverMaxUnRecv)
	{
		roRes.set_errorcode(KKSG::ERR_IBGIFT_OVERMAXUNRECV);
		return;
	}


	std::string billno = TLogMgr::Instance()->GetTagString();
	KKSG::PayGiftIbItem *pItem = new KKSG::PayGiftIbItem();
	pItem->set_billno(billno);
	pItem->set_fromroleid(roArg.fromroleid());
	pItem->set_toroleid(roArg.toroleid());
	pItem->set_goodsid(roArg.goodsid());
	pItem->set_itemid(roArg.itemid());
	pItem->set_count(roArg.count());
	pItem->set_ts(UINT32(GameTime::GetTime()));
	pItem->set_status(KKSG::PayGiftIbStatus_CreateBill);
	pItem->set_text(roArg.text());
	pItem->set_paydegree(roArg.price()*roArg.count());

	if (role->GetLoginPlatID() == KKSG::PLAT_IOS)
	{	
		IBGiftMgr::Instance()->AddIbItem(roArg.fromroleid(), pItem);
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		roRes.set_billno(billno);
		return ;
	}

	role->SetPayParameterInfo(roArg.payparam());
	PayBuyGoodsTask* pTask = new PayBuyGoodsTask();
	pTask->SetParamData(roArg.payparam());
	pTask->SetPayItem(*pItem, roArg.price());
	pTask->SetGoodsMeta(roArg.fromroleid(), roArg.name(), roArg.desc());
	pTask->SetRpcId(DelayRpc());
	CPayMgr::Instance()->BuildHttpParam(role, pTask->m_oparams, PayTypeBuyGoods);
	pTask->m_oparams["pf"] = pTask->m_oparams["pf"] + "-" + billno;
	CPayMgr::Instance()->BuildSign(role, pTask->m_oparams, BuyGoodsUrlPath, PayTypeBuyGoods);
	HttpTaskMgr::Instance()->AddTask(pTask);
}

void RpcG2M_UpPayFriendItem::OnDelayReplyRpc(const UpPayFriendItemArg &roArg, UpPayFriendItemRes &roRes, const CUserData &roUserData)
{
	if(roUserData.m_pUserPtr == NULL || roUserData.m_dwUserData != 0)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_IBBUY);
	}
	else
	{
		KKSG::PayFriendItemRes* pData = (KKSG::PayFriendItemRes*)roUserData.m_pUserPtr;
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
		roRes.set_token(pData->token());
		roRes.set_url_param(pData->url_param());
		roRes.set_billno(pData->billno());
	}
}
