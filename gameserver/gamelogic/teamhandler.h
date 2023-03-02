#ifndef __TEAMHANDLER_H__
#define __TEAMHANDLER_H__

#include "table/ExpeditionTable.h"
#include "table/SceneTable.h"
#include "teammember.h"
#include "define/teamdef.h"


class Unit;
class Role;
class CTeamMember;
class TeamCopy;

namespace KKSG
{
	class BuyTeamSceneCountRet;
}

UINT32 GetSampleSceneID(ExpeditionTable::RowData *pConf);

class ITeamTypeHandlerBase
{
public:
	ITeamTypeHandlerBase(int type);
	virtual ~ITeamTypeHandlerBase(){};

	virtual int GetDayCountLeft(Role *pRole);

	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);
	
	int ChangeExpIDBy(TeamCopy * pTeam, int expID , UINT64& problemID);
	int CheckBattleStartCondition(TeamCopy *pTeam, UINT64& problemID);

	int CheckHelperCondition(Role* pRole, ExpeditionTable::RowData *pRow);
	int CheckUseTicketCondition(Role* pRole, ExpeditionTable::RowData *pRow);

	virtual void BuyCount(Role* pRole, KKSG::BuyTeamSceneCountRet& roRes){ }

	static ITeamTypeHandlerBase* GetTeamHanderByType(int pType);

protected:
	
	// 0 == KKSG::ERR_SUCCESS
	int CheckRoleInGuild(ExpeditionTable::RowData *pConf, UINT64 leaderID, const std::vector<TeamCopyMember> &members, UINT64& problemRoleID);

	//默认判断 如果不同于默认判断需要继承
	virtual int CheckIdipCondition(Role* pRole);

	int m_type;

private:
	static std::unordered_map<int, ITeamTypeHandlerBase*> m_TypeHandler;

};

class ExpTeamHandler : public ITeamTypeHandlerBase
{
public:
	ExpTeamHandler():ITeamTypeHandlerBase(EXP_TEAM_SCENE){}
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static ExpTeamHandler GlobalExpTeamHandler;
};

class GuildTeamHandler : public ITeamTypeHandlerBase
{
public:
	GuildTeamHandler():ITeamTypeHandlerBase(GUILD_TEAM_SCENE){}
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static GuildTeamHandler GlobalGuildTeamHandler;
};

class NestTeamHandler : public ITeamTypeHandlerBase
{
public:
	NestTeamHandler():ITeamTypeHandlerBase(NEST_TEAM_SCENE){}
	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);

	virtual void BuyCount(Role* pRole, KKSG::BuyTeamSceneCountRet& roRes);

	static NestTeamHandler GlobalNextTeamHandler;
};

class AbyssTeamHandler : public ITeamTypeHandlerBase
{
public:
	AbyssTeamHandler():ITeamTypeHandlerBase(ABYSS_TEAM_SCENE){}

	virtual void BuyCount(Role* pRole, KKSG::BuyTeamSceneCountRet& roRes);

	static AbyssTeamHandler GlobalAbyssTeamHandler;
};

class PvpTeamHandler : public ITeamTypeHandlerBase
{
public:
	PvpTeamHandler():ITeamTypeHandlerBase(PVP_TEAM_SCENE){}
	virtual int GetDayCountLeft(Role *pRole) { return 0;}
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static PvpTeamHandler GlobalPvpTeamHandler;
};

class PkTwoTeamHandler : public ITeamTypeHandlerBase
{
public:
	PkTwoTeamHandler():ITeamTypeHandlerBase(TEAM_TYPE_PKTWO){}
	virtual int GetDayCountLeft(Role *pRole) { return 0;}
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static PkTwoTeamHandler GlobalPvpTeamHandler;
};

class DragonTeamHandler : public ITeamTypeHandlerBase
{
public:
	DragonTeamHandler():ITeamTypeHandlerBase(DRAGON_TEAM_SCENE){}
	virtual int GetDayCountLeft(Role *pRole) { return 0; }
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	static DragonTeamHandler GlobalDragonTeamHandler;
};

class GoddessTeamHandler : public ITeamTypeHandlerBase
{
public:
	GoddessTeamHandler():ITeamTypeHandlerBase(GODDESS_TEAM_SCENE){}

	static GoddessTeamHandler GlobalGoddessTeamHandler;
};

class HeroBattleTeamHandler : public ITeamTypeHandlerBase
{
public:
	HeroBattleTeamHandler():ITeamTypeHandlerBase(HEROBATTLE_TEAM_SCENE){}
	virtual int GetDayCountLeft(Role *pRole) { return 0;}

	static HeroBattleTeamHandler GlobalHeroBattleTeamHandler;
};

class MobaBattleTeamHandler : public ITeamTypeHandlerBase
{
public:
	MobaBattleTeamHandler():ITeamTypeHandlerBase(MOBABATTLE_TEAM_SCENE){}
	virtual int GetDayCountLeft(Role *pRole) { return 0;}

	static MobaBattleTeamHandler GlobalMobaBattleTeamHandler;
};

class EndlessabyssTeamHandler : public ITeamTypeHandlerBase
{
public:
	EndlessabyssTeamHandler():ITeamTypeHandlerBase(ENDLESSABYSS_SCENE){}

	static EndlessabyssTeamHandler GlobalEndlessabyssTeamHandler;

};

