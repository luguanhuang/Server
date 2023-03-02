#include "pch.h"
#include "paylisttask.h"
#include "util.h"
#include "crpc.h"
#include "util/dbutil.h"

CPayListTask::CPayListTask()
	:CMysqlTask("PayListTask")
{

}
CPayListTask::~CPayListTask()
{

}

void CPayListTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	ss << "insert into paylist(`orderid`,`account`,`roleid`,`paraid`,`amount`,`paytime`) values('" << m_oData.orderid() << "','"
		<< m_oData.account() << "'," << m_oData.roleid() << ",'" << m_oData.paraid() << "'," << m_oData.amount() << "," 
		<< m_oData.paytime() << ")";


	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << " sql=" << ss.str().c_str() << " ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CPayListTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		LogError("PayListTask failed,roleid: %llu, errMsg: %s", m_oData.roleid(), GetErrStream().str().c_str());
	}
}
