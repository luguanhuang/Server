#ifndef __TEAM_H__
#define __TEAM_H__

#include "define/teamdef.h"
#include "math/XVector3.h"
#include "scene/scene.h"
#include "table/expeditionconfigmgr.h"

#include "teammember.h"

class CProtocol;

typedef void (*TeamCopyCB)(void *arg);
typedef void (*TeamCopyOnLineCB)(Role *pRole, void *arg);

namespace KKSG
{
	class TeamSynAll;
}

class TeamCopy
{
public:
	TeamCopy();
	~TeamCopy();

	int GetTeamID(){ return m_teamID; }
	ExpeditionTable::RowData* GetConf();
	UINT64 GetLeaderID(){ return m_leaderID; }
	UINT64 GetGuildID(){ return m_guildID; }
	int GetExpID(){ return m_expID; }

	void LoadT(const KKSG::TeamSynAll& data);

	UINT32 GetMemberCount(){ return m_members.size(); }

	bool HaveRole(UINT64 roleID);

	void EnterTakeCost();
	void EnterTakeCost(UINT64 roleID);

	void BroadCast(CProtocol &proto, UINT64 qwExludeRole = 0);

	void   GetOnlineRoles(std::vector<Role *> &roles);
	inline const std::vector<TeamCopyMember>& GetMember() { return m_members; }

	UINT32 GetPPTLimit() { return m_pptlimit; }
	bool IsPPTLimit(UINT32 roleppt) { return roleppt < m_pptlimit; }
	bool IsTeamCost() { return TeamCostInvalidID != m_teamcost; }
	bool HasPassword() { return m_haspassward; }

	KKSG::TeamMemberType GetMemberType(UINT64 roleID);
	void FilleRoleIDs(std::vector<UINT64>& roleIDs, bool excludeRobot);

	void InfoPrint();
private:

	//如果是TeamCopyMgr里面的数据 则并未全部实时同步
	int m_teamID;
	int m_expID;
	bool m_haspassward;
	UINT64 m_leaderID;
	UINT64 m_guildID;
	UINT32 m_pptlimit;
	UINT32 m_teamcost;
	std::vector<TeamCopyMember> m_members;
};

class TeamCostCtrl
{
public:
	void CheckAddTeamDayCount(Role *pRole, ExpeditionTable::RowData* pConf, TeamCostCountType cctype);
	void EnterTakeItem(Role *pRole, std::vector<Sequence<int, 2>>& pVec);
	void TakeUseTicket(Role *pRole, ExpeditionTable::RowData* pConf);
};

#endif // __TEAM_H__