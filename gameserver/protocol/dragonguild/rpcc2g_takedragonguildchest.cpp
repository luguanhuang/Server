#include "pch.h"
#include "dragonguild/rpcc2g_takedragonguildchest.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguildrecord.h"
#include "dragonguild/rpcg2m_getdragonguildinfog2m.h"
#include "gamelogic/bagtransition.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "config/itemdropconfig.h"


// generate by ProtoGen at date: 2017/9/5 14:48:30

RPC_SERVER_IMPLEMETION(RpcC2G_TakeDragonGuildChest, TakePartnerChestArg, TakePartnerChestRes)

void RpcC2G_TakeDragonGuildChest::OnCall(const TakePartnerChestArg &roArg, TakePartnerChestRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	
	DragonGuildRecord* dragonDg = pRole->Get<DragonGuildRecord>(); 
	if ( DragonGuildSimpleMgr::Instance()->GetDragonGuildId(pRole->GetID()) == 0)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	if (dragonDg->IsChestTaked(roArg.index()))
	{
		roRes.set_result(KKSG::ERR_PARTNER_CHEST_TAKED);
		return;
	}

	RpcG2M_GetDragonGuildInfoG2M* rpc = RpcG2M_GetDragonGuildInfoG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendToMS(*rpc);
	
}

void RpcC2G_TakeDragonGuildChest::OnDelayReplyRpc(const TakePartnerChestArg &roArg, TakePartnerChestRes &roRes, const CUserData &roUserData)
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

	DragonGuildRecord* dragonDg = pRole->Get<DragonGuildRecord>(); 
	if (dragonDg->IsChestTaked(roArg.index()))
	{
		roRes.set_result(KKSG::ERR_PARTNER_CHEST_TAKED);
		return;
	}
	if (res->partnerid() == 0)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}

	DragonGuildLivenessTable::RowData* data = DragonGuildTableMgr::Instance()->GetLivenessTable(res->level(), roArg.index());
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

	dragonDg->SetChestTaked(roArg.index());
	roRes.set_takedchest(dragonDg->GetChestTakeInfo());
	dragonDg->CheckChest(res->level(), res->liveness());

	LogInfo("[%llu] take partner liveness chest[%u]", pRole->GetID(), roArg.index());
}
