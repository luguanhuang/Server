#include "pch.h"
#include "role/rpcm2d_msgetallrole.h"
#include "db/task/roleallinforeadtask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2016/7/21 0:18:30

RPC_SERVER_IMPLEMETION(RpcM2D_MsGetAllRole, GetAllRoleArg, GetAllRoleRes)

void RpcM2D_MsGetAllRole::OnCall(const GetAllRoleArg &roArg, GetAllRoleRes &roRes)
{
	CRoleAllInfoReadTask* poTask = new CRoleAllInfoReadTask();
	poTask->m_dwRpcDelayed = DelayRpc();
	poTask->m_szAccount = roArg.account();
	poTask->m_ServerID = roArg.serverid();
	CMysqlMgr::Instance()->PushTask(poTask, GetAccountThreadIndex());
}

void RpcM2D_MsGetAllRole::OnDelayReplyRpc(const GetAllRoleArg &roArg, GetAllRoleRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
	GetAllRoleRes* pstRole = (GetAllRoleRes*)roUserData.m_pUserPtr;
	for(auto i = 0; i != pstRole->roleallinfolist_size(); ++i)
	{
		KKSG::RoleAllInfo* poInfo = roRes.add_roleallinfolist();
		poInfo->CopyFrom(pstRole->roleallinfolist(i));
	}
}
