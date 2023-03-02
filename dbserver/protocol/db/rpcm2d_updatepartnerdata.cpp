#include "pch.h"
#include "db/rpcm2d_updatepartnerdata.h"
#include "db/mysqlmgr.h"
#include "db/task/partnerinserttask.h"
#include "db/task/partnerupdatetask.h"
#include "db/task/partnerdeltask.h"
#include "db/task/partnerreplaceidtask.h"


// generate by ProtoGen at date: 2016/12/8 16:01:12

RPC_SERVER_IMPLEMETION(RpcM2D_UpdatePartnerData, UpdatePartnerDataArg, UpdatePartnerDataRes)

void RpcM2D_UpdatePartnerData::OnCall(const UpdatePartnerDataArg &roArg, UpdatePartnerDataRes &roRes)
{
	switch (roArg.type())
	{
	case KKSG::DBOper_Insert:
		{
			PartnerInsertTask* task = new PartnerInsertTask;
			task->m_rpcId = DelayRpc();
			task->m_partnerId = roArg.data().id();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_partnerId));
			break;
		}
	case KKSG::DBOper_Update:
		{
			PartnerUpdateTask* task = new PartnerUpdateTask;
			task->m_rpcId = DelayRpc();
			task->m_partnerId = roArg.data().id();
			task->m_data = roArg.data();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_partnerId));
			break;
		}
	case KKSG::DBOper_Del:
		{
			PartnerDelTask* task = new PartnerDelTask;
			task->m_rpcId = DelayRpc();
			task->m_partnerId = roArg.data().id();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_partnerId));
			break;
		}
	case KKSG::DBOper_ReplaceId:
		{
			PartnerReplaceIdTask* task = new PartnerReplaceIdTask;
			task->m_rpcId = DelayRpc();
			task->m_partnerId = roArg.data().id();
			task->m_oldId = roArg.oldid();
			CMysqlMgr::Instance()->PushTask(task, GetRoleThreadIndex(task->m_partnerId));
			break;
		}
	default:
		LogError("invalid db type[%u]", roArg.type());
		roRes.set_result(KKSG::ERR_FAILED);
	}
}

void RpcM2D_UpdatePartnerData::OnDelayReplyRpc(const UpdatePartnerDataArg &roArg, UpdatePartnerDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
