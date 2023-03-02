#include "pch.h"
#include "task/rpcc2m_dailytaskrefreshoper.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "task/roletaskmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"
#include "util/gametime.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"
#include "table/globalconfig.h"
#include "task/rpcm2g_dailytaskrefreshm2g.h"
#include "define/taskdef.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2017/9/18 1:45:18

RPC_SERVER_IMPLEMETION(RpcC2M_DailyTaskRefreshOper, DailyTaskRefreshOperArg, DailyTaskRefreshOperRes)


void RpcC2M_DailyTaskRefreshOper::OnCall(const DailyTaskRefreshOperArg &roArg, DailyTaskRefreshOperRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CRoleTask* pRTask = RoleTaskMgr::Instance()->GetRoleTask(pRole->GetID());
	if (pRTask == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	KKSG::ErrorCode code = KKSG::ERR_FAILED;
	if (roArg.type() == KKSG::DROT_AskHelp)
	{
		code = pRTask->AskHelp(roArg.roleid());
		if (code == KKSG::ERR_SUCCESS )
		{
			TLogMgr::Instance()->DoDailyTaskLog(pRole->GetID(), TDailyTaskReFreshFlowType_AskHelp, roArg.roleid(), 0);
		}
	}
	else if (roArg.type() == KKSG::DROT_Refuse)
	{
		code = pRTask->RefuseHelp(roArg.roleid());
		if (code == KKSG::ERR_SUCCESS )
		{
			TLogMgr::Instance()->DoDailyTaskLog(roArg.roleid(), TDailyTaskReFreshFlowType_Refuse, pRole->GetID(), 0);
		}
	}
	else if (roArg.type() == KKSG::DROT_Refresh)
	{
		CRoleTask* pOTask = NULL;
		if (roArg.roleid() == 0 || roArg.roleid() == pRole->GetID())
		{
			pOTask = pRTask;
		}
		else
		{
			pOTask = RoleTaskMgr::Instance()->GetRoleTask(roArg.roleid());
		}
		if (pOTask == NULL)
		{
			roRes.set_result(KKSG::ERR_FAILED);
			return;
		}
		roRes.set_oldscore(pOTask->m_currScore);
		code = pRTask->Refresh(pRole, roArg.roleid());
		roRes.set_score(pOTask->m_currScore);

		TLogMgr::Instance()->DoDailyTaskLog(roArg.roleid(), TDailyTaskReFreshFlowType_Refresh, pRole->GetID(), roRes.oldscore());
	}
	else if (roArg.type() == KKSG::DROT_BuyCount)
	{
		if (pRTask->m_dailyBuyCount >= GetGlobalConfig().DailyTaskMaxBuyRefreshCount)
		{
			roRes.set_result(KKSG::ERR_DAILY_TASK_NO_BUY_COUNT);
			return;
		}
		RpcM2G_DailyTaskRefreshM2G* rpc = RpcM2G_DailyTaskRefreshM2G::CreateRpc();
		rpc->m_sessionID = m_sessionID;
		rpc->m_oArg.set_type(TaskRefreshType_BuyRefreshCount);
		rpc->m_oArg.set_rpcid(DelayRpc());
		rpc->m_oArg.set_count(pRTask->m_dailyBuyCount);
		pRole->SendMsgToGS(*rpc);
	}
	else
	{
		roRes.set_result(KKSG::ERR_FAILED);
	}
	roRes.set_result(code);
}

void RpcC2M_DailyTaskRefreshOper::OnDelayReplyRpc(const DailyTaskRefreshOperArg &roArg, DailyTaskRefreshOperRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CRoleTask* pRTask = RoleTaskMgr::Instance()->GetRoleTask(pRole->GetID());
	if (pRTask == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	if (code != KKSG::ERR_SUCCESS)
	{
		roRes.set_result(code);
		return;
	}

	if (roArg.type() == KKSG::DROT_BuyCount)
	{
		pRTask->m_dailyBuyCount++;
		pRTask->m_extraDailyRefreshCount++;
		pRTask->SetDirty();

		TLogMgr::Instance()->DoDailyTaskLog(pRole->GetID(), TDailyTaskReFreshFlowType_BuyCount, 0, 0);
	}
	roRes.set_result(code);
}
