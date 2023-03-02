#ifndef __TEAMHANDLER_H__
#define __TEAMHANDLER_H__

#include "table/ExpeditionTable.h"
#include "define/teamdef.h"

namespace KKSG
{
	class TeamOPArg;
	class TeamOpRes;
}

class THBase;
class CRole;
class Team;

class TeamHandler
{
public:
	int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf);
	int JoinCond(CRole* pRole, Team* pTeam);
	int InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole);
	int ChangeExpCond(CRole* pRole, Team* pTeam, ExpeditionTable::RowData* pConf, KKSG::TeamOPRes& roRes);
	int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);
	bool CanSeeCond(CRole* pRole, Team* pTeam);

	//加入之前
	void OnJoinTeam(CRole* pRole, Team* pTeam);
	//离开之前
	void OnLeaveTeam(CRole* pRole, Team* pTeam);
	//改变Exp之前
	void OnChangeExp(Team* pTeam, ExpeditionTable::RowData* pToConf);

	int CheckExpCond(int expid);
	UINT32 GetExpOpenTime(int expid);
	int CheckGuildCond(CRole* pRole, ExpeditionTable::RowData* pConf);

private:
	void CheckStopMatch(Team* pTeam);

	THBase* GetSubHandler(int type);
	static std::unordered_map<int, THBase*> m_subHandler;

	friend class THBase;
};

class THBase
{
public:
	THBase(int type);
	virtual ~THBase();

	virtual int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf) { return 0; }
	virtual int JoinCond(CRole* pRole, Team* pTeam) { return 0; }
	virtual int InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole) { return 0; }
	virtual int ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes) { return 0; }
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam) { return true; }

	virtual void OnJoinTeam(CRole* pRole, Team* pTeam) {}
	virtual void OnLeaveTeam(CRole* pRole, Team* pTeam) {}
	virtual void OnChangeExpFrom(Team* pTeam, ExpeditionTable::RowData* pToConf) {}
};

class THGoddess : public THBase
{
public:
	THGoddess():THBase(GODDESS_TEAM_SCENE){}
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam);

private:
	static THGoddess GlobalTHGoddess;
};

class THEndlessAbyss : public THBase
{
public:
	THEndlessAbyss():THBase(ENDLESSABYSS_SCENE){}
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam);

private:
	static THEndlessAbyss GlobalTHEndlessAbyss;
};

class THWeekNest : public THBase
{
public:
	THWeekNest():THBase(WEEK_NEST_TEAM_SCENE){}
	virtual int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf);

private:
	static THWeekNest GlobalHTWeekNest;
};

class THGuildCamp : public THBase
{
public:
	THGuildCamp():THBase(GUILDCAMP_TEAM_SCENE){}
	virtual int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf);

private:
	static THGuildCamp GlobalHTGuildCamp;
};

class THResWar : public THBase
{
public:
	THResWar():THBase(RESWAR_SCENE){}
	virtual int JoinCond(CRole* pRole, Team* pTeam);
	virtual void OnJoinTeam(CRole* pRole, Team* pTeam);
	virtual void OnLeaveTeam(CRole* pRole, Team* pTeam);

private:
	static THResWar GlobalHTResWar;
};

class THLeague : public THBase
{
public:
	THLeague():THBase(LEAGUETEAM_SCENE){}
	virtual int JoinCond(CRole* pRole, Team* pTeam);
	virtual int InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole);
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);
	virtual int ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes);
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam);

private:
	static THLeague GlobalHTLeague;
};

class THSkyCraft : public THBase
{
public:
	THSkyCraft() : THBase(SKYCRAFT_SCENE){}
	virtual int JoinCond(CRole* pRole, Team* pTeam);
	virtual int InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole);
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);
	virtual int ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes);
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam);

private:
	static THSkyCraft GlobalSkyCraft;
};

class THSkyCity : public THBase
{
public:
	THSkyCity():THBase(SKYCITY_SCENE){}
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);

private:
	static THSkyCity GlobalTHSkyCity;
};

class THPartner : public THBase
{
public:
	THPartner():THBase(PARTNER_TEAM_SCENE){}
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);

private:
	static THPartner GlobalTHPartner;
};

class THCustomPkTwo : public THBase
{
	THCustomPkTwo():THBase(TEAM_TYPE_CUSTOMPKTWO){}
	virtual int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf);
	virtual int JoinCond(CRole* pRole, Team* pTeam);

private:
	static THCustomPkTwo GlobalTHCustomPkTwo;
};

class THMarriageCouple : public THBase
{
	THMarriageCouple():THBase(TEAM_TYPE_COUPLE){}
	virtual int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf);
	virtual int JoinCond(CRole* pRole, Team* pTeam);
	virtual int ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes);
	virtual int InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole);
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam);
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);

private:
	static THMarriageCouple GlobalTHMarriageCouple;
};

class THMarriage : public THBase
{
	THMarriage():THBase(TEAM_TYPE_MARRIAGE){}
	virtual int CreateCond(CRole* pRole, ExpeditionTable::RowData* pConf);
	virtual int JoinCond(CRole* pRole, Team* pTeam);
	virtual int ChangeExpCond(CRole* pRole, Team* pTeam, KKSG::TeamOPRes& roRes);
	virtual int InviteCond(Team* pTeam, CRole* pFromRole, CRole* pToRole);
	virtual bool CanSeeCond(CRole* pRole, Team* pTeam);
	virtual int StartBattleCond(CRole* pRole, Team* pTeam, const KKSG::TeamOPArg& roArg, KKSG::TeamOPRes& roRes);

private:
	static THMarriage GlobalTHMarriage;
};


#endif