/** @file $RCSfile: tbus.h,v $
  Invoked function header file for tsf4g-tbus module
  $Id: tbus.h,v 1.24 2009-09-14 10:53:48 jacky Exp $
  @author $Author: jacky $
  @date $Date: 2009-09-14 10:53:48 $
  @version $Revision: 1.24 $
  @note Editor: Vim 6.3, Gcc 4.0.2, tab=4
  @note Platform: Linux
  */


#ifndef _TBUS_H
#define _TBUS_H

/* automatically include the correct library on windows */
#if defined(_WIN32)|| defined(_WIN64)


#ifdef TSF4G_SMALL_LIB
#undef _TSF4G_LARGE_LIB_
#endif


#ifdef _TSF4G_LARGE_LIB_
#ifdef _DEBUG
#pragma comment(lib, "libtsf4g_d.lib")
#else
#pragma comment(lib, "libtsf4g.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "libtbus_d.lib")
#else
#pragma comment(lib, "libtbus.lib")
#endif
#endif



#endif /*#if defined(_WIN32)|| defined(_WIN64)*/

#include <stdio.h>
#include "pal/pal.h"
#include "tlog/tlog.h"
#include "tbus/tbus_macros.h"
#include "tbus/tbus_error.h"



/** @name 函数参数输入/输出属性
 * @{
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


/* TbusFlag*/
enum tagTbusFlag
{
    TBUS_FLAG_START_DYE_MSG = 1,     /* 指示tbus系统对此消息进行染色 */
    TBUS_FLAG_KEEP_DYE_MSG = 2,      /* 指示tbus系统保持前一个消息的染色信息 */
    TBUS_FLAG_SET_TEST_MSG = 4,      /* 指示tbus系统此数据包为测试数据包 */
    TBUS_FLAG_WITH_TIMESTAMP = 8,    /* 指示携带时间戳信息 */
    TBUS_FLAG_USE_GETTIMEOFDAY = 16, /* 指示使用gettimeofday取得的时间作为时间戳值 */
	TBUS_FLAG_USE_NOTIFY = 32,       /* 指示使用事件机制发送一个通知消息到对端 */
};

/* initialize flags*/
enum tagTbusInitFlag
{
    TBUS_INIT_FLAG_NONE    =    0x00000000,     /*无特殊标记*/
    TBUS_INIT_FLAG_USE_SAVED_TIME = 0x00000001, /*记录时间戳值时使用tbus_set_current_time设置的时间值*/
};

/*channel mode initialize flags*/
enum tagTbusChannelModeInitFlag
{
    TBUS_CHANNEL_MODE_INIT_FLAG_NONE    =    0x00000000,        /*无特殊标记*/

    /*
     * 打开缺省日志功能，对channel调用进行调试跟踪，
     * 此标志位只适合单线程且只是调试问题时使用，
     * 正式运营不能打开此标志位
     */
    TBUS_CHANNEL_MODE_INIT_FLAG_ENABLE_TRACE = 0x00000001,
};

/* tbus global option */
enum tagTbusOptionName
{
    /* 和tbusd交互时的超时时间  */
    TBUS_OPT_NAME_TBUSD_TIMEOUT     = 1,
    /* 动态模式下，通道不存在不自动创建 */
    TBUS_OPT_NAME_NONAUTO_CREATED   = 2,
};
typedef enum tagTbusOptionName TBUSOPTIONNAME;
typedef enum tagTbusOptionName *LPTBUSOPTIONNAME;

/* tbus handle option */
enum tagTbusHandleOptionName
{
    /* 设置了此选项后，句柄只能接收tbus_connect指定的相关通道 */
    TBUS_HANDLE_OPT_NAME_EXCLUSIVE_CHANNELS = 1,
    /* 通过设置此选项，指示所有通道发送数据时携带时间戳信息 */
    TBUS_HANDLE_OPT_NAME_CHANNELS_TIMESTAMP = 2,
    /* 通过设置此选项，句柄可以动态适配，无需创建通道也可绑定成功 */
    TBUS_HANDLE_OPT_NAME_ADAPTIVE_BINDING = 3,
};
typedef enum tagTbusHandleOptionName    TBUSHANDLEOPTIONAME;
typedef enum tagTbusHandleOptionName    *LPTBUSHANDLEOPTIONAME;

enum tagTbusPeerStateType
{
    TBUS_PEER_STATE_UNAVAILABLE = 0x00,
    TBUS_PEER_STATE_AVAILABLE   = 0x01,
};

#define TBUS_HAVE_MESSAGE                            1     /* 表示有消息可以接收*/

#define TBUS_ADDR_DEFAULT                            0     /*  表示适配第一个地址*/
#define TBUS_ADDR_ALL                                -1     /* 表示适配所有地址 */


#define TBUS_LOGCAT_NAME    "tsf4g.tbus"        /*tbus日志类名称*/

#define TBUS_DEFAULT_GCIM_KEY   "1688"    /*default share memory key for GCIM(Global Channel Infomation Map)*/
#define TBUS_DEFAULT_GRM_KEY    "2688"    /*default share memory key for GRM(Global Route Map)*/
#define TBUS_DEFAULT_MAP_KEY    "3688"    /*default share memory key for tbus address map */

#define TBUS_ROUTE_GLOBAL_SHMKEY "1688"

#define TBUS_CHANNEL_MAGIC    0x42545553        //TBUS

#define TBUS_DEFAULT_TBUSD_URL "udp://127.0.0.1:1599"

#define TBUS_DEFAULT_TBUSD_TIMEOUT (800)   /*default timeout for the oper with tbusd*/

#define TBUS_MAX_ADDR_LEN 64

