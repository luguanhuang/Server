#ifndef __PUSH_SUBSCRIBEMGR_H__
#define __PUSH_SUBSCRIBEMGR_H__
#include "table/PushSubscribeTable.h"
#include "timer.h"
#include "util/timespecificmgr.h"

struct EqualPushWeekDays
{
	UINT32 days;
	EqualPushWeekDays(INT32 udays) { days = udays;}
	bool operator()(const UINT32& udays)
	{
		return udays == days;
	}
};
class CPushSubscribeMgr : public ITimer
{
public:
	struct SpecificPushTimer
	{
		PushSubscribeTable::RowData table;
		SpecificPushTimer(const TimeSpecific& time, PushSubscribeTable::RowData* data);
		~SpecificPushTimer();
		bool OnTimer();
		UINT32 timerId;
	};
	CPushSubscribeMgr();
	~CPushSubscribeMgr();
	DECLARE_SINGLETON(CPushSubscribeMgr)

public:
	bool Init();
	void Uninit();
	bool LoadConfig();
	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);
	void LoadTimer();
	void ClearTimer();
	UINT32 GetTxMsgId(UINT32 msgid);
	UINT32 GetMsgIdByTxMsgId(UINT32 id);
private:
	PushSubscribeTable m_oTable;
	HTIMER m_TimeHandler;
	time_t m_lastUpdateTime;
	std::list<SpecificPushTimer*> m_timers;
	//PushSubscribeHandler m_Handler;
};
#endif