#include "pch.h"
#include "team/rpcm2g_teamrequestm2g.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "team/teamrequest.h"
#include "team/teammgr.h"

// generate by ProtoGen at date: 2016/8/18 20:01:24

RPC_CLIENT_IMPLEMETION(RpcM2G_TeamRequestM2G, TeamOPM2GArg, TeamOPM2GRes)

void RpcM2G_TeamRequestM2G::OnReply(const TeamOPM2GArg &roArg, const TeamOPM2GRes &roRes, const CUserData &roUserData)
{
	TeamOPM2GRes tmpRes;
	tmpRes.CopyFrom(roRes);

	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	ReplyDelayRpc(roArg.delayid(), userData);		
}

void RpcM2G_TeamRequestM2G::OnTimeout(const TeamOPM2GArg &roArg, const CUserData &roUserData)
{
	//暂不考虑超时
}
