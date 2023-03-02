#include "pch.h"
#include "activity/rpcg2m_getnewzonebenefitg2m.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "role/rpcm2i_getzoneroleinfom2i.h"
#include "define/miscdef.h"
#include "network/loginlink.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2017/10/24 14:33:28

RPC_SERVER_IMPLEMETION(RpcG2M_GetNewZoneBenefitG2M, GetNewZoneBenefitG2MArg, GetNewZoneBenefitG2MRes)

void RpcG2M_GetNewZoneBenefitG2M::OnCall(const GetNewZoneBenefitG2MArg &roArg, GetNewZoneBenefitG2MRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		/*
		if (pRole->GetCacheZoneRole().size())
		{
			roRes.set_result(KKSG::ERR_SUCCESS);
			roRes.mutable_roles()->CopyFrom(pRole->GetCacheZoneRole());
			roRes.set_select_roleid(pRole->GetChargeBackRoleId());
		}
		else 
		*/
		{
			RpcM2I_GetZoneRoleInfoM2I* rpc = RpcM2I_GetZoneRoleInfoM2I::CreateRpc();
			rpc->m_oArg.set_opertype(ChargeBackOperType_Query);
			rpc->m_oArg.set_account(pRole->GetAccount());
			rpc->m_oArg.set_rpcid(DelayRpc());
			rpc->m_oArg.set_openservertime(GetGlobalConfig().NewZoneOpenTime);
			LoginLink::Instance()->SendTo(*rpc);
		}
	}
	else if (roArg.opertype() == ChargeBackOperType_Bind)
	{
		if (roArg.selectroleid() == 0)
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		if (pRole->GetChargeBackRoleId())
		{
			roRes.set_result(KKSG::ERR_ALREADY_CHARGE_BACK);
			return;
		}
		bool found = false;
		auto& roles = pRole->GetCacheZoneRole();
		for (int i = 0; i < roles.size(); ++i)
		{
			const KKSG::ZoneRoleInfo& info = roles.Get(i);
			if (info.roleid() != roArg.selectroleid())
			{
				continue;
			}
			found = true;
			RpcM2I_GetZoneRoleInfoM2I* rpc = RpcM2I_GetZoneRoleInfoM2I::CreateRpc();
			rpc->m_oArg.set_opertype(ChargeBackOperType_Bind);
			rpc->m_oArg.set_account(pRole->GetAccount());
			rpc->m_oArg.set_serverid(info.serverid());
			rpc->m_oArg.set_roleid(info.roleid());
			rpc->m_oArg.set_paycnt(pRole->GetAllZonePayCnt());
			rpc->m_oArg.set_rpcid(DelayRpc());
			LoginLink::Instance()->SendTo(*rpc);
			break;
		}
		if (found == false)
		{
			roRes.set_result(KKSG::ERR_FAILED);
		}
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}
}

void RpcG2M_GetNewZoneBenefitG2M::OnDelayReplyRpc(const GetNewZoneBenefitG2MArg &roArg, GetNewZoneBenefitG2MRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}

	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(code);
	if (code != KKSG::ERR_SUCCESS)
	{
		return;
	}

	KKSG::GetZoneRoleInfoM2IRes* res = (KKSG::GetZoneRoleInfoM2IRes*)(roUserData.m_pUserPtr);
	if (res == NULL)
	{
		return;
	}
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		roRes.mutable_roles()->CopyFrom(res->roles());
		roRes.set_select_roleid(res->bindroleid());
		roRes.set_bind_pay(res->bind_pay());
		pRole->SetCacheZoneRole(res->roles());
		pRole->SetChargeBackRoleId(res->bindroleid());
	}
	else if (roArg.opertype() == ChargeBackOperType_Bind)
	{
		pRole->SetChargeBackRoleId(roArg.selectroleid());
	}

}
