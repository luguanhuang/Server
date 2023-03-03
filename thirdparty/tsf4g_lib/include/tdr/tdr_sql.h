/**
*
* @file     tdr_sql.h
* @brief    TSF-G-DR DB结合处理函数
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


#ifndef TSF_G_TDR_SQL_H
#define TSF_G_TDR_SQL_H

#include <stdio.h>
#include "tdr/tdr_external.h"
#include "tdr/tdr_types.h"



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/** @defgroup TDR_CREATE_TABLE TDR_DB_利用元数据描述生成数据定义语言DDL
* @{
*/

/**生成为特定DBMS所用的、针对特定元数据版本的建表语句
*生成的建表语句保存在字符串缓冲区中
*@param[in] a_pstDBMS 保护DBMS信息的指针
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstSql 存储建表语句的结构体指针
*	- 输入	a_pstSql.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstSql.iBuff	指定缓冲区的大小
*	- 输出  a_pstSql.iBuff  实际生成信息的总长度
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 目前支持的数据库管理系统DBMS：mysql
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 对于a_pstDBMS参数，只要其szDBMSName数据域设置了合理的值即可，其他数据可以不赋值
*@note 不支持指针和引用数据成员，在生成建表语句时指针和引用数据成员将被忽略
*@note 不支持union/struct数组成员到建表语句的转换，如果要存储union/struct数组，需要用一个结构体封装成单一的非数组成员
*@note 不支持wstring类型指定缺省值
*
* @pre \e a_pstDBMS 不能为NULL
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstSql 不能为 NULL
* @pre \e a_pstSql.pszBuff 不能为 NULL
* @pre \e a_pstSql.iBuff 必须大于0

*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_create_table(IN LPTDRDBMS a_pstDBMS, IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstSql, IN int a_iVersion);

/**生成为特定DBMS所用的、针对特定元数据版本的建表语句
*生成的建表语句保存在指定文件中
*@param[in] a_pstDBMS 保护DBMS信息的指针
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_fpSql 存储建表语句的文件句柄
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 目前支持的数据库管理系统DBMS：mysql
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 对于a_pstDBMS参数，只要其szDBMSName数据域设置了合理的值即可，其他数据可以不赋值
*@note 不支持指针和引用数据成员，在生成建表语句时指针和引用数据成员将被忽略
*@note 不支持union/struct数组成员到建表语句的转换，如果要存储union/struct数组，需要用一个结构体封装成单一的非数组成员
*@note 不支持wstring类型指定缺省值
*
* @pre \e a_pstDBMS 不能为NULL
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_fpSql 不能为 NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_create_table_fp(IN LPTDRDBMS a_pstDBMS, IN LPTDRMETA a_pstMeta, IN FILE *a_fpSql, IN int a_iVersion);

/**生成为特定DBMS所用的、针对特定元数据版本的建表语句
*生成的建表语句保存在指定文件中
*@param[in] a_pstDBMS 保护DBMS信息的指针
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_pszSqlFile 存储建表语句的文件名指针
*@param[in] a_iVersion	要剪裁的版本号
*
*@note 目前支持的数据库管理系统DBMS：mysql
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 对于a_pstDBMS参数，只要其szDBMSName数据域设置了合理的值即可，其他数据可以不赋值
*@note 不支持指针和引用数据成员，在生成建表语句时指针和引用数据成员将被忽略
*@note 不支持union/struct数组成员到建表语句的转换，如果要存储union/struct数组，需要用一个结构体封装成单一的非数组成员
*@note 不支持wstring类型指定缺省值
*
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pszSqlFile 不能为 NULL
* @pre \e a_pstDBMS 不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_create_table_file(IN LPTDRDBMS a_pstDBMS, IN LPTDRMETA a_pstMeta, IN const char *a_pszSqlFile, IN int a_iVersion);

/**生成为特定DBMS所用的、针对从特定元数据版本到当前最新版本的表修改语句
*生成的建表语句保存在字符串缓冲区中
*@param[in] a_pstDBMS 保护DBMS信息的指针
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in,out]	a_pstSql 存储建表语句的结构体指针
*	- 输入	a_pstSql.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstSql.iBuff	指定缓冲区的大小
*	- 输出  a_pstSql.iBuff  实际生成信息的总长度
*@param[in] a_iVersion	特定元数据版本
*
*@note 对于a_pstDBMS参数，只要其szDBMSName数据域设置了合理的值即可，其他数据可以不赋值
*@note 目前支持的数据库管理系统DBMS：mysql
*@note 如果a_iVersion大于等于元数据的最高版本，则直接返回
*@note 目前本接口只支持:向原有数据表中添加数据列,修改主键，unique键
*@note 如果a_iVersion为0 ，则安装剪裁到最高版本
*@note 不支持指针和引用数据成员，在生成建表语句时指针和引用数据成员将被忽略
*@note 不支持union/struct数组成员到建表语句的转换，如果要存储union/struct数组，需要用一个结构体封装成单一的非数组成员
*@note 不支持wstring类型指定缺省值
*
* @pre \e 基于a_iVersion版本的元数据描述对应的数据库表已经存在
* @pre \e a_pstDBMS 不能为NULL
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pstSql 不能为 NULL
* @pre \e a_pstSql.pszBuff 不能为 NULL
* @pre \e a_pstSql.iBuff 必须大于0

*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_alter_table(IN LPTDRDBMS a_pstDBMS, IN LPTDRMETA a_pstMeta, INOUT LPTDRDATA a_pstSql, IN int a_iVersion);

/**生成为特定DBMS所用的、针对从特定元数据版本到当前最新版本的表修改语句
*生成的建表语句保存在指定文件中
*@param[in] a_pstDBMS 保护DBMS信息的指针
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_fpSql 存储建表语句的文件句柄
*@param[in] a_iVersion	特定元数据版本
*
*@note 对于a_pstDBMS参数，只要其szDBMSName数据域设置了合理的值即可，其他数据可以不赋值
*@note 目前支持的数据库管理系统DBMS：mysql
*@note 如果a_iVersion大于等于元数据的最高版本，则直接返回
*@note 目前本接口只支持:向原有数据表中添加数据列,修改主键，unique键
*@note 不支持指针和引用数据成员，在生成建表语句时指针和引用数据成员将被忽略
*@note 不支持union/struct数组成员到建表语句的转换，如果要存储union/struct数组，需要用一个结构体封装成单一的非数组成员
*@note 不支持wstring类型指定缺省值
*
* @pre \e 基于a_iVersion版本的元数据描述对应的数据库表已经存在
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_fpSql 不能为 NULL
* @pre \e a_pstDBMS 不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_alter_table_fp(IN LPTDRDBMS a_pstDBMS, IN LPTDRMETA a_pstMeta, IN FILE *a_fpSql, IN int a_iVersion);

/**生成为特定DBMS所用的、针对从特定元数据版本到当前最新版本的表修改语句
*生成的建表语句保存在指定文件中
*@param[in] a_pstDBMS 保护DBMS信息的指针
*@param[in]	a_pstMeta 元数据描述的指针，通过tdr_get_meta_by_name和tdr_get_meta_by_id可以从元数据库中获取到此指针
*@param[in]	a_pszSqlFile 存储建表语句的文件名指针
*@param[in] a_iVersion	特定元数据版本
*
*@note 对于a_pstDBMS参数，只要其szDBMSName数据域设置了合理的值即可，其他数据可以不赋值
*@note 目前支持的数据库管理系统DBMS：mysql
*@note 如果a_iVersion大于等于元数据的最高版本，则直接返回
*@note 目前本接口只支持:向原有数据表中添加数据列,修改主键，unique键
*@note 不支持指针和引用数据成员，在生成建表语句时指针和引用数据成员将被忽略
*@note 不支持union/struct数组成员到建表语句的转换，如果要存储union/struct数组，需要用一个结构体封装成单一的非数组成员
*@note 不支持wstring类型指定缺省值
*
* @pre \e 基于a_iVersion版本的元数据描述对应的数据库表已经存在
* @pre \e a_pstMeta 不能为 NULL
* @pre \e a_pszSqlFile 不能为 NULL
* @pre \e a_pstDBMS 不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*		- TDR_ERROR_INVALID_CUTOFF_VERSION 指定剪裁版本比元数据基准版本要小
*
*
* @see	tdr_get_meta_by_name
* @see	tdr_get_meta_by_id
*/
TDR_API int tdr_alter_table_file(IN LPTDRDBMS a_pstDBMS, IN LPTDRMETA a_pstMeta, IN const char *a_pszSqlFile, IN int a_iVersion);


