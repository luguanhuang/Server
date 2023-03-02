#ifndef _H_Reload_Mgr_H__
#define _H_Reload_Mgr_H__


enum ReloadErrorCode
{
	Reload_Succeed,
	Reload_Failed,
	Reload_DelayAndSucceed,
	Reload_DelayAndFailed,
};

// 异步加载相应数据
struct ReloadData
{
	ReloadData(UINT32 _dataNum)
	{
		dataNum = _dataNum;
	}

	bool OnReloadReply(UINT32 delayid, const KKSG::GMToolCommandRes& res);

	UINT32 dataNum;
	std::vector<KKSG::GMToolCommandRes> replyData;
};

enum ReloadServerType
{
	ReloadServer_NONE = 0,	
	ReloadServer_GS = (1<<0),
	ReloadServer_NS = (1<<1),
	ReloadServer_DB = (1<<2),
};

typedef std::function<bool(void)> ReloadFunc;

struct ReloadHandler
{
	ReloadHandler() : serverType(ReloadServer_NONE) {}

	ReloadFunc func;
	UINT32 serverType;
};

bool ReloadNoUse();
class CReloadMgr
{
	CReloadMgr();
	~CReloadMgr();
	DECLARE_SINGLETON(CReloadMgr)

public:
	bool Init();
	void Uninit();

	//strParam:  whitelist /version / queuing
	ReloadErrorCode ReloadNew(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid);

	void OnReloadDelayReply(UINT32 delayid, const KKSG::GMToolCommandRes& res);

private:
	void RegisterReloadHandler(const std::string& name, UINT32 type, ReloadFunc func = ReloadNoUse);

	ReloadErrorCode _MSSpecialReload(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid);	
	UINT32 _ReloadNs(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid);	
	UINT32 _ReloadDb(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid);
	UINT32 _ReloadGs(UINT32 type, const std::string& strParam, std::string& strErrMsg, UINT32 delayid);

	bool CustomBattleLoadGM();
	bool ClearGuildArenaPoint1();
	bool ClearGuildArenaPoint2();
	bool ClearGuildArenaPoint3();
	bool ClearGuildArenaPoint4();

private:
	typedef ReloadErrorCode (CReloadMgr::*PReload_Func)(UINT32 type, const std::string&, std::string&, UINT32);
	typedef std::unordered_map<std::string, PReload_Func>	CReloadFuncMap;
	CReloadFuncMap	m_oFuncMap;

	std::unordered_map<UINT32, ReloadData> m_reloadDataMap;

	std::unordered_map<std::string, ReloadHandler> m_handlers;
};

#endif