//2015-12-21 by purperzhang

#ifndef LARGE_ROOM_INCLUDE_API_LOG_DEMO_H_
#define LARGE_ROOM_INCLUDE_API_LOG_DEMO_H_

#ifdef linux
#include <sys/time.h>
#elif defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif

#ifdef WIN32
#define snprintf _snprintf
#endif

#include<stdarg.h>
#include<fstream>
#include<time.h>
#include"api_log.h"

#ifdef USE_TSF4G_TLOG
#include "tlog/tlog.h"
#include "tlog/tlog_event.h"
#include "tlog/tlog_category.h"
namespace ns_log
{
class TLogger : public MLogInterface
{
private:
    TLOGCATEGORYINST *m_cattegory;
public:
    TLogger(TLOGCATEGORYINST* cattegory):m_cattegory(cattegory){}
    virtual ~TLogger(){}

    virtual void log(LogLevel level,const char* file, int line, const char* func, const char* format, ...)
    {
        if(is_priority_enabled(level))
        {
            TLOGEVENT stEvt;
            if(level==CS_LOG_DEBUG)
            {
            	stEvt.evt_priority=TLOG_PRIORITY_DEBUG;
            }
            else if(level==CS_LOG_ERROR)
            {
            	stEvt.evt_priority=TLOG_PRIORITY_ERROR;
            }
            else
            {
            	stEvt.evt_priority=TLOG_PRIORITY_NULL;
            }

            va_list ap;
            TLOGLOCINFO locinfo;

            locinfo.loc_file = file;
            locinfo.loc_line = line;
            locinfo.loc_function = func;
            locinfo.loc_data = NULL;
            stEvt.evt_id = 0;
            stEvt.evt_cls  = 0;
            stEvt.evt_is_msg_binary = 0;
            stEvt.evt_loc = &locinfo;
            va_start(ap, format);
            tlog_category_logv_va_list(m_cattegory, &stEvt, format, ap);
            va_end(ap);
        }
    }

    virtual bool is_priority_enabled(LogLevel level)
    {
    	if(level==CS_LOG_DEBUG)
    	{
    		return tlog_category_is_priority_enabled(m_cattegory, TLOG_PRIORITY_DEBUG) && tlog_category_can_write(m_cattegory, TLOG_PRIORITY_DEBUG,0, 0);
    	}
    	else if(level==CS_LOG_ERROR)
        {
    		return tlog_category_is_priority_enabled(m_cattegory, TLOG_PRIORITY_ERROR) && tlog_category_can_write(m_cattegory, TLOG_PRIORITY_ERROR,0, 0);
        }
    	else if(level==CS_LOG_NO_LOG)
    	{
    		return tlog_category_is_priority_enabled(m_cattegory, TLOG_PRIORITY_NULL) && tlog_category_can_write(m_cattegory, TLOG_PRIORITY_NULL,0, 0);
    	}
    	else
    	{
    		return false;
    	}
    }

	virtual void *GetLoggerHandle()
    {
        return m_cattegory;
    }

};

}//namespace
#endif  // USE_TSF4G_TLOG


namespace ns_log
{
class MLog:public MLogInterface
{
	std::ofstream error_file;
	std::ofstream debug_file;
	int debug_len;
	int error_len;
	int file_limit;
	LogLevel cur_log_level;
public:
	MLog()
	{
		error_file.open("./log/apollo.error",std::ios::out);
		debug_file.open("./log/apollo.debug",std::ios::out);
		debug_len=0;
		error_len=0;
		file_limit=100000000;
		cur_log_level=CS_LOG_DEBUG;
	}
	virtual ~MLog()
	{
		error_file.close();
		debug_file.close();
	}
	virtual void log(ns_log::LogLevel log_level,const char* file,int line,const char* func,const char* format,...)
	{
		char buf[10240];
		char time_buf[100];
		int len=snprintf(buf,sizeof(buf),"[%s](%s:%d)(%s)] [%s]: ",
				get_log_time(time_buf,sizeof(time_buf)),
				file,
				line,
				func,
				log_level==ns_log::CS_LOG_DEBUG?"DEBUG":"ERROR"
				);

		va_list ap;
		va_start(ap,format);
		len=vsnprintf(buf+len,sizeof(buf)-len,format,ap)+len;
		va_end(ap);

		buf[len]='\n';len++;

		if(log_level==ns_log::CS_LOG_DEBUG)
		{
			debug_file.write(buf,len);
			debug_len+=len;
		}
		else
		{
			debug_file.write(buf,len);
			error_file.write(buf,len);
			debug_len+=len;
			error_len+=len;
		}
		debug_file.flush();
		error_file.flush();

		if(debug_len>file_limit)
		{
			debug_file.close();
			debug_file.open("log.debug",std::ios::out);
			debug_len=0;
		}
		if(error_len>file_limit)
		{
			error_file.close();
			error_file.open("log.error",std::ios::out);
			error_len=0;
			
		}
		return;
	}
    virtual bool is_priority_enabled(ns_log::LogLevel level)
    {
    	return level>=cur_log_level;
    }
#ifdef linux
    const char* get_log_time(char*buf,int sizebuf)
    {
    	time_t now=time(0);
    	struct timeval tv;
    	struct tm* tm=localtime(&now);
    	gettimeofday(&tv,0);
    	snprintf(buf,sizebuf,"%04d%02d%02d-%02d%02d%02d %06d",
    			tm->tm_year+1900,
    			tm->tm_mon+1,
    			tm->tm_mday,
    			tm->tm_hour,
    			tm->tm_min,
    			tm->tm_sec,
    			(int)tv.tv_usec);
    	return buf;

    }
#elif defined(WIN32) || defined(WIN64)
    const char* get_log_time(char*buf,int sizebuf)
    {
    	time_t now=time(0);
    	struct tm* tm=localtime(&now);
    	snprintf(buf,sizebuf,"%04d%02d%02d-%02d%02d%02d %06d",
    			tm->tm_year+1900,
    			tm->tm_mon+1,
    			tm->tm_mday,
    			tm->tm_hour,
    			tm->tm_min,
    			tm->tm_sec,
    			GetTickCount()%1000);
    	return buf;

    }
#endif
    void set_log_level(LogLevel ll)
    {
    	cur_log_level=ll;
    }
    void set_log_file_limit_MB(int s)
    {
    	file_limit=s*1000000;
    }
};
}//namespace


#endif /* LARGE_ROOM_INCLUDE_API_LOG_DEMO_H_ */
