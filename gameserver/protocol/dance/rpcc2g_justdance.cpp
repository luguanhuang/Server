#include "pch.h"
#include "dance/rpcc2g_justdance.h"
#include "unit/rolemanager.h"
#include "unit/roleoutlook.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2016/11/11 20:21:22

RPC_SERVER_IMPLEMETION(RpcC2G_JustDance, JustDanceArg, JustDanceRes)

void RpcC2G_JustDance::OnCall(const JustDanceArg &roArg, JustDanceRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_UNKNOWN);
		return;
	}	
	if(roArg.type() == KKSG::OutLook_Dance)
	{
		if(!pRole->StateCanChangeTo((KKSG::OutLookStateType)roArg.type()))
		{
			roRes.set_errorcode(KKSG::ERR_STATE_CANTCHANGE);
			return;
		}
		pRole->SetState((KKSG::OutLookStateType)roArg.type(), roArg.danceid());
	}
	else
	{
		pRole->SetState(KKSG::OutLook_Normal);
	}

	RoleOutLook outlook(pRole);
	outlook.Fill(SUMMARY_STATE, pRole);
	outlook.BroadCast();

	roRes.set_errorcode(KKSG::ERR_SUCCESS);

	TDanceFlow oLog(pRole);
	oLog.m_iOp = roArg.type();
	oLog.Do();
}

void RpcC2G_JustDance::OnDelayReplyRpc(const JustDanceArg &roArg, JustDanceRes &roRes, const CUserData &roUserData)
{
}
