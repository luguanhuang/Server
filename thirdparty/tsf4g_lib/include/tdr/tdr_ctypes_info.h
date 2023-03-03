/**
*
* @file     tdr_ctypes_info.h
* @brief    TDR支持的一些C语言以外的基本数据类型
*
* @author steve jackyai
* @version 1.0
* @date 2007-03-26
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TDR_CTYPES_H
#define TDR_CTYPES_H


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <time.h>
#if defined(_WIN32) || defined(_WIN64)
#include <winsock2.h>

#pragma comment(lib, "Ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#include "tdr/tdr_define.h"
#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"

/** @defgroup TDR_CTYPE_INFO TDR_基本数据类型处理
*  @{
*/

/** @name 基本数据类型ID
*  @{
*/
#define TDR_TYPE_UNKOWN         -1
#define TDR_TYPE_MIN		0x00
#define TDR_TYPE_UNION		TDR_TYPE_MIN
#define TDR_TYPE_STRUCT		0x01
#define	TDR_TYPE_COMPOSITE	TDR_TYPE_STRUCT
#define TDR_TYPE_CHAR		0x02
#define TDR_TYPE_UCHAR		0x03
#define TDR_TYPE_BYTE		0x04
#define TDR_TYPE_SMALLINT		0x05
#define TDR_TYPE_SHORT		0x05
#define TDR_TYPE_SMALLUINT		0x06
#define TDR_TYPE_USHORT		0x06
#define TDR_TYPE_INT		0x07
#define TDR_TYPE_UINT		0x08
#define TDR_TYPE_LONG		0x09
#define TDR_TYPE_ULONG		0x0a
#define TDR_TYPE_LONGLONG	0x0b
#define TDR_TYPE_ULONGLONG	0x0c
#define TDR_TYPE_DATE		0x0d
#define TDR_TYPE_TIME		0x0e
#define TDR_TYPE_DATETIME	0x0f
#define TDR_TYPE_MONEY		0x10
#define TDR_TYPE_FLOAT		0x11
#define TDR_TYPE_DOUBLE		0x12
#define TDR_TYPE_IP			0x13
#define TDR_TYPE_WCHAR		0x14
#define TDR_TYPE_STRING		0x15
#define TDR_TYPE_WSTRING	0x16
#define TDR_TYPE_VOID		0x17
#define TDR_TYPE_MAX		TDR_TYPE_VOID
/**   @}*/

/**
*将numbers-and-dots 格式的IP地址串转换成tdr自定义格式的IP地址数据类型
*@param[out]	a_piTDRIp 保存tdr定义的IP地址数据的指针
*@param[in]	a_pszIP 存储numbers-and-dots 格式的IP地址串的指针
*
*@retval 0   处理成功
*@retval  non-zero  处理失败
*
*@note 对于255.255.255.255地址，Linux能够成功转换，Windows则返回错误。一般情况下， 此地址无实际使用意义。
*
* @pre \e a_piTDRIp 不能为 NULL
* @pre \e a_pszIP 不能为 NULL
*
*/
TDR_API int tdr_ineta_to_tdrip(OUT tdr_ip_t *a_piTDRIp, IN const char *a_pszIP);

/**
*将tdr自定义格式的IP地址数据转换成struct in_addr结构数据
*@param[out]	a_pInp 输出struct in_addr结构的数据
*@param[in]	a_iTDRIp tdr自定义格式的IP地址数据
*
*@retval 0   处理成功
*@retval  non-zero  处理失败
*
* @pre \e a_pInp 不能为 NULL
*
*/
TDR_API int tdr_tdrip_to_inetaddr(OUT struct in_addr *a_pInp, IN tdr_ip_t a_iTDRIp);

/**
*将tdr自定义格式的IP地址数据转换成numbers-and-dots 格式的IP地址串
*@param[in]	a_iTDRIp tdr自定义格式的IP地址数据
*
*@return 如果处理成功,返回ip地址串首指针,否则返回NULL
*@note IP地址串保存在tdr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*/
TDR_API char *tdr_tdrip_to_ineta(IN tdr_ip_t a_iTDRIp);

