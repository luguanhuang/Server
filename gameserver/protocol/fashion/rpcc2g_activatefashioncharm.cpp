#include "pch.h"
#include "fashion/rpcc2g_activatefashioncharm.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolefashion.h"

// generate by ProtoGen at date: 2017/4/20 16:13:26

RPC_SERVER_IMPLEMETION(RpcC2G_ActivateFashionCharm, ActivateFashionArg, ActivateFashionRes)

	void RpcC2G_ActivateFashionCharm::OnCall(const ActivateFashionArg &roArg, ActivateFashionRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_result(ERR_INVALID_REQUEST);
		return;
	}
	///> system open
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SYS_FASHION_DISPLAY))
	{
		SSInfo<<"system close"<<END;
		roRes.set_result(ERR_SYS_NOTOPEN);
		return;
	}
	RoleFashion* record = pRole->Get<RoleFashion>();

	UINT32 real_count =	record->ActiveFashionCharm(roArg.suit_id(),roRes);
	roRes.set_active_count(real_count);
}

void RpcC2G_ActivateFashionCharm::OnDelayReplyRpc(const ActivateFashionArg &roArg, ActivateFashionRes &roRes, const CUserData &roUserData)
{
}
