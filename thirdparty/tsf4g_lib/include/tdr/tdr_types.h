/**
*
* @file     tdr_types.h
* @brief    DR使用的主要类型定义
*
* @author steve jackyai
* @version 1.0
* @date 2007-03-22
*
*
* Copyright (c)  2007, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/



#ifndef TDR_TYPES_H
#define TDR_TYPES_H

#if defined(_WIN32) || defined(_WIN64)
    #if(_WIN32_WINNT >= 0x0400)
        #include <winsock2.h>
        #include <windows.h>
    #else /* _WIN32_WINNT */
        #include <winsock2.h>
        #include <windows.h>
        #include <Ws2tcpip.h>
    #endif /* _WIN32_WINNT */

    #include <tchar.h>
#else
    #include <unistd.h>
    #include <endian.h>
#endif

#include <stddef.h>
#include <time.h>
#include <sys/types.h>
#include "tdr/tdr_define.h"


#if defined(_WIN32) || defined(_WIN64)

    #if defined (LITTLEENDIAN) && LITTLEENDIAN >0
        #define TDR_OS_LITTLEENDIAN
        #if defined (TDR_OS_BIGENDIAN)
        #undef TDR_OS_BIGENDIAN
        #endif
    #else
        #define TDR_OS_BIGENDIAN
        #if defined (TOS_LITTLEENDIAN)
        #undef TDR_OS_LITTLEENDIAN
        #endif
    #endif

#else

    #if __BYTE_ORDER == __LITTLE_ENDIAN
        #define TDR_OS_LITTLEENDIAN

        #if defined (TDR_OS_BIGENDIAN)
        #undef TDR_OS_BIGENDIAN
        #endif
    #else
        #define TDR_OS_BIGENDIAN
        #if defined (TDR_OS_LITTLEENDIAN)
        #undef TDR_OS_LITTLEENDIAN
    #endif
#endif

#endif


/** @defgroup TDR_TYPE TDR_主要类型定义
 *  @{
 */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



#ifndef TDR_CUSTOM_C_TYPES
#define TDR_CUSTOM_C_TYPES


    #if !defined(_WIN32) && !defined(_WIN64)
        #include <stdint.h>
        #include <inttypes.h>

    #else /*#if !defined(_WIN32) && !defined(_WIN64)*/
        //#include <crtdefs.h>

        //The stdint declaras
        typedef signed char  int8_t;
        typedef signed short int16_t;
        typedef signed int   int32_t;
        typedef unsigned char  uint8_t;
        typedef unsigned short uint16_t;
        typedef unsigned int   uint32_t;
        #if _MSC_VER >= 1300
            typedef unsigned long long 	uint64_t;
            typedef long long 	int64_t;
        #else /* _MSC_VER */
            typedef unsigned __int64	uint64_t;
            typedef __int64	int64_t;
        #endif /* _MSC_VER */

    #endif /*#if !defined(_WIN32) && !defined(_WIN64)*/


	typedef int64_t tdr_longlong;
	typedef uint64_t tdr_ulonglong;
	typedef uint16_t tdr_wchar_t;  /**<Wchar基本数据类型*/
	typedef uint32_t tdr_date_t;	/**<data基本数据类型*/
	typedef uint32_t tdr_time_t;	/**<time基本数据类型*/
	typedef uint64_t tdr_datetime_t; /**<datetime基本数据类型*/
	typedef uint32_t tdr_ip_t;  /**<IPv4数据类型*/
#endif /*TDR_CUSTOM_C_TYPES*/



/** @name 基本数据类型的内存存储空间大小
*  @{
*/
#define TDR_SIZEOF_INT8		1	/**<8bit整数存储字节数 */
#define TDR_SIZEOF_INT16	2	/**<16bit整数存储字节数*/
#define TDR_SIZEOF_INT32	4	/**<32bit整数存储字节数*/
#define TDR_SIZEOF_INT64	8	/**<64bit整数存储字节数*/
#define TDR_SIZEOF_BYTE		TDR_SIZEOF_INT8	/**<char存储字节数 */
#define TDR_SIZEOF_SHORT	TDR_SIZEOF_INT16	/**<short类型整数的存储字节数*/
#define TDR_SIZEOF_INT	TDR_SIZEOF_INT32	/**<int类型整数的存储字节数*/
#define TDR_SIZEOF_LONGLONG	TDR_SIZEOF_INT64	/**<longlong类型存储的字节数*/
/**   @}*/

