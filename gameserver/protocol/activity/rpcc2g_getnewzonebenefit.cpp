#include "pch.h"
#include "activity/rpcc2g_getnewzonebenefit.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "activity/rpcg2m_getnewzonebenefitg2m.h"
#include "define/miscdef.h"
#include "table/globalconfig.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2017/10/24 10:40:03

RPC_SERVER_IMPLEMETION(RpcC2G_GetNewZoneBenefit, GetNewZoneBenefitArg, GetNewZoneBenefitRes)

void RpcC2G_GetNewZoneBenefit::OnCall(const GetNewZoneBenefitArg &roArg, GetNewZoneBenefitRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	RpcG2M_GetNewZoneBenefitG2M* rpc = RpcG2M_GetNewZoneBenefitG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_opertype(ChargeBackOperType_Query);
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendToMS(*rpc);
}

void RpcC2G_GetNewZoneBenefit::OnDelayReplyRpc(const GetNewZoneBenefitArg &roArg, GetNewZoneBenefitRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(code);
	if (code != KKSG::ERR_SUCCESS)
	{
		return;
	}
	KKSG::GetNewZoneBenefitG2MRes* res = (KKSG::GetNewZoneBenefitG2MRes*)(roUserData.m_pUserPtr);
	if (res == NULL)
	{
		return;
	}
	roRes.set_select_roleid(res->select_roleid());
	UINT32 totalCnt = 0;
	for (int i = 0; i < res->roles_size(); ++i)
	{
		const KKSG::ZoneRoleInfo& info = res->roles(i);
		totalCnt += info.paycnt();
		if (info.opentime() >= GetGlobalConfig().NewZoneOpenTime)
		{
			roRes.add_roles()->CopyFrom(info);
		}
	}
	if (res->select_roleid())
	{
		roRes.set_total_paycnt(res->bind_pay());
	}
	else
	{
		roRes.set_total_paycnt(totalCnt);
	}
	roRes.set_has_select(res->select_roleid() ? true : false);
	UINT32 now = GameTime::GetTime();
	if (now < GetGlobalConfig().NewZoneChargeBackTimeStartTime || now > GetGlobalConfig().NewZoneChargeBackTimeEndTime)
	{
		roRes.set_is_open(false);
	}
	else
	{
		roRes.set_is_open(true);
	}
}











