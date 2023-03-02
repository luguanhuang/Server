#include "pch.h"
#include "db/rpcm2d_saveleagueteamdata.h"
#include "db/mysqlmgr.h"
#include "db/task/leagueteaminserttask.h"
#include "db/task/leagueteamupdatetask.h"
#include "db/task/leagueteamdeltask.h"

// generate by ProtoGen at date: 2016/12/30 10:40:22

RPC_SERVER_IMPLEMETION(RpcM2D_SaveLeagueTeamData, SaveLeagueTeamDataArg, SaveLeagueTeamDataRes)

void RpcM2D_SaveLeagueTeamData::OnCall(const SaveLeagueTeamDataArg &roArg, SaveLeagueTeamDataRes &roRes)
{
	switch (roArg.type())
	{
	case KKSG::DBOper_Insert:
		{
			LeagueTeamInsertTask* task = new LeagueTeamInsertTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case KKSG::DBOper_Update:
		{
			LeagueTeamUpdateTask* task = new LeagueTeamUpdateTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case KKSG::DBOper_Del:
		{
			LeagueTeamDelTask* task = new LeagueTeamDelTask;
			task->m_rpcId = DelayRpc();
			task->m_id = roArg.data().id();
			task->m_isDeleteAll = false;
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_id));
			break;
		}
	case KKSG::DBOper_DelAll:
		{
			LeagueTeamDelTask* task = new LeagueTeamDelTask;
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

void RpcM2D_SaveLeagueTeamData::OnDelayReplyRpc(const SaveLeagueTeamDataArg &roArg, SaveLeagueTeamDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
