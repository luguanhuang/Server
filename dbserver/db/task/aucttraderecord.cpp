#include "pch.h"
#include "aucttraderecord.h"

CTradeRecordTask::CTradeRecordTask(const KKSG::AuctDBTradeAccount& data)
	:CMysqlTask("aucttraderecord"),m_data(data)
{

}

CTradeRecordTask::~CTradeRecordTask()
{

}

void CTradeRecordTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	if(!FillSql(ss, poMysqlConn, pcBuffer, nLength))
	{
		return;
	}
	if(!poMysqlConn->Query(ss.str().c_str(), ss.str().size(), NULL))
	{
		GetErrStream() << "ErrDesc: " << poMysqlConn->GetError() << ", ErrNo: " << poMysqlConn->GetErrno();
		return;
	}

	SetMysqlError(MYSQL_ERR_SUCESS);
}

void CTradeRecordTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		SSError << " auct trade record task failed op = " << m_data.op() << " auctid = " << m_data.auctuid() << " errMsg = " << GetErrStream().str() << END;
	}
}

bool CTradeRecordTask::FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	switch(m_data.op())
	{
	case KKSG::AUCTDBDATA_INSERT:
		{
			ss << "insert into aucttraderecord values("
				<< m_data.recorduid() << ","
				<< m_data.itemid() << ","
				<< m_data.itemcount() << ","
				<< m_data.perprice() << ","
				<< m_data.attime() << ","
				<< m_data.auctuid() << ",";
				if(-1 == poMysqlConn->EscapeString(m_data.buyaccount().c_str(), (int)m_data.buyaccount().size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "',";
				if(-1 == poMysqlConn->EscapeString(m_data.saleaccount().c_str(), (int)m_data.saleaccount().size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "')";
		}
		break;
	case KKSG::AUCTDBDATA_DELETE:
		{
			ss << "delete from aucttraderecord where attime <= " << m_data.attime();
		}
		break;
	default:
		break;
	}
	return true;
}
