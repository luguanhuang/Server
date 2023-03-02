#include "pch.h"
#include "role/rpcn2m_checkrolebaninfo.h"
#include "role/rpcn2d_nsreadroledata.h"
#include "network/dblink.h"
#include "account/accountsessionmgr.h"

// generate by ProtoGen at date: 2016/11/9 10:17:54

RPC_CLIENT_IMPLEMETION(RpcN2M_CheckRoleBanInfo, CheckRoleBanInfoArg, CheckRoleBanInfoRes)

void RpcN2M_CheckRoleBanInfo::OnReply(const CheckRoleBanInfoArg &roArg, const CheckRoleBanInfoRes &roRes, const CUserData &roUserData)
{
	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		CUserData oUser;
		oUser.m_dwUserData = (UINT32)roRes.result();
		if(roRes.result() == KKSG::ERR_LOGIN_FORBID||roRes.result()==KKSG::ERR_PLAT_BANACC || roRes.result()==KKSG::ERR_HG_FORBID)
		{
			oUser.m_pUserPtr = const_cast<CheckRoleBanInfoRes*>(&roRes);
		}

		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
		return;
	}

	//Check session exist?
	CAccountSession* poAccount = CAccountSessionMgr::Instance()->GetBySession(m_sessionID);
	if(NULL == poAccount)
	{
		LogError("Session [%llu] was closed when check role ban info from ms reply, role [%llu]", m_sessionID, roArg.roleid());

		CUserData oUser;
		oUser.m_dwUserData = (UINT32)KKSG::ERR_STATE_ERROR;
		CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
		return;
	}

	RpcN2D_NsReadRoleData* rpc = RpcN2D_NsReadRoleData::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_index(roArg.index());
	rpc->m_oArg.set_roleid(roArg.roleid());
	rpc->m_oArg.set_readtype(KKSG::ROLE_DATA_READ_SELECT_ROLE);
	rpc->m_oArg.set_rpcid(roArg.rpcid());
	DBLink::Instance()->SendTo(*rpc);
}

void RpcN2M_CheckRoleBanInfo::OnTimeout(const CheckRoleBanInfoArg &roArg, const CUserData &roUserData)
{
	CUserData oUser;
	oUser.m_dwUserData = (UINT32)KKSG::ERR_TIMEOUT;
	CRpc::ReplyDelayRpc(roArg.rpcid(), oUser);
}
