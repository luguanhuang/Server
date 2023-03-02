#include "pch.h"
#include "partner/rpcc2g_takepartnerchest.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolepartner.h"
#include "partner/rpcg2m_getpartnerinfog2m.h"
#include "gamelogic/bagtransition.h"
#include "tablemgr/partnertablemgr.h"
#include "config/itemdropconfig.h"

// generate by ProtoGen at date: 2016/12/10 21:03:27

RPC_SERVER_IMPLEMETION(RpcC2G_TakePartnerChest, TakePartnerChestArg, TakePartnerChestRes)

void RpcC2G_TakePartnerChest::OnCall(const TakePartnerChestArg &roArg, TakePartnerChestRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	RolePartner* partner = pRole->Get<RolePartner>(); 
	if (partner->GetPartnerID() == 0)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	if (partner->IsChestTaked(roArg.index()))
	{
		roRes.set_result(KKSG::ERR_PARTNER_CHEST_TAKED);
		return;
	}

	RpcG2M_GetPartnerInfoG2M* rpc = RpcG2M_GetPartnerInfoG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendToMS(*rpc);
}

void RpcC2G_TakePartnerChest::OnDelayReplyRpc(const TakePartnerChestArg &roArg, TakePartnerChestRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	GetPartnerInfoG2MRes* res = (GetPartnerInfoG2MRes*)roUserData.m_pUserPtr;

	RolePartner* partner = pRole->Get<RolePartner>(); 
	partner->SetPartnerID(res->partnerid());
	if (partner->IsChestTaked(roArg.index()))
	{
		roRes.set_result(KKSG::ERR_PARTNER_CHEST_TAKED);
		return;
	}
	if (res->partnerid() == 0)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}

	auto data = PartnerTableMgr::Instance()->GetLivenessTable(res->level(), roArg.index());
	if (data == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	// 活跃度不足
	if (res->liveness() < data->liveness)
	{
		roRes.set_result(KKSG::ERR_PARTNER_LN_NOT_ENOUGH);
		return;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);

	// 发送奖励
	std::vector<ItemDesc> vecItem;
	for (auto i = 0;  i < data->dropid.size(); ++i)
	{
		UINT32 dropId = data->dropid[i];
		const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropId, pRole->GetLevel());
		if (NULL == pDrop)
		{
			continue;
		}
		ItemDesc stItem(*pDrop);
		vecItem.push_back(stItem);
	}

	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_Partner, ItemFlow_PartnerChest);
	for (auto i = vecItem.begin(); i != vecItem.end(); ++i)
	{
		stTransition.GiveItem(*i);
		roRes.add_itemid(i->itemID);
		roRes.add_itemcount(i->itemCount);
	}
	stTransition.NotifyClient();

	partner->SetChestTaked(roArg.index());
	roRes.set_takedchest(partner->GetChestTakeInfo());
	partner->CheckChest(res->level(), res->liveness());

	LogInfo("[%llu] take partner liveness chest[%u]", pRole->GetID(), roArg.index());
}
