#include "pch.h"
#include "match/rpcc2m_kmatchcommonreq.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teamcommon.h"
#include "team/teamtranshandler.h"
#include "network/teamlink.h"
#include "config.h"
#include "other/matchhandler.h"
#include "team/teammgr.h"
#include "match/matchrequest.h"

// generate by ProtoGen at date: 2017/5/5 15:00:45

RPC_SERVER_IMPLEMETION(RpcC2M_KMatchCommonReq, KMatchCommonArg, KMatchCommonRes)

void RpcC2M_KMatchCommonReq::OnCall(const KMatchCommonArg &roArg, KMatchCommonRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	//跨服组队
	if(roArg.isteam() && pRole->HasTeamInTs())
	{
		if(TeamLink::Instance()->IsConnected())
		{
			TeamTransHandler trans;
			trans.HandleMatchReqFromC(DelayRpc(), pRole, roArg);
		}
		else
		{
			roRes.set_errorcode(KKSG::ERR_SERVER_UNNORMAL);
		}
		return;
	}

	MatchRequest req(pRole);
	req.HandleC2MArg(DelayRpc(), roArg);
}

void RpcC2M_KMatchCommonReq::OnDelayReplyRpc(const KMatchCommonArg &roArg, KMatchCommonRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		return;
	}
	KMatchCommonRes* tmpRes = (KMatchCommonRes*)(roUserData.m_pUserPtr);
	roRes = (*tmpRes);
}
