#pragma once

#include "LogMgr.h"

bool Log_Init(const char *appName, const char* cfgFile);
void Log_Flush();
void Log_Uninit();

#ifdef WIN32
#define __OS_PATH_SEP__ '\\'
#else
#define __OS_PATH_SEP__ '/'
#endif

#ifdef LOG_FULL_FILE_NAME
#define __FILENAME__ __FILE__
#else
#define __FILENAME__ (strrchr(__FILE__, __OS_PATH_SEP__) + 1)
#endif

#define LogDebug(LogFormat, ...) !LogMgr::Instance()->IsEnabled(LOG_DEBUG) ? false : LogMgr::Instance()->Debug("%s(%s:%d) " LogFormat, __FUNCTION__, __FILENAME__, __LINE__, ## __VA_ARGS__) 
#define LogInfo(LogFormat, ...)	 !LogMgr::Instance()->IsEnabled(LOG_INFO)  ? false : LogMgr::Instance()->Info("%s(%s:%d) " LogFormat, __FUNCTION__, __FILENAME__, __LINE__, ## __VA_ARGS__)
#define LogWarn(LogFormat, ...)  !LogMgr::Instance()->IsEnabled(LOG_WARN)  ? false : LogMgr::Instance()->Warn("%s(%s:%d) " LogFormat, __FUNCTION__, __FILENAME__, __LINE__, ## __VA_ARGS__)
#define LogError(LogFormat, ...) !LogMgr::Instance()->IsEnabled(LOG_ERROR) ? false : LogMgr::Instance()->Error("%s(%s:%d) " LogFormat, __FUNCTION__, __FILENAME__, __LINE__, ## __VA_ARGS__)
#define LogFatal(LogFormat, ...) !LogMgr::Instance()->IsEnabled(LOG_FATAL) ? false : LogMgr::Instance()->Fatal("%s(%s:%d) " LogFormat, __FUNCTION__, __FILENAME__, __LINE__, ## __VA_ARGS__)

#define END      LogEnd()
#define SSDebug  !LogMgr::Instance()->IsEnabled(LOG_DEBUG) ? LogMgr::Stream(LOG_DEBUG) : LogMgr::Stream(LOG_DEBUG) << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
#define SSInfo   !LogMgr::Instance()->IsEnabled(LOG_INFO)  ? LogMgr::Stream(LOG_INFO)  : LogMgr::Stream(LOG_INFO) << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
#define SSWarn   !LogMgr::Instance()->IsEnabled(LOG_WARN)  ? LogMgr::Stream(LOG_WARN)  : LogMgr::Stream(LOG_WARN) << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
#define SSError  !LogMgr::Instance()->IsEnabled(LOG_ERROR) ? LogMgr::Stream(LOG_ERROR) : LogMgr::Stream(LOG_ERROR) << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
#define SSFatal  !LogMgr::Instance()->IsEnabled(LOG_FATAL) ? LogMgr::Stream(LOG_FATAL) : LogMgr::Stream(LOG_FATAL) << __FUNCTION__ << "(" << __FILENAME__ << ':' << __LINE__ << ") "