/** @}*/ /*defgroup TDR_CREATE_TABLE TDR_DB_利用元数据描述生成数据定义语言DDL*/



/** @defgroup TDR_ORMAPPING TDR_DB_对象关系映射
* @{
*/

/**根据DBMS的信息打开DB处理句柄
*@param[out] a_phDBHandle 保存DBMS处理句柄的指针
*@param[in]	a_pstDBMS 保存DBMSxt信息的结构的指针
*@param[in] a_pszErrMsg 保存错误信息的缓冲区指针，如果此参数不为NULL，则如果打开句柄失败时，可通过此参数获取出错信息
*
*@note 通过本接口打开的句柄，必须调用tdr_close_dbhandle接口关闭句柄，否则会有内存泄漏
*@note 本接口创建的句柄只能在同一个线程内使用，即多个线程不能共享同一个协议处理句柄
*
* @pre \e a_phDBHandle 不能为 NULL
* @pre \e a_pstDBMS 不能为 NULL
* @pre \e a_pszErrMsg为NULL时，则不输出错误信息，错误消息对应的buff大小建议配置为1024字节，配置太小可能会有越界的风险
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_NO_MEMORY	创建句柄时内存不够
*	- TDR_ERROR_DB_FAILD_TO_CONNECT_SERVER	连接数据库服务器失败
*
* @see	tdr_close_dbhandle
*/
TDR_API int tdr_open_dbhanlde(OUT TDRDBHANDLE *a_phDBHandle, IN LPTDRDBMS a_pstDBMS, IN char *a_pszErrMsg);


