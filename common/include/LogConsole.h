#ifndef _H_Log_Console_H__
#define _H_Log_Console_H__

#include "LogInterface.h"


class LogConsole : public LogInterface
{
public:
	LogConsole(void);
	~LogConsole(void);

	virtual void ProcessLog(LOGTYPE type, const char *header, const char *msg);
	virtual void Flush();
};


#endif