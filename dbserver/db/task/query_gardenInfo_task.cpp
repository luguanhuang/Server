#include "pch.h"
#include "query_gardenInfo_task.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CQueryGardenInfoTask::CQueryGardenInfoTask()
	:CMysqlTask("QueryGardenInfoTask")
	,m_dwRpcDelayed(0)
	,m_qwRoleID(0)
{
}

CQueryGardenInfoTask::~CQueryGardenInfoTask()
{ 
}

void CQueryGardenInfoTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss<< "select roleid,rolename,plant, cooking,fishing,banquet,eventlog from garden  where roleid=" << m_qwRoleID;

	IMysqlRecordSet* poRes = NULL;
	if(!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	poRes->FetchRow();
	char* pVal   = NULL;
	UINT32 dwLen = 0;
	std::string strdata;
	// roleid		
	if (poRes->GetFieldValue(0, &pVal, dwLen))
	{
		garden_info_.set_role_id(convert<UINT64>(pVal));
	}
	else
	{
		LogError("Read 'roleid' column failed");
		return ;
	}	
	// name
	if(poRes->GetFieldValue(1, &pVal, dwLen))
	{
		strdata.clear();
		strdata.assign(pVal, dwLen);
		garden_info_.set_role_name(strdata);
	}
	else
	{
		LogError("Read 'rolename' column failed");
		return ;
	}

	// plant
	if(poRes->GetFieldValue(2, &pVal, dwLen))
	{
		strdata.clear();
		strdata.assign(pVal, dwLen);
		garden_info_.set_plant(strdata);
	}
	else
	{
		LogError("Read 'plant' column failed");
		return ;
	}
	// cooking
	if(poRes->GetFieldValue(3, &pVal, dwLen))
	{
		strdata.clear();
		strdata.assign(pVal, dwLen);
		garden_info_.set_cooking(strdata);
	}
	else
	{
		LogError("Read 'cooking' column failed");
		return ;
	}
	// fishing
	if(poRes->GetFieldValue(4, &pVal, dwLen))
	{
		strdata.clear();
		strdata.assign(pVal, dwLen);
		garden_info_.set_fishing(strdata);
	}
	else
	{
		LogError("Read 'fishing' column failed");
		return ;
	}
	// banquet
	if(poRes->GetFieldValue(5, &pVal, dwLen))
	{
		strdata.clear();
		strdata.assign(pVal, dwLen);
		garden_info_.set_banquet(strdata);
	}
	else
	{
		LogError("Read 'banquet' column failed");
		return ;
	}

	// eventlog
	if(poRes->GetFieldValue(6, &pVal, dwLen))
	{
		strdata.clear();
		strdata.assign(pVal, dwLen);
		garden_info_.set_eventlog(strdata);
	}
	else
	{
		LogError("Read 'eventlog' column failed");
		return ;
	}


	poRes->Release();
	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CQueryGardenInfoTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("Select role garden data task failed, roleid: %llu, errMsg: %s", m_qwRoleID, GetErrStream().str().c_str());
	}

	CUserData oUser((UINT32)nErrCode, &garden_info_);
	CRpc::ReplyDelayRpc(m_dwRpcDelayed, oUser);
}