/**根据DBMS的信息创建DB处理句柄
*@param[out] a_phDBHandle 保存DBMS处理句柄的指针
*@param[in]	a_pstDBMS 保存DBMS信息的结构的指针
*@param[in] a_pszErrMsg 保存错误信息的缓冲区指针，如果此参数不为NULL，则如果打开句柄失败时，可通过此参数获取出错信息
*@param[in] a_iSize 保存错误信息的缓冲区大小
*
*@note 通过本接口创建的句柄，必须调用tdr_close_dbhandle接口关闭句柄，否则会有内存泄漏
*@note 本接口创建的句柄只能在同一个线程内使用，即多个线程不能共享同一个协议处理句柄
*@note 本接口创建的句柄默认设置MYSQL_OPT_READ_TIMEOUT和MYSQL_OPT_WRITE_TIMEOUT值为三分钟
*
* @pre \e a_phDBHandle 不能为 NULL
* @pre \e a_pstDBMS 不能为 NULL
* @pre \e a_pszErrMsg为NULL时，则不输出错误信息
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_NO_MEMORY	创建句柄时内存不够
*
* @see	tdr_close_dbhandle
*/
TDR_API int tdr_new_dbhanlde(OUT TDRDBHANDLE *a_phDBHandle, IN LPTDRDBMS a_pstDBMS, INOUT char *a_pszErrMsg, IN size_t a_iSize);

