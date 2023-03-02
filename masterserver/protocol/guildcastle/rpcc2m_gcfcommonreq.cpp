#include "pch.h"
#include "guildcastle/rpcc2m_gcfcommonreq.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "guildcastle/guildcastlerequest.h"
#include "guildcastle/guildcastlemgr.h"
#include "castle/rpcm2g_gcfcommonreqm2g.h"

// generate by ProtoGen at date: 2016/12/14 23:27:50

RPC_SERVER_IMPLEMETION(RpcC2M_GCFCommonReq, GCFCommonArg, GCFCommonRes)

void RpcC2M_GCFCommonReq::OnCall(const GCFCommonArg &roArg, GCFCommonRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}

	GcfRequest req(pRole);
	int ret = (ErrorCode)(ERR_SUCCESS);
	bool gotogs = false;
	switch(roArg.reqtype())
	{
	case GCF_JOIN_READY_SCENE:
		{
			gotogs = true;
		}
		break;
	case GCF_JOIN_FIGHT_SCENE:
		{
            ret = req.JoinFightScene(roArg.mapid());
            if(ERR_SUCCESS == ret)
            {
                req.BattleReport(roRes);
            }
		}
		break;
	case GCF_FIGHT_REPORT:
		{
			ret = req.BattleReport(roRes);
		}
		break;
	case GCF_FIGHT_RESULT:
		{
			ret = req.FightResult(roRes);
		}
		break;
	default:
		break;
	}

	if(ERR_SUCCESS != ret || false == gotogs)
	{
		if(ErrorCode_IsValid(ret))
		{
			roRes.set_errorcode((ErrorCode)ret);
		}
		else
		{
			roRes.set_errorcode(ERR_FAILED);
		}
	}
	else
	{
		RpcM2G_GCFCommonReqM2G* rpc = RpcM2G_GCFCommonReqM2G::CreateRpc();
		rpc->m_oArg.set_delayid(DelayRpc());
		rpc->m_oArg.set_sessionid(m_sessionID);
		rpc->m_oArg.mutable_c2marg()->CopyFrom(m_oArg);
		pRole->SendMsgToGS(*rpc);
	}

}

void RpcC2M_GCFCommonReq::OnDelayReplyRpc(const GCFCommonArg &roArg, GCFCommonRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return ;
	}
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_errorcode(ERR_FAILED);
		return ;
	}

	GCFCommonResM2G* m2gRes = (GCFCommonResM2G*)(roUserData.m_pUserPtr);

	if(ERR_SUCCESS != m2gRes->errorcode())
	{
		roRes.set_errorcode(m2gRes->errorcode());
		return;
	}

	GcfRequest req(pRole);
	int ret = (ErrorCode)(ERR_SUCCESS);
	switch(roArg.reqtype())
	{
	case GCF_JOIN_READY_SCENE:
		{
			ret = req.JoinReadyScene(roArg.territoryid());	
            if(ERR_SUCCESS == ret)
            {
                req.BattleReport(roRes, roArg.territoryid());
            }
		}
		break;
	default:
		break;
	}

	if(ErrorCode_IsValid(ret))
	{
		roRes.set_errorcode((ErrorCode)ret);
	}
	else
	{
		roRes.set_errorcode(ERR_FAILED);
	}
}
