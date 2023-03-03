/*
 * Copyright (c) 2013 Tencent all rights reserved
 *
 * date:   2015年1月4日
 * file:   tbus_cpp.h
 * desc:   tbus api的c++封装
 */

#ifndef TBUS_CPP_H_
#define TBUS_CPP_H_
#include <string>
#include "tbus.h"


/** @defgroup CPP_API C++接口
 * @{
 */


namespace tsf4g_tbus
{

/**
 * @brief 字符串格式的特殊地址
 */
const char BUS_ADDR_ALL[] = "all";          //全部
const char BUS_ADDR_DEFAULT[] = "default";  //默认

/**
 * @brief bus全局环境功能包装类，这个类的方法都是静态方法
 */
class BusEnv
{
public:

/**
 * @brief 初始化普通模式的tbus
 * @see tbus_init_ex
 * @param[in] shm_key GCIM共享内存的key
 * @param[in] flag 功能标记，见tbus_init_ex
 * @param[in] business_id 业务id
 * @retval 0  成功
 * @retval <0 失败
 */
static
int InitNormalModeBusEnv(IN const char* shm_key, IN int flag, IN int business_id=0);

/**
 * @brief 初始化自动化模式的tbus
 * @see tbus_automatic_init
 * @param[in] tbusd_url tbusd的本地管理url
 * @param[in] flag 功能标记
 * @param[in] timeout 和 tbusd的交互超时，单位毫秒
 * @param[in] business_id 业务id
 * @retval 0  成功
 * @retval <0 失败
 */
static
int InitAutomaticModeBusEnv(IN const char* tbusd_url, IN int flag, IN int time_out,
                            IN int business_id=0);

/**
 * @brief 清理tbus环境，释放资源
 * @see tbus_fini
 */
static
void FiniBusEnv(void);

/**
 * @brief 设置自动化模式下和tbusd的交互超时
 * @see tbus_set_opt
 * @param[in] timeout 超时时间，单位毫秒
 * @retval 0  成功
 * @retval <0 失败
 */
static
int SetOptAutomaticTbusdTimeout(int timeout);

/**
 * @brief 设置日志句柄
 * @see tbus_set_logcat
 * @param[in] logcat tlog的句柄实例
 */
static
void SetLogCat(IN LPTLOGCATEGORYINST logcat);

/**
 * @brief 设置日志级别
 * @see tbus_set_logpriority
 * @param[in] priority tlog日志级别
 */
static
void SetLogPriority(IN int priority);

/**
 * @brief 设置zookeeper api使用的日志文件
 * @see tbus_automatic_set_zookeeper_log_file
 * @param[in] log_file 日志文件路径
 * @retval 0  成功
 * @retval !0 失败
 */
static
int SetZookeeperLogFile(IN const char* log_file);

/**
 * @brief 自动化模式下注册通道
 * @see tbus_automatic_regist_relations
 * @param[in] relations 要注册的通道信息(两端地址和大小)
 * @param[in] relations_num 要注册的通道数量
 * @retval 0  成功
 * @retval <0 失败
 */
static
int AutomaticRegistRelations(IN const AddrRelation* relations, IN int relations_num);

/**
 * @brief 自动化模式下注册通道
 * @see tbus_automatic_regist_relations_by_str
 * @param[in] relations 要注册的通道信息(两端地址和大小)
 * @param[in] relations_num 要注册的通道数量
 * @retval 0  成功
 * @retval <0 失败
 */
static
int AutomaticRegistRelations(IN const AddrRelationStr* relations, IN int relations_num);

/**
 * @brief 把字符串格式的tbus地址转换为整数格式
 * @see tbus_addr_aton
 * @param[in] str_addr  要转换的tbus地址
 * @param[out] bus_addr 转换为整数的tbus地址
 * @retval 0  成功
 * @retval <0 失败
 */
static
int AddrAToN(IN const char* str_addr, OUT TBUSADDR* bus_addr);

/**
 * @brief 把整数格式的tbus地址转换为字符串格式
 * @see tbus_addr_ntoa
 * @param[in] bus_addr 要转换的tbus地址
 * @retval 字符串格式的tbus地址, 失败会返回"", 也就是空串
 */
static
const char* AddrNToA(IN TBUSADDR bus_addr);

/**
 * @brief 把整数格式的tbus地址转换为字符串格式，填到指定的buffer中
 * @see tbus_addr_ntop
 * @param[in] bus_addr       要转换的tbus地址
 * @param[out] str_addr_buff 存放转换结果的buffer
 * @param[in] buff_len       str_addr_buff的长度
 * @retval str_addr_buff的地址, 失败会返回""，也就是空串
 */
static
char* AddrNToP(IN TBUSADDR bus_addr, OUT char* str_addr_buff, IN unsigned int buff_len);

/**
 * @brief 获取消息的染色id
 * @see tbus_get_dyedmsgid
 * @retval 染色id
 */
static
unsigned int GetDyedMsgId(void);

/**
 * @brief 设置tbus系统使用的当前时间
 * @see tbus_set_current_time
 * @param[in] current_time 当前时间
 * @retval 0  成功
 * @retval <0 失败
 */
static
int SetCurrentTime(IN const struct timeval* current_time);

/**
 * @brief 让tbus系统使用用户提供的系统时间
 * @see tbus_use_saved_time
 * @retval 始终为0
 */
static
int UseSavedTime(void);

/**
 * @brief 让tbus系统使用gettimeofday来获取系统时间
 * @see tbus_use_sys_time
 * @retval 始终为0
 */
static
int UseSysTime(void);

/**
 * @brief 获取错误码对应的错误信息
 * @see tbus_error_string
 * @retval 错误信息字符串的地址
 */
static
const char* GetErrorStr(int err_code);

};

/**
 * @brief tbus handle的包装类
 */
class BusHandle
{
public:
    /**
     * @brief 默认构造函数，会在内部调用tbus_new分配一个句柄，并own为true,
     *        在析构或者set_handle时会调用tbus_delete释放这个句柄
     */
    BusHandle(void);
    /**
     * @brief 使用外部传入的tbus handle的构造函数
     *        own表示是否由本BusHandle在适当的时候(析构函数或者set_handle)调用tbus_delete释放这个handle
     * @param[in] handle 外部已经使用tbus_new获得的tbus句柄
     * @param[in] own 是否需要BusHandle在析构时对handle调用tbus_delete
     */
    BusHandle(IN int handle, IN bool own);
    virtual ~BusHandle();

public:
    /**
     * @brief 绑定一个地址，对应tbus_bind_by_str
     * @note  和tbus_bind_by_str不同的是，为了简化概念和API，这里一个BusHandle只允许Bind一个地址,
     *        如果需要Bind多个地址，那么可以考虑使用多个BusHandle，或者使用c版本的tbus api
     * @see tbus_bind_by_str
     * @param[in] addr 要绑定的地址
     * @retval 0  成功
     * @retval <0 失败，使用BusEnv::GetErrorStr获取出错信息
     */
    int Bind(IN const char* addr);
    /**
     * @brief 绑定一个地址，对应tbus_bind
     * @note  和tbus_bind不同的是，为了简化概念和API，这里一个BusHandle只允许Bind一个地址,
     *        如果需要Bind多个地址，那么可以考虑使用多个BusHandle，或者使用c版本的tbus api
     * @see tbus_bind
     * @param[in] addr 要绑定的地址
     * @retval 0  成功
     * @retval <0 失败，使用BusEnv::GetErrorStr获取出错信息
     */
    int Bind(IN TBUSADDR addr);
    /**
     * @brief 独占模式下指定一个要通信的对端地址, 非独占模式下无意义，对应tbus_connect_by_str
     * @see tbus_connect_by_str
     * @param[in] peer_addr 对端地址
     * @retval 0  成功
     * @retval <0 失败，使用BusEnv::GetErrorStr获取出错信息
     */
    int Connect(IN const char* peer_addr);
    /**
     * @brief 独占模式下指定一个要通信的对端地址, 非独占模式下无意义，对应tbus_connect
     * @see tbus_connect
     * @param[in] peer_addr 对端地址
     * @retval 0  成功
     * @retval <0 失败，使用BusEnv::GetErrorStr获取出错信息
     */
    int Connect(IN TBUSADDR peer_addr);
    /**
     * @brief 向指定目标地址发送一个消息, 对应tbus_send
     * @note  相比tbus_send, 这个接口没有传src，是因为BusHandle类只允许Bind一个地址，src就是Bind的地址，从而简化了接口
     * @see tbus_send
     * @param[in] msg 要发送的消息
     * @param[in] msg_len msg的长度
     * @param[in] peer_addr 目的地址，字符串格式
     * @param[in] flag 功能标记，见tbus_send的说明
     * @retval 0  成功
     * @retval <0 失败，使用BusEnv::GetErrorStr获取出错信息
     */
    int SendTo(IN const void* msg, IN size_t msg_len, IN const char* peer_addr, int flag=0);
    /**
     * @brief 向指定目标地址发送一个消息, 对应tbus_send
     * @note  相比tbus_send, 这个接口没有传src，是因为BusHandle类只允许Bind一个地址，src就是Bind的地址，从而简化了接口
     * @see tbus_send
     * @param[in] msg 要发送的消息
     * @param[in] msg_len msg的长度
     * @param[in] peer_addr 目的地址，整数格式
     * @param[in] flag 功能标记，见tbus_send的说明
     * @retval 0  成功
     * @retval <0 失败，使用BusEnv::GetErrorStr获取出错信息
     */
    int SendTo(IN const void* msg, IN size_t msg_len, IN TBUSADDR peer_addr, int flag=0);
    /**
     * @brief 将多个buffer中的数据作为一个消息发送出去
     * @note  相比tbus_sendv, 这个接口没有传src，是因为BusHandle类只允许Bind一个地址，src就是Bind的地址，从而简化了接口
     * @see tbus_sendv
     */
    int SendVTo(IN const struct iovec* vec, IN const int vec_num, IN const char* peer_addr,
                IN int flag=0);
    /**
     * @brief 将多个buffer中的数据作为一个消息发送出去
     * @note  相比tbus_sendv, 这个接口没有传src，是因为BusHandle类只允许Bind一个地址，src就是Bind的地址，从而简化了接口
     * @see tbus_sendv
     */
    int SendVTo(IN const struct iovec* vec, IN const int vec_num, IN TBUSADDR peer_addr,
                IN int flag=0);
    /**
     * @brief 从指定目的地址接收一个消息，对应tbus_recv
     * @note  相比tbus_recv, 这个接口参数只有一个peer_addr，是因为BusHandle类只允许Bind一个地址，Bind的地址就是消息的目的地址，从而简化了接口
     * @see tbus_recv
     * @param[out]      recv_buffer 用来保存收到消息的内存buffer
     * @param[in,out]   recv_buffer_len 输入-recv_buffer的长度
     *                                  输出-收到消息的长度
     * @param[in,out]   peer_addr 输入-指定通信对端的地址, 支持特殊地址(广播或默认)
     *                            输出-消息的具体来源地址
     * @param[in]       peer_addr_len peer_addr的缓冲区长度
     * @param[in]       flag 功能标记，见tbus_recv
     * @retval 0 收到了消息
     * @retval <0 出错(也可能是通道中没有消息, 此时返回TBUS_ERR_CHANNEL_EMPTY), 使用BusEnv::GetErrorStr获取出错信息
     */
    int RecvFrom(OUT void* recv_buffer, INOUT size_t* recv_buffer_len,
                 INOUT char* peer_addr, IN size_t peer_addr_len, int flag=0);
    /**
     * @brief 从指定目的地址接收一个消息，对应tbus_recv
     * @note  相比tbus_recv, 这个接口参数只有一个peer_addr，是因为BusHandle类只允许Bind一个地址，Bind的地址就是消息的目的地址，从而简化了接口
     * @see tbus_recv
     * @param[out]      recv_buffer 用来保存收到消息的内存buffer
     * @param[in,out]   recv_buffer_len 输入-recv_buffer的长度
     *                                  输出-收到消息的长度
     * @param[in,out]   peer_addr 输入-指定通信对端的地址, 支持特殊地址(广播或默认)
     *                            输出-消息的具体来源地址
     * @param[in]       flag 功能标记，见tbus_recv
     * @retval 0 收到了消息
     * @retval <0 出错(也可能是通道中没有消息, 此时返回TBUS_ERR_CHANNEL_EMPTY), 使用BusEnv::GetErrorStr获取出错信息
     */
    int RecvFrom(OUT void* recv_buffer, INOUT size_t* recv_buffer_len,
                 INOUT TBUSADDR* peer_addr, int flag=0);
    /**
     * @brief 从数据通道中获取出队列头部第一个消息的存储位置和此消息的长度信息, 需要手工调用DeleteMsg才会从队列中删除这条消息(RecvFrom会自动删除)
     * @note  相比tbus_peek_msg, 这个接口参数只有一个peer_addr，是因为BusHandle类只允许Bind一个地址，Bind的地址就是消息的目的地址，从而简化了接口
     * @see tbus_peek_msg
     * @retval 0 取到了消息
     * @retval <0 出错(也可能是通道中没有消息, 此时返回TBUS_ERR_CHANNEL_EMPTY), 使用BusEnv::GetErrorStr获取出错信息
     */
    int PeekMsg(INOUT const char** data, OUT size_t* data_len, INOUT char* peer_addr, size_t peer_addr_len,
                IN int flag=0);
    /**
     * @brief 从数据通道中获取出队列头部第一个消息的存储位置和此消息的长度信息, 需要手工调用DeleteMsg才会从队列中删除这条消息(RecvFrom会自动删除)
     * @note  相比tbus_peek_msg, 这个接口参数只有一个peer_addr，是因为BusHandle类只允许Bind一个地址，Bind的地址就是消息的目的地址，从而简化了接口
     * @see tbus_peek_msg
     * @retval 0 取到了消息
     * @retval <0 出错(也可能是通道中没有消息, 此时返回TBUS_ERR_CHANNEL_EMPTY), 使用BusEnv::GetErrorStr获取出错信息
     */
    int PeekMsg(INOUT const char** data, OUT size_t* data_len, INOUT TBUSADDR* peer_addr, IN int flag=0);
    /**
     * @brief 删除"peer_addr->绑定地址"通道上的第一条消息, 通常在PeekMsg后使用
     * @see tbus_delete_msg
     * @retval 0 删除了消息
     * @retval <0 出错(也可能是通道中没有消息, 此时返回TBUS_ERR_CHANNEL_EMPTY), 使用BusEnv::GetErrorStr获取出错信息
     */
    int DeleteMsg(const char* peer_addr);
    /**
     * @brief 删除"peer_addr->绑定地址"通道上的第一条消息, 通常在PeekMsg后使用
     * @see tbus_delete_msg
     * @retval 0 删除了消息
     * @retval <0 出错(也可能是通道中没有消息, 此时返回TBUS_ERR_CHANNEL_EMPTY), 使用BusEnv::GetErrorStr获取出错信息
     */
    int DeleteMsg(TBUSADDR peer_addr);
    /**
     * @brief 清空指定通道"peer_addr->绑定地址"的所有数据
     * @see tbus_empty_channel_recvside
     * @retval 0  成功
     * @retval <0 失败
     */
    int EmptyRecvSide(const char* peer_addr);
    /**
     * @brief 清空指定通道"peer_addr->绑定地址"的所有数据
     * @see tbus_empty_channel_recvside
     * @retval 0  成功
     * @retval <0 失败
     */
    int EmptyRecvSide(TBUSADDR peer_addr);
    /**
     * @brief 清空指定通道"绑定地址->peer_addr"的所有数据
     * @see tbus_empty_channel_sendside
     * @retval 0  成功
     * @retval <0 失败
     */
    int EmptySendSide(const char* peer_addr);
    /**
     * @brief 清空指定通道"绑定地址->peer_addr"的所有数据
     * @see tbus_empty_channel_sendside
     * @retval 0  成功
     * @retval <0 失败
     */
    int EmptySendSide(TBUSADDR peer_addr);
    /**
     * @brief 刷新句柄, 对应tbus_refresh_handle
     * @see tbus_refresh_handle
     * @retval 0  成功
     * @retval <0 失败
     */
    int Refresh();

public:
    /**
     * @brief 把handle设置为独占模式,因为此选项是只能打开，无法关闭的，所以这个函数没有参数
     * @see tbus_set_handle_opt
     * @retval 0  成功
     * @retval <0 失败
     */
    int SetOptTurnOnExclusiveMode();
    /**
     * @brief 打开/关闭消息时间戳功能，见tbus.h-TBUS_HANDLE_OPT_NAME_CHANNELS_TIMESTAMP
     * @see tbus_set_handle_opt
     * @retval 0  成功
     * @retval <0 失败
     */
    int SetOptWithTimeStamp(IN bool turn_on);
    /**
     * @brief 把句柄设置为自适应模式，见tbus.h-TBUS_HANDLE_OPT_NAME_ADAPTIVE_BINDING
     * @see tbus_set_handle_opt
     * @retval 0  成功
     * @retval <0 失败
     */
    int SetOptAdaptiveMode(IN bool turn_on);

public:
    /**
     * @brief 获取收到的上一条消息的时间戳
     * @see tbus_get_msg_timestamp
     * @retval 0  成功
     * @retval <0 失败
     */
    int GetMsgTimeStamp(OUT LPTBUSTIMESTAMP time_stamp);

public:
    /**
     * @brief 获取原始bus handle
     * @retval tbus handle的原始值
     */
    inline
    int handle(){ return handle_; }
    /**
     * @brief 设置原始bus handle
     * @note  如果当前own_是true, 那么会先调用tbus_delete释放handle_, 再把handle_设置为新的handle
     * @param[in] handle 叫给BusHandle管理的原始tbus句柄
     * @param[in] own    是否由BusHandle类负责handle的释放
     * @retval 0  成功
     * @retval <0 失败
     */
    int set_handle(IN int handle, IN bool own);

private:
    void SetLocalAddr(int handle);

private:
    int handle_;
    bool own_;
    TBUSADDR local_addr_;
    std::string local_addr_str_;
};


}

#endif /* TBUS_CPP_H_ */
