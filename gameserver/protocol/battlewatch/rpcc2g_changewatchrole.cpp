#include "pch.h"
#include "battlewatch/rpcc2g_changewatchrole.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "live/livemanager.h"

// generate by ProtoGen at date: 2016/7/11 16:33:38

RPC_SERVER_IMPLEMETION(RpcC2G_ChangeWatchRole, ChangeWatchRoleArg, ChangeWatchRoleRes)

void RpcC2G_ChangeWatchRole::OnCall(const ChangeWatchRoleArg &roArg, ChangeWatchRoleRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role || !role->GetCurrScene())
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}

	UINT32 error = role->GetCurrScene()->ChangeWatchRole(role, roArg.roleid());

	roRes.set_error((ErrorCode)error);
}

void RpcC2G_ChangeWatchRole::OnDelayReplyRpc(const ChangeWatchRoleArg &roArg, ChangeWatchRoleRes &roRes, const CUserData &roUserData)
{
}
