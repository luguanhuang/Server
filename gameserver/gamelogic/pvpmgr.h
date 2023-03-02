#ifndef __PVPMGR_H__
#define __PVPMGR_H__

#include "pvpbasedata.h"
#include "timer.h"

class Role;

class PvpMgr : public ITimer
{
	PvpMgr();
	~PvpMgr();
	DECLARE_SINGLETON(PvpMgr);
public:
	bool Init();
	void Uninit();

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void CheckOpenNotice();
	void OnRoleLogin(Role* pRole);
	void OnRoleFinishBattle(Role* pRole);

	bool IsInOpenTime();

	time_t GetGameTimeDown(bool allend);

private:
	void HallIconNoticeCheck(Role* pRole, bool open);


	bool m_openInTNoticeFlag;

	HTIMER m_handler;
};

#endif