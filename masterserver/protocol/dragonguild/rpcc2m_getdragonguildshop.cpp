#include "pch.h"
#include "dragonguild/rpcc2m_getdragonguildshop.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguildmgr.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2017/9/12 18:31:20

RPC_SERVER_IMPLEMETION(RpcC2M_GetDragonGuildShop, GetDragonGuildShopArg, GetDragonGuildShopRes)

void RpcC2M_GetDragonGuildShop::OnCall(const GetDragonGuildShopArg &roArg, GetDragonGuildShopRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	DragonGuild* pDguild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDguild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return ;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	DragonGuildShop& shop = pDguild->GetDragonGuildShop();
	shop.FillShopItems(pRole->GetID(), roRes);
	DragonGuildMember* m = pDguild->GetMember(pRole->GetID());
	if (m)
	{
		m->openShopTime = GameTime::GetTime();
	}
}

void RpcC2M_GetDragonGuildShop::OnDelayReplyRpc(const GetDragonGuildShopArg &roArg, GetDragonGuildShopRes &roRes, const CUserData &roUserData)
{
}
