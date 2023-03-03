#ifndef TAPP_CPP_H_
#define TAPP_CPP_H_

#include "tapp/tapp.h"
#include "tlog/tlog.h"
#include "tloghelp/tlogload.h"
#include <string>

namespace tapp {

class AppLauncher;
class IApp;
class IAppControllable;
class IAppThreadable;
class TappCtxObject;
class AppDelegate;

/// @brief  应用进程Launcher，App服务运行的入口、App服务的全局资源管理器
class AppLauncher {
    public:
        /// @brief  app运行的入口函数 - 带有配置文件
        ///
        /// @param[in]  app           app基类指针
        /// @param[in]  argc          命令行参数 argc
        /// @param[in]  argv          命令行参数 argv
        /// @param[in]  metalib_ptr   配置文件tdr 元数据描述库
        /// @param[in]  cfg_name      配置文件配置结构meta名字
        /// @param[in]  runcumu_name  配置文件runcumu meta名字
        /// @param[in]  runstatus_name配置文件runstatus meta名字
        ///
        /// @return <0: failed
        static int Run(IApp* app, int argc, char **argv,
                       unsigned char* metalib_ptr, const char* cfg_name,
                       const char* runcumu_name, const char* runstatus_name);

        /// @brief  app运行的入口函数 - 无配置文件
        ///
        /// @param[in]  app           app基类指针
        /// @param[in]  argc          命令行参数 argc
        /// @param[in]  argv          命令行参数 argv
        ///
        /// @return <0: failed
        static int Run(IApp* app, int argc, char **argv);

    public:
        /// @brief  获取全局tappctx对象实例
        ///
        /// @return 全局tappctx对象实例的引用
        static TappCtxObject& tappctx_object();

        /// @brief  获取文本日志句柄
        ///
        /// @return  文本日志句柄指针
        static LPTLOGCATEGORYINST tlogcat();

        /// @brief  获取运行数据日志句柄
        ///
        /// @return  运行数据日志句柄指针
        static LPTLOGCATEGORYINST data_tlogcat();

        /// @brief  获取账单日志句柄
        ///
        /// @return  账单日志句柄指针
        static LPTLOGCATEGORYINST bill_tlogcat();

    public:
        /// @brief  获取当前运行进程app指针，可通过dynamic_cast转换成原始app类实例指针
        static IApp* GetUserApp();

    public:
        /// @brief  发送给第tidx工作线程数据，tidx从1开始
        /// @note   这个接口必须在Run启动app成功后才能调用
        ///
        /// @param[in]  data  发送数据的指针
        /// @param[in]  size  发送的数据长度
        /// @param[in]  tidx  工作线程编码。从1开始，最大为nthread
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int SendDataToWorkerThread(const char* data, size_t size, uint32_t tidx);

        /// @brief  发送给第tidx工作线程数据，tidx从1开始
        /// @note   这个接口必须在Run启动app成功后才能调用
        ///
        /// @param[in]  data  发送数据的指针
        /// @param[in]  size  发送的数据长度
        /// @param[in]  tidx  工作线程编码。从1开始，最大为nthread
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int ForwardDataToWorkerThread(const char* data, size_t size, uint32_t tidx);

        /// @brief  发送给第tidx工作线程数据，tidx从1开始
        /// @note   这个接口必须在Run启动app成功后才能调用
        ///
        /// @param[in]  data  发送数据的指针
        /// @param[in]  size  发送的数据长度
        /// @param[in]  tidx  工作线程编码。从1开始，最大为nthread
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int ForwardvDataToWorkerThread(const struct iovec* vec, size_t count, uint32_t tidx);

        /// @brief  从工作线程中接收数据
        ///
        /// @param[in, out]  data  接收数据的缓冲区
        /// @param[in, out]  size  输入：接收数据的缓冲区大小。输出:数据包大小
        /// @param[in]       tidx  线程idx。默认为0(轮流去工作线程中收包)
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int RecvDataFromWorkerThread(char* data, size_t* size, uint32_t tidx=0);

        /// @brief  从工作线程中接收数据 - 无内存拷贝
        ///
        /// @param[out]  data  数据包指针
        /// @param[out]  size  数据包大小
        /// @param[in]  tidx  线程idx。默认为0(轮流去工作线程中收包)
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int PeekDataFromWorkerThread(char** p_data, size_t* size, uint32_t tidx=0);

        /// @brief  从工作线程中删除数据 - 与PeekDataFromWorkerThread配对使用
        ///
        /// @param[in]  tidx  线程idx。默认为0(删除最后一次PeekDataFromWorkerThread工作线程中收到的包)
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int DeleteDataFromWorkerThread(uint32_t tidx=0);

        /// @brief  将从工作线程收到的消息，backward到原始位置
        ///
        /// @param[in]  data  数据包指针
        /// @param[in]  size  数据包大小
        ///
        /// @note 必须事先调用PeekDataFromWorkerThread 或 RecvDataFromWorkerThread
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int BackwardDataFromWorkerThread(const char* data, size_t size);

        /// @brief  将从工作线程收到的消息，backward到原始位置
        ///
        /// @param[in]  vec    vec指针
        /// @param[in]  count  iovec数组大小
        ///
        /// @note 必须事先调用PeekDataFromWorkerThread 或 RecvDataFromWorkerThread
        ///
        /// @return 0 success, !0 failed
        /// @see tapp_error_string()
        static int BackwardvDataFromWorkerThread(const struct iovec* vec, size_t count);

    private:
        AppLauncher();
        virtual ~AppLauncher();
};

/// @brief  Tapp业务进程 接口类
/// @note   所有基于tapp的业务都必须提供一个派生IApp的实现类，用以响应tapp框架事件
class IApp {
    public:
        IApp(){};
        virtual ~IApp(){};

    /* 常见需要关注的事件回调 */
    public:
        /// @brief 初始化
        ///
        /// @param[in] cfg       配置结构体指针
        /// @param[in] runcumu   runcumu结构体指针
        /// @param[in] runstatus runstatus结构体指针
        /// @param[in] tlogcat   文本日志句柄实例指针
        ///
        /// @return !0: 进程退出
        ///
        /// @note 如果不需要配置、rundata等特性，可以不关心cfg、runcumu、runstatus
        virtual int OnInit(void* cfg, void* runcumu, void* runstatus,
                         LPTLOGCATEGORYINST tlogcat);

        /// @brief 资源清理
        virtual int OnCleanUp();

        /// @brief 框架主循环，会不断触发
        /// @return <0: 表明进程当前周期空闲
        virtual int OnUpdate();

        /// @brief 框架定时器事件
        ///
        /// @note 定时器时长可以在启动时--timer指定，或者通过
        ///       AppLauncher::tappctx_object()获取到Tapp上下文对象，调用set_timer_ms来设置
        ///       同时需要注意的是：这里的定时器并不是完全准确的，取决于OnUpdate处理时长和空闲休眠时长
        virtual int OnTick();

        /// @brief 配置文件reload事件过滤接口
        ///
        /// @note 业务收到reload命令后进行配置文件预检查。
        /// @return !0: 配置不合法，配置不会被替换，不会触发OnReload事件
        ///          0: 配置合法，配置会被替换成新的，触发OnReload事件
        /// @note 特别说明：如果业务需要保留旧配置，需要事先自行保留配置的副本
        /// @note 默认返回-1，如果业务希望支持配置文件reload，必须重载这个接口
        virtual int CheckCfgPreOnReload(void* new_cfg);

        /// @brief 配置文件reload事件
        /// @note 业务可以在此完成热更新特性
        virtual int OnReload(void* cfg);

        /// @brief 进程空闲事件
        ///
        /// @param[in] sleep_ms idle sleep毫秒值
        /// @note 如果不关注的话，默认休眠 sleep_ms指定的毫秒时长
        virtual int OnIdle(int sleep_ms);

        /// @brief 设置tapp上下文。在初始化之前的一个Hook事件，框架会调用此接口
        /// @note 如果业务需要在启动前设置相关选项，需要实现此虚接口
        ///       注意：此接口只是提供一个Hook，需要业务自行调用AppLauncher::tappctx_object()
        ///       取得tappctx引用，然后再调用相应的set接口完成，通常在此设置version版本值。
        ///       （调用 tappctx 示例的 set_tapp_formated_version方法，具体方法参考TappCtxObject的各接口说明）
        ///
        /// @return <0，会初始化失败直接退出。默认返回0，且不做任何处理
        virtual int SetTappCtxPara();

    /* 可能需要关注的事件回调 - 定制命令行、定制退出事件处理、tbus对端进程状态变更通知 */
    public:
        /// @brief 收到stop事件
        /// @return <0 退出主循环。 默认返回-1（即退出主循环）
        virtual int OnStop();

        /// @brief 收到quit事件
        /// @return <0 退出主循环。 默认返回-1（即退出主循环）
        virtual int OnQuit();

        /// @brief 添加业务自定义命令行选项。在初始化之前的一个Hook事件，框架会调用此接口
        /// @note 如果业务有需要，需要实现此接口
        ///       注意：此接口只是提供一个Hook，具体注册命令行需要自行调用tapp_register_option
        ///
        /// @return !0, 初始化失败退出。默认返回0
        virtual int AddUserDefinedCmdLine();

        /// @brief tbus通道对端进程状态发生变化事件
        ///
        /// @param[in]  bus_addr  对端bus地址
        /// @param[in]  new_state 对端服务状态
        ///
        /// @note 如果开启了tbus心跳探测，且关心tbus对端服务状态，需要捕捉此事件并完成相应的操作
        ///       由于tbus心跳不够合理，建议业务层自行完成心跳机制，不要开启tbus心跳探测
        virtual int onBusPeerStateFlipped(TBUSADDR bus_addr, int new_state);

    /* 可能需要关注的事件回调 - 控制模式 */
    public:
        /// @brief 添加业务控制模式对象
        /// @note 如果需要定制控制模式，需要实现IAppControllable接口类并实例化一个全局对象，并通过此接口返回
        /// @return 返回控制模式对象，如果不实现此虚接口，默认返回NULL
        ///
        /// @note 必须保证返回的对象的生命周期在app运行期间都有效
        virtual IAppControllable* GetAppControllableObject();

    public:
        /// @brief 添加业务worker-dispatcher多线程模型对象
        /// @note 如果需要定制tapp多线程，需要实现IAppThreadable接口类并实例化一个全局对象，并通过此接口返回
        /// @return 返回多线程模式对象，如果不实现此虚接口，默认返回NULL
        ///
        /// @note 必须保证返回的对象的生命周期在app运行期间都有效
        virtual IAppThreadable* GetAppThreadableObject();

    /* 一般不需要关注的事件回调 */
    public:
        /// @brief 输出进程帮助信息
        /// @return 0:输出信息的同时，另外输出框架生成帮助信息
        ///        !0:只输出此接口的信息，不再输出框架生成帮助信息
        ///        默认只输出框架生成的帮助信息，建议一般情况下不用实现此虚接口。
        virtual int PrintUsage(int argc, char** argv);

        /// @brief 注册服务描述信息
        /// @return 返回服务描述信息
        /// @note 一般在windows服务模式下，用来定制服务的描述信息
        virtual const char* GetServiceDescription();

        /// @brief 输出业务进程版本信息
        /// @note  建议通过set_tapp_formated_version设置tapp格式的版本号，不去实现此虚接口（采用默认行为）
        virtual void PrintAppVersion();
};

/// @brief Tapp 控制模式接口类
/// @note 如果业务需要定制相应的控制模式，需要提供一个派生自ITappControllable的实现类
class IAppControllable {
    public:
        IAppControllable(){}
        virtual ~IAppControllable(){}

    /* console控制端 和 server被控制端的关系，又可以理解为client 和 server的关系
     */
    public:
        /* console 控制端 */
        /// @brief console端初始化接口
        /// @note 控制模式下，框架会主动调用此虚接口
        virtual int InitConsoleController();

        /// @brief console端清理接口
        /// @note 控制模式下，框架会主动调用此虚接口
        virtual int FiniConsoleController();

        /// @brief console端命令行过滤接口
        /// @return 0：只在console端处理，不发给被控制端
        ///        !0: 直接发送给被控制端
        virtual int PreprocessCmdLine(int argc, const char** argv);

        /// @brief 二进制控制命令响应事件
        virtual int OnBinaryCtrlResponse(const char* meta_name,
                                         const char* buff, size_t buf_len);

        // server 被控制端
        /// @brief 收到console 发起help询问事件，在此输出支持的控制命令
        /// @return 返回控制帮助信息
        virtual const char* OnGetCtrlUsage();

        /// @brief 收到console 发起的命令行命令请求，在此做响应
        virtual int OnCmdLineRequest(int argc, const char** argv);

        /// @brief 收到console 发起的二进制命令请求，在此做响应
        virtual int OnBinaryCtrlRequest(const char* meta_name,
                                        const char* buff, size_t buf_len);
};

/// @brief Tapp 多线程模式接口类
/// @note 如果业务需要支持worker-dispatcher多线程模式，需要提供一个派生自IAppThreadable的实现类
class IAppThreadable {
    public:
        IAppThreadable(){}
        virtual ~IAppThreadable(){}

    public:
        /// @brief worker线程Init事件
        ///
        /// @param[in]  thread_ctx  工作线程ctx
        /// @param[in]  cfg         进程配置文件指针
        /// @param[in]  runcumu     工作线程runcumu指针
        /// @param[in]  runstatus   工作线程runstatus指针
        virtual int OnThreadInit(LPTAPPTHREADCTX thread_ctx, void* arg,
                                 void* cfg, void* runcumu, void* runstatus);

        /// @brief worker线程Update事件
        ///
        /// @param[in]  thread_ctx  工作线程ctx
        virtual int OnThreadUpdate(LPTAPPTHREADCTX thread_ctx, void* arg);

        /// @brief worker线程CleanUp事件回调
        ///
        /// @param[in]  thread_ctx  工作线程ctx
        virtual int OnThreadCleanUp(LPTAPPTHREADCTX thread_ctx, void* arg);

        /// @brief worker线程Reload事件回调
        ///
        /// @param[in]  thread_ctx  工作线程ctx
        /// @param[in]  cfg         reload后的新配置指针
        virtual int OnThreadReload(LPTAPPTHREADCTX thread_ctx, void* arg, void* cfg);

        /// @brief worker线程Tick事件回调
        ///
        /// @param[in]  thread_ctx  工作线程ctx
        virtual int OnThreadTick(LPTAPPTHREADCTX thread_ctx, void* arg);

        /// @brief  为每个worker线程创建用户自定义参数对象
        ///
        /// @return NULL 不需要自定义参数
        ///        !NULL 返回worker线程参数的全局对象
        ///
        /// @note !!!! 必须保证返回的变量是通过malloc 或者 new分配出来的 !!!!
        virtual void* NewThreadArgvObject();

        /// @brief 释放为每个worker线程创建的参数对象
        ///
        /// @param[in]  obj NewThreadArgvObject创建的对象
        ///
        /// @return 0 success, !0 failed
        ///
        /// @note 与NewThreadArgvObject配套使用
        /// @note 必须保证NewThreadArgvObject返回非NULL对象时，DeleteDataFromWorkerThread的实现去释放此对象
        virtual int DeleteThreadArgvObject(void* obj);

        /// @brief 线程runcumu元数据名字
        virtual const char* GetThreadRunCumuName();
        /// @brief 线程runstatus元数据名字
        virtual const char* GetThreadRunStatusName();
        /// @brief 线程名字
        virtual const char* GetThreadName();
};

class TappCtxObject {
    /* set 类接口 */
    public:
        /* 常用接口 - tdr 元数据相关接口 */

