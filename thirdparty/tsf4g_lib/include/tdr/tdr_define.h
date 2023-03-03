/**
*
* @file     tdr_define.h
* @brief    TDR常量定义
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

#ifndef TDR_DEFINE_H
#define TDR_DEFINE_H


/** @name 函数参数输入/输出属性
 *  @{
 */
/*表示该参数只是输入参数*/
#ifndef IN
#define IN
#endif


/*表示该参数只是输出参数*/
#ifndef OUT
#define OUT
#endif


/*表示该参数既是输入参数，又是输出参数*/
#ifndef INOUT
#define INOUT
#endif

/** @}*/ // 函数参数输入/输出属性

/**
*bool型变量取值定义
*/
typedef enum tagBoolean
{
	TDR_FALSE = 0,	    /**< false*/
	TDR_TRUE			/**< true*/
}TDRBOOLEAN;

typedef enum tagCompatibleBoolean
{
    TDR_XML_EQUIVALENT = 0,     /**< equivalent*/
	TDR_XML_COMPATIBLE = 1,	    /**< compatible*/
	TDR_XML_UNCOMPATIBLE	    /**< uncompatible*/
}TDRCOMPATIBLEBOOLEAN;


/** @name 元数据描述常量
 *  @{
 */
#define TDR_MAGIC		0x02D6      /**< 元数据二进制文件中的"魔数"*/


#define	TDR_MAX_VERSION		0x7fffffff		/**< 元数据库最大版本号*/

#define TDR_NAME_LEN		128		/**< 元数据名字串最大字节数*/
#define TDR_DESC_LEN		1024		/**< 元数据描述串最大字节数*/
#define TDR_MACRO_LEN	128		/**< 元数据宏定义名字串最大字节数*/
#define TDR_CHNAME_LEN	512		/**< 元数据中文名串最大字节数*/
#define TDR_DEFAULT_VALUE_LEN	1024	/**< 元数据缺省值最大字节数*/
/** @}*/ // 元数据描述常量

#define TDR_MAX_FILES_IN_ONE_PARSE		512   /**< 一次解析的最大XML文件数*/





/** @name 支持的元数据XML标签集的版本信息
 *  @{
 */
#define TDR_XML_TAGSET_VERSION_0             0      /**< 第0版本*/
#define TDR_SUPPORTING_MIN_XMLTAGSET_VERSION	TDR_XML_TAGSET_VERSION_0       /**< 支持的最少版本号*/
#define TDR_XML_TAGSET_VERSION_1             1      /**< 第1版本*/
#define TDR_SUPPORTING_MAX_XMLTAGSET_VERSION	TDR_XML_TAGSET_VERSION_1      /**< 支持的最大版本号*/
/** @}*/ // 支持的元数据XML标签集的版本信息






/** @name 元数据生成c语言头文件规则(各规则可以通过‘与’的方式组合)
*  @{
*/
/**成员名命名规则缺省规则：添加类型前缀，如果没有类型前缀则成员名首字母变成小写。
*例如某成员在DR中的名字属性值为"name",其类型为"string",则在c语言头文件中名字为:
*	szName
*/
#define TDR_HPPRULE_DEFAULT   0x0000

#define TDR_HPPRULE_NO_TYPE_PREFIX	0x0001	/**<成员名命名规则：不添加类型前缀*/

/**成员名命名规则：添加自定义前缀规则：例如某成员在DR中的名字属性值为"name",其类型为"string",自定义前缀为"m_"
*则在c语言头文件中名字为:	char*	m_Name
*/
#define TDR_HPPRULE_ADD_CUSTOM_PREFIX 0x0002

#define TDR_HPPRULE_NO_LOWERCASE_PREFIX	0x0004	/**<成员名命名规则：不强制将成名首字母变成小写*/

#define TDR_HPPRULE_NO_TYPE_DECLARE		0x0008 /**<头文件中不添加类型前缀*/

#define TDR_HPPRULE_CUSTOMED_PREFIX     0x0010 /**<使用前缀配置文件*/
/** @ }*/

#define TDR_MAX_CUSTOM_NAME_PREFIX_LEN	9		/**<用户自定义名字前缀的最大长度*/

#define TDR_MAX_CUMTOMATTR_VALUE_LEN	2048	/**<数据成员用户自定义属性值最大长度*/

