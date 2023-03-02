#include "pch.h"
#include "guild/rpcc2g_askforcheckinbonus.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"
#include "guild/guildrecord.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/6/15 11:34:46

RPC_SERVER_IMPLEMETION(RpcC2G_AskForCheckInBonus, AskForCheckInBonusArg, AskForCheckInBonusRes)

void RpcC2G_AskForCheckInBonus::OnCall(const AskForCheckInBonusArg &roArg, AskForCheckInBonusRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	if (!pRole->GetCurrScene())
	{
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return ;
	}

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return ;
	}

	auto record = pRole->Get<CGuildRecord>();
	int now = time(0);
	if ((record->GetAskBonusTime() +  GetGlobalConfig().GuildBonusAskTimeSpan) > now)
	{
		roRes.set_errorcode(KKSG::ERR_GUILD_CHECKINBONUS_ASKTOOMUCH);
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
	//req->m_oArg.mutable_getbonusrewardarg()->CopyFrom(roArg);
	req->m_oArg.set_rpcid(DelayRpc());
	req->m_oArg.set_optype(GBonus_OP_GetBonusAskFor);
	pRole->Get<CGuildRecord>()->Convert2_KKSGGuildRecord(req->m_oArg.mutable_roleguildrecord());
	pRole->SendToMS(*req);
}

void RpcC2G_AskForCheckInBonus::OnDelayReplyRpc(const AskForCheckInBonusArg &roArg, AskForCheckInBonusRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode((ErrorCode)roUserData.m_dwUserData);
	if(roUserData.m_dwUserData != KKSG::ERR_SUCCESS)
	{
		return;
	}
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is out, session id:"<<m_sessionID<<END;
		return;
	}
	auto record = pRole->Get<CGuildRecord>();
	record->SetAskBonusTime(time(0));
}
