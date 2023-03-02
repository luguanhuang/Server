#include "pch.h"
#include "crossrolesummarydelete.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CrossRoleSummaryDelete::CrossRoleSummaryDelete()
:CMysqlTask("CrossRoleSummaryDelete")
{
}

CrossRoleSummaryDelete::~CrossRoleSummaryDelete()
{ 
}

void CrossRoleSummaryDelete::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::string sql("delete from cross_rolesummary");
	if(!poMysqlConn->Query(sql.c_str(), sql.size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CrossRoleSummaryDelete::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("delete cross rolesummary failed, errMsg: %s", GetErrStream().str().c_str());
	}
}