/**为DBMS处理句柄设置mysql选项MYSQL_OPT_CONNECT_TIMEOUT
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_dwTimeOut 连接超时时间值
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
*
* @note 这里选项设置仅仅在TDR层面上，只有在调用tdr_connect_db后才会在MYSQL层面真正生效
*
* @see tdr_connect_db
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
TDR_API int tdr_db_opt_connect_timeout(IN TDRDBHANDLE a_hDBHandle, IN unsigned int a_dwTimeOut);

/**为DBMS处理句柄设置mysql选项MYSQL_OPT_READ_TIMEOUT
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_dwTimeOut 读超时时间值
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
*
* @note 这里选项设置仅仅在TDR层面上，只有在调用tdr_connect_db后才会在MYSQL层面真正生效
*
* @see tdr_connect_db
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
TDR_API int tdr_db_opt_read_timeout(IN TDRDBHANDLE a_hDBHandle, IN unsigned int a_dwTimeOut);

/**为DBMS处理句柄设置mysql选项MYSQL_OPT_WRITE_TIMEOUT
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_dwTimeOut 写超时时间值
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
*
* @note 这里选项设置仅仅在TDR层面上，只有在调用tdr_connect_db后才会在MYSQL层面真正生效
*
* @see tdr_connect_db
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
TDR_API int tdr_db_opt_write_timeout(IN TDRDBHANDLE a_hDBHandle, IN unsigned int a_dwTimeOut);

/**为DBMS处理句柄设置mysql选项MYSQL_OPT_RECONNECT
 * DBA强烈建议开发者不要开启重连选项，由业务自身实现重连逻辑
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_chBool 重连选项值
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
*
* @note 这里选项设置仅仅在TDR层面上，只有在调用tdr_connect_db后才会在MYSQL层面真正生效
*
* @see tdr_connect_db
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
TDR_API int tdr_db_opt_reconnect(IN TDRDBHANDLE a_hDBHandle, IN char a_chBool);

/**为DBMS处理句柄在进行mysql_real_connect时设置标志位CLIENT_FOUND_ROWS
 * 如果希望UPDATE语句affected-nums值为匹配到的（found），而不是实际改变的（changed）
 * 请使用该接口设置非0选项值，业务根据自身需要来设置此选项
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_chBool update affected-nums 是否开启CLIENT_FOUND_ROWS，详情请参考mysql手册
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
*
* @note 这里选项设置仅仅在TDR层面上，只有在调用tdr_connect_db后才会在MYSQL层面真正生效
*
* @see tdr_connect_db
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
TDR_API int tdr_db_opt_foundrows(IN TDRDBHANDLE a_hDBHandle, IN char a_chBool);

/**设置数据压缩时需要的缓冲区大小
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_dwSize 需要设定的缓冲区大小，如果小于1M，则使用1M
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
*
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*/
TDR_API int tdr_db_opt_compress_buffer(IN TDRDBHANDLE a_hDBHandle, IN uint32_t a_dwSize);


/**根据DBMS处理句柄连接DBMS
*@param[in] a_hDBHandle DBMS处理句柄
*@param[in] a_pszErrMsg 保存错误信息的缓冲区指针，如果此参数不为NULL，则如果打开句柄失败时，可通过此参数获取出错信息
*@param[in] a_iSize 保存错误信息的缓冲区大小
*
* @pre \e a_hDBHandle 不能为 NULL，需要注意变量类型为TDRDBHANDLE
* @pre \e a_pszErrMsg为NULL时，不输出错误信息
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_SET_DB_OPTIONS	设置数据库选项错误
*	- TDR_ERROR_DB_FAILD_TO_CONNECT_SERVER	连接数据库服务器失败
*/
TDR_API int tdr_connect_db(IN TDRDBHANDLE a_hDBHandle, INOUT char *a_pszErrMsg, IN size_t a_iSize);

/** 根据连接句柄，做一些检查逻辑
* 1)如果连接断了，则重建连接
*@param[out] a_hDBHandle 保护DBMS处理句柄的指针
*
* @pre \e a_hDBHandle 不能为 NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_DB_FAILD_TO_CONNECT_SERVER	连接数据库服务器失败
*
* @see	tdr_open_dbhanlde
*/
int tdr_keep_dbmsconnection(IN TDRDBHANDLE a_hDBHandle);


/**关闭DB处理句柄
*@param[in,out] a_phDBHandle 保护DBMS处理句柄
*
*/
TDR_API void tdr_close_dbhanlde(INOUT LPTDRDBHANDLE a_phDBHandle);

