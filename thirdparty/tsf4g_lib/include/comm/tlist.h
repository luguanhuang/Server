#ifndef T_LIST_H
#define T_LIST_H

#include "pal/ttypes.h"
#include "comm/tmempool.h"

#ifdef __cplusplus
extern "C"
{
#endif
#if 0
// Here we just deal with the list node.
// So the Addr should only contail iOffset.

struct tagAddr
{
	// The idx of data.
	intptr_t idx;
	// The head of data.
	intptr_t iOffset;
};

typedef struct tagAddr ADDR;
typedef struct tagAddr *LPADDR;
#endif

struct tlist_node
{
	size_t iPrev;
	size_t iNext;
};

typedef struct tlist_node TLISTNODE;
typedef struct tlist_node *LPTLISTNODE;

#define TLIST_INIT(pstList) do{\
		(pstList)->iPrev = 0;\
		(pstList)->iNext = 0;\
	}while(0)

#define TLIST_NEXT(pstList) ((LPTLISTNODE)((size_t)(pstList) + (pstList)->iNext ))
#define TLIST_PREV(pstList) ((LPTLISTNODE)((size_t)(pstList) + (pstList)->iPrev ))

#define TLIST_SET_NEXT(pstList,pstNext) ((pstList)->iNext = ((size_t)(pstNext) - (size_t)(pstList)))
#define TLIST_SET_PREV(pstList,pstPrev) ((pstList)->iPrev = ((size_t)(pstPrev) - (size_t)(pstList)))


#define TLIST_INSERT_NEXT(pstList,pstNew) do{\
		TLIST_SET_PREV(TLIST_NEXT(pstList),pstNew);\
		TLIST_SET_NEXT(pstNew,TLIST_NEXT(pstList));\
		TLIST_SET_PREV(pstNew,pstList);\
		TLIST_SET_NEXT(pstList,pstNew);\
	}while(0)

#define TLIST_INSERT_PREV(pstList,pstNew) do{\
		TLIST_SET_NEXT(TLIST_PREV(pstList),pstNew);\
		TLIST_SET_NEXT(pstNew,pstList);\
		TLIST_SET_PREV(pstNew,TLIST_PREV(pstList));\
		TLIST_SET_PREV(pstList,pstNew);\
	}while(0)

//Del the current node.
#define TLIST_DEL(pstNode) do{\
		TLIST_SET_NEXT(TLIST_PREV(pstNode),TLIST_NEXT(pstNode));\
		TLIST_SET_PREV(TLIST_NEXT(pstNode),TLIST_PREV(pstNode));\
		TLIST_INIT(pstNode);\
	}while(0)

#define TLIST_IS_EMPTY(pstList) ((0  == (pstList)->iNext ) && (0 == (pstList)->iPrev))

#define TLIST_FOR_EACH_SAFE(pstPos, pstN, pstHead) \
	for (pstPos = TLIST_NEXT(pstHead), pstN = TLIST_NEXT(pstPos); pstPos != (pstHead);\
		pstPos = pstN, pstN = TLIST_NEXT(pstPos))

#define TLIST_FOR_EACH_PREV_SAFE(pstPos, pstN, pstHead) \
	for (pstPos = TLIST_PREV(pstHead), pstN = TLIST_PREV(pstPos); pstPos != (pstHead);\
		pstPos = pstN, pstN = TLIST_PREV(pstPos))		




#if 0

struct tlist_head;
typedef struct tlist_head *LPTLIST;

typedef int (*TLIST_CALLBACK)(LPTLIST pstList,LPTLISTNODE pElement,void* pvData);

/*
	Create a new list:
*/
int tlist_new(LPTLIST *ppstList,void * pstPool);
/*
	Get the size of the list.
*/

int tlist_get_size(void *pUnused);
int tlist_init(LPTLIST *ppstList,LPTMEMPOOL pstPool,void *pvBuff,size_t iBuffSize);
int tlist_attach(LPTLIST *ppstList,void *pvBuff,int iBuff,void *pvConf);
int tlist_fini(LPTLIST *ppstList);

int tlist_insert_next(LPTLIST pstList,LPTLISTNODE pstNode);
int tlist_insert_prev(LPTLIST pstList,LPTLISTNODE pstNode);
int tlist_remove(LPTLIST pstList,LPTLISTNODE pstNode);

int tlist_for_each(LPTLIST pstList,TLIST_CALLBACK callback,void *data);
LPTLISTNODE tlist_get_tail(LPTLIST pstList);
LPTLISTNODE tlist_get_head(LPTLIST pstList);
int tlist_check_list(LPTLIST pstList);
#endif
#ifdef __cplusplus
}
#endif

#endif


