/**
*
* @file     tdr_meta_entries_index.h
* @brief    TDR结构体内加快成员访问速度的索引模块头文件
*
* @author jackyai
* @version 1.0
* @date 2007-12-04
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/
#ifndef TORM_META_ENTRIES_INDEX_H
#define TORM_META_ENTRIES_INDEX_H

#include <stdio.h>
#include "tdr/tdr_types.h"
#include "tdr/tdr_external.h"

typedef struct  tagTDRNameEntryIndexInfo TDRNAMEENTRYINDEXINFO;
typedef struct  tagTDRNameEntryIndexInfo	*LPTDRNAMEENTRYINDEXINFO;
typedef struct tagTDRMetaEntryNameIndex TDRMETAENTRYNAMEINDEX;
typedef struct tagTDRMetaEntryNameIndex *LPTDRMETAENTRYNAMEINDEX;
typedef struct  tagTDREntryOffIndexInfo TDRENTRYOFFINDEXINFO;
typedef struct  tagTDREntryOffIndexInfo	*LPTDRENTRYOFFINDEXINFO;
typedef struct tagTDRMetaEntryOffIndex TDRMETAENTRYOFFINDEX;
typedef struct tagTDRMetaEntryOffIndex *LPTDRMETAENTRYOFFINDEX;

/**结构体内成员名－指针 索引信息
*/
struct  tagTDRNameEntryIndexInfo
{
	TDRBOOLEAN bIsInExtendArr;	/**<此成员是否是开扩展的结构数据成员的子孙成员。目前此成员已经不用date:2009-3-20*/
	LPTDRMETAENTRY	pstEntry;	/**<此成员的在元数据中的指针*/
};


/*结构体 成员名－指针索引表*/
struct tagTDRMetaEntryNameIndex
{
	int iFlag;	/**<索引表标志信息，上层应用不能修改*/
	int iEntryNum;	/**<Meta中建立成员索引的数目*/
	TDRNAMEENTRYINDEXINFO astIndexs[1];		/**<索引信息区*/
};


/**结构体内成员指针－存储偏移 索引信息
*/
struct  tagTDREntryOffIndexInfo
{
	LPTDRMETAENTRY	pstEntry;	/**<此成员的在元数据中的指针*/
	TDROFF iHOff;			/**<此成员在结构中的偏移*/
};



