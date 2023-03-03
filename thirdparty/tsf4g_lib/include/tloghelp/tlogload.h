/**
 *
 * @file    tlogload.h
 * @brief   日志配置文件加载模块
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

#ifndef TLOGLOAD_H
#define TLOGLOAD_H

#include "tlog/tlog.h"
#include "tmng/tmng.h"

#if defined(_WIN32) || defined(_WIN64)

#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif

#ifdef _TSF4G_LARGE_LIB_
#ifdef _DEBUG
#pragma comment(lib, "libtsf4g_d.lib")
#else
#pragma comment(lib, "libtsf4g.lib")
#endif
#else

#ifdef _DEBUG
#pragma comment(lib, "libtloghelp_d.lib")
#else
#pragma comment(lib, "libtloghelp.lib")
#endif
#endif
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define TLOG_DEF_LAYOUT_FORMAT "[%d.%u][%h][(%f:%l) (%F)][%M][%p] %m%n"
#define TLOG_DEF_BILL_LAYOUT_FORMAT "[%d.%u]%m%n"

#define TLOG_DEF_CATEGORY_TEXTROOT		"text"
#define TLOG_DEF_CATEGORY_DATAROOT      "data"
#define TLOG_DEF_CATEGORY_TEXTTRACE     "texttrace"
#define TLOG_DEF_CATEGORY_TEXTERR       "texterr"
#define TLOG_DEF_CATEGORY_BUS           "texttrace.bus"
#define TLOG_DEF_CATEGORY_BILL          "bill"

/**从配置文件初始化日志句柄
 *
 * @param[in] a_pstzPath 日志配置文件路径
 * @return  NULL 失败
 *			非NULL 获取的日志上下问指针
 */
LPTLOGCTX tlog_init_from_file(const char *a_pstzPath);

/*
 * 初始化默认只有一个文件category的日志结构
 * @param[in] a_pszCatName 日志空间名
 * @param[in] iPriorityLow 日志空间的低优先级，参考日志优先级文档
 * @param[in] a_pszFilePattern 日志文件pattern
 * @param[in] iMaxRotate 日志文件最大滚动数0 无限制，1一个文件 >1 多个文件
 * @param[in] iSizeLimit 单个日志文件大小
 * @param[in] iRotateStick 是否写当前文件
 * @return NULL 失败
 *         非NULL成功获取日志上下文指针
 * @note   必须调用tlog_fini_ctx释放接口
 * */
LPTLOGCTX tlog_init_file_ctx(const char *a_pszCatName , int iPriorityLow ,const char *a_pszFilePattern, int iMaxRotate, size_t iSizeLimit, int iRotateStick);

/*
 * 设置日志的全局优先级
 * @param[in] pstCtx 日志上下文
 * @param[in] iPriorityHigh 日志高优先级,如果为0表示不设置
 * @param[in] iPriorityLow 日志低优先级，如果为0表示不设置
 * @return 0 成功
 *          非0 失败
 * */
int tlog_set_ctx_priority(LPTLOGCTX pstCtx, int iPriorityHigh, int iPriorityLow);

/**释放日志配置文件句柄
 *
 * @param[in] a_pstzPath 日志配置文件路径
 * @return  NULL 失败
 *			非NULL 获取的日志上下问指针
 */
int tlog_fini_ctx(LPTLOGCTX *a_ppstCtx);

#ifdef __cplusplus
}
#endif

#endif /* TLOGLOAD_H */

