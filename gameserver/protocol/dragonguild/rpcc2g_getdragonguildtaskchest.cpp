#include "pch.h"
#include "dragonguild/rpcc2g_getdragonguildtaskchest.h"
#include "dragonguild/rpcg2m_getdragonguildtaskchestg2m.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "dragonguild/dragonguildrecord.h"
#include "gamelogic/bagtransition.h"
#include "tablemgr/dragonguildtablemgr.h"
#include "config/itemdropconfig.h"
#include "dragonguild/ptcg2m_costdragonguildreceivecount.h"
#include "dragonguild/dragonguildmgr.h"
// generate by ProtoGen at date: 2017/9/11 11:15:45

RPC_SERVER_IMPLEMETION(RpcC2G_GetDragonGuildTaskChest, GetDragonGuildTaskChestArg, GetDragonGuildTaskChestRes)

void RpcC2G_GetDragonGuildTaskChest::OnCall(const GetDragonGuildTaskChestArg &roArg, GetDragonGuildTaskChestRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	DragonGuildRecord* record = pRole->Get<DragonGuildRecord>(); 
	UINT64 guildId = DragonGuildSimpleMgr::Instance()->GetDragonGuildId(pRole->GetID());
	if (guildId == 0)
	{
		roRes.set_errorcode(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}

	KKSG::ErrorCode errorCode = record->GetTaskStatus(roArg.type(), roArg.taskid());
	if (errorCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode(errorCode);
		return;
	}
	
	//master检查小分队领取次数, 是否可领取等条件
	RpcG2M_GetDragonGuildTaskChestG2M* rpc = RpcG2M_GetDragonGuildTaskChestG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_type(roArg.type());
	rpc->m_oArg.set_taskid(roArg.taskid());
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendToMS(*rpc);
}

void RpcC2G_GetDragonGuildTaskChest::OnDelayReplyRpc(const GetDragonGuildTaskChestArg &roArg, GetDragonGuildTaskChestRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;
	}
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	GetDragonGuildTaskChestG2MRes* res = (GetDragonGuildTaskChestG2MRes*)roUserData.m_pUserPtr;
	if(res->errorcode() != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode(res->errorcode());
		return;
	}
	DragonGuildRecord* record = pRole->Get<DragonGuildRecord>(); 
	KKSG::ErrorCode errorCode = record->GetTaskStatus(roArg.type(), roArg.taskid());
	if (errorCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode(errorCode);
		return;
	}
	if (res->dragonguildid() == 0)
	{
		roRes.set_errorcode(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	std::vector<UINT32>dropList;
	dropList.clear();
	dropList = DragonGuildTableMgr::Instance()->GetTaskChestList(roArg.type(), roArg.taskid());
	if (dropList.size() == 0)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	// 发送奖励
	std::vector<ItemDesc> vecItem;
	for (auto i = 0;  i < dropList.size(); ++i)
	{
		UINT32 dropId = dropList[i];
		const ItemDesc* pDrop = ItemDropConfig::Instance()->GetRandomDrop(dropId, pRole->GetLevel());
		if (NULL == pDrop)
		{
			continue;
		}
		ItemDesc stItem(*pDrop);
		vecItem.push_back(stItem);
	}

	BagGiveItemTransition stTransition(pRole);
	stTransition.SetReason(ItemFlow_DragonGuild, ItemFlow_DragonGuildTaskChest);
	for (auto i = vecItem.begin(); i != vecItem.end(); ++i)
	{
		stTransition.GiveItem(*i);
		roRes.add_itemid(i->itemID);
		roRes.add_itemcount(i->itemCount);
	}
	roRes.set_guildexp(res->exp());
	stTransition.NotifyClient();

	record->ReceiveOneTaskChest(roArg.type(), roArg.taskid());
	
	//减少奖励次数
	if(roArg.type() == KKSG::TASK_ACHIVEMENT)
	{
		PtcG2M_CostDragonGuildReceiveCount ptc;
		ptc.m_Data.set_type(roArg.type());
		ptc.m_Data.set_taskid( roArg.taskid());
		ptc.m_Data.set_roleid(pRole->GetID());
		pRole->SendToMS(ptc);
	}
}
