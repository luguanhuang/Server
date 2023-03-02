#include "pch.h"
#include "dragonguild/rpcm2g_getrolepower.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "table/OpenSystemMgr.h"
#include "dragonguild/dragonguildrecord.h"
#include "util/gametime.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2017/9/5 14:39:27

RPC_SERVER_IMPLEMETION(RpcM2G_GetRolePower, GetRolePowerArg, GetRolePowerRes)

void RpcM2G_GetRolePower::OnCall(const GetRolePowerArg &roArg, GetRolePowerRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}

	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(roArg.sysid()))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}

	if (pRole->GetLevel() < OpenSystemMgr::Instance()->GetPlayerLevelBySystemID(roArg.sysid()))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return ;
	}

	/*
	if (GameTime::GetTime() < pRole->Get<DragonGuildRecord>()->GetDragonGuildLastTime())
	{
		roRes.set_result(KKSG::ERR_DG_IN_CD);
		roRes.set_cdtime(pRole->Get<DragonGuildRecord>()->GetDragonGuildLastTime());
		return ;
	}
	
	pRole->Get<DragonGuildRecord>()->AddDragonGuildEnterCnt();
	if (pRole->Get<DragonGuildRecord>()->GetDragonGuileEnterCnt() >= GetGlobalConfig().DragonGuildEnterLimit[0])
	{
		pRole->Get<DragonGuildRecord>()->SetDragonGuildLastTime(GameTime::GetTime() + GetGlobalConfig().DragonGuildEnterLimit[1] * 60 * 60);
	}
	*/

	roRes.set_rolepower(pRole->GetAttr(TOTAL_POWERPOINT));
	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcM2G_GetRolePower::OnDelayReplyRpc(const GetRolePowerArg &roArg, GetRolePowerRes &roRes, const CUserData &roUserData)
{
}
