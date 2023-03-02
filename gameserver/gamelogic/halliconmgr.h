#ifndef __HALLICONMGR_H__
#define __HALLICONMGR_H__

#include "pvpbasedata.h"
#include "timer.h"

class Role;

class HallIconMgr : public ITimer
{
	HallIconMgr();
	~HallIconMgr();
	DECLARE_SINGLETON(HallIconMgr);
public:
	enum State
	{
		eStop = 0,
		eRuning  = 1,
		eEnd	 = 2,
	};
	bool Init();
	void Uninit();
	void CheckSkyCityState(UINT32 nID);
	void CheckOpenNotice(UINT32 nSystemID,UINT32 nState);
	void HallIconNoticeCheck(Role* pRole,UINT32 nSystemID,UINT32 nState);
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void OnRoleLogin(Role* pRole);
	void OnFirstEnterScene(Role* pRole);
private:
	HTIMER m_handler;
	struct IconState
	{
		UINT32 nState;
		UINT32 nTime;
		UINT32 nSystemID;
	};
	std::map<UINT32,IconState> m_mapIcon;
};

#endif