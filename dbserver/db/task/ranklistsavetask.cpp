#include "pch.h"
#include "ranklistsavetask.h"

RankListSaveTask::RankListSaveTask()
:CMysqlTask("RankListSaveTask")
{

}

RankListSaveTask::~RankListSaveTask()
{

}

void RankListSaveTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "select `_id` from ranklist where _id=" << m_dwType << " and serverid=" << m_dwServerID;

	IMysqlRecordSet* poRes = NULL;
	if (!poMysqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	ss.str("");
	
	if (-1 == poMysqlConn->EscapeString(m_strRankData.c_str(), (INT32)m_strRankData.size(), pcBuffer, nLength)) 
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		poRes->Release();
		return;
	}

	if (0 == poRes->GetRowNum())
	{
		ss << "insert into ranklist(`_id`, `data`, `serverid`) values (" << m_dwType <<", '" << pcBuffer << "', " << m_dwServerID << ")";
	}
	else
	{
		ss << "update ranklist set data = '"<< pcBuffer <<"' where _id=" << m_dwType << " and serverid=" << m_dwServerID;	
	}

	poRes->Release();

	if (!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void RankListSaveTask::OnReply()
{

}