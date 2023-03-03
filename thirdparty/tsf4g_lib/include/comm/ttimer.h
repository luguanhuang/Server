#if 0

#ifndef _TTIMER_H_
#define _TTIMER_H_

#include "pal/pal.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef uint32_t tick_t;

struct tagTTimer
{
	int iCallback;
	intptr_t callBackData;
	tick_t timeOut;
};
typedef struct tagTTimer TTIMER;
typedef struct tagTTimer *LPTTIMER;

typedef int (*TTIMERCALLBACK)(TTIMER *pstTimer);
struct tagTTimerCallBack
{
	uint32_t iCallbackId;
	TTIMERCALLBACK pfnCallback;
};

struct tagTTimerVal;
typedef struct tagTTimerVal TTIMERVAL;
typedef struct tagTTimerVal *LPTTIMERVAL;

struct tagTTimerElement;
typedef struct tagTTimerElement TTIMERELEMENT;
typedef struct tagTTimerElement *LPTTIMERELEMENT;

struct tagTTimerHandle;
typedef struct tagTTimerHandle TTIMERHANDLE;

struct tagTTimerVal
{
	TLISTNODE stList;
};

struct tagTTimerElement
{
	TTIMER stTimer;
	tick_t timeoutTime;
	TLISTNODE stNode;
};

#define NSLICE (sizeof(tick_t))

struct tagTTimerHandle
{

	TTIMERCALLBACK apfnCallBack[MAX_CALL_BACK + 1];

	intptr_t iMaxTimer;

	tick_t currentTime;

	TTIMERVAL tv[NSLICE][256];

	LPTTIMERELEMENT apstTimerElements;
	void *pvBase;
	LPTMEMPOOL pstTimerPool;

};

typedef struct tagTTimerCallBack TTIMERTICKCALLBACK;
typedef struct tagTTimerCallBack *LPTTIMERTICKCALLBACK;

struct tagTTimerHandle;
typedef struct tagTTimerHandle *LPTTIMERHANDLE;

int ttimer_init(LPTTIMERHANDLE *ppstTimer, size_t a_max_timer);

int ttimer_init_handler_from_buffer(LPTTIMERHANDLE *a_handle,
		size_t a_max_timer, char *a_pszBuff, size_t a_iBuff);

int ttimer_register_callback(LPTTIMERHANDLE pstTimer, int a_id,
		TTIMERCALLBACK a_pfnCallBack);

int ttimer_register_callbacks(LPTTIMERHANDLE pstTimer,
		LPTTIMERTICKCALLBACK astCallBack, int iCount);

int ttimer_add(LPTTIMERHANDLE a_handle, LPTTIMER a_pstTimer);

int ttimer_tick(LPTTIMERHANDLE a_handle, int MaxExpire);

#ifdef __cplusplus
}
#endif

#endif

#endif

