#include "pch.h"
#include "guildbonus/rpcc2g_getguildbonusleft.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"
#include "guild/guildrecord.h"
#include "network/mslink.h"


// generate by ProtoGen at date: 2016/12/24 11:57:15

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildBonusLeft, GetGuildBonusLeftArg, GetGuildBonusLeftRes)

void RpcC2G_GetGuildBonusLeft::OnCall(const GetGuildBonusLeftArg &roArg, GetGuildBonusLeftRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = pRole->getGuildId();
	if (gid == INVALID_GUILDID)
	{
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}


	RpcG2M_MGuildBonusOperateReq* req = RpcG2M_MGuildBonusOperateReq::CreateRpc();
	req->m_sessionID = m_sessionID;
	req->m_oArg.set_rpcid(DelayRpc());
	req->m_oArg.mutable_leftbonusarg()->CopyFrom(roArg);
	req->m_oArg.set_optype(GBonus_OP_GetBonusLeft);
	pRole->Get<CGuildRecord>()->Convert2_KKSGGuildRecord(req->m_oArg.mutable_roleguildrecord());
	pRole->SendToMS(*req);
}

void RpcC2G_GetGuildBonusLeft::OnDelayReplyRpc(const GetGuildBonusLeftArg &roArg, GetGuildBonusLeftRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode(KKSG::ERR_FAILED);
	if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
		return;
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
	const GetGuildBonusLeftRes* res = (GetGuildBonusLeftRes*)roUserData.m_pUserPtr;
	roRes.CopyFrom(*res);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}
