#include "pch.h"
#include "dragonguild/rpcc2m_getdragonguildshoprecord.h"
#include "role/rolemanager.h"
#include "util/gametime.h"
#include "dragonguild/dragonguildmgr.h"

// generate by ProtoGen at date: 2017/9/12 20:02:15

RPC_SERVER_IMPLEMETION(RpcC2M_GetDragonGuildShopRecord, GetDragonGuildShopRecordArg, GetDragonGuildShopRecordRes)

void RpcC2M_GetDragonGuildShopRecord::OnCall(const GetDragonGuildShopRecordArg &roArg, GetDragonGuildShopRecordRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (pDGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
		return ;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
	DragonGuildShop& shop = pDGuild->GetDragonGuildShop();
	shop.FillShopRecords(roRes);
}

void RpcC2M_GetDragonGuildShopRecord::OnDelayReplyRpc(const GetDragonGuildShopRecordArg &roArg, GetDragonGuildShopRecordRes &roRes, const CUserData &roUserData)
{
}
