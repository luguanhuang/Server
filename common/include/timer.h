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
	//dwCount:	Timer��ǰ��������
	//
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount) = 0;
};

class XDelegate;

class ITimerManager
{
public:
	virtual ~ITimerManager() {}
	
	//poTimer:		Timer�ص��ӿ�
	//dwID:			��ʱIdentifier
	//dwInterval:	��ʱ���������Ϊ��λ	
	//nCount:		��������, -1Ϊ��Զ���� 
	//return:		�ɹ�����Timer�ľ����ʧ�ܷ��� INVALID_HTIMER
	virtual HTIMER SetTimer(ITimer* poTimer, UINT32 dwID, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno) = 0;
	virtual HTIMER SetTimer(const XDelegate &roDelegate, IArgs *pargs, void *arg, UINT32 dwInterval, UINT32 dwCount, const char *fileName, int lineno) = 0;

	//���Timer���೤ʱ���ᴥ��
	virtual UINT32 GetTimeLeft(HTIMER hTimer) = 0;

	//hTimer:		ҪKill��Timer��� (SetTimerʱ���ص�ֵ)
	virtual void KillTimer(HTIMER hTimer) = 0; 


	virtual void Update() = 0; 


	virtual void Release() = 0;
};


ITimerManager*	CreateTimerMgr();


#endif
