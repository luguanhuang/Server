#include "pch.h"
#include "fashion/rpcc2g_choosespecialeffects.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolefashion.h"

// generate by ProtoGen at date: 2017/10/16 11:31:07

RPC_SERVER_IMPLEMETION(RpcC2G_ChooseSpecialEffects, ChooseSpecialEffectsArg, ChooseSpecialEffectsRes)

void RpcC2G_ChooseSpecialEffects::OnCall(const ChooseSpecialEffectsArg &roArg, ChooseSpecialEffectsRes &roRes)
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

	record->ChooseSpecialEffects(roArg.special_effects_id(),roRes);
}

void RpcC2G_ChooseSpecialEffects::OnDelayReplyRpc(const ChooseSpecialEffectsArg &roArg, ChooseSpecialEffectsRes &roRes, const CUserData &roUserData)
{
}
