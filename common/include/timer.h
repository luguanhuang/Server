#ifndef _Timer_H__
#define _Timer_H__

#include "define.h"
#include "xdelegate.hpp"

typedef UINT64 HTIMER; 
#define INVALID_HTIMER 0 


class ITimer
{
public: 
	virtual ~ITimer() {}

	//
	//dwID:		Timer Identifier
	//dwCount:	Timer当前触发次数
	//
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount) = 0;
};

class XDelegate;

class ITimerManager
{
public:
	virtual ~ITimerManager() {}
	
	//poTimer:		Timer回调接口
	//dwID:			定时Identifier
	//dwInterval:	定时间隔，毫秒为单位	
	//nCount:		触发次数, -1为永远触发 
	//return:		成功返回Timer的句柄，失败返回 INVALID_HTIMER
	virtual HTIMER SetTimer(ITimer* poTimer, UINT32 dwID, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno) = 0;
	virtual HTIMER SetTimer(const XDelegate &roDelegate, IArgs *pargs, void *arg, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno) = 0;

	//获得Timer过多长时间后会触发
	virtual UINT32 GetTimeLeft(HTIMER hTimer) = 0;

	//hTimer:		要Kill的Timer句柄 (SetTimer时返回的值)
	virtual void KillTimer(HTIMER hTimer) = 0; 


	virtual void Update() = 0; 


	virtual void Release() = 0;
};


ITimerManager*	CreateTimerMgr();


#endif
