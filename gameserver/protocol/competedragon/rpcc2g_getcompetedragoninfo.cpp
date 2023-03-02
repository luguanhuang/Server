#include "pch.h"
#include "competedragon/rpcc2g_getcompetedragoninfo.h"
#include "gamelogic/competedragonrecord.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/9/7 11:20:31

RPC_SERVER_IMPLEMETION(RpcC2G_GetCompeteDragonInfo, GetCompeteDragonInfoArg, GetCompeteDragonInfoRes)

void RpcC2G_GetCompeteDragonInfo::OnCall(const GetCompeteDragonInfoArg &roArg, GetCompeteDragonInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CCompeteDragonRecord* record = pRole->Get<CCompeteDragonRecord>();
	if (roArg.oparg() == KKSG::CompeteDragon_GetInfo)
	{
		UINT32 error = record->FillCompeteDragonInfo2Client(roRes);
		roRes.set_error((ErrorCode)error);
	}
	else if (roArg.oparg() == KKSG::CompeteDragon_GetReward)
	{
		UINT32 error = record->GetReward();
		roRes.set_error((ErrorCode)error);
	}
}

void RpcC2G_GetCompeteDragonInfo::OnDelayReplyRpc(const GetCompeteDragonInfoArg &roArg, GetCompeteDragonInfoRes &roRes, const CUserData &roUserData)
{
}