#define TBUS_MAX_RELATION_COUNT (256)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

    typedef  int    TBUSADDR;        /*tbus系统通信地址*/
    typedef  int    *LPTBUSADDR;     /*tbus系统通信地址*/


    typedef struct tagTbusChannel                              *LPTBUSCHANNEL;

    /*route info of package */
    struct tagHeadRoute
    {
        TBUSADDR iSrc;
    };
    typedef struct tagHeadRoute                            HEADROUTE;
    typedef struct tagHeadRoute                            *LPHEADROUTE;

    struct tagTbusTimeStamp
    {
        int iIsAvailable;      /* 前一个消息是否有时间戳 */
        long long llSec;       /* 时间戳值      seconds 部分 */
        unsigned int dwUSec;   /* 时间戳值 microseconds 部分 */
        unsigned int dwCumm;   /* TBUS处理该消息所用的总时间 */
    };
    typedef struct tagTbusTimeStamp                                        TBUSTIMESTAMP;
    typedef struct tagTbusTimeStamp                                        *LPTBUSTIMESTAMP;

    struct tagTbusRecv
    {
        IN    int           iHandle;            /* tbus 句柄 */
        INOUT TBUSADDR      iSrc;               /* tbus 源地址 */
        INOUT TBUSADDR      iDst;               /* tbus 目的地址 */
        INOUT void          *pvData;            /* 接收数据的缓冲区 */
        INOUT size_t        tDataLen;           /* 保存接收数据的长度 */
        IN    int           iFlag;              /* 取tagTbusFlag中的值，可"|"操作 */
        INOUT LPTBUSTIMESTAMP pstTimeStamp;     /* 如果不为NULL，接收消息的时间戳值 */
    };
    typedef struct tagTbusRecv    TBUSRECV;
    typedef struct tagTbusRecv *LPTBUSRECV;

    struct tagTbusPeerState
    {
        long long llLastBeatTime;
        int iIsAvailable;                 /*参考enum类型tagTbusPeerStateType*/
        int iTimeOutGap;
    };
    typedef struct tagTbusPeerState       TBUSPEERSTATE;
    typedef struct tagTbusPeerState    *LPTBUSPEERSTATE;

    struct tagAddrRelation
    {
        TBUSADDR iAddr1;
        TBUSADDR iAddr2;
        int iSendQueueSize;           /* iAddr1->iAddr2内存队列的大小，<=0表示使用tbusd配置的默认值 */
        int iRecvQueueSize;           /* iAddr2->iAddr1内存队列的大小,<=0表示使用tbusd配置的默认值 */
    };
    typedef struct tagAddrRelation AddrRelation;
    struct tagAddrRelationStr
    {
        char szAddr1[TBUS_MAX_ADDR_LEN];
        char szAddr2[TBUS_MAX_ADDR_LEN];
        int iSendQueueSize;           /* szAddr1->szAddr2内存队列的大小，<=0表示使用tbusd配置的默认值 */
        int iRecvQueueSize;           /* szAddr2->szAddr1内存队列的大小,<=0表示使用tbusd配置的默认值 */
    };
    typedef struct tagAddrRelationStr AddrRelationStr;


    /** @defgroup TBUS_INIT TBUS_句柄管理
     * @{
     */

    /**
     * @brief 初始化Tbus系统。
     * 主要职责是尝试挂载(attach)到保存了tbus通道配置信息的共享内存GCIM(Global Channel Infomation Map)
     * 使用本函数进行初始化后，tbus系统运行在静态模式下，也就是说所有的CHANNEL都需要预先使用工具或者TCM创建好，tbus不会在运行过程中创建所需的CHANNEL
     *
     * @param[in] a_pszShmkey  保存生产GCIM共享内存key的信息串。
     * 此信息串中的信息可以为十进制数字串，
     * 或为一个文件路径(当此文件不存在时，tbus系统会尝试生成此文件)。
     * 根据此信息串生成GCIM共享内存key的算法是:
     * - 十进制数字串  将此数字串转换整数，此整数作为GCIM的共享内存的key
     * - 文件路径      将此文件路径和a_iBussId作为参数，调用ftok接口生产GCIM共享内存的key
     * - NULL或空串    使用缺省值 TBUS_DEFAULT_GCIM_KEY(1688)作为GCIM共享内存的key
     * @param[in]    a_iFlag 初始化操作标志位:
     * - TBUS_INIT_FLAG_USE_SAVED_TIME
     * 所有tbus时间戳和心跳机制需要使用系统时间时，使用通过tbus_set_current_time保存的时间。
     *
     * @note 本接口必须在其他tbus接口调用之前调用
     * @note 本接口与tbus_automatic_init接口互斥，即只能调用其中一个接口进行初始化，否则后调的接口会返回失败
     * @note 当业务进程退出时，必须调用tbus_fini接口，是否本接口分配的资源
     * @note 在同一个进程内，本接口只有第一次成功调用会产生作用，后续调用本接口时将直接返回
     * @note 本接口非多线程安全
     *
     * @pre     通过tbusmgr工具或TCM系统成功创建GCIM共享内存
     *
     * @retval 0 处理成功
     * @retval <0 处理失败，请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_fini
     * @see tbus_use_saved_time
     * @see tbus_use_sys_time
     */
    TSF4G_API int tbus_init_ex(IN const char *a_pszShmkey, IN int a_iFlag);

    /**
     * @brief 在自动化模式下初始化Tbus系统。
     * 主要职责是尝试连接tbusd，并使用tbusd提供的GCIMKey挂载(attach)到保存了tbus通道配置信息的共享内存GCIM(Global Channel Infomation Map)
     * 使用本函数进行初始化后，tbus系统运行在自动化模式下，也就是说会由tbusd负责channel的自动创建，无需使用工具或TCM提前创建通道
     * 在自动化模式下这些接口在发现channel不存在时会自动向tbusd请求创建通道:
     * tbus_connect/tbus_connect_by_str
     * tbus_send/tbus_sendv
     * tbus_forward/tbus_forwardv
     * tbus_backward/tbus_backwardv
     * tbus_send_heartbeat
     *
     * @param[in] a_pszTbusdUrl tbusd的地址url
     * 只支持udp, 格式为"udp://127.0.0.1:xxxx"
     * - NULL或空串("") 使用缺省值 TBUS_DEFAULT_TBUSD_URL连接tbusd
     * @param[in]    a_iFlag 初始化操作标志位:
     * - TBUS_INIT_FLAG_USE_SAVED_TIME
     * 所有tbus时间戳和心跳机制需要使用系统时间时，使用通过tbus_set_current_time保存的时间。
     * @param[in] a_iTimeout 和tbusd进行通信时的超时时间,单位毫秒
     * 这个值会作为后续所有和tbusd交互时的超时值，在后续操作中可以使用tbus_set_opt/tbus_get_opt(TBUS_OPT_NAME_TBUSD_TIMEOUT)修改和获取这个值
     * @note 本接口必须在其他tbus接口调用之前调用
     * @note 本接口与tbus_init/tbus_init_ex接口互斥，即只能调用其中一个接口进行初始化，否则后调的接口会返回失败
     * @note 当业务进程退出时，必须调用tbus_fini接口，释放本接口分配的资源
     * @note 在同一个进程内，本接口只有第一次成功调用会产生作用，后续调用本接口时将直接返回
     * @note 本接口非多线程安全
     *
     * @pre     对应的tbusd已经启动
     *
     * @retval 0 处理成功
     * @retval <0 处理失败，请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_fini
     * @see tbus_use_saved_time
     * @see tbus_use_sys_time
     */
    TSF4G_API int tbus_automatic_init(IN const char* a_pszTbusdUrl, IN int a_iFlag, IN int a_iTimeout);

    /**
     * @brief 自动化模式下，注册所需的通信关系
     * 只有在自动化模式下才可以调用本接口(使用tbus_automatic_init进行初始化)
     * 对于当前GCIM中不存在的CHANNEL，会请求tbusd进行创建
     * 可以在进程启动进行初始化时调用此接口来确保所需要的所有CHANNEL都已创建好
     * 如果不调用本接口，在后续的send,connect,forward,backward,send_heartbeat操作中，
     * 如果channel不存在，也会自动向tbusd请求创建channel，可以简化开发过程，但创建通道的过程是同步的(与本机的tbusd通信)，可能会带来一定的阻塞
     * 所以最好在进程初始化时就调用本接口创保证所有需要的通道都创建好
     *
     * @param[in] a_pstRelations 所需注册的通信关系(两段地址和队列的大小)
     * @param[in] a_iRelationCount 关系的数目，必须>0并且<=256
     * @note 本接口非多线程安全
     *
     * @pre 已经成功调用过tbus_automatic_init, tbusd已启动
     *
     * @retval 0 处理成功
     * @retval <0 处理失败，请调用tbus_error_string接口取得错误原因
     */
    int tbus_automatic_regist_relations(IN const AddrRelation* a_pstRelations, IN int a_iRelationCount);

    /**
     * @brief 功能同tbus_automatic_regist_relations，只是地址使用字符串形式，而不是TBUSADDR
     *
     * @param[in] a_pstRelations 所需注册的通信关系(两段地址和队列的大小)
     * @param[in] a_iRelationCount 关系的数目，必须>0并且<=256
     * @note 本接口非多线程安全
     *
     * @pre 已经成功调用过tbus_automatic_init
     *
     * @retval 0 处理成功
     * @retval <0 处理失败，请调用tbus_error_string接口取得错误原因
     */
    TSF4G_API int tbus_automatic_regist_relations_by_str(IN const AddrRelationStr* a_pstRelations, IN int a_iRelationCount);

    /**
     * @brief automatic模式下，获取初始化时连接的tbusd url
     *
     * @retval !NULL: automatic模式
     * @retval NULL:  非automatic模式
     */
    TSF4G_API const char* tbus_automatic_get_tbusd_url();

    /**
     * @brief 创建tbus处理句柄
     *
     * @param[in,out] a_piHandle 存储处理句柄的指针
     *
     * @retval 0 -- successful, get handle ok
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre \e   a_piHandle 不能为 NULL
     *
     * @note 本接口是非线程安全的，创建出来的句柄也只能在同一个线程空间里使用
     * @note 本接口创建的句柄必须使用tbus_delete来关闭，以释放资源
     */
    TSF4G_API int tbus_new(INOUT int *a_piHandle);

    /**
     * @brief 删除一个tubs处理句柄
     *
     * @param[in,out] a_piHandle 保存tbus处理句柄的指针，tbus句柄通过tbus_new()获取
     */
    TSF4G_API void tbus_delete(INOUT int *a_piHandle);


    /**
     * @brief 关闭tbus系统
     *
     * @note 只要调用了本接口，tbus其他接口的功能将不再可用
     */
    TSF4G_API void tbus_fini(void);

    /**
     * @brief 设置 tbus 选项
     *
     * @param[in] a_iOptionName    此参数指定需要设置的tbus选项，目前支持的选项有:
     * - TBUS_OPT_NAME_TBUSD_TIMEOUT 在自动化模式下，与tbusd交互的超时时间,单位为毫秒,a_pvOptionValue为指向int(超时值)的指针,a_dwOptionLen必须为sizeof(int)
     * - TBUS_OPT_NAME_NONAUTO_CREATED 在自动化模式下，如果通道不存在，是否向tbusd请求自动创建通道,a_pvOptionValue为指向int的指针。非0表示不创建，0表示创建，默认为0
     *
     * @param[in] a_pvOptionValue 相应选项的具体设置值
     * @param[in] a_dwOptionLen   指定选项值的长度，其具体值由a_pvOptionValue数据类型决定。
     *
     * @see tbus_set_handle_opt
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     */
    TSF4G_API int tbus_set_opt(IN int a_iOptionName, IN const void *a_pvOptionValue, IN unsigned int a_dwOptionLen);


    /**
     * @brief 获取 tbus 选项
     *
     * @param[in] a_iOptionName    此参数指定需要设置的tbus选项，目前支持的选项有:
     * - TBUS_OPT_NAME_TBUSD_TIMEOUT 在自动化模式下，与tbusd交互的超时时间,单位为毫秒,a_pvOptionValue为指向int(超时值)的指针,a_dwOptionLen必须为sizeof(int)
     * @param[in] a_pvOptionValue  相应选项的具体设置值
     *
     * @param[in,out] a_pdwOptionLen  获取a_pvOptionValue值的具体长度，
     * 如果a_pdwOptionLen所指的长度小于实际需要的长度，则返回失败。此参数输入输出约定如下：
     * - 输入    指定a_pvOptionValue所指缓冲区的大小
     * - 输出    获取a_pvOptionValue获取数据的实际长度
     *
     * @see tbus_set_handle_opt
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     */
    TSF4G_API int tbus_get_opt(IN int a_iOptionName, IN const void* a_pvOptionValue, INOUT unsigned int* a_pdwOptionLen);


    /**
     * @brief 设置tbus通道选项
     *
     * @param[in] a_pstHandle      此参数指定tbus句柄，句柄可由tbus_new获取
     * @param[in] a_iOptionName    此参数指定需要设置的tbus句柄选项，目前支持的选项有：
     * - TBUS_HANDLE_OPT_NAME_EXCLUSIVE_CHANNELS    互斥通道选项。
     *   如果此选项被打开，则通过tbus_connect指定的channel只能属于a_pstHandle，
     *   a_pstHandle也只能通过tbus_connect添加通道。
     * - TBUS_HANDLE_OPT_NAME_CHANNELS_TIMESTAMP    时间戳选项。
     *   如果些选项被打开，则此句柄下所有通道上发送或中转的消息都携带时间戳值。
     * - TBUS_HANDLE_OPT_NAME_ADAPTIVE_BINDING   适应性绑定选项。
     * - 如果此选项被打开，则此句柄在绑定地址时如果相关通道数为0，仍然绑定成功。
     * @param[in] a_pvOptionValue   相应选项的具体设置值，选项不同，其结构也不同：
     * - TBUS_HANDLE_OPT_NAME_EXCLUSIVE_CHANNELS
     *   此选项值的数据类型为int型boolean选项，即其值只能为0或1。
     *   其值为1，打开互斥通道选项。
     *   其值为0，忽略此调用，即互斥通道选项一旦打开就不支持动态关闭。
     * - TBUS_HANDLE_OPT_NAME_CHANNELS_TIMESTAMP
     *   此选项值的数据类型为int型boolean选项，即其值只能为0或1。
     *   其值为1，表示打开时间戳选项。其值为0，表示关闭时间戳选项。
     * - TBUS_HANDLE_OPT_NAME_ADAPTIVE_BINDING
     *   此选项值的数据类型为int型boolean选项，即其值只能为0或1。
     *   其值为1，表示打开适配型绑定选项。其值为0，表示关闭适配型绑定选项。
     * @param[in] a_dwOptionLen    指定选项值的长度，其具体值由a_pvOptionValue数据类型决定。
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_handle_opt
     */
    TSF4G_API int tbus_set_handle_opt(IN int a_iHandle, IN TBUSHANDLEOPTIONAME a_iOptionName,
                                      IN const void *a_pvOptionValue, IN unsigned int a_dwOptionLen);

    /**
     * @brief 获取tbus句柄选项
     *
     * @param[in] a_pstHandle      指定tbus句柄，句柄可由tbus_new获取
     * @param[in] a_iOptionName    此参数指定需要设置的tbus句柄选项，目前支持的选项有：
     * - TBUS_HANDLE_OPT_NAME_EXCLUSIVE_CHANNELS    互斥通道选项。
     *   如果此选项被打开，则通过tbus_connect指定的channel只能属于a_pstChannel，
     *   a_pstChannel也只能添加由tbus_connect指定的通道。
     * - TBUS_HANDLE_OPT_NAME_CHANNELS_TIMESTAMP    时间戳选项。
     *   如果此选项被打开，则此句柄在的通道上发送或中转的消息都携带时间戳值。
     *
     * @param[in] a_pvOptionValue  相应选项的具体设置值，选项不同，其结构也不同：
     * - TBUS_OPT_EXCLUSIVE_CHANNELES 此选项值的数据类型为int型boolean选项，即其值只能为0或1。
     *   其值为1表示互斥通道选项处在打开状态。其值为0表示互斥通道选项处在关闭状态。
     * @param[in,out] a_pdwOptionLen    获取a_pvOptionValue值的具体长度，
     * 如果a_pdwOptionLen所指的长度小于实际需要的长度，则返回失败。
     * 此参数输入输出约定如下：
     * - 输入    指定a_pvOptionValue所指缓冲区的大小
     * - 输出    获取a_pvOptionValue获取数据的实际长度
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_set_handle_opt
     */
    TSF4G_API int tbus_get_handle_opt(IN int a_iHandle, IN int a_iOptionName,
                                      IN const void* a_pvOptionValue, INOUT unsigned int* a_pdwOptionLen);

    /**
     * @deprecated this function has been replaced by tbus_init_ex
     *
     * @see tbus_init_ex
     */
    TSF4G_API int tbus_init(IN const unsigned int a_iMapKey, IN const unsigned int a_iSize);


    /**
     * @deprecated this function has been replaced by tbus_set_bussid and tbus_init_ex
     *
     * @see tbus_init_ex
     */
    TSF4G_API int tbus_initialize(IN const char *a_pszShmkey, IN int a_iBussID);

    /**
     * @brief Set business id
     *
     * @param[in] a_iBussId business id assigned by TCM system, otherwise just set it to 0
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note if business id is not euqal to zero, call this function before tbus_init_ex
     *
     * @see tbus_init_ex
     * @see tbus_ini
     */
    TSF4G_API int tbus_set_bussid(IN const int a_iBussId);

    /**
     * @brief 查询 "disable的通道的最大生存时间"
     *
     * @note disable的通道的最大生存时间:
     *       当通道处于disable的时间大于此值时，进程会从通道通道共享内存detach,
     *
     * @note 建议在 tbus_init_ex 或 tbus_initialize 之后调用
     *
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @retval >=0 -- successful, 返回值即是 "disable的通道的最大生存时间", 单位:秒
     */
    TSF4G_API int tbus_get_disabled_channel_life(void);

    /**
     * @brief 设置 "disable的通道的最大生存时间"
     *
     * @param[in] a_iLife "disable的通道的最大生存时间", 必须 >= 0, 单位:秒
     *
     * @note disable的通道的最大生存时间:
     *       当通道处于disable的时间大于此值时，进程会从通道通道共享内存detach,
     *
     * @note 建议在 tbus_init_ex 或 tbus_initialize 之后调用
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     */
    TSF4G_API int tbus_set_disabled_channel_life(IN int a_iLife);

    /** @} */


    /** @defgroup TBUS_ADDR_CONVERT TBUS_通信地址转换
     * @{
     */

    /**
     * @brief 将点分十进制通信地址字符串转换成tbus系统内存地址
     *
     * @param[in] a_pszAddr 点分十进制通信地址字符串
     * @param[out] a_piAddr 保存tbus通信地址的指针
     *
     * @pre \e a_pszAddr不能为NULL
     * @pre \e a_piAddr 不能为NULL
     *
     * @pre    已经调用tbus_init_ex初始化tbus系统
     *
     * @retval 0 -- successful
     * @retval <0 failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note a_pszAddr中保存的点分十进制通信地址串必须符合点分十进制通信地址模板的约定
     *
     * @see tbus_init_ex
     *
     */
    TSF4G_API int tbus_addr_aton(IN const char *a_pszAddr, OUT TBUSADDR *a_piAddr);


    /**
     * @brief 将tbus通信地址转换成点分十进制地址信息串
     *
     * @param[in] a_iAddr tbus通信地址
     *
     * @return 如果成功转换则返回点分十进制地址串，否则返回空串""
     *
     * @note 返回的成点分十进制表示的地址串保存在一个静态缓冲区中，后一次调用会覆盖前一次调用时获取的信息
     * @note 调用本接口之前必须调用tbus_init_ex已经初始化好tbus系统
     *
     * @see tbus_init_ex
     */
    TSF4G_API char *tbus_addr_ntoa(IN TBUSADDR a_iAddr);

    /**
     * @brief 将tbus通信地址转换成点分十进制地址信息串，并输出到指定缓冲区中，
     *
     * @param[in] a_iAddr tbus通信地址
     * @param[in] a_pszDstBuff 保存输出信息的缓冲区
     * @param[in] a_dwBuffSize 缓冲区的大小
     *
     * @return 返回a_pszDstBuff的地址，
     *
     */
    TSF4G_API char *tbus_addr_ntop(IN TBUSADDR a_iAddr, char *a_pszDstBuff, unsigned int a_dwBuffSize);
    /** @} */

    /** @defgroup TBUS_CHANNEL_MODE TBUS_直接基于通道的通信
     * @{
     */

    /**
     * @brief 完成通道相关接口所需的全局初始化操作，整个进程内只需调用一次
     *
     * @param[in] a_iFlag 初始化标志，预留将来用，目前传入值0
     *
     * @retval 0 -- successful,
     * @retval !0 --failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 在调用tbus_channel_open之前必须调用本接口
     * @note 退出时调用tbus_channel_mode_fini释放资源
     * @note 直接使用通进行通信的模式，是tbus通信模式的简化。
     * 与tbus机制不同，不需事先通过tbus相关工具创建GCIM(存储于共享内存的全局通道配置表)。
     * 本模式仅需指定一个共享内存key，就可以创建一个全双工的数据通道，为两个进程或线程提供通信
     *
     * @note 直接使用通进行通信的模式，大致流程如下：
     * @par 示例代码:
     * @code
    //step1:通信配置初始化
    tbus_channel_mode_init(0);

    //step2: 创建通信通道
    LPTBUSCHANNEL *pstChannel = NULL;  //保存创建的句柄
    TBUSADDR iLocalAddr = 0x01010101;  //表示本通信端的地址
    TBUSADDR iPeerAddr = 0x01010201;   //表示对端的地址
    int iShmkey = 1000;            //通信通道所使用的共享内存key
    int a_iChannelQueSize = 10240;  //读写队列的大小

    tbus_channel_open(&pstChannel,iLocalAddr,iPeerAddr,iShmkey,a_iChannelQueSize);

    //step3: 数据读写
    tbus_channel_send();

    ...
    tbus_channel_peek_msg();
    tbus_channel_delete_msg();


    //step4：使用完毕，释放相关资源
    tbus_channel_close(&pstChannel);
    ......
    tbus_channel_mode_fini();

     * @endcode
     */
    TSF4G_API int tbus_channel_mode_init(IN int a_iFlag);



    /**
     * @brief 当进程退出时调用此接口 释放直接通道通信模式分配的系统资源，每个进程仅需调用一次
     *
     */
    TSF4G_API void tbus_channel_mode_fini();

