#include "pch.h"
#include "skycraft/rpcc2m_getskycraftrecord.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "skyteam/skyteammgr.h"

// generate by ProtoGen at date: 2017/4/11 14:47:47

RPC_SERVER_IMPLEMETION(RpcC2M_GetSkyCraftRecord, GetSkyCraftRecordArg, GetSkyCraftRecordRes)

void RpcC2M_GetSkyCraftRecord::OnCall(const GetSkyCraftRecordArg &roArg, GetSkyCraftRecordRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamByRoleId(pRole->GetID());
	if (pSTeam == NULL)
	{
		roRes.set_result(KKSG::ERR_SKY_HAS_NO_TEAM);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	pSTeam->GetTeamRecord().FillRecord(roRes);
}

void RpcC2M_GetSkyCraftRecord::OnDelayReplyRpc(const GetSkyCraftRecordArg &roArg, GetSkyCraftRecordRes &roRes, const CUserData &roUserData)
{
}
