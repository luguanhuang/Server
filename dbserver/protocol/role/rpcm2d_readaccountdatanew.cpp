#include "pch.h"
#include "role/rpcm2d_readaccountdatanew.h"
#include "db/task/readaccounttasknew.h"
#include "db/mysqlmgr.h"
#include "define/miscdef.h"

// generate by ProtoGen at date: 2017/10/25 15:48:37

RPC_SERVER_IMPLEMETION(RpcM2D_ReadAccountDataNew, ReadAccountDataNewArg, ReadAccountDataNewRes)

void RpcM2D_ReadAccountDataNew::OnCall(const ReadAccountDataNewArg &roArg, ReadAccountDataNewRes &roRes)
{
	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		CReadAccountTaskNew* pTask = new CReadAccountTaskNew;
		pTask->m_strAccount = roArg.account();
		pTask->m_serverId = roArg.serverid();
		pTask->m_dwRpcDelayed = DelayRpc();
		CMysqlMgr::Instance()->PushTask(pTask, GetAccountThreadIndex());
	}
}

void RpcM2D_ReadAccountDataNew::OnDelayReplyRpc(const ReadAccountDataNewArg &roArg, ReadAccountDataNewRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode code = (KKSG::ErrorCode)roUserData.m_dwUserData;
	roRes.set_result(code);
	if (code != KKSG::ERR_SUCCESS)
	{
		return;
	}

	if (roArg.opertype() == ChargeBackOperType_Query)
	{
		std::vector<KKSG::ZoneRoleInfo>* pRoles = (std::vector<KKSG::ZoneRoleInfo>*)roUserData.m_pUserPtr;
		if (pRoles == NULL)
		{
			return;
		}

		for (auto it = pRoles->begin(); it != pRoles->end(); ++it)
		{
			roRes.add_roles()->CopyFrom(*it);
		}
	}

}
