#ifndef __SCENESTATISTICS_H__
#define __SCENESTATISTICS_H__

#include "utility/tloggerbattle.h"
#include "scene/scenecb.h"

struct MonsterInfo
{
	int MonsterCount;
	int MonsterEndCount;
	int MonsterCount1;
	int MonsterCount2;
	int MonsterCount3;
	int BossCount;
	int BossKillCount;

	MonsterInfo()
	{
		MonsterCount = 0;
		MonsterEndCount = 0;
		MonsterCount1 = 0;
		MonsterCount2 = 0;
		MonsterCount3 = 0;
		BossCount = 0;
		BossKillCount = 0;
	}
};

struct PlayerInfo
{
	PlayerInfo()
	{
		roleid = 0;
		PlayerSide = 0;
		PlayerType = 0;
		PlayerBattlePoint = 0;
	}

	std::string PlayerOpenID;
	UINT64 roleid;
	int PlayerSide;
	int PlayerType;
	int PlayerBattlePoint;
};

struct RoleBattleRecord
{
	RoleBattleRecord(Role* role)
	:start(role)
	,end(role)
	,endcount(role)
	{
		m_clientreceived = false;
	}

	~RoleBattleRecord()
	{
	}

	bool m_clientreceived;
	TSecRoundStartFlow start;
	TSecRoundEndFlow end;
	TSecRoundEndCount endcount;

	///> ¹Ø¿¨µôÂä
	std::map<UINT32, UINT32> itemid2count;

	void AddItem(UINT32 itemid, UINT32 itemcount)
	{
		auto i = itemid2count.find(itemid);
		if (i == itemid2count.end())
		{
			itemid2count[itemid] = itemcount;
		}
		else
		{
			i->second += itemcount;
		}
	}

	bool Init(Scene* scene)
	{
		start.FillBegin(scene);
		end.FillBegin(scene);
		endcount.FillBegin(scene);
		return true;
	}

	bool Send()
	{
		if (!m_clientreceived)
		{
			return false;
		}
		start.Send(TSecRoundStartFlow::m_fields, TSecRoundStartFlow::m_field2handler);
		end.Send(TSecRoundEndFlow::m_fields, TSecRoundEndFlow::m_field2handler);
		endcount.Send(TSecRoundEndCount::m_fields, TSecRoundEndCount::m_field2handler);
		return true;
	}
};

class Scene;
class Role;
class DummyRole;
class SceneTeam;
class XSecuritySceneStatistics;
class SceneStatistics : public ISceneEventListener
{
public:
	SceneStatistics();
	~SceneStatistics();

public:
	void Reset();

	virtual void OnEnterSceneFinally(Scene *pScene, Role* role);
	virtual void OnLeaveScene(Scene *pScene, Role* role);
	virtual void OnEnemyDie(Scene* pScene, Enemy* enemy);
	virtual void OnAddNonRole(Scene *pScene, Unit* unit);

	RoleBattleRecord* GetRecord(UINT64 roleid);
	void DoRank(UINT32 scenetype, std::vector<Role*>& roles);
	DummyRole* DoRankRobot(UINT32 scenetype, SceneTeam* team);
	UINT32 GetRank(UINT64 roleid, UINT32 type);
	UINT32 GetPPTRank(UINT64 roleid);

	void AddItem(UINT64 roleid, UINT32 itemid, UINT32 itemcount);
	void AddDummyRole(Unit* unit);

	void SetPkBeginRank(UINT64 roleid, UINT32 rank);
	void SetPkEndRank(UINT64 roleid, int change, UINT32 rank);

	void SetClientReceived(UINT64 roleid); 
	bool IsCheat() { return m_ischeated; }
	void SetCheat() { m_ischeated = true; }	
	inline XSecuritySceneStatistics* GetSecurityStatistics() { return m_securityStatistics; }
public:
	bool m_ischeated;

	double m_TotalDamage;
	double m_TotalBehit;

	MonsterInfo m_monsterinfo;

	std::multimap<float, UINT64> m_damagerank;
	std::multimap<float, UINT64> m_treatrank;
	std::multimap<UINT32, UINT64> m_ppptrank;

	std::map<UINT64, PlayerInfo> m_players;
	std::map<UINT64, RoleBattleRecord*> m_roleid2record;

private:
	XSecuritySceneStatistics* m_securityStatistics;
};

#endif