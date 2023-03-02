#ifndef __RELOADMGR_H__
#define __RELOADMGR_H__


class ReloadMgr
{
	ReloadMgr();
	~ReloadMgr();
	DECLARE_SINGLETON(ReloadMgr)

public:
	typedef std::function<bool(void)> Reload_Func;

	bool Init();
	void Uninit();

	bool ReloadTable(UINT32 type, const std::string& cmd, std::string& strErrMsg);

	Reload_Func& GetFun(const std::string& key);

private:
	std::unordered_map<std::string, Reload_Func> m_reloadMap;
};



#endif