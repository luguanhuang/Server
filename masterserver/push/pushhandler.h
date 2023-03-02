#ifndef _PUSH_HANDLER_H_
#define _PUSH_HANDLER_H_

#include <string>
#include "table/PushMessageTable.h"
#include "util/timespecificmgr.h"

enum PushHandlerType
{
	PushHanlder_SpecificTime = 1,	// �ض�ʱ��ȫ������
	PushHandler_LevelSeal = 2,		// ��ӡ����
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

// �ض�ʱ��ȫ������
class SpecificTimePushHandler : public PushHandler
{
public:
	SpecificTimePushHandler() {};
	virtual ~SpecificTimePushHandler() {}

	virtual void Load();
};

// �ȼ���ӡ���ȫ������
class LevelSealPushHandler : public PushHandler
{
public:
	LevelSealPushHandler() { }

	virtual ~LevelSealPushHandler() {}

	virtual void Load();

};

#endif
