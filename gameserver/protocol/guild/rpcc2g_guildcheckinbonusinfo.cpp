#include "pch.h"
#include "guild/rpcc2g_guildcheckinbonusinfo.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"
#include "guild/guildrecord.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/6/15 10:21:02

RPC_SERVER_IMPLEMETION(RpcC2G_GuildCheckInBonusInfo, GuildCheckInBonusInfoArg, GuildCheckInBonusInfoRes)

void RpcC2G_GuildCheckInBonusInfo::OnCall(const GuildCheckInBonusInfoArg &roArg, GuildCheckInBonusInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}
	if (!pRole->GetCurrScene())
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}

	RpcG2M_MGuildBonusOperateReq* req = RpcG2M_MGuildBonusOperateReq::CreateRpc();
	req->m_sessionID = m_sessionID;
	req->m_oArg.mutable_checkinbonusarg()->CopyFrom(roArg);
	req->m_oArg.set_rpcid(DelayRpc());
	req->m_oArg.set_optype(GBonus_OP_GetCheckInBonusInfo);
	pRole->Get<CGuildRecord>()->Convert2_KKSGGuildRecord(req->m_oArg.mutable_roleguildrecord());
	pRole->SendToMS(*req);
}

void RpcC2G_GuildCheckInBonusInfo::OnDelayReplyRpc(const GuildCheckInBonusInfoArg &roArg, GuildCheckInBonusInfoRes &roRes, const CUserData &roUserData)
{
	int now = time(0);
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
	const GuildCheckInBonusInfoRes* res = (GuildCheckInBonusInfoRes*)roUserData.m_pUserPtr;
	roRes.CopyFrom(*res);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	roRes.set_ischeckedin(pRole->Get<CGuildRecord>()->GetCheckin() ? true : false);
	CGuildRecord* record = pRole->Get<CGuildRecord>();
	if (record)
	{
		if ((record->GetAskBonusTime() +  GetGlobalConfig().GuildBonusAskTimeSpan) > now)
		{
			roRes.set_leftaskbonustime((record->GetAskBonusTime() +  GetGlobalConfig().GuildBonusAskTimeSpan) - now);
		}
		else
			roRes.set_leftaskbonustime(0);
	}

}
