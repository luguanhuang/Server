#include "pch.h"
#include "role/rpci2m_getzoneroleinfoi2m.h"
#include "config.h"
#include "role/rpcm2d_readaccountdatanew.h"
#include "network/dblink.h"
#include "define/miscdef.h"
#include "pay/paymgr.h"
#include "event/eventmgr.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2017/10/24 15:13:26

RPC_SERVER_IMPLEMETION(RpcI2M_GetZoneRoleInfoI2M, GetZoneRoleInfoI2MArg, GetZoneRoleInfoI2MRes)

void RpcI2M_GetZoneRoleInfoI2M::OnCall(const GetZoneRoleInfoI2MArg &roArg, GetZoneRoleInfoI2MRes &roRes)
{
	if (!MSConfig::Instance()->HasServerId(roArg.serverid()))
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		RpcM2D_ReadAccountDataNew* rpc = RpcM2D_ReadAccountDataNew::CreateRpc();
		rpc->m_oArg.set_opertype(ChargeBackOperType_Query);
		rpc->m_oArg.set_account(roArg.account());
		rpc->m_oArg.set_serverid(roArg.serverid());
		rpc->m_oArg.set_rpcid(DelayRpc());
		DBLink::Instance()->SendTo(*rpc);
	}
	else if (roArg.opertype() == ChargeBackOperType_Bind)
	{
		CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(roArg.roleid());
		if (pSu == NULL)
		{
			roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
			return;
		}
		EventMgr::Instance()->AddEvent(roArg.roleid(), TASK_EVENT_OFFSET + TASK_EVENT_CHARGE_BACK, roArg.paycnt());
		roRes.set_result(KKSG::ERR_SUCCESS);
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}
}

void RpcI2M_GetZoneRoleInfoI2M::OnDelayReplyRpc(const GetZoneRoleInfoI2MArg &roArg, GetZoneRoleInfoI2MRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(code);
	if (code != KKSG::ERR_SUCCESS)
	{
		return;
	}

	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		ReadAccountDataNewRes* res = (ReadAccountDataNewRes*)(roUserData.m_pUserPtr);
		if (res == NULL)
		{
			return;
		}
		for (int i = 0; i < res->roles_size(); ++i)
		{
			const KKSG::ZoneRoleInfo& roleinfo = res->roles(i);
			KKSG::ZoneRoleInfo* pInfo = roRes.add_roles();
			pInfo->CopyFrom(roleinfo);

			pInfo->set_opentime(MSConfig::Instance()->GetGameServerOpenTimeStamp());
			pInfo->set_paycnt(CPayMgr::Instance()->GetSaveAmt(pInfo->roleid()));
		}
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}
}
