#include "pch.h"
#include "tlogwriter.h"
#include "baseconfig.h"
#include "util.h"


INSTANCE_SINGLETON(TLogWriter)

std::string TLogWriter::sm_strConfigFile("serveronly/tlog_config.xml");


TLogWriter::TLogWriter()
#ifndef WIN32
:m_pstLogCtx(NULL)
,m_pstCat(NULL)
#endif
{
}

TLogWriter::~TLogWriter()
{
}

bool TLogWriter::Init()
{
#ifndef WIN32
	const char* pszFilePath = TLogWriter::GetConfigFile().c_str();
	m_pstLogCtx = tlog_init_from_file(Config::GetConfig()->GetFilePath(pszFilePath).c_str());
	if(m_pstLogCtx == NULL)
	{
		LogWarn("Init tlog from file: %s failed", pszFilePath);
		return false;
	}

	m_pstCat = tlog_get_category(m_pstLogCtx, "test");
	if(m_pstCat == NULL)
	{
		LogWarn("tlog_get_category is error, file: %s", pszFilePath);
		return -1;
	}
#endif
	return true;
}

void TLogWriter::Uninit()
{
#ifndef WIN32
	if(m_pstLogCtx != NULL)
	{
		tlog_fini_ctx(&m_pstLogCtx);
		m_pstLogCtx = NULL;
	}
#endif
}

const std::string& TLogWriter::GetConfigFile()
{
	return TLogWriter::sm_strConfigFile;
}

void TLogWriter::SetConfigFile(const std::string& strFile)
{
	TLogWriter::sm_strConfigFile = strFile;
}

std::string ProcessRoleName(const std::string& strName)
{
	std::string strResult;
	for(std::string::size_type i = 0; i < strName.size(); ++i)
	{
		if(strName[i] != '|' && strName[i] != 0xA && strName[i] != 0xD) strResult.push_back(strName[i]);
	}
	return strResult;
}

LogHelperBase::LogHelperBase()
{
	m_Log.set_dbtype(KKSG::LOG_DB_NORMAL);
}

LogHelperBase::~LogHelperBase()
{

}

void LogHelperBase::SetDBType(KKSG::LogDBType nType)
{
	m_Log.set_dbtype(nType);
}

void LogHelperBase::SetTableName(const char *TableName)
{
	m_Log.set_tablename(TableName);
}

void LogHelperBase::AddKeyValue(const char *key, const std::string &value)
{
	m_Log.add_key(key);
	m_Log.add_value(value);
}

void LogHelperBase::AddKeyValue(const char *key, UINT32 value)
{
	m_Log.add_key(key);
	m_Log.add_value(ToString(value));
}

void LogHelperBase::AddKeyValueTime(const char *key, time_t t)
{
	char buf[128];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));

	m_Log.add_key(key);
	m_Log.add_value(buf);
}

void LogHelperBase::Send()
{
	if(m_Log.dbtype() == KKSG::LOG_DB_TENCENT)
	{
		std::stringstream ss;
		ss << m_Log.tablename();
		for(int i = 0; i < m_Log.value_size(); ++i)
		{
			ss << "|" << m_Log.value(i);
		}
		TLogInfo("%s", ss.str().c_str());
	}
}

void LogHelperBase::Clear()
{
	m_Log.Clear();
}

void LogHelperBase::AddPrimaryKeyValue(const char *key, const std::string &value)
{
	m_Log.set_primarykey(key);
	m_Log.set_primaryvalue(value);
}

void TSimpleCommonLog::Do()
{
	LogHelperBase log;
	log.SetDBType(KKSG::LOG_DB_TENCENT);
	log.SetTableName(GetTableName());
	log.AddKeyValueTime("dtEventTime", time(NULL));
	Build(log);
	log.Send();
}