#if defined(_WIN64) || (__WORDSIZE == 64)
    //64位系统
    #define TDR_PALTFROM_ALIGN_VALUE		8	/**<平台缺省字节对齐值*/
    #define TDR_SIZEOF_POINTER 8         /**<指针变量存储的字节数*/
    #define  TDR_POINTER_TYPE_ALIGN 8	/*指针数据类型对齐方式*/
#else
    //32位系统
    #define TDR_PALTFROM_ALIGN_VALUE		4	/**<平台缺省字节对齐值*/
    #define TDR_SIZEOF_POINTER 4         /**<指针变量存储的字节数*/
    #define  TDR_POINTER_TYPE_ALIGN 4	/*指针数据类型对齐方式*/
#endif


#if (defined(_WIN32) || defined(_WIN64)) &&  _MSC_VER < 1300
#define TDR_OS_SWAP64(x) \
    ((((x) & (uint64_t)0xff00000000000000) >> 56)                                   \
    | (((x) & (uint64_t)0x00ff000000000000) >> 40)                                 \
    | (((x) & (uint64_t)0x0000ff0000000000) >> 24)                                 \
    | (((x) & (uint64_t)0x000000ff00000000) >> 8)                                  \
    | (((x) & (uint64_t)0x00000000ff000000) << 8)                                  \
    | (((x) & (uint64_t)0x0000000000ff0000) << 24)                                 \
    | (((x) & (uint64_t)0x000000000000ff00) << 40)                                 \
    | (((x) & (uint64_t)0x00000000000000ff) << 56))
#else
#define TDR_OS_SWAP64(x) \
    ((((x) & (uint64_t)0xff00000000000000LL) >> 56)                                   \
    | (((x) & (uint64_t)0x00ff000000000000LL) >> 40)                                 \
    | (((x) & (uint64_t)0x0000ff0000000000LL) >> 24)                                 \
    | (((x) & (uint64_t)0x000000ff00000000LL) >> 8)                                  \
    | (((x) & (uint64_t)0x00000000ff000000) << 8)                                  \
    | (((x) & (uint64_t)0x0000000000ff0000) << 24)                                 \
    | (((x) & (uint64_t)0x000000000000ff00) << 40)                                 \
    | (((x) & (uint64_t)0x00000000000000ff) << 56))

#endif


#define TDR_OS_SWAP32(x) \
    ((((x) & 0xff000000) >> 24)                                  \
    | (((x) & 0x00ff0000) >> 8)                                 \
    | (((x) & 0x0000ff00) << 8)                                 \
    | (((x) & 0x000000ff) << 24))

#define TDR_OS_SWAP16(x) \
    ((((x) & 0xff00) >> 8)                                  \
    | (((x) & 0x00ff) << 8))






#if (defined(_WIN32) || defined(_WIN64))  && (_MSC_VER < 1300)
typedef int intptr_t;
typedef int ptrdiff_t;
#endif

/** a offset-alike type   */
typedef intptr_t TDRPTR;

/** a index-alike type   */
typedef int TDRIDX;

/**a offset-alike type, describe the position in a meta: struct or union */
typedef ptrdiff_t TDROFF;





/**
 * This is the pointer type delcaration for TDR MetaLib.
 */
typedef struct tagTDRMetaLib	*LPTDRMETALIB;




/**
 * This is the type delcaration for TDR MetaLib param.
 */
typedef struct tagTDRLibParam	TDRLIBPARAM;

/**
 * This is the pointer type delcaration for TDR MetaLib param.
 */
typedef struct tagTDRLibParam	*LPTDRLIBPARAM;




/**
 * This is the pointer type delcaration for TDR Meta.
 */
