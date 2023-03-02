#include "pch.h"
#include "scene/rpcc2g_stagecountreset.h"
#include "unit/rolemanager.h"
#include "gamelogic/stagemgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/takecostmgr.h"
#include "scene/sceneconfig.h"

// generate by ProtoGen at date: 2015/10/8 14:21:24

RPC_SERVER_IMPLEMETION(RpcC2G_StageCountReset, StageCountResetArg, StageCountResetRes)

void RpcC2G_StageCountReset::OnCall(const StageCountResetArg &roArg, StageCountResetRes &roRes)
{
	roRes.set_errorcode(ERR_UNKNOWN);

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}	

	StageMgr* stage = role->Get<StageMgr>();
	stage->Update();
	
	UINT32 groupid = roArg.groupid();
	int buycount = stage->GetBuyCount(groupid);
	if (!TakeCostManager::Instance()->TakeCost("BuyStageCountCost", role, buycount, ItemFlow_Stage, ItemFlow_Stage_Count))
	{
		roRes.set_errorcode(ERR_ITEM_NOT_ENOUGH);
		return;
	}
	stage->ResetPlayCount(groupid);

	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_StageCountReset::OnDelayReplyRpc(const StageCountResetArg &roArg, StageCountResetRes &roRes, const CUserData &roUserData)
{
}
