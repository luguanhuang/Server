/**
 *
 * @file     shtable.h
 * @brief    哈希表实现
 *
 * @author steve
 * @version 1.0
 * @date 2007-04-05
 *
 *
 * Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
 * All rights reserved.
 *
 */

#ifndef SHTABLE_H
#define SHTABLE_H

/*************************************************************
 #FUNCTION:	static hash table defintion.
 #AUTHOR:	Niezhiming
 #DATE:		14-7-2005
 *************************************************************/
#include <stdio.h>
#include "pal/ttypes.h"

#if defined(_WIN32) || defined(_WIN64)

#endif/*#if defined(_WIN32) || defined(_WIN64)*/

#ifdef __cplusplus
extern "C"
{
#endif

struct tagSHtable;
typedef struct tagSHtable *LPSHTABLE;

typedef int (*SHT_CMP)(const void* pvData1, const void* pvData2);
typedef unsigned int (*SHT_HASHCODE)(const void* pvData);
typedef int (*SHT_PRINT)(FILE* fp, const void* pvData);

/** The default method for converting string to int.
 *@param[in]	 szKey  a string end with '\0'

 *@retval  hash code
 */
unsigned int sht_get_code(const char* szKey);

/**计算shtable的大小
 *@param[in]	 iBuck  桶数
 *@param[in]  iMax   元素个数
 *@param[in]  iUnit  每个元素的大小
 *@retval  0  失败

 */
size_t sht_get_size(size_t iBuck, size_t iMax, size_t iUnit);

/**检查内存是否符合shtable的格式，一般是   重新attach shtable shm 时使用
 *@param[in]	 pvBuff  内存地址指针
 *@param[in]  iBuff   内存大小
 *@param[in]  iBucket  Bucket num
 *@param[in]  iMax    Max item num
 *@param[in]  iUnit   每个item的大小
 *@retval  0  成功
 -1  失败
 */
int sht_check(void* pvBuff, size_t iBuff, size_t iBucket, size_t iMax,
		size_t iUnit);

/**在内存中生成一个shtable
 *@param[in]      iBucket  Bucket num
 *@param[in]      iMax    Max item num
 *@param[in]      iUnit   每个item的大小
 *@param[in,out]  piBuff  指向生成的shtable的大小的指针

 *@retval  not NULL  成功shtable 指针
 NULL  失败

 *@note  此函数与sht_destroy 配对使用
 */
LPSHTABLE sht_create(size_t iBucket, size_t iMax, size_t iUnit, size_t* piBuff);

/**初始化shtable 内存
 *@param[in]	 pvBuff  内存地址指针
 *@param[in]  iBuff   内存大小
 *@param[in]  iBucket  Bucket num
 *@param[in]  iMax    Max item num
 *@param[in]  iUnit   每个item的大小

 *@retval  not NULL  成功shtable 指针
 NULL  失败
 */
LPSHTABLE
sht_init(void* pvBuff, size_t iBuff, size_t iBucket, size_t iMax, size_t iUnit);

/**attach shtable 内存，实际上是执行sht_check检查内存再返回shtable 指针
 *@param[in]	 pvBuff  内存地址指针
 *@param[in]  iBuff   内存大小
 *@param[in]  iBucket  Bucket num
 *@param[in]  iMax    Max item num
 *@param[in]  iUnit   每个item的大小

 *@retval  not NULL  成功shtable 指针
 NULL  失败
 */
LPSHTABLE sht_attach(void* pvBuff, size_t iBuff, size_t iBucket, size_t iMax, size_t iUnit);

/**insert unique item into shtable
 *@param[in]	 pstTab  shtable 指针
 *@param[in]  pvData  insert item data 指针
 *@param[in]  pfnCmp  比较两个item data 的函数指针, 此比较函数的
 返回值为=0 两item data相同, <0 或>0 两item data不相同
 *@param[in]  pfnHashCode  根据item data 生成hash code 的函数指针

 *@retval  not NULL  成功，shtable 内存放data的地址指针
 NULL      失败
 *@note    此函数调用成功后只是在shtable内分配了一块内存，需要外部
 将要插入的数据再copy到shtable中，此方式是为了增加灵活性，
 可参见文档中的代码示例
 */
void* sht_insert_unique(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp,
		SHT_HASHCODE pfnHashCode); /* insert a item into hashtable, keep it unique.*/

/** insert a item into hashtable, do not keep unique.*/
void* sht_insert_multi(LPSHTABLE pstTab, const void* pvData,
		SHT_HASHCODE pfnHashCode);

/**find item in shtable
 *@param[in]	 pstTab  shtable 指针
 *@param[in]  pvData  find item data 指针
 *@param[in]  pfnCmp  比较两个item data 的函数指针, 此比较函数的
 返回值为=0 两item data相同, <0 或>0 两item data不相同
 *@param[in]  pfnHashCode  根据item data 生成hash code 的函数指针

 *@retval  not NULL  成功，shtable 内存放data的地址指针
 NULL      失败

 *@note   pvData只需要填入与pfnCmp和pfnHashCode 相关的部分即可
 */
void* sht_find(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);

/**remove item in shtable
 *@param[in]	 pstTab  shtable 指针
 *@param[in]  pvData  remove item data 指针
 *@param[in]  pfnCmp  比较两个item data 的函数指针, 此比较函数的
 返回值为=0 两item data相同, <0 或>0 两item data不相同
 *@param[in]  pfnHashCode  根据item data 生成hash code 的函数指针

 *@retval  not NULL  成功，shtable 内存放data的地址指针
 NULL      失败

 *@note   pvData只需要填入与pfnCmp和pfnHashCode 相关的部分即可
 */
void* sht_remove(LPSHTABLE pstTab, const void* pvData, SHT_CMP pfnCmp, SHT_HASHCODE pfnHashCode);

/**remove item in shtable by pos
 *@param[in]	 pstTab  shtable 指针
 *@param[in]  iPos    remove item data 在shtable 在item list 中的索引位置

 *@retval  not NULL  成功，shtable 内存放data的地址指针
 NULL      失败

 *@note   item list 是一个数组，其索引>=0 , <pstTab->iMax
 */
void* sht_remove_by_pos(LPSHTABLE pstTab, int iPos);

int sht_remove_by_dataptr(LPSHTABLE pstTab, void *pvData);

/**get item in shtable by iPos
 *@param[in]	 pstTab  shtable 指针
 *@param[in]  iPos    item data 在shtable 在item list 中的索引位置
 *@param[in,out] pfValid  标识此item data 是否有效, *pfValid = 1 有效，=0 无效

 *@retval  not NULL  成功，shtable 内存放data的地址指针
 NULL      失败

 *@note   item list 是一个数组，其索引>=0 , <pstTab->iMax

 */
void* sht_pos(LPSHTABLE pstTab, int iPos, int* pfValid);

size_t sht_get_max_items(LPSHTABLE pstTab);
/*
 * get the num of used items in the sht
 * @param[in]	 pstTab  shtable 指针
 * @retval Num of used items in the sht.
 * */
size_t sht_get_used_items(LPSHTABLE pstTab);

/**与sht_create配对使用，释放资源
 *@param[in,out]	 ppstTab  shtable 指针的指针
 *@retval NULL 成功
 not NULL 失败（不会返回失败）
 */
int sht_destroy(LPSHTABLE* ppstTab);

/**重建item 空闲链表和bucket 链，较少使用
 *@param[in,out]	 pstTab  shtable 指针
 *@retval NULL 成功
 not NULL 失败（不会返回失败）
 */
int sht_rebuild(LPSHTABLE pstTab);

/**判断shtable item data空间是否为空, 返回1 为空, 0 不空*/
int sht_is_empty(LPSHTABLE pstTab);

/**判断shtable item data空间是否已满, 返回1 已满, 0 未满*/
int sht_is_full(LPSHTABLE pstTab);

/**遍历shtable的item list 中的所有项，对每一个valid item data 调用pfnPrint(fp, pvData)*/
int sht_dump_all(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

/**遍历shtable的item list 中的所有valid item项，对每一个item data 调用pfnPrint(fp, pvData)*/
int sht_dump_valid(LPSHTABLE pstTab, FILE* fp, SHT_PRINT pfnPrint);

/**已知item hash值强制插入，特殊情况下使用，by kent 2008.1.18*/
void *sht_insert_force(LPSHTABLE pstTab, unsigned int uCode);

void *sht_get_by_pos(LPSHTABLE pstTab, size_t iIdx);

/**通过数据获取元素pos
 *@param pvData  sht_insert* 返回的指针
 *@retval 0 成功， -1 失败
 */
int sht_get_pos(LPSHTABLE pstTab, void* pvData);

#ifdef __cplusplus
}
#endif

#endif /* SHTABLE_H */
