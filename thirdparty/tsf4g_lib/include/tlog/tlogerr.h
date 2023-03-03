/*
**  @file $RCSfile: tlogerr.h,v $
**  general description of this module
**  $Id: tlogerr.h,v 1.1 2009-03-27 06:18:25 kent Exp $
**  @author $Author: kent $
**  @date $Date: 2009-03-27 06:18:25 $
**  @version $Revision: 1.1 $
**  @note Editor: Vim 6.1, Gcc 4.0.1, tab=4
**  @note Platform: Linux
*/

#ifndef TLOG_ERROR_H
#define TLOG_ERROR_H

#include "comm/tmodid.h"
#include "comm/terr.h"


/** @defgroup TLOG_ERR TLOG_错误处理
* @{
*@note TLOG错误处理的基本思想:
*  - 使用一个整数来存储错误代码信息，此整数值的含义如下:
*      -# <0 表示发生错误，其值为具体错误的代码
*      -# 0 表示成功
*      -# >0 表示成功，但发生了某些特殊事情
*  - 错误代码存储在4字节的整数中，由三部分组成:
*      -# 错误级别: 1个字节，具体为高位的第1个字节
*      -# 模块ID:  1字节，具体为高位的第2个字节
*      -# 错误号:  2字节，0-1023号表示通用错误，各模块定义错误从1024开始
* 
*/
	
#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */
	
	
#define TLOG_SUCCESS				0
#define TLOG_ERR_ERROR				-1
	
#define TLOG_ERR_IS_ERROR(error)		TERR_IS_ERROR(error)

#define TLOG_ERR_START_ERRNO		1024
#define TLOG_ERR_MAKE_ERROR(err)    TERR_MAKE_ERROR(TMODID_LOG, (TLOG_ERR_START_ERRNO + err))

		
		typedef enum
		{
			TLOG_ERROR_NONE = 0, 				 /**< 没有错误*/
			TLOG_ERROR_FILE_OPEN,	             /**< 日志文件打开出错 */
			TLOG_ERROR_NET_CONN,                 /**< 打开网络连接失败 */
			TLOG_ERROR_LOOP,                     /**< forward 关系有循环*/
			TLOG_ERROR_COUNT,                    /**< 错误代码结束标志*/ 
		}TLOGERROR;

		/**
		* 根据错误代码获取错误信息
		* @param[in] iErrorCode 错误代码
		*
		* @return  错误信息串的指针
		*/
		extern char const*	tlog_error_string( int iErrorCode);
	
		/** @} */ // TLOG_ERR TLOG错误处理
	
#ifdef __cplusplus
	}
#endif
	
	
	
	
#endif /* TLOG_ERROR_H */

