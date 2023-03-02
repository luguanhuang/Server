#ifndef _Mini_Dump_H__
#define _Mini_Dump_H__

#include <Windows.h>
#include <DbgHelp.h>
#include <time.h>
#include <stdio.h>

#pragma comment(lib, "DbgHelp.lib")

LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS *pExceptionInfo)
{
	LONG ret = EXCEPTION_CONTINUE_SEARCH;

	time_t now = time(NULL);
	struct tm oTime;
	localtime_s(&oTime, &now);

	char szFile[128] = {0};

	sprintf_s(szFile, 128, "%4d-%02d-%02d %02d-%02d-%02d.dmp",
		oTime.tm_year + 1900, oTime.tm_mon + 1, oTime.tm_mday,
		oTime.tm_hour, oTime.tm_min, oTime.tm_sec);

	HANDLE hFile = CreateFileA(szFile, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); 
	if (hFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = ::GetCurrentThreadId();
		ExInfo.ExceptionPointers = pExceptionInfo;
		ExInfo.ClientPointers = NULL;
		BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &ExInfo, NULL, NULL);
		ret = EXCEPTION_EXECUTE_HANDLER;
		CloseHandle(hFile);
	}

	return ret;
}


#endif