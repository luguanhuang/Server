#include "pch.h"
#include "mailoptask.h"


CMailOpTask::CMailOpTask()
:CMysqlTask("MailOpTask")
,m_qwRoleID(0)
,m_bByTemplate(false)
,m_qwTemplateID(0)
,m_nOP(KKSG::MAIL_OP_ADD)
{
}

CMailOpTask::~CMailOpTask()
{

}

void CMailOpTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	std::string strMailData = m_oData.SerializeAsString();
	if(-1 == poMysqlConn->EscapeString(strMailData.c_str(), (INT32)strMailData.size(), pcBuffer, nLength)) return;
	switch(m_nOP)
	{
	case KKSG::MAIL_OP_ADD:
		{
			ss << "insert into mail(_id, roleid, mail) values(" << m_oData.uid() << ", " << m_qwRoleID << ", '" << pcBuffer << "')";
		}
		break;

	case KKSG::MAIL_OP_UPDATE:
		{
			ss << "update mail set mail='" << pcBuffer << "' where _id=" << m_oData.uid();
		}
		break;

	case KKSG::MAIL_OP_DELETE:
		{
			ss << "delete from mail where _id=" << m_oData.uid();
		}
		break;

	case KKSG::MAIL_OP_DELETE_ALL:
		{
			ss << "delete from mail where roleid=" << m_qwRoleID;
		}
		break;

	default:
		GetErrStream() << "Invalid mail op type " << m_nOP;
		return;
	}

	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	if(m_nOP == KKSG::MAIL_OP_ADD && m_bByTemplate)
	{
		ss.str("");
		ss << "insert into mailrecord(roleid, maxtemplateid) values(" << m_qwRoleID <<", " << m_qwTemplateID << ") ON DUPLICATE KEY UPDATE maxtemplateid=" << m_qwTemplateID;
		if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CMailOpTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("MailOptask failed, id: %llu, roleid: %llu, errMsg: %s", m_oData.uid(), m_qwRoleID, GetErrStream().str().c_str());
	}
}