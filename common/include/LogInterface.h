#pragma once

#include <string>

enum LOGTYPE {
	LOG_INFO,
	LOG_DEBUG,
	LOG_WARN,
	LOG_ERROR,
	LOG_FATAL,

	LOG_MAX,
};

enum LogRollType
{
	LOG_ROLL_NONE,
	LOG_ROLL_HOUR,
	LOG_ROLL_DAY,
};



class LogInterface
{
public:
	LogInterface(void);
	virtual ~LogInterface(void);

	void SetLogName(const std::string &name) { m_name = name; }
	const std::string &Name() { return m_name; }

	virtual void ProcessLog(LOGTYPE type, const char *header, const char *msg) = 0;
	virtual void Flush() = 0;

protected:
	std::string m_name;
};

