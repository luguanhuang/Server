#ifndef __ACCOUNTBANMGR_H__
#define __ACCOUNTBANMGR_H__
#include "pb/project.pb.h"
#include "util/timelist.h"
#include "timer.h"

class AccountBanMgr : public ITimer
{
	AccountBanMgr();
	~AccountBanMgr();
	DECLARE_SINGLETON(AccountBanMgr)

public:

	bool Init();
	void Uninit();
	bool IsOpenIDInBanList(const std::string &openid);
	
	void FillReason(const std::string &openid,KKSG::PlatBanAccount *pAcc);
	void AddAccount(KKSG::BanAccount &acc);
	void DelAccount(const std::string &openid);

	void SyncBanList(UINT32 nLink);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
private:

	std::unordered_map<std::string, KKSG::BanAccount> m_BanOpenIDs;
	HTIMER m_hTimer;
};


#endif // __ACCOUNTBANMGR_H__