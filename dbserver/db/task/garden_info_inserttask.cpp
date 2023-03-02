
#include "pch.h"
#include "garden_info_inserttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"


GardenInfoInsertTask::GardenInfoInsertTask() : CMysqlTask("GardenInfoInsertTask")
{
}

GardenInfoInsertTask::~GardenInfoInsertTask()
{ 
}


void GardenInfoInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// insert gardeninfo
	std::stringstream ss;
	ss << "insert into garden( `roleid`, `rolename`, `plant` , `cooking` , `fishing`, `banquet`,`eventlog`) values(";
	ss << garden_info_.role_id() << ", '";
	if(-1 == poMysqlConn->EscapeString(garden_info_.role_name().c_str(), (INT32)garden_info_.role_name().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";
	if(-1 == poMysqlConn->EscapeString(garden_info_.plant().c_str(), (INT32)garden_info_.plant().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";
	if(-1 == poMysqlConn->EscapeString(garden_info_.cooking().c_str(), (INT32)garden_info_.cooking().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";
	if(-1 == poMysqlConn->EscapeString(garden_info_.fishing().c_str(), (INT32)garden_info_.fishing().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";

	if(-1 == poMysqlConn->EscapeString(garden_info_.banquet().c_str(), (INT32)garden_info_.banquet().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "', '";

	if(-1 == poMysqlConn->EscapeString(garden_info_.eventlog().c_str(), (INT32)garden_info_.eventlog().size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "');";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void GardenInfoInsertTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{	
		LogError("Insert garden info  failed, role_id:%llu, errMsg: %s", garden_info_.role_id(), GetErrStream().str().c_str());
	}
}