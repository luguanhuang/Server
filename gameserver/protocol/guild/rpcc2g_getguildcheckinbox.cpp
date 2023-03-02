#include "pch.h"
#include "guild/rpcc2g_getguildcheckinbox.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "guild/guildrecord.h"
#include "unit/rolemanager.h"
#include "gamelogic/bagtransition.h"
#include "guild/rpcg2m_notifycheckinbox.h"
#include "mslink.h"

// generate by ProtoGen at date: 2015/10/8 14:12:43

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildCheckinBox, GetGuildCheckinBoxArg, GetGuildCheckinBoxRes)

void RpcC2G_GetGuildCheckinBox::OnCall(const GetGuildCheckinBoxArg &roArg, GetGuildCheckinBoxRes &roRes)
{

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	CGuildRecord* record = role->Get<CGuildRecord>();
	record->Update();
	
	UINT32 index = roArg.index();
	if (record->IsBoxTaken(index))
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_TAKEN);
		return;
	}

	RpcG2M_NotifyCheckinBox* pMsg = RpcG2M_NotifyCheckinBox::CreateRpc();
	pMsg->m_oArg.set_roleid(role->GetID());
	pMsg->m_oArg.set_index(roArg.index());
	pMsg->m_oArg.set_rpcid(DelayRpc());
	MSLink::Instance()->Send(MSLink::Instance()->GetConnID(), *pMsg);
	return;
}

void RpcC2G_GetGuildCheckinBox::OnDelayReplyRpc(const GetGuildCheckinBoxArg &roArg, GetGuildCheckinBoxRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData != ERR_SUCCESS)
	{
		roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
		return ;
	}

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	CGuildRecord* record = role->Get<CGuildRecord>();
	if (record->IsBoxTaken(roArg.index()))
	{
		roRes.set_errorcode(ERR_GUILDCHECKIN_TAKEN);
		return;
	}

	record->SetBoxMask(roArg.index());

	BagGiveItemTransition transition(role);
	transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Checkin);

	std::map<UINT32, UINT32>& itemMap = *((std::map<UINT32, UINT32>*)roUserData.m_pUserPtr); 
	for (auto i = itemMap.begin(); i != itemMap.end(); ++i)
	{
		transition.GiveItem(i->first, i->second);
	}
	transition.NotifyClient();

	roRes.set_errorcode(ERR_SUCCESS);
}
