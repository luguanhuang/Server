#ifndef __TLOGGERBATTLE_H__
#define __TLOGGERBATTLE_H__

#include "tlogger.h"

class Scene;
class TSecBattleFlow : public TCommonLog
{
public:
	explicit TSecBattleFlow(Role* poRole)
		:TCommonLog(poRole)
	{

	};
	virtual ~TSecBattleFlow() { };

	void FillCommon(Scene* scene);

	void Send(const std::vector<std::string>& fields, const std::map<std::string, int>& handles);
public:
	std::map<std::string, std::string> m_data;
	static std::map<std::string, std::string> m_specaildefault;
};

///> 战斗开始
class DummyRole;
struct PlayerInfo;
struct MonsterInfo;
class TSecRoundStartFlow : public TSecBattleFlow
{
public:
	explicit TSecRoundStartFlow(Role* poRole)
		:TSecBattleFlow(poRole)
		{
		};
	virtual ~TSecRoundStartFlow(){}
	const char* GetTableName() const { return "SecRoundStartFlow";}

	static bool Init();
	bool FillBegin(Scene* scene);

	bool FillPkRank(const std::string& rank);
	bool FillAttr();
	bool FillSkill();
	bool FillMonster(Scene* scene);
	bool FillPlayers(UINT32 fightgroup, const std::map<UINT64, PlayerInfo>& players);

public:
	static std::map<std::string, int> m_field2handler;
	static std::vector<std::string> m_fields;
};

///> 战斗结束，中间数据
class TSecRoundEndFlow : public TSecBattleFlow
{
public:
	explicit TSecRoundEndFlow(Role* poRole)
		:TSecBattleFlow(poRole)
		,m_endtype(0)
		{
		}
	virtual ~TSecRoundEndFlow(){}
	const char* GetTableName() const { return "SecRoundEndFlow";}

	static bool Init();
	bool FillBegin(Scene* scene);

	bool FillMonsterKill(const MonsterInfo& info);
	bool FillPlayerDeath(int death, int revive);
	bool FillTimeSpan(UINT32 time);
	bool FillEnd(Scene* scene);
	bool FillPk(const std::string& change, const std::string& rank);

public:
	UINT32 m_endtype;
	static std::map<std::string, int> m_field2handler;
	static std::vector<std::string> m_fields;
};

///> 战斗结束
class TSecRoundEndCount : public TSecBattleFlow
{
public:
	explicit TSecRoundEndCount(Role* poRole)
		:TSecBattleFlow(poRole){}
	virtual ~TSecRoundEndCount(){}
	const char* GetTableName() const { return "SecRoundEndCount";}

	static bool Init();
	bool FillBegin(Scene* scene);

public:
	static std::map<std::string, int> m_field2handler;
	static std::vector<std::string> m_fields;
};

#endif