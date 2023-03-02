#ifndef __MATCHDATA_H__
#define __MATCHDATA_H__

typedef UINT32 KMatchID;
const KMatchID InvalidKMatchID = 0;

typedef std::pair<KMatchID,UINT32> MatchID2Count;

struct KMatchKey
{
	KMatchKey()
	{
		serverID = 0;
		teamID = 0;
		roleID = 0;
	}

	bool operator < (const KMatchKey& other) const
	{
		if(serverID != other.serverID)
		{
			return serverID < other.serverID;
		}
		if(teamID != other.teamID)
		{
			return teamID < other.teamID;
		}
		return roleID < other.roleID;
	}

	UINT32 serverID;
	int teamID;
	UINT64 roleID;
};

enum MatchDelReason
{
	MDR_NONE = 0,
	MDR_ERR = 1,
	MDR_TIMEOUT = 2,
	MDR_MATCHED = 3,
};

struct Count2MatchID
{
	Count2MatchID()
	{
		count = 0;
		matchID = InvalidKMatchID;
	}

	bool operator < (const Count2MatchID& other) const
	{
		if(count != other.count)
		{
			return count > other.count;
		}
		return matchID < other.matchID;
	}

	UINT32 count;
	KMatchID matchID;
};

class KMatchHalfRoom
{
public:
	KMatchHalfRoom(UINT32 fullCount)
		:m_fullCount(fullCount)
	{
		m_curCount = 0;
	}

	bool Add(KMatchID matchID, UINT32 count);
	bool IsFull();

	inline UINT32 GetCurCount() { return m_curCount; }
	inline std::list<KMatchID>& Units() { return m_units; }

private:
	std::list<KMatchID> m_units;
	UINT32 m_curCount;
	UINT32 m_fullCount;
};

class KMatchRoom
{
public:
	KMatchRoom(UINT32 fullCount)
		:m_room1(fullCount), m_room2(fullCount)
	{
	}
	bool Add(KMatchID matchID, UINT32 count);
	bool IsFull();

	void FillUnits(std::list<KMatchID>& units);

private:
	KMatchHalfRoom m_room1;
	KMatchHalfRoom m_room2;
};

class MatchUnit
{
public:
	MatchUnit(KMatchID matchID, int type, const KKSG::KMatchUnit& kunit);
	virtual ~MatchUnit();

	inline KMatchID GetMatchID() const { return m_matchID; }
	inline int GetType() const { return m_type; }

	UINT32 GetServerID() { return m_kunit.serverid(); }
	UINT64 GetRoleID() { return m_kunit.roleid(); }
	int GetTeamID() { return m_kunit.teamid(); }

	UINT32 GetStartTime() { return m_startTime; }

	KKSG::KMatchUnit& KKSGData() { return m_kunit; }
	virtual UINT32 GetRoleSize() { return m_kunit.roles_size(); }

	bool IsFromTs();
	static bool IsFromTs(const KKSG::KMatchUnit& kunit);

	void FillRoles(std::unordered_set<UINT64>& roleIDs);
	static void FillRoles(std::unordered_set<UINT64>& roleIDs, const KKSG::KMatchUnit& kunit);

	void FillServers(std::unordered_set<UINT32>& serverIDs);
	static void FillServers(std::unordered_set<UINT32>& serverIDs, const KKSG::KMatchUnit& kunit);

	void FillMatchKey(KMatchKey& key);
	static void FillMatchKey(KMatchKey& key, const KKSG::KMatchUnit& kunit);

	void InfoPrint();

protected:
	KMatchID m_matchID;
	int m_type;
	KKSG::KMatchUnit m_kunit;//不要改变里面的数据
	UINT32 m_startTime;
};

class MatchMgrBase;

class MatchUnitMgrBase
{
public:
	MatchUnitMgrBase(int type, MatchMgrBase* pMgr);
	virtual ~MatchUnitMgrBase();
	
	inline int GetType() const { return m_type; }

	//添加和删除都通过外部调用
	virtual void AddMatchUnit(MatchUnit* pMatchUnit) = 0;
	virtual void DelMatchUnit(MatchUnit* pMatchUnit) = 0;

	virtual void Update() = 0;

protected:
	int m_type;
	MatchMgrBase* m_pMgr;
};

#endif
