/*
**  @file $RCSfile: swaitque.h,v $
**  general description of this module
**  $Id: swaitque.h,v 1.1 2008-07-03 11:18:27 steve Exp $
**  @author $Author: steve $
**  @date $Date: 2008-07-03 11:18:27 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef SWAITQUE_H

#include <stdio.h>
#include <stddef.h>

#include "pal/ttypes.h"

#ifdef __cplusplus
extern "C"
{
#endif


#define SWQ_MIN_HEAD			0x100
#define SWQ_MIN_ALIGN			0x10
#define SWQ_VERSION				0x0101

#define SWQF_NEEDFREE			0x02
#define SWQF_INITED				0x01

struct tagSWQItem
{
	int iIsValid;
	int iID;
	int iPrev;
	int iNext;

	char szData[1];
};

typedef struct tagSWQItem   SWQITEM;
typedef struct tagSWQItem   *LPSWQITEM;

struct tagSWaitQue
{
	unsigned int cbSize;
	unsigned int uiFlags;
	int iVersion;
	int iBuff;

	int iMax;
	int iItem;

	int iHeadSize;
	int iFreeHead;

	int iDataOff;
	int iDataSize;
	int iDataUnitMin;			/* the data-unit's real size. */
	int iDataUnitMax;			/* the data-unit's occupy size.*/

	int iQueHead;
	int iSeqNo;

	int iRes;					/* reserved. */
	int iRes2;					
};

typedef struct tagSWaitQue	SWAITQUE;
typedef struct tagSWaitQue	*LPSWAITQUE;

#ifndef offsetof
#define offsetof(TYPE, MEMBER) ((intptr_t) &((TYPE *)0)->MEMBER)
#endif
#define SWQ_ROUND(size)					( ( (size) + SWQ_MIN_ALIGN - 1) /SWQ_MIN_ALIGN*SWQ_MIN_ALIGN )

#define SWQ_HEADSIZE()					( SWQ_MIN_HEAD < sizeof(SWAITQUE) ? sizeof(SWAITQUE) : SWQ_MIN_HEAD )

#define SWQ_DATAUNIT(data)				SWQ_ROUND((data) + offsetof(SWQITEM, szData))

#define SWQ_DATASIZE(max, unit)			( (max) * SWQ_DATAUNIT(unit) )

#define SWQ_SIZE(max, unit)				( SWQ_HEADSIZE() + SWQ_DATASIZE(max, unit) )

#define SWQ_GET_ITEM(pstQue, i)			( (LPSWQITEM) ( ((intptr_t)(pstQue)) + pstQue->iDataOff + i*pstQue->iDataUnitMax ) )

#define SWQ_DATA2ITEM(pvData)			( (LPSWQITEM) ( ((intptr_t)(pvData)) - offsetof(SWQITEM, szData)) )
#define SWQ_ITEM2DATA(pvItem)			( (pvItem)->szData )

typedef int (*SWQ_PRINT)(FILE* fp, const void* pvData);

int swq_check(void* pvBuff, int iBuff, int iMax, int iUnit);

/* some operation on wait-queue. */
LPSWAITQUE swq_create(int iMax, int iUnit, int* piBuff);
LPSWAITQUE swq_init(void* pvBuff, int iBuff, int iMax, int iUnit);
LPSWAITQUE swq_attach(void* pvBuff, int iBuff, int iMax, int iUnit);

void* swq_find(LPSWAITQUE pstQue, int* piPos, int iID);
void* swq_pos(LPSWAITQUE pstQue, int iPos, int* piIsValid);

void* swq_alloc(LPSWAITQUE pstQue, int iID);
void* swq_remove(LPSWAITQUE pstQue, int iPos);


int swq_destroy(LPSWAITQUE* ppstQue);

int swq_rebuild(LPSWAITQUE pstQue);

int swq_is_empty(LPSWAITQUE pstQue);
int swq_is_full(LPSWAITQUE pstQue);

int swq_dump_all(LPSWAITQUE pstQue, FILE* fp, SWQ_PRINT pfnPrint);
int swq_dump_valid(LPSWAITQUE pstQue, FILE* fp, SWQ_PRINT pfnPrint);

#ifdef __cplusplus
}
#endif

#endif  /* SWAITQUE_H */
