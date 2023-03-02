#include "pch.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"
#include "garden_update_task.h"

GardenUpdateTask::GardenUpdateTask() : CMysqlTask("GardenUpdateTask")
{
}

GardenUpdateTask::~GardenUpdateTask()
{ 
}

void GardenUpdateTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	// update 
	std::stringstream ss;
	ss << "update garden set ";

	bool isFirst = true;

	if (garden_info_.has_role_name())
	{
		if (!isFirst)
		{
			ss << ", ";
		}
		ss << "rolename = '";
		if(-1 == poMysqlConn->EscapeString(garden_info_.role_name().c_str(), (INT32)garden_info_.role_name().size(), pcBuffer, nLength)) return;
		ss << pcBuffer ;
		ss << "'";	
		isFirst = false;
	}

	if (garden_info_.has_plant())
	{
		if (!isFirst)
		{
			ss << ", ";
		}
		ss << "plant = '";
		if(-1 == poMysqlConn->EscapeString(garden_info_.plant().c_str(), (INT32)garden_info_.plant().size(), pcBuffer, nLength)) return;
		ss << pcBuffer ;
		ss << "'";
		isFirst = false;
	}

	if (garden_info_.has_cooking())
	{	
		if (!isFirst)
		{
			ss << ", ";
		}

		ss << "cooking = '";
		if(-1 == poMysqlConn->EscapeString(garden_info_.cooking().c_str(), (INT32)garden_info_.cooking().size(), pcBuffer, nLength)) return;
		ss << pcBuffer ;
		ss << "'";		
		isFirst = false;
	}

	if (garden_info_.has_fishing())
	{
		if (!isFirst)
		{
			ss << ", ";
		}
		ss << "fishing = '";
		if(-1 == poMysqlConn->EscapeString(garden_info_.fishing().c_str(), (INT32)garden_info_.fishing().size(), pcBuffer, nLength)) return;
		ss << pcBuffer ;
		ss << "'";		
		isFirst = false;
	}

	if (garden_info_.has_banquet())
	{
		if (!isFirst)
		{
			ss << ", ";
		}

		ss << " banquet = '";
		if(-1 == poMysqlConn->EscapeString(garden_info_.banquet().c_str(), (INT32)garden_info_.banquet().size(), pcBuffer, nLength)) return;
		ss << pcBuffer ;
		ss << "'";	
		isFirst = false;
	}

	if (garden_info_.has_eventlog())
	{
		if (!isFirst)
		{
			ss << ", ";
		}

		ss << "eventlog = '";
		if(-1 == poMysqlConn->EscapeString(garden_info_.eventlog().c_str(), (INT32)garden_info_.eventlog().size(), pcBuffer, nLength)) return;
		ss << pcBuffer ;
		ss << "'";	
		isFirst = false;
	}

	ss <<" where roleid = " << garden_info_.role_id() << ";";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}


void GardenUpdateTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{	
		LogError("update garden failed, role_id:%llu, errMsg: %s", garden_info_.role_id(), GetErrStream().str().c_str());
	}
}