/**利用元数据描述生成数据操作语句DML
*@param[in] a_hDBHandle 保护DBMS处理句柄
*@param[in,out] a_pstSql 保存SQL语句的缓冲区结构指针
*	- 输入	a_pstSql.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstSql.iBuff	指定缓冲区的大小
*	- 输出  a_pstSql.iBuff  实际生成信息的总长度
*@param[in] a_iOPType DB操作的（Insert、Update、Delete和Select）ID,具体值的定义请参看TDRDBOPID定义
*@param[in,out] a_pstObj 保存操作数据的结构体指针
*	- 输入	a_pstObj.pstMeta 指定此数据的元数据结构指针
*	- 输入  a_pstObj.iVersion	指定此数据的剪裁版本
*	- 输入  a_pstObj.iObjSize  指定此数据的大小
*	- 输入  a_pstObj.pszObj  指定此数据的缓冲区指针
*	- 输出  a_pstObj.iObjSize  此数据实际被处理的大小
*@param[in] a_pszWhere 自定义where条件，如果此参数为NULL或空串，则将利用主键元素生成where条件
*
*@note 如果a_pstObj.iVersion为0，则将按最高版本进行剪裁
*@note 如果a_pszWhere为NULL或空串，则将利用主键元素生成where条件,对于update,select,delete操作组成主键的成员
*必须已经赋予了有效的值
*@note update操作语句SET部分不会包含更新主键列的表达式
*
*@note insert语句的格式： INSERT   INTO tbl_name (col_name,...) VALUES (col_val,...)
*@note update语句的格式： UPDATE  tbl_name  SET col_name1=expr1 [, col_name2=expr2 ...]    WHERE where_definition
*@note select语句的格式： SELECT col_name,... FROM  tbl_name  WHERE where_definition；
*@note delete语句的格式： DELETE FROM tbl_name WHERE where_definition
*
*@note 如果处理失败，获取错误信息的方法:
	*	- 根据返回值，调用tdr_error_string()可以获取出错信息
    *	- 通过打印出a_pstSql.pszBuff的内容，可以辅助性查看出错时的位置
*
* @pre \e a_hDBHandle 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstSql 不能为 NULL
* @pre \e a_pstSql.pszBuff 不能为 NULL
* @pre \e a_pstSql.iBuff 必须大于0
* @pre \e a_pstObj 不能为NULL
*@pre \e a_pstObj->pstMeta 不能为NULL
* @pre \e a_pstObj->pszObj 不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_INVALID_CUTOFF_VERSION	剪裁版本不正确
*	- TDR_ERROR_NO_HOSTBUFF_SPACE 保存SQL语句的缓冲区空间不够
*	- TDR_ERROR_DB_NO_PRIMARYKEY	根据主键生成WHERE条件失败，可能元数据没有指定主键属性，也可能该剪裁版本破坏了主键信息的完整性
*
* @note 如果该接口返回失败，可以调用 tdr_get_error_detail 取得详细的错误信息
*
*@see TDRDBOPID
*@see tdr_error_string
*@see tdr_get_error_detail
*/
TDR_API int tdr_obj2sql(IN TDRDBHANDLE a_hDBHandle, INOUT LPTDRDATA a_pstSql, IN int a_iOPType,
				INOUT  LPTDRDBOBJECT a_pstObj, IN const char *a_pszWhere);

/**执行SQL语句，如果SQL查询语句会返回结果集，则将该结果集检索到本地，并通过参数将结果句柄返回
*@param[out] a_phDBResult 保护DBMS处理结果句柄的指针
*@param[in] a_hDBHandle 保护DBMS处理句柄
*@param[in] a_pstSql 保存SQL语句的缓冲区结构指针
*	- 输入	a_pstSql.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstSql.iBuff	指定缓冲区的大小
*
*@note 一旦完成了对结果集的操作，必须调用tdr_free_dbresult释放结果句柄,以释放资源
* @pre \e a_phDBResult 不能为NULL
* @pre \e a_hDBHandle 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstSql 不能为 NULL
* @pre \e a_pstSql.iBuff 必须大于0
* @pre \e a_pstObj 不能为NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*
*@see tdr_free_dbresult
*@see tdr_dbms_errno
*/
TDR_API int tdr_query(INOUT TDRDBRESULTHANDLE *a_phDBResult, IN TDRDBHANDLE a_hDBHandle, IN LPTDRDATA a_pstSql);

