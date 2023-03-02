#include "pch.h"
#include "giftibhisttask.h"
#include "util/dbutil.h"

GiftIBHistTask::GiftIBHistTask()
	:CMysqlTask("GiftIBHistTask")
{
}

GiftIBHistTask::~GiftIBHistTask()
{
}

void GiftIBHistTask::SetData(const KKSG::SaveIbGiftRecordArg& oData)
{
	m_oData = oData;
}

void GiftIBHistTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);
	if (m_oData.opertype()== KKSG::SaveIbGift_All)
	{		
		std::stringstream ss;
		std::string strSelfData = m_oData.giftself().SerializeAsString();
		if(-1 == poMysqlConn->EscapeString(strSelfData.c_str(), (INT32)strSelfData.size(), pcBuffer, nLength))
			return;
		ss << "replace into giftrecord(`_id`, `giftself`, `gifttarget`) values(" << m_oData.roleid() << ", '" << pcBuffer  << "'";

		std::string strTargetData = m_oData.gifttarget().SerializeAsString();
		if(-1 == poMysqlConn->EscapeString(strTargetData.c_str(), (INT32)strTargetData.size(), pcBuffer, nLength))
			return;
		ss << ", '" << pcBuffer <<  "')";
		if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
		{
			GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
			return;
		}
		SetMysqlError(MYSQL_ERR_SUCESS);
	}
}

void GiftIBHistTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("Write giftrecord data task failed, roleid: %llu, errMsg: %s", m_oData.roleid(), GetErrStream().str().c_str());
	}
}

