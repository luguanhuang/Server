#include "pch.h"
#include "pk/rpcm2g_pkreqm2g.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "pk/pkopenmgr.h"
#include "pk/pkmgr.h"
#include "team/teammatchmgr.h"
#include "other/matchhandler.h"

// generate by ProtoGen at date: 2016/8/25 20:15:29

RPC_CLIENT_IMPLEMETION(RpcM2G_PkReqM2G, PkReqM2GArg, PkReqM2GRes)

void RpcM2G_PkReqM2G::OnReply(const PkReqM2GArg &roArg, const PkReqM2GRes &roRes, const CUserData &roUserData)
{
	const PkReqArg& c2mArg = roArg.c2marg();
	UINT32 delayid = roArg.delayid();

	PkReqRes c2mRes;
	c2mRes.CopyFrom(roRes.c2mres());

	CUserData pUserData;
	pUserData.m_pUserPtr = (void*)(&c2mRes);

	if(ERR_SUCCESS != c2mRes.errorcode())
	{
		ReplyDelayRpc(delayid, pUserData);
		return ;
	}

	SessionType pSessionID = (SessionType)roArg.sessionid();
	CRole* pRole = CRoleManager::Instance()->GetBySession(pSessionID);
	if(NULL == pRole)
	{
		c2mRes.set_errorcode(ERR_INVALID_REQUEST);
		ReplyDelayRpc(delayid, pUserData);
		return;	
	}

	//不要出现return
	ErrorCode ret = ERR_SUCCESS;
	switch(c2mArg.type())
	{
	case PKREQ_ADDPK:
		{
			if(0 != PkOpenMgr::Instance()->TimeLimit())
			{
				c2mRes.set_time(0);
				ret = ERR_PK_OPENTIME;
				break;
			}
			if(PkMgr::Instance()->InPrepare(pRole->GetID()))
			{
				c2mRes.set_time(0);
				ret = ERR_SUCCESS;
				break;
			}
			if(PkMgr::Instance()->InDelay(pRole->GetID()))
			{
				ret = ERR_FAILED;//wait
				break;
			}
			//if(0 != pRole->GetTeamID())
			//{
			//	c2mRes.set_time(0);
			//	ret = ERR_FAILED;
			//	break;
			//}
			//UINT32 point = roRes.point();
			HMatchHandler handler;
			handler.Data().pkAddRobot = roRes.addrobot();
			//handler.Data().pkRec = roRes.pkrec();
			*handler.Data().pkMatchInfo.mutable_rolerec() = roRes.pkrec();
			handler.Data().pkMatchInfo.set_rolelevel(roRes.rolelevel());
			ret = (ErrorCode)handler.Start(pRole, KKSG::KMT_PK);
		}
		break;
	case PKREQ_REMOVEPK:
		{
			HMatchHandler handler;
			handler.Stop(pRole, KKSG::KMT_PK);
			ret = ERR_SUCCESS;
		}
		break;
	case PKREQ_ALLINFO:
		{
			QueryPkInfoRes* allinfo = c2mRes.mutable_allinfo();

			allinfo->set_rank1v1(PkMgr::Instance()->GetRank(pRole->GetID(), KKSG::PK_1v1));
			allinfo->set_rank2v2(PkMgr::Instance()->GetRank(pRole->GetID(), KKSG::PK_2v2));
			allinfo->set_rankrewardleftt(PkMgr::Instance()->RankRewardLeftTime());
			allinfo->set_timelimit(PkOpenMgr::Instance()->TimeLimit());
		}
		break;
	case PKREQ_FETCHPOINTREWARD:
		{
			;//no code
		}
		break;
	default:
		break;
	}

	c2mRes.set_errorcode(ret);

	ReplyDelayRpc(delayid, pUserData);

}

void RpcM2G_PkReqM2G::OnTimeout(const PkReqM2GArg &roArg, const CUserData &roUserData)
{
}
