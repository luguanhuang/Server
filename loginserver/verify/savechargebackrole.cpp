#include "pch.h"
#include "savechargebackrole.h"

#include "crpc.h"
#include "SQLStmt.h"
#include "verify/logincontrol.h"
#include "gmmgr/gmmgr.h"
#include "gatemgr/gatemgr.h"
#include "role/rpci2m_getzoneroleinfoi2m.h"
#include "define/miscdef.h"
#include "network/mslink.h"

SaveChargeBackRole::SaveChargeBackRole()
:m_rpcid(0)
,m_serverID(0)
,m_roleID(0)
,m_payCnt(0)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
}

SaveChargeBackRole::~SaveChargeBackRole()
{
}

int SaveChargeBackRole::Execute(MYSQL *mysql)
{
	{
		const char query[] = "replace into charge_back(openid, serverid, roleid, total_pay) values(?, ?, ?, ?)";
		SQLStmt<4, 0> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindIn(&m_serverID);
		stmt.BindIn(&m_roleID);
		stmt.BindIn(&m_payCnt);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
	}

	return 0;
}

void SaveChargeBackRole::FinishTask()
{
	if (m_result != 0)
	{
		LogError("sql error[%d]", m_result);
		CUserData oUserData;
		oUserData.m_dwUserData = KKSG::ERR_FAILED;
		oUserData.m_pUserPtr = NULL;
		CRpc::ReplyDelayRpc(m_rpcid, oUserData);
		return;
	}

	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 200)
	{
		LogWarn("task finish use time %llu ms", qwTimeUsed);
	}

	if (!MsLink::Instance()->IsServerConnected(m_serverID))
	{
		CUserData oUserData(KKSG::ERR_SERVER_UNNORMAL, NULL);
		CRpc::ReplyDelayRpc(m_rpcid, oUserData);
		return;
	}

	RpcI2M_GetZoneRoleInfoI2M* rpc = RpcI2M_GetZoneRoleInfoI2M::CreateRpc();
	rpc->m_oArg.set_opertype(ChargeBackOperType_Bind);
	rpc->m_oArg.set_account(m_account);
	rpc->m_oArg.set_roleid(m_roleID);
	rpc->m_oArg.set_rpcid(m_rpcid);
	rpc->m_oArg.set_paycnt(m_payCnt);
	rpc->m_oArg.set_serverid(m_serverID);
	MsLink::Instance()->SendToServer(m_serverID, *rpc);
}
