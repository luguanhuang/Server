#ifndef _TimerMgr_H__
#define _TimerMgr_H__

#include "timer.h"
#include "singleton.h"


class CTimerMgr
{
	CTimerMgr();
	~CTimerMgr();
	DECLARE_SINGLETON(CTimerMgr)

public:
	bool Init();
	void UnInit();


	//poTimer:		Timer回调接口
	//dwID:			定时Identifier
	//dwInterval:	定时间隔，毫秒为单位	
	//nCount:		触发次数, -1为永远触发 
	//return:		成功返回Timer的句柄，失败返回 INVALID_HTIMER
	HTIMER SetTimer(ITimer* poTimer, UINT32 dwID, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno);
	HTIMER SetTimer(const XDelegate &roDelegate, IArgs *pargs, void *arg, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno);

	UINT32 GetTimeLeft(HTIMER hTimer);

	//hTimer:		要Kill的Timer句柄 (SetTimer时返回的值)
	void KillTimer(HTIMER hTimer); 


	void Update(); 

private:
	ITimerManager*	m_poTimerMgr;
};

#endif