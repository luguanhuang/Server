/**
 *
 * @file     tdr_data_io.h
 * @brief    TDR元数据库IO操作模块
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


#ifndef TDR_DATA_IO_H
#define TDR_DATA_IO_H

#include <stdio.h>
#include "tdr/tdr_external.h"
#include "tdr/tdr_types.h"
#include "tdr/tdr_define.h"
#include "tdr/tdr_operate_data.h"


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    /** XML文件输入输出结构体对象的定义
    */

    struct tagTDRInOutputSrc
    {
        const char* szFileName;
        FILE* fp;
        TDRDATA stXmlBuf;
    };
    typedef struct tagTDRInOutputSrc TDRINOUTPUTSRC;

    struct tagTDRIOWrap
    {
        LPTDRMETA pstMeta;
        TDRINOUTPUTSRC stInOutputSrc;
        TDRDATA stHost;
        int iCutOffVer;
        int iXmlDataFormat;
        TDRBOOLEAN bEsc;    /** 是否转义处理特殊字符(注意：输入/输出支持的转义字符集不同) */
    };

    typedef struct tagTDRIOWrap TDRIOWRAP, *LPTDRIOWRAP;


    /** @defgroup  TDR_METALIB_IO TDR_从文件/文件流中保存/恢复元数据库
     * @{
     */

    /**从元数据描述二进制格式文件中加载元数据库,并校验其散列值是否和期望的散列值(NULL)一致
     * @param[in,out] a_ppstLib 保存元数据库的指针的指针
     * @param[in] a_pszBinFile 保存元数据库二进制格式的文件的文件名
     * @param[in] a_pszHash 可视化的要校验的元数据描述库散列值字符串指针
     *
     * @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
     * @note 利用TDR的工具将元数据描述库转换成.h文件时，会将元数据库的散列值字符串作为宏定义写到.h文件中，此宏
     * 定义的名字的命名规则为: "TDR_" + "元数据描述库名_" + "HASH"
     * @note 如果a_pszHash参数为NULL，则不进行散列值校验，此时此接口和tdr_load_metalib接口的功能相同
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码:
     *	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
     *	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
     *	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
     *	- TDR_ERROR_DIFF_METALIB_HASH	元数据库的散列值和期望的散列值不一致
     *
     * @pre \e a_ppstLib 不能为NULL
     * @pre \e a_pszBinFile 不能为 NULL
     * @see tdr_free_lib
     */
    TDR_API int tdr_load_verify_metalib(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pszBinFile, IN const char *a_pszHash);

    /**校验metalib的散列值是否和期望的散列值一致
     * @param[in,out] a_pstLib 保存元数据库的指针
     * @param[in] a_pszHash 可视化的要校验的元数据描述库散列值字符串指针
     *
     * @note 利用TDR的工具将元数据描述库转换成.h文件时，会将元数据库的散列值字符串作为宏定义写到.h文件中，此宏
     * 定义的名字的命名规则为: "TDR_" + "元数据描述库名_" + "HASH"
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码:
     *	- TDR_ERROR_DIFF_METALIB_HASH	元数据库的散列值和期望的散列值不一致
     *
     * @pre \e a_ppstLib 不能为NULL
     * @pre \e a_pszHash 不能为NULL
     */
    TDR_API int tdr_verify_metalib(IN LPTDRMETALIB a_pstLib, IN const char *a_pszHash);


    /**从元数据描述二进制格式文件中加载元数据库
     * @param[in,out] a_ppstLib 保存元数据库的指针的指针
     * @param[in] a_pszBinFile 保存元数据库二进制格式的文件的文件名
     * @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码:
     *	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
     *	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
     *	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
     *
     * @pre \e a_ppstLib 不能为NULL
     * @pre \e a_pszBinFile 不能为 NULL
     * @see tdr_free_lib
     */
    TDR_API int tdr_load_metalib(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pszBinFile);

    /**从元数据描述二进制格式中加载元数据库
     * @param[in,out] a_ppstLib 保存元数据库的指针的指针
     * @param[in] a_fpBin 保存元数据库的元数据描述二进制格式的文件句柄
     * @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码:
     *	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
     *	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
     *	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
     *
     * @pre \e a_ppstLib 不能为NULL
     * @pre \e a_fpBin 不能为 NULL
     * @see tdr_free_lib
     */
    TDR_API int tdr_load_metalib_fp(INOUT LPTDRMETALIB* a_ppstLib, IN FILE* a_fpBin);

    /**从元数据描述缓冲区中加载元数据库
     * @param[in,out] a_ppstLib 保存元数据库的指针的指针
     * @param[in] a_pBuf 保存元数据库的缓冲区
     * @param[in,out] a_ptLen 输入保存元数据库的缓冲的长度，输出实际读取的长度
     * @note 元数据库的空间在函数内动态分配，需调用tdr_free_lib释放元数据库的空间
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码:
     *	- TDR_ERROR_FAILED_OPEN_FILE_TO_READ	打开文件读失败
     *	- TDR_ERROR_INVALID_METALIB_FILE	元数据库文件的字节数为0，或读取的字节数和元数据描述库结构中记录的字节数不同
     *	- TDR_ERROR_BUILD_VERSION_CONFLICT	TDR生成元数据库文件的工具的构建版本和加载工具不一致
     *
     * @pre \e a_ppstLib 不能为NULL
     * @pre \e a_pBuf 不能为 NULL
     * @pre \e a_ptLen 不能为 NULL
     * @see tdr_free_lib
     */
    TDR_API int tdr_load_metalib_buf(INOUT LPTDRMETALIB* a_ppstLib, IN const char* a_pBuf, IN size_t* a_ptLen);

    /**将元数据库保存到二进制格式的文件中
     * @param[in,out] a_pstLib 保存元数据库的指针
     * @param[in] a_pszBinFile 保存元数据库的二进制格式文件的文件名
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，可能的错误代码如下:
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     - TDR_ERRIMPLE_FAILED_OPEN_FILE_TO_WRITE failed to open file to write
     - TDR_ERROR_FAILED_TO_WRITE_FILE	failed to write data to file
     *
     * @pre \e a_pstLib 不能为NULL
     * @pre \e a_pszBinFile 不能为 NULL
     */
    TDR_API int tdr_save_metalib(IN LPTDRMETALIB a_pstLib, IN const char* a_pszBinFile);

    /**将元数据库保存到二进制格式的文件中
     * @param[in,out] a_pstLib 保存元数据库的指针
     * @param[in] a_fpBin 保存元数据库的二进制格式的文件句柄
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，可能的错误代码如下:
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     - TDR_ERROR_FAILED_TO_WRITE_FILE	failed to write data to file
     *
     * @pre \e a_pstLib 不能为NULL
     * @pre \e a_fpBin 不能为 NULL
     */
    TDR_API int tdr_save_metalib_fp(IN LPTDRMETALIB a_pstLib, IN FILE* a_fpBin);

    /** @} */ /*  文件/文件流中保存/恢复元数据库*/

    /** @defgroup  TDR_METALIB_DUMP TDR_元数据库导出(调试时用)
     * @{
     */

    /**打印元数据库的内容
     * @param[in] a_pstLib 要输出的元数据库
     * @param[in] a_fp 输出文件句柄
     * @retval 0   处理成功
     * @retval <0  处理失败，可能的错误代码如下:
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     *
     * @pre \e a_pstLib 不能为NULL
     * @pre \e a_fp 不能为 NULL
     */
    TDR_API void tdr_dump_metalib(IN LPTDRMETALIB a_pstLib, IN FILE* a_fp);

    /**打印元数据库的内容
     *将内容保存在a_pszFile指定的文件中
     * @param[in] a_pstLib 要输出的元数据库
     * @param[in] a_pszFile 输出文件名字串的指针
     * @retval <0  处理失败，返回表示出错信息的错误代码
     * @retval 0   处理成功
     * @retval >0  处理成功，但发生某些特殊情况
     *
     * @pre \e a_pstLib 不能为NULL
     * @pre \e a_pszFile 不能为 NULL
     */
    TDR_API void tdr_dump_metalib_file(IN LPTDRMETALIB a_pstLib, IN const char* a_pszFile);

    /** @} */ /*  TDR_METALIB_DUMP TDR_元数据库导出(调试时用)*/


    /** @defgroup  TDR_DATA_IO TDR_数据以XML格式输入/输出
     * @{
     */

    /**设置被tdr接口处理的内存数据的字符集，如"GBK","UTF8"等
     * @param[in] a_pszEncoding 指定字符集信息的字符串缓冲区首地址
     * @note 本接口是非线程安全的，如果在多线程环境调用此接口，调用者必须保证互斥调用本接口
     * @note tdr缺省认为数据的字符集为"GBK"
     */
    void tdr_set_encoding(IN const char *a_pszEncoding);

    /**初始化tdr_input_ex和tdr_output_ex接口中结构体TDRIOWRAP类型的输入参数
     *
     * @param[in] a_pstIOWrap
     * @see TDRIOWRAP
     * */
    TDR_API void tdr_data_inoutput_init(LPTDRIOWRAP a_pstIOWrap);

    /**
     *根据元数据描述，把一个内存数据结构格式转化成一个符合特定规则的xml信息，并保存在指定缓冲区中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstXml 存储XML格式信息的结构
     *	- 输入	a_pstXml.pszBuff 指定保存XML信息的缓冲区起始地址
     *	- 输入  a_pstXml.iBuff	指定缓冲区的大小
     *	- 输出  a_pstXml.iBuff  实际转换后的信息的总长度
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
             TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc>,
             “Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过打印出a_pstXml内容，可以辅助定位出错的位置
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码：
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     *		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
     *		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
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
     * @see tdr_get_error_detail
     *
     * @pre \e a_pstMeta 不能为NULL
     * @pre \e a_pstXml 不能为NULL
     * @pre \e a_pstXml->pszBuff不能为NULL
     * @pre \e a_pstXml->iBuff 必须大于0
     * @pre \e a_pstHost 不能为NULL
     * @pre \e a_pstHost->pszBuff不能为NULL
     * @pre \e a_pstHost->iBuff 必须大于0
     */
    TDR_API int tdr_output(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstXml, IN LPTDRDATA a_pstHost,
                           IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

    /**
     *根据元数据描述，把一个内存数据结构格式转化成一个符合特定规则的xml信息，并保存在指定文件中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in]	a_fpXml 存储XML格式信息的文件句柄
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     *
     * @note 如果a_iCutOffVersion为0 ，则按照最高版本处理
     * @note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
             TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc>,
             “Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”

     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出文件内容，可以辅助定位出错位置
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_fpXml 不能为 NULL
     * @pre \e a_pstHost 不能为NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码：
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     *		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
     *		- TDR_ERROR_NO_HOSTBUFF_SPACE 本地存储缓冲区剩余空间不够
     *		- TDR_ERROR_TOO_COMPLIEX_META 元数据描述复合数据类型嵌套层次超过32层
     *		- TDR_ERROR_NET_UNSUPPORTED_TYPE    不支持的元数据类型
     *		- TDR_ERROR_NET_INVALID_STRING_LEN string类型的元素其字符串长度超过了预定最大长度
     *		- TDR_ERROR_NET_INVALID_VERSIONINDICATOR 元数据版本指示器的值比保存其值的元数据成员的版本要小
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see tdr_output
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_output_fp(IN LPTDRMETA a_pstMeta, IN FILE *a_fpXml, IN LPTDRDATA a_pstHost,
                              IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

    /**
     *根据元数据描述，把一个内存数据结构格式转化成一个符合特定规则的xml信息，并保存在指定文件中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in]	a_szFile 存储XML格式信息的文件的文件名字符串指针
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
             TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc>,
             “Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出文件内容，可以辅助定位出错位置
     *
     * @pre \e a_pstMeta 不能为NULL
     * @pre \e a_szFile 不能为NULL
     * @pre \e a_pstHost 不能为NULL
     * @pre \e a_pstHost->pszBuff不能为NULL
     * @pre \e a_pstHost->iBuff 不能为NULL
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see tdr_output
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_output_file(IN LPTDRMETA a_pstMeta, IN const char *a_szFile, IN LPTDRDATA a_pstHost,
                                IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

    /**根据元数据描述，"把一个内存数据结构格式转化成一个符合特定规则的xml信息" 的统一接口
     * 集成了 tdr_output/tdr_output_fp/tdr_output_file 接口的功能，并且支持转义输出某些字符
     *
     * @param[in] a_pstIOWrap 功能控制参数
     *
     * @note 调用该接口之前必须先调用接口tdr_data_inoutput_init初始化输入参数，或者把输入参数指向的对象 memset 为 0
     *
     * @note 转义处理仅仅支持以八进制格式(\0??)输出 回车符('\\r')、换行符('\\n')、制表符('\\t')、空格(' '),  以 \\\\ 表示 反斜杠('\\')
     *
     * @note a_pstIOWrap->stInOutputSrc的成员的优先级：szFileName > fp > stXmlBuf。例如 szFileName!=NULL 时，以szFileName为准，忽略 fp 和 stXmlBuf 的值
     *
     * @note 如果该接口返回失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出文件/缓冲区内容，可以辅助定位出错位置
     *	- 调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see tdr_data_inoutput_init
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     * @see TDRIOWRAP
     */
    TDR_API int tdr_output_ex(IN LPTDRIOWRAP a_pstIOWrap);

    /**根据元数据描述将符合特定规则的xml信息转化成本地内存数据结构格式
     *xml信息保存在指定缓冲区中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     *	- 输出  a_pstHost.iBuff  如果传入的长度大于结构体长度，修改为结构体长度；
                                 如果小于结构体的长度，则不会修改该值
     * @param[in]	a_pstXml 存储XML格式信息的结构
     *	- 输入	a_pstXml.pszBuff 指定保存XML信息的缓冲区起始地址
     *	- 输入  a_pstXml.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
             TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc>,
             “Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
     * @note 本接口不支持versionindicator属性
     * @note 本接口不支持 指针和引用 属性
     *
     * @note 对于描述中定义的数组，如果配置的元素个数超过定义的大小，该接口默默丢弃多余的元素
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_pstXml 不能为 NULL
     * @pre \e a_pstXml.pszBuff 不能为 NULL
     * @pre \e a_pstXml.iBuff 必须大于0
     * @pre \e a_pstXml 不能为 NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码：
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     *	- TDR_ERROR_NO_XML_ROOT	没有合适的根节点，请确保XML元素树中包含以元数据名为根的节点
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see  tdr_output
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_input(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN LPTDRDATA a_pstXml,
                          IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

    /**根据元数据描述将符合特定规则的xml信息转化成本地内存数据结构格式
     *xml信息保存在指定文件中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     *	- 输出  a_pstHost.iBuff   如果传入的长度大于结构体长度，修改为结构体长度；
                                  如果小于结构体的长度，则不会修改该值
     * @param[in]	a_fp 存储XML格式信息的文件句柄
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
             TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc>,
             “Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
     * @note 本接口不支持versionindicator属性，
     * @note 本接口不支持 指针和引用 属性
     *
     * @note 对于描述中定义的数组，如果配置的元素个数超过定义的大小，该接口默默丢弃多余的元素

     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_fp 不能为 NULL
     * @pre \e a_psHost 不能为NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码：
     *	- TDR_ERROR_NO_XML_ROOT	没有合适的根节点，请确保XML元素树中包含以元数据名为根的节点
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see  tdr_output
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_input_fp(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN FILE *a_fp,
                             IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

    /**根据元数据描述将符合特定规则的xml信息转化成本地内存数据结构格式
     *xml信息保存在指定文件中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     *	- 输出  a_pstHost.iBuff   如果传入的长度大于结构体长度，修改为结构体长度；
                                  如果小于结构体的长度，则不会修改该值
     * @param[in]	a_pszFile 存储XML格式信息的文件的文件名字符串指针
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_MATA_NAME
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这几种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_MATA_NAME，TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，
             TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note 关于xml数据文件的具体格式说明，请参考文档<TSF-G-DR-FAQ.doc>,
             “Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
     * @note 本接口不支持versionindicator属性
     * @note 本接口不支持 指针和引用 属性
     *
     * @note 对于描述中定义的数组，如果配置的元素个数超过定义的大小，该接口默默丢弃多余的元素
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_pszFile 不能为 NULL
     * @pre \e a_psHost 不能为NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码：
     *	- TDR_ERROR_INVALID_PARAM	参数无效
     *	- TDR_ERROR_NO_XML_ROOT	没有合适的根节点，请确保XML元素树中包含以元数据名为根的节点
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see  tdr_output
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_input_file(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN const char *a_pszFile,
                               IN int a_iCutOffVersion, IN int a_iXmlDataFormat);


    /**根据元数据描述, "将符合特定规则的xml信息转化成本地内存数据结构格式" 的统一接口，
     * 集成了 tdr_input/tdr_input_fp/tdr_input_file 接口的功能，并且支持输入转义字符序列
     *
     * @param[in] a_pstIOWrap 功能控制参数
     *
     * @note 调用该接口之前必须先调用接口tdr_data_inoutput_init初始化输入参数，或者把输入参数指向的对象 memset 为 0
     *
     * @note 转义处理仅仅支持输入八进制格式的转义字符序列。例如, \\012 表示 回车符('\\n'), \\\\ 表示 反斜杠('\\')
     *
     * @note a_pstIOWrap->stInOutputSrc的成员的优先级：szFileName > fp > stXmlBuf。例如 szFileName!=NULL 时，以szFileName为准，忽略 fp 和 stXmlBuf 的值
     *
     * @note 对于描述中定义的数组，如果配置的元素个数超过定义的大小，该接口默默丢弃多余的元素
     *
     * @note 如果该接口返回失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *  - 调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see tdr_data_inoutput_init
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     * @see TDRIOWRAP
     *
     */
    TDR_API int tdr_input_ex(IN LPTDRIOWRAP a_pstIOWrap);


    /** @}*/ /*defgroup  TDR_DATA_IO TDR_数据以XML格式输入/输出 */


    /** @defgroup  TDR_DATA_IO_JSON TDR_数据以JSON格式输入/输出
     * @{
     */

    /**把JSON格式数据转换成C语言二进制内存数据
     * @deprecated 目前已不推荐使用
     * @param[in]       a_pstMeta 元数据描述的指针, 只能为<struct>的描述
     * @param[in,out]   a_pstHost 内存数据缓冲区
     * @param[in]       a_pstJson 输入缓冲区, 指向JSON数据
     * @param[in]       a_iCutOffVersion 数据成员剪裁版本
     *
     * @pre a_pstMeta 不能为NULL
     * @pre a_pstHost 不能为NULL
     * @pre a_pstJson 不能为NULL
     *
     * @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
     * - TDR float/double                          对应 JSON float
     * - TDR string/wstring/date/time/datetime/ip  对应 JSON string
     * - TDR wchar 等其他数据类型　                对应 JSON integer
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     *
     * @note 对于输入的JSON格式数据，本接口有如下要求：
     * - 所有key使用""引用, 不允许出现未经""引用的key
     * - 不允许出现空数组[]
     * - 不允许出现 null  值
     * - 不允许出现 true  值
     * - 不允许出现 false 值
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     * @note 指针/引用类型的entry不处理
     * @note 对于指定了refer属性的entry, 如果entry有相应的JSON数据项，根据refer的值检查该数据项是否正确
     * @note 对于指定了refer属性的entry, 如果entry没有相应的JSON数据项，则本接口不检查数据的有效性
     */
    TDR_API int tdr_input_json(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost,
                               IN LPTDRDATA a_pstJson, IN int a_iCutOffVersion);


   /**使用rapidjson库把JSON格式数据转换成C语言二进制内存数据
    * 由于rapidjson库的自身的原因，本接口不支持windows VC6版本
	* @param[in]	   a_pstMeta 元数据描述的指针, 只能为<struct>的描述
	* @param[in,out]   a_pstHost 内存数据缓冲区,传入的a_pstHost->iBuff建议等于传入字符串实际占用的字节数，或者字符串长度(strlen结果)加1(字符串结束标志)
	* @param[in]	   a_pstJson 输入缓冲区, 指向JSON数据
	* @param[in]	   a_iCutOffVersion 数据成员剪裁版本
	*
	* @pre a_pstMeta 不能为NULL
	* @pre a_pstHost 不能为NULL
	* @pre a_pstJson 不能为NULL
	*
	* @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
	* - TDR float/double						  对应 JSON float
	* - TDR string/wstring/date/time/datetime/ip  对应 JSON string
	* - TDR wchar 等其他数据类型　				  对应 JSON integer
	*
	* @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
	*
	* @note 对于输入的JSON格式数据，本接口有如下要求：
	* - 所有key使用""引用, 不允许出现未经""引用的key
	* - 不允许出现空数组[]
	* - 不允许出现 null  值
	* - 不允许出现 true  值
	* - 不允许出现 false 值
	*
	* @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
	* @note 指针/引用类型的entry不处理
	* @note 对于指定了refer属性的entry, 如果entry有相应的JSON数据项，根据refer的值检查该数据项是否正确
	* @note 对于指定了refer属性的entry, 如果entry没有相应的JSON数据项，则本接口不检查数据的有效性
     */
	TDR_API int tdr_input_json_ex(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost,
                   IN LPTDRDATA a_pstJson, IN int a_iCutOffVersion);



    /**把JSON格式数据转换成C语言二进制内存数据
     * @deprecated 目前已不推荐使用
     * @param[in]       a_pstMeta 元数据描述的指针, 只能为<struct>的描述
     * @param[in,out]   a_pstHost 内存数据缓冲区
     * @param[in]       a_fpJson  JSON数据文件指针
     * @param[in]       a_iCutOffVersion 数据成员剪裁版本
     *
     * @pre a_pstMeta 不能为NULL
     * @pre a_pstHost 不能为NULL
     * @pre a_pstJson 不能为NULL
     *
     * @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
     * - TDR float/double                          对应 JSON float
     * - TDR string/wstring/date/time/datetime/ip  对应 JSON string
     * - TDR wchar 等其他数据类型　                对应 JSON integer
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     *
     * @note 对于输入的JSON格式数据，本接口有如下要求：
     * - 所有key使用""引用, 不允许出现未经""引用的key
     * - 不允许出现空数组[]
     * - 不允许出现 null  值
     * - 不允许出现 true  值
     * - 不允许出现 false 值
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     * @note 指针/引用类型的entry不处理
     * @note 对于指定了refer属性的entry, 如果entry有相应的JSON数据项，根据refer的值检查该数据项是否正确
     * @note 对于指定了refer属性的entry, 如果entry没有相应的JSON数据项，则本接口不检查数据的有效性
     */
    TDR_API int tdr_input_json_fp(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost,
                                  IN FILE *a_fpJson, IN int a_iCutOffVersion);

    /**把JSON格式数据转换成C语言二进制内存数据
     * @deprecated 目前已不推荐使用
     * @param[in]       a_pstMeta 元数据描述的指针, 只能为<struct>的描述
     * @param[in,out]   a_pstHost 内存数据缓冲区
     * @param[in]       a_pszJsonFile JSON数据文件
     * @param[in]       a_iCutOffVersion 数据成员剪裁版本
     *
     * @pre a_pstMeta 不能为NULL
     * @pre a_pstHost 不能为NULL
     * @pre a_pstJson 不能为NULL
     *
     * @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
     * - TDR float/double                          对应 JSON float
     * - TDR string/wstring/date/time/datetime/ip  对应 JSON string
     * - TDR wchar 等其他数据类型　                对应 JSON integer
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     *
     * @note 对于输入的JSON格式数据，本接口有如下要求：
     * - 所有key使用""引用, 不允许出现未经""引用的key
     * - 不允许出现空数组[]
     * - 不允许出现 null  值
     * - 不允许出现 true  值
     * - 不允许出现 false 值
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     * @note 指针/引用类型的entry不处理
     * @note 对于指定了refer属性的entry, 如果entry有相应的JSON数据项，根据refer的值检查该数据项是否正确
     * @note 对于指定了refer属性的entry, 如果entry没有相应的JSON数据项，则本接口不检查数据的有效性
     */
    TDR_API int tdr_input_json_file(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost,
                                    IN const char *a_pszJsonFile, IN int a_iCutOffVersion);

    /**把C语言二进制内存数据转换成JSON格式数据
     * @deprecated 目前已不推荐使用
     * @param[in]       a_pstMeta 元数据描述的指针, 只能为<struct>的描述
     * @param[in,out]   a_pstJson 输出缓冲区，用于保存生成的JSON数据
     * @param[in]       a_pstHost 内存数据缓冲区
     * @param[in]       a_iCutOffVersion 数据成员剪裁版本
     *
     * @pre a_pstMeta 不能为NULL
     * @pre a_pstJson 不能为NULL
     * @pre a_pstHost 不能为NULL
     *
     * @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
     * - TDR float/double                          对应 JSON float
     * - TDR string/wstring/date/time/datetime/ip  对应 JSON string
     * - TDR wchar 等其他数据类型　                对应 JSON integer
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     * @note 不输出指针/引用类型的数据，不输出元数据描述中指定了 io="nooutput" 属性的数据
     *
     * @note 输出的JSON数据外层使用{}包装
     * @note 输出的JSON数据中，关键字全部使用""引用
     * @note 输出的JSON数据中，关键字顺序与 a_pstMeta 定义的顺序相同
     */
    TDR_API int tdr_output_json(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstJson,
                                IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

    /**把C语言二进制内存数据转换成JSON格式数据
     * @deprecated 目前已不推荐使用
     * @param[in]       a_pstMeta 元数据描述的指针, 只能为<struct>的描述
     * @param[in,out]   a_fpJson  用于保存生成的JSON数据的文件指针
     * @param[in]       a_pstHost 内存数据缓冲区
     * @param[in]       a_iCutOffVersion 数据成员剪裁版本
     *
     * @pre a_pstMeta 不能为NULL
     * @pre a_pstJson 不能为NULL
     * @pre a_pstHost 不能为NULL
     *
     * @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
     * - TDR float/double                          对应 JSON float
     * - TDR string/wstring/date/time/datetime/ip  对应 JSON string
     * - TDR wchar 等其他数据类型　                对应 JSON integer
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     * @note 不输出指针/引用类型的数据，不输出元数据描述中指定了 io="nooutput" 属性的数据
     *
     * @note 输出的JSON数据外层使用{}包装
     * @note 输出的JSON数据中，关键字全部使用""引用
     * @note 输出的JSON数据中，关键字顺序与 a_pstMeta 定义的顺序相同
     */
    TDR_API int tdr_output_json_fp(IN LPTDRMETA a_pstMeta, IN FILE *a_fpJson,
                                   IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

    /**把C语言二进制内存数据转换成JSON格式数据
     * @deprecated 目前已不推荐使用
     * @param[in]       a_pstMeta 元数据描述的指针, 只能为<struct>的描述
     * @param[in,out]   a_pszJsonFile  用于保存生成的JSON数据的文件
     * @param[in]       a_pstHost 内存数据缓冲区
     * @param[in]       a_iCutOffVersion 数据成员剪裁版本
     *
     * @pre a_pstMeta 不能为NULL
     * @pre a_pstJson 不能为NULL
     * @pre a_pstHost 不能为NULL
     *
     * @note TDR支持的C的基本类型与JSON基本类型的对应关系如下:
     * - TDR float/double                          对应 JSON float
     * - TDR string/wstring/date/time/datetime/ip  对应 JSON string
     * - TDR wchar 等其他数据类型　                对应 JSON integer
     *
     * @note 由于字符编码的问题，对于本接口，建议元数据描述中不要使用 wchar/wstring
     * @note 不输出指针/引用类型的数据，不输出元数据描述中指定了 io="nooutput" 属性的数据
     *
     * @note 输出的JSON数据外层使用{}包装
     * @note 输出的JSON数据中，关键字全部使用""引用
     * @note 输出的JSON数据中，关键字顺序与 a_pstMeta 定义的顺序相同
     */
    TDR_API int tdr_output_json_file(IN LPTDRMETA a_pstMeta, IN const char *a_pszJsonFile,
                                     IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

    /** @}*/ /*defgroup  TDR_DATA_IO_JSON TDR_数据以JSON格式输入/输出 */



    /** @defgroup  TDR_PRINT_DATA TDR_数据打印
     * @{
     */

    struct tagTdrPrintFormat
    {
        const char* pszSepStr; /* 字段分隔字符串 */
        int  iNoVarName;       /* 不输出变量名 */
        char chIndentChar;     /* 缩进字符, 每层缩进 4 个缩进字符 */
        int iWithVersion;  /* 剪裁版本 */
    };

    typedef struct tagTdrPrintFormat      TDRPRINTFORMAT;
    typedef struct tagTdrPrintFormat*   LPTDRPRINTFORMAT;

    /**取得tdr_sprintf/tdr_fprintf使用的默认打印格式
     * @param[in, out] 输入保存打印格式的缓冲区，输出默认的打印格式
     *
     * @note a_pstFormat 不能为 NULL
     */
    TDR_API int tdr_get_default_format(INOUT LPTDRPRINTFORMAT a_pstFormat);

    /**根据元数据描述将本地内存数据结构以可视化的方式输出
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in]	a_fp 存储可视化数据信息的文件句柄
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出文件内容，可以辅助定位出错位置
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_fp 不能为 NULL
     * @pre \e a_psHost 不能为NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     *
     * @retval 0   处理成功
     * @retval >0  处理成功，但发生某些特殊情况
     * @retval <0  处理失败，返回表示出错信息的错误代码
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_fprintf(IN LPTDRMETA a_pstMeta, IN FILE  *a_fp, IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

    /**根据元数据描述将本地内存数据结构以可视化的方式输出
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in]	a_fp 存储可视化数据信息的文件句柄
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_pstFormat 输出格式控制结构
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 设置a_pstFormat的成员之前，必须先调用 tdr_get_default_format 取得默认的格式设置
     * @note 如果a_pstFormat->iNoVarName!=0, 则不输出变量名
     * @note 如果a_pstFormat->pszSepStr==NULL，则使用"\\n"作为分隔字符串
     * @note 设置a_pstFormat->chIndentChar=0, 就可以输出紧凑的可视化数据
     * @note 如果a_pstFormat->iWithVersion=0，则不输出'V+剪裁版本'信息，否则在字符串头部信息中输出'V+剪裁版本号'
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出文件内容，可以辅助定位出错位置
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_fp 不能为 NULL
     * @pre \e a_psHost 不能为NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     * @pre \e a_pstFormat 不能为 NULL
     *
     * @retval 0   处理成功
     * @retval >0  处理成功，但发生某些特殊情况
     * @retval <0  处理失败，返回表示出错信息的错误代码
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_fprintf_ex(IN LPTDRMETA a_pstMeta, IN FILE  *a_fp, IN LPTDRDATA a_pstHost,
                               IN int a_iCutOffVersion, IN LPTDRPRINTFORMAT a_pstFormat);

    /**根据元数据描述将本地内存数据结构以可视化的方式输出到字符串缓冲区中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstOut 存储可视化数据的结构体指针
     *	- 输入	a_pstOut.pszBuff 指定保存存储可视化数据的信息的缓冲区起始地址
     *	- 输入  a_pstOut.iBuff	指定缓冲区的大小
     *	- 输出  a_pstOut.iBuff  实际生成的存储可视化数据的信息的总长度
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出缓冲区内容，可以辅助定位出错位置
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_pstHost 不能为 NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     * @pre \e a_pstOut 不能为 NULL
     * @pre \e a_pstOut.pszBuff 不能为 NULL
     * @pre \e a_pstOut.iBuff 必须大于0
     *
     * @retval 0   处理成功
     * @retval >0  处理成功，但发生某些特殊情况
     * @retval <0  处理失败，返回表示出错信息的错误代码
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     */
    TDR_API int tdr_sprintf(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstOut, IN LPTDRDATA a_pstHost, IN int a_iCutOffVersion);

    /**根据元数据描述将本地内存数据结构以可视化的方式输出到字符串缓冲区中
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstOut 存储可视化数据的结构体指针
     *	- 输入	a_pstOut.pszBuff 指定保存存储可视化数据的信息的缓冲区起始地址
     *	- 输入  a_pstOut.iBuff	指定缓冲区的大小
     *	- 输出  a_pstOut.iBuff  实际生成的存储可视化数据的信息的总长度
     * @param[in]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_pstFormat 输出格式控制结构
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本
     * @note 设置a_pstFormat的成员之前，必须先调用 tdr_get_default_format 取得默认的格式设置
     * @note 如果a_pstFormat->iNoVarName!=0, 则不输出变量名
     * @note 如果a_pstFormat->pszSepStr==NULL，则使用"\\n"作为分隔字符串
     * @note 设置a_pstFormat->chIndentChar=0, 就可以输出紧凑的可视化数据
     * @note 如果a_pstFormat->iWithVersion=0，则不输出'V+剪裁版本'信息，否则在字符串头部信息中输出'V+剪裁版本号'
     *
     * @note 如果生成的数据需要使用接口tdr_sscanf_ex反操作，受tdr_sscanf_ex的限制，分隔字符串只能包含一个字符
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *	- 通过查看输出缓冲区内容，可以辅助定位出错位置
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_pstHost 不能为 NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     * @pre \e a_pstOut 不能为 NULL
     * @pre \e a_pstOut.pszBuff 不能为 NULL
     * @pre \e a_pstOut.iBuff 必须大于0
     * @pre \e a_pstFormat 不能为 NULL
     *
     * @retval 0   处理成功
     * @retval >0  处理成功，但发生某些特殊情况
     * @retval <0  处理失败，返回表示出错信息的错误代码
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     * @see tdr_sscanf_ex
     */
    TDR_API int tdr_sprintf_ex(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstOut, IN LPTDRDATA a_pstHost,
                               IN int a_iCutOffVersion, IN LPTDRPRINTFORMAT a_pstFormat);

    /**根据元数据描述把可视化输入转换成本地内存数据结构, 是接口tdr_sprintf_ex的反操作
     * @param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in,out]	a_pstHost 存储本地内存数据结构信息的结构体指针
     *	- 输入	a_pstHost.pszBuff 指定保存本地结构信息的缓冲区起始地址
     *	- 输入  a_pstHost.iBuff	指定缓冲区的大小
     *	- 输出  a_pstHost.iBuff   如果传入的长度大于结构体长度，修改为结构体长度；
                                  如果小于结构体的长度，则不会修改该值
     * @param[in]	a_pstVisual 存储可视化数据的结构体指针
     *	- 输入	a_pstVisual.pszBuff 指定保存存储可视化数据的信息的缓冲区起始地址
     *	- 输入  a_pstVisual.iBuff	指定缓冲区的大小
     * @param[in] a_iCutOffVersion	要剪裁的版本号
     * @param[in] a_pstFormat 输出格式控制结构
     *
     * @note 如果a_iCutOffVersion为0 ，则安装剪裁到最高版本,如果a_pstFormat->iWithVersion为0(此时认为输出文本中不含V+版本号)，则a_iCutOffVersion值必须与输出时a_iCutOffVersion版本值相同
     * @note 设置a_pstFormat的成员之前，必须先调用 tdr_get_default_format 取得默认的格式设置初始化
     * @note 如果a_pstFormat->iNoVarName!=0, 则读取时忽略变量名，如果和可视化文本不一致，则读取失败
     * @note 如果a_pstFormat->pszSepStr==NULL，则使用"\\n"作为分隔字符串，必须和可视化文本中分隔符相同
     * @note 出于性能与实现的考虑，暂时仅仅支持一个分隔符(字符串a_pstFormat->pszSepStr的长度为1)
     * @note 设置a_pstFormat->chIndentChar为要读取的可视化文本的缩进符，必须和可视化本文中缩进符相同
     * @note 如果a_pstFormat->iWithVersion=0，则假定可视化文本中没有版本信息，否则认为可视化文本中具有版本信息。
     * 如果a_pstFormat->iWithVersion为0，则以a_iCutOffVersion字段为剪裁版本基准值，否则，以解析出来的'V+'版本值为剪裁版本基准值。
     *
     * @note 如果处理失败，获取错误信息的方法:
     *	- 根据返回值，调用tdr_error_string()可以获取出错信息
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_pstHost 不能为 NULL
     * @pre \e a_pstHost.pszBuff 不能为 NULL
     * @pre \e a_pstHost.iBuff 必须大于0
     * @pre \e a_pstVisual 不能为 NULL
     * @pre \e a_pstVisual.pszBuff 不能为 NULL
     * @pre \e a_pstVisual.iBuff 必须大于0
     * @pre \e a_pstFormat 不能为 NULL
     *
     * @retval 0   处理成功
     * @retval >0  处理成功，但发生某些特殊情况
     * @retval <0  处理失败，返回表示出错信息的错误代码
     *
     * @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
     *
     * @see	tdr_get_meta_by_name
     * @see	tdr_get_meta_by_id
     * @see	tdr_error_string
     * @see tdr_get_error_detail
     * @see tdr_sprintf_ex
     */
    TDR_API int tdr_sscanf_ex(IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstHost, IN LPTDRDATA a_pstVisual,
                               IN int a_iCutOffVersion, IN LPTDRPRINTFORMAT a_pstFormat);
    /** @}*/ /* @defgroup  TDR_PRINT_DATA TDR_数据打印*/


    /** @defgroup TDR_GEN_XMLTMPLT TDR_为元数据生成配置模板
     * @{
     */

    /**
     * 为指定元数据生成XML格式的配置模板，并保存到指定文件中
     * @param[in] a_pstMeta     元数据描述指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
     * @param[in] a_fpFile      存储配置模板的文件句柄
     * @param[in] a_iCutOffVersion 要剪裁的版本号
     * @param[in] a_iXmlDataFormat XML数据文件的格式,目前支持的格式有：
     *	- TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME
     *	- TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME
     *	- 这两种格式的区别请参见文件 TSF4G-DR-FAQ.doc 关于xml数据文件输入/输出部分
     *
     * @note    关于xml数据文件的具体格式说明，请参考
     *          文档<TSF-G-DR-FAQ.doc>中“Xml数据文件输入/输出篇” -> “Tdr支持什么格式的xml数据文件？”
     * @note    如果a_iCutOffVersion为0 ，则按照最高版本处理
     * @note    如果a_iXmlDataFormat指定除TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME，TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME以外的值，
     *          则强制按照格式TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME进行处理
     * @note    如果成员有desc属性信息，则以注释方式输出desc属性
     * @note    对于union结构，首先输出该union的select当前值对应的子成员，然后以"注释"的形式输出其他子成员
     *          由于XML不支持注释嵌套，目前使用“commented_union_item”包装需注释的子成员，
     *          示意子成员已经被注释
     * @note    对于数组成员，以注释的形式指明数组的大小。
     *          如果相关的refer属性值不为0, 则输出的元素个数由refer属性值指定。
     *          否则仅仅输出2个元素。
     *
     * @pre \e a_pstMeta 不能为 NULL
     * @pre \e a_fpFile 不能为 NULL
     *
     * @retval 0   处理成功
     * @retval <0  处理失败，返回表示出错信息的错误代码
     *
     */
    TDR_API int tdr_gen_xmltmplt(IN LPTDRMETA a_pstMeta, OUT FILE *a_fpFile, IN int a_iCutOffVersion, IN int a_iXmlDataFormat);

    /** @}*/ /* @defgroup TDR_GEN_XMLTMPLT TDR_为元数据生成配置模板*/

#ifdef __cplusplus
}
#endif

#endif /* TDR_DATA_IO_H */