        /* 配置描述文件的tdr元数据描述库 */
        void set_tdr_metalib(unsigned char metalib[]) {
            tapp_ctx_ref_.iLib = (intptr_t)metalib;
        }

        /* 配置结构的meta名 */
        void set_cfg_metaname(char* name) {
            tapp_ctx_ref_.stConfData.pszMetaName = name;
        }

        /* runcumu结构的meta名 */
        void set_runcumu_metaname(char* name) {
            tapp_ctx_ref_.stRunDataCumu.pszMetaName = name;
        }

        /* runstatus结构的meta名 */
        void set_runstatus_metaname(char* name) {
            tapp_ctx_ref_.stRunDataStatus.pszMetaName = name;
        }

        /* tapp规范下的版本值 */
        /* 版本输出格式为 major.minor.rev.build */
        void set_tapp_formated_version(int major, int minor, int rev, int build) {
            tapp_ctx_ref_.uiVersion = TAPP_MAKE_VERSION(major, minor, rev, build);
        }

    public:
        /* 常用接口 - 配置、tbus相关接口 */

        /* 无配置模式 */
        void set_no_load_cfg() {
            tapp_ctx_ref_.iNoLoadConf = 1;
        }

        /* bus key */
        void set_buskey(char* key) {
            tapp_ctx_ref_.pszGCIMKey = key;
        }

        /* bus id */
        void set_bus_addr(char* addr) {
            tapp_ctx_ref_.pszId = addr;
        }

        /* use bus */
        void set_use_bus_on() {
            tapp_ctx_ref_.iUseBus = 1;
        }

        /* NOT use bus */
        void set_use_bus_off() {
            tapp_ctx_ref_.iUseBus = 0;
        }

        /* bus timeout gap */
        void set_bus_timeout_gap(int max_timeout) {
            tapp_ctx_ref_.iBusTimeOutGap = max_timeout;
        }

        /* bus use saved time */
        void set_bus_use_saved_time(bool use_saved) {
            tapp_ctx_ref_.iBusUseFreshTime = use_saved ? 1 : 0;
        }

        /* bus heartbeat gap */
        void set_bus_heartbeat_gap(int heartbeat_gap) {
            tapp_ctx_ref_.iBusHeartBeatGap = heartbeat_gap;
        }

        /* bus check peer gap */
        void set_bus_check_peer_gap(int checkpeer_gap) {
            tapp_ctx_ref_.iBusCheckPeerGap = checkpeer_gap;
        }

    public:
        /* 多线程相关接口 */

        /* 工作线程数 */
        void set_worker_thread_num(int num) {
            tapp_ctx_ref_.iNthreads = num;
        }

        /* 主线程与工作线程通信通道大小 */
        void set_thread_channel_size(size_t size) {
            tapp_ctx_ref_.iChannelSize = size;
        }

    public:
        /* 其它接口 */

        /* xml配置文件格式 */
        void set_cfg_format(enum tagTDRXmlDataFormat format) {
            tapp_ctx_ref_.iConfigFileFormat = format;
        }

        /* 业务 id */
        void set_bussiness_id(int id) {
            tapp_ctx_ref_.iBusinessID = id;
        }

        /* 监控 tsm key */
        void set_tsm_idx_key(char* key) {
            tapp_ctx_ref_.pszTsmIdxKey = key;
        }

        /* tick 定时器精度 */
        void set_timer_ms(int timer) {
            tapp_ctx_ref_.iTimer = timer;
        }

        /* idle 周期 */
        void set_idle_count(int count) {
            tapp_ctx_ref_.iIdleCount = count;
        }

        /* idle 休眠时长 */
        void set_idle_sleep(int time_ms) {
            tapp_ctx_ref_.iIdleSleep = time_ms;
        }

        /* stat日志相关 - 关闭 */
        void set_stat_log_off() {
            tapp_ctx_ref_.iNotOutputStat = 1;
        }

        /* stat日志相关 - 开启 */
        void set_stat_log_on() {
            tapp_ctx_ref_.iNotOutputStat = 0;
        }

