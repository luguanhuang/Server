#ifndef __ACHIVEMENTHANDLER_H__
#define __ACHIVEMENTHANDLER_H__

#include "AchivementDef.h"
#include "table/AchivementTable.h"

typedef bool (*AchivementPred)(AchivementEvent &, AchivementTable::RowData *);
struct AchivementPredPair
{
	AchiveType     type;
	AchivementPred function;
};

class IAchivementHandler
{
public:
	virtual ~IAchivementHandler() {}

	virtual AchiveEvent GetRegistEvent() = 0;

	void RegistFunction(AchiveType type, AchivementPred fun);

	void OnAchiveEvent(AchivementEvent & evt, AchivementTable::RowData *pConf);

protected:
	std::list<AchivementPredPair> m_Handlers;
};

class LevelupAchivement : public IAchivementHandler
{
public:
	virtual AchiveEvent GetRegistEvent() { return ACHEVT_LEVLEUP; }

	LevelupAchivement();

	static bool LevelHandler(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

class StageCompeleteAchivement : public IAchivementHandler
{
public:

	virtual AchiveEvent GetRegistEvent() { return ACHEVT_STAGE_COMPLETE; }

	StageCompeleteAchivement();

	static bool NormalStageComplete(AchivementEvent &evt, AchivementTable::RowData *pConf);
	static bool NormalStage3Star(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

class ChapterCompleteAchivement : public IAchivementHandler
{
public:

	virtual AchiveEvent GetRegistEvent() { return ACHEVT_CHAPTER_COMPLETE; }

	ChapterCompleteAchivement();

	static bool ChapterComplete(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

class DrawLotteryAchivement : public IAchivementHandler
{
public:
	virtual AchiveEvent GetRegistEvent() { return ACHEVT_DRAWLOTTERY; }

	DrawLotteryAchivement();

	static bool DrawLottery(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

class PromoteAchivement : public IAchivementHandler
{
public:
	virtual AchiveEvent GetRegistEvent() { return ACHEVT_PROMOTE; }

	PromoteAchivement();

	static bool Promote(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

class PPTAchivement : public IAchivementHandler
{
public:
	virtual AchiveEvent GetRegistEvent() { return ACHEVT_PPT; }

	PPTAchivement();

	static bool ProcessPowerPointChange(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

class LoginAchivement : public IAchivementHandler
{
public:
	virtual AchiveEvent GetRegistEvent() { return ACHEVT_LOGIN; }

	LoginAchivement();

	static bool login(AchivementEvent &evt, AchivementTable::RowData *pConf);
	static bool isSameDayWithCurDay(UINT16 curDay);
	static bool isSameDayWithCur(UINT32 curDay);//YYYYMMDD curDay∏Ò Ω
};

class FashionAchivement : public IAchivementHandler
{
public:
	virtual AchiveEvent GetRegistEvent() { return ACHEVT_FASHION; }

	FashionAchivement();

	static bool addFashion(AchivementEvent &evt, AchivementTable::RowData *pConf);
};

#endif // __ACHIVEMENTHANDLER_H__