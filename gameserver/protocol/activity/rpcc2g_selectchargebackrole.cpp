#include "pch.h"
#include "activity/rpcc2g_selectchargebackrole.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "activity/rpcg2m_getnewzonebenefitg2m.h"
#include "define/miscdef.h"
#include "util/gametime.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2017/10/26 12:00:50

RPC_SERVER_IMPLEMETION(RpcC2G_SelectChargeBackRole, SelectChargeBackRoleArg, SelectChargeBackRoleRes)

void RpcC2G_SelectChargeBackRole::OnCall(const SelectChargeBackRoleArg &roArg, SelectChargeBackRoleRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	UINT32 now = GameTime::GetTime();
	if (now < GetGlobalConfig().NewZoneChargeBackTimeStartTime || now > GetGlobalConfig().NewZoneChargeBackTimeEndTime)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (roArg.roleid() == 0)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	RpcG2M_GetNewZoneBenefitG2M* rpc = RpcG2M_GetNewZoneBenefitG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_opertype(ChargeBackOperType_Bind);
	rpc->m_oArg.set_rpcid(DelayRpc());
	rpc->m_oArg.set_selectroleid(roArg.roleid());
	pRole->SendToMS(*rpc);
}

void RpcC2G_SelectChargeBackRole::OnDelayReplyRpc(const SelectChargeBackRoleArg &roArg, SelectChargeBackRoleRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(code);
}
