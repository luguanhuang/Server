#include "pch.h"
#include "pushconfig.h"
#include "db/mysqlmgr.h"
#include "timeutil.h"
#include "util.h"

INSTANCE_SINGLETON(CPushConfigMgr);
CPushConfigMgr::CPushConfigMgr()
{
	m_bIsWxPush = false;
}

CPushConfigMgr::~CPushConfigMgr()
{

}

bool CPushConfigMgr::Init()
{
	if (!ReadDataFromDB())
	{
		return false;
	}
	return true;
}

void CPushConfigMgr::Uninit()
{

}

bool CPushConfigMgr::ReadDataFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	std::stringstream ss;
	ss << "select `isopen` from `wxpush` where `_id`=1";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table wxpush failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	if (poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;
		INT32 isOpen = 0;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			isOpen = convert<INT32>(pVal);
		}
		else
		{
			LogError("Read 'isopen' column failed");
			return false;
		}
		m_bIsWxPush = isOpen==1?true:false;
	}
	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read wxpush data from db use time: %llu ms", qwTimeUsed);
	return true;
}
