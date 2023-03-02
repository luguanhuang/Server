#include "pch.h"
#include "db/mysqlmgr.h"
#include "garden/rpcm2d_querygardeninfo.h"
#include "db/task/query_gardenInfo_task.h"
// generate by ProtoGen at date: 2017/4/27 10:59:37

RPC_SERVER_IMPLEMETION(RpcM2D_QueryGardenInfo, QueryGardenInfoArg, QueryGardenInfoRes)

void RpcM2D_QueryGardenInfo::OnCall(const QueryGardenInfoArg &roArg, QueryGardenInfoRes &roRes)
{
	CQueryGardenInfoTask* poTask = new CQueryGardenInfoTask();
	poTask->m_qwRoleID = roArg.role_id();
	poTask->m_dwRpcDelayed = DelayRpc();
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.role_id()));
	SSInfo<<"query garden info, roleid:"<<roArg.role_id()<<END;
}

void RpcM2D_QueryGardenInfo::OnDelayReplyRpc(const QueryGardenInfoArg &roArg, QueryGardenInfoRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	KKSG::GardenInfo * data = (KKSG::GardenInfo*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && data != NULL)
	{
		roRes.mutable_garden_info()->CopyFrom(*data);
	}
}
