#include "pch.h"
#include "role/rpcn2d_createroleondbnew.h"
#include "db/mysqlmgr.h"
#include "db/task/roleinserttask.h"

// generate by ProtoGen at date: 2016/11/3 1:56:57

RPC_SERVER_IMPLEMETION(RpcN2D_CreateRoleOnDBNew, CreateRoleOnDBNewArg, CreateRoleOnDBNewRes)

void RpcN2D_CreateRoleOnDBNew::OnCall(const CreateRoleOnDBNewArg &roArg, CreateRoleOnDBNewRes &roRes)
{
	CRoleInsertTask* poTask = new CRoleInsertTask();
	poTask->m_oRoleInfo = roArg.roledata();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetAccountThreadIndex());
}

void RpcN2D_CreateRoleOnDBNew::OnDelayReplyRpc(const CreateRoleOnDBNewArg &roArg, CreateRoleOnDBNewRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);

	KKSG::RoleAllInfo* poRoleInfo = (KKSG::RoleAllInfo*)roUserData.m_pUserPtr;
	if(nResult == KKSG::ERR_SUCCESS && poRoleInfo != NULL)
	{
		roRes.set_nickid(poRoleInfo->brief().nickid());
	}
}
