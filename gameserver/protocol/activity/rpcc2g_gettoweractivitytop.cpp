#include "pch.h"
#include "activity/rpcc2g_gettoweractivitytop.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerrecord.h"

// generate by ProtoGen at date: 2016/5/25 16:40:12

RPC_SERVER_IMPLEMETION(RpcC2G_GetTowerActivityTop, GetTowerActivityTopArg, GetTowerActivityTopRes)

void RpcC2G_GetTowerActivityTop::OnCall(const GetTowerActivityTopArg &roArg, GetTowerActivityTopRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	CTowerRecord* record = pRole->Get<CTowerRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	record->FillTopInfo2Client(roRes);
	roRes.set_error(KKSG::ERR_SUCCESS);
}

void RpcC2G_GetTowerActivityTop::OnDelayReplyRpc(const GetTowerActivityTopArg &roArg, GetTowerActivityTopRes &roRes, const CUserData &roUserData)
{
}
