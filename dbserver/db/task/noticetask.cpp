#include "pch.h"
#include "noticetask.h"
#include "util/dbutil.h"

CNoticeTask::CNoticeTask()
	:CMysqlTask("NoticeTask")
{

}

CNoticeTask::~CNoticeTask()
{

}

void CNoticeTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	UINT32 uOpen = m_oData.isopen()?0:1;
	UINT32 uIsNew = m_oData.isnew()?1:0;

	ss << "insert into notice(`type`,`id`,`isopen`,`areaid`,`platid`,`content`,`updatetime`,`isnew`,`title`) values(" << m_oData.type() <<"," 
		<< m_oData.noticeid() << "," << uOpen << "," << m_oData.areaid() << "," << m_oData.platid() << ",'";
	if(-1 == poMysqlConn->EscapeString(m_oData.content().c_str(), (INT32)m_oData.content().size(), pcBuffer, nLength)) 
	{
		return;
	}
	ss << pcBuffer << "',";
	ss << m_oData.updatetime() << "," << uIsNew <<  ",'";
	if(-1 == poMysqlConn->EscapeString(m_oData.title().c_str(), (INT32)m_oData.title().size(), pcBuffer, nLength)) 
	{
		return;
	}
	ss << pcBuffer << "'";
	ss << ") on duplicate key update `isopen`=" << uOpen << ",`content`='";
	if(-1 == poMysqlConn->EscapeString(m_oData.content().c_str(), (INT32)m_oData.content().size(), pcBuffer, nLength)) 
	{
		return;
	}
	ss << pcBuffer << "',";
	ss << "`updatetime`=" << m_oData.updatetime() << ",`isnew`=" << uIsNew << ",`title`='";
	if(-1 == poMysqlConn->EscapeString(m_oData.title().c_str(), (INT32)m_oData.title().size(), pcBuffer, nLength)) 
	{
		return;
	}
	ss << pcBuffer << "'";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << " sql=" << ss.str().c_str() << " ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CNoticeTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Notice failed, errMsg: %s", GetErrStream().str().c_str());
	}
}
