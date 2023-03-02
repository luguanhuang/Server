#include "pch.h"
#include "battlefield/rpcc2g_battlefieldawardnumreq.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/battlefieldrecord.h"

// generate by ProtoGen at date: 2017/9/4 14:06:51

RPC_SERVER_IMPLEMETION(RpcC2G_BattleFieldAwardNumReq, BattleFieldAwardNumArg, BattleFieldAwardNumRes)

void RpcC2G_BattleFieldAwardNumReq::OnCall(const BattleFieldAwardNumArg &roArg, BattleFieldAwardNumRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
		return;
	CBattleFieldRecord* pRecord = pRole->Get<CBattleFieldRecord>();
	pRecord->FillBattleReward(roRes);
}

void RpcC2G_BattleFieldAwardNumReq::OnDelayReplyRpc(const BattleFieldAwardNumArg &roArg, BattleFieldAwardNumRes &roRes, const CUserData &roUserData)
{
}
