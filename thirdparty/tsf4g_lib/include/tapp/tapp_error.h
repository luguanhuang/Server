/**
 *
 * @file        tapp_error.h
 * @brief       TAPP返回值处理头文件
 *
 * @author      flyma
 * @version     1.0
 * @date        2011-12-5
 *
 * Copyright (c)  2010-2020, 腾讯科技有限公司互动娱乐研发部
 * All rights reserved.
 *
 */

#ifndef TAPP_ERROR_H
#define TAPP_ERROR_H


#include "comm/tmodid.h"
#include "pal/pal.h"


/** @defgroup TAPP_ERR TAPP_错误处理
 * @{
 *
 *@note TAPP-RETURN-VALUE:
 *  - 使用一个整数来存储返回值，此整数值的含义如下:
 *      -#  0 表示成功, 此时 TAPP_RET_IS_ERR(ret) == 0
 *      -# <0 表示错误, 此时 TAPP_RET_IS_ERR(ret) == 1
 *  - 返回值表示错误时，根据如下结构解释返回值：
 *      -# 错误级别: 1字节，对应返回值的第1个字节
 *      -# 模块ID:   1字节，对应返回值的第2个字节, 参考下文"模块ID"
 *      -# 错误号:   2字节，起始值为 1024 ([0,1023] 为通用错误预留)
 *  - 模块ID (MOD-ID):
 *      一个MOD-ID占用一个字节(8bit)，对应返回值的第2个字节，
 *      为了后期可以取得子模块的 MOD-ID，从而取得子模块的错误信息，
 *      TAPP的 MOD-ID 使用 MOD-ID 对应的字节的第一bit，其余给子模块使用，
 *      例如：TMODID_TAPP | TAPP_SUB_MOD_DR 中既有TAPP模块ID也有子模块(TDR)的ID，
 *  - 返回表示错误时，假设返回值保存在变量 ret 中, 可以通过如下方式取得错误信息：
 *      - 调用函数 tapp_error_string 取得错误描述信息
 *      - 使用宏 TAPP_ERR_GET_ERRNO(ret) 取得 TAPP_ERRNO_* 格式的错误码
 *      - 使用宏 TAPP_RET_MATCH(ret, TAPP_ERROR_*) 判断是否发生了特定的错误
 *
 */


#ifdef __cplusplus
extern "C" {
#endif


#define TAPP_ERROR         -1
#define TAPP_OK             0
#define TAPP_SUCCESS        0

    /**
     * 这里定义的子模块ID值完全对应干文件comm/tmodid.h中定义的值，
     * 只是因为文件tmodid.h中没有(实际上也是无法)定义BUS的模块ID，
     * 所以在这里重新完全定义一套子模块ID用于区分各个模块的错误码，
     */
#define TAPP_SUB_MOD_NON            0x00
#define TAPP_SUB_MOD_DR			    0x01
#define TAPP_SUB_MOD_LOG            0x02
#define TAPP_SUB_MOD_BUS            0x03
#define TAPP_SUB_MOD_MNG			0x04
#define TAPP_SUB_MOD_SEC			0x05


    /**
     * 根据 TAPP-API 的返回值判断是否有错误发生
     */
#define TAPP_RET_IS_ERR(ret)                ((ret) & 0x80000000 )


    /**
     * 判断 TAPP-API 的返回值中是否携带相应的错误信息
     */
#define TAPP_RET_MATCH(ret, error)          (((ret) & 0xFF80FFFF) == ((error) & 0xFF80FFFF))


    /*
     * 下述定义中的 MOD_ID 是 TAPP_SUB_MOD_*
     */
#define TAPP_RET_SET_MOD_ID(ret, modid)     ((ret) | (((modid) << 16) & 0x007F0000))
#define TAPP_RET_GET_MOD_ID(ret)            (((ret) & 0x007F0000) >> 16)
#define TAPP_RET_CLR_MOD_ID(ret)            ((ret) & 0xFF80FFFF)


    /*
     * 以下宏用于生成携带错误信息的返回值
     */
#define TAPP_ERR_LEVEL_WARN                 0x01
#define TAPP_ERR_LEVEL_ERROR                0x02
#define TAPP_ERR_LEVEL_FATAL                0x03

#define TAPP_ERR_START_ERRNO                1024
    /**
     * 从返回值取出 TAPP_ERRNO_* 形式的错误码
     */
#define TAPP_ERR_GET_ERRNO(error)           (((error) & 0xFFFF) - TAPP_ERR_START_ERRNO)

#define TAPP_ERR_MAKE(level, mod, errno)    (0x80000000 | ((level)<<24) | ((mod)<<16) | (errno))

#define TAPP_ERR_MAKE_WARN(errno)           TAPP_ERR_MAKE(TAPP_ERR_LEVEL_WARN,   TMODID_TAPP, errno)
#define TAPP_ERR_MAKE_ERROR(errno)          TAPP_ERR_MAKE(TAPP_ERR_LEVEL_ERROR,  TMODID_TAPP, errno + TAPP_ERR_START_ERRNO)
#define TAPP_ERR_MAKE_FATAL(errno)          TAPP_ERR_MAKE(TAPP_ERR_LEVEL_FATAL,  TMODID_TAPP, errno)


    /**
     * TAPP 错误代码定义
     * 可以通过 TAPP_ERR_GET_ERRNO 从 TAPP-API 返回值中提取
     *
     * 使用示例如下:
     *
     *
     * if (TAPP_RET_IS_ERR(ret) && (TAPP_ERRNO_ARG == TAPP_ERR_GET_ERRNO(ret)))
     * {
     *      *** ***
     * }
     */
    typedef enum
    {
        TAPP_ERRNO_NONE = 0,                /**<no error*/
        TAPP_ERRNO_ARG_INVALID,             /**<invalid argument*/
        TAPP_ERRNO_BUF_FULL,                /**<send-buffer is full*/
        TAPP_ERRNO_BUF_EMPTY,               /**<no data in recv-buffer*/
        TAPP_ERRNO_COUNT                    /**<end of error number*/
    } TAPPERRNO;


    /**
     * TAPP-API 返回值携带的错误信息定义
     * 注意: 错误信息只能通过 TAPP_RET_MATCH 与返回值匹配,
     *
     * 使用示例如下:
     *
     * if (TAPP_RET_IS_ERR(ret) && TAPP_RET_MATCH(ret, TAPP_ERROR_INVALID_ARG))
     * {
     *      *** ***
     * }
     */
    typedef enum
    {
        TAPP_ERROR_INVALID_ARG  = TAPP_ERR_MAKE_ERROR(TAPP_ERRNO_ARG_INVALID),
        TAPP_ERROR_BUF_FULL     = TAPP_ERR_MAKE_ERROR(TAPP_ERRNO_BUF_FULL),
        TAPP_ERROR_BUF_EMPTY    = TAPP_ERR_MAKE_ERROR(TAPP_ERRNO_BUF_EMPTY),
    } TAPPERROR;


    /**
     * 根据错误代码获取错误信息
     * @param[in] iErrorCode 错误代码
     *
     * @return  错误信息串的指针
     */
    TSF4G_API const char *tapp_error_string(IN int iRetCode);


#ifdef __cplusplus
}
#endif


/** @} */


#endif /* TAPP_ERROR_H */
