#include "pch.h"
#include "guild/rpcc2g_getguildbonusreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolesummarymgr.h"
#include "guild/rpcg2m_mguildbonusoperatereq.h"
#include "guild/guilddef.h"
#include "guild/guildrecord.h"
#include "gamelogic/bagtransition.h"
#include "utility/loghelper.h"
#include "network/mslink.h"
#include "guild/guildconfig.h"

// generate by ProtoGen at date: 2015/10/8 20:17:14

RPC_SERVER_IMPLEMETION(RpcC2G_GetGuildBonusReward, GetGuildBonusRewardArg, GetGuildBonusRewardResult)

void RpcC2G_GetGuildBonusReward::OnCall(const GetGuildBonusRewardArg &roArg, GetGuildBonusRewardResult &roRes)
{
	roRes.set_getvalue(0);

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
	{
		return ;
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
	req->m_oArg.mutable_getbonusrewardarg()->CopyFrom(roArg);
	req->m_oArg.set_rpcid(DelayRpc());
	req->m_oArg.set_optype(GBonus_OP_GetBonusReward);
	pRole->Get<CGuildRecord>()->Convert2_KKSGGuildRecord(req->m_oArg.mutable_roleguildrecord());
	pRole->SendToMS(*req);
}

void RpcC2G_GetGuildBonusReward::OnDelayReplyRpc(const GetGuildBonusRewardArg &roArg, GetGuildBonusRewardResult &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode(KKSG::ERR_FAILED);
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
	const MGuildBonusOperateReqRes* res = (MGuildBonusOperateReqRes*)roUserData.m_pUserPtr;
	if (res->ischeckinbonus())
	{
		UINT32 getNum = pRole->Get<CGuildRecord>()->GetGuildBonusData().m_getCheckInBonusNum;
		if (getNum >= GetGlobalConfig().MaxGetGuildCheckInBonusNum)
		{
			LogError("%llu, %s, get bonus %u too fast", pRole->GetRoleID(), pRole->GetName().c_str(), roArg.bonusid());
			roRes.set_errorcode(KKSG::ERR_GUILDBONUS_ALREADYGET);
			return;
		}
	}

	if (res->bonustemplateid())
	{
		UINT32 error = pRole->Get<CGuildRecord>()->AddGetBonusNum(res->bonustemplateid(), 1);
		if (error != KKSG::ERR_SUCCESS)
		{
			roRes.set_errorcode((ErrorCode)error);
			return;
		}
	}

	if (res->has_getbonusreward())
	{
		roRes.CopyFrom(res->getbonusreward());
	}
	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	BagGiveItemTransition transition(pRole);
	transition.SetReason(ItemFlow_Guild, ItemFlow_Guild_Bonus);
	transition.GiveItem(res->bonustype(), res->realgetrewardnum());
	transition.NotifyClient();
	if (res->ischeckinbonus())
	{
		pRole->Get<CGuildRecord>()->AddCheckInBonusNum(1);
	}
}