/**
*将tdr自定义格式的IP地址数据转换成numbers-and-dots 格式的IP地址串
*@param[in]	a_iTDRIp tdr自定义格式的IP地址数据
*@param[in] a_pszDstBuff	保存IP地址信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回a_pszDstBuff的指针,否则返回NULL

*
*/
TDR_API const char *tdr_tdrip_ntop(IN tdr_ip_t a_iTDRIp, IN char *a_pszDstBuff, IN int a_iBuffSize);
/**
*将'YYYY-MM-DD' 格式的date字符串转换成tdr自定义格式的日期数据
*@param[out]	a_piTdrDate 保存tdr定义的日期数据的指针
*@param[in]	a_pszDate 日期字符串
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDate 不能为NULL
*@pre \e a_pszDate 不能为NULL
*/
TDR_API int tdr_str_to_tdrdate(OUT tdr_date_t *a_piTdrDate, IN const char *a_pszDate);

/**将tdr自定义格式的日期数据转换成'YYYY-MM-DD' 格式的date字符串
*@param[in]	a_piTdrDate 保存tdr定义的日期数据的指针
*
*@return 如果处理成功,返回日期字符串的指针,否则返回空串
*@note 返回的日期字符串保存在tdr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDate 不能为NULL
*
*/
TDR_API char *tdr_tdrdate_to_str(IN const tdr_date_t *a_piTdrDate);