#define tbus_open_channel    tbus_channel_open
    /**
     * @brief 通过指定共享内存打开一个通信通道
     *
     * @param[in,out] a_ppstChannel 保存打开的通道句柄的指针
     * @param[in] a_iLocalAddr 通道本地地址，地址值通过inet_aton计算出或为自定义整数
     * @param[in] a_iPeerAddr 通道对端地址，地址值通过inet_aton计算出或为自定义整数
     * @param[in] a_iShmkey 共享内存key
     * @param[in] a_iSize 通道数据队列的大小，读写队列的大小相同
     *
     * @retval 0 -- successful,
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 通过此接口打开的通道句柄，必须调用tbus_channel_close关闭
     *
     * @pre a_ppstchannel 不能为NULL
     * @pre a_iSize 必须大于0
     *
     * @note 本接口非多线程安全
     * @note tbus_channel_open返回的句柄是非线程安全的，所有对该函数打开的句柄的操作都是非线程安全的
     * @see tbus_channel_close
     */
    TSF4G_API int tbus_channel_open(INOUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iLocalAddr,
                                    IN TBUSADDR a_iPeerAddr,IN int a_iShmkey, IN int a_iSize);

#define tbus_open_channel_ex    tbus_channel_open_ex
    /**
     * @brief 通过指定共享内存打开一个通信通道
     *
     * @param[in,out] a_ppstChannel 保存打开的通道句柄的指针
     * @param[in] a_iLocalAddr 通道本地地址，地址值通过inet_aton计算出或为自定义整数
     * @param[in] a_iPeerAddr 通道对端地址，地址值通过inet_aton计算出或为自定义整数
     * @param[in] a_iShmkey 共享内存key
     * @param[in] a_iSendSize 通道发送数据队列的大小
     * @param[in] a_iRecvSize 通道接收数据队列的大小
     *
     * @retval 0 -- successful,
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 通过此接口打开的通道句柄，必须调用tbus_channel_close关闭
     *
     * @pre a_ppstchannel 不能为NULL
     * @pre a_iSendSize 必须大于0
     * @pre a_iRecvSize 必须大于0
     *
     * @note 本接口非多线程安全
     * @note tbus_channel_open返回的句柄是非线程安全的，所有对该函数打开的句柄的操作都是非线程安全的
     * @see tbus_channel_close
     */
    TSF4G_API int tbus_channel_open_ex(INOUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iLocalAddr,
                                    IN TBUSADDR a_iPeerAddr,IN int a_iShmkey, IN int a_iSendSize, IN int a_iRecvSize);


#define tbus_open_channel_by_str    tbus_channel_open_by_str
    /**
     * @brief 通过指定共享内存打开一个通信通道 此接口提过一个tbus系统的简易实现
     *
     * @param[in,out] a_ppstChannel 保存打开的通道句柄的指针
     * @param[in] a_pszLocalAddr 通道本地地址，地址格式为IPv4网络地址 点分十进制表示串，即x.x.x.x
     * @param[in] a_pszPeerAddr 通道对端地址，地址格式为IPv4网络地址 点分十进制表示串，即x.x.x.x
     * @param[in] a_iShmkey 共享内存key
     * @param[in] a_iSize 通道数据队列的大小，读写队列的大小相同
     *
     * @retval 0 -- successful,
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 通过此接口打开的通道句柄，必须调用tbus_channel_close关闭
     * @see tbus_channel_close
     *
     * @pre a_ppstchannel 不能为NULL
     * @pre a_iSize 必须大于0
     * @pre a_pszLocalAddr 不能为NULL
     * @pre a_pszPeerAddr 不能为NULL
     *
     * @note 本接口非多线程安全
     * @note tbus_channel_open返回的句柄是非多线程安全的，所有对该句柄的操作都是非多线程安全的
     */
    TSF4G_API int tbus_channel_open_by_str(INOUT LPTBUSCHANNEL *a_ppstChannel, IN const char *a_pszLocalAddr,
                                           IN const char *a_pszPeerAddr,IN int a_iShmkey, IN int a_iSize);

