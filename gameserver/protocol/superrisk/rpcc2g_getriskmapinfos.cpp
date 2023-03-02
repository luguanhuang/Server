#include "pch.h"
#include "superrisk/rpcc2g_getriskmapinfos.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/superriskrecord.h"

// generate by ProtoGen at date: 2016/9/5 16:04:20

RPC_SERVER_IMPLEMETION(RpcC2G_GetRiskMapInfos, GetRiskMapInfosArg, GetRiskMapInfosRes)

void RpcC2G_GetRiskMapInfos::OnCall(const GetRiskMapInfosArg &roArg, GetRiskMapInfosRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CSuperRiskRecord* record = pRole->Get<CSuperRiskRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	UINT32 error = record->GetMapInfo2Client(roArg, roRes);
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_GetRiskMapInfos::OnDelayReplyRpc(const GetRiskMapInfosArg &roArg, GetRiskMapInfosRes &roRes, const CUserData &roUserData)
{
}
