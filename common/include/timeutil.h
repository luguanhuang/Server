#ifndef _Time_Util_H__
#define _Time_Util_H__

#include <string>


class TimeUtil
{
public:

	/**
	 * ��ȡ�� epoch (1970-1-1 00:00:00)��ʼ����ǰʱ�̾���������
	 */
	static UINT32 GetTime();


	/**
	 * ��ȡ����ڻ���������ĺ�����
	 */
	static UINT64 GetMilliSecond();

	/**
	 * ��ȡ����ڻ����������΢����
	 */
	static UINT64 GetMicroSecond();

	
	/**
	 * ��ʽ��Ϊ YY-MM-DD hh:mm::ss ���ַ���
	 */
	static std::string GetDateTimeStr(UINT32 dwTimeSec);


	/**
	 * ��ʽ��Ϊ YY-MM-DD ���ַ���
	 */
	 static std::string GetDateStr(UINT32 dwTimeSec);


	/**
	 * ��ʽ��Ϊ hh:mm::ss ���ַ���
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