#define tbus_open_channel_by_str_ex    tbus_channel_open_by_str_ex
    /**
     * @brief 通过指定共享内存打开一个通信通道 此接口提过一个tbus系统的简易实现
     *
     * @param[in,out] a_ppstChannel 保存打开的通道句柄的指针
     * @param[in] a_pszLocalAddr 通道本地地址，地址格式为IPv4网络地址 点分十进制表示串，即x.x.x.x
     * @param[in] a_pszPeerAddr 通道对端地址，地址格式为IPv4网络地址 点分十进制表示串，即x.x.x.x
     * @param[in] a_iShmkey 共享内存key
     * @param[in] a_iSendSize 通道发送数据队列的大小
     * @param[in] a_iRecvSize 通道接收数据队列的大小
     *
     * @retval 0 -- successful,
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 通过此接口打开的通道句柄，必须调用tbus_channel_close关闭
     * @see tbus_channel_close
     *
     * @pre a_ppstchannel 不能为NULL
     * @pre a_iSize 必须大于0
     * @pre a_pszLocalAddr 不能为NULL
     * @pre a_pszPeerAddr 不能为NULL
     *
     * @note 本接口非多线程安全
     * @note tbus_channel_open返回的句柄是非多线程安全的，所有对该句柄的操作都是非多线程安全的
     */
    TSF4G_API int tbus_channel_open_by_str_ex(INOUT LPTBUSCHANNEL *a_ppstChannel, IN const char *a_pszLocalAddr,
                                           IN const char *a_pszPeerAddr,IN int a_iShmkey, IN int a_iSendSize, IN int a_iRecvSize);

#define tbus_close_channel    tbus_channel_close
    /**
     * @brief 关闭一个通信通道
     *
     * @param[in,out] a_ppstChannel 保存打开的通道句柄的指针
     *
     * @note 本接口非多线程安全
     * @note a_ppstChannel所指的通道句柄必须是调用tbus_channel_open打开的
     *
     * @see tbus_channel_open
     */
    TSF4G_API void tbus_channel_close(INOUT LPTBUSCHANNEL *a_ppstChannel);