/**执行SQL语句，如果该SQL查询语句会返回结果集，则只执行初始化工作，并不取回任何数据行
*通过参数返回将结果句柄
*@param[out] a_phDBResult 保护DBMS处理结果句柄的指针
*@param[in] a_hDBHandle 保护DBMS处理句柄
*@param[in] a_pstSql 保存SQL语句的缓冲区结构指针
*	- 输入	a_pstSql.pszBuff 指定保存本地结构信息的缓冲区起始地址
*	- 输入  a_pstSql.iBuff	指定缓冲区的大小
*
*@note 一旦完成了对结果集的操作，必须调用tdr_free_dbresult释放结果句柄,以释放资源
*@note 本接口不像tdr_query()那样将结果集实际读取到客户端，真正取回数据行的工作通过一系列的tdr_fetch_now调用完成，
* 因此tdr_query_quick执行速度更快而且使用的内存也更少。但是tdr_query_quick加重了数据库服务器的负担，服务器必须把结果集中
*的数据保存到客户端程序把它们都检索到客户端为止。因此tdr_query_quick不适合在以下几种客户端程序中调用：
*	- 根据用户请求逐个遍历各有关数据的交互式客户端程序
*	- 在前，后两次数据行检索操作之间需要大量处理的客户端程序
*
* @pre \e a_phDBResult 不能为NULL
* @pre \e a_hDBHandle 必须是通过tdr_open_dbhanlde打开的句柄
* @pre \e a_pstSql 不能为 NULL
* @pre \e a_pstSql.iBuff 必须大于0
* @pre \e a_pstObj 不能为NULL
*
*@retval 0   处理成功
*@retval <0  处理失败，返回表示出错信息的错误代码：
*
*@see tdr_free_dbresult
*@see tdr_dbms_errno
*/
TDR_API int tdr_query_quick(INOUT TDRDBRESULTHANDLE *a_phDBResult, IN TDRDBHANDLE a_hDBHandle, IN LPTDRDATA a_pstSql);


/**获取结果集中的记录数
*@param[in] a_hDBResult 保存结果集的句柄
*
*@note tdr_num_rows()的使用取决于是否采用了tdr_query()或tdr_query_quick()来返回结果集。
*如果使用了tdr_query()，可以立刻调用tdr_num_rows()；如果使用了tdr_query_quick()，tdr_num_rows()不返回正确的值，直至检索了结果集中的所有行为止。
*
*@return 结果集中的行数
*@pre \e a_hDBResult 不能为NULL
*/
TDR_API unsigned long tdr_num_rows(IN TDRDBRESULTHANDLE a_hDBResult);


/**返回上次UPDATE更改的行数，上次DELETE删除的行数，或上次INSERT语句插入的行数
*对于UPDATE、DELETE或INSERT语句，可在tdr_query()后立刻调用。
*对于SELECT语句，tdr_affected_row()的工作方式与tdr_num_rows()类似
*
*@param[in] a_hDBResult 保存结果集的句柄
*
*@note tdr_affected_rows()的使用取决于是否采用了tdr_query()或tdr_query_quick()来返回结果集。
*如果使用了tdr_query()，可以立刻调用tdr_affected_rows()；如果使用了tdr_query_quick()，tdr_affected_rows()不返回正确的值，直至检索了结果集中的所有行为止。
*
*@return 大于0的整数表明受影响或检索的行数。
* - 0	表示UPDATE语句未更新记录，在查询中没有与WHERE匹配的行，或未执行查询。
* - -1 表示查询返回错误，或者，对于SELECT查询，在调用tdr_query()之前调用了tdr_affected_rows()。
**@pre \e a_hDBResult 不能为NULL
*@see tdr_num_rows
*/
TDR_API long tdr_affected_rows(IN TDRDBRESULTHANDLE a_hDBResult);

/** 根据元数据描述检索结果集的下一个数据，并按照指定剪裁版本将数据读取到指定缓冲区中
*@param[in] a_hDBResult 保存DB操作结果集的句柄
*@param[in,out] a_pstObj 保存检索到的数据的结构体指针
*	- 输入	a_pstObj.pstMeta 指定此数据的元数据结构指针
*	- 输入  a_pstObj.iVersion	指定此数据的剪裁版本
*	- 输入  a_pstObj.iObjSize  指定保存此数据缓冲区的大小
*	- 输入  a_pstObj.pszObj  指定保存此数据缓冲区的指针
*	- 输出  a_pstObj.iObjSize  此数据实际的大小
*
*@note 如果a_pstObj.iVersion为0，则按照当前最高版本进行剪裁
*@note 如果某个数据域的空间不够将对检索出的数据进行截断处理
*@note 如果结构的某个成员在数据记录中为NULL，则将此数据域设置为缺省值
*
* @pre \e a_hDBResult 必须是通过tdr_query或tdr_query_quick获取的结果句柄
* @pre \e a_pstObj 不能为NULL
* @pre \e	a_pstObj.pstMeta 不能为NULL
* @pre \e   a_pstObj.iObjSize必须大于或等于整个元数据所需的存取空间
* @pre \e	a_pstObj.pszObj  不能为NULL
*
*@retval 0   处理成功
*@retval >0  处理成功，但发生某些特殊情况：
*	- TDR_SUCWARN_TRUNCATE_DATE 保存数据时进行了数据截断
*@retval <0  处理失败，返回表示出错信息的错误代码：
*	- TDR_ERROR_INVALID_CUTOFF_VERSION	剪裁版本不正确
*	- TDR_ERROR_DB_NO_RESULT_SET 结果集为空，请确定此接口在tdr_query或tdr_query_quick之后使用
*	- TDR_ERROR_DB_NO_RECORD_IN_RESULTSET 没有更多要检索的行或出现了错误(在dr_query_quick之后使用)
*	- TDR_ERROR_DB_NO_EXPECTED_FIELD 特定成员没有出现在数据记录中，请确定
*	- TDR_ERROR_INVALID_REFER_VALUE	元素的refer属性值不正确
*	- TDR_ERROR_DB_UNSUPPORTED_COMPOSITE_ARRAY 不支持struct数组的存取
*	- TDR_ERROR_TOO_COMPLIEX_META	元数据复合层次太深
*	- TDR_ERROR_NO_HOSTBUFF_SPACE	保存数据的缓冲区不够
*/
TDR_API int tdr_fetch_row(IN  TDRDBRESULTHANDLE a_hDBResult, INOUT  LPTDRDBOBJECT a_pstObj);

