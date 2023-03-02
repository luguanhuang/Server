#include "pch.h"
#include "guild/rpcm2g_enterguildpower.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/8/29 17:36:36

RPC_SERVER_IMPLEMETION(RpcM2G_EnterGuildPower, EnterGuildPowerArg, EnterGuildPowerRes)

void RpcM2G_EnterGuildPower::OnCall(const EnterGuildPowerArg &roArg, EnterGuildPowerRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	roRes.set_roleattr(pRole->GetAttr(TOTAL_POWERPOINT));


	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(GuildSysId))
	{
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
	}
	else
	{
		roRes.set_errorcode(KKSG::ERR_SUCCESS);
	}
}

void RpcM2G_EnterGuildPower::OnDelayReplyRpc(const EnterGuildPowerArg &roArg, EnterGuildPowerRes &roRes, const CUserData &roUserData)
{
}
