#include "pch.h"
#include "common/rpcc2g_setroleconfig.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "utility/tlogger.h"
#include "gamelogic/rolesummarymgr.h"
#include "table/PushTableConfig.h"
#include "gamelogic/rolepushinforecord.h"

// generate by ProtoGen at date: 2016/1/21 16:36:01

RPC_SERVER_IMPLEMETION(RpcC2G_SetRoleConfig, SetRoleConfigReq, SetRoleConfigRes)

void RpcC2G_SetRoleConfig::OnCall(const SetRoleConfigReq &roArg, SetRoleConfigRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	roRes.set_errorcode(role->SetConfig(roArg.type(), roArg.value()) ? ERR_SUCCESS : ERR_UNKNOWN);
	if (roArg.type() == "OD_VIEW")
	{
		TOdViewFlow oLog(role);
		oLog.m_nOdType = convert<UINT32>(roArg.value());
		oLog.Do();
	}

	// 玩家推送配置变化
	RolePushInfoRecord* pushRecord = role->Get<RolePushInfoRecord>();
	pushRecord->UpdateConfig(roArg.type(), roArg.value());
}

void RpcC2G_SetRoleConfig::OnDelayReplyRpc(const SetRoleConfigReq &roArg, SetRoleConfigRes &roRes, const CUserData &roUserData)
{
}
