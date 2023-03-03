/**
*
* @file     shtable_shm.h
* @brief   基于共享内存的哈希表实现
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

#ifndef SHTABLE_SHM_H

#define SHTABLE_SHM_H

#include "comm/shtable.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**在共享内存上创建哈希表
*@param[in]	 iBucket  哈希表桶数
*@param[in]  iMax     哈希表最多容纳的元素个数
*@param[in]  iUnit    每个元素的大小
*@param[in]  uiKey    共享内存Key
*@retval  not NULL  成功，哈希表指针
          NULL      失败

*@note   需要判断返回值

*/
LPSHTABLE sht_create_shm(size_t iBucket, size_t iMax, size_t iUnit, key_t uiKey);
#ifdef __cplusplus
}
#endif

#endif /* SHTABLE_SHM_H */
