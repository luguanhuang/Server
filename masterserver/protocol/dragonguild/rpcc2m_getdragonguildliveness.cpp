#include "pch.h"
#include "dragonguild/rpcc2m_getdragonguildliveness.h"
#include "dragonguild/dragonguildmgr.h"
#include "role/rolemanager.h"
#include "dragonguild/rpcm2g_getdragonguildrecordm2g.h"
// generate by ProtoGen at date: 2017/9/5 14:50:34

RPC_SERVER_IMPLEMETION(RpcC2M_GetDragonGuildLiveness, GetPartnerLivenessArg, GetPartnerLivenessRes)

void RpcC2M_GetDragonGuildLiveness::OnCall(const GetPartnerLivenessArg &roArg, GetPartnerLivenessRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	DragonGuild* dragonGd = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (dragonGd == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	
	RpcM2G_GetDragonGuildRecordM2G* rpc = RpcM2G_GetDragonGuildRecordM2G::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_partnerid(dragonGd->GetID());
	rpc->m_oArg.set_level(dragonGd->GetLevel());
	rpc->m_oArg.set_liveness(dragonGd->GetLiveness());
	rpc->m_oArg.set_shop_refresh_time(dragonGd->GetDragonGuildShop().GetLastRefreshTime());
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendMsgToGS(*rpc);
	
}

void RpcC2M_GetDragonGuildLiveness::OnDelayReplyRpc(const GetPartnerLivenessArg &roArg, GetPartnerLivenessRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("can't find role by session[%llu]", m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}
	DragonGuild* record = DragonGuildMgr::Instance()->GetDragonGuildByRole(pRole->GetID());
	if (record == NULL)
	{
		roRes.set_result(KKSG::ERR_PARTNER_NO_PARTNER);
		return;
	}
	if (roUserData.m_pUserPtr == NULL)
	{
		roRes.set_result((KKSG::ErrorCode)roUserData.m_dwUserData);
		return;	
	}
	
	roRes.set_result(KKSG::ERR_SUCCESS);
	KKSG::GetPartnerRecordM2GRes* res = (KKSG::GetPartnerRecordM2GRes*)(roUserData.m_pUserPtr);
	roRes.set_takedchest(res->takedchest());
	DragonGuildLiveness& ln = record->GetDragonGuildLiveness();
	roRes.set_liveness(ln.GetLiveness());
	ln.FillLivenessRecord(roRes);
	
}
