#ifndef _H_TLog_Writer_H__
#define _H_TLog_Writer_H__

#ifndef WIN32
#include "pal/tos.h"
#include "tdr/tdr.h"
#include "tlog/tlog.h"
#include "tloghelp/tlogload.h"
#endif
#include <time.h>
#include "pb/enum.pb.h"
#include "pb/project.pb.h"


class TLogWriter
{
	TLogWriter();
	~TLogWriter();
	DECLARE_SINGLETON(TLogWriter);
public:
	bool Init();
	void Uninit();

	static const std::string& GetConfigFile();
	static void SetConfigFile(const std::string& strFile);

#ifndef WIN32
	LPTLOGCATEGORYINST GetCategory() { return m_pstCat; }
private:
	LPTLOGCTX			m_pstLogCtx;
	LPTLOGCATEGORYINST	m_pstCat;
#endif

private:
	static std::string	sm_strConfigFile;
};


#ifdef WIN32
#define TLogInfo(fmt, ...)		LogInfo(fmt, __VA_ARGS__)
#else
#define TLogInfo(fmt, ...)		tlog_info(TLogWriter::Instance()->GetCategory(), 0, 0, fmt, __VA_ARGS__)
#endif

std::string ProcessRoleName(const std::string& strName);
//IDIP tlog
class LogHelperBase
{
public:
	LogHelperBase();
	virtual ~LogHelperBase();

	void SetDBType(KKSG::LogDBType nType);
	void SetTableName(const char *TableName);
	void AddKeyValue(const char *key, const std::string &value);
	void AddKeyValue(const char *key, UINT32 value);
	void AddKeyValueTime(const char *key, time_t t);
	void AddPrimaryKeyValue(const char *key, const std::string &value);
	void Send();
	void Clear();
protected:
	KKSG::LogMsg m_Log;
};

class TSimpleCommonLog
{
public:
	TSimpleCommonLog(){}
	virtual ~TSimpleCommonLog(){}

	void Do();
	virtual void Build(LogHelperBase& roLog) {}
	virtual const char* GetTableName() const { return ""; }
};
#endif