#include "pch.h"
#include "AchivementHandler.h"
#include "AchivementMgr.h"
#include "unit/role.h"
#include "gamelogic/bagtransition.h"
#include "AchivementActivityManager.h"
#include "achievev2Record.h"
#include "foreach.h"


INSTANCE_SINGLETON(AchivementMgr)

AchivementMgr::AchivementMgr()
{
	memset(m_Handlers, 0, sizeof(m_Handlers));
}

AchivementMgr::~AchivementMgr()
{

}

bool AchivementMgr::Init()
{
	if (!m_oTable.LoadFile("table/AchivementList.txt"))
	{
		LogWarn("Load file AchivementList.txt failed!");
		return false;
	}

	for (UINT32 i = 0; i < m_oTable.Table.size(); ++i)
	{
		auto pRow = m_oTable.Table[i];
		if (pRow->AchievementActiveDays > 0)
		{
			AchivementActivityManager::Instance()->Add(pRow);
		}
	}

	RegistHandler(new LevelupAchivement);
	RegistHandler(new StageCompeleteAchivement);
	RegistHandler(new ChapterCompleteAchivement);
	RegistHandler(new DrawLotteryAchivement);
	RegistHandler(new PromoteAchivement);
	RegistHandler(new PPTAchivement);
	RegistHandler(new LoginAchivement);
	RegistHandler(new FashionAchivement);
	return true;
}

void AchivementMgr::Uninit()
{
	for (int i = 0; i < ACHEVT_MAX; ++i)
	{
		if (m_Handlers[i])
		{
			delete m_Handlers[i];
		}
	}

	memset(m_Handlers, 0, sizeof(m_Handlers));

	m_oTable.Clear();
}

void AchivementMgr::OnAchivementEvent(AchivementEvent &evt)
{
	foreach(i in m_oTable.Table)
	{
		AchivementTable::RowData *pRow = *i;
		if (evt.pRole->GetLevel() < pRow->AchievementLevel)
			continue;

		if (evt.pRole->Get<AchieveV2Record>()->GetAchivementState(pRow->AchievementID) != ACHST_NOT_ACHIVE)
			continue;

		m_Handlers[evt.type]->OnAchiveEvent(evt, pRow);
	}
}


int AchivementMgr::DoCompleteAchivement(Role *pRole, int aid)
{
	if (pRole->Get<AchieveV2Record>()->GetAchivementState(aid) != ACHST_ACHIVE_NOFETCH)
	{
		return KKSG::ERR_ACHIVE_NOTCOMPLETE;
	}

	pRole->Get<AchieveV2Record>()->SetAchivementState(aid, ACHST_FETCHED);

	foreach(i in m_oTable.Table)
	{
		AchivementTable::RowData *pRow = *i;
		if (pRow->AchievementID == aid)
		{
			if (pRow->AchievementFetchDays > 0 && 
				GSConfig::Instance()->GetGameServerOpenDays() > pRow->AchievementFetchDays)
			{
				pRole->Get<AchieveV2Record>()->SetAchivementState(aid, ACHST_EXCEED);
				return KKSG::ERR_SUCCESS;
			}

			BagGiveItemTransition transition(pRole);
			transition.SetReason(ItemFlow_Achievement, ItemFlow_Achievement_Give);
			foreach(j in pRow->AchievementItem)
			{
				const Sequence<int, 2> &seq = *j;
				transition.GiveItem(seq[0], seq[1]);
			}

			transition.NotifyClient();

			return KKSG::ERR_SUCCESS;
		}
	}

	return KKSG::ERR_ACHIVE_NOTCONFIG;
}

void AchivementMgr::RegistHandler(IAchivementHandler *pHandler)
{
	m_Handlers[pHandler->GetRegistEvent()] = pHandler;
}

bool AchivementMgr::Reload()
{
	Uninit();
	return Init();
}

void AchivementEvent::Emit()
{
	AchivementMgr::Instance()->OnAchivementEvent(*this);
}