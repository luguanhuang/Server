#include "pch.h"
#include "task/rpcg2m_dailytaskrefreshg2m.h"
#include "task/roletaskmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"
#include "util/gametime.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"
#include "define/taskdef.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2017/9/19 15:54:19

RPC_SERVER_IMPLEMETION(RpcG2M_DailyTaskRefreshG2M, DailyTaskRefreshG2MArg, DailyTaskRefreshG2MRes)

void RpcG2M_DailyTaskRefreshG2M::OnCall(const DailyTaskRefreshG2MArg &roArg, DailyTaskRefreshG2MRes &roRes)
{
	CRoleTask* pRoleTask = RoleTaskMgr::Instance()->GetRoleTask(roArg.roleid());
	if (pRoleTask == NULL)
	{
		roRes.set_result(KKSG::ERR_ROLE_NOTEXIST);
		return ;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	if (roArg.type() == TaskRefreshType_CantRefresh)
	{
		pRoleTask->m_canRefresh = false;
		pRoleTask->m_currScore = roArg.score();
		pRoleTask->SetDirty();
	}
	else if (roArg.type() == TaskRefreshType_CanRefresh)
	{
		pRoleTask->m_canRefresh = true;
		pRoleTask->m_currScore = roArg.score();
		pRoleTask->SetDirty();
	}
	else if (roArg.type() == TaskRefreshType_UpdateScore)
	{
		pRoleTask->m_currScore = roArg.score();
		pRoleTask->SetDirty();
	}
	else if (roArg.type() == TaskRefreshType_NewAccept)
	{
		pRoleTask->m_currScore = roArg.score();
		pRoleTask->OnNewAccept();
		TLogMgr::Instance()->DoDailyTaskLog(roArg.roleid(), TDailyTaskReFreshFlowType_Accept, 0, 0);
	}
}

void RpcG2M_DailyTaskRefreshG2M::OnDelayReplyRpc(const DailyTaskRefreshG2MArg &roArg, DailyTaskRefreshG2MRes &roRes, const CUserData &roUserData)
{
}
