#include "pch.h"
#include "firstpass/rpcc2g_firstpassinforeq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "firstpass/rpcg2m_getrankinfo.h"
#include "gamelogic/firstpassrecord.h"
#include "commondef.h"
#include "network/mslink.h"

// generate by ProtoGen at date: 2016/8/24 16:20:50

#define FIRSTPASSRANKCOUNT 1000

RPC_SERVER_IMPLEMETION(RpcC2G_FirstPassInfoReq, FirstPassInfoReqArg, FirstPassInfoReqRes)

void RpcC2G_FirstPassInfoReq::OnCall(const FirstPassInfoReqArg &roArg, FirstPassInfoReqRes &roRes)
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

	RpcG2M_GetRankInfo* rpc = RpcG2M_GetRankInfo::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_ranktype(KKSG::FirstPassRank);

	pRole->SendToMS(*rpc);
}

void RpcC2G_FirstPassInfoReq::OnDelayReplyRpc(const FirstPassInfoReqArg &roArg, FirstPassInfoReqRes &roRes, const CUserData &roUserData)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_error(KKSG::ERR_SUCCESS);
	if (!roUserData.m_pUserPtr)
	{
		return;
	}
	GetRankInfoRes* poRes = (GetRankInfoRes*)roUserData.m_pUserPtr;
	if (poRes)
	{
		for (int i = 0; i < poRes->firstpassrankinfos_size(); ++i)
		{
			KKSG::FirstPassStageInfo2Client* info = roRes.add_stageinfos();

			info->set_firstpassid(poRes->firstpassrankinfos(i).firstpassid());
			info->set_totalrank(poRes->firstpassrankinfos(i).totalrank());

			auto recordInfo = pRole->Get<CFirstPassRecord>()->GetFirstPassStageInfo(info->firstpassid());
			if (recordInfo)
			{
				if (!recordInfo->has_rank())
				{
					info->set_myrank(0);
				}
				else
				{
					if (recordInfo->rank() == INVALID_RANK)
						info->set_myrank(FIRSTPASSRANKCOUNT + 1);
					else
						info->set_myrank(recordInfo->rank());
				}

				info->set_isgetreward(recordInfo->isgetreward());

				bool canCommend = true;
				for (int j = 0; j < recordInfo->commendedstarlevels_size(); ++j)
				{
					if (recordInfo->commendedstarlevels(j) == poRes->firstpassrankinfos(i).toprankstarlevel())
					{
						canCommend = false;
						break;
					}
				}
				if (!canCommend)
				{
					info->set_hascommended(true);
				}
				else
				{
					info->set_hascommended(false);
				}
			}
		}
	}
}
