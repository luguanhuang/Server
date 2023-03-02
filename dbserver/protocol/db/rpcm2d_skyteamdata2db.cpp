#include "pch.h"
#include "db/rpcm2d_skyteamdata2db.h"
#include "db/task/skyteaminserttask.h"
#include "db/task/skyteamupdatetask.h"
#include "db/task/skyteamdeltask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/3/30 10:36:10

RPC_SERVER_IMPLEMETION(RpcM2D_SkyTeamData2DB, SkyTeamData2DBArg, SkyTeamData2DBRes)

void RpcM2D_SkyTeamData2DB::OnCall(const SkyTeamData2DBArg &roArg, SkyTeamData2DBRes &roRes)
{
	switch (roArg.type())
	{
	case KKSG::DBOper_Insert:
		{
			SkyTeamInsertTask* task = new SkyTeamInsertTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case KKSG::DBOper_Update:
		{
			SkyTeamUpdateTask* task = new SkyTeamUpdateTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case KKSG::DBOper_Del:
		{
			SkyTeamDelTask* task = new SkyTeamDelTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_isDeleteAll = false;
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case KKSG::DBOper_DelAll:
		{
			SkyTeamDelTask* task = new SkyTeamDelTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_isDeleteAll = true;
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	default:
		LogError("invalid db type[%u]", roArg.type());
		roRes.set_result(KKSG::ERR_FAILED);
	}
}

void RpcM2D_SkyTeamData2DB::OnDelayReplyRpc(const SkyTeamData2DBArg &roArg, SkyTeamData2DBRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