/*结构体 成员指针－存储偏移索引表*/
struct tagTDRMetaEntryOffIndex
{
	int iFlag;	/**<索引表标志信息，上层应用不能修改*/
	int iEntryNum;	/*Meta中建立索引的成员数目*/
	TDRENTRYOFFINDEXINFO astIndexs[1];		/*索引信息区*/
};

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_META_ENTRY_INDEX TDR_结构体成员索引表相关
* @{
*/

/**为结构体内的成员创建名字－指针索引表
*@param[in,out]	a_pstIndex 获取成员名字－指针索引表数据结构指针的指针
*@param[in] a_pstMeta	描述结构体的元数据句柄
*@param[in,out] a_pstBuff 索引表存储区信息，如果此参数为NULL，则索引表的内存存储区在函数内动态分配；否则索引表将以此参数的指定缓冲区做为存储区
	- 输入	a_pstBuff.pszbuff 指定缓冲区的首地址
	- 输入	a_pstBuff.iBuff 指定缓冲区的可用空间大小（字节数）
	- 输出  a_pstBuff.iBuff 创建索引表后，已使用的缓冲区空间大小
*@note 成员名字－指针索引表建立的前提是结构体中成员的名字不重复，目前在TDR支持的ORM系统中的结构符合此约束
*@retval 0   处理成功
*@retval <0  处理失败,错误代码
*	-	TDR_ERROR_NO_MEMORY 分配内存失败，或指定的缓冲区指针为空，或指定缓冲区空间不够
*@pre \e  a_pstIndex 不能为NULL
*@pre \e a_pstMeta 不能为NULL
*/
TDR_API int tdr_create_entry_name_index(INOUT LPTDRMETAENTRYNAMEINDEX *a_pstIndex, IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstBuff);

/**销毁成员名字－指针索引表
*@param[in,out] a_pstIndex 指向成员名字－指针索引表数据结构指针的指针
*@note 如果创建索引表时没有指定存储缓冲区的，即索引表的存储空间是在接口内分配的，则必须调用此接口以释放空间
*/
TDR_API void tdr_destroy_entry_name_index(INOUT LPTDRMETAENTRYNAMEINDEX *a_pstIndex);

/**以可视化的方式导出索引表
*@param[in] a_pstIndex 指向成员名字－指针索引表数据结构的指针
*@param[in] a_fpFile 保存导出信息的文件句柄
*@pre \e  a_pstIndex 不能为NULL
*@pre \e a_fpFile 不能为NULL
*/
TDR_API void tdr_dump_entry_name_index(IN FILE *a_fpFile, IN LPTDRMETAENTRYNAMEINDEX a_pstIndex);

/**根据成员名查找此成员的名字_指针索引信息
*@param[in] a_pstIndex 指向成员名字－指针索引表数据结构的指针
*@param[in] a_pszName 成员名字字符串指针
*@return 如果找到名字匹配的索引项，则返回此索引项的指针；否则返回NULL
*@pre \e  a_pstIndex 不能为NULL
*@pre \e  a_pszName 不能为NULL
*/
TDR_API LPTDRNAMEENTRYINDEXINFO tdr_find_entry_name_index(IN LPTDRMETAENTRYNAMEINDEX a_pstIndex, IN const char *a_pszName);



/**为结构体内的成员创建指针－偏移索引表
*@param[in,out]	a_pstIndex 获取成员指针－偏移索引表数据结构指针的指针
*@param[in] a_pstMeta	描述结构体的元数据句柄
*@param[in,out] a_pstBuff 索引表存储区信息，如果此参数为NULL，则索引表的内存存储区在函数内动态分配；否则索引表将以此参数的指定缓冲区做为存储区
- 输入	a_pstBuff.pszbuff 指定缓冲区的首地址
- 输入	a_pstBuff.iBuff 指定缓冲区的可用空间大小（字节数）
- 输出  a_pstBuff.iBuff 创建索引表后，已使用的缓冲区空间大小
*@pre \e  a_pstIndex 不能为NULL
*@pre \e a_pstMeta 不能为NULL
s
*@retval 0   处理成功
*@retval <0  处理失败
*/
TDR_API int tdr_create_entry_off_index(INOUT LPTDRMETAENTRYOFFINDEX *a_pstIndex, IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstBuff);

/**销毁成员指针_偏移索引表
*@param[in,out]	a_pstIndex 指向成员指针－偏移索引表数据结构指针的指针
*@note 如果创建索引表时没有指定存储缓冲区的，即索引表的存储空间是在接口内分配的，则必须调用此接口以释放空间
*/
TDR_API void tdr_destroy_entry_off_index(INOUT LPTDRMETAENTRYOFFINDEX *a_pstIndex);

/**以可视化的方式导出索引表
*@param[in]	a_pstIndex 指向成员指针－偏移索引表数据结构的指针
*@param[in]	a_fpFile 保存导出信息的文件句柄
*@pre \e  a_pstIndex 不能为NULL
*@pre \e a_fpFile 不能为NULL
*/
TDR_API void tdr_dump_entry_off_index(IN FILE *a_fpFile, IN LPTDRMETAENTRYOFFINDEX a_pstIndex);

/**根据成员元数据指针查找此成员的指针－偏移索引信息
*@param[in] a_pstIndex 指向成员指针－偏移索引表数据结构的指针
*@param[in] a_pstEntry 成员元数据描述的指针
*@return 如果找到名字匹配的索引项，则返回此索引项的指针；否则返回NULL
*@pre \e  a_pstIndex 不能为NULL
*@pre \e  a_pszName 不能为NULL
*/
TDR_API LPTDRENTRYOFFINDEXINFO tdr_find_entry_off_index(IN LPTDRMETAENTRYOFFINDEX a_pstIndex, IN LPTDRMETAENTRY a_pstEntry);


/**根据元数据成员索引表和元数据的基址计算某个成员的存储地址
*@param[in]	a_pMetaBase 此元数据存储区的首地址
*@param[in]	a_pstNameIndex 指向成员名字－指针索引表数据结构的指针
*@param[in]	a_pstOffIndex 指向成员指针－偏移索引表数据结构的指针
*@param[in] a_pszName 成员名字字符串指针
*@return 如果成功找到名字匹配的成员,则返回此成员的存储地址,否则返回NULL
*@pre \e  a_pMetaBase 不能为NULL
*@pre \e  a_pstNameIndex 不能为NULL
*@pre \e  a_pstOffIndex 不能为NULL
*@pre \e  a_pszName 不能为NULL
*/
TDR_API void *tdr_calc_entry_address(IN void *a_pMetaBase, IN LPTDRMETAENTRYNAMEINDEX a_pstNameIndex,
									 IN LPTDRMETAENTRYOFFINDEX a_pstOffIndex, IN const char *a_pszName);

/** @}*/ /*defgroup TDR_META_ENTRY_INDEX TDR_结构体成员索引表相关*/
#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif

