/**
*
* @file     tmempool.h
* @brief    内存池实现
*
* @author kent
* @version 1.0
* @date 2007-04-05
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TMEMPOOL_H
#define TMEMPOOL_H
#include <stdio.h>
#include <stddef.h>
#include "pal/ttypes.h"

#if defined(_WIN32) || defined(_WIN64)

#endif/*#if defined(_WIN32) || defined(_WIN64)*/

#define TMEMPOOL_FIFO           0x00
#define TMEMPOOL_LIFO           0x01

enum tagTMEMPOOLOPT
{
	TMEMPOOL_SET_ALLOC_METHOD_FIFO,
	TMEMPOOL_SET_ALLOC_METHOD_LIFO,
	TMEMPOOL_SET_PAGE_PROTECT,
	TMEMPOOL_RESET_PAGE_PROTECT,
};

typedef enum tagTMEMPOOLOPT TMEMPOOLOPT;

#ifdef __cplusplus
extern "C"
{
#endif

#define IN
#define OUT
#define INOUT
#define TMEMPOOL_API

struct tagTMemPool;
typedef struct tagTMemPool		TMEMPOOL;
typedef struct tagTMemPool		*LPTMEMPOOL;

/**Caculate the size of mem needed by mempool
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@retval  the size of memory required by mempool
*/
TMEMPOOL_API size_t tmempool_calc_size(size_t iMax, size_t iUnit);

/**初始化mempool内存，形成空闲链表
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@param[in]      pvBase    用于形成mempool的内存的指针
*@param[in]      iSize     pvBase 内存的大小

*@retval  0  成功
         -1  失败
*/
TMEMPOOL_API int tmempool_init(INOUT TMEMPOOL** ppstPool,IN  int iMax,IN  size_t iUnit,IN void* pvBase,IN size_t iSize);

/**分配mempool内存并初始化
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小

*@retval  0  成功
         -1  失败

*@note   与tmempool_destroy配对使用
*@note   当ppstPool为NULL时, 返回需要的内存大小
*/
TMEMPOOL_API int tmempool_new(INOUT TMEMPOOL** ppstPool, IN size_t iMax,IN size_t iUnit);

/**attach 一块已存在的mempool内存，并检查内存格式
*@param[in,out]	 ppstPool  mempool指针的指针
*@param[in]      iMax      mempool的容量
*@param[in]      iUnit     应用数据的大小
*@param[in]      pvBase    用于形成mempool的内存的指针
*@param[in]      iSize     pvBase 内存的大小

*@retval  0  成功
         -1  失败
*/
TMEMPOOL_API int tmempool_attach(INOUT TMEMPOOL** ppstPool,IN int iMax,IN  int iUnit, IN void* pvBase, IN size_t iSize);

/**销毁由tmempool_new创建的内存池*/
TMEMPOOL_API int tmempool_destroy(INOUT TMEMPOOL** ppstPool);

/**设置mempool的memblock分配方式
*@param[in]	     pstPool   mempool指针
*@param[in]      iMethod   TMEMPOOL_FIFO  先进先出,默认方式; TMEMPOOL_LIFO 后进先出

*@retval  0  成功
         -1  失败
*/
TMEMPOOL_API int tmempool_set_method(IN TMEMPOOL* pstPool, IN int iMethod);

/**根据索引获取应用数据
*@param[in]	     pstPool   mempool指针
*@param[in]      iIdx      memblock的索引

*@retval  not NULL  成功
          NULL      失败
*/
TMEMPOOL_API void* tmempool_get(IN TMEMPOOL* pstPool,IN int iIdx);

/**get a user data by position
*@param[in]	     pstPool   mempool指针
*@param[in]      iPos      memblock 数组索引0~max-1

*@retval  not NULL  成功
          NULL      失败
*/
TMEMPOOL_API void* tmempool_get_bypos(IN  TMEMPOOL* pstPool, IN  int iPos);

/**/
/**分配一块memblock
*@param[in]	     pstPool   mempool指针

*@retval  >=0     成功,   返回memblock的索引
          -1      失败
*/
TMEMPOOL_API int tmempool_alloc(IN TMEMPOOL* pstPool);

/** Alloc data by pos.
*@param[in]	     pstPool   mempool指针
*@retval  >=0  成功
          -1      失败
*/
TMEMPOOL_API int tmempool_alloc_bypos(IN TMEMPOOL* pstPool);

/** Free the data by it's position */
TMEMPOOL_API int tmempool_free_bypos(IN TMEMPOOL* pstPool,IN int iIdx);

/* Get the number of used items in the mempool */
TMEMPOOL_API int tmempool_get_used_items(IN TMEMPOOL* pstPool);

/* Get capacity of the mempool */
TMEMPOOL_API int tmempool_get_max_items(IN TMEMPOOL* pstPool);

/**释放一块索引为iIdx的memblock */
TMEMPOOL_API int tmempool_free(IN TMEMPOOL* pstPool,IN int iIdx);

TMEMPOOL_API int tmempool_free_byptr(IN TMEMPOOL *pstPool,IN void *pvData);

/**重新生成空闲链表，一般在iStart!=iEnd时使用*/
TMEMPOOL_API int tmempool_fix(IN TMEMPOOL* pstPool);

/**从索引0开始找到第一个使用中的memblock
*@param[in]	     pstPool   mempool指针
*@param[out]     piPos     存储memblock index

*@retval   0      成功
          -1      失败
*/
TMEMPOOL_API int tmempool_find_used_first(IN TMEMPOOL* pstPool, OUT int* piPos);

/**从输入的一个索引的下一个索引开始找到一个使用中的memblock
*@param[in]	     pstPool   mempool指针
*@param[in,out]  piPos
                       in  输入的索引
                      out  >=0  找到的索引
                           -1   已经没有下一个使用中的memblock了

*@retval  >=0    返回传入参数piPos对应的memblock的idx
          -1    查找过程中遇到错误
*/
TMEMPOOL_API int tmempool_find_used_next(IN TMEMPOOL* pstPool,INOUT int* piPos);

/*Dump the used items in the mempool*/
TMEMPOOL_API int tmempool_dump_used(IN TMEMPOOL *pstPool,IN FILE *fp);

/*Get the size fo the mempool */
TMEMPOOL_API size_t tmempool_get_size(IN TMEMPOOL *pstPool);

/* is all the memblocks allocated in pstPool */
TMEMPOOL_API int tmempool_is_full(IN TMEMPOOL *pstPool);

/**
 * 遍历 mempool 中的所有节点(已分配的、未分配的)
 * 并以 节点、pvArg 分别为第一、第二参数调用 pfnTravel
 * @param[in]       pstPool   mempool指针
 * @param[in]       pfnTravel 遍历函数指针
 * @param[in]       pvArg     调用遍历函数的第二个参数
 * @retval   ==0    成功
 * @retval   !=0    失败
 * @pre             pstPool   不能为NULL
 * @pre             pfnTravel 不能为NULL
 * @note    一旦遍历到某个结点，pfnTravel指向的函数返回值!=0, tmempool_travel_all就返回该值
 */
typedef int (*TMEMPOOL_TRAVEL_FUNC)(void* pvNode, void* pvArg);
TMEMPOOL_API int tmempool_travel_all(IN TMEMPOOL *pstPool, TMEMPOOL_TRAVEL_FUNC pfnTravel, void* pvArg);

/**
 * 遍历 mempool 中的所有已分配的节点
 * 并以 节点、pvArg 分别为第一、第二参数调用 pfnTravel
 * @param[in]       pstPool   mempool指针
 * @param[in]       pfnTravel 遍历函数指针
 * @param[in]       pvArg     调用遍历函数的第二个参数
 * @retval   ==0    成功
 * @retval   !=0    失败
 * @pre             pstPool   不能为NULL
 * @pre             pfnTravel 不能为NULL
 * @note    一旦遍历到某个结点，pfnTravel指向的函数返回值!=0, tmempool_travel_all就返回该值
 * @note    节点的遍历按照内存物理顺序，与节点的分配顺序无关
 */
TMEMPOOL_API int tmempool_travel_used(IN TMEMPOOL *pstPool, TMEMPOOL_TRAVEL_FUNC pfnTravel, void* pvArg);

int tmempool_ptr2idx(IN TMEMPOOL *pstPool, void *pvData);

#ifdef __cplusplus
}
#endif

#endif /* TMEMPOOL_H */

