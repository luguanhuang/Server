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


	//poTimer:		Timer�ص��ӿ�
	//dwID:			��ʱIdentifier
	//dwInterval:	��ʱ���������Ϊ��λ	
	//nCount:		��������, -1Ϊ��Զ���� 
	//return:		�ɹ�����Timer�ľ����ʧ�ܷ��� INVALID_HTIMER
	HTIMER SetTimer(ITimer* poTimer, UINT32 dwID, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno);
	HTIMER SetTimer(const XDelegate &roDelegate, IArgs *pargs, void *arg, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno);

	UINT32 GetTimeLeft(HTIMER hTimer);

	//hTimer:		ҪKill��Timer��� (SetTimerʱ���ص�ֵ)
	void KillTimer(HTIMER hTimer); 


	void Update(); 

private:
	ITimerManager*	m_poTimerMgr;
};

#endif