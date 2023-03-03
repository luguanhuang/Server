/**
*
* @file     tdr_XMLMetaLib.h
* @brief    元数据描述库与XML描述相互转换
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


#ifndef TDR_XML_METALIB_H
#define TDR_XML_METALIB_H


#include <stdio.h>
#include "tdr_types.h"
#include "tdr_define.h"
#include "tdr/tdr_external.h"

/** @defgroup TDR_XMLMETALIB   TDR_元数据描述库与XML描述相互转换
* @{
*/ 

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */





/**
 *根据XML信息缓冲区中的信息生成元数据描述库
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[in,out]	a_ppstLib 根据XML文件生成的元数据库
 *@param[in]	a_pszXml 保存XML信息的缓冲区地址
 *@param[in]	a_iXml 保存XML信息的缓冲区大小
 *@param[in]	a_iTagSetVersion 指定元数据描述XML标签定义集的版本
 *@param[in]	a_fpError	用来输出错误信息的文件句柄
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 *@note 元数据描述库的空间在函数内分配，需调用tdr_free_lib释放空间
 * @note 若XML信息中包含了"tagsetversion"属性则按其属性值作为元数据描述标签集的版本，否则以参数a_iXMLTagVersion为准
 * @note 元数据描述的XML元素树的根元素必须包含 name属性，以确定元数据描述的名字空间
 *
 * @pre \e a_ppstLib 不能为 NULL
 * @pre \e a_pszXml 不能为 NULL
 * @pre \e a_iXml    必须大于0
 * @see	tdr_free_lib
 */
TDR_API int  tdr_create_lib(INOUT LPTDRMETALIB *a_ppstLib, IN const char* a_pszXml, IN int a_iXml, IN int a_iTagSetVersion, IN FILE* a_fpError);


/**
 *利用单个XML文件生成元数据库
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[in,out]	a_ppstLib 根据XML文件生成的元数据库
 *@param[in]	a_fpXML 保存XML信息的文件指针
 *@param[in]	a_iTagSetVersion 指定元数据描述XML标签定义集的版本
 *@param[in]	a_fpError	用来输出错误信息的文件句柄
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 * @note 元数据库TDRMETALIB的空间在函数内分配，需调用tdr_free_lib释放空间
 * @note XML信息中包含了"tagsetversion"则按其属性值作为标签集的版本，如果没有
 * 包含"tagsetversion"信息，则以参数a_iXMLTagVersion为准
 * @note 元数据描述的XML元素树的根元素必须包含 name属性，以确定元数据描述的名字空间
 *
 *@pre \e a_ppstLib 不能为NULL
 * @pre \e a_fpXML 不能为 NULL
 * @see	tdr_free_lib
 */
TDR_API int tdr_create_lib_fp(INOUT LPTDRMETALIB *a_ppstLib, IN FILE* a_fpXML, IN int a_iTagSetVersion, IN FILE* a_fpError);




/**
 *利用单个XML文件生成元数据库
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[in,out]	a_ppstLib 根据XML文件生成的元数据库
 *@param[in]	a_pszFile 保存XML信息的文件名
 *@param[in]	a_iXMLTagVersion 指定元数据描述XML标签定义集的版本
 *@param[in]	a_fpError	用来输出错误信息的文件句柄
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 * @note 元数据库TDRMETALIB的空间在函数内分配，需调用tdr_free_lib释放空间
* @note XML信息中包含了"tagsetversion"则按其属性值作为标签集的版本，如果没有
 * 包含"tagsetversion"信息，则以参数a_iXMLTagVersion为准
 * @note 元数据描述的XML元素树的根元素必须包含 name属性，以确定元数据描述的名字空间
 *
 * @pre \e a_ppstLib 不能为 NULL
 * @pre \e a_pszFile 不能为 NULL
 * @see	tdr_free_lib
 */
TDR_API int tdr_create_lib_file(INOUT LPTDRMETALIB *a_ppstLib, IN const char* a_pszFile, IN int a_iXMLTagVersion, IN FILE* a_fpError);


/**
 *利用多个XML文件生成元数据库
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[in,out]	a_ppstLib 根据XML文件生成的元数据库
 *@param[in]	a_aszFile 保存XML信息的文件名数组
 *@param[in] a_iFile xml文件个数
 *@param[in]	a_iXMLTagVersion 指定元数据描述XML标签定义集的版本
 *@param[in]	a_fpError	用来输出错误信息的文件句柄
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 *@note 元数据库TDRMETALIB的空间在函数内分配，需调用tdr_free_lib释放空间
* @note XML信息中包含了"tagsetversion"则按其属性值作为标签集的版本，如果没有
 * 包含"tagsetversion"信息，则以参数a_iXMLTagVersion为准
 * @note 不能将不同名字空间的元数据描述加到同一个元数据库中, 即各XML文件中根元素
 * 的name属性值必须相同
 * @note 元数据描述的XML元素树的根元素必须包含 name属性，以确定元数据描述的名字空间
 * @note 加到同一个库中的XML元数据描述文件根元素的ID属性值必须相同
 *
 * @pre \e a_ppstLib 不能为 NULL
 * @pre \e a_aszFile 不能为 NULL
 * @pre \e a_iFile    必须大于0，即至少指定一个文件
 * @see	tdr_free_lib
 */