/**将tdr自定义格式的日期数据转换成'YYYY-MM-DD' 格式的date字符串 本接口是线程安全版本
*@param[in]	a_piTdrDate 保存tdr定义的日期数据的指针
*@param[in] a_pszDstBuf	保存时间信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回日期字符串的指针,否则返回空串
*
*@pre \e a_piTdrDate 不能为NULL
*
*/
TDR_API const char *tdr_tdrdate_to_str_r(IN const tdr_date_t *a_piTdrDate,
										 IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*将'HHH:MM:SS' 格式的time字符串转换成tdr自定义格式的时间数据
*@param[out]	a_piTdrTime 保存tdr定义的时间time数据
*@param[in]	a_pszTime 时间字符串
*
*@note 支持3个字符来表示小时值的目的: time数据类型不仅可以用于表示一天的时间(这一定不会超过 24 小时)，而且可以用来表示所经过的时间或两个事件之间的时间间隔(这可能比 24 小时大许多或是一个负值)
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrTime 不能为NULL
*@pre \e a_pszTime 不能为NULL
*/
TDR_API int tdr_str_to_tdrtime(OUT tdr_time_t *a_piTdrTime, IN const char *a_pszTime);

/**
*将tdr自定义格式的时间数据转换成'HHH:MM:SS' 格式的time字符串
*@param[in]	a_piTdrTime 保存tdr定义的时间数据的指针
*
*@return 如果处理成功,返回时间字符串的指针,否则返回空串
*@note 返回的时间字符串保存在tdr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrTime 不能为NULL
*
*/
TDR_API char *tdr_tdrtime_to_str(IN tdr_time_t *a_piTdrTime);

/**
*将tdr自定义格式的时间数据转换成'HHH:MM:SS' 格式的time字符串.线程安全版本
*@param[in]	a_piTdrTime 保存tdr定义的时间数据的指针
*@param[in] a_pszDstBuf	保存时间信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回时间字符串的指针,否则返回空串
*@note 返回的时间字符串保存在tdr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrTime 不能为NULL
*
*/
TDR_API char *tdr_tdrtime_to_str_r(IN tdr_time_t *a_piTdrTime,
								   IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*将'YYYY-MM-DD HHH:MM:SS' 格式的日期/时间字符串转换成tdr自定义格式的日期时间数据
*@param[out]	a_piTdrDateTime 保存tdr定义的时间time数据
*@param[in]	a_pszDateTime 时间字符串
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDateTime 不能为NULL
*@pre \e a_pszDateTime 不能为NULL
*/
TDR_API int tdr_str_to_tdrdatetime(OUT tdr_datetime_t *a_piTdrDateTime, IN const char *a_pszDateTime);

/**
*convert the number of seconds elapsed since midnight (00:00:00), January 1, 1970, Coordinated Universal Time (UTC)  to tdr_datetime_t
*@param[out]	a_piTdrDateTime 保存tdr定义的时间time数据
*@param[in]	a_tTimer  时间
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDateTime 不能为NULL
*/
TDR_API int tdr_utctime_to_tdrdatetime(OUT tdr_datetime_t *a_piTdrDateTime, IN time_t a_tTimer);

/**
*convert tdr_datetime_t to the number of seconds elapsed since midnight (00:00:00), January 1, 1970, Coordinated Universal Time (UTC)
*@param[out]	a_ptTimer  保存时间的指针
*@param[in]	a_iTdrDateTime 保存tdr定义的时间time数据
*
*@return  成功返回0, 否则返回非0值
*
*@pre \e a_piTdrDateTime 不能为NULL
*/
TDR_API int tdr_tdrdatetime_to_utctime(OUT  time_t *a_ptTimer, IN tdr_datetime_t a_iTdrDateTime);


/**
*将tdr自定义格式的日期/时间数据转换成'YYYY-MM-DD HHH:MM:SS' 格式的日期/时间字符串
*@param[in]	a_piTdrDateTime 保存tdr定义的日期/时间数据的指针
*
*@return 如果处理成功,返回日期/时间字符串的指针,否则返回空串
*@note 返回的日期/时间字符串保存在tdr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDateTime 不能为NULL
*/
TDR_API char *tdr_tdrdatetime_to_str(IN const tdr_datetime_t *a_piTdrDateTime);

/**
*将tdr自定义格式的日期/时间数据转换成'YYYY-MM-DD HHH:MM:SS' 格式的日期/时间字符串.线程安全版本
*@param[in]	a_piTdrDateTime 保存tdr定义的日期/时间数据的指针
*@param[in] a_pszDstBuf	保存时间信息的字符串缓冲区
*@param[in] a_iBuffSize 缓冲区的可用大小
*
*@return 如果处理成功,返回日期/时间字符串的指针,否则返回空串
*@note 返回的日期/时间字符串保存在tdr管理的静态缓冲区,当此函数下次调用时,其内容会被覆盖
*
*@pre \e a_piTdrDateTime 不能为NULL
*/
TDR_API char *tdr_tdrdatetime_to_str_r(IN const tdr_datetime_t *a_piTdrDateTime,
									   IN char *a_pszDstBuf, IN int a_iBuffSize);


/**
*比较两个日期/时间值的大小
*@param[in]	a_piTdrDateTime1 保存tdr定义的日期/时间数据的指针
*@param[in]	a_piTdrDateTime2 保存tdr定义的日期/时间数据的指针
*
*@retval >0 a_piTdrDateTime1比a_piTdrDateTime2大,即a_piTdrDateTime1时间晚于a_piTdrDateTime2
*@retval 0  时间相同
*@retval <0 a_piTdrDateTime1比a_piTdrDateTime2小,即a_piTdrDateTime1时间早于a_piTdrDateTime2
*
*@pre \e a_piTdrDateTime1 不能为NULL
*@pre \e a_piTdrDateTime2 不能为NULL
*/
TDR_API int tdr_compare_datetime(IN const tdr_datetime_t *a_piTdrDateTime1, IN const tdr_datetime_t *a_piTdrDateTime2);

/**比较两个日期值的大小
*@param[in]	a_piTdrDate1 保存tdr定义的日期数据的指针
*@param[in]	a_piTdrDate2 保存tdr定义的日期数据的指针
*
*@retval >0 a_piTdrDate1比a_piTdrDate2大,即a_piTdrDate1时间晚于a_piTdrDate2
*@retval 0  时间相同
*@retval <0 a_piTdrDate1比a_piTdrDate2小,即a_piTdrDate1时间早于a_piTdrDate2
*
*@pre \e a_piTdrDate1 不能为NULL
*@pre \e a_piTdrDate2 不能为NULL
*/
TDR_API int tdr_compare_date(IN const tdr_date_t *a_piTdrDate1, IN const tdr_date_t *a_piTdrDate2);

/**
*比较两个时间值的大小
*@param[in]	a_piTdrTime1 保存tdr定义的时间数据的指针
*@param[in]	a_piTdrTime2 保存tdr定义的时间数据的指针
*
*@retval >0 a_piTdrTime1比a_piTdrTime2大,即a_piTdrTime1时间晚于a_piTdrTime2
*@retval 0  时间相同
*@retval <0 a_piTdrTime1比a_piTdrTime2小,即a_piTdrTime1时间早于a_piTdrTime2
*
*@pre \e a_piTdrTime1 不能为NULL
*@pre \e a_piTdrTime2 不能为NULL
*/
TDR_API int tdr_compare_time(IN const tdr_time_t *a_piTdrTime1, IN const tdr_time_t *a_piTdrTime2);

/**  @}*/
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /*TDR_CTYPES_H*/
