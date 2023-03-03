/**
*
* @file     tdr_data_sort.h  
* @brief    TDR数据排序
* 
* @author steve jackyai  
* @version 1.0
* @date 2007-05-18 
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/

#ifndef TDR_DATA_SROT_H
#define TDR_DATA_SROT_H


#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

	/** @defgroup TDR_DATA_SORT TDR_数据排序
	*  @{
	*/

	/**
	*按照指定版本根据元数据描述对数据中所有需要排序的数组进行排序
	*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
	*@param[in,out]	a_pstHost 存储本地结构数据的结构体指针
	*	- 输入	a_pstHost.pszBuff 指定保存本地结构数据的缓冲区起始地址
	*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
	*	- 输出  a_pstHost.iBuff  实际解码本地结构信息的总长度
	*@param[in] a_iVersion	要剪裁的版本号
	*
	*@note 如果a_pstMeta描述的union类型的结构,则不进行排序直接返回
	*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
	*@note 如果此元数据指定了排序关键字sortkey属性，则按排序关键字进行排序；否则根据元数据各成员定义顺序
	*依次比较各成员的大小进行排序:
	*	- 如果某成员为数组，且此成员没有指定排序方法，则认为此成员相等；否则将依次比较各数组成员，如果成员数组长度不等，
	*     则在数组前面元素都等的情况下，数组长度大的成员判为大
	*
	* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
	* @pre \e a_pstMeta 不能为 NULL
	* @pre \e a_pstHost 不能为 NULL
	* @pre \e a_pstHost.pszBuff 不能为 NULL
	* @pre \e a_pstHost.iBuff 必须大于0	
	*
	*@retval 0   处理成功
	*@retval <0  处理失败，返回表示出错信息的错误代码	
	*
	*
	* @see	tdr_get_meta_by_name
	* @see	tdr_get_meta_by_id
	*/
TDR_API int tdr_sort(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/**
*按照指定版本根据元数据描述对复合数据结构数组进行排序
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstHost 存储本地结构数据的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存结构数组数据的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*	- 输出  a_pstHost.iBuff  实际解码本地结构信息的总长度
*@param[in] a_iCount	指定待排序的结构数据的个数
*@param[in] a_enSortMethod	排序方法
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 如果a_pstMeta描述的union类型的结构,则不进行排序直接返回
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 如果此元数据指定了排序关键字sortkey属性，则按排序关键字进行排序；否则根据元数据各成员定义顺序
*依次比较各成员的大小进行排序:
*	- 如果某成员为数组，且此成员没有指定排序方法，则认为此成员相等；否则将依次比较各数组成员，如果成员数组长度不等，
*     则在数组前面元素都等的情况下，数组长度大的成员判为大
*
* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码	
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_sort_metas(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iCount, IN TDRSORTMETHOD a_enSortMethod, IN int a_iVersion);


/**
*对数据结构中的指定数组成员进行指定类型的排序
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_pstHost 存储本地结构数据的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构数据的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*@param[in] a_iSortType 指定排序方法,有效取值为0,1,2,分别表示"不排序", "按升序排序","按降序排序"三中排序方法
*@param[in] a_pszSortEntry 指定要排序的数据成员,其取值为该成员在元数据描述中的路径Path
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 如果此元数据指定了排序关键字sortkey属性，则按排序关键字进行排序；否则根据元数据各成员定义顺序
*依次比较各成员的大小进行排序:
*	- 如果某成员为数组，且此成员没有指定排序方法，则认为此成员相等；否则将依次比较各数组成员，如果成员数组长度不等，
*     则在数组前面元素都等的情况下，数组长度大的成员判为大
*
* @pre \e a_pstHost.pszBuff缓冲区中必须是按照a_pstMeta描述的数据
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstHost 不能为 NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0	
* @pre \e a_pszSortEntry 不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码:
*	-	TDR_ERROR_INVALID_CUTOFF_VERSION 指定版本比元数据基准版本小
*	-	TDR_ERROR_ENTRY_INVALID_SORTMETHOD_VALUE	排序方法不正确
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_sort_spec(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iSortType, IN const char* a_pszSortEntry, IN int a_iVersion);

/** @}*/ /* TDR_DATA_SORT TDR_数据排序*/

/** @defgroup TDR_DATA_NORMALIZE TDR_数据正则化
*  @{
*/

/** 
*@brief 依据元数据定义，检查数据的内容是否符合元数据中定义的约束。
*这些约束主要包括：数组大小、值域约束、键约束和排序等。如果数据没有排序，即进行排序，如果不符合数组大小、值域约束、键约束
*则直接报错
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
TDR_API int tdr_normalize(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/** @}*/  /* TDR_DATA_NORMALIZE TDR_数据正则化*/

#ifdef __cplusplus
}
#endif

#endif /* TDR_DATA_SROT_H */
