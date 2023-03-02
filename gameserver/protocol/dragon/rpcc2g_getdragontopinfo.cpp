#include "pch.h"
#include "dragon/rpcc2g_getdragontopinfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/dragonrecord.h"

// generate by ProtoGen at date: 2016/7/4 17:55:37

RPC_SERVER_IMPLEMETION(RpcC2G_GetDragonTopInfo, GetDragonTopInfoArg, GetDragonTopInfoRes)

void RpcC2G_GetDragonTopInfo::OnCall(const GetDragonTopInfoArg &roArg, GetDragonTopInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CDragonRecord* record = pRole->Get<CDragonRecord>();
	record->FillTopInfo2Client(roRes);
}

void RpcC2G_GetDragonTopInfo::OnDelayReplyRpc(const GetDragonTopInfoArg &roArg, GetDragonTopInfoRes &roRes, const CUserData &roUserData)
{
}