#define tbus_get_channel_outmsg_num tbus_channel_get_outmsg_num
    /**
     * @brief 查询指定的通道上的发送队列中的消息数量
     *
     * @param[in,out] a_iMsgNum    如成功返回，保存发送队列中的消息数量。
     * @param[in]     a_pstChannel 通道句柄
     *
     * @retval        0 -- successful
     * @retval        !0 -- failed, 请调用tbus_error_string接口取得错误原因
     */
    int tbus_channel_get_outmsg_num(IN LPTBUSCHANNEL a_pstChannel, OUT int* a_piMsgNum);


    /**
     * @brief 查询指定的通道上的接收队列中的消息数量
     *
     * @param[in,out] a_iMsgNum    如成功返回，保存发送队列中的消息数量。
     * @param[in]     a_pstChannel 通道句柄
     *
     * @retval        0 -- successful
     * @retval        !0 -- failed, 请调用tbus_error_string接口取得错误原因
     */
    int tbus_get_channel_inmsg_num(IN LPTBUSCHANNEL a_pstChannel, OUT int* a_piMsgNum);


    /**
     * @brief 在指定通道上发送一个数据包
     *
     * @param[in] a_pstChannel 通道句柄
     * @param[in] a_pvData 保存数据的缓冲区首地址
     * @param[in] a_iDataLen 数据长度
     * @param[in] a_iFlag 控制字段
     *
     * @retval 0 -- successful,
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 本接口非多线程安全
     * @pre a_pstChannel 不能为NULL
     *
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_channel_send(IN LPTBUSCHANNEL a_pstChannel,IN const void *a_pvData,
                                    IN const int a_iDataLen, IN const int a_iFlag);

    /**
     * @brief 将a_ptVector数组中指定的数据缓冲区的信息 组成一个数据包，发送到数据队列中。
     *
     * @param[in] a_pstChannel 通道句柄
     * @param[in] a_ptVector data vector buffers
     * @param[in] a_iVecCnt data vector count
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前本接口支持的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @see tbus_send
     * @pre a_ptVector 不能为NULL
     * @pre a_iVecCnt 必须大于0
     * @pre 待发送数据的总长度的有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_channel_sendv(IN LPTBUSCHANNEL a_pstChannel, IN const struct iovec *a_ptVector,
                                     IN const int a_iVecCnt,        IN const int a_iFlag );

    /**
     * @brief 在指定通道上接受一个数据包
     *
     * @param[in] a_pstChannel 通道句柄
     * @param[in] a_pvData 保存数据的缓冲区首地址
     * @param[in,out] a_piDataLen 保存数据长度的指针
     *    in    - 接受缓冲区最大大小
     *    out - 接受数据的实际大小
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * -  TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     *
     * @retval 0 -- successful,
     * @retval !0 --failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 本接口非多线程安全
     *
     * @see tbus_get_dyedmsgid
     * @pre a_pstChannel 不能为NULL
     * @pre a_pvData 不能为NULL
     * @pre *a_ptDataLen 必须大于0
     */
    TSF4G_API int tbus_channel_recv(IN LPTBUSCHANNEL a_pstChannel, INOUT void *a_pvData,
                                    INOUT size_t *a_ptDataLen, IN const int a_iFlag);

    /**
     * @brief 从指定数据数据通道中获取出队列头部第一个消息的存储位置和此消息的长度信息
     *
     * @param[in] a_pstChannel 通道句柄
     * @param[in,out] a_ppvData 如果成功读取到消息，则此参数为用于保存数据存储地址的指针
     * @param[out] a_piDataLen 获取数据长度的指针
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * -  TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 本接口非多线程安全
     * @note 本接口与tbus_channel_recv的不同之处在于，
     * tbus_channel_recv从数据队列中读取出数据后，会将数据从队列中移除；
     * 而本接口仅仅只是返回数据在队列中的存储位置和长度，并不会将数据从队列中移除，
     * 当此数据被处理后，请务必调用tbus_channel_delete_msg将数据从队列中移除，
     * 否则tbus_channel_peek_msg将总是读取出同一个消息。
     *
     * @see tbus_get_dyedmsgid
     * @see tbus_channel_delete_msg
     * @see tbus_channel_recv
     *
     * @pre a_ppvData 不能为NULL
     * @pre a_ptDataLen 不能为NULL
     */
    TSF4G_API int tbus_channel_peek_msg(IN LPTBUSCHANNEL a_pstChannel,
                                        INOUT const char **a_ppvData,
                                        OUT size_t *a_ptDataLen,
                                        IN const int a_iFlag);


    /**
     * @brief 将指定通道输入数据队列头部第一消息移除
     *
     * @param[in] a_pstChannel 通道句柄
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 本接口非多线程安全
     * @note 本接口与tbus_channel_peek_msg接口配合使用从队列中读取数据进行处理，
     * 由于读取的数据直接保存在数据队列中，因此可以减少一次数据拷贝过程
     *
     * @see tbus_peek_msg
     */
    TSF4G_API int tbus_channel_delete_msg(IN LPTBUSCHANNEL a_pstChannel);


    /**
     * @brief clean remaining msg from channel
     *
     * @param[in] a_pstChannel handle of channel
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @note 本接口非多线程安全
     */
    TSF4G_API int tbus_channel_clean(IN LPTBUSCHANNEL a_pstChannel);


    /**
     * @brief 获取通道中前一个消息的tbus头部，以便用于forward/backward消息
     *
     * @param[in] a_pstChannel 链接通道句柄
     * @param[in,out] a_pvBuffer 保存头部信息的缓冲区
     * @param[in,out] a_piLen 保存头部长度信息
     * - 输入    缓冲区最大可用字节数
     * - 输出    返回头部信息的实际长度
     *
     * @retval 0 -- successful, get ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_pvBuffer 不能为NULL
     * @pre a_piLen 不能为NULL，且其保存的整数值必须大于0
     */
    TSF4G_API int tbus_channel_get_bus_head(IN LPTBUSCHANNEL a_pstChannel, INOUT void *a_pvBuffer, INOUT int *a_piLen);




    /**
     * @brief 将tbus头部信息写入到通道句柄中，以便用于forward/backward消息
     *
     * @param[in] a_pstChannel 链接通道句柄
     * @param[in] a_pvBuffer 保存头部信息的缓冲区
     * @param[in] a_piLen 保存头部长度信息
     *
     * @retval 0 -- successful, set ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 本接口非多线程安全
     * @note this function should be invoked explicitly
     * @pre a_pvBuffer 不能为NULL
     */
    TSF4G_API int tbus_channel_set_bus_head(IN LPTBUSCHANNEL a_pstChannel, IN const void *a_pvBuffer, IN const int a_iLen);


    /**
     * @brief 向前转发消息。其功能与tbus_channel_send类似，
     * 其区别是tbus_channel_forward会记录消息传输过程中的路由信息，
     * 如果使用tbus_channel_backward消息可以原路返回
     *
     * @param[in] a_pstChannel    通道句柄
     * @param[in] a_pvData 保存发送数据的缓冲区支持
     * @param[in] a_iDataLen 待发送数据的长度
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @note 本接口非多线程安全
     * @note 调用本接口直接，必须通过tbus_channel_set_bus_head设置路由信息
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_channel_forward(IN LPTBUSCHANNEL a_pstChannel, IN const void *a_pvData, IN const size_t a_iDataLen,    IN const int a_iFlag);

    /**
     * @brief 将缓冲区数据组中各buff里的数据组装成一个消息向前转发消息。
     * 其功能与tbus_channel_forward类似，其区别是 tbus_channel_forward转发的是一个完整缓冲区中的消息
     *
     * @param[in] a_pstChannel    通道句柄
     * @param[in] a_ptVector data vector buffers
     * @param[in] a_iVecCnt data vector count
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @note 本接口非多线程安全
     * @note 调用本接口直接，必须通过tbus_channel_set_bus_head设置路由信息
     * @pre a_ptVector 不能为NULL
     * @pre a_iVecCnt 必须大于0
     * @pre 待发送数据的总长度的有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_channel_forwardv(IN LPTBUSCHANNEL a_pstChannel, IN const struct iovec *a_ptVector,
                                        IN const int a_iVecCnt,    IN const int a_iFlag);



    /**
     * @brief 发数据数据，与tbus_channel_send不同的是发送数据时会转发前一个消息中附带的路由信息
     *
     * @param[in] a_pstChannel    通道句柄
     *
     * @param[in] a_pvData 保存发送数据的缓冲区支持
     * @param[in] a_iDataLen 待发送数据的长度
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @note 本接口非多线程安全
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_channel_backward(IN LPTBUSCHANNEL a_pstChannel, IN const void *a_pvData,
                                        IN const size_t a_iDataLen,    IN const int a_iFlag);


    /**
     * @brief 功能与tbus_channel_backward类似，其区别是本接口将iovec数组中各缓冲区的中的消息 组装成一个消息发送
     *
     * @param[in] a_pstChannel    通道句柄
     * @param[in] a_ptVector data vector buffers
     * @param[in] a_iVecCnt data vector count
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @note 本接口非多线程安全
     * @pre a_ptVector 不能为NULL
     * @pre a_iVecCnt 必须大于0
     * @pre 待发送数据的总长度的有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_channel_backwardv(IN LPTBUSCHANNEL a_pstChannel,IN const struct iovec *a_ptVector,
                                         IN const int a_iVecCnt,  IN const int a_iFlag);



    /** @} */ /*defgroup TBUS_CHANNEL_MODE TBUS_直接基于通道的通信*/


    /** @defgroup TBUS_CHANNEL_MNG TBUS_通道管理
     * @{
     */




    /**
     * @brief 指定tbus通道本地地址，即tbus接口调用者进程的通信地址
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_szSrcAddr 本地地址，点分十进制表示的通信地址串，格式类似如: xxx.xxx.xxx.xxx.xxx
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @note 本接口会通过读写锁互斥访问GCIM共享内存中的信息，因此本接口是阻塞式的
     * @pre a_szSrcAddr 不能为NULL
     * @pre a_iHandle 为有效句柄
     * @see tbus_new
     */
    TSF4G_API int tbus_bind_by_str(IN const int a_iHandle, IN const char *a_szSrcAddr);


    /**
     * @brief 指定tbus通道本地地址，即tbus接口调用者进程的通信地址
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_iSrcAddr 本地地址，通过tbus_addr_aton接口可以获取
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @note 本接口会通过读写锁互斥访问GCIM共享内存中的信息，因此本接口是阻塞式的
     * @see tbus_new
     */
    TSF4G_API int tbus_bind(IN const int a_iHandle, IN const TBUSADDR a_iSrcAddr);


    /**
     * @brief 根据全局GCIM中的配置刷新tbus句柄管理的相关通道。
     * 如果绑定地址相关通道有添加则自动添加到tbus句柄中；如果tbus句柄管理的相关通道已经不在gcim配置中则定期回收
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @note tbus API使用者可以定时调用本接口以刷新该句柄下相关通道配置
     *
     * @pre a_iHandle 为有效句柄
     *
     * @see tbus_new
     * @see tbus_bind
     */
    TSF4G_API int tbus_refresh_handle(IN const int a_iHandle);


    /**
     * @brief 指定tbus通道的对端地址
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_szDstAddr 点分十进制表示的通信地址串，格式类似如: xxx.xxx.xxx.xxx.xxx
     *
     * @retval 0 -- successful,
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_new
     */
    TSF4G_API int tbus_connect_by_str(IN const int a_iHandle, IN const char *a_szDstAddr);


    /**
     * @brief 指定tbus通道的对端地址，接口功能与tbus_connect_by_str  相同
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_iDstAddr 对端地址，通过tbus_addr_aton()获取
     *
     * @retval 0 -- successful,
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_new
     * @see tbus_addr_aton
     */
    TSF4G_API int tbus_connect(IN const int a_iHandle, IN const TBUSADDR a_iDstAddr);


    /**
     * @brief 通道对端地址控制，禁止/开启 收发某个对端地址的通道数据
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_iDstAddr 需要控制的对端地址
     * @param[in] a_iMode 控制位，可用取以下两个值:
     * - TBUS_MODE_DISABLE    禁止从此对端地址收发数据
     * - TBUS_MODE_ENABLE    开启从此对端地址收发数据
     * @param[in] a_iBatch 通道选项表示位，可用取以下两个值：
     * - TBUS_PEER_CTRL_ONE    仅针对bus句柄所管理的第一个通道进行控制
     * - TBUS_PEER_CTRL_ALL    针对bus句柄所管理的所有通道进行控制
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     */
    TSF4G_API int tbus_peer_ctrl(IN const int a_iHandle, IN const TBUSADDR a_iDstAddr, IN const int a_iMode, IN const int a_iBatch);

    /**
     * @brief 根据通道地址获取指定通道的指针
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[out] a_ppstChannel 保存指定通道指针的指针
     * @param[in] a_iSrcAddr 通道的本地地址
     * @param[in] a_iDstAddr 通道的对端地址
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @pre a_ppstchannel 不能为NULL
     */
    TSF4G_API int tbus_get_channel(IN const int a_iHandle, OUT LPTBUSCHANNEL *a_ppstChannel, IN TBUSADDR a_iSrcAddr, IN TBUSADDR a_iDstAddr);

    /**
     * @brief 根据通道地址获取指定与该地址相关的通道的数量
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_iLocalAddr     通道的地址
     * @param[out] a_piPeerAddr    输出通道的数量
     *
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @pre a_piPeerCount 不能为NULL
     * @note a_iLocalAddr 不支持传入特殊地址TBUS_ADDR_DEFAULT/TBUS_ADDR_ALL
     */
    TSF4G_API int tbus_get_peer_count(IN int a_iHandle, IN TBUSADDR a_iLocalAddr, OUT int* a_piPeerCount);


    /**
     * @brief 根据通道地址获取指定与该地址相关的通道的地址
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     * @param[in] a_iLocalAddr      通道的地址
     * @param[out] a_piAddrBuff     保存通道地址的缓冲
     * @param[in,out] a_piAddrCount 输入数组a_piAddrBuff的大小; 输出使用的a_piAddrBuff数组的元素的数量
     *
     * @retval 0 -- successful
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_piAddrBuff 不能为NULL
     * @pre a_piAddrCount 不能为NULL
     * @note a_iLocalAddr 不支持传入特殊地址TBUS_ADDR_DEFAULT/TBUS_ADDR_ALL
     */
    TSF4G_API int tbus_get_peer_addr(IN int a_iHandle, IN TBUSADDR a_iLocalAddr, OUT TBUSADDR* a_piAddrBuff, INOUT int* a_piAddrCount);

    /**
     * @brief calc the realsize with the channel size a_iSize
     *
     * @param[in] a_iSize the real size per channel.
     * @retval the size of buffer required by the channle.
     *
     * @note 此接口内部没有考虑乘法计算可能溢出，建议a_iSize的值在范围[1, 0x70000000]内
     * @note 本接口的返回值仅具有参考意义，例如用于推断某块共享内存是否对应一个TBUS通道
     *
     */
    TSF4G_API size_t tbus_channel_calc_mem_size(IN int a_iSize);


    /**
     * @brief create a channel on the specified memory
     *
     * @note channel created by this api need to be closed by tbus_channel_close_mem
     * @see tbus_channel_open
     * @see tbus_channel_close_mem
     */
    TSF4G_API int tbus_channel_open_mem(INOUT LPTBUSCHANNEL *a_ppstChannel,
                                        IN TBUSADDR a_iLocalAddr, IN TBUSADDR a_iPeerAddr,
                                        IN void *a_pvBuff, IN size_t a_iBuffSize,
                                        int a_iNeedInit, IN int a_iQueueSize);
    /**
     * @brief close channel created by tbus_channel_open_mem
     *
     * @see tbus_channel_open_mem
     * @see tbus_channel_close
     */
    TSF4G_API void tbus_channel_close_mem(INOUT LPTBUSCHANNEL *a_ppstChannel);

    /** @} */




    /** @defgroup TBUS_DATA_TRANSFER TBUS_数据传输
     * @{
     */

    /**
     * @brief 发送数据，可以实现只对一个通道发送数据，也可以对多个通道发送此数据，
     * 具体哪些通道需发送数据由源地址和目的地址决定
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 指定发送数据的源地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的源地址，输出具体使用通道的源地址。\n
     * 对于输入， 其值分为以下三种情况：
     * - *a_piSrc == 0     此为缺省值，即使用第一个匹配目的地址的通道来发送数据
     * - *a_piSrc == -1    此为广播地址，即所有能匹配目的的地址*a_piDst的通道都发送数据
     * - 具体地址值        指定具体源地址，只有匹配此源地址的通道才可能会发送数据
     * @param[in,out] a_piDst 指定发送数据的目的地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的目的地址，输出具体使用通道的目的地址。\n
     * 对于输入，其值分为以下三种情况：
     * - *a_piDst == 0    此为缺省值，即使用第一个匹配源地址的通道来发送数据。
     *   如果此时*a_piSrc == 0, 则使用tbus句柄管理的第一个通道来发送数据。
     * - *a_piDst == -1    此为广播地址，即所有能匹配源地址的通道都发送数据。
     * - 具体地址值        指定具体目的地址，只有匹配此目的地址的通道才可能会发送数据。
     *
     * @param[in] a_pvData 保存发送数据的缓冲区支持
     * @param[in] a_iDataLen 待发送数据的长度
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_WITH_TIMESTAMP 发送消息时携带时间戳信息
     * - TBUS_FLAG_USE_GETTIMEOFDAY 仅仅在指定了TBUS_FLAG_WITH_TIMESTAMP时有意义，指示使用gettimeofday取得当前最新系统时间
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @see tbus_addr_aton
     * @see tbus_new
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 必须大于0
     * @see tbus_init_ex
     * @see tbus_use_saved_time
     * @see tbus_use_sys_time
     * @see tbus_set_current_time
     * @see tbus_turn_timestamp_on
     * @see tbus_turn_timestamp_off
     * @pre a_iDataLen 有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_send(IN const int a_iHandle, INOUT TBUSADDR *a_piSrc,
                            INOUT TBUSADDR *a_piDst, IN const void *a_pvData,
                            IN const size_t a_iDataLen, IN const int a_iFlag);


    /**
     * @brief Send data which stored at multiple buffers
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in,out] a_piSrc -- see tbus_send()
     * @param[in,out] a_piDst -- see tbus_send()
     * @param[in] a_ptVector data vector buffers
     * @param[in] a_iVecCnt data vector count
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_WITH_TIMESTAMP 发送消息时携带时间戳信息
     * - TBUS_FLAG_USE_GETTIMEOFDAY 仅仅在指定了TBUS_FLAG_WITH_TIMESTAMP时有意义，指示使用gettimeofday取得当前最新系统时间
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @see tbus_send
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_ptVector 不能为NULL
     * @pre a_iVecCnt 必须大于0
     * @pre 待发送数据的总长度的有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_sendv(IN const int a_iHandle,
                             INOUT TBUSADDR *a_piSrc,
                             INOUT TBUSADDR *a_piDst,
                             IN const struct iovec *a_ptVector,
                             IN const int a_iVecCnt,
                             IN const int a_iFlag);


    /**
     * @brief 从通道中读取出一个消息，可以只从某一个指定通道接收数据，也可尝试从多个通道中读取数据。
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 指定发送数据的源地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的源地址，输出实际读取数据的通道的源地址。\n
     * 对于输入，其值分为以下两种情况：
     * - 具体地址值         指定具体源地址，只有匹配此源地址的通道才可能会接收数据
     * - 特殊地址(0 | -1)   匹配任意地址。
     * @param[in,out] a_piDst 指定数据的目的地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的目的地址， 输出实际读取数据的通道的目的地址。\n
     * 对于输入，其值分为以下两种情况：
     * - 具体地址值        指定具体目的地址，只有匹配此目的地址的通道才可能会接收数据
     * - 特殊地址(0 | -1)  匹配任意地址。
     *
     * @param[in] a_pvData 保存接收数据的缓冲区指针
     * @param[in,out] a_piDataLen 待发送数据的长度:
     * - 输入    缓冲区最大可用长度
     * - 输出    实际接收数据的长度
     * @param[in] a_iFlag 控制字段，各控制标志支持'或(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     *
     * @retval 0  -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @note 如果*a_piSrc或*a_piDst是特殊的tbus地址，则由tbus根据如下(伪代码)规则决定从哪个通道上收取数据。
     * - 注意，为了防止通道"饥饿"，加入了连续从一个通道收取的消息数量的限制(TBUS_MAX_RECV_CNT，当前值为10)。
     * @code
     for(i = 0; i < a_iHandle.channelCount; i++)
     {
     channel = getChannel(a_iHandle.recvPos);
     if(channel NOT matches <*a_piSrc, *a_piDst>)
     {
     a_iHandle.recvPos =(a_iHandle.recvPos + 1)% a_iHandle.channelCount;
     a_iHandle.recvedCount = 0;
     continue;
     }
     iRet = recvData(channel);
     if(TBUS_SUCCESS != iRet)
     {
     if(isSpecialAddress(*a_piSrc)|| isSpecialAddress(*a_piDst))
     {
     a_iHandle.recvPos =(a_iHandle.recvPos + 1)% a_iHandle.channelCount;
     a_iHandle.recvedCount = 0;
     continue;
     } else {
     break;
     }
     }
     if(isSpecialAddress(*a_piSrc)|| isSpecialAddress(*a_piDst))
     {
     a_iHandle.recvedCount++;
     if(TBUS_MAX_RECV_CNT <= a_iHandle.recvedCount)
     {
     a_iHandle.recvPos =(a_iHandle.recvPos + 1)% a_iHandle.channelCount;
     a_iHandle.recvedCount = 0;
     }
     }
     break;
     }
     @endcode
     *
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 必须大于0
     */
    TSF4G_API int tbus_recv(IN const int a_iHandle,    INOUT TBUSADDR *a_piSrc,    INOUT TBUSADDR *a_piDst,
                            INOUT void *a_pvData,    INOUT size_t *a_ptDataLen,    IN const int a_iFlag);

    /**
     * @brief 从通道中读取出一个消息
     *
     * @param[in, out] a_pstRecv 传入传出参数结构
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @pre a_pstRecv 不能为NULL
     * @note 定义TBUSRECV类型的变量后，必须先调用 memset 初始化这个变量全部成员为0，然后再设置关注的字段值
     * @note a_pstRecv指向的结构的各个成员的意义可参考LPTBUSRECV的定义，以及tbus_recv函数参数的意义
     * @see tbus_recv
     */
    TSF4G_API int tbus_recv_ex(INOUT LPTBUSRECV a_pstRecv);

    /**
     * @brief 从数据通道中获取出队列头部第一个消息的存储位置和此消息的长度信息，
     * 本接口实现只从某一个指定通道读取数据，也可以对尝试从从多个通道中读取数据，
     * 具体选择那个通道接收消息，有参数中指定的源，目的地址确定
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 指定发送数据的源地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的源地址，输出实际读取数据的通道的源地址。\n
     * 对于输入，其值分为以下两种情况：
     * - 具体地址值         指定具体源地址，只有匹配此源地址的通道才可能会接收数据
     * - 特殊地址(0 | -1)   匹配任意地址。
     * @param[in,out] a_piDst 指定数据的目的地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的目的地址， 输出实际读取数据的通道的目的地址。\n
     * 对于输入，其值分为以下两种情况：
     * - 具体地址值        指定具体目的地址，只有匹配此目的地址的通道才可能会接收数据
     * - 特殊地址(0 | -1)  匹配任意地址。
     *
     * @param[in,out] a_ppvData 如果成功读取到消息，则此参数为用于保存数据存储地址的指针
     * @param[out] a_piDataLen 获取数据长度的指针
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * -  TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @note 本接口与tbus_recv的不同之处在于
     * - tbus_recv从数据队列中读取出数据后，会将数据从队列中移除；
     * - 本接口仅仅只是返回数据在队列中的存储位置和长度，并不会将数据从队列中移除，
     *   当此数据被处理后，请务必调用tbus_delete_msg将数据从队列中移除，
     *   否则tbus_peek_msg将总是读取出同一个消息。
     *
     * @see tbus_recv
     * @see tbus_delete_msg
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_ppvData 不能为NULL
     * @pre a_ptDataLen 不能为NULL
     */
    TSF4G_API int tbus_peek_msg(IN const int a_iHandle,    INOUT TBUSADDR *a_piSrc,    INOUT TBUSADDR *a_piDst,
                                INOUT const char **a_ppvData,    OUT size_t *a_ptDataLen,    IN const int a_iFlag);


    /**
     * @brief 将指定通道输入数据队列头部第一消息移除
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in] a_iSrc 指定数据的源地址，通过tbus_addr_aton()转换而来。
     * @param[in] a_iDst 指定数据的目的地址，通过tbus_addr_aton()转换而来。
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note 本接口与tbus_peek_msg接口配合使用从队列中读取数据进行处理，
     * 由于读取的数据直接保存在数据队列中，因此可以减少一次数据拷贝过程。
     * @note 建议使用 tbus_peek_msg 的两个地址参数的传出值作为本接口的两个地址参数值。
     *
     * @see tbus_peek_msg
     */
    TSF4G_API int tbus_delete_msg(IN const int a_iHandle,    IN TBUSADDR a_iSrc,    IN TBUSADDR a_iDst);



    /**
     * @brief 向前转发消息，其功能与tbus_send类似。
     * 其区别是tbus_forward会记录消息传输过程中的路由信息，
     * 这样如果使用tbus_backward消息可以原路返回。
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 指定发送数据的源地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的源地址， 输出具体使用通道的源地址，通过tbus_addr_aton()转换而来。\n
     * 对于输入，其值分为以下三种情况：
     * - *a_piSrc == 0     此为缺省值，即使用第一个匹配目的地址的通道来发送数据
     * - *a_piSrc == -1    此为广播地址，即所有能匹配目的的地址*a_piDst的通道都发送数据。
     * - 具体地址值        指定具体源地址，只有匹配此源地址的通道才可能会发送数据。
     * @param[in,out] a_piDst 指定发送数据的目的地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的目的地址，输出具体使用通道的目的地址。\n
     * 对于输入，有如下三种情况：
     * - *a_piDst == 0    此为缺省值，即使用第一个匹配源地址的通道来发送数据。
     *   如果此时*a_piSrc == 0, 则使用tbus句柄管理的第一个通道来发送数据。
     * - *a_piDst == -1    此为广播地址，即所有能匹配源地址的通道都发送数据
     * - 具体地址值        指定具体目的地址，只有匹配此目的地址的通道才可能会发送数据
     *
     * @param[in] a_pvData 保存发送数据的缓冲区支持
     * @param[in] a_iDataLen 待发送数据的长度
     *
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @see tbus_backward
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     *
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_forward(IN const int a_iHandle,    INOUT TBUSADDR *a_piSrc,    INOUT TBUSADDR *a_piDst,
                               IN const void *a_pvData,    IN const size_t a_iDataLen,    IN const int a_iFlag);



    /**
     * @brief 其功能与tbus_forward类似，区别是本接口将iovec各缓冲区的内存组装成一个消息发送
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 参考 tbus_forward
     * @param[in,out] a_piDst 参考 tbus_forward
     * @param[in] a_ptVector data vector buffers
     * @param[in] a_iVecCnt data vector count
     *
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @see tbus_forward
     * @see tbus_backward
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_ptVector 不能为NULL
     * @pre a_iVecCnt 必须大于0
     * @pre 待发送数据的总长度的有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_forwardv(IN const int a_iHandle,    INOUT TBUSADDR *a_piSrc,    INOUT TBUSADDR *a_piDst,
                                IN const struct iovec *a_ptVector,  IN const int a_iVecCnt,  IN const int a_iFlag);



    /**
     * @brief 延tbus头部记录的路由信心，向后转发消息
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 指定发送数据的源地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的源地址， 输出体使用通道的源地址。\n
     * 对于输入，其值分为以下三种情况：
     * - *a_piSrc == 0     此为缺省值，即使用第一个匹配目的地址的通道来发送数据
     * - *a_piSrc == -1    此为广播地址，即所有能匹配目的的地址*a_piDst的通道都发送数据
     * - 具体地址值        指定具体源地址，只有匹配此源地址的通道才可能会发送数据
     * @param[in,out] a_piDst 指定发送数据的目的地址，通过tbus_addr_aton()转换而来。\n
     * 输入指定数据的目的地址， 输出体使用通道的目的地址。\n
     * 对于输入，其值分为以下三种情况：
     * - *a_piDst == 0     此为缺省值，自动取上一个接收消息中记录的路由地址信息做为目的地址
     * - *a_piDst == -1    此为广播地址，即所有能匹配源地址的通道都发送数据
     * - 具体地址值        指定具体目的地址，只有匹配此目的地址的通道才可能会发送数据
     *
     * @param[in] a_pvData 保存发送数据的缓冲区支持
     * @param[in] a_iDataLen 待发送数据的长度
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @pre \e 本接口要正常工作，必须之前以收到过一个通过tbus_forward发过来的数据包
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_pvData 不能为NULL
     * @pre a_iDataLen 有效取值范围 [1, 0x7FFFFFFF]
     */
    TSF4G_API int tbus_backward(IN const int a_iHandle,    INOUT TBUSADDR *a_piSrc,    INOUT TBUSADDR *a_piDst,
                                IN const void *a_pvData,    IN const size_t a_iDataLen,    IN const int a_iFlag);


    /**
     * @brief 其功能与tbus_backward类似，区别是本接口将iovec中各缓冲区中的数据组装成一个消息发送
     *
     * @param[in] a_iHandle tbus句柄，通过调用tbus_new()获取
     * @param[in,out] a_piSrc 参考 tbus_backward
     * @param[in,out] a_piDst 参考 tbus_backward
     * @param[in] a_ptVector data vector buffers
     * @param[in] a_iVecCnt data vector count
     * @param[in] a_iFlag 控制字段，各控制标志支持'与(|)'方式结合使用，目前已定义的标志位有：
     * - TBUS_FLAG_START_DYE_MSG    对发送消息进行染色跟踪
     * - TBUS_FLAG_KEEP_DYE_MSG    继承上一个消息的染色跟踪的功能，即如果上一个接收的消息是染色的，则此发送消息也进行染色
     *
     * @retval 0 -- successful, sent ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @see tbus_get_dyedmsgid
     * @pre \e 本接口要正常工作，必须之前以收到过一个通过tbus_forward发过来的数据包
     * @pre a_piSrc 不能为NULL
     * @pre a_piDst 不能为NULL
     * @pre a_ptVector 不能为NULL
     * @pre a_iVecCnt 必须大于0
     * @pre 待发送数据的总长度的有效取值范围 [1, 0x7FFFFFFF]
     * @see tbus_backward
     */
    TSF4G_API int tbus_backwardv(IN const int a_iHandle,    INOUT TBUSADDR *a_piSrc,    INOUT TBUSADDR *a_piDst,
                                 IN const struct iovec *a_ptVector,  IN const int a_iVecCnt,     IN const int a_iFlag);



    /**
     * @brief 获取上一个染色消息的特征码。
     * 当调用tbus系统接口的上层应用对发送/接收的消息进行染色跟踪后，立即调用此接口，
     * 可以获取区别与其他染色消息的特数码，上层应用记录此特征码，就可保持对染色消息跟踪。
     *
     * @retval 染色消息的特征码
     */
    TSF4G_API unsigned int tbus_get_dyedmsgid();

    /**
     * @brief 获取通道两端队列上剩余消息的字节总数
     *
     * @param[in] a_hHandle tbus句柄
     * @param[in] a_iLocalAddr 此tbus通道相对于本端的地址
     * @param[in] a_iPeerAddr 此tbus通道相对于对端的地址
     * @param[in] a_piInFlux 获取输入通道(a_iLocalAddr <-- a_iPeerAddr)剩余消息字节数的指针
     * @param[in] a_piOutFlux 获取输出通道(a_iLocalAddr --> a_iPeerAddr)剩余消息字节数的指针
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     */
    TSF4G_API int tbus_get_channel_flux(IN int a_hHandle, IN TBUSADDR a_iLocalAddr, IN TBUSADDR a_iPeerAddr,
                                        OUT int *a_piInFlux, OUT int *a_piOutFlux);

    /** @} */

    /** @defgroup TBUS_TIMESTAMP_AND_HEARTBEAT TBUS_消息时间戳机制和心跳机制
     * @{
     */
    /**
     * @brief 设置当前时间
     *
     * @param[in] a_pstTime
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @pre a_pstTime不能为NULL
     */
    TSF4G_API int tbus_set_current_time(IN const struct timeval *a_pstTime);

    /**
     * @brief 使用通过tbus_set_current_time设置的时间作为时间戳的初值以及心跳机制的当前时间
     *
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @note 如果应用调用了本接口，需要定时调用tbus_set_current_time刷新tbus保存的当前时间。
     * - 相对于每次发包都使用gettimeofday(可以通过调用tbus_use_sys_time设置)而言，
     * - tbus_use_saved_time牺牲了时间精确度，提高了程度执行效率。
     * @see tbus_use_sys_time
     * @see tbus_init_ex
     */
    TSF4G_API int tbus_use_saved_time(void);

    /**
     * @brief 使用通过gettimeofday获取的时间作为时间戳的初值以及心跳机制的当前时间
     *
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     * @see tbus_use_saved_time
     * @see tbus_init_ex
     */
    TSF4G_API int tbus_use_sys_time(void);

    /**
     * @brief 获取前一个收取的消息的时间戳信息
     *
     * @param[in] a_iHandle       指定tbus句柄，句柄可由tbus_new获取
     * @param[out] a_pstTimeStamp 接收返回的时间戳信息
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_iHandle必须是一个有效的句柄
     * @pre a_pstTimeStamp不能为NULL
     */
    TSF4G_API int tbus_get_msg_timestamp(IN int a_iHandle, OUT LPTBUSTIMESTAMP a_pstTimeStamp);

    /**
     * @brief 打开一个通道发送消息时记录时间戳的功能
     *
     * @param[in] a_iHandle       指定tbus句柄，句柄可由tbus_new获取
     * @param[in] a_iAddr1        通道一端的地址
     * @param[in] a_iAddr2        通道另一端的地址
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_iHandle必须是一个有效的句柄
     * @pre a_iAddr1必须是一个特定的地址，不能为特殊地址(0,-1)
     * @pre a_iAddr2必须是一个特定的地址，不能为特殊地址(0,-1)
     * @see tbus_turn_timestamp_off
     * @see tbus_set_handle_opt
     * @see tbus_send
     * @see tbus_sendv
     * @see tbus_forward
     * @see tbus_backward
     */
    TSF4G_API int tbus_turn_timestamp_on(IN int a_iHandle, IN TBUSADDR a_iAddr1, IN TBUSADDR a_iAddr2);

    /**
     * @brief 关闭一个通道发送消息时记录时间戳的功能
     *
     * @param[in] a_iHandle       指定tbus句柄，句柄可由tbus_new获取
     * @param[in] a_iAddr1        通道一端的地址
     * @param[in] a_iAddr2        通道另一端的地址
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_iHandle必须是一个有效的句柄
     * @pre a_iAddr1必须是一个特定的地址，不能为特殊地址(0,-1)
     * @pre a_iAddr2必须是一个特定的地址，不能为特殊地址(0,-1)
     * @see tbus_turn_timestamp_on
     * @see tbus_set_handle_opt
     * @see tbus_send
     * @see tbus_sendv
     * @see tbus_forward
     * @see tbus_backward
     */
    TSF4G_API int tbus_turn_timestamp_off(IN int a_iHandle, IN TBUSADDR a_iAddr1, IN TBUSADDR a_iAddr2);

    /**
     * @brief 取得本地保存的a_iPeerAddr对应的进程的状态信息
     *
     * @param[out] a_pstPeerState 指向接收状态信息的本地变量
     * @param[in]  a_iHandle      指定tbus句柄，句柄可由tbus_new获取
     * @param[in]  a_iPeerAddr    指定通道对端进程的地址
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_pstPeerState不能为NULL
     * @pre a_iPeerAddr必须是一个特定的地址，不能为特殊地址(0,-1)
     * @note 当应用通过tbus_bind/tbus_bind_by_string绑定某个通道时，
     * 通道的对应的stPeerState的llLastBeatTime将被设置为系统的当前时间(通过gettimeofday取得)，
     * 通道的对应的stPeerState的iIsAlive被设置为1, 表明对端进程正在运行。
     * 虽然这种处理并不是太合理，但是可以避免进程刚刚启动时就查询到iIsAlive==0的情况发生。
     * 如果应用层需要在进程之间(通过tbus)交换某些消息后，才认为对方进程正在运行，
     * 那么使用TBUSPEERSTATE.iIsAlive的值作为进程是否正在运行的判断标准就不太合理。
     * 实际上，由于tbus通道的控制通道目前还没有实现，此接口返回的TBUSPEERSTATE.iIsAlive始终==1.
     */
    TSF4G_API int tbus_get_peer_state_detail(OUT LPTBUSPEERSTATE a_pstPeerState, IN int a_iHandle, IN TBUSADDR a_iPeerAddr);

    /**
     * @brief 取得a_iHandle句柄下下一个通道的对端地址
     *
     * @param[in]  a_iHandle      指定tbus句柄，句柄可由tbus_new获取
     * @param[in, out] a_piPeerAddr   输入0或前一次调用tbus_get_next_peer_addr返回的地址，输出下一个通道的对端地址
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_piPeerAddr不能为NULL
     * @see tbus_get_first_peer_addr
     * @note 传入*a_piPeerAddr == 0, 取得第一个peer的地址
     */
    TSF4G_API int tbus_get_next_peer_addr(IN int a_iHandle, INOUT TBUSADDR *a_piPeerAddr);

    /**
     * @brief 设置本地保存的a_iPeerAddr对应的进程的超时时间
     *
     * @param[in]  a_iHandle      指定tbus句柄，句柄可由tbus_new获取
     * @param[in]  a_iPeerAddr    指定通道对端进程的地址
     * @param[in]  a_iTimeout     指定超时时间
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_iPeerAddr必须是一个特定的地址，不能为特殊地址(0,-1)
     */
    TSF4G_API int tbus_set_peer_timeout_gap(IN int a_iHandle, IN TBUSADDR a_iPeerAddr, IN int a_iTimeout);

    /**
     * @brief 查询本地保存的a_iPeerAddr对应的进程是否超时
     *
     * @param[out] a_piPreState   指向接收前一个状态信息的本地变量
     * @param[out] a_piCurState   指向接收当前状态信息的本地变量
     * @param[in]  a_iHandle      指定tbus句柄，句柄可由tbus_new获取
     * @param[in]  a_iPeerAddr    指定通道对端进程的地址
     * @param[in]  a_iNeedGetCurTime　指示tbus是否使用gettimeofday取得当前时间
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_iPeerAddr必须是一个特定的地址，不能为特殊地址(0,-1)
     * @note 取得的进程状态是enum类型tagTbusPeerStateType的值
     * @note 当返回的*a_piPreState!=*a_piCurState时，
     * - 对tbus_get_peer_states的调用会更新peer的状态，
     * - 用*a_piCurState的值覆盖*a_piPreState的值。
     * @see tbus_init_ex
     * @see tbus_use_saved_time
     * @see tbus_use_sys_time
     */
    TSF4G_API int tbus_get_peer_states(OUT int *a_piPreState, OUT int *a_piCurState, IN int a_iHandle,
                                       IN TBUSADDR a_iPeerAddr, IN int a_iNeedGetCurTime);

    /**
     * @brief 向a_iPeerAddr对应的进程的发送心跳消息
     *
     * @param[in]  a_iHandle      指定tbus句柄，句柄可由tbus_new获取
     * @param[in]  a_iPeerAddr    指定通道对端进程的地址
     * @param[in]  a_iFlag        接口操作标志位:
     * - TBUS_INIT_FLAG_USE_SAVED_TIME
     * 所有tbus时间戳和心跳机制需要使用系统时间时，使用通过tbus_set_current_time保存的时间。
     * - TBUS_FLAG_START_DYE_MSG & TBUS_FLAG_KEEP_DYE_MSG
     * 发送的消息需要进行染色跟踪
     * - TBUS_FLAG_SET_TEST_MSG
     * 测试消息标志位

     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_iPeerAddr不能为特殊地址0(TBUS_ADDR_DEFAULT)
     */
    TSF4G_API int tbus_send_heartbeat(IN int a_iHandle, IN TBUSADDR a_iPeerAddr, IN int a_iFlag);

    /** @} */



    /** @defgroup TBUS_ROUTE_MNG TBUS_路由信息管理
     * @{
     */


    /**
     * @brief get current package's route information
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in,out] a_ptRouteVec vector to be stored route information, the first one is the latest route
     * @param[in,out] a_piCnt route vector count, if get package route ok, this value would be set to real route count
     *
     * @retval 0 -- successful, get route information ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_ptRouteVec 不能为NULL
     * @pre a_piCnt 不能为NULL，且其保存的整数值必须大于0
     */
    TSF4G_API int tbus_get_pkg_route(IN const int a_iHandle, INOUT HEADROUTE *a_ptRouteVec, INOUT int *a_piCnt);

    /**
     * @brief set current package's route information
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_ptRouteVec vector to store route information, the first one is the latest route
     * @param[in] a_iCnt route vector count, indicated how many routes should be set into package header
     *
     * @retval 0 -- successful, set route information ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @pre a_ptRouteVec 不能为NULL
     * @pre a_iCnt 其保存的整数值必须大于0
     */
    TSF4G_API int tbus_set_pkg_route(IN const int a_iHandle, IN const HEADROUTE *a_ptRouteVec, IN const int a_iCnt);




