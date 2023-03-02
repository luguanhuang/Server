#include "pch.h"
#include "openserverdatemgr.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "util/gametime.h"
#include "config.h"

INSTANCE_SINGLETON(COpenServerDateMgr);
COpenServerDateMgr::COpenServerDateMgr()
{

}

COpenServerDateMgr::~COpenServerDateMgr()
{

}

bool COpenServerDateMgr::Init()
{
	if (!ReadDataFromDB())
	{
		SSError << "empty table openserverdate,please insert the data!" << END;
		return false;
	}
	return true;
}

void COpenServerDateMgr::Uninit()
{

}

bool COpenServerDateMgr::ReadDataFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	std::stringstream ss;
	ss << "select `date` from `openserverdate` where `_id`=1";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table openserverdate failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	bool isFetch = poRes->FetchRow();
	if (isFetch)
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;
		std::string date;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			date.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'date' column failed");
			return false;
		}
		LogInfo("open server date=%s", date.c_str());
		struct tm T;
		sscanf(date.c_str(), "%d-%d-%d", &T.tm_year, &T.tm_mon, &T.tm_mday);
		T.tm_year -= 1900;
		T.tm_mon -= 1;
		T.tm_hour = HOUR_ONE_DAY_BEGIN;
		T.tm_min = T.tm_sec = 0;
		MSConfig::Instance()->SetGameServerOpenTimeStamp(mktime(&T));
	}
	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read openserverdate data from db use time: %llu ms", qwTimeUsed);
	return isFetch;
}
