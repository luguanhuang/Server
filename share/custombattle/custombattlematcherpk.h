#ifndef __CUSTOMBATTLEMATCHERPK_H__
#define __CUSTOMBATTLEMATCHERPK_H__

#include "custombattledef.h"
#include "match/matchlimit.h"

struct CustomBattlePkInfo
{
	CustomBattlePkInfo() { }

	inline UINT64 RoleID() const { return m_matchinfo.roleid(); }
	inline UINT32 Point() const { return m_matchinfo.point(); }
	inline UINT32 ServerID() { return m_matchinfo.serverid(); }

	bool IsFitSpan(UINT32 temppoint);
	bool CheckTimeout();
	bool IsEmpty();

	void ChangeDataByTimeDiff(UINT32 msTime, UINT32 wsTime);

	bool operator < (const CustomBattlePkInfo& other) const
	{
		if(Point() < other.Point())
		{
			return true;
		}
		else if(Point() == other.Point() && RoleID() < other.RoleID())
		{
			return true;
		}
		return false;
	}

	const KKSG::PkMatchStage* GetBack();
	void PopBack();

	KKSG::CustomBattleMatchRoleInfo m_matchinfo;
};

class CustomBattleMatcherMgr;
class CustomBattleMatcherPk
{
public:
	CustomBattleMatcherPk();
	CustomBattleMatcherPk(CustomBattleMatcherMgr* matcher, UINT64 battleuid, bool isfair);
	~CustomBattleMatcherPk();

	bool Upate(UINT32& limit);
	bool CheckMatch(UINT32& limit);
	void CheckTimeout();

	bool IsMatch(CustomBattlePkInfo& info11, CustomBattlePkInfo& info22);
	bool MatchPair(CustomBattlePkInfo& info11, CustomBattlePkInfo& info22);

	bool Add(const KKSG::CustomBattleMatchRoleInfo& matchinfo, UINT32 msTime);
	void Remove(UINT64 roleID);
	bool IsIn(UINT64 roleID);

	void SetOfficial(bool isofficial) { m_isofficial = isofficial; }

private:
	bool m_isofficial;
	MatchLimit m_limit;
	CustomBattleMatcherMgr* m_matcher;
	CustomBattleMatchSceneInfo m_sceneinfo;
	std::unordered_map<UINT64, CustomBattlePkInfo> m_allInfo;
};

#endif