class SkyCityTeamHandler: public ITeamTypeHandlerBase
{
public:
	SkyCityTeamHandler():ITeamTypeHandlerBase(SKYCITY_SCENE){}
	virtual int GetDayCountLeft(Role *pRole) { return 1;}//应该为0且前端不显示

	virtual int CheckIdipCondition(Role* pRole);
	static SkyCityTeamHandler GlobalSkyCityTeamHandler;
};


class ResWarTeamHandler: public ITeamTypeHandlerBase
{
public:
	ResWarTeamHandler():ITeamTypeHandlerBase(RESWAR_SCENE){}
	virtual int GetDayCountLeft(Role *pRole) { return 0;}

	virtual int CheckIdipCondition(Role* pRole);

	static ResWarTeamHandler GlobalResWarTeamHandler;
};

class GuildCampTeamHandler : public ITeamTypeHandlerBase
{
public:
	GuildCampTeamHandler():ITeamTypeHandlerBase(GUILDCAMP_TEAM_SCENE){}

	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static GuildCampTeamHandler GlobalGuildCampTeamHandler;
};

class WeekNestTeamHandler : public ITeamTypeHandlerBase
{
public:
	WeekNestTeamHandler():ITeamTypeHandlerBase(WEEK_NEST_TEAM_SCENE){}

	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static WeekNestTeamHandler GlobalWeekNestTeamHandler;
};

class PartnerTeamHandler : public ITeamTypeHandlerBase
{
public:
	PartnerTeamHandler():ITeamTypeHandlerBase(PARTNER_TEAM_SCENE){}

	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static PartnerTeamHandler GlobalPartnerTeamHandler;
};

class LeagueTeamHandler : public ITeamTypeHandlerBase
{
public:
	LeagueTeamHandler() : ITeamTypeHandlerBase(LEAGUETEAM_SCENE) {}

	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static LeagueTeamHandler GlobalLeagueTeamHandler;
};

class SkyCraftTeamHandler : public ITeamTypeHandlerBase
{
public:
	SkyCraftTeamHandler() : ITeamTypeHandlerBase(SKYCRAFT_SCENE) {}

	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static SkyCraftTeamHandler GlobalSkyCraftTeamHandler;
};

class FestivalTeamHandler : public ITeamTypeHandlerBase
{
public:
	FestivalTeamHandler() : ITeamTypeHandlerBase(FESTIVAL_TEAM_SCENE) {}

	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static FestivalTeamHandler GlobalFestivalTeamHandler;
};

class TaskTeamHandler : public ITeamTypeHandlerBase
{
public:
	TaskTeamHandler() : ITeamTypeHandlerBase(TASK_TEAM_SCENE) {}

	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);
	virtual int CheckIdipCondition(Role* pRole) { return 0; }

	static TaskTeamHandler GlobalTaskTeamHandler;
};

class WeekEnd4v4TeamHandler : public ITeamTypeHandlerBase
{
public:
    WeekEnd4v4TeamHandler() : ITeamTypeHandlerBase(TEAM_WEEKEND4V4_SCENE) {}
    virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
    virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	virtual int CheckIdipCondition(Role* pRole) { return 0; }

    static WeekEnd4v4TeamHandler GlobalWeekEnd4v4TeamHandler;
};

//class TowerTeamHandler : public ITeamTypeHandlerBase
//{
//public:
//	virtual int GetDayCountLeft(Role *pRole);
//	virtual int GetDayCountLeft(Role *pRole, ExpeditionTable::RowData *pConf);
//	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow, UINT64 *pGID);
//	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, UINT64 *pGID);
//
//	static TowerTeamHandler GlobalTowerTeamHandler;
//};
//

class CustomBattlePkTwoTeamHandler: public ITeamTypeHandlerBase
{
public:
    CustomBattlePkTwoTeamHandler() : ITeamTypeHandlerBase(TEAM_TYPE_CUSTOMPKTWO) {}
    virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
    virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	virtual int CheckIdipCondition(Role* pRole) { return 0; }

    static CustomBattlePkTwoTeamHandler GlobalCustomBattlePkTwoTeamHandler;
};

class BioHellTeamHandler : public ITeamTypeHandlerBase
{
public:
	BioHellTeamHandler() : ITeamTypeHandlerBase(TEAM_TYPE_BIOHELL) {}
	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	static BioHellTeamHandler GlobalBioHellTeamHandler;
};

class MarriageCoupleTeamHandler : public ITeamTypeHandlerBase
{
public:
	MarriageCoupleTeamHandler () : ITeamTypeHandlerBase(TEAM_TYPE_COUPLE) {}
	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	static MarriageCoupleTeamHandler GlobalMarriageCoupleTeamHandler;
};

class MarriageTeamHandler : public ITeamTypeHandlerBase
{
public:
	MarriageTeamHandler () : ITeamTypeHandlerBase(TEAM_TYPE_MARRIAGE) {}
	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	static MarriageTeamHandler GlobalMarriageTeamHandler;
};

class CompeteDragonTeamHandler : public ITeamTypeHandlerBase
{
public:
	CompeteDragonTeamHandler():ITeamTypeHandlerBase(TEAM_TYPE_COMPETE_DRAGON){}
	virtual int CheckOpenCondition(Role *pRole, ExpeditionTable::RowData *pRow);
	virtual int CheckEnterCondtion(Role *pRole, ExpeditionTable::RowData *pRow, TeamEnterData* pExtrData);

	static CompeteDragonTeamHandler GlobalCompeteDragonTeamHandler;
};

#endif
