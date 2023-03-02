#ifndef _H_Log_File_H__
#define _H_Log_File_H__

#include "LogInterface.h"
#include <stdio.h>
#include <time.h>


class LogFile : public LogInterface
{
public:
	LogFile(const char *appName);
	~LogFile(void);

	virtual void ProcessLog(LOGTYPE type, const char *header, const char *msg);
	virtual void Flush();

private:
	void CheckFile();

	FILE *m_fp;
	const char *m_appName;
	struct tm m_timeinfo;
};

#endif