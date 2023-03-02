#include "pch.h"
#include "role/rpcm2d_mssaveroledata.h"
#include "db/mysqlmgr.h"
#include "db/task/rolewritetask.h"

// generate by ProtoGen at date: 2016/7/18 11:08:11

RPC_SERVER_IMPLEMETION(RpcM2D_MsSaveRoleData, SaveRoleDataArg, SaveRoleDataRes)

void RpcM2D_MsSaveRoleData::OnCall(const SaveRoleDataArg &roArg, SaveRoleDataRes &roRes)
{
	if(roArg.fieldname_size() != roArg.fielddata_size())
	{
		LogError("Account=%s, role=%llu save data, but filed name size is not matched with data size", roArg.account().c_str(), roArg.roleid());
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}

	CRoleWriteTask* poTask = new CRoleWriteTask();
	poTask->m_qwRoleID = roArg.roleid();
	poTask->m_strAccount = roArg.account();
	poTask->m_nSlot = roArg.slot();
	poTask->m_strBriefData = roArg.briefdata();
	poTask->m_dwRpcDelayed = DelayRpc();
	for(int i = 0; i < roArg.fieldname_size(); ++i)
	{
		poTask->AddField(roArg.fieldname(i), roArg.fielddata(i));
	}
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcM2D_MsSaveRoleData::OnDelayReplyRpc(const SaveRoleDataArg &roArg, SaveRoleDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
