#include "pch.h"
#include "ibshop/rpcc2g_queryibitem.h"
#include "gamelogic/ibshopmgr.h"
#include "unit/rolemanager.h"
#include "pay/rpcg2m_paydegreereq.h"
#include "unit/role.h"
#include "gamelogic/ibshoprecord.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/9/1 16:58:01

RPC_SERVER_IMPLEMETION(RpcC2G_QueryIBItem, IBQueryItemReq, IBQueryItemRes)

void RpcC2G_QueryIBItem::OnCall(const IBQueryItemReq &roArg, IBQueryItemRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}

	CRoleSystem* pRoleSys = pRole->Get<CRoleSystem>();
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	UINT32 nTime = GameTime::GetTime();
	if (pRoleSys->IsSystemOpened(SYS_IBSHOP_GIFT) == true && pRecord->GetPayDegree() < IBShopMgr::Instance()->GetMaxPlayerDegree()&&nTime>pRecord->m_nPullTime)
	{
		RpcG2M_PayDegreeReq *pRpc = RpcG2M_PayDegreeReq::CreateRpc();
		pRpc->m_oArg.set_roleid(pRole->GetID());
		pRpc->m_oArg.set_rpcid(DelayRpc());
		pRole->SendToMS(*pRpc);
		pRecord->m_nPullTime = GameTime::GetTime()+300;
	}else
	{
		IBShopMgr::Instance()->PackIBItems(pRole,roArg.type(),roArg.subtype(),roRes);
	}
}

void RpcC2G_QueryIBItem::OnDelayReplyRpc(const IBQueryItemReq &roArg, IBQueryItemRes &roRes, const CUserData &roUserData)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		SSWarn<<"role is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	IBShopMgr::Instance()->PackIBItems(pRole,roArg.type(),roArg.subtype(),roRes);
}
