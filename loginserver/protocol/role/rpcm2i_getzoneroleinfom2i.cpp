#include "pch.h"
#include "role/rpcm2i_getzoneroleinfom2i.h"
#include "define/miscdef.h"
#include "verify/getselfservertasknew.h"
#include "verify/tokenverifymgr.h"
#include "serverrole/readrolecontrol.h"
#include "verify/savechargebackrole.h"
#include "gatemgr/gatemgr.h"

// generate by ProtoGen at date: 2017/10/24 15:01:59

RPC_SERVER_IMPLEMETION(RpcM2I_GetZoneRoleInfoM2I, GetZoneRoleInfoM2IArg, GetZoneRoleInfoM2IRes)

void RpcM2I_GetZoneRoleInfoM2I::OnCall(const GetZoneRoleInfoM2IArg &roArg, GetZoneRoleInfoM2IRes &roRes)
{
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		GetSelfServerTaskNew* pTask = new GetSelfServerTaskNew(DelayRpc(), roArg.account());
		TokenVerifyMgr::Instance()->AddDBTask(pTask);
	}
	else if (roArg.opertype() == ChargeBackOperType_Bind)
	{
		SaveChargeBackRole* pTask = new SaveChargeBackRole;
		pTask->m_rpcid = DelayRpc();
		pTask->m_account = roArg.account();
		pTask->m_serverID = roArg.serverid();
		pTask->m_roleID  = roArg.roleid();
		pTask->m_payCnt = roArg.paycnt();
		TokenVerifyMgr::Instance()->AddDBTask(pTask);
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}
}

void RpcM2I_GetZoneRoleInfoM2I::OnDelayReplyRpc(const GetZoneRoleInfoM2IArg &roArg, GetZoneRoleInfoM2IRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(code);
	if (code != KKSG::ERR_SUCCESS)
	{
		return;
	}

	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		ReadRoleInfo* pInfo = (ReadRoleInfo*)roUserData.m_pUserPtr;
		if (pInfo == NULL)
		{
			return;
		}
		roRes.set_bindroleid(pInfo->m_bindRoleId);
		roRes.set_bind_pay(pInfo->m_bindPay);
		for (auto it1 = pInfo->m_zoneRoles.begin(); it1 != pInfo->m_zoneRoles.end(); ++it1)
		{
			OneZoneRoleInfo& info = it1->second;
			for (auto it2 = info.m_roles.begin(); it2 != info.m_roles.end(); ++it2)
			{
				roRes.add_roles()->CopyFrom(*it2);
			}
		}
		auto& servers = GateMgr::Instance()->GetAllServer();
		for (auto it = servers.begin(); it != servers.end(); ++it)
		{
			GateInfo& sinfo = it->second;
			if (sinfo.openTime <= roArg.openservertime())
			{
				continue;
			}
			UINT32 serverID = it->first;
			if (pInfo->m_zoneRoles.find(serverID) != pInfo->m_zoneRoles.end())
			{
				continue;
			}
			KKSG::ZoneRoleInfo* info = roRes.add_roles();
			info->set_serverid(serverID);
			info->set_servername(sinfo.serverName);
			info->set_opentime(sinfo.openTime);
		}
	}
	else if (roArg.opertype() == ChargeBackOperType_Bind)
	{

	}
}