TDR_API int tdr_create_lib_multifile(INOUT LPTDRMETALIB *a_ppstLib, IN const char* a_aszFile[], IN int a_iFile, IN int a_iXMLTagVersion, IN FILE* a_fpError);




/**
 *往已有的元数据Lib中添加新的元数据定义内容
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[in,out]	a_pstLib 保存元数据描述的库
 *@param[in]	a_pszXml 保存XML信息的缓冲区地址
 *@param[in]	a_iXml 保存XML信息的缓冲区大小
 *@param[in]	a_iXMLTagVersion 指定元数据描述XML标签定义集的版本
 *@param[in]	a_fpError	用来输出错误信息的文件句柄
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 * @note 元数据库TDRMETALIB的空间在函数内分配，需调用tdr_free_lib释放空间
 * @note XML信息中包含了"tagsetversion"则按其属性值作为标签集的版本，如果没有
 * 包含"tagsetversion"信息，则以参数a_iXMLTagVersion为准
 * @note 不能将不同名字空间的元数据描述加到同一个元数据库中, 即各XML文件中根元素
 * 的name属性值必须相同
 * @note 元数据描述的XML元素树的根元素必须包含 name属性，以确定元数据描述的名字空间
 * @note 加到同一个库中的XML元数据描述文件根元素的ID属性值必须相同
 *
 * @pre \e a_pstLib 不能为 NULL, 且必须是调用 tdr_init_lib 进行创建的，以保证
 * pstLib中留有足够的空间来添加新的定义
 * @pre \e a_pszXml 不能为 NULL
 * @pre \e a_iXml    必须大于0，
 * @see	tdr_init_lib
 */
TDR_API int tdr_add_meta(INOUT LPTDRMETALIB a_pstLib, IN const char* a_pszXml, IN int a_iXml, IN int a_iXMLTagVersion, IN FILE* a_fpError);


/**
 *往已有的元数据Lib中添加新的元数据定义内容
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[in,out]	a_pstLib 保存元数据描述的库
 *@param[in]	a_pszFileName 保存XML信息的文件
  *@param[in]	a_iXMLTagVersion 指定元数据描述XML标签定义集的版本
 *@param[in]	a_fpError	用来输出错误信息的文件句柄
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 * @note 元数据库TDRMETALIB的空间在函数内分配，需调用tdr_free_lib释放空间
 * @note XML信息中包含了"tagsetversion"则按其属性值作为标签集的版本，如果没有
 * 包含"tagsetversion"信息，则以参数a_iXMLTagVersion为准
 * @note 不能将不同名字空间的元数据描述加到同一个元数据库中, 即各XML文件中根元素
 * 的name属性值必须相同
 * @note 元数据描述的XML元素树的根元素必须包含 name属性，以确定元数据描述的名字空间
 * @note 加到同一个库中的XML元数据描述文件根元素的ID属性值必须相同
 *
 * @pre \e a_pstLib 不能为 NULL, 且必须是调用 tdr_init_lib 进行创建的，以保证
 * pstLib中留有足够的空间来添加新的定义
 * @pre \e a_pszFileName 不能为 NULL
 * @see	tdr_init_lib
 */
TDR_API int tdr_add_meta_file(INOUT LPTDRMETALIB a_pstLib, IN const char* a_pszFileName, IN int a_iXMLTagVersion, IN FILE* a_fpError);


/**
 *将元数据库转换成XML描述信息，并保存到内存缓冲区中。
 *@param[in] a_pstLib 根据XML信息生成的元数据库
 *@param[in]	a_pszXML 保存XML信息的缓冲区地址; 
 *@param[in,out]	a_iXML XML信息的大小
 *  -输入： pszXml所指缓冲区的大小；
 *  -输出:  生成的的XML信息的大小
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 *@pre \e a_pstLib 不能为NULL
 * @pre \e a_pszXML 不能为 NULL
 * @pre \e a_iXML 不能为 NULL,且其所指整数值必须大于零
 */
TDR_API int tdr_save_xml(IN LPTDRMETALIB a_pstLib, OUT char *a_pszXML, INOUT int *a_iXML);


/**
 *将元数据库转换成XML描述信息，并保存到文件中。
 *@param[in] a_pstLib 根据XML信息生成的元数据库
 *@param[out]	a_fp 保存XML信息的文件指针; 
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 * @pre \e a_pstLib 不能为 NULL
 * @pre \e a_fp 不能为 NULL
 */
TDR_API int tdr_save_xml_fp(IN LPTDRMETALIB a_pstLib, OUT FILE* a_fp);


/**
 *将元数据库转换成XML描述信息，并保存到文件中。
 *@param[in] a_pstLib 根据XML信息生成的元数据库
 *@param[in]	a_pszXmlFile 保存XML信息的文件的文件名; 
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *@retval >0  处理成功，但发生某些特殊情况
 *
 * @pre \e a_pstLib 不能为 NULL
 * @pre \e a_pszXmlFile 不能为 NULL
 *	
 */
TDR_API int tdr_save_xml_file(IN LPTDRMETALIB a_pstLib, IN const char* a_pszXmlFile);


#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @} */ //   元数据描述库与XML描述相互转换


#endif  /* TDR_XML_METALIB_H */
