#ifndef __ACHIVEMENTACTIVITYMANAGER_H__
#define __ACHIVEMENTACTIVITYMANAGER_H__

#include "table/AchivementTable.h"


#define ACTIVITY_SYSTEM_ID 37

class AchivementActivityManager
{
	AchivementActivityManager();
	~AchivementActivityManager();
	DECLARE_SINGLETON(AchivementActivityManager)

public:

	bool Init();
	void Uninit();
	void Add(AchivementTable::RowData *pRow);
	time_t GetActivityTimeLeft();

private:
	int m_MaxActivityDays;
};

#endif // __ACHIVEMENTACTIVITYMANAGER_H__