#define tbus_save_pkg_header    tbus_save_bus_head
    /**
     * @brief saved current package's header into internal buffer
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     *
     * @retval 0 -- successful, saved ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note this function should be invoked explicitly
     */
    TSF4G_API int tbus_save_bus_head(IN const int a_iHandle);

#define tbus_restore_pkg_header    tbus_restore_bus_head
    /**
     * @brief restored package's header
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     *
     * @retval 0 -- successful, restored ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note this function should be invoked explicitly
     */
    TSF4G_API int tbus_restore_bus_head(IN const int a_iHandle);


#define tbus_get_pkg_header    tbus_get_bus_head
    /**
     * @brief get current package's header and save it into external buffer
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in,out] a_pvBuffer value to be stored return header buffer
     * @param[in,out] a_piLen buffer length as in value, and if get header successfully, this value would be set as real buffer length
     *
     * @retval 0 -- successful, get ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note this function should be invoked explicitly
     * @pre a_pvBuffer 不能为NULL
     * @pre a_piLen 不能为NULL，且其保存的整数值必须大于0
     */
    TSF4G_API int tbus_get_bus_head(IN const int a_iHandle, INOUT void *a_pvBuffer, INOUT int *a_piLen);


#define tbus_set_pkg_header    tbus_set_bus_head
    /**
     * @brief set current package's header via specified external data
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_pvBuffer value to store header buffer
     * @param[in] a_iLen buffer length
     *
     * @retval 0 -- successful, set ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note this function should be invoked explicitly
     * @pre a_pvBuffer 不能为NULL
     * @pre a_iLen 其保存的整数值必须大于0
     */
    TSF4G_API int tbus_set_bus_head(IN const int a_iHandle, IN const void *a_pvBuffer, IN const int a_iLen);

    /**
     * @brief get and delete previous node's bus address from bus head of current package
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in,out] a_piAddr pointer to output buffer
     *
     * @retval 0 -- successful, set ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note this function should be invoked explicitly
     * @pre a_piAddr 不能为NULL
     */
    TSF4G_API int tbus_pop_previous_addr(IN const int a_iHandle, INOUT TBUSADDR* a_piAddr);

    /**
     * @brief get destination address list via specified external data
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in,out] a_piDstList vector buffer to store destination list
     * @param[in,out] a_piVecCnt  vector count as in value, and if get list successfully, this value would be set as real destination address count
     *
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     * @note this function is added at 2008/03/27 to meet FO2's needs
     * @pre a_piDstList 不能为NULL
     * @pre a_piVecCnt 不能为NULL，且其保存的整数值必须大于0
     */
    TSF4G_API int tbus_get_dst_list(IN const int a_iHandle, INOUT unsigned int *a_piDstList, INOUT int *a_piVecCnt);

    /** @} */


    /** @defgroup TBUS_MONITOR TBUS_通道状态监测和控制
     * @{
     */

    /**
     * 建议下面这些接口在对通道进行监控和修复时使用，正常通信时谨慎使用这些接口
     */

    /**
     * @brief get the used proportion of channel recvside queue
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_iPeerAddr 对方bus地址，消息源地址，不支持特殊地址
     * @param[in] a_iLocalAddr 本地bus地址，消息目的地址，不支持特殊地址
     * @param[out] a_pfUsed 获取到的队列空间使用比例，0~1之间的浮点型。不能为NULL
     *
     * @retval 0    SUCCESS
     *         !0   FAILED
     */
    TSF4G_API int tbus_get_channel_recvside_used_proportion(IN const int a_iHandle, IN TBUSADDR a_iPeerAddr, IN TBUSADDR a_iLocalAddr, OUT float* a_pfUsed);


    /**
     * @brief get the used proportion of channel sendside queue
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_iLocalAddr 本地bus地址，消息源地址，不支持特殊地址
     * @param[in] a_iPeerAddr 对方bus地址，消息目的地址，不支持特殊地址
     * @param[out] a_pfUsed 获取到的队列空间使用比例，0~1之间的浮点型。不能为NULL
     *
     * @retval 0    SUCCESS
     *         !0   FAILED
     */
    TSF4G_API int tbus_get_channel_sendside_used_proportion(IN const int a_iHandle, IN TBUSADDR a_iLocalAddr, IN TBUSADDR a_iPeerAddr, OUT float* a_pfUsed);

    /**
     * @brief empty the channel recvside queue
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_iPeerAddr 对方bus地址，消息源地址，不支持特殊地址
     * @param[in] a_iLocalAddr 本地bus地址，消息目的地址，不支持特殊地址
     *
     * @retval 0    SUCCESS
     *         !0   FAILED
     */
    TSF4G_API int tbus_empty_channel_recvside(IN const int a_iHandle, IN TBUSADDR a_iPeerAddr, IN TBUSADDR a_iLocalAddr);


    /**
     * @brief empty the channel sendside queue
     *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_iLocalAddr 本地bus地址，消息源地址，不支持特殊地址
     * @param[in] a_iPeerAddr 对方bus地址，消息目的地址，不支持特殊地址
     *
     * @note 此函数非线程安全，建议由通信的对方进程调用 tbus_empty_channel_recvside 清空消息队列
     *
     * @retval 0    SUCCESS
     *         !0   FAILED
     */
    TSF4G_API int tbus_empty_channel_sendside(IN const int a_iHandle, IN TBUSADDR a_iLocalAddr, IN TBUSADDR a_iPeerAddr);
    /** @} */

    /** @defgroup TBUS_LOG TBUS_日志处理
     * @{
     */

    /**
     * @brief set log category instance for tbus log system
     *
     * @param[in] a_pstLogCat category instance of tbus log system
     *
     */
    TSF4G_API void tbus_set_logcat(IN LPTLOGCATEGORYINST a_pstLogCat);

    /**
     * @brief set priority  of log category  used by tbus
     *
     * @param[in] a_iPriority Priority  log category  used by tbus
     */
    TSF4G_API void tbus_set_logpriority(IN int a_iPriority);

    /**
     * @brief 设置使用自动化-集群模式时，zookeeper api的日志文件句柄
     * @note 如果不调用本接口，那么默认使用/tmp/tbus_zk.log
     *       仅在链接libtsf4g_zk.a或者libtbus_zk.a时可以使用本接口，否则会报链接错误
     *       本接口需要在tbus_automatic_init之前调用, 且只能调用一次
     * @param[in] a_pszLogFile 日志文件的文件路径
     * @retval 0  SUCCESS
     *         !0 FAILED
     */
    TSF4G_API int tbus_automatic_set_zookeeper_log_file(IN const char* a_pszLogFile);

    /** @} */

	
	/** @defgroup TBUS_MSG_NOTIFY TBUS_消息通知机制
     * @{
     */

    /**
     * @brief 通知对端及时接收消息
	 *
	 * @note 此函数只适用于自动化模式，参考tbus_automatic_init的说明
	 * @note 在(一次或者多次)调用tbus_send，tbus_sendv，tbus_forward，
	 *       tbus_forwardv，tbus_backward, tbus_backwardv发送消息之后,
	 *       如果需要通知对方及时接收消息，调用此函数通知。	 
	 *
	 * @see tbus_proc_events
	 *
     * @param[in] a_iHandle tbus handle, acquired by tbus_new()
     * @param[in] a_iSrc 指定数据的源地址，参考tbus_send的参数说明
     * @param[in] a_iDst 指定数据的目的地址，参考tbus_send的参数说明
     *
     * @retval 0 -- successful, send notify ok
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     */
    TSF4G_API int tbus_send_notify(IN const int a_iHandle, IN TBUSADDR a_iSrc, IN TBUSADDR a_iDst);

	/**
     * @brief 获取事件管理连接的描述符
	 *
	 * @note 此函数只适用于自动化模式，参考tbus_automatic_init的说明
	 *
     * @retval >=0  -- UDP socketfd
     * @retval <0 -- failed, 请调用tbus_error_string接口取得错误原因
	 *
	 * @note 如果通过此函数取得的描述符被注册到EPOLL中，只支持水平触发方式。
     *
     */
    TSF4G_API int tbus_get_event_fd();

	/**
     * @brief 检测到 事件管理连接的描述符 有事件发生时，应调用此函数处理，
	 * 此函数会从事件管理连接接收通知，如果是消息通知，能提取出源地址和目的地址并
	 * 设置输出参数a_piSrc, a_piDst，无消息通知或出错时设置输出参数a_piSrc, a_piDst为通配地址。
	 *
	 * @note 此函数只适用于自动化模式，参考tbus_automatic_init的说明
	 * @note 此函数提供不可靠的服务，由于通道中的数据可能在处理消息通知前就被取出，
     *       当函数返回TBUS_HAVE_MESSAGE，可能并没有数据可接收。
	 *       由于通知消息可能有多个，需要全部处理，建议在while中调用此函数，当返回值为1时继续循环,
	 *       出错或者返回值为0时结束循环。
	 *
	 * @param[out] a_piSrc 输出发送数据的源地址，通过tbus_addr_aton()转换而来。
     * @param[out] a_piDst 输出数据的目的地址，通过tbus_addr_aton()转换而来。
	 * @param[in] events，保留参数，目前必须设置为 0
	 *
     * @retval 0 -- successful, have no message
     * @retval 1(TBUS_HAVE_MESSAGE) -- successful, has message, and set a_piSrc,a_piDst
     * @retval <0 || >1 -- failed, 请调用tbus_error_string接口取得错误原因
	 *
	 * @note 当返回值等于 TBUS_ERR_MAKE_ERROR(TBUS_ERROR_NOTIFY_SOCKETFD_RESET) 时....
     *
     */
    TSF4G_API int tbus_proc_events(OUT TBUSADDR* a_piSrc, OUT TBUSADDR* a_piDst, IN int events);

    /**
     * @brief tbusd存储了客户端的tbus地址和udp socket地址映射信息，需要定时更新地址映射表。
     * 定时向tbusd发送地址表，可以屏蔽 tbusd重启、进程迁移对事件机制的影响，保证事件机制的有效性。 
	 *
	 * @note 为了保证事件机制的有效性，需要定期调用此函数，建议每秒至少调用一次。
     *
	 * @note 此函数只适用于自动化模式，参考tbus_automatic_init的说明
     *
     * @param[in] a_iHandle tbus处理句柄，通过调用tbus_new()获取
     *
     * @retval 0 -- successful
     * @retval !0 -- failed, 请调用tbus_error_string接口取得错误原因
     *
     */
    TSF4G_API int tbus_event_keep_alive(IN const int a_iHandle);

	/** @} */


#ifdef __cplusplus
}
#endif


#endif /**< _TBUS_H */

