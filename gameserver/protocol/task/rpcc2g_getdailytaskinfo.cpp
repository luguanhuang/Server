#include "pch.h"
#include "task/rpcc2g_getdailytaskinfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/designationMgr.h"
#include "gamelogic/QAMgr.h"
#include "gamelogic/QARecord.h"
#include "gamelogic/taskrecord.h"
#include "task/rpcg2m_getdailytaskg2m.h"

// generate by ProtoGen at date: 2016/11/14 19:56:29

RPC_SERVER_IMPLEMETION(RpcC2G_GetDailyTaskInfo, GetDailyTaskInfoArg, GetDailyTaskInfoRes)

void RpcC2G_GetDailyTaskInfo::OnCall(const GetDailyTaskInfoArg &roArg, GetDailyTaskInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (!pRole)
	{
		return;
	}
	TaskRecord* taskRecord = pRole->Get<TaskRecord>();
	DailyTaskRecord& dailyRecord = taskRecord->GetDailyRecord();
	dailyRecord.FillDailyTaskInfo(roRes);
	RpcG2M_GetDailyTaskG2M* rpc = RpcG2M_GetDailyTaskG2M::CreateRpc();
	rpc->m_sessionID = m_sessionID;
	rpc->m_oArg.set_rpcid(DelayRpc());
	pRole->SendToMS(*rpc);
}

void RpcC2G_GetDailyTaskInfo::OnDelayReplyRpc(const GetDailyTaskInfoArg &roArg, GetDailyTaskInfoRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_pUserPtr == NULL)
	{
		return;
	}
	KKSG::GetDailyTaskG2MRes* pRes = (KKSG::GetDailyTaskG2MRes*)roUserData.m_pUserPtr;
	roRes.set_remain_refresh_count(pRes->remain_refresh_count());
	roRes.set_luck(pRes->luck());
}
