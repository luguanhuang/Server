#include "pch.h"
#include "tmplmailinserttask.h"


CTemplateMailInsertTask::CTemplateMailInsertTask()
:CMysqlTask("TemplateMailInsertTask")
{
}

CTemplateMailInsertTask::~CTemplateMailInsertTask()
{
}

void CTemplateMailInsertTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into mailtemplate(_id, mail) values(" << m_oData.uid();
	ss << ", '";

	std::string strMailData = m_oData.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(strMailData.c_str(), (INT32)strMailData.size(), pcBuffer, nLength)) return;
	ss << pcBuffer << "')";

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTemplateMailInsertTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Insert template mail data task failed, id: %llu, errMsg: %s", m_oData.uid(), GetErrStream().str().c_str());
	}
}