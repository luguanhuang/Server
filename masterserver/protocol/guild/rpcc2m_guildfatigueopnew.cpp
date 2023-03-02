#include "pch.h"
#include "guild/rpcc2m_guildfatigueopnew.h"
#include "guild/rpcm2g_reqrecvfatigue.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildfatigue.h"

// generate by ProtoGen at date: 2016/9/19 10:29:50

RPC_SERVER_IMPLEMETION(RpcC2M_GuildFatigueOPNew, GuildFatigueArg, GuildFatigueRes)

void RpcC2M_GuildFatigueOPNew::OnCall(const GuildFatigueArg &roArg, GuildFatigueRes &roRes)
{
	roRes.set_result(KKSG::ERR_GUILD_NOT_IN_GUILD);

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
	if (gid == INVALID_GUILDID)
	{
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuild(gid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}
	//////////////////////
	if (roArg.optype() == 1)
	{
		RpcM2G_ReqRecvFatigue* pMsg = RpcM2G_ReqRecvFatigue::CreateRpc();
		pMsg->m_oArg.set_roleid(pRole->GetID());
		pMsg->m_oArg.set_rpcid(DelayRpc());
		pRole->SendMsgToGS(*pMsg);
		return ;
	}
	else if (roArg.optype() == 0)
	{
		int totalSend = 0;
		GuildFatigue *pFatigue = pGuild->Get<GuildFatigue>();
		roRes.set_result((KKSG::ErrorCode)pFatigue->SendFatigue(pRole, roArg.targetid(), totalSend));
		roRes.set_totalsend(totalSend);
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}

	////////////////////
	/*
	int totalSend = 0;
	int totalRecv = 0;
	GuildFatigue *pFatigue = pGuild->Get<GuildFatigue>();
	if (roArg.optype() == 0)
	{
		roRes.set_result((KKSG::ErrorCode)pFatigue->SendFatigue(pRole, roArg.targetid(), totalSend));
		roRes.set_totalsend(totalSend);
	}
	else if (roArg.optype() == 1)
	{
		roRes.set_result((KKSG::ErrorCode)pFatigue->RecvFatigue(pRole, roArg.targetid(), totalRecv));
		roRes.set_totalrecv(totalRecv);
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}
	*/
}

void RpcC2M_GuildFatigueOPNew::OnDelayReplyRpc(const GuildFatigueArg &roArg, GuildFatigueRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = CGuildMgr::Instance()->GetRoleGuild(pRole->GetID());
	if (gid == INVALID_GUILDID)
	{
		return;
	}

	Guild *pGuild = CGuildMgr::Instance()->GetGuild(gid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	int totalRecv = 0;
	GuildFatigue *pFatigue = pGuild->Get<GuildFatigue>();
	if (pFatigue == NULL)
	{
		return ;
	}
	roRes.set_result((KKSG::ErrorCode)pFatigue->RecvFatigue(pRole, roArg.targetid(), totalRecv, roUserData.m_dwUserData));
	roRes.set_totalrecv(totalRecv);
}
