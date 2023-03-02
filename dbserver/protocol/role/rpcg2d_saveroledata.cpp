#include "pch.h"
#include "role/rpcg2d_saveroledata.h"
#include "db/mysqlmgr.h"
#include "db/task/rolewritetask.h"

// generate by ProtoGen at date: 2016/7/12 13:58:58

RPC_SERVER_IMPLEMETION(RpcG2D_SaveRoleData, SaveRoleDataArg, SaveRoleDataRes)

void RpcG2D_SaveRoleData::OnCall(const SaveRoleDataArg &roArg, SaveRoleDataRes &roRes)
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
	// hidden roles
	if (roArg.has_changepro())
	{
		const KKSG::ChangeProSaveData& data = roArg.changepro();
		poTask->m_lastslot = data.lastslot();
		poTask->m_lastslotrole = data.lastslotrole();
		poTask->m_hiddenroles = data.hiddenroles();
	}
	CMysqlMgr::Instance()->PushTask(poTask, GetRoleThreadIndex(roArg.roleid()));
}

void RpcG2D_SaveRoleData::OnDelayReplyRpc(const SaveRoleDataArg &roArg, SaveRoleDataRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(nResult);
}
