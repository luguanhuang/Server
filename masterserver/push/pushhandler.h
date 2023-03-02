#ifndef _PUSH_HANDLER_H_
#define _PUSH_HANDLER_H_

#include <string>
#include "table/PushMessageTable.h"
#include "util/timespecificmgr.h"

enum PushHandlerType
{
	PushHanlder_SpecificTime = 1,	// 特定时间全服推送
	PushHandler_LevelSeal = 2,		// 封印推送
};

class PushHandler
{
public:
	struct SpecificTimerItem
	{
		PushMessageTable::RowData tableData;
		UINT32 timerId;

		SpecificTimerItem(const TimeSpecific& time, PushMessageTable::RowData* data);
		~SpecificTimerItem();
		bool OnTimer();
	};

	PushHandler()
	:m_lastUpdateTime(0)
	{
	}

	virtual ~PushHandler();

	virtual void Update(bool force = false);

	virtual void Load() = 0;

	void ClearTimerItem();
	void AddTimerItem(SpecificTimerItem* timer);

protected:
	UINT32 m_lastUpdateTime;
	std::list<SpecificTimerItem*> m_timers;
};

// 特定时间全服推送
class SpecificTimePushHandler : public PushHandler
{
public:
	SpecificTimePushHandler() {};
	virtual ~SpecificTimePushHandler() {}

	virtual void Load();
};

// 等级封印解除全服推送
class LevelSealPushHandler : public PushHandler
{
public:
	LevelSealPushHandler() { }

	virtual ~LevelSealPushHandler() {}

	virtual void Load();

};

#endif
