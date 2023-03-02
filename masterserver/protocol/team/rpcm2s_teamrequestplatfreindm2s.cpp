#include "pch.h"
#include "team/rpcm2s_teamrequestplatfreindm2s.h"
#include "team/ptcm2c_teamrequestplatfreind2client.h"
#include "role/rolemanager.h"
#include "role/role.h"

// generate by ProtoGen at date: 2017/5/3 15:25:11

RPC_CLIENT_IMPLEMETION(RpcM2S_TeamRequestPlatFreindM2S, TeamRequestPlatFreindM2SArg, TeamRequestPlatFreindM2SRes)

void RpcM2S_TeamRequestPlatFreindM2S::OnReply(const TeamRequestPlatFreindM2SArg &roArg, const TeamRequestPlatFreindM2SRes &roRes, const CUserData &roUserData)
{
	if (roRes.error() == KKSG::ERR_ROLE_NOT_ONLINE)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.teaminviteinfo().invfromroleid());
		if (!role)
		{
			return;
		}
		PtcM2C_TeamRequestPlatFreind2Client ntf;
		ntf.m_Data.set_useroffline(true);
		ntf.m_Data.set_openid(roArg.destinfo().openid());
		role->Send(ntf);
	}
}

void RpcM2S_TeamRequestPlatFreindM2S::OnTimeout(const TeamRequestPlatFreindM2SArg &roArg, const CUserData &roUserData)
{
}