typedef struct tagTDRMeta	*LPTDRMETA;



/**
 * This is the pointer type delcaration for TDR MetaEntry.
 */
typedef struct tagTDRMetaEntry	*LPTDRMETAENTRY;


/**
 * This is the pointer type delcaration for TDR MetaIndex.
 */
typedef struct tagTDRMetaIndex	*LPTDRMETAINDEX;


/**
 * This is the type delcaration for TDR TDRData.
 */
typedef struct tagTDRData	TDRDATA;
/**
 * This is the pointer type delcaration for TDR TDRData.
 */
typedef struct tagTDRData	*LPTDRDATA;


/**
 * This is the pointer type delcaration for TDR Macro.
 */
typedef struct tagTDRMacro		*LPTDRMACRO;


/**
 * This is the pointer type delcaration for TDR Macro.
 */
typedef struct tagTDRMacrosGroup		*LPTDRMACROSGROUP;


/**
 * This is the pointer type info for c/c++ base date type info.
 */
typedef struct tagTDRCTypeInfo	*LPTDRCTYPEINFO;

/**生成c/c++ 头文件规则结构声明
*/
typedef struct tagTDRHppRule TDRHPPRULE;
typedef struct tagTDRHppRule *LPTDRHPPRULE;

/** 生成数据库管理系统信息结构的声明
*/
typedef struct tagTDRDBMS TDRDBMS;
typedef struct tagTDRDBMS *LPTDRDBMS;


/** 生成DB处理句柄的声明
*/
typedef void * TDRDBHANDLE;
typedef TDRDBHANDLE * LPTDRDBHANDLE;

/** 生成DB处理句柄的声明
*/
typedef void * TDRDBRESULTHANDLE;
typedef TDRDBRESULTHANDLE * LPTDRDBRESULTHANDLE;

/** DB处理数据对象的声明
*/
typedef  struct tagDBObject  TDRDBOBJECT;
typedef  struct tagDBObject  *LPTDRDBOBJECT;


/**初始化元数据库的结构体
*/
struct tagTDRLibParam
{
	int iID;        /**< 元数库的ID*/

	int iTagSetVersion; /**< 元数据描述XML标签集的版本*/

	size_t iSize;		/**< 元数据的内存大小，tdr_init_lib将根据此数据分配空间*/
	size_t iMacrosGroupSize;      /**< 自定义数据类型所占的空间大小*/
	size_t iMetaSize;      /**< 自定义数据类型所占的空间大小*/
	size_t iStrBufSize;    /**< 字符串缓冲区所占的大小*/

	int iMaxMacros;     /**< 此元数据库包含的最大宏定义数*/
	int iMaxMetas;	/**< 此元数据库包含的最大结构体数*/

	int iMaxMacrosGroupNum; /*最多能容纳的宏定义组数目, Add at TDR build Version: 0x00000008*/

	int  iResFlag;     /**< 预留位，暂时使用1bit*/
	int  iVersion;     /**< 元数库的版本*/

	char szName[TDR_NAME_LEN];  /**< 元数据库名字*/

};

/**
* struct of TDR DRData
*/
struct tagTDRData
{
	char* pszBuff;          /**< 保存DR数据的缓冲区指针*/
	size_t iBuff;		/**< 保存DR数据的缓冲区大小 */
};



/**生成c/c++ 头文件规则结构
*/
struct tagTDRHppRule
{
	int iRule;	/**<生成头文件的规则*/

	/*用户自定义名字前缀 iRule设置了TDR_HPPRULE_ADD_CUSTOM_PREFIX标志位时才有意义*/
	char szCustomNamePrefix[TDR_MAX_CUSTOM_NAME_PREFIX_LEN];
};

/**数据库管理系统DBMS信息结构
*/
struct tagTDRDBMS
{
	int iMajorVersion;				/**<DBMS的主版本号*/
	int iMinVersion;				/**<DBMS的次版本号*/


	/**<与DBMS服务器端建立连接的信息，不同DBMS设置内容可能不同。对于mysql系统，此字段指定mysql服务器的主机信息*/
	int iReconnectOpt;	 /*如果因为连接丢失引起执行SQL失败时，此字段用来表示是否重建连接并重新
						 尝试执行SQL。此字段值为非零值时重建连接；否则不重建连接*/
	char szDBMSName[TDR_NAME_LEN];   /**<DBMS的名称，如：mysql,oracle,sqlserver等*/
	char szDBMSConnectionInfo[TDR_DBMS_CONNECTION_INFO_LEN];
    int  iDBMSPortNum;      /**<DBMS数据库服务器端口号*/
	char szDBMSCurDatabaseName[TDR_DBMS_DATABASE_NAME_LEN]; /**<DBMS当前数据库名*/
	char szDBMSUser[TDR_DBMS_USER_LEN];					/**<DBMS当前数据库用户名称*/
	char szDBMSPassword[TDR_DBMS_PASSWORD_LEN];			/**<DBMS用户密码*/
	char szDBMSEngine[TDR_DBMS_TABLE_ENGINE_LEN];		/**<DBMS数据库表的引擎,对于mysql可以是：ISAM,MyISAM, InnoDB*/
	char szDBMSCharset[TDR_DBMS_TABLE_CHARTSET_LEN];     /**<DBMS数据库表的字符集*/
	char szDBMSSock[TDR_DBMS_SOCK_FILE_LEN];		/*与dbms使用UNIX sock通信时所使用的文件*/
};



struct tagDBObject
{
	LPTDRMETA pstMeta;    /**<数据的元数据描述*/
	int iVersion;		/**<数据的剪裁版本*/
	size_t  iObjSize;     /**<保存数据的数据区的大小*/
	char *pszObj;     /**<保存数据的数据区首地址*/
};

typedef enum tagTDRSortMethod TDRSORTMETHOD;
typedef enum tagTDRSortMethod *LPTDRSORTMETHOD;


/**
sizeinfo/sizetype属性
*/
struct tagTDRSizeInfo
{
    TDROFF iNOff;   /**<网络传输信息偏移*/
    TDROFF iHOff;    /**<本地存储信息偏移*/
    size_t iUnitSize;
    int idxSizeType;	/*保存sizeinfo 用简单类型打包方法时使用类型，在TDR_BUILD version 5时加入*/
};
typedef struct tagTDRSizeInfo TDRSIZEINFO;
typedef struct tagTDRSizeInfo *LPTDRSIZEINFO;


/*回调函数，在遍历结构成员时，如果此成员为struct/uion类型则回调此函数，获取此子成员处理句柄
@param[in] a_pHandle 此成员父结点的处理句柄
@param[in] a_pstChildEntry 此子成员的元数据描述句柄
@param[in] a_iTotal 此子成员的数组长度
@param[in] a_Index	如果此成员所在结构体为数组，则传入其数组当前索引值，否则传 -1
@param[out] a_ppChildHandle 获取此子成员处理句柄的指针
@note 根据子成员的a_pstChildEntry句柄调用tdr_get_entry_type_meta可以获得此子成员的类型
@return 0 成功 !0 失败
*/
typedef int (*PFN_ENTER_META)(IN void *a_pHandle, IN LPTDRMETAENTRY a_pstChildEntry, IN int a_iTotal, IN int a_Index,  OUT void ** a_ppHandle);


/*回调函数，在遍历结构成员时，如果此成员为简单数据类型,则回调此函数，
@param[in] a_pHandle 此成员父结点的处理句柄
@param[in] a_pstEntry 此子成员的元数据描述句柄
@param[in] a_iCount 此成员的数组长度
@param[in] a_pszHostStart 此成员本地存储起始地址
@param[in] a_iSize 此成员存储空间可用长度
@return 0 成功 !0 失败
*/
typedef int (*PFN_VISIT_SIMPLE_ENTRY)(IN void *a_pHandle, IN LPTDRMETAENTRY a_pstEntry, IN int a_iCount,
									  IN char *a_pszHostStart, IN int a_iSize);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/** @}*/ // TDR_TYPE DR主要类型定义
#endif /* TDR_TYPES_H */
