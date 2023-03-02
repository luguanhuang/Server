#include "pch.h"
#include "skycraft/rpcc2m_getskycraftteaminfo.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "skyteam/skyteammgr.h"

// generate by ProtoGen at date: 2017/4/11 15:20:32

RPC_SERVER_IMPLEMETION(RpcC2M_GetSkyCraftTeamInfo, GetSkyCraftTeamInfoArg, GetSkyCraftTeamInfoRes)

void RpcC2M_GetSkyCraftTeamInfo::OnCall(const GetSkyCraftTeamInfoArg &roArg, GetSkyCraftTeamInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamById(roArg.stid());
	if (pSTeam == NULL)
	{
		roRes.set_result(KKSG::ERR_SKY_HAS_NO_TEAM);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	pSTeam->FillTeamDetail(*roRes.mutable_team());
}

void RpcC2M_GetSkyCraftTeamInfo::OnDelayReplyRpc(const GetSkyCraftTeamInfoArg &roArg, GetSkyCraftTeamInfoRes &roRes, const CUserData &roUserData)
{
}
