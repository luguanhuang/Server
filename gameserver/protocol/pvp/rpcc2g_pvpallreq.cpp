#include "pch.h"
#include "pvp/rpcc2g_pvpallreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/pvprequest.h"
#include "gamelogic/pvprecord.h"
#include "gamelogic/pvpmgr.h"
#include "pvp/rpcg2m_pvpallg2mreq.h"
#include "idip/idiprecord.h"
#include "mslink.h"

// generate by ProtoGen at date: 2016/6/15 14:30:11

RPC_SERVER_IMPLEMETION(RpcC2G_PvpAllReq, PvpArg, PvpRes)

void RpcC2G_PvpAllReq::OnCall(const PvpArg &roArg, PvpRes &roRes)
{
	roRes.set_reqtype(roArg.type());
	roRes.set_err(KKSG::ERR_SUCCESS);
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		if(PVP_REQ_BASE_DATA == roArg.type())
		{
			roRes.set_err(ERR_CAN_INGORE);//正常情况
		}
		else
		{
			roRes.set_err(ERR_INVALID_REQUEST);
		}
		return;
	}

	bool gotoms = false;

	PvpRequest req(pRole);
	switch(roArg.type())
	{
	case KKSG::PVP_REQ_IN_MATCH:
		{
			if(pRole->Get<CIdipRecord>()->CheckIdipAndNotify(KKSG::PUNISH_USER_MULTI_ACTIVITY))
			{
				roRes.set_err(KKSG::ERR_FAILED);
				break;
			}

			gotoms = true;
		}
		break;
	case KKSG::PVP_REQ_OUT_MATCH:
		{
			gotoms = true;
		}
		break;
	case KKSG::PVP_REQ_BASE_DATA:
		{
			KKSG::PvpBaseData* data = roRes.mutable_basedata();
			pRole->Get<MyPvpRecord>()->FillAllCountClient(data);

			gotoms = true;
		}
		break;
	case KKSG::PVP_REQ_HISTORY_REC:
		{
			pRole->Get<MyPvpRecord>()->FillMyRecsClient(roRes.mutable_history());
		}
		break;
	case KKSG::PVP_REQ_GET_WEEKREWARD:
		{
			PvpRequest req(pRole);
			roRes.set_err((KKSG::ErrorCode)req.GetWeekReward());
		}
		break;
	default:
		break;
	}

	if(gotoms)
	{
		if(MSLink::Instance()->GetConnID() == INVALID_CONN_ID)
		{
			roRes.set_err(ERR_MS_UNNORMAL);
			return ;
		}

		RpcG2M_PvpAllG2MReq *rpc = RpcG2M_PvpAllG2MReq::CreateRpc();
		rpc->m_oArg.set_roleid(pRole->GetID());
		rpc->m_oArg.mutable_c2garg()->CopyFrom(m_oArg);

		UINT32 delayid = DelayRpc();
		rpc->m_oArg.set_delayid(delayid);

		pRole->SendToMS(*rpc);
	}
}

void RpcC2G_PvpAllReq::OnDelayReplyRpc(const PvpArg &roArg, PvpRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_err(KKSG::ERR_FAILED);
	}
	PvpRes* tmp = (PvpRes*)roUserData.m_pUserPtr;
	switch(roArg.type())
	{
	case KKSG::PVP_REQ_IN_MATCH:
	case KKSG::PVP_REQ_OUT_MATCH:
		{
			roRes.CopyFrom(*tmp);		
		}
		break;
	case KKSG::PVP_REQ_BASE_DATA:
		{
			roRes.mutable_basedata()->set_matchingcount(tmp->basedata().matchingcount());
		}
		break;
	default:
		break;
	}
}
