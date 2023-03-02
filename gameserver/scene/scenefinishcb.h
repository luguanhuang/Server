#ifndef __SCENEFINISHCB_H__
#define __SCENEFINISHCB_H__

#include "scenecb.h"

// common 
class NormalBattleResultListener;
class CommonSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

private:
	void _CheckAchievement(Scene* scene, bool iswin, std::vector<Role *> &roRoles);
	void FillExDataToMs(NormalBattleResultListener* pListener, Scene* pScene, bool iswin, std::vector<Role *> &roRoles);
public:
	static CommonSceneFinishCB GlobalCommonSceneFinishCB;
};

// arena
class ArenaSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static ArenaSceneFinishCB GlobalArenaSceneFinishCB;
};

// pk
class PkSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static PkSceneFinishCB GlobalPkSceneFinishCB;
};

//pk two
class PkTwoSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static PkTwoSceneFinishCB GlobalPkTwoSceneFinishCB;
};

// custombattle pk
class CustomPkSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static CustomPkSceneFinishCB GlobalCustomPkSceneFinishCB;
};

// custombattle pktwo
class CustomPkTwoSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static CustomPkTwoSceneFinishCB GlobalCustomPkTwoSceneFinishCB;
};

// pvp 
class PvpSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static PvpSceneFinishCB GlobalPvpSceneFinishCB;
};

//gmf
class GmfSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static GmfSceneFinishCB GlobalGmfSceneFinishCB;
};

// dragonexp ÁúÖ®Ô¶Õ÷
class DragonExpSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static DragonExpSceneFinishCB GlobalDragonExpSceneFinishCB;

};

class SkyCitySceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);

	static SkyCitySceneFinishCB GlobalSkyCitySceneFinishCB;
};

class ResWarSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static ResWarSceneFinishCB GlobalResWarSceneFinishCB;
};


class HorseSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static HorseSceneFinishCB GlobalHorseSceneFinishCB;
};

class HeroBattleSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static HeroBattleSceneFinishCB GlobalHeroBattleSceneFinishCB;
};

class MobaBattleSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static MobaBattleSceneFinishCB GlobalMobaBattleSceneFinishCB;
};

class InvFightSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static InvFightSceneFinishCB GlobalInvFightSceneFinishCB;
};

class SurviveSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static SurviveSceneFinishCB GlobalSurviveSceneFinishCB;
};

class LeagueSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static LeagueSceneFinishCB GlobalLeagueSceneFinishCB;
};

class WeekEnd4v4MonsterFightSceneFinishCB : public ISceneListener
{
public:
    virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
    static WeekEnd4v4MonsterFightSceneFinishCB GlobalMonsterFightSceneFinishCB;
};
class WeekEnd4v4GhostActionSceneFinishCB : public ISceneListener
{
public:
    virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
    static WeekEnd4v4GhostActionSceneFinishCB GlobalGhostActionSceneFinishCB;
};
class WeekEnd4v4LiveChallengeSceneFinishCB : public ISceneListener
{
public:
    virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
    static WeekEnd4v4LiveChallengeSceneFinishCB GlobalLiveChallengeSceneFinishCB;
};
class WeekEnd4v4CrazyBombSceneFinishCB : public ISceneListener
{
public:
    virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
    static WeekEnd4v4CrazyBombSceneFinishCB GlobalCrazyBombSceneFinishCB;
};
class WeekEnd4v4HorseRacingSceneFinishCB : public ISceneListener
{
public:
    virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
    static WeekEnd4v4HorseRacingSceneFinishCB GlobalHorseRacingSceneFinishCB;
};
class WeekEnd4v4DuckSceneFinishCB : public ISceneListener
{
public:
    virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
    static WeekEnd4v4DuckSceneFinishCB GlobalDuckSceneFinishCB;
};

class BattleFieldSceneFinishCB : public ISceneListener
{
public:
	virtual void OnSceneFinished(Scene *pScene, std::vector<Role *> &roRoles, bool win);
	static BattleFieldSceneFinishCB GlobalBattleFieldSceneFinishCB;
};


#endif // __SCENEFINISHCB_H__