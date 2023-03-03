/**
 *
 * @file     tapp.h
 * @brief    TAPP主头文件
 *
 * @author jackyai flyma
 * @version 1.0
 * @date 2009-9-29
 *
 * Copyright (c)  2008-2009, 腾讯科技有限公司互动娱乐研发部
 * All rights reserved.
 *
 */

#ifndef TAPP_H
#define TAPP_H

#include "tapp/tapp_error.h"

#include "pal/pal.h"
#include "tbus/tbus.h"
#ifdef TAPP_TDR
#include "tdr/tdr.h"
#endif
#include "tlog/tlog.h"

/* version pattern: major.minor.build */
#define TAPP_MAKE_VERSION(major, minor, rev, build)    (((unsigned long long)(major))<<56 |((unsigned long long) (minor))<<48 | ((unsigned long long)(rev))<<32 | (build) )
#define TAPP_GET_MAJOR(ver)                        ((ver)>>56 & 0xff)
#define TAPP_GET_MINOR(ver)                        ((ver)>>48 & 0xff)
#define TAPP_GET_REV(ver)                        ((ver)>>32 & 0xffff)
#define TAPP_GET_BUILD(ver)                        ((ver) & 0xffffffff)

//BUS相关配置项
#define TAPP_REFRESH_TBUS_CONFIGRE_DEFAULT_TIMER    60000    /*the default timer used by tapp to refresh tbus configure*/
#define TAPP_TBUS_EVENT_KEEP_ALIVE_DEFAULT_TIMER    1000    /*the default timer used by tapp to call keep_tbus_event_alive*/

/* 启动模式相关配置 */
#define TAPP_START_MODE_INIT      0
#define TAPP_START_MODE_RESUME    1

//Win23 lib problem
#if defined(_WIN32) || defined(_WIN64)
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
#pragma comment(lib, "libtapp_d.lib")
#else
#pragma comment(lib, "libtapp.lib")
#endif

#endif

#endif

