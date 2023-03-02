#ifndef __LOGHELPER_H__
#define __LOGHELPER_H__

#include <time.h>
#include "common/ptcm2l_mslognotify.h"

class LogHelper
{
public:
	LogHelper();
	~LogHelper();

	void SetDBType(KKSG::LogDBType nType);
	void SetTableName(const char *TableName);
	void AddKeyValue(const char *key, const std::string &value);
	void AddKeyValue(const char *key, UINT32 value);
	void AddKeyValueTime(const char *key, time_t t);
	void AddPrimaryKeyValue(const char *key, const std::string &value);
	void Send();
	void Clear();

private:
	PtcM2L_MSLogNotify m_oLogPtc;
};

#endif // __LOGHELPER_H__