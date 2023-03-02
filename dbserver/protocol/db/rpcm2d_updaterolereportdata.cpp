#include "pch.h"
#include "db/rpcm2d_updaterolereportdata.h"
#include "db/mysqlmgr.h"
#include "db/task/updatereporttask.h"
#include "db/task/delreporttask.h"

// generate by ProtoGen at date: 2017/5/14 22:11:05

RPC_SERVER_IMPLEMETION(RpcM2D_UpdateRoleReportData, UpdateRoleReportDataArg, UpdateRoleReportDataRes)

void RpcM2D_UpdateRoleReportData::OnCall(const UpdateRoleReportDataArg &roArg, UpdateRoleReportDataRes &roRes)
{
	switch (m_oArg.type())
	{
	case KKSG::DBOper_Update:	
	{
		UpdateReportTask* pTask = new UpdateReportTask;
		pTask->m_roleId = m_oArg.roleid();
		pTask->m_data = m_oArg.data();
		pTask->m_rpcId = DelayRpc();
		CMysqlMgr::Instance()->PushTask(pTask, GetRoleThreadIndex(pTask->m_roleId));
	}
		break;
	case KKSG::DBOper_Del:
	{	
		DelReportTask* pTask = new DelReportTask;
		pTask->m_roleId = m_oArg.roleid();
		pTask->m_rpcId = DelayRpc();
		CMysqlMgr::Instance()->PushTask(pTask, GetRoleThreadIndex(pTask->m_roleId));
	}
		break;
	default:
		LogInfo("invalid type[%u]", m_oArg.type());
		roRes.set_result(KKSG::ERR_FAILED);
	}	
}

void RpcM2D_UpdateRoleReportData::OnDelayReplyRpc(const UpdateRoleReportDataArg &roArg, UpdateRoleReportDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
