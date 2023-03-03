/**
*
* @file     tdr_metalib_to_hpp.h 
* @brief    内部使用的宏定义
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



#ifndef TDR_C_HEADER_H
#define TDR_C_HEADER_H

#include <stdio.h>
#include "tdr/tdr_types.h"
#include "tdr/tdr_define.h"
#include "tdr/tdr_external.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_WORK_FOR_C TDR_C语言结合相关处理函数
*@{
*/



/**
 *根据特定元数据描述转换成c语言头文件结构
 *@param[in] a_fp: 保存数据声明的fpp文件句柄;
 *@param[in] a_pstMeta 要转换的meta元素
 *@param[in] a_pstRule 生成声明的规则信息指针
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 *@pre \e a_fp 不能为NULL
 *@pre \e a_pstMeta 不能为 NULL
 *@pre \e a_pstRule 不能为 NULL
 */ 
TDR_API int tdr_meta_to_hpp(IN FILE* a_fp, IN LPTDRMETA a_pstMeta,  IN LPTDRHPPRULE a_pstRule);

/**
 *将元数据库中的信息转换成c语言头文件结构
 *@param[in] a_pstLib: 元数据库指针
 *@param[in] a_pszHppFile 头文件的文件名
 *@param[in] a_pstRule 生成声明的规则信息指针
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 *@pre \e a_pszHppFile 不能为NULL
 *@pre \e a_pstLib 不能为 NULL
 *@pre \e a_pstRule 不能为 NULL
 */
TDR_API int tdr_metalib_to_hpp(IN LPTDRMETALIB a_pstLib, IN const char* a_pszHppFile, IN LPTDRHPPRULE a_pstRule);


/**
 *将特定XML文件转换成c语言头文件
 *将XML文件中描述的宏和union/struct出现在元数据库中的部分，生成c语言结构，而元数据库中没有定义的内容将
 *不生成声明
 *@param[in] a_pstLib: 元数据库指针
 *@param[in] a_pszXmlConf 定义了宏和自定义类型的XML文件的文件名
 *@param[in]	a_iTagSetVersion 指定元数据描述XML标签定义集的版本
 *@param[in] a_pszHppFile 头文件的文件名
 *@param[in] a_pstRule 生成声明的规则信息指针
 *@param[in] a_fpError 保存错误信息的文件句柄
 * @note XML信息中包含了"tagsetversion"则按其属性值作为标签集的版本，如果没有
 * 包含"tagsetversion"信息，则以参数a_iXMLTagVersion为准
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 *@pre \e a_pstLib 不能为 NULL
 *@pre \e a_pstRule 不能为 NULL
 *@pre \e a_pszXmlConf 不能为NULL
 *@pre \e a_pszHppFile 不能为NULL
 */
TDR_API int tdr_metalib_to_hpp_spec(IN LPTDRMETALIB a_pstLib, IN const char* a_pszXmlConf, IN int a_iTagSetVersion,
                        IN const char* a_pszHppFile, IN LPTDRHPPRULE a_pstRule, IN FILE* a_fpError);

/**
*将元数据库中的信息转换成.c文件
*本接口实现的基本思想是:
*	- 将元数据库的信息写到.c文件的一个全局静态无符号字符数组中；
*	- 保存元数据库的字符数据的命名规则是："g_szMetalib_" + 元数据库的名字。比如原始数据库的名字为"net", 
*		元数据库保存在"g_szMetalib_net"数组中；
*	- 当应用程序要使用元数据库时，只须将此.c文件连接到工程中，然后将此静态数组强制类型转换成LPTDRMETALIB即可使用
*@param[in] a_pstLib: 元数据库指针
*@param[in] a_pszCFile .c文件的文件名
*
*@retval <0  处理失败，返回表示出错信息的错误代码
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*
*@pre \e a_pstLib 不能为 NULL
*@pre \e a_pstRule 不能为 NULL
*/
TDR_API int tdr_metalib_to_cfile(IN LPTDRMETALIB a_pstLib, IN const char* a_pszCFile);


/** @}*/ /* defgroup TDR_WORK_FOR_C TDR_C语言结合相关处理函数*/

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* TDR_C_HEADER_H */
