#include "pch.h"
#include "auctsaleitemtask.h"

CSaleItemTask::CSaleItemTask(const KKSG::AuctDBItemDale& data)
	:CMysqlTask("auctsaleitemtask"),m_data(data)
{

}

CSaleItemTask::~CSaleItemTask()
{

}

void CSaleItemTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	SetMysqlError(MYSQL_ERR_FAILED);

	std::stringstream ss;
	bool ret = FillSql(ss, poMysqlConn, pcBuffer, nLength);
	if(!ret)
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

void CSaleItemTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		SSError << " auct sale item task failed op = " << m_data.op() << " auctid = " << m_data.auctuid() << " errMsg = " << GetErrStream().str() << END;
	}
}

bool CSaleItemTask::FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	switch(m_data.op())
	{
	case KKSG::AUCTDBDATA_INSERT:
		{
			ss << "insert into auctsaleitem values(" 
				<< m_data.auctuid() << "," 
				<< m_data.roleid() << ",";
				if(-1 == poMysqlConn->EscapeString(m_data.roleaccount().c_str(), (int)m_data.roleaccount().size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "',"
				<< m_data.state() << ","
				<< m_data.perprice() << ","
				<< m_data.reqsaletime() << ","
				<< m_data.onsaletime() << ","
				<< m_data.outsaletime() << ",";
				if(-1 == poMysqlConn->EscapeString(m_data.itemdata().c_str(), (int)m_data.itemdata().size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "')";
		}
		break;
	case KKSG::AUCTDBDATA_UPDATE:
		{
			ss << "update auctsaleitem set"
				<< " roleid = " << m_data.roleid()
				<< ",roleaccount = ";
				if(-1 == poMysqlConn->EscapeString(m_data.roleaccount().c_str(), (int)m_data.roleaccount().size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "'"
				<< ",state = " << m_data.state() 
				<< ",perprice = " << m_data.perprice()
				<< ",reqsaletime = " << m_data.reqsaletime()
				<< ",onsaletime = " << m_data.onsaletime()
				<< ",outsaletime = " << m_data.outsaletime()
				<< ",itemdata = ";
				if(-1 == poMysqlConn->EscapeString(m_data.itemdata().c_str(), (int)m_data.itemdata().size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "'"
				<< " where _id = " << m_data.auctuid();
		}
		break;
	case KKSG::AUCTDBDATA_DELETE:
		{
			ss << "delete from auctsaleitem where _id = " << m_data.auctuid();
		}
		break;
	default:
		break;
	}
	return true;
}
