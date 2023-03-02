#include "pch.h"
#include "globalupdatetask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CGlobalUpdateTask::CGlobalUpdateTask()
:CMysqlTask("GlobalUpdateTask")
,m_dwRpcDelayed(0)
{
}

CGlobalUpdateTask::~CGlobalUpdateTask()
{ 
}

void CGlobalUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	//insert role
	std::stringstream ss;
	ss << "insert into globalconfig(_id, data) values(" << m_uID;
	ss << ", '";

	if(-1 == poMysqlConn->EscapeString(m_strData.c_str(), (INT32)m_strData.length(), pcBuffer, nLength)) return;
	ss << pcBuffer << "') on duplicate key update data='" << pcBuffer << "'";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CGlobalUpdateTask::OnReply()
{
	KKSG::ErrorCode nErrCode = KKSG::ERR_SUCCESS;
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		nErrCode = KKSG::ERR_FAILED;
		LogError("update global update fail, id: %d, errMsg: %s", m_uID, GetErrStream().str().c_str());
	}
}
