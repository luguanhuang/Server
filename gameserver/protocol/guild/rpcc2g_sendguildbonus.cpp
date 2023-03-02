#include "pch.h"
#include "guild/rpcc2g_sendguildbonus.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"
#include "guild/guildrecord.h"
#include "guild/guilddef.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/6/15 11:29:10

RPC_SERVER_IMPLEMETION(RpcC2G_SendGuildBonus, SendGuildBonusArg, SendGuildBonusRes)

void RpcC2G_SendGuildBonus::OnCall(const SendGuildBonusArg &roArg, SendGuildBonusRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return ;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	RpcG2M_MGuildBonusOperateReq* req = RpcG2M_MGuildBonusOperateReq::CreateRpc();
	req->m_sessionID = m_sessionID;
	req->m_oArg.set_rpcid(DelayRpc());
	req->m_oArg.mutable_sendbonusarg()->CopyFrom(roArg);
	req->m_oArg.set_optype(GBonus_OP_GetBonusSend);
	pRole->Get<CGuildRecord>()->Convert2_KKSGGuildRecord(req->m_oArg.mutable_roleguildrecord());
	pRole->SendToMS(*req);

	//UINT32 error = pGuildBonus->SendCheckInBonus(pRole);
}

void RpcC2G_SendGuildBonus::OnDelayReplyRpc(const SendGuildBonusArg &roArg, SendGuildBonusRes &roRes, const CUserData &roUserData)
{
	if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		roRes.set_errorcode((ErrorCode)roUserData.m_dwUserData);
		return;
	}
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is out, session id:"<<m_sessionID<<END;
		return;
	}
	if (!roUserData.m_pUserPtr)
	{
		return;
	}
	const SendGuildBonusRes* res = (SendGuildBonusRes*)roUserData.m_pUserPtr;
	roRes.CopyFrom(*res);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}
