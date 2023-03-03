/**
*
* @file     tdr_net.h
* @brief    TDR元数据网络交换消息编解码模块
*
* @author steve jackyai
* @version 1.0
* @date 2007-04-28
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/



#ifndef TSF_G_TDR_NET_H
#define TSF_G_TDR_NET_H


#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"


#ifdef TDR_OS_LITTLEENDIAN
#define tdr_ntoh64(x)                    TDR_OS_SWAP64(x)
#define tdr_hton64(x)                    TDR_OS_SWAP64(x)
#define tdr_ntoh32(x)                    TDR_OS_SWAP32(x)
#define tdr_hton32(x)                    TDR_OS_SWAP32(x)
#define tdr_ntoh16(x)                    TDR_OS_SWAP16(x)
#define tdr_hton16(x)                    TDR_OS_SWAP16(x)
#else
#define tdr_ntoh64(x)                    (x)
#define tdr_hton64(x)                    (x)
#define tdr_ntoh32(x)                   (x)
#define tdr_hton32(x)                   (x)
#define tdr_ntoh16(x)                   (x)
#define tdr_hton16(x)                   (x)
#endif



#define TDR_SET_INT_NET(p, iSize, i)		switch(iSize)		      \
{									      \
    case 2:								      \
    *(unsigned short*)(p)	=	tdr_hton16((unsigned short)(i));   \
    break;							      \
    case 4:								      \
    *(uint32_t*)(p)	=	tdr_hton32((uint32_t)(i));    \
    break;							      \
    case 8:									\
    *(uint64_t*)(p)	=	tdr_ntoh64((uint64_t)(i));    \
    break;								\
    default:							      \
    *(unsigned char*)(p)	=	(unsigned char)(i);	      \
}


#define TDR_GET_INT_NET(i, iSize, p)		switch(iSize)		      \
{									      \
    case 2:								      \
    i = (int)tdr_ntoh16(*(unsigned short*)(p));			      \
    break;							      \
    case 4:								      \
    i = (int)tdr_ntoh32(*(uint32_t*)(p));			      \
    break;							      \
    case 8:									\
    i = tdr_ntoh64(*(uint64_t*)p);	\
    break;												\
    default:							      \
    i = (int)*(unsigned char*)(p);				      \
}



#define TDR_GET_UINT_NET(i, iSize, p)		switch(iSize)		      \
{									      \
    case 2:								      \
    i = (uint32_t)tdr_ntoh16(*(unsigned short*)(p));			      \
    break;							      \
    case 4:								      \
    i = (uint32_t)tdr_ntoh32(*(uint32_t*)(p));			      \
    break;							      \
    case 8:									\
    i = tdr_ntoh64(*(uint64_t*)p);	\
    break;												\
    default:							      \
    i = (uint32_t)*(unsigned char*)(p);				      \
}




#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_NETMSG_PACKUNPACK TDR_网络交换消息编/解码
* @{
*/

/**
	*按照指定版本根据元数据描述将本地存储数据转换成网络交换消息
	*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
	*@param[in,out]	a_pstNet 存储网络信息结构体指针
	*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
	*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
	*	- 输出  a_pstNet.iBuff  实际编码网络信息的总长度
	*@param[in,out]	a_pstHost 存储本地结构信息的结构体指针
	*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
	*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
	*	- 输出  a_pstHost.iBuff  如果传入的长度大于结构体长度，修改为结构体长度；
                                 如果小于结构体的长度，则不会修改该值
	*@param[in] a_iVersion	剪裁的版本号，即要编码到网络信息块中的结构体版本，
	*
	*@note a_iVersion必须不小于本地结构体描述的基础版本(tdr_get_meta_based_version)，如果a_iVersion为0 ，则安装剪裁到结构当前最高版本，
	*@note 如果处理失败，获取错误信息的方法:
	*	- 1.根据返回值，调用tdr_error_string()可以获取出错信息
	*	- 2.由于a_pstHost.iBuff得到了实际编码的信息的总长度，通过a_pstHost中记录的信息，可以调用
	*		tdr_fprintf/tdr_sprintf 打印出已经成功编码的结构信息，通常如果tdr_hton调用失败，通过
	*		此手段可以定位编码出错的具体位置。
	*
	* @pre \e a_pstMeta 不能为 NULL，并且必须对应 xml 描述文件中 struct 元素。
	* @pre \e a_pstNet 不能为 NULL
	* @pre \e a_pstNet.pszBuff 不能为 NULL
	* @pre \e a_pstNet.iBuff 必须大于0
	* @pre \e a_pstHost 不能为 NULL
	* @pre \e a_pstHost.pszBuff 不能为 NULL
	* @pre \e a_pstHost.iBuff 必须大于0
    * @pre \e
	*
	*@retval 0   处理成功
	*@retval >0  处理成功，但发生某些特殊情况
	*@retval <0  处理失败，返回表示出错信息的错误代码：
	*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
	*		- TDR_ERR_NET_NO_NETBUFF_SPACE 保存网络信息的缓冲区剩余空间不够
	*		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
	*		- TDR_ERROR_INVALID_REFER_VALUE		此元素的refer属性值不正确，其值不能为负数且必须比count属性值小
	*		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
	*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
	*		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
	*		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR 元数据版本指示器的值比保存其值的元数据成员的版本要小
	*
    * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
	*
	* @see	tdr_get_meta_by_name
	* @see	tdr_get_meta_by_id
	* @see	tdr_error_string
	* @see	tdr_entry_off_to_path
	* @see  tdr_get_meta_based_version
    * @see  tdr_get_error_detail
	*/
