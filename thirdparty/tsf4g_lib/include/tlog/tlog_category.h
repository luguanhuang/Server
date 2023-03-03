/**
 *
 * @file    tlog_category.h
 * @brief   日志类记录日志模块
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

#ifndef TLOG_CATEGORY_H
#define TLOG_CATEGORY_H

#include <stdarg.h>
#include <tlog/tlog_event.h>
#include "pal/ttypes.h"

#define TLOG_NAME_LEN 64
#ifdef __cplusplus
extern "C"
{
#endif

struct tagTLogCategoryInst
{
    // 以a开头的变量已经不能使用，被移动到内部了，但是为了保持兼容，不要删除这些变量的声明
    // 也不要在代码中使用这些变量
    char a_szName[TLOG_NAME_LEN];
    struct tagTLogCategoryInst* a_pstParent;
    struct tagTLogCategoryInst* a_pstForward;
    struct tagTLogCategoryInst* a_pstCloneParent;
    struct tagTLOGCategory * a_pstCat;
    int a_iInited;
    int a_iSeq;
    // 到这里为止，为了和1.11兼容，前面的东西都不能修改，后面的东西可以去掉
    int* piPriorityHigh;
    int* piPriorityLow;
};

typedef struct tagTLogCategoryInst TLOGCATEGORYINST;
typedef struct tagTLogCategoryInst *LPTLOGCATEGORYINST;

/**
 * @bref 向日志类写日志，有传递关系
 * @param[in] a_pstCatInst 日志文件句柄
 * @param[in] a_pstEvt     日志事件内容
 *
 * @return 0 成功
 *		非0 失败
 */
int tlog_category_log(LPTLOGCATEGORYINST a_pstCatInst, TLOGEVENT* a_pstEvt);

/**
 * 使用valist方式写入日志，有传递关系
 * @param[in] a_pstCatInst 日志文件句柄
 * @param[in] a_pstEvt     日志事件内容
 * @param[in] a_pszFmt     fmt字符串
 * @param[in] ap     	  valist
 *
 * @return 0 成功
 *		非0 失败
 */
int tlog_category_logv_va_list(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT *a_pstEvt, const char* a_pszFmt, va_list a_ap);

#if defined(_WIN32) || defined(_WIN64)
int tlog_category_logv(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT* a_pstEvt, const char* a_pszFmt, ...);
#else
int tlog_category_logv(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT* a_pstEvt, const char* a_pszFmt, ...) __attribute__((format(printf,3,4)));
#endif

/*
 * 设置日志category的优先级
 * @param [in] a_pstCatInst 日志类
 * @param [in] iPriorityHigh 高优先级
 * @param [in] iPriorityLow 低优先级
 * @return 0 成功
 *      非0 失败
 * */
int tlog_category_set_priority(TLOGCATEGORYINST *a_pstCatInst, int iPriorityHigh, int iPriorityLow);

/*
 * 用tdr的方式将内存数据结构写入日志
 * @param [in] a_pstCatInst 日志类
 * @param [in] a_pstEvt 日志事件
 * @param [in] a_pmeta 对应缓冲区的tdr
 * @param [in] a_buff 缓冲区
 * @param [in] a_bufflen 缓冲区大小
 * @param [in] a_version 数据裁减版本
 * @return 0 成功
 *         非0 失败
 * */
int tlog_category_log_tdr(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT *a_pstEvt, void * a_pmeta, void *a_buff, intptr_t a_bufflen,
        int a_version);
/*
 * 设置日志类的format
 * @param [in] a_pstCatInst 日志类
 * @param [in] pszPattern 日志format
 * @return 0 成功
 *         非0 失败
 * */
int tlog_category_set_format(TLOGCATEGORYINST *a_pstCatInst, const char *pszPattern);

/**
 * 设置日志类的时间位置，如果设置了，每次记录日志都用这个时间。
 * @param[in] a_pstCatInst 日志类句柄
 * @param[in] pstCurr     指向当前时间的指针
 *
 * @return 0 设置成功
 *       非0 出错
 */
int tlog_category_set_time_ptr(TLOGCATEGORYINST *a_pstCatInst, struct timeval *pstCurr);

/**
 * 写入十六进制日志
 * @param[in] a_pstCatInst 日志类句柄
 * @return 0 成功
 *       非0 失败
 */
int tlog_category_log_hex(TLOGCATEGORYINST *a_pstCatInst, TLOGEVENT *a_pstEvt);

/**
 * 查看category的优先级是否符合，如果这个日志类或者他的传递日志类可以写入日志，返回1，否则返回0
 * @param[in] a_pstCatInst 日志类句柄
 * @param[in] priority     指定优先级
 *
 * @return 0 不需要写入日志
 *      非0 需要写入日志
 */
int tlog_category_can_write(TLOGCATEGORYINST *a_pstCatInst, int a_priority, unsigned int a_uId, unsigned int a_uCls);


/**
* 查询tlog是否将日志发送到SOCKET缓冲区中
* @param[in] a_pstCatInst 日志类句柄
*
* @return 0 : 将日志发送到SOCKET缓冲区成功
*    非0    : 失败(包括参数错误和发送到网络缓冲区失败;
                   如果是vec类型，只要有一个网络类型发送失败就认为是失败)
*/
int tlog_query_net_category_status(TLOGCATEGORYINST *a_pstCatInst);


#ifdef __cplusplus
}
#endif

#endif /* TLOG_CATEGORY_H */