    /* get 类接口 */
    public:
        /* 常用接口 - 业务配置相关 */
        /* 配置结构体指针，需要上层自行转换 */
        void* get_cfg_ptr() {
            return tapp_ctx_ref_.stConfData.pszBuff;
        }

        size_t get_cfg_size() const {
            return tapp_ctx_ref_.stConfData.iLen;
        }

        intptr_t get_cfg_meta() const {
            return tapp_ctx_ref_.stConfData.iMeta;
        };

        /* reload配置结构体指针，需要上层自行转换 */
        void* get_reload_cfg_ptr() {
            return tapp_ctx_ref_.stConfPrepareData.pszBuff;
        }

        size_t get_reload_cfg_size() const {
            return tapp_ctx_ref_.stConfPrepareData.iLen;
        }

        /* runcumu结构体指针，需要上层自行转换 */
        void* get_runcumu_ptr() {
            return tapp_ctx_ref_.stRunDataCumu.pszBuff;
        }

        size_t get_runcumu_size() const {
            return tapp_ctx_ref_.stRunDataCumu.iLen;
        }

        /* runstatus结构体指针，需要上层自行转换 */
        void* get_runstatus_ptr() {
            return tapp_ctx_ref_.stRunDataStatus.pszBuff;
        }

        size_t get_runstatus_size() {
            return tapp_ctx_ref_.stRunDataStatus.iLen;
        }

    public:
        /* 常用接口 - 进程标识相关 */
        /* tbus句柄 */
        int get_bus_handler() const {
            return tapp_ctx_ref_.iBus;
        }

        /* 业务id */
        int get_bussiness_id() const {
            return tapp_ctx_ref_.iBusinessID;
        }

        /* 进程 id */
        int get_bus_id() const {
            return tapp_ctx_ref_.iId;
        }

        /* 进程 id - string格式 */
        const char* get_bus_id_string() const {
            return tapp_ctx_ref_.pszId;
        }

        /* 是否使用tbus */
        bool is_use_bus() const {
            return tapp_ctx_ref_.iUseBus;
        }

        const char* get_buskey() const {
            return tapp_ctx_ref_.pszGCIMKey;
        }

    public:
        /* 系统时间 */
        struct timeval get_curr_timeval() {
            return tapp_ctx_ref_.stCurr;
        }

    public:
        /* 多线程相关 */
        /* 工作线程数量 */
        int get_worker_thread_num() const {
            return tapp_ctx_ref_.iNthreads;
        }

        /* 线程间通信队列大小 - 单位：Byte */
        size_t get_thread_channel_size() const {
            return tapp_ctx_ref_.iChannelSize;
        }

    public:
        /* app 进程名 */
        const char* get_app_name() const {
            return (tapp_ctx_ref_.pszApp != NULL) ? tapp_ctx_ref_.pszApp : "";
        }

        /* 监控tsm idx */
        const char* get_tsm_idx_key() const {
            return tapp_ctx_ref_.pszTsmIdxKey ? tapp_ctx_ref_.pszTsmIdxKey : "";
        }

        /* mbase key */
        const char* get_mbase_key() const {
            return tapp_ctx_ref_.pszMBaseKey;
        }

        /* tapp 格式版本号 */
        unsigned long long get_tapp_formated_version() const {
            return tapp_ctx_ref_.uiVersion;
        }

    public:
        /* 完全透传的成员 */

        /* epoll wait time */
        int get_epoll_wait() const {
            return tapp_ctx_ref_.iEpollWait;
        }

        /* 是否 resume 模式 */
        bool is_resume_mode() const {
            return (TAPP_START_MODE_RESUME == tapp_ctx_ref_.iStartMode) ?
                true : false;
        }

    public:
        inline LPTAPPCTX tapp_ctx() {
            return &tapp_ctx_ref_;
        }

    private:
        TappCtxObject(TAPPCTX& ctx) : tapp_ctx_ref_(ctx){}
        TappCtxObject& operator=(const TappCtxObject&);
        TappCtxObject();

    private:
        TAPPCTX& tapp_ctx_ref_;
        friend class AppDelegate;
};

}

#endif
