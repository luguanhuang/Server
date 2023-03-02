#include "pch.h"
#include "activity/rpcc2g_sweeptower.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/towerrecord.h"
#include "gamelogic/activityhandler.h"

// generate by ProtoGen at date: 2016/7/15 20:07:40

RPC_SERVER_IMPLEMETION(RpcC2G_SweepTower, SweepTowerArg, SweepTowerRes)

void RpcC2G_SweepTower::OnCall(const SweepTowerArg &roArg, SweepTowerRes &roRes)
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
	UINT32 itemID, count, leftTime;
	itemID = count = leftTime = 0;
	if(roArg.has_cost())
	{
		itemID = roArg.cost().itemid();
		count = roArg.cost().itemcount();
	}

	UINT32 error = record->SweepTower(roArg.hardlevel(), itemID, count, leftTime);
	if (error == KKSG::ERR_SUCCESS)
	{
		CActivityHandler::GlobalHandler.FinishScene(pRole, KKSG::SCENE_TOWER);
	}
	roRes.set_error((ErrorCode)error);
	roRes.set_lefttime(leftTime);
}

void RpcC2G_SweepTower::OnDelayReplyRpc(const SweepTowerArg &roArg, SweepTowerRes &roRes, const CUserData &roUserData)
{
}
