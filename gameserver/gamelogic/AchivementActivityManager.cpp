#include "pch.h"
#include <time.h>
#include "AchivementActivityManager.h"


INSTANCE_SINGLETON(AchivementActivityManager)

AchivementActivityManager::AchivementActivityManager()
{
	m_MaxActivityDays = 0;
}

AchivementActivityManager::~AchivementActivityManager()
{

}

bool AchivementActivityManager::Init()
{
	return true;
}

void AchivementActivityManager::Uninit()
{

}

void AchivementActivityManager::Add(AchivementTable::RowData *pRow)
{
	if (pRow->AchievementFetchDays > m_MaxActivityDays)
	{
		m_MaxActivityDays = pRow->AchievementFetchDays;
	}
}

time_t AchivementActivityManager::GetActivityTimeLeft()
{
	time_t now = time(NULL);
	time_t dwActivityEndTime = GSConfig::Instance()->GetGameServerOpenTimeStamp() + m_MaxActivityDays * (24 * 3600);
	return dwActivityEndTime > now ? (dwActivityEndTime - now) : 0;
}