#ifdef __cplusplus
extern "C"
{
#endif

    struct tagTappData
    {
        // The buffer for user to write data,Can be casted to user structure.
        char* pszBuff;

        // The length of buffer,You should use assertion here.
        size_t iLen;

        // The meta name of tapp data
        char* pszMetaName;

        // The pointer to the meta
        intptr_t iMeta;
        void* iLib;
    };

    typedef struct tagTappData TAPPDATA;
    typedef struct tagTappData *LPTAPPDATA;

    struct tagTAPPCTX;
    struct tagTappThreadConf;

    struct tagTappThreadCtx;
    typedef struct tagTappThreadCtx TAPPTHREADCTX;
    typedef struct tagTappThreadCtx *LPTAPPTHREADCTX;

    typedef enum tappThreadStatus
    {
        TAPP_THREAD_STATUS_CREATED,
        TAPP_THREAD_STATUS_RUNNING,
        TAPP_THREAD_STATUS_FINI,
        TAPP_THREAD_STATUS_DEAD,
    }TAPP_THREAD_STATUS;

    //The common thread config files.
    struct tagTappThreadConf
    {
        /*Called first after the thread was created*/
        int (*pfnInit)(IN TAPPTHREADCTX *pstThreadCtx, IN void* pvArg);

        /*Like tapp proc, called everytime in the proc*/
        int (*pfnProc)(IN TAPPTHREADCTX *pstThreadCtx, IN void* pvArg);

        /*Like tapp tick, called everytime the tapp tick reaches.*/
        int (*pfnTick)(IN TAPPTHREADCTX *pstThreadCtx, IN void* pvArg);

        /*Thre reload call back ,called when the reload signal received*/
        int (*pfnReload)(IN TAPPTHREADCTX *pstThreadCtx, IN void* pvArg);

        /*Called when the thread finis*/
        int (*pfnFini)(IN TAPPTHREADCTX *pstThreadCtx, IN void* pvArg);

        /*The metaname for run data qumu.*/
        char szRunDataCumuMeta[512];

        /*The metaname for run status data*/
        char szRunStatusMeta[512];

        /*The Name of the thread*/
        char szThreadName[128];

        /* Called when the main thread has detected that the thread has fini*/
        int (*pfnCleanup)(struct tagTAPPCTX *pstCtx, TAPPTHREADCTX *pstThreadCtx,
                          void* pvArgMain,void *pvArgThread);
    };

    typedef struct tagTappThreadConf TAPPTHREADCONF;
    typedef struct tagTappThreadConf *LPTAPPTHREADCONF;

    struct tagTappThreadCtx
    {
        /*The id of the working thread, Readonly, Returned by libtapp*/
        int iThreadIdx;

        /*The thread handler */
        pthread_t pstThread;

        /*Log category for the thread, can be used only in the thread*/
        LPTLOGCATEGORYINST pstLogCategory;

        /*The bill category for the thread ,used for writting bill*/
        LPTLOGCATEGORYINST pstBillCategory;

        /*The bus channel for the thread to communicate with main thread*/
        LPTBUSCHANNEL pstChannel;

        /*The config of the app*/
        const TAPPDATA *pstAppConf;

        /*The run status for tapp thread*/
        TAPPDATA stRunDataStatus;

        /*The run cumu data for tapp thread.*/
        TAPPDATA stRunDataCumu;
    };

    struct tagTAPPCTX;

    /* iPeerCurState 是enum类型tagTbusPeerStateType的值，见tbus.h */
    typedef int (*PFNTAPPTBUSSTATEFUNC)(struct tagTAPPCTX*, void*, TBUSADDR, int);

    struct tagTAPPCTX
    {
        /* should be set before calling tapp_def_init. */
        int argc;

        /* should be set before calling tapp_def_init. */
        char** argv;

        /* can be set before calling tapp_def_init. */
        /* can be set by user.*/
        unsigned long long uiVersion;

        /* the tick timer period. */
        /* can be set before calling tapp_def_init. */
        /* if not, tapp_def_init will set it to a by option ' --timer'. */
        /* if no ' --timer' , set it with default value 10ms. */
        int iTimer;

        /* can be set before calling tapp_def_init. */
        /* if not, tapp_def_init will set it to a default value. */
        /* the milliseconds to wait the epoll-wait timeout. */
        int iEpollWait;

        /* when an internal counter reaches iIdleCount, call pfnIdle or sleep for a while. */
        /* can be set before calling tapp_def_init. */
        /* if not, tapp_def_init will set it to a default value. */
        int iIdleCount;

        /* can be set before calling tapp_def_init. */
        /* if not, tapp_def_init will set it to a default value. */
        int iIdleSleep;
        int iTickCount;

        /* Bussiness id assigned by tagent */
        int iBusinessID;

        /* Shm key for tbus GCIM */
        char *pszGCIMKey;

        /* !0: use tbus; 0: not use tbus */
        int iUseBus;

        /* iBus is really handle of tbus. */
        int iBus;

        /* identifier of process. if tbus is used, record tbus-address */
        const char* pszId;
        int iId;

        /* read-only */
        struct timeval stCurr;

        /* read-only */
        const char* pszApp;

        const char* pszConfFile;

        /* tlog cfg file path */
        const char *pszLogConfFile;

        const char* pszMBaseKey;
        const char* pszTsmIdxKey;

        /* if used, and non-zero, can be cast to TDRMETALIB* pointer. */
        intptr_t iLib;

        /* if set, tapp will not automatically load conf-file */
        int iNoLoadConf;

        TAPPDATA stConfData; /*conf data*/
        TAPPDATA stConfPrepareData; /*when reload, can use this for prepare load conf */

        TAPPDATA stRunDataStatus; /*run time data for status val*/
        TAPPDATA stRunDataCumu; /*run time data for cumulate val*/

        /* Number of thread to start ,We will just create Nthreads slot for threads.*/
        int iNthreads;

        /* The config for default thread to start with*/
        TAPPTHREADCONF stThreadConf;

        /* The channel size for tbus, Default 1M
           Can be changed by cmdline options*/
        int iChannelSize;

        /* save tapp start mode, default: TAPP_START_MODE_INIT */
        int iStartMode;

        /* indicate whether to output statistic log */
        int iNotOutputStat;

        /* bus channel timeout gap */
        int iBusTimeOutGap;

        /* bus use saved time or use gettimeofday */
        int iBusUseFreshTime;

        /* time interval for bus channel to send heartbeat msg */
        int iBusHeartBeatGap ;

        /* time interval to check bus channel peer state */
        int iBusCheckPeerGap ;

        /* format of config-file, refer to "enum tagTDRXmlDataFormat" defined in tdr_define.h */
        int iConfigFileFormat;

        /* the metalib_id in monitor plus system, used for data reporting */
        int iReportMetalibId;

        /* the dir addr used for data reporting, the format is tcp://ip:port */
        const char* pszReportDirAddrs;

        /* the interval for data reporting, second */
        int iReportInterval;

        /* whether to disable cumu data reporting */
        int iDisableCumuReport;

        /* whether to disable status data reporting */
        int iDisableStatusReport;

        int (*pfnArgv)(struct tagTAPPCTX *pstCtx, void* pvArg);
        int (*pfnInit)(struct tagTAPPCTX *pstCtx, void* pvArg);
        int (*pfnProc)(struct tagTAPPCTX *pstCtx, void* pvArg);
        int (*pfnTick)(struct tagTAPPCTX *pstCtx, void* pvArg);
        int (*pfnReload)(struct tagTAPPCTX *pstCtx, void* pvArg);
        int (*pfnIdle)(struct tagTAPPCTX *pstCtx, void* pvArg);

        /* when SIGTERM signal is received,
         * this func while be called,
         * if -1 is returned, the mainloop will break.
         */
        int (*pfnQuit)(struct tagTAPPCTX *pstCtx, void* pvArg);

        /* when SIGUSR1 signal is received,
         * this func while be called,
         * if -1 is returned, the mainloop will break.
         */
        int (*pfnStop)(struct tagTAPPCTX *pstCtx, void* pvArg);

        int (*pfnFini)(struct tagTAPPCTX *pstCtx, void* pvArg);

        int (*pfnUsage)(int argc, char* argv[]);

        /* Optional. The user defined version generator.
         * If assigend, tapp call this function to process --version.
         */
        int (*pfnVersionGenerator)(struct tagTAPPCTX *pstCtx, void* pvArg);

        /* this func which is called to generate templet of configure file. */
        int (*pfnGenerate)(struct tagTAPPCTX *pstCtx, void* pvArg);

        /* tapp control mode: interfaces for tapp-server */
        int (*pfnProcCmdLine)(struct tagTAPPCTX *pstCtx, void* pvArg,
                              unsigned short argc, const char** argv);
        int (*pfnProcCtrlReq)(struct tagTAPPCTX *pstCtx, void* pvArg,
                              const char* pszMetaName, const char* pBuf, int iLen);
        /*获取进程控制使用帮助*/
        const char* (*pfnGetCtrlUsage)(void);

        /* tapp controller interface for controller */
        /* 控制器初始化接口 其作用相当于 正常程序的pfnInit回调接口 */
        int (*pfnControllerInit)(struct tagTAPPCTX *pstCtx, void* pvArg);
        int (*pfnPreprocCmdLine)(unsigned short argc, const char** argv);
        int (*pfnProcCtrlRes)(const char* pszMetaName, const char* pBuf, int tLen);
        int (*pfnControllerFini)(struct tagTAPPCTX *pstCtx, void* pvArg);

        PFNTAPPTBUSSTATEFUNC pfnBusPeerStateFlipped;
    };

    typedef struct tagTAPPCTX TAPPCTX;
    typedef struct tagTAPPCTX *LPTAPPCTX;

#define TAPP_MAX_SERVICE_NAME 256
#define TAPP_MAX_SERVICE_DESC 1024

    /** @defgroup  TAPP_INTERFACE
     * @{
     */

    typedef int (*PFNTAPPFUNC)(struct tagTAPPCTX*, void*);

    typedef int (*PFNTAPPTHREADFUNC)(LPTAPPTHREADCTX, void*);
    typedef int (*PFNTAPPTHREADCLEANUPFUNC)(struct tagTAPPCTX *pstCtx,
                                            TAPPTHREADCTX *pstThreadCtx,
                                            void* pvArgMain, void *pvArgThread);
    /**
     * @brief  init TAPP, should be called before other tapp_* functions.
     * @param a_pstCtx pointer to variable of type TAPPCTX
     * @param pvArg  pointer to use-defined data
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_def_init(TAPPCTX* a_pstCtx, void* a_pvArg);

    /**
     *    @brief tapp mainloop
     *    @param a_pstCtx pointer to variable of type TAPPCTX
     *    @param a_pvArg  pointer to use-defined data
     *    @retval 0 successful
     *    @retval !0 failed
     *    @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_def_mainloop(TAPPCTX* a_pstCtx, void* a_pvArg);

    /**
     *    @brief  exit the mainloop.
     *    @param void no param is needed.
     */
    TSF4G_API void tapp_exit_mainloop(void);

    /**
     *    @brief  judge whether recieve exit request.
     *    @param void no param is needed.
     *    @return whether need to exit the process.
     *    @retval 0 no exit request received.
     *    @retval 1 TAPP_EXIT_QUIT need exit, just quit.
     *    @retval 2 TAPP_EXIT_STOP need exit, a full stop required.
     */
    TSF4G_API int tapp_is_exit(void);

    /**
     *    @brief  do housekeeping, should be called before application exits.
     *    @param a_pstCtx pointer to variable of type TAPPCTX
     *    @param a_pvArg  pointer to use-defined data
     *    @retval 0 successful
     *    @retval !0 failed
     *    @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_def_fini(TAPPCTX* a_pstCtx, void* a_pvArg);

    /**
     *    @brief  get tlog's category of pszName.
     *    @param[in] a_pszName
     *    @param[out] a_ppiCatInst if not null, stores
     *    @retval 0 category of pszName found.
     *    @retval -1 category of pszName not found.
     */
    TSF4G_API int tapp_get_category(IN const char* a_pszName,
                                    OUT LPTLOGCATEGORYINST* a_ppiCatInst);
    /**
     *    @brief  get tlog's bill category of pszName.
     *    @param[in] a_pszName
     *    @param[out] a_ppiCatInst if not null, stores
     *    @retval 0 category of pszName found.
     *    @retval -1 category of pszName not found.
     */
    TSF4G_API int tapp_get_bill_category(IN const char* a_pszName,
                                         OUT LPTLOGCATEGORYINST* a_ppiCatInst);

    /**
     * @brief set the log module name for the tlog system.
     * @param [in] a_pszName the new name of the log
     * @retval 0 successful
     * @retval !0 failed
     */
    TSF4G_API int tapp_set_log_module_name(IN char *a_pszName);

    /**
     *    @brief  get tdr file name.
     *    @param void no param is needed.
     *    @retval valid file name or NULL.
     */
    TSF4G_API const char* tapp_get_tdrfile(void);

    /**
     *    @brief  get pid-file name.
     *    @param void no param is needed.
     *    @retval valid file name or NULL.
     */
    TSF4G_API const char* tapp_get_pidfile(void);

    /**
     * @brief  config windows-service-description
     * @param [in] a_pszServiceDesc service-description
     * @retval 0 successful
     * @retval !0 failed
     * @note   only have meaning for Windows platform
     * @pre a_pszServiceDesc 不能为 NULL
     */
    TSF4G_API int tapp_config_service_desc(IN const char* a_pszServiceDesc);

    /**
     * @brief  config windows-service-display-name
     * @param [in] a_pszDisplayName service-display-name
     * @retval 0 successful
     * @retval !0 failed
     * @note   only have meaning for Windows platform
     * @pre a_pszDisplayName 不能为 NULL
     */
    TSF4G_API int tapp_set_service_display_name(const char* a_pszDisplayName);

    /** callback interface  to init service
      @param[in] a_pstCtx global var of TAPPCTX of the process
      @param[in] a_pvArg the costumed var of the process
      @param[in] argc the number of the args
      @param[in] argv the args
      @retval 0   successful
      @retval !0  failed
      */
    typedef int (*PFNTAPP_SERVICE_INIT)(IN LPTAPPCTX a_pstCtx, IN void *a_pvArg,
                                        IN int argc, IN char* argv[]);

    /**
     * @brief register service-initiation function
     * @param [in] a_pfnSvrInit pointer to service-initiation function
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pfnSvrInit不能为NULL
     */
    TSF4G_API int tapp_register_service_init(IN PFNTAPP_SERVICE_INIT a_pfnSvrInit);

    /**
     * @brief notify tapp to execute registered funtions
     * @param[in] a_pstCtx global var of TAPPCTX of the process
     * @param[in] a_pvArg the costumed var of the process
     * @param [in] argc same meaning as to 'main'
     * @param [in] argv same meaning as to 'main'
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_run_service(IN LPTAPPCTX a_pstCtx, IN void *a_pvArg,
                                   IN int argc, IN char* argv[]);


    /**
     * @brief   set a brief description
     *
     * @param   a_pszBriefDesc  pointer to biref description
     *
     * @pre     a_pszBriefDesc 不能为 NULL
     *
     * @note    a_pszBriefDesc will be output when TAPP processing --help option
     */
    TSF4G_API void tapp_set_brief_desc(const char* a_pszBriefDesc);

    /**
     * @brief   protype of function for option process
     *
     * @param   a_pvAppData    callback data, registered by tapp_register_option
     * @param   a_pszOptArg    refers to opion's arg value, if any
     * @param   a_pszOptName   refers to option's long_name
     *
     * @retval   0   success
     * @retval  !0   error
     *
     * @note    if non-zero returned, tapp treats this as an error and exit
     *
     * @ref     tapp_register_option
     */
    typedef int (*TAPP_OPT_PROC_FUNC)(void* a_pvAppData,
                                      const char* a_pszOptArg,
                                      const char* a_pszOptName);

    /**
     * @brief   pre-defined call_back for boolean type of option
     *
     * @note    parameter 'a_pszOptArg' will never be used
     *
     * @note    this callback assigns 1 to *a_piDest
     *
     * @ref     TAPP_OPT_PROC_FUNC
     * @ref     tapp_register_option
     */
    TSF4G_API int tapp_bool_opt_proc(void* a_piDest,
                                     const char* a_pszOptArg,
                                     const char* a_pszOptName);

    /**
     * @brief   pre-defined call_back for integer type of option
     *
     * @note    this callback parses a_pszOptArg as an int and assigns it to *a_piDest
     *
     * @note    this callback is ONLY for options that registered with a_iHasArg != 0
     *
     * @ref     TAPP_OPT_PROC_FUNC
     * @ref     tapp_register_option
     */
    TSF4G_API int tapp_int_opt_proc(void* a_piDest,
                                    const char* a_pszOptArg,
                                    const char* a_pszOptName);

    /**
     * @brief   pre-defined call_back for string type of option
     *
     * @note    a_ppszDest is type of char**
     * @note    a_pvAppData to tapp_register_option should by type of char**
     *
     * @note    this callback assigns a_pszOptArg to *a_ppszDest
     *
     * @note    this callback is ONLY for options that registered with a_iHasArg != 0
     *
     * @ref     TAPP_OPT_PROC_FUNC
     * @ref     tapp_register_option
     */
    TSF4G_API int tapp_str_opt_proc(void* a_ppszDest,
                                    const char* a_pszOptArg,
                                    const char* a_pszOptName);

    /**
     * @brief   register an option to TAPP
     *
     * @param   a_pszLongName   long option name for this option
     * @param   a_bShortName    short option name for this option
     * @param   a_iHasArg       specify if this option needs an option
     * @param   a_pszDesc       description for this option
     * @param   a_pfnProc       call_back for this option
     * @param   a_pvAppData     will be passed as first paramter to a_pfnProc
     *
     * @retval  >= 0    success
     * @retval   < 0    error
     *
     * @pre     a_pszLongName can NOT be NULL
     * @pre     a_pszLongName can NOT be empty string ""
     * @pre     a_pszLongName can NOT start with '-'
     * @pre     a_bShortName can ONLY be 0/['0'-'9']/['a'-'z']/['A'-'Z']
     * @pre     a_pfnProc can NOT be NULL
     * @pre     this API should only be called within (TAPPCTX*)->pfnArgv
     *
     * @note    every option MUST have an unique long name
     * @note    a_bShortName == 0 means this option has NO short name
     * @note    a_iHasArg != 0 means this option needs an argument
     * @note    if a_pszDesc is NOT NULL, it will be saved as part of --help information
     * @note    if a_pszDesc is NOT NULL, its length MUST NOT surpass 128 bytes
     *
     * @note    建议通过 tapp_register_option 注册的 options 不超过 32 个
     */
    TSF4G_API int tapp_register_option(const char* a_pszLongName, char a_bShortName,
                                       int a_iHasArg, const char* a_pszDesc,
                                       TAPP_OPT_PROC_FUNC a_pfnProc, void* a_pvAppData);

    TSF4G_API const char* tapp_get_auto_tbusd_url(void);

    /** @} *//*TAPP_INTERFACE*/


    /** @defgroup  TAPP_CONTROL_MODE tapp_control_mode_interface
     * @{
     */

    /**
     * @brief query status of tappctrl
     * @retval !0 tappctrl is on
     * @retval 0  tappctrl is off
     */
    TSF4G_API int tappctrl_query_status(void);

    /**
     * @brief turn off tappctrl
     */
    TSF4G_API void tappctrl_turn_off(void);

    /**
     * @brief send string to tapp-console
     * @param [in] a_pszString 等待发送的字符串信息
     * @retval 0 successful
     * @retval !0 failed
     */
    TSF4G_API int tappctrl_send_string(const char* a_pszString);

    /**
     * @brief send binary data to tapp-console
     * @param [in] a_pszMetaName 等待发送二进制信息对应的元数据名
     * @param [in] a_pBuff 等待发送的数据,该Buffer的长度不可超过64K
     * @param [in] a_iLen 等待发送的数据长度
     * @retval 0 successful
     * @retval !0 failed
     */
    TSF4G_API int tappctrl_send_msg(const char* a_pszMetaName, const char* a_pBuff, int a_iLen);

    /**
     * @brief send large binary data to tapp-console
     * @param [in] a_pszMetaName 等待发送二进制信息对应的元数据名
     * @param [in] a_pBuff 等待发送的数据,该Buffer的长度可超过64K,但不超过500K
     * @param [in] a_iLen 等待发送的数据长度
     * @retval 0 successful
     * @retval !0 failed
     */
    TSF4G_API int tappctrl_send_msg_ex(const char* a_pszMetaName, const char* a_pBuff, int a_iLen);

#if defined(_WIN32) || defined(_WIN64)
    int tapp_ctrl_printf(const char *fmt, ...);
#else
    int tapp_ctrl_printf(const char *fmt, ...)  __attribute__((format(printf,1,2)));
#endif

    /** @} *//* @defgroup  TAPP_CONTROL_MODE tapp_control_mode_interface */

    /** @defgroup  TAPP_MULTI_THREAD tapp_multithread_interface
     * @{
     */

    /**
     * @brief create a thread and run it
     * @param [in] a_pstCtx process context.
     * @param [in] a_pstConf the config for running the thread.
     * @param [in] a_pvArg user parm for the thread to use.
     * @param [out] a_apstThread the handler for storing thread handler created
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_create_thread(IN TAPPCTX *a_pstCtx,
                                     IN LPTAPPTHREADCONF a_pstConf, IN void *a_pvArg,
                                     INOUT LPTAPPTHREADCTX *a_pstThread);

    /**
     * @brief Notify the thread to stop.
     * @param [in] a_pstCtx process context.
     * @param [in] pstThreadCtx the handler for storing thread handler created
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pstCtx不能为NULL
     * @note 工作线程在运行过程中，主线程组合使用tapp_stop_thread和tapp_delete_thread可能会引起问题，
     *       建议临时使用tapp_thread_exit和tapp_delete_thread来销毁工作线程及回收相关资源
     */
    TSF4G_API int tapp_stop_thread(IN TAPPCTX *pstCtx,IN TAPPTHREADCTX *pstThreadCtx);

    /**
     * @brief Delete the thread res in tapp
     * @param [in] a_pstCtx process context.
     * @param [in] pstThreadCtx the handler for storing thread handler created
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_delete_thread(IN TAPPCTX *pstCtx,IN TAPPTHREADCTX *pstThreadCtx);

    /**
     * @brief get status of  the thread res in tapp
     * @param [in] a_pstCtx process context.
     * @param [in] a_pstThreadCtx the handler for storing thread handler created
     * @return   the status of the thread
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API TAPP_THREAD_STATUS tapp_get_thread_status(IN TAPPCTX *a_pstCtx,
                                                        IN TAPPTHREADCTX *a_pstThreadCtx);

    /**
     * @brief exit the thread main loop.Used when the thread want to exit.
     * @param [in] pstThreadCtx thre thread ctx.
     * @retval 0 successful
     * @retval <0 failed
     */
    TSF4G_API int tapp_thread_exit(IN TAPPTHREADCTX *pstThreadCtx);

    /**
     * @brief get all the handlers of the threads created.
     * @param [in] a_pstCtx process context
     * @param [in] a_ppstThread array of thread handler
     * @param [in] a_piCount input the length of the array
     * @param [out] a_piCount output the number of handlers abtained
     * @retval 0 successful
     * @retval !0 failed
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_get_thread_list(IN TAPPCTX *a_pstCtx,
                                       OUT LPTAPPTHREADCTX *a_ppstThread, INOUT int *a_piCount);

    /**
     * @brief get the number of work threads created by the process.
     * @param [in] a_pstCtx process context
     * @retval >=0 successful,return the number of threads created
     * @retval <0 failed
     * @pre a_pstCtx不能为NULL
     */

    TSF4G_API int tapp_get_thread_num(IN TAPPCTX *a_pstCtx);

    /**
     * @brief send a packet to the thread specified by handler,Should be called only in main thread.
     * @param [in] a_pstCtx process context
     * @param [in] a_pstThreadCtx the target thread ctx,
     * @param [in] a_pszBuff the data to send.
     * @param [in] a_iBuff the length of data to send.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_send_to_thread(IN TAPPCTX *a_pstCtx,
                                      IN LPTAPPTHREADCTX a_pstThreadCtx,
                                      IN char *a_pszBuff,
                                      IN size_t a_iBuff);

    /**
     * @brief send a packet from iovec to the thread specified by handler,Should be called only in main thread.
     * @param [in] a_pstCtx process context
     * @param [in] a_pstThreadCtx the target thread ctx,
     * @param[in] a_ptVector -- data vector buffers
     * @param[in] a_iVecCnt -- data vector count
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_sendv_to_thread(IN TAPPCTX *a_pstCtx,
                                       IN LPTAPPTHREADCTX a_pstThreadCtx,
                                       IN const struct iovec *a_ptVector,
                                       IN const int a_iVecCnt);


    /**
     * @brief forward a packet to the thread specified by handler,Should be called only in main thread.
     this can be used with backward.
     * @param [in] a_pstCtx process context
     * @param [in] a_pstThreadCtx the target thread ctx,
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_forward_to_thread(IN TAPPCTX *a_pstCtx,
                                         IN TAPPTHREADCTX *a_pstThreadCtx,
                                         IN char * a_buff, IN size_t a_iBuff);

    /**
     * @brief forward a packet from iovec to the thread specified by handler,Should be called only in main thread.
     this can be used with backward.
     * @param [in] a_pstCtx process context
     * @param [in] a_pstThreadCtx the target thread ctx,
     * @param[in] a_ptVector -- data vector buffers
     * @param[in] a_iVecCnt -- data vector count
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_forwardv_to_thread(IN TAPPCTX *a_pstCtx,
                                          IN TAPPTHREADCTX *a_pstThreadCtx,
                                          IN const struct iovec *a_ptVector,
                                          IN const int a_iVecCnt);


    /**
     * @brief receive a packet from the specified thread.
     * @param [in] a_pstCtx process context.
     * @param [in] a_pstThreadCtx the target thread ctx,If null we will recv from the threads recursively.
     * @param [in,out] a_pszBuff the buffer to get the data.
     * @param [in,out] a_iBuff input the size of buff, output the length of data received.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_EMPTY)    ->  no data in recv-buffer
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_recv_from_thread(IN TAPPCTX *a_pstCtx,
                                        IN LPTAPPTHREADCTX a_pstThreadCtx,
                                        INOUT char *a_pszBuff,
                                        INOUT size_t *a_iBuff);

    /**
     * @brief receive a packet from the specified thread, just the packet address and len in memory, don't copy packet to buff .
     * @param [in] a_pstCtx process context.
     * @param [in] a_pstThreadCtx the target thread ctx,If null we will recv from the threads recursively.
     * @param [in,out] a_pszBuff the address to packet in memory.
     * @param [out] a_iBuff the length of data received.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_EMPTY)    ->  no data in recv-buffer
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_peek_from_thread(IN TAPPCTX *a_pstCtx,
                                        IN LPTAPPTHREADCTX a_pstThreadCtx,
                                        INOUT char **a_pszBuff,
                                        OUT size_t *a_iBuff);

    /**
     * @brief delete the frist msg in the channel of thread.
     * @param [in] a_pstCtx process context.
     * @param [in] a_pstThreadCtx the target thread ctx,If null,we will choose the last channel that recved msg.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_EMPTY)    ->  no data in recv-buffer
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_delete_from_thread(IN TAPPCTX *a_pstCtx,
                                          IN LPTAPPTHREADCTX a_pstThreadCtx);


    /**
     * @brief back a message from the last thread received.
     This function should be used when you use the tbus within TAPPCTX.iBus. You should have called
     tapp_recv_from_thread first.
     * @param [in] a_pstCtx process context
     * @param [in] a_pstThreadCtx the target thread ctx,
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_backward_from_thread(IN TAPPCTX *a_pstCtx, char *a_buff, size_t iBuff);



    /**
     * @brief backup a packet by iovec from the thread specified by handler,Should be called only in main thread.
     * @param [in] a_pstCtx process context
     * @param[in] a_ptVector -- data vector buffers
     * @param[in] a_iVecCnt -- data vector count
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     * @pre a_pstCtx不能为NULL
     */
    TSF4G_API int tapp_backwardv_from_thread(IN TAPPCTX *a_pstCtx,
                                             IN const struct iovec *a_ptVector,
                                             IN const int a_iVecCnt);



    /**
     * @brief receive data from main thread.
     * @param [in] pstThreadCtx the thread ctx.
     * @param [in,out] buf the buffer to put data in.
     * @param [in,out] iBuff Input the size of buff,out the size of data received.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_EMPTY)    ->  no data in recv-buffer
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_recv(IN TAPPTHREADCTX *pstThreadCtx,
                                   INOUT char *buff, INOUT size_t *iBuff);

    /**
     * @brief peek data from main thread, just get the address and size of msg, do not copy msg.
     * @param [in] pstThreadCtx the thread ctx.
     * @param [in,out] a_ppBuff the point to save the address to the msg in memory.
     * @param [out] a_iBuff Input the size of buff,out the size of data received.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_EMPTY)    ->  no data in recv-buffer
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_peek(IN TAPPTHREADCTX *pstThreadCtx,
                                   INOUT char **a_ppBuff, OUT size_t *a_iBuff);


    /**
     * @brief delete the first msg in the channel of main thread to thread,
     * @param [in] pstThreadCtx the thread ctx.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_EMPTY)    ->  no data in recv-buffer
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_delete(IN TAPPTHREADCTX *pstThreadCtx);

    /**
     * @brief send data to main thread.
     * @param [in] pstThreadCtx the thread ctx.
     * @param [in] buf the buffer to put data in.
     * @param [in] the length of data to send.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_send(IN TAPPTHREADCTX *pstThreadCtx,
                                   IN char *buff, IN size_t iBuff);

    /**
     * @brief send data to main thread，but this api will combine the data in iovec to one message..
     * @param [in] pstThreadCtx the thread ctx.
     * @param[in] a_ptVector -- data vector buffers
     * @param[in] a_iVecCnt -- data vector count
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_sendv(IN TAPPTHREADCTX *pstThreadCtx,
                                    IN const struct iovec *a_ptVector,
                                    IN const int a_iVecCnt);


    /**
     * @brief Get the number of running threads in the queue.
     * @param [in] a_pstCtx The tapp Ctx.
     * @retval Number of running thread.
     * @pre a_pstCtx不能为NULL
     * */
    TSF4G_API int tapp_get_num_running_thread(TAPPCTX *a_pstCtx);

    /**
     * @brief backward the message to main thread.
     * - This should be called just after tapp_thread_receive,
     * - for the bus head was stored in the channel handler.
     * @param [in] pstThreadCtx the thread ctx.
     * @param [in] buf the buffer to put data in.
     * @param [in] the length of data to send.
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_backward(IN TAPPTHREADCTX *pstThreadCtx,
                                       IN char *buff, IN size_t iBuff);

    /**
     * @brief like tapp_thread_backward, but this api will combine the data in iovec to one message.
     * - This should be called just after tapp_thread_receive,
     * - for the bus head was stored in the channel handler.
     * @param [in] pstThreadCtx the thread ctx.
     * @param[in] a_ptVector -- data vector buffers
     * @param[in] a_iVecCnt -- data vector count
     * @retval 0 successful
     * @retval <0 failed, see "TAPP-RETURN-VALUE" in tapp_error.h
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_ARG_INVALID)  ->  argument invalid
     *      - if (TAPP_ERR_GET_ERRNO(ret) == TAPP_ERRNO_BUF_FULL)     ->  send-buffer is full
     *      - else  ->  other error, refer log for more information
     *
     */
    TSF4G_API int tapp_thread_backwardv(IN TAPPTHREADCTX *pstThreadCtx,
                                        IN const struct iovec *a_ptVector,
                                        IN const int a_iVecCnt);

#if !defined (_WIN32) && !defined (_WIN64)
    /**
     * @brief set the data pointer to report to monitor plus system
     * @param [in] a_pstCumuData -- the cumu data pointer
     * @param [in] a_pstStatusData -- the status data pointer
     * @return void
     */
    void tapp_report_set_data(IN TAPPDATA* a_pstCumuData, IN TAPPDATA* a_pstStatusData);

    typedef void (*REPORT_CALLBACK)(IN void* a_pData) ;

    /**
     * @brief set the report runcumu data callback
     * @param [in] report_cumu_callback -- the cumu callback
     * @return void
     */
    void tapp_report_set_cumu_callback_func(IN REPORT_CALLBACK report_cumu_callback);

    /**
     * @brief set the report runstatus data callback
     * @param [in] report_status_callback -- the status callback
     * @return void
     */
    void tapp_report_set_status_callback_func(IN REPORT_CALLBACK report_status_callback);
#endif

    /** @} */

#ifdef __cplusplus
}
#endif

#endif /* TAPP_H */
