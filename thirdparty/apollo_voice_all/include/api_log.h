//2015-12-21 by purperzhang

#ifndef APPS_APOLLO_CHAT_INCLUDE_API_LOG_H_
#define APPS_APOLLO_CHAT_INCLUDE_API_LOG_H_

///>如果没有定义WIN32则定义linux宏
#ifndef WIN32
#ifndef linux
#define linux
#endif
#endif

namespace ns_log
{

enum LogLevel
{
	CS_LOG_DEBUG,
	CS_LOG_ERROR,
	CS_LOG_NO_LOG,
};

#ifdef linux
class MLogInterface
{
public:
	virtual ~MLogInterface(){}
	virtual void log(LogLevel log_level,const char* file,int line,const char* func,const char* format,...) __attribute__((format(printf,6,7)))=0;
    virtual bool is_priority_enabled(LogLevel level)=0;
};
#endif

#if defined(WIN32) || defined(WIN64)
class MLogInterface
{
public:
	virtual ~MLogInterface(){}
	virtual void log(LogLevel log_level,const char* file,int line,const char* func,const char* format,...)=0;
    virtual bool is_priority_enabled(LogLevel level)=0;
};
#endif
}



#endif /* APPS_APOLLO_CHAT_INCLUDE_API_LOG_H_ */



