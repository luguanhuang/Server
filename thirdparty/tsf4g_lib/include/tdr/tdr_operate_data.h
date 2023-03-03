/**
*
* @file     tdr_operate_data.h
* @brief    TDR数据初始化/正则化
*
* @author steve jackyai
* @version 1.0
* @date 2007-06-05
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/
#ifndef TDR_OPERATE_DATA
#define TDR_OPERATE_DATA


#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/** @defgroup TDR_OPERATE_DATA TDR_数据操作
*  @{
*/

/**
*@brief 数据初始化
*根据元数据的缺省值约束对内存结构的相应成员赋初始值
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_pstHost 存储本地结构数据的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构数据的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 如果a_pstMeta描述的union类型的结构,则不进行处理直接返回
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*
* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_init(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);


/**
*@brief 数据拷贝
*根据元数据描述将内存数据拷贝到指定内存中
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_pstHostDst 保存剪裁拷贝数据的缓冲区结构体指针
*	- 输入	a_pstHostDst.pszBuff 指定保存拷贝数据的缓冲区起始地址
*	- 输入  a_pstHostDst.iBuff	指定缓冲区的大小
*	- 输出	a_pstHostDst.iBuff  如果传入的长度大于结构体长度，修改为结构体长度；
                                如果小于结构体的长度，则不会修改该值
*@param[in]	a_pstHostSrc 存储源数据的内存缓冲区结构体指针
*	- 输入	a_pstHostSrc.pszBuff 指定保存源数据的缓冲区起始地址
*	- 输入  a_pstHostSrc.iBuff	指定缓冲区的大小
*
*@note 数据拷贝的源内存区与目的内存区不能重叠
*@note 如果a_pstMeta描述的union类型的结构,则不进行处理直接返回
*
* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHostDst 不能为 NULL
* @pre \e a_pstHostDst.pszBuff 不能为 NULL
* @pre \e a_pstHostDst.iBuff 必须大于0
* @pre \e a_pstHostSrc 不能为 NULL
* @pre \e a_pstHostSrc.pszBuff 不能为 NULL
* @pre \e a_pstHostSrc.iBuff 必须大于0
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_copy(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHostDst, IN LPTDRDATA a_pstHostSrc);

/**
*@brief 数据遍历
*根据元数据描述遍历整个数据
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in] a_pHandle 数据处理句柄
*@param[in]	a_pstHost 保存数据的缓冲区结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存拷贝数据的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in]	a_iVersion 数据的版本,如果其值为0,则按照当前最高版本处理
*@param[in] pfnEnter 碰到复合数据类型时的回调函数
*@param[in] pfnVistEntry 访问简单数据类型的回调函数
*
*@note 如果a_pstMeta描述的union类型的结构,则不进行处理直接返回
*
* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHostDst 不能为 NULL
* @pre \e a_pstHostDst.pszBuff 不能为 NULL
* @pre \e a_pstHostDst.iBuff 必须大于0
* @pre \e pfnEnter 不能为 NULL
* @pre \e pfnEnter  不能为 NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
* @see  PFN_ENTER_META
* @see  PFN_VISIT_SIMPLE_ENTRY
*/
TDR_API int tdr_browse(IN LPTDRMETA a_pstMeta, IN void *a_pHandle, IN LPTDRDATA a_pstHost, IN int a_iVersion,
					   IN PFN_ENTER_META pfnEnter, IN PFN_VISIT_SIMPLE_ENTRY pfnVistEntry);

/** @}*/ /* TDR_OPERATE_DATA TDR_数据操作*/


#ifdef __cplusplus
}
#endif

#endif /* TDR_OPERATE_DATA */
