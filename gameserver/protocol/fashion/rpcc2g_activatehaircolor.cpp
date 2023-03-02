#include "pch.h"
#include "fashion/rpcc2g_activatehaircolor.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolefashion.h"

// generate by ProtoGen at date: 2017/6/13 20:38:43

RPC_SERVER_IMPLEMETION(RpcC2G_ActivateHairColor, ActivateHairColorArg, ActivateHairColorRes)

void RpcC2G_ActivateHairColor::OnCall(const ActivateHairColorArg &roArg, ActivateHairColorRes &roRes)
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

	record->ActivationHairColor(roArg.hair_id(),roArg.hair_color_id(),roRes);
}

void RpcC2G_ActivateHairColor::OnDelayReplyRpc(const ActivateHairColorArg &roArg, ActivateHairColorRes &roRes, const CUserData &roUserData)
{
}
