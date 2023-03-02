#include "pch.h"
#include "emblem/rpcc2g_emblemlevelup.h"
#include "unit/rolemanager.h"
#include "gamelogic/bag.h"
#include "gamelogic/item.h"
#include "gamelogic/emblemmgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/noticemgr.h"

// generate by ProtoGen at date: 2015/6/11 15:58:26

RPC_SERVER_IMPLEMETION(RpcC2G_EmblemLevelUp, EmblemLevelUpArg, EmblemLevelUpRes)

void RpcC2G_EmblemLevelUp::OnCall(const EmblemLevelUpArg &roArg, EmblemLevelUpRes &roRes)
{
	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_EmblemLevelUp::OnDelayReplyRpc(const EmblemLevelUpArg &roArg, EmblemLevelUpRes &roRes, const CUserData &roUserData)
{

}
