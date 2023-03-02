#ifndef __ACHIVEMENTDEF_H__
#define __ACHIVEMENTDEF_H__

enum AchiveState
{
	ACHST_EXCEED    = 0, // 超过了成就领取时间
	ACHST_FETCHED = 1,
	ACHST_NOT_ACHIVE  = 2,
	ACHST_ACHIVE_NOFETCH = 3,
};

enum AchiveType
{
	ACHTYPE_NONE  = 0,
	ACHTYPE_LEVEL = 1,
	ACHTYPE_NORMALSTAGE_COMPLETE = 2,
	ACHTYPE_NORMALSTAGE_3START   = 3,
	ACHTYPE_ELITESTAGE_COMPLETE  = 4,
	ACHTYPE_ELITESTAGE_3START    = 5,
	ACHTYPE_CHAPTER_COMPLETE     = 6,
	ACHTYPE_DRAWLOTTERY          = 7,
	ACHTYPE_PROMOTE              = 8,
	ACHTYPE_PPT                  = 9,
	ACHTYPE_LOGIN                = 10,
	ACHTYPE_FASHION              = 11,
	ACHTYPE_MAX
};

enum AchiveEvent
{
	ACHEVT_NONE,
	ACHEVT_LEVLEUP,
	ACHEVT_STAGE_COMPLETE,
	ACHEVT_CHAPTER_COMPLETE,
	ACHEVT_DRAWLOTTERY,
	ACHEVT_PROMOTE,
	ACHEVT_PPT,
	ACHEVT_LOGIN,
	ACHEVT_FASHION,


	ACHEVT_MAX,
};

class Role;

struct AchivementEvent
{
	AchiveEvent type;
	Role *pRole;

	AchivementEvent(Role *pRole_) : pRole(pRole_)
	{
		type = ACHEVT_NONE;
	}

	void Emit();
};

struct AchivementLevelupEvent : public AchivementEvent
{
	int level;

	AchivementLevelupEvent(Role *pRole, int level) : AchivementEvent(pRole)
	{
		type = ACHEVT_LEVLEUP;
		this->level = level;
	}
};

struct AchivementStageCompleteEvent : public AchivementEvent
{
	int sceneType;
	int sceneID;
	int rank;

	AchivementStageCompleteEvent(Role *pRole, int sceneType, int sceneID, int rank) : AchivementEvent(pRole)
	{
		type = ACHEVT_STAGE_COMPLETE;
		this->sceneType = sceneType;
		this->sceneID = sceneID;
		this->rank = rank;
	}
};

struct AchivementChapterCompleteEvent : public AchivementEvent
{
	int chapter;

	AchivementChapterCompleteEvent(Role *pRole, int chapter) : AchivementEvent(pRole)
	{
		type = ACHEVT_CHAPTER_COMPLETE;
		this->chapter = chapter;
	}
};

struct AchivementDrawLotteryEvent : public AchivementEvent
{
	int drawType;

	AchivementDrawLotteryEvent(Role *pRole, int drawType) : AchivementEvent(pRole)
	{
		type = ACHEVT_DRAWLOTTERY;
		this->drawType = drawType;
	}
};

struct AchivementPromoteEvent : public AchivementEvent
{
	AchivementPromoteEvent(Role *pRole) : AchivementEvent(pRole)
	{
		type = ACHEVT_PROMOTE;
	}
};

struct AchivementPowerPointEvent : public AchivementEvent
{
	double ppt;

	AchivementPowerPointEvent(Role *pRole, double ppt) : AchivementEvent(pRole)
	{
		type = ACHEVT_PPT;
		this->ppt = ppt;
	}
};

struct AchivementLoginEvent : public AchivementEvent
{
	AchivementLoginEvent(Role *pRole) : AchivementEvent(pRole)
	{
		type = ACHEVT_LOGIN;
	}
};

struct AchivementFashionEvent : public AchivementEvent
{
	int fashionNum;
	AchivementFashionEvent(Role *pRole, int num) : AchivementEvent(pRole)
	{
		type = ACHEVT_FASHION;
		fashionNum = num;
	}
};

#endif // __ACHIVEMENTDEF_H__