#ifndef __LOGHELPER_H__
#define __LOGHELPER_H__

#include <time.h>
#include "log/ptcg2l_gslognotify.h"

class LogHelper
{
public:
	LogHelper();
	~LogHelper();

	void SetDBType(KKSG::LogDBType nType);
	void SetTableName(const char *TableName);
	void AddKeyValue(const char *key, const std::string &value);
	void AddKeyValue(const char *key, UINT64 value);
	void AddKeyValueTime(const char *key, time_t t);
	void AddPrimaryKeyValue(const char *key, const std::string &value);
	void Send();
	void Clear();

private:
	PtcG2L_GSLogNotify m_oLogPtc;
};

#endif // __LOGHELPER_H__