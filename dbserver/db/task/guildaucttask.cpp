#include "pch.h"
#include "guildaucttask.h"

CGuildAuctTask::CGuildAuctTask(const KKSG::GuildAuctPara& data)
	:CMysqlTask("guildauctitem"),m_data(data)
{

}

CGuildAuctTask::~CGuildAuctTask()
{

}

void CGuildAuctTask::Execute(IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
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

void CGuildAuctTask::OnReply()
{
	if(m_nMysqlErr != MYSQL_ERR_SUCESS)
	{
		SSError << " guldauctitem task failed op = " << m_data.op() << " uid = " << m_data.dbdata().uid() << " errMsg = " << GetErrStream().str() << END;
	}
}

bool CGuildAuctTask::FillSql(std::stringstream& ss, IMysqlConn* poMysqlConn, char* pcBuffer, INT32 nLength)
{
	const KKSG::GAItemDBData& dbdata = m_data.dbdata();
	switch(m_data.op())
	{
	case KKSG::AUCTDBDATA_INSERT:
		{
			std::string stmp;
			dbdata.SerializeToString(&stmp);

			ss << "insert into guildauctitem values(" 
				<< dbdata.uid() << "," ;
				if(-1 == poMysqlConn->EscapeString(stmp.c_str(), (int)stmp.size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "')";
		}
		break;
	case KKSG::AUCTDBDATA_UPDATE:
		{
			std::string stmp;
			dbdata.SerializeToString(&stmp);

			ss << "update guildauctitem set"
				<< " alldata = ";
				if(-1 == poMysqlConn->EscapeString(stmp.c_str(), (int)stmp.size(), pcBuffer, nLength))
				{
					return false;
				}
				ss << "'" << pcBuffer << "'"
				<< " where _id = " << dbdata.uid();
		}
		break;
	case KKSG::AUCTDBDATA_DELETE:
		{
			ss << "delete from guildauctitem where _id = " << dbdata.uid();
		}
		break;
	default:
		break;
	}
	return true;
}
