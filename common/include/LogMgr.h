#pragma once
#include <stdarg.h>
#include <sstream>
#include "LogInterface.h"
#include "define.h"

enum ELogHandlerType
{
	LOG_HANDLER_CONSOLE = 0,
	LOG_HANDLER_FILE = 1,
	MAX_LOG_HANDLER = 2,
};

struct LogEnd
{

};

struct Hex
{
	UINT64 m_value;
	Hex(UINT64 v) : m_value(v) {}
};

std::ostream & operator<<(std::ostream &stream, const Hex &hex);

class LogMgr : public LogInterface
{
public:
	static LogMgr* Instance();
	static void Destory();

	void Log(LOGTYPE type, const char *fmt, ...);
	void LogAP(LOGTYPE type, const char *fmt, va_list ap);
	static bool Info(const char *fmt, ...);
	static bool Warn(const char *fmt, ...);
	static bool Debug(const char *fmt, ...);
	static bool Error(const char *fmt, ...);
	static bool Fatal(const char *fmt, ...);

	void Flush();
	LogInterface *FindLog(const char *name);
	void SetLogHandler(ELogHandlerType type, LogInterface* log);

	template <typename T>
	LogMgr &operator << (const T &t)
	{
		m_Stream << t;
		return *this;
	}

	LogMgr &operator << (const LogEnd &)
	{
		FlushStream();
		return *this;
	}

	LogMgr &operator << (const Hex &hex)
	{
		m_Stream << hex;
		return *this;
	}

	static LogMgr& Stream(LOGTYPE type);

	virtual void ProcessLog(LOGTYPE type, const char *header, const char *msg);


	bool IsFileOn() const			{ return m_bFileOn; }
	bool IsConsoleOn() const		{ return m_bConsoleOn; }
	LogRollType GetRollType() const { return m_nRollType; }
	bool IsEnabled(LOGTYPE type) const { return (m_nLogEnableFlag & (1 << type)) != 0; }

	void SetFileOn(bool bVal)			{ m_bFileOn = bVal; }
	void SetConsoleOn(bool bVal)		{ m_bConsoleOn = bVal; }
	void SetRollType(LogRollType nType) { m_nRollType = nType; }
	void SetEnableLogType(LOGTYPE type, bool enable);
private:
	void BuildHeader(LOGTYPE type, char *buf, int len);
	void FlushStream();

	LogMgr(void);
	~LogMgr(void);

	std::stringstream m_Stream;
	LOGTYPE m_type;

	bool			m_bFileOn;
	bool			m_bConsoleOn;
	INT32			m_nLogEnableFlag;
	LogRollType		m_nRollType;
	LogInterface*	m_LogHandlers[MAX_LOG_HANDLER];
};
