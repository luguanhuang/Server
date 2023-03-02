#ifndef __WHITELISTMGR_H__
#define __WHITELISTMGR_H__

class WhiteListMgr
{
	WhiteListMgr();
	~WhiteListMgr();
	DECLARE_SINGLETON(WhiteListMgr)

public:

	bool Init();
	void Uninit();
	void Reload();

	bool IsOpenIDInWhiteList(const std::string &id);

	void DumpWhiteList();

private:
	void LoadWhiteList();

	std::unordered_set<std::string> m_WhiteOpenIDs;
};


#endif // __WHITELISTMGR_H__