#ifndef _H_WhiteListMgr_H__
#define _H_WhiteListMgr_H__


class CWhiteListMgr
{
	CWhiteListMgr();
	~CWhiteListMgr();
	DECLARE_SINGLETON(CWhiteListMgr)

public:
	bool Init();
	void Uninit();

	bool LoadWhiteList();
	bool IsOpenIDInWhiteList(const std::string& id) const;
	void Add(const std::string& account);
	void Del(const std::string& account);
	void SaveWhiteList();

private:

	std::unordered_set<std::string> m_WhiteOpenIDs;
};


#endif // __WHITELISTMGR_H__