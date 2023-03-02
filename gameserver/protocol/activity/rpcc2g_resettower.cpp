#include "pch.h"
#include "activity/rpcc2g_resettower.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerrecord.h"

// generate by ProtoGen at date: 2016/7/15 20:17:44

RPC_SERVER_IMPLEMETION(RpcC2G_ResetTower, ResetTowerArg, ResetTowerRes)

void RpcC2G_ResetTower::OnCall(const ResetTowerArg &roArg, ResetTowerRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL || !pRole->GetCurrScene())
	{
		roRes.set_error(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	roRes.set_error(KKSG::ERR_SUCCESS);
	if(pRole->CheckIdipAndNotify(KKSG::PUNISH_USER_DAILY_PLAY))
	{
		return ;
	}

	CTowerRecord* record = pRole->Get<CTowerRecord>();
	if (!record)
	{
		roRes.set_error(KKSG::ERR_FAILED);
		return;
	}

	UINT32 error = record->ResetTower(roArg.hardlevel());
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_ResetTower::OnDelayReplyRpc(const ResetTowerArg &roArg, ResetTowerRes &roRes, const CUserData &roUserData)
{
}
