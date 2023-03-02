#ifndef _H_Reload_Mgr_H__
#define _H_Reload_Mgr_H__


class CReloadMgr
{
	CReloadMgr();
	~CReloadMgr();
	DECLARE_SINGLETON(CReloadMgr)

public:
	bool Init();
	void Uninit();

	//strParam:  gatelist
	bool Reload(const std::string& strParam, std::string& strErrMsg);

private:
	bool _ReloadVersionFile();
	bool _ReloadTestVersionFile();
private:
	typedef bool (CReloadMgr::*PReload_Func)();
	typedef std::unordered_map<std::string, PReload_Func>	CReloadFuncMap;
	CReloadFuncMap	m_oFuncMap;
};

#endif