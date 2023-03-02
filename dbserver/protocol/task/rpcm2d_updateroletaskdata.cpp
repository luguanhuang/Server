#include "pch.h"
#include "task/rpcm2d_updateroletaskdata.h"
#include "db/mysqlmgr.h"
#include "db/task/roletaskupdatetask.h"
#include "db/task/roletaskdeletetask.h"

// generate by ProtoGen at date: 2017/9/20 7:41:32

RPC_SERVER_IMPLEMETION(RpcM2D_UpdateRoleTaskData, UpdateRoleTaskDataArg, UpdateRoleTaskDataRes)

void RpcM2D_UpdateRoleTaskData::OnCall(const UpdateRoleTaskDataArg &roArg, UpdateRoleTaskDataRes &roRes)
{
	switch (roArg.type())
	{
	case KKSG::DBOper_Insert:
	case KKSG::DBOper_Update:
		{
			RoleTaskUpdateTask* task = new RoleTaskUpdateTask;
			task->m_rpcId = DelayRpc();
			task->m_roleId = roArg.roleid();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_roleId));
			break;
		}
	case KKSG::DBOper_Del:
		{
			RoleTaskDelTask* task = new RoleTaskDelTask;
			task->m_rpcId = DelayRpc();
			task->m_roleId =  roArg.roleid();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_roleId));
			break;
		}
	default:
		LogError("invalid db type[%u]", roArg.type());
		roRes.set_result(KKSG::ERR_FAILED);
	}
}

void RpcM2D_UpdateRoleTaskData::OnDelayReplyRpc(const UpdateRoleTaskDataArg &roArg, UpdateRoleTaskDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
