#include "pch.h"
#include "pay/rpcc2g_activatepreshow.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "tablemgr/prerogativemgr.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/payv2Record.h"

// generate by ProtoGen at date: 2017/10/9 11:48:54

RPC_SERVER_IMPLEMETION(RpcC2G_ActivatePreShow, ActivatePreShowArg, ActivatePreShowRes)

void RpcC2G_ActivatePreShow::OnCall(const ActivatePreShowArg &roArg, ActivatePreShowRes &roRes)
{
	roRes.set_result(KKSG::ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	if (pRole->Get<CPayV2Record>()->IsActivatePre(roArg.id()))
	{
		SSError<<"already activate, roleid="<<pRole->GetID()<<",preid="<<roArg.id()<<END;
		return;
	}

	auto it = CPreprogativeConfig::Instance()->GetPrerogativeConf(roArg.id());
	if (it == NULL || it->Item[0] == 0 || it->Item[1] == 0)
	{
		SSError<<"roleid="<<pRole->GetID()<<",invalid preid="<<roArg.id()<<END;
		return;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_UseItem, ItemFlow_ActivatePre);
	if (!take.TakeItem(it->Item[0], it->Item[1]))
	{
		take.RollBack();
		roRes.set_result(KKSG::ERR_ITEM_NOT_ENOUGH);
		return;
	}
	take.NotifyClient();

	roRes.set_result(KKSG::ERR_SUCCESS);
	pRole->Get<CPayV2Record>()->ActivatePre(roArg.id());
}

void RpcC2G_ActivatePreShow::OnDelayReplyRpc(const ActivatePreShowArg &roArg, ActivatePreShowRes &roRes, const CUserData &roUserData)
{
}
