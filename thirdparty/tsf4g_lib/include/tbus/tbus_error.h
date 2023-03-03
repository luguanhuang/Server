/**
*
* @file     tbus_error.h
* @brief    TBUS错误处理函数
*
* @author jackyai
* @version 1.0
* @date 2008-09-12
*
*
* Copyright (c)  2008, 腾讯科技有限公司互动娱乐研发部
* All rights reserved.
*
*/


#ifndef TBUS_ERROR_H
#define TBUS_ERROR_H

#include "pal/pal.h"

/** @defgroup TBUS_ERR TBUS_错误处理
* @{
*@note TBUS错误处理的基本思想:
*  - 使用一个整数来存储错误代码信息，此整数值的含义如下:
*      -# <0 表示发生错误，其值为具体错误的代码
*      -# 0 表示成功
*      -# >0 表示成功，但发生了某些特殊事情
*  - 错误代码存储在4字节的整数中，由三部分组成:
*      -# 错误级别: 1个字节，具体为高位的第1个字节
*      -# 模块ID:  1字节，具体为高位的第2个字节
*      -# 错误号:  2字节，0-1023号表示通用错误，各模块定义错误从1024开始
*
*/



#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef TMODID_BUS
#define TMODID_BUS  0x01
#endif

#define TBUS_ERR_MAKE(level, mod, errno)	(0x80000000 | ((level)<<24) | ((mod)<<16) | (errno))

#define TBUS_SUCCESS				0
#define TBUS_OK				0
#define TBUS_ERROR				-1

#define TBUS_ERR_IS_ERROR(error)			( (error) & 0x80000000 )
#define TBUS_ERR_START_ERRNO		1024
#define TBUS_ERR_GET_ERROR_CODE(err)       (((err) & 0xFFFF) - TBUS_ERR_START_ERRNO)

#define TBUS_ERR_LEVEL_DEBUG			0x01
#define TBUS_ERR_LEVEL_WARN				0x02
#define TBUS_ERR_LEVEL_ERROR			0x03
#define TBUS_ERR_LEVEL_SEVERE			0x04

#define TBUS_ERR_MAKE_DEBUG(errno)		TBUS_ERR_MAKE(TBUS_ERR_LEVEL_DEBUG, TMODID_BUS, errno)
#define TBUS_ERR_MAKE_WARN(errno)		TBUS_ERR_MAKE(TBUS_ERR_LEVEL_WARN, TMODID_BUS, errno)
#define TBUS_ERR_MAKE_ERROR(errno)		TBUS_ERR_MAKE(TBUS_ERR_LEVEL_WARN, TMODID_BUS, errno + TBUS_ERR_START_ERRNO)
#define TBUS_ERR_MAKE_SEVERE(errno)		TBUS_ERR_MAKE(TBUS_ERR_LEVEL_WARN, TMODID_BUS, errno)






