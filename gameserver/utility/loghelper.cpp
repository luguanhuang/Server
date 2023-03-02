#include "pch.h"
#include "loghelper.h"
#include "config.h"
#include "util.h"
#include "tlogwriter.h"

LogHelper::LogHelper()
{
	m_oLogPtc.m_Data.set_dbtype(KKSG::LOG_DB_NORMAL);
}

LogHelper::~LogHelper()
{

}

void LogHelper::SetDBType(KKSG::LogDBType nType)
{
	m_oLogPtc.m_Data.set_dbtype(nType);
}

void LogHelper::SetTableName(const char *TableName)
{
	m_oLogPtc.m_Data.set_tablename(TableName);
}

void LogHelper::AddKeyValue(const char *key, const std::string &value)
{
	m_oLogPtc.m_Data.add_key(key);
	m_oLogPtc.m_Data.add_value(value);
}

void LogHelper::AddKeyValue(const char *key, UINT64 value)
{
	m_oLogPtc.m_Data.add_key(key);
	m_oLogPtc.m_Data.add_value(ToString(value));
}

void LogHelper::AddKeyValueTime(const char *key, time_t t)
{
	char buf[128];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

	m_oLogPtc.m_Data.add_key(key);
	m_oLogPtc.m_Data.add_value(buf);
}

void LogHelper::Send()
{
	if(m_oLogPtc.m_Data.dbtype() == KKSG::LOG_DB_TENCENT)
	{
		std::stringstream ss;
		ss << m_oLogPtc.m_Data.tablename();
		for(int i = 0; i < m_oLogPtc.m_Data.value_size(); ++i)
		{
			ss << "|" << m_oLogPtc.m_Data.value(i);
		}
		TLogInfo("%s", ss.str().c_str());
	}
}

void LogHelper::Clear()
{
	m_oLogPtc.m_Data.Clear();
}

void LogHelper::AddPrimaryKeyValue(const char *key, const std::string &value)
{
	m_oLogPtc.m_Data.set_primarykey(key);
	m_oLogPtc.m_Data.set_primaryvalue(value);
}

