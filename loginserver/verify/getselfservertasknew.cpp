#include "pch.h"
#include "getselfservertasknew.h"
#include "crpc.h"
#include "SQLStmt.h"
#include "verify/logincontrol.h"
#include "gmmgr/gmmgr.h"
#include "gatemgr/gatemgr.h"
#include "network/mslink.h"
#include "serverrole/readrolecontrol.h"
#include "role/rpci2m_getzoneroleinfoi2m.h"
#include "define/miscdef.h"

GetSelfServerTaskNew::GetSelfServerTaskNew(UINT32 rpcid, const std::string& account)
:m_rpcid(rpcid)
,m_account(account)
,m_bindRoleId(0)
,m_bindPay(0)
{
	m_qwBeginTime = TimeUtil::GetMilliSecond();
}

int GetSelfServerTaskNew::Execute(MYSQL *mysql)
{
	{
		const char query[] = "select roleid, total_pay from charge_back where openid=?";
		SQLStmt<1,2> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&m_bindRoleId);
		stmt.BindOut(&m_bindPay);
		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}
		stmt.FetchResult();
	}

	{
		const char query[] = "select serverid from selfserver where userid=?";
		SQLStmt<1,1> stmt(mysql);
		if (!stmt.SetQuery(query, sizeof(query)))
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		int serverid = 0;
		stmt.BindIn(&m_account[0], m_account.size());
		stmt.BindOut(&serverid);

		if (!stmt.Execute())
		{
			stmt.ReportError();
			return stmt.GetError();
		}

		while (stmt.FetchResult())
		{
			m_serverIds.insert(serverid);
		}	
	}

	return 0;
}

void GetSelfServerTaskNew::FinishTask()
{
	if (m_result != 0)
	{
		LogError("sql error[%d]", m_result);
		CUserData data(KKSG::ERR_FAILED, NULL);
		CRpc::ReplyDelayRpc(m_rpcid, data);
		return;
	}

	UINT64 qwEndTime = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = qwEndTime >= m_qwBeginTime ? qwEndTime - m_qwBeginTime : 0;
	if(qwTimeUsed > 200)
	{
		LogWarn("Get self server task finish use time %llu ms", qwTimeUsed);
	}

	ReadRoleInfo* pInfo = ReadRoleControl::Instance()->NewReadRole(m_account);
	if (pInfo == NULL)
	{
		CUserData data(KKSG::ERR_FAILED, NULL);
		CRpc::ReplyDelayRpc(m_rpcid, data);
		return;
	}

	for (auto it = m_serverIds.begin(); it != m_serverIds.end(); ++it)
	{
		UINT32 serverID = *it;
		if (!MsLink::Instance()->IsServerConnected(serverID))
		{
			LogError("account[%s] has role in server[%u], but server not open", m_account.c_str(), serverID);
			continue;
		}
		pInfo->m_zoneRoles.insert(std::make_pair(serverID, OneZoneRoleInfo()));
		RpcI2M_GetZoneRoleInfoI2M* rpc = RpcI2M_GetZoneRoleInfoI2M::CreateRpc();
		rpc->m_oArg.set_opertype(ChargeBackOperType_Query);
		rpc->m_oArg.set_account(m_account);
		rpc->m_oArg.set_serverid(serverID);
		MsLink::Instance()->SendToServer(serverID, *rpc);
	}	
	pInfo->m_bindRoleId = m_bindRoleId;
	pInfo->m_bindPay = m_bindPay;
	pInfo->m_rpcId = m_rpcid;

	if (pInfo->m_zoneRoles.empty())
	{
		CUserData data(KKSG::ERR_SUCCESS, (void*)pInfo);
		CRpc::ReplyDelayRpc(m_rpcid, data);
		ReadRoleControl::Instance()->RemoveReadRole(m_account);
	}
}

