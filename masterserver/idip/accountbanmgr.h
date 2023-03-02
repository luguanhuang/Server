#ifndef __ACCOUNTBANMGREX_H__
#define __ACCOUNTBANMGREX_H__
#include "pb/project.pb.h"
#include "util/timelist.h"
#include "timer.h"

class AccountBanMsMgr : public ITimer
{
	AccountBanMsMgr();
	~AccountBanMsMgr();
	DECLARE_SINGLETON(AccountBanMsMgr)
public:
	bool Init();
	void Uninit();
	bool IsOpenIDInBanList(const std::string &openid,UINT32 &nEnd,std::string &reason);
	void AddAccount(const KKSG::BanAccount &acc);
	void DelAccount(const std::string &openid);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
private:

	std::unordered_map<std::string, KKSG::BanAccount> m_BanOpenIDs;
	HTIMER m_hTimer;
};


#endif // __ACCOUNTBANMGR_H__