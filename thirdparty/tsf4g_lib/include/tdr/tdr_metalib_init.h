/**
*
* @file     tdr_metalib_init.h
* @brief    元数据库资源管理
* : 初始化，释放资源,获取资源大小
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


#ifndef TDR_XML_METALIB_INIT_H
#define TDR_XML_METALIB_INIT_H


#include "tdr_types.h"
#include "tdr_define.h"
#include "tdr/tdr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_METALIB_MNG TDR_元数据库(metalib)管理
 *  @{
 */


/**
 *根据参数初始化元数据库
  *在处理过程中，如果发现语法错误、语义错误等问题，会用fpError这个文件句柄来输出一些详细的信息
 *@param[out]	pstLib 根据参数初始化的元数据库
 *@param[in]	pstParam 初始化元数据库的参数体
 *
 *@retval <0  处理失败，返回表示出错信息的错误代码
 *@retval 0   处理成功
 *
 *@note 元数据库TDRMETALIB的空间在函数内分配，需调用tdr_free_lib释放空间
 *
 * @pre \e pstLib 不能为 NULL
 * @pre \e pstParam 不能为 NULL
 * @pre \e pstParam中指定的宏元素和自定义类型元素的数目必须大于0；
 *pstParam->iMetaSize必须大于0，即自定义类型元素的空间必须大于0
 * @see	tdr_free_lib
 * @see	LPTDRLIBPARAM
 */
TDR_API int tdr_init_lib(OUT LPTDRMETALIB pstLib, IN const LPTDRLIBPARAM pstParam);

/**
 *释放元数据库的资源，并将指向元数库的指针置为NULL。
 */
TDR_API void tdr_free_lib(INOUT LPTDRMETALIB* ppstLib);


/**
*将元数据描述库复制到指定缓冲区中
*@param[out] a_pvDst 保存复制元数据描述库的缓冲区地址
*@param[in,out] a_piBuff 传入缓冲区的大小， 输出复制后元数据库的大小
*@param[in] a_pstSrcLib 待复制元数据库的指针
*
*@return 复制成功返回复制元数据库的指针，否则返回NULL
*@note 如果指定缓冲区的大小比实际需要的要小,则复制会失败
* @pre \e a_pvDst 不能为 NULL
* @pre \e a_piBuff 不能为NULL，且其所指缓冲区的大小必须大于0
* @pre \e a_pstSrcLib 不能为NULL
*/
TDR_API LPTDRMETALIB tdr_clone_lib(OUT void* a_pvDst, INOUT int* a_piBuff, IN LPTDRMETALIB a_pstSrcLib);


/**
*获取元数库所占内存字节数
*@return success:元数库所占内存字节数 failed: 0
* @pre \e ppstLib 不能为 NULL
*/
TDR_API size_t tdr_size(IN LPTDRMETALIB pstLib);

/**获取元数据描述库的名字
*@param[in] a_pstLib 元数据描述库的指针
*
*@return success:元数据描述库的名字字符串的指针  failed: ""
*
* @pre \e a_pstLib 不能为 NULL
*/
TDR_API const char *tdr_get_metalib_name(IN LPTDRMETALIB a_pstLib);

/**获取元数据描述库的版本
*@param[in] a_pstLib 元数据描述库的指针
*
*@return success: 元数据描述库的版本 failed: -1
*
* @pre \e a_pstLib 不能为 NULL
*/
TDR_API int tdr_get_metalib_version(IN LPTDRMETALIB a_pstLib);

/**获取元数据描述库的构建版本
*@param[in] a_pstLib 元数据描述库的指针
*
*@return cuccess: 元数据描述库的构建版本 failed : -1
*
* @pre \e a_pstLib 不能为 NULL
*/
TDR_API int tdr_get_metalib_build_version(IN LPTDRMETALIB a_pstLib);

/** @}*/

#ifdef __cplusplus
}
#endif /* __cplusplus */



#endif  /* TDR_XML_METALIB_INIT_H */

