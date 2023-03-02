#ifndef __MATCHHANDLER_H__
#define __MATCHHANDLER_H__

#include "pb/enum.pb.h"

class CRole;
class Team;
class HMatchHandler;

//跨服组队和匹配都正常运行
bool TIsCrossTMRun();

class HMatchBase
{
public:
	HMatchBase(int type);
	virtual ~HMatchBase() {}
	//匹配相关
	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

	//条件相关
	virtual KKSG::ErrorCode MatchCondition(CRole* pRole);
	virtual KKSG::ErrorCode FillMatchData(CRole* pRole, KKSG::KMatchRole* krole);

protected:
	int MatchSendToWorld(CRole* pRole, KKSG::KMatchCommonArg& roArg, KKSG::KMatchRole* krole = NULL);
};

class HMatchExp : public HMatchBase
{
public:
	HMatchExp():HMatchBase(KKSG::KMT_EXP){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

private:
	static HMatchExp GlobalHMatchExp;
};

class HMatchPvp : public HMatchBase
{
public:
	HMatchPvp():HMatchBase(KKSG::KMT_PVP){}

private:
	static HMatchPvp GlobalHMatchPvp;
};

class HMatchHero : public HMatchBase
{
public:
	HMatchHero():HMatchBase(KKSG::KMT_HERO){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

	bool IsMatchInWorld();

private:
	static HMatchHero GlobalHMatchHero;
};

class HMatchMoba : public HMatchBase
{
public:
	HMatchMoba():HMatchBase(KKSG::KMT_MOBA){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

	//条件相关
	virtual KKSG::ErrorCode MatchCondition(CRole* pRole);
	virtual KKSG::ErrorCode FillMatchData(CRole* pRole, KKSG::KMatchRole* krole);

	bool IsMatchInWorld();

private:
	static HMatchMoba GlobalHMatchMoba;
};

class HMatchPk : public HMatchBase
{
public:
	HMatchPk():HMatchBase(KKSG::KMT_PK){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	
private:
	static HMatchPk globalHMatchPk;
};

class HMatchPkTwo : public HMatchBase
{
public:
	HMatchPkTwo():HMatchBase(KKSG::KMT_PKTWO){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

private:
	int Start(UINT64 roleID, int teamID, std::vector<UINT64>& roleIDs, HMatchHandler* pHandler);
	void Stop(UINT64 roleID, int teamID, HMatchHandler* pHandler);
	const KKSG::PkRoleRec* GetRoleRec(UINT64 roleID, HMatchHandler* pHandler);

private:
	static HMatchPkTwo GlobalHMatchPkTwo;
};

class HMatchCustomPkTwo : public HMatchBase
{
public:
	HMatchCustomPkTwo():HMatchBase(KKSG::KMT_CUSTOM_PKTWO){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

private:
	int Start(UINT64 roleID, int teamID, std::vector<UINT64>& roleIDs, HMatchHandler* pHandler);
	void Stop(UINT64 roleID, int teamID, HMatchHandler* pHandler);

private:
	static HMatchCustomPkTwo GlobalHMatchCustomPkTwo;
};

class HMatchLeague : public HMatchBase
{
public:
	HMatchLeague():HMatchBase(KKSG::KMT_LEAGUE){}

	virtual int Start(CRole* pRole, HMatchHandler* pHandler);
	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(CRole* pRole, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

private:
	static HMatchLeague GlobalHMatchLeague;
};

class HMatchSkyCraft : public HMatchBase
{
public:
	HMatchSkyCraft(): HMatchBase(KKSG::KMT_SKYCRAFT) {}

	virtual int Start(Team* pTeam, HMatchHandler* pHandler);
	virtual void Stop(Team* pTeam, HMatchHandler* pHandler);

private:
	static HMatchSkyCraft GlobalHMatchSkyCraft;
};

class HMatchWeekendAct : public HMatchBase
{
public:
	HMatchWeekendAct(): HMatchBase(KKSG::KMT_WEEKEND_ACT) {}

private:
	static HMatchWeekendAct GlobalHMatchWeekendAct;

};

class HMatchSurvive : public HMatchBase
{
public:
	HMatchSurvive(): HMatchBase(KKSG::KMT_SURVIVE){}

private:
	static HMatchSurvive GlobalHMatchSurvive;
};

struct HMData
{
	HMData()
	{
		joinTeam = false;
		expid = 0;
		pkAddRobot = false;
		leagueTeamId = 0;
		stId = 0;
	}
	bool joinTeam;

	int expid;

	bool pkAddRobot;
	//KKSG::PkRoleRec pkRec;
	KKSG::PkMatchInfo pkMatchInfo;

	UINT64 leagueTeamId;
	UINT64 stId;

	double eloPoint;

	KKSG::KMatchCommonArg matchArg;
	KKSG::KMatchCommonResM2G matchData;
};

class HMatchHandler
{
public:
	//开始匹配 返回对应错误码
	int Start(CRole* pRole, int type);
	int Start(Team* pTeam, int type);

	//取消匹配 必须成功 (待优化 TODO)
	void Stop(CRole* pRole, int type);
	void Stop(Team* pTeam, int type);

	//匹配完成 恢复状态
	void Done(CRole* pRole, int type);
	void Done(Team* pTeam, int type);

	void OnJoinTeam(CRole* pRole);
	void OnChangeScene(CRole* pRole);
	void OnLogout(CRole* pRole);

	HMData& Data() { return m_data; }

	KKSG::ErrorCode RoleCondition(CRole* pRole, int type, KKSG::KMatchRole* krole);//待整合

	void CheckRoleStop(CRole* pRole);
	void CheckRoleStop(CRole* pRole, int type);
	void CheckTeamStop(Team* pTeam);
	void CheckTeamStop(Team* pTeam, int type);

	static UINT64 GetMatchUniqueIndex(bool inc = false);

private:
	int RoleMatchCond(CRole* pRole, int type);

	HMatchBase* GetMatchHandler(int type);

	HMData m_data;

	static std::unordered_map<int, HMatchBase*> m_allMatch;

	friend class HMatchBase;
};

#endif