/**@name IO属性值
*@ {*/
#define TDR_IO_NOLIMIT		0x00  /**<表示其IO输出/输出没有限制*/
#define TDR_IO_NOINPUT		0x01    /**<表示此成员不能输入*/
#define TDR_IO_NOOUTPUT		0x02    /**<表示此成员不能输出*/
#define TDR_IO_NOIO		0x03    /**< 表示此成员不参与输入/输出*/
#define TDR_IO_MIN_VALUE      TDR_IO_NOLIMIT
#define TDR_IO_MAX_VALUE TDR_IO_NOIO
/*@ }*/

/** @name 输入输出XML格式
*  @{
*/

enum tagTDRXmlDataFormat
{
	/**
	此数据文件的特征是：结构体成员数据都作为子节点，对于复合数据类型的成员，
	子节点以结构体名进行命名
	*@deprecated 目前已不推荐使用，推荐使用以下定义的xml数据格式
	*/
	TDR_XML_DATA_FORMAT_LIST_MATA_NAME =1,

	/**
	此数据文件的特征是：结构体成员数据都作为子节点，对于复合数据类型的成员，
	子节点以成员名进行命名
	*/
	TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME = 2,

	/**
	此数据文件的特征是：对于符合数据类型的成员，子节点以成员名进行命名
	结构体成员数据简单非数据成员以属性的方式存储，其他成员数据以子节点的方式存储，
	*/
	TDR_XML_DATA_FORMAT_ATTR_ENTRY_NAME = 3,
};


/**<TDR_XML_DATA_FORMAT_LIST_MATA_NAME宏定义指定的xml数据格式*/
#define TDR_IO_OLD_XML_VERSION TDR_XML_DATA_FORMAT_LIST_MATA_NAME

/**<TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME指定的xml数据格式*/
#define TDR_IO_NEW_XML_VERSION TDR_XML_DATA_FORMAT_LIST_ENTRY_NAME


/** @} */

/**@name 排序方法
*@ {*/
enum tagTDRSortMethod
{
	TDR_SORTMETHOD_NONE_SORT =	0x00,  /**< 不排序*/
	TDR_SORTMETHOD_ASC_SORT	=	0x01,    /**<升序排序*/
	TDR_SORTMETHOD_DSC_SORT =0x02,    /**<降序排序*/
};
/*@ }*/

/**@name DBMS相关宏定义
*@ {*/
#define TDR_DBMS_NAME_LEN					32	/**<DBMS名称最大长度*/
#define TDR_DBMS_CONNECTION_INFO_LEN		256  /**< DBMS连接信息的最大长度*/
#define TDR_DBMS_DATABASE_NAME_LEN			64	/**<DBMS数据库名称最大长度*/
#define TDR_DBMS_USER_LEN					256 /**<DBMS用户名称最大长度*/
#define TDR_DBMS_PASSWORD_LEN				64  /**<DBMS用户密码最大长度*/
#define TDR_DBMS_TABLE_ENGINE_LEN			64  /**<DBMS数据库表的引擎名字串的长度*/
#define TDR_DBMS_TABLE_CHARTSET_LEN			64  /**<DBMS数据库表的字符集名字串的长度*/
#define TDR_MYSQL_DEFAULT_TABLE_ENGINE	"InnoDB"  /**<缺省使用InnoDB作为数据库表的引擎*/
#define TDR_MYSQL_DEFAULT_TABLE_CHARTSET	"latin1"  /**<缺省使用latin1作为数据库表的字符集*/
#define TDR_DBMS_MYSQL			"mysql"		/*MYSQL 数据库管理系统的名字*/
#define TDR_DEFAULT_DBMS		TDR_DBMS_MYSQL		/*tdr缺省支持的数据库管理系统*/
#define TDR_DBMS_SOCK_FILE_LEN	256		/*与dbms使用UNIX sock通信时所使用的文件长度*/
/*@ }*/


/** @name DB操作ID
*  @{
*/
typedef enum
{
	TDR_DBOP_NONE = 0,		/**<无操作*/
	TDR_DBOP_INSERT,		/**<插入操作*/
	TDR_DBOP_UPDATE,		/**<更新操作*/
	TDR_DBOP_DELETE,		/**<删除操作*/
	TDR_DBOP_SELECT,		/**<查询操作*/
    TDR_DBOP_REPLACE,       /**<更新操作*/
}TDRDBOPID;


/*const of type */
#define  TDR_INVALID_INDEX		-1
#define  TDR_INVALID_PTR                -1
#define  TDR_INVALID_OFFSET             -1

#define TDR_INVALID_ID     -1           /**< 无效ID值*/

#define TDR_INVALID_VERSION -1          /**< 无效版本号*/

#endif /* TDR_DEFINE_H */
