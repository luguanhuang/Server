#include "pch.h"
#include "pb/project.pb.h"
#include "AchivementHandler.h"
#include "unit/role.h"
#include "foreach.h"
#include "scene/scene.h"
#include "rolefashion.h"
#include "achievev2Record.h"
#include <time.h>

static bool IsOutofDays(int day)
{
	if (day > 0 && GSConfig::Instance()->GetGameServerOpenDays() > day)
	{
		return true;
	}

	return false;
}

void IAchivementHandler::RegistFunction(AchiveType type, AchivementPred fun)
{
	AchivementPredPair pair;
	pair.type = type;
	pair.function = fun;
	m_Handlers.push_back(pair);
}

void IAchivementHandler::OnAchiveEvent(AchivementEvent & evt, AchivementTable::RowData *pConf)
{
	foreach (i in m_Handlers)
	{
		if (i->type == pConf->AchievementType && i->function(evt, pConf))
		{
			evt.pRole->Get<AchieveV2Record>()->SetAchivementState(pConf->AchievementID, ACHST_ACHIVE_NOFETCH);
		}
	}
}

LevelupAchivement::LevelupAchivement()
{
	RegistFunction(ACHTYPE_LEVEL, LevelHandler);
}

bool LevelupAchivement::LevelHandler(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	return pConf->AchievementParam <= ((AchivementLevelupEvent &)evt).level;
}

bool StageCompeleteAchivement::NormalStageComplete(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	AchivementStageCompleteEvent &stageEvt = (AchivementStageCompleteEvent &)evt;
	return pConf->AchievementParam == stageEvt.sceneID;
}

bool StageCompeleteAchivement::NormalStage3Star(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	AchivementStageCompleteEvent &stageEvt = (AchivementStageCompleteEvent &)evt;
	return pConf->AchievementParam == stageEvt.sceneID && stageEvt.rank == 3;
}

StageCompeleteAchivement::StageCompeleteAchivement()
{
	RegistFunction(ACHTYPE_NORMALSTAGE_COMPLETE, NormalStageComplete);
	RegistFunction(ACHTYPE_NORMALSTAGE_3START, NormalStage3Star);
}

ChapterCompleteAchivement::ChapterCompleteAchivement()
{
	RegistFunction(ACHTYPE_CHAPTER_COMPLETE, ChapterComplete);
}

bool ChapterCompleteAchivement::ChapterComplete(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	AchivementChapterCompleteEvent &chapterEvt = (AchivementChapterCompleteEvent &)evt;
	return pConf->AchievementParam == chapterEvt.chapter;
}

DrawLotteryAchivement::DrawLotteryAchivement()
{
	RegistFunction(ACHTYPE_DRAWLOTTERY, DrawLottery);
}

bool DrawLotteryAchivement::DrawLottery(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	AchivementDrawLotteryEvent &drawLotteryEvt = (AchivementDrawLotteryEvent &)evt;
	if (drawLotteryEvt.drawType == KKSG::Sprite_Draw_One)
	{
		return true;
	}

	return false;
}

PromoteAchivement::PromoteAchivement()
{
	RegistFunction(ACHTYPE_PROMOTE, Promote);
}

bool PromoteAchivement::Promote(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	return true;
}

PPTAchivement::PPTAchivement()
{
	RegistFunction(ACHTYPE_PPT, ProcessPowerPointChange);
}

bool PPTAchivement::ProcessPowerPointChange(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	AchivementPowerPointEvent &e = (AchivementPowerPointEvent &)evt;
	return e.ppt >= pConf->AchievementParam;
}

LoginAchivement::LoginAchivement()
{
	RegistFunction(ACHTYPE_LOGIN, login);
}

bool LoginAchivement::login(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	return isSameDayWithCurDay(pConf->AchievementParam);
}

bool LoginAchivement::isSameDayWithCurDay(UINT16 day)
{
	UINT32 openDay = GSConfig::Instance()->GetGameServerOpenDays();

	if ((openDay + 1) == day)
	{
		return true;
	}

	return false;

}

bool LoginAchivement::isSameDayWithCur(UINT32 curDay)//YYYYMMDD curDay¸ñÊ½
{
	int year = curDay/10000;
	int month = curDay/100%100;
	int day = curDay%100;

	time_t cur = time(0);
	struct tm * localDate = localtime(&cur);
	if ((localDate->tm_year + 1900) == year && (localDate->tm_mon + 1) == month && (localDate->tm_mday) == day)
	{
		return true;
	}
	return false;

}

FashionAchivement::FashionAchivement()
{
	RegistFunction(ACHTYPE_FASHION, addFashion);
}

bool FashionAchivement::addFashion(AchivementEvent &evt, AchivementTable::RowData *pConf)
{
	if (IsOutofDays(pConf->AchievementActiveDays))
	{
		return false;
	}

	AchivementFashionEvent &e = (AchivementFashionEvent &)evt;
	return e.fashionNum >= pConf->AchievementParam;

}
