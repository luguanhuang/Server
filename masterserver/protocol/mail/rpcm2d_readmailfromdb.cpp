#include "pch.h"
#include "mail/rpcm2d_readmailfromdb.h"
#include "role/rolemanager.h"
#include "mail/mailmgr.h"

// generate by ProtoGen at date: 2017/11/6 17:39:44

RPC_CLIENT_IMPLEMETION(RpcM2D_ReadMailFromDB, ReadMailFromDBArg, ReadMailFromDBRes)

void RpcM2D_ReadMailFromDB::OnReply(const ReadMailFromDBArg &roArg, const ReadMailFromDBRes &roRes, const CUserData &roUserData)
{
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		LogInfo("Read mail data from db success, but role is offline now, role id [%llu]", roArg.roleid());
		return; 
	}

	if(roRes.result() != KKSG::ERR_SUCCESS)
	{
		LogInfo("Role [%llu] read mail data from db failed", roArg.roleid());
		return;
	}

	CMailMgr::Instance()->OnReadRoleMailFromDB(pRole, roRes);
}

void RpcM2D_ReadMailFromDB::OnTimeout(const ReadMailFromDBArg &roArg, const CUserData &roUserData)
{
}
