#include "pch.h"
#include "arenastar/rpcm2d_arenastarroledatasave.h"
#include "db/task/arenastartask.h"
#include "db/mysqlmgr.h"

// generate by ProtoGen at date: 2017/3/29 11:14:35

RPC_SERVER_IMPLEMETION(RpcM2D_ArenaStarRoleDataSave, ArenaStarDataSaveArg, ArenaStarDataSaveRes)

void RpcM2D_ArenaStarRoleDataSave::OnCall(const ArenaStarDataSaveArg &roArg, ArenaStarDataSaveRes &roRes)
{
	SSInfo << " save arenastarroledata size = " << roArg.roledata_size() << END;

	for(int i = 0; i < roArg.roledata_size(); ++i)
	{
		const auto& data = roArg.roledata(i);
		CArenaStarTask* pTask = new CArenaStarTask(data);
		CMysqlMgr::Instance()->PushTask(pTask);
	}
}

void RpcM2D_ArenaStarRoleDataSave::OnDelayReplyRpc(const ArenaStarDataSaveArg &roArg, ArenaStarDataSaveRes &roRes, const CUserData &roUserData)
{
}