/**释放结果句柄
*@param[in] a_phDBResult 保护DBMS处理结果句柄的指针
*@see tdr_free_dbresult
*/
TDR_API void tdr_free_dbresult(IN TDRDBRESULTHANDLE *a_phDBResult);

/**获取DBMS底层操作的错误代码
*@param[in] a_hDBHandle 保护DBMS处理句柄
*
*@note tdr_query等操作执行失败时，可以获取DBMS层的错误信息
*
*@return 如果失败，返回上次mysql_xxx()调用的错误代码。“0”表示未出现错误。
*@pre \e a_hDBHandle 不能为NULL
*/
TDR_API int tdr_dbms_errno(IN TDRDBHANDLE a_hDBHandle);

/**获取DBMS底层操作的错误信息
*@param[in] a_hDBHandle 保护DBMS处理句柄
*
*@note tdr_query等操作执行失败时，可以获取DBMS层的错误信息
*
*@return 返回描述错误的、由Null终结的字符串。如果未出现错误，返回空字符串。
*@pre \e a_hDBHandle 不能为NULL
*/
TDR_API const char *tdr_dbms_error(IN TDRDBHANDLE a_hDBHandle);


/**返回此处理句柄上最进一次执行数据库查询命令所生成的AUTO_INCREMENT值，在包含AUTO_INCREMENT字段的表
*中执行了INSERT语句后，应使用该函数获取包含AUTO_INCREMENT列的值。

*@param[in] a_hDBHandle 保护DBMS处理句柄
*
*@note 如果前面的语句未使用AUTO_INCREMENT，mysql_insert_id()返回0。如果需要保存值，在生成值的语句后，
务必立刻调用tdr_dbms_insert_id()
*@pre \e a_hDBHandle 不能为NULL
*
*@return 前面已经内容已经介绍
*/
TDR_API tdr_ulonglong tdr_dbms_insert_id(IN TDRDBHANDLE a_hDBHandle);

/**获取数据表中配置指定查询条件的记录总数
*@param[in] a_hDBHandle  保护DBMS处理句柄
*@param[in] a_pszTableName  保存数据库表名的字符串指针
*@param[in] a_pszWhereDef 查询条件缓冲区指针
*@param[out] a_pdwCount  获取总记录条数
*
*@pre \e a_hDBHandle不能为NULL
*@pre \e a_pszTableName不能为NULL
*@pre \e a_pdwCount 不能为NULL
*@note	如果a_pszWhereDef为NULL，则本接口将查询出数据库表中记录行的总数，
*	即相当于指定条件"where 1"
*
*@return 0: success,  !0: failed
*@pre \e a_hDBHandle 不能为NULL

*/
TDR_API int  tdr_get_records_count(IN TDRDBHANDLE a_hDBHandle, IN const char *a_pszTableName,
								   IN const char *a_pszWhereDef, OUT unsigned int *a_pdwCount);



/** @}*/ /*defgroup TDR_CREATE_TABLE TDR_DB_对象关系映射*/
#ifdef __cplusplus
}
#endif


#endif /* TSF_G_TDR_SQL_H */
