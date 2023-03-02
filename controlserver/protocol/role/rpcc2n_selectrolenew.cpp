#include "pch.h"
#include "role/rpcc2n_selectrolenew.h"
#include "role/rpcn2m_checkrolebaninfo.h"
#include "account/accountsessionmgr.h"
#include "role/rolemanager.h"
#include "queuing/queuingmgr.h"
#include "network/mslink.h"
#include "login/ptcn2t_notifygtsessionlogin.h"
#include "table/globalconfig.h"

// generate by ProtoGen at date: 2016/11/3 2:19:45

RPC_SERVER_IMPLEMETION(RpcC2N_SelectRoleNew, SelectRoleNewArg, SelectRoleNewRes)

void RpcC2N_SelectRoleNew::OnCall(const SelectRoleNewArg &roArg, SelectRoleNewRes &roRes)
{
	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if (NULL == poAccount)
	{
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}

	if(!MSLink::Instance()->IsConnected())
	{
		roRes.set_result(KKSG::ERR_MS_UNREADY);
		return;
	}

	if (QueuingMgr::Instance()->IsWaiting(poAccount->GetAccountID()))
	{
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}

	if (roArg.index() + 1 > GetGlobalConfig().MaxRole)
	{
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}

	UINT64 qwRoleID = CAccountSessionMgr::Instance()->GetRoleIDByIndex(m_sessionID, roArg.index());
	if(qwRoleID == 0)
	{
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return;
	}

	CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
	if(poRole != NULL)
	{
		LogError("Account [%s], Role [%llu] has been login, the request session [%llu]", poAccount->GetAccountID().c_str(), qwRoleID, m_sessionID);
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(poRole != NULL)
	{
		LogError("Account [%s], Session [%llu] has been login, the request roleId [%llu]", poAccount->GetAccountID().c_str(), m_sessionID, qwRoleID);
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	if(!poAccount->CheckSelectRole())
	{
		LogWarn("Account [%s] select role too fast", poAccount->GetAccountID().c_str());
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	poAccount->OnSelectRoleBegin();

	RpcN2M_CheckRoleBanInfo* rpc = RpcN2M_CheckRoleBanInfo::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_roleid(qwRoleID);
	rpc->m_oArg.set_index(roArg.index());
	rpc->m_oArg.set_openid(poAccount->GetAccountID());
	rpc->m_oArg.set_rpcid(DelayRpc());
	MSLink::Instance()->SendTo(*rpc);
}

void RpcC2N_SelectRoleNew::OnDelayReplyRpc(const SelectRoleNewArg &roArg, SelectRoleNewRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);

	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if(poAccount != NULL)
	{
		poAccount->OnSelectRoleEnd();
	}

	if(nResult == KKSG::ERR_SUCCESS)
	{
		CAccountSessionMgr::Instance()->NotifyUsedTokenToLoginServer(m_sessionID);

		UINT64 qwRoleID = CAccountSessionMgr::Instance()->GetRoleIDByIndex(m_sessionID, roArg.index());
		CRole* poRole = CRoleManager::Instance()->GetByRoleID(qwRoleID);
		if(poRole == NULL)
		{
			LogError("can't find role [%llu], session [%llu] when select role finished", qwRoleID, m_sessionID);
			return;
		}

		// 通知Gate登录状态
		PtcN2T_NotifyGtSessionLogin ntf;
		ntf.m_sessionID = m_sessionID;
		poRole->Send(ntf);
	}
	else
	{
		if(nResult == KKSG::ERR_LOGIN_FORBID)
		{
			CheckRoleBanInfoRes* poBanInfoRes = (CheckRoleBanInfoRes*)roUserData.m_pUserPtr;
			if(poBanInfoRes != NULL)
			{
				roRes.set_bantime(poBanInfoRes->bantime());
				roRes.set_endtime(poBanInfoRes->endtime());
			}
		}else if(nResult ==  KKSG::ERR_PLAT_BANACC || nResult == KKSG::ERR_HG_FORBID)
		{
			CheckRoleBanInfoRes* poBanInfoRes = (CheckRoleBanInfoRes*)roUserData.m_pUserPtr;
			if(poBanInfoRes != NULL)
			{
				roRes.set_reason(poBanInfoRes->reason());
				roRes.set_endtime(poBanInfoRes->endtime());
			}
		}
		//CAccountSessionMgr::Instance()->ReleaseRole(m_sessionID); 不会调用到
	}
}
