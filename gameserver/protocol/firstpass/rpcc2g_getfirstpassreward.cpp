#include "pch.h"
#include "firstpass/rpcc2g_getfirstpassreward.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "network/mslink.h"
#include "gamelogic/firstpassrecord.h"

// generate by ProtoGen at date: 2016/8/24 17:19:45

RPC_SERVER_IMPLEMETION(RpcC2G_GetFirstPassReward, GetFirstPassRewardArg, GetFirstPassRewardRes)

void RpcC2G_GetFirstPassReward::OnCall(const GetFirstPassRewardArg &roArg, GetFirstPassRewardRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	if (MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	/*RpcG2M_GetRankInfo* rpc = RpcG2M_GetRankInfo::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_firstpassid(roArg.firstpassid());
	rpc->m_oArg.set_ranktype(KKSG::FirstPassRank);
	rpc->m_oArg.set_serverid(pRole->GetServerId());
	rpc->m_oArg.set_getfirstpassreward(true);

	pRole->SendToMS(*rpc);*/

	UINT32 error = pRole->Get<CFirstPassRecord>()->GetFirstPassReward(roArg.firstpassid(), 0);

	roRes.set_error((KKSG::ErrorCode)error);
}

void RpcC2G_GetFirstPassReward::OnDelayReplyRpc(const GetFirstPassRewardArg &roArg, GetFirstPassRewardRes &roRes, const CUserData &roUserData)
{
	/*if (!roUserData.m_pUserPtr)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	roRes.set_error(KKSG::ERR_SUCCESS);
	GetRankInfoRes* poRes = (GetRankInfoRes*)roUserData.m_pUserPtr;
	if (poRes)
	{
		for (int i = 0; i < poRes->firstpassrankinfos_size(); ++i)
		{
			if (poRes->firstpassrankinfos(i).firstpassid() == roArg.firstpassid())
			{
				Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
				if (!role)
				{
					return;
				}
				CFirstPassRecord* recordInfo = role->Get<CFirstPassRecord>();
				if (recordInfo)
				{
					UINT32 error = recordInfo->GetFirstPassReward(roArg.firstpassid(), poRes->firstpassrankinfos(i).myrank());
					roRes.set_error((KKSG::ErrorCode)error);
					return;
				}
			}
		}
	}
	*/
}