/**
*TBUS错误代码定义
*/
typedef enum
{
	TBUS_ERROR_NONE = 0,                    /**< 没有错误*/
	TBUS_ERROR_NO_MEMORY,					/**<分配内存失败*/
	TBUS_ERROR_GET_HEAD_META,				/**<获取tbus头部的元数据描述失败*/
	TBUS_ERROR_INIT_LOGSYSTEM_FAILED,		/**< 初始化日志系统失败*/
	TBUS_ERROR_PARSE_ADDR_TEMPLET,			/**< 解析通信地址十进制点分表示模板失败*/
	TBUS_ERROR_INVALID_ADDR_STRING,			/**<点分十进制通信地址串不正确*/
	TBUS_ERROR_REGISTER_BUSSID,				/**<向代理系统注册业务ID失败*/
	TBUS_ERROR_SHMGET_FAILED,				/**<通过shmget分配共享内存失败*/
	TBUS_ERROR_SHMAT_FAILED,				/**<通过shmat加载共享内存失败*/
	TBUS_ERROR_GEN_SHMKEY_FAILED,			/**<生成共享内存的key值失败*/
	TBUS_ERROR_CHECK_GCIMSHM_FAILED,		/**<通道共享内存校验失败*/
	TBUS_ERROR_INVALID_ARG,					/**<传递给接口的参数不对*/
	TBUS_ERROR_NOT_INITIALIZED,				/**<Tbus系统还没有初始化*/
	TBUS_ERROR_HANDLE_FULL,				/**<已分配tbus句柄数据已经达到设定的最大数目，不能在分配*/
	TBUS_ERROR_INVALID_HANDLE,			/**<tbus处理句柄无效*/
	TBUS_ERROR_CHANNEL_NUM_LIMITED,			/**<一个tbus处理句柄所管理的通道数已经到达上限*/
	TBUS_ERROR_BIND_FAILED,				/**<通过指定地址绑定不到任何通道*/
	TBUS_ERROR_NO_CHANNEL,				/**<tbus句柄没有绑定任何通信通道*/
	TBUS_ERROR_CHANNEL_ADDRESS_CONFLICT,	/**<通道共享内存中记录的地址信息与GCIM中记录的地址信息不一致*/
	TBUS_ERROR_NO_PEER_ADDRESS_MATCHED,		/**<与tbus句柄管理的任何通道的对端地址都不匹配*/
	TBUS_ERROR_NO_CHANNEL_MATCHED,			/*没有与指定源地址和目的地址匹配的通道*/
	TBUS_ERROR_PACK_HEAD,					/**<tbus头部打包失败*/
	TBUS_ERROR_TOO_BIG_HEAD,				/**<tbus头部打包信息长度超过了最大预定长度 */
	TBUS_ERROR_CHANNEL_IS_FULL,				/**<tbus发送消息通道已满，不能再发送数据*/
	TBUS_ERROR_CHANNEL_IS_EMPTY,			/**<tbus消息接收通道已空，没有任何消息可接收*/
	TBUS_ERROR_CHANNEL_DATA_CONFUSE,		/**<通道中数据长度错误，只有丢弃此数据才能恢复正常*/
	TBUS_ERROR_RECV_BUFFER_LIMITED,			/**<接收数据的缓冲区太小，不足以接收整个数据包*/
	TBUS_ERROR_UNPACK_HEAD,					/**<tbus消息头部解包失败*/
	TBUS_ERROR_FORWARD_ROUTE_LIMITED,		/**<受最大转发路由跳点限制，不能继续转发*/
	TBUS_ERROR_INVALID_ROUTE,				/**<路由信息无效*/
	TBUS_ERROR_INVALID_GCIM_CONF,			/**<GCIM的配置不正确*/
	TBUS_ERROR_SHMCTL_FAILED,				/*shmctl操作共享内存失败*/
	TBUS_ERROR_RELAY_NUM_LIMITED,			/**<管理的中转通道数已经到达最大数目限制*/
	TBUS_ERROR_FAILED_CHECK_HEADSUM,					/**<校验数据包头部失败*/
	TBUS_ERROR_DELETE_USED_CHANNEL,					/**<数据通道正在使用，不能删除此通道的共享内存*/
	TBUS_ERROR_UNSUPPORTED_OPTION,			/**<不支持的option*/
	TBUS_ERROR_INVALID_OPTION_VALUE,		/**<选项值不正确*/
	TBUS_ERROR_NULL_CHANNEL_HEAD,		/*tbus channel保存核心数据的头部结构指针为NULL*/
	TBUS_ERROR_INVALID_CHANNEL_MAGIC,	/*TBus channel共享内存数据中记录的Magic匹配不上*/
	TBUS_ERROR_INVALID_ALIGN,			/*Tbus 数据通道中的字节对齐设置不正确*/
	TBUS_ERROR_CHANNEL_SIZE_NOTMATCH,	/*TBUS  通道所需大小与共享内存实际大小不一致*/
	TBUS_ERROR_INVALID_CHANNEL_SIZE,	/*TBUS 数据通道的大小与 指定配置的值不一致*/
	TBUS_ERROR_INVALID_CAHNNEL_QUEUE_PARAM, /*TBus 通道队列中的参数不正确*/
    TBUS_ERROR_UNEXPECTED_SPECIAL_ADDR, /**<不应该使用特殊的TBUS地址*/
    TBUS_ERROR_NO_MATCHED_PEER,         /**<没有匹配的对端进程*/
    TBUS_ERROR_NO_MORE_PEER,            /**<没有更多的peer进程*/
	TBUS_ERROR_ADDR_BUFFER_LIMITED,		/**<接收地址的缓冲区太小，不足以接收所有的peer地址*/
	TBUS_ERROR_CHECK_GRMSHM_FAILED,		/**<路由共享内存校验失败*/
	TBUS_ERROR_CHECK_BAMSHM_FAILED,		/**<地址映射共享内存校验失败*/
	TBUS_ERROR_ADDR_BIND_NUM_LIMITED,	/**<一个tbus处理句柄所管理的绑定地址数已经到达上限*/
	TBUS_ERROR_INITED_IN_NORMAL_MODE,   /**<已经在普通模式下init成功，不能再调用tbus_automatic_init*/
	TBUS_ERROR_INITED_IN_AUTOMATIC_MODE,  /**<已经在自动模式下init成功，不能再调用tbus_init/tbus_init_ex*/
	TBUS_ERROR_CONNECT_TBUSD_FAILED,    /**<连接tbusd失败*/
	TBUS_ERROR_GET_MGR_META,            /**<获取tbusd mgr的meta失败*/
	TBUS_ERROR_TDR_FAIL,                /**<tdr操作失败*/
	TBUS_ERROR_TBUSD_OPER_FAIL,         /**<与tbusd的交互失败*/
	TBUS_ERROR_TBUSD_RESPONSE_ERROR,    /**<tbusd返回出错信息*/
	TBUS_ERROR_DST_ADDR_INVALID,        /**<目标地址非法*/
	TBUS_ERROR_WRONG_MODE,              /**<所调用的函数不能在当前模式下使用*/
    TBUS_ERROR_CHANNEL_MATCH_TWO_ROUTES,/**<通道两端都找到对应的路由*/
    TBUS_ERROR_HAS_BINDED,              /**<BusHandle只允许绑定一个地址，已经绑定过了*/
    TBUS_ERROR_TIMEOUT,                 /**<操作超时*/
    TBUS_ERROR_ZK_OPER_FAILED,          /**<zookeeper操作失败*/
    TBUS_ERROR_BINDED_BY_OTHER,         /**<已被其他进程绑定*/
    TBUS_ERROR_CANNOT_CALLED_AFTER_INITED, /**<操作不允许在init之后被调用 */
	TBUS_ERROR_NOT_INITED_IN_AUTOMATIC_MODE, /**<tbus未初始化为自动化模式 */
	TBUS_ERROR_MSG_PROCESSOR_NOT_SET, /**<未注册消息处理回调函数 */
	TBUS_ERROR_NOTIFY_SOCKETFD_RESET, /**<notify socket出错需要重置*/
	TBUS_ERROR_GET_NOTIFY_META,            /**<获取tbusd notify的meta失败*/
	TBUS_ERROR_COUNT                    /**< 错误代码结束标志*/
} TBUSERROR;


