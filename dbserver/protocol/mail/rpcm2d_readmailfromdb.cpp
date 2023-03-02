#include "pch.h"
#include "mail/rpcm2d_readmailfromdb.h"
#include "db/mysqlmgr.h"
#include "db/task/readrolemailtask.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2017/11/6 17:39:44

RPC_SERVER_IMPLEMETION(RpcM2D_ReadMailFromDB, ReadMailFromDBArg, ReadMailFromDBRes)

void RpcM2D_ReadMailFromDB::OnCall(const ReadMailFromDBArg &roArg, ReadMailFromDBRes &roRes)
{
	CReadRoleMailTask* poTask = new CReadRoleMailTask();
	poTask->m_qwRoleID = roArg.roleid();
	poTask->m_dwRpcDelayed = DelayRpc();
	poTask->m_dwTimeNow = TimeUtil::GetTime();
	CMysqlMgr::Instance()->PushTask(poTask);
}

void RpcM2D_ReadMailFromDB::OnDelayReplyRpc(const ReadMailFromDBArg &roArg, ReadMailFromDBRes &roRes, const CUserData &roUserData)
{
	KKSG::ErrorCode nResult = (KKSG::ErrorCode)roUserData.m_dwUserData;
	std::map<UINT64, KKSG::SMail>* poMailMap = (std::map<UINT64, KKSG::SMail>*)roUserData.m_pUserPtr;
	roRes.set_result(nResult);
	if(nResult == KKSG::ERR_SUCCESS && poMailMap != NULL)
	{
		for(std::map<UINT64, KKSG::SMail>::iterator it = poMailMap->begin(); it != poMailMap->end(); ++it)
		{
			KKSG::SMail* poMail = roRes.add_mails();
			poMail->CopyFrom(it->second);
		}
	}
}
