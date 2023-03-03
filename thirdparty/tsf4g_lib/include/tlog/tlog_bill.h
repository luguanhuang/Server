/**
*
* @file    tlog_bill.h  
* @brief   账单日志模块
* 
* @author mickeyxu
* @version 1.0
* @date 2007-04-05 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TLOG_BILL_H
#define TLOG_BILL_H

#include "pal/ttypes.h"

struct tagTLogFile;
typedef struct tagTLogFile		* LPTLOGBILL;

#ifdef __cplusplus
	extern "C" {
#endif /* __cplusplus */
/** @defgroup TLOG_BILL TLOG_账单日志
* @{*/

/**
 * @brief Init a bill loger
 * @param [in,out] ppstTlogBill bill handler addr, Passed for creating bill
 * @param [in] iSizeLimit The max size of a bill file
 * @param [in] iPrecision The sync time for the file ,Used when the fileName has time patterns.
 * @param [in] szName File name for logging, can be used with strftime.
 * @retval 0 successful
 * @retval !0 failed
 */
int tlog_bill_init(LPTLOGBILL *ppstTlogBill,size_t iSizeLimit,int iPrecision,const char *szName);

/**
 * @brief Write one line of bill into the bill logger. And see whether it has succeeded.
 * @param [in] pstTlogBill bill handler addr, Passed for creating bill
 * @param [in] a_pszFmt The formate string
 * @param [in] ... vaparms
 * @retval 0 successful
 * @retval !0 failed
 * @alert You should check the return value
 */
int tlog_bill_log(LPTLOGBILL pstTlogBill, const char* a_pszFmt, ...);

/**
 * @brief write buffer to the bill handler.
 * @param [in] pstTlogBill bill handler addr, Passed for creating bill
 * @param [in] pszBuff the buffer used for writting.
 * @param [in] iBuff the size of buffer to write.
 * @retval 0 successful
 * @retval !0 failed
 * @alert You should check the return value
 */
int tlog_bill_write(LPTLOGBILL pstTlogBill,char *pszBuff,size_t iBuff);
/**
 * @brief destruct a bill logger
 * @param [in] ppstTlogBill bill handler addr, Passed for creating bill
 * @retval 0 successful
 * @retval !0 failed
 */
int tlog_bill_fini(LPTLOGBILL *ppstTlogBill);

#ifdef __cplusplus
	}
#endif
	

/** @} */ 
#endif
