/**
*
* @file     tmempool_shm.h  
* @brief   基于共享内存的 内存池
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
#ifndef TMEMPOOL_SHM_H
#define TMEMPOOL_SHM_H

#include "comm/tmempool.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**在共享内存上创建mempool
*@param[in,out]	 ppstPool  内存池指针的指针
*@param[in]  uiKey    共享内存Key
*@param[in]  iMax     内存池最多容纳的元素个数
*@param[in]  iUnit    每个元素的大小

*@retval  >=0  成功，从ppstPool中返回mempool指针
          <0      失败
          
*@note   需要判断返回值
  
*/
int tmempool_new_shm(TMEMPOOL** ppstPool, key_t uiKey, int iMax, int iUnit);

#ifdef __cplusplus
}
#endif

#endif  /* TMEMPOOL_SHM_H */

