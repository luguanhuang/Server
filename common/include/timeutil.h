#ifndef _Time_Util_H__
#define _Time_Util_H__

#include <string>


class TimeUtil
{
public:

	/**
	 * 获取从 epoch (1970-1-1 00:00:00)开始到当前时刻经历的秒数
	 */
	static UINT32 GetTime();


	/**
	 * 获取相对于机器启动后的毫秒数
	 */
	static UINT64 GetMilliSecond();

	/**
	 * 获取相对于机器启动后的微秒数
	 */
	static UINT64 GetMicroSecond();

	
	/**
	 * 格式化为 YY-MM-DD hh:mm::ss 的字符串
	 */
	static std::string GetDateTimeStr(UINT32 dwTimeSec);


	/**
	 * 格式化为 YY-MM-DD 的字符串
	 */
	 static std::string GetDateStr(UINT32 dwTimeSec);


	/**
	 * 格式化为 hh:mm::ss 的字符串
	 */
	 static std::string GetTimeStr(UINT32 dwTimeSec);


	 /**
	  * Sleep with milliseconds
	  */ 
	static void Sleep(UINT32 dwMsec); 
	static void SleepAtLeast(UINT32 dwMsec);

	static UINT64 GetFrameTime();
	static void UpdateFrameTime();
	static float GetDeltaTime();
};

#endif