#define TBUS_ERR_ARG	TBUS_ERR_MAKE_ERROR(TBUS_ERROR_INVALID_ARG)
#define TBUS_ERR_INIT	TBUS_ERR_MAKE_ERROR(TBUS_ERROR_NOT_INITIALIZED);
#define TBUS_ERR_CHANNEL_FULL	TBUS_ERR_MAKE_ERROR(TBUS_ERROR_CHANNEL_IS_FULL)
#define	TBUS_ERR_CHANNEL_EMPTY	TBUS_ERR_MAKE_ERROR(TBUS_ERROR_CHANNEL_IS_EMPTY)
#define	TBUS_ERR_CHANNEL_CONFUSE TBUS_ERR_MAKE_ERROR(TBUS_ERROR_CHANNEL_DATA_CONFUSE)
#define TBUS_ERR_LIMITED_BUF  TBUS_ERR_MAKE_ERROR(TBUS_ERROR_ADDR_BUFFER_LIMITED)




/**
* 根据错误代码获取错误信息
* @param[in] iErrorCode 错误代码
*
* @return  错误信息串的指针
*/
TSF4G_API const char *tbus_error_string(IN int iErrorCode);

/** @} */ // TBUS_ERR TBUS_错误处理

#ifdef __cplusplus
}
#endif




#endif /* TBUS_ERROR_H */
