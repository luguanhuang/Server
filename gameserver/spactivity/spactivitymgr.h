#ifndef __SPACTIVITYMGR_H__
#define __SPACTIVITYMGR_H__

#include "spactivityconfig.h"
#include "util/timespecificmgr.h"
#include "timer.h"
#include "spactivity/ptcg2c_spactivityoffsetdayntf.h"


class Role;
class SpActivityBase;

class SpActivityMgr : public ITimer
{
	SpActivityMgr();
	~SpActivityMgr();
	DECLARE_SINGLETON(SpActivityMgr)

public:
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	bool Init();
	void Uninit();	
	
	bool onTimer();

	bool LoadFile(UINT32 args);

	bool OnReloadTimeAndTaskTable();

	void OnGetGameServerOpenTime();

	void SendActivityOffsetDayPtc(Role* pRole);

	void UpdateSpActivityData(Role* pRole, bool updateTask = false);
	void CloseNotOpenSpActSystem(Role* pRole);

	bool OnStart(UINT32 actid);
	bool OnNextStage(UINT32 actid);
	bool OnEnd(UINT32 actid);

	inline const std::map<UINT32, SpActivityBase*>& GetAllSpActivity() { return m_allSpActivity; }

	SpActivityBase* GetSpActivity(UINT32 actid);

	static bool IsValidDateType(UINT32 type);

	void FillOffsetDayPtc(SpActivityBase* pBase, PtcG2C_SpActivityOffsetDayNtf& oPtc);

	bool GMStart(UINT32 actid);
	bool GMEnd(UINT32 actid);
	//bool GMAddOneDay(UINT32 actid);

private:	
	void _InitSpActivity(time_t nowTime, time_t startTime, const SpActivityTimeConf* conf);
	void _RegisterNextTimer(SpActivityBase* pSpActBase, const SpActivityTimeConf* conf);
	
	void _ResetSpActivity(SpActivityBase* pSpActBase);

	time_t _GetStartTime(const SpActivityTimeConf* conf, time_t nowTime, time_t serverOpenTime = 0);

	void _SyncEndToMS(UINT32 actid);

private:
	int m_getServerOpenTimeCnt;

	

	HTIMER m_timerHandler;

	std::map<UINT32, SpActivityBase*> m_allSpActivity;
};


#endif