/**
*
* @file    tlognet.h  
* @brief   日志网络模块
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

#ifndef TLOGNET_H
#define TLOGNET_H

#include "tlogio.h"
#include "tlognetdef.h"

#ifdef __cplusplus
extern "C"
{
#endif

struct tagTLogDevNetInst
{
	int s;
	int iType; 
	int iCurRetry;
	char *pszBuff;
	int iIsSendNetSucc; /*用于判断日志是否发送到SOCKET缓冲区中的标志位，非零:发送成功，零表示失败*/
};

typedef struct tagTLogDevNetInst	TLOGDEVNETINST;
typedef struct tagTLogDevNetInst	*LPTLOGDEVNETINST;

struct tagTLogNet
{
	TLOGDEVNET* pstDev;
	TLOGDEVNETINST stInst;
};

typedef struct tagTLogNet		TLOGNET;
typedef struct tagTLogNet		*LPTLOGNET;

/**
* 初始化网络句柄
* @param[in,out] a_pstLogNet 日志网络模块句柄
* @param[in] a_pstDev 日志网络模块配置
*
* @return  0 成功
*			非0 失败
*/
int tlognet_init(TLOGNET* a_pstLogNet, TLOGDEVNET* a_pstDev);

/**
* 向网络句柄写入日志内容
* @param[in,out] a_pstLogNet 日志网络模块句柄
* @param[in] a_pszBuff 需要写入的字符串内容
* @param[in] a_iBuff 需要写入的字符串长度
*
* @return  0 成功
*			非0 失败
*/
int tlognet_write(TLOGNET* a_pstLogNet, const char* a_pszBuff, int a_iBuff);

/**
* 释放网络句柄
* @param[in,out] a_pstLogNet 日志网络模块句柄
*
* @return  0 成功
*			非0 失败
*/
int tlognet_fini(TLOGNET* a_pstLogNet);

/**
* 向网络句柄写入向量
* @param[in,out] a_pstLogNet 日志网络模块句柄
* @param[in] a_pstIOVec 写入的字符串向量数组
* @param[in] a_iCount 写入的字符串向量数组大小
*
* @return  0 成功
*			非0 失败
*/
int tlognet_writev(TLOGNET* a_pstLogNet, const TLOGIOVEC* a_pstIOVec, int a_iCount);

#ifdef __cplusplus
}
#endif

#endif /* TLOGNET_H */