TDR_API int tdr_hton(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstNet, INOUT LPTDRDATA a_pstHost, IN int a_iVersion);

/**
*按照指定版本根据元数据描述将网络交换消息转换成本地存储数据
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstHost 存储本地结构信息的结构体指针
*	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstHost.iBuff	指定缓冲区的大小
*	- 输出  a_pstHost.iBuff   如果传入的长度大于结构体长度，修改为结构体长度；
                              如果小于结构体的长度，则不会修改该值
*@param[in,out]	a_pstNet 存储网络信息结构体指针
*	- 输入	a_pstNet.pszBuff 指定保存网络信息的缓冲区起始地址
*	- 输入  a_pstNet.iBuff	指定缓冲区的大小
*	- 输出  a_pstNet.iBuff  实际解码网络信息的总长度
*@param[in] a_iVersion	指定编码在网络信息块中结构体的版本号，
*
*@note a_iVersion必须不小于本地结构体描述的基础版本(tdr_get_meta_based_version)，且不高于本地结构体描述的最高版本(tdr_get_meta_current_version)。
*	如果a_iVersion为0 ，则按照结构体当前最高版本处理
*@note 如果处理失败，获取错误信息的方法:
	*	- 根据返回值，调用tdr_error_string()可以获取出错信息
	*	- 由于a_pstHost.iBuff得到了实际解码的信息的总长度，以这个长度为偏移值，调用tdr_entry_off_to_path可以获取
	*		出错时成员的路径信息，从而定位解码具体在哪个位置出错
	*	- 调用tdr_fprintf/tdr_sprintf可以打印出已经解码的内容
*
* @pre \e a_pstMeta 不能为 NULL，并且必须对应 xml 描述文件中 struct 元素。
* @pre \e a_pstNet 不能为 NULL
* @pre \e a_pstNet.pszBuff 不能为 NULL
* @pre \e a_pstNet.iBuff 必须大于0
* @pre \e a_pstHost 不能为NULL
* @pre \e a_pstHost.pszBuff 不能为 NULL
* @pre \e a_pstHost.iBuff 必须大于0
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小,或高于元数据当前最高版本
*		- TDR_ERR_NET_NO_NETBUFF_SPACE 保存网络信息的缓冲区剩余空间不够
*		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
*		- TDR_ERROR_INVALID_REFER_VALUE		此元素的refer属性值不正确，其值不能为负数且必须比count属性值小
*		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
*		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
*		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
*
* @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
* @see	tdr_error_string
* @see	tdr_entry_off_to_path
* @see  tdr_fprintf
* @see  tdr_get_meta_current_version
* @see  tdr_get_meta_based_version
* @see  tdr_get_error_detail
*/
TDR_API int tdr_ntoh(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, INOUT LPTDRDATA a_pstNet, IN int a_iVersion);

/** @} */ /*@defgroup TDR_网络交换消息编/解码*/



/**
 *获取元数据对应结构体在经过tdr_hton打包后的最大可能网络空间大小
 *@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
 *@param[in]    a_iVersion	指定元数据的剪裁版本
 *@param[out]   a_ptMaxNetSize 返回最大可能的网络空间大小
 *
 *@note 如果处理失败，获取错误信息的方法:
 *	- 根据返回值，调用tdr_error_string()可以获取出错信息
 *
 *@pre \e a_pstMeta 不能为 NULL，并且必须对应 xml 描述文件中 struct 元素。
 *@pre \e a_piMaxNetSize 不能为 NULL
 *
 *@retval >0  处理成功，但发生某些特殊情况
 *@retval <0  处理失败，返回表示出错信息的错误代码：
 *		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
 *
 *
 * @see	tdr_get_meta_by_name
 * @see	tdr_get_meta_by_id
 * @see	tdr_error_string
 * @see  tdr_fprintf
 * @see  tdr_get_meta_current_version
 * @see  tdr_get_meta_based_version
 */
TDR_API int tdr_get_meta_max_net_size(IN LPTDRMETA a_pstMeta, IN int a_iVersion, OUT size_t* a_ptMaxNetSize);



#ifdef __cplusplus
}
#endif

#endif /* TSF_G_TDR_NET_H */
