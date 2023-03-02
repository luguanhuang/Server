#ifndef __TEAMMEMBER_H__
#define __TEAMMEMBER_H__

#include "table/ExpeditionTable.h"
#include "role/rolesummarymgr.h"
#include "team/teammemberbase.h"

namespace KKSG 
{
	class TeamMember; 
	class RoleSummaryStored;
}

class CRoleSummary;
class CRole;

class CTeamMember : public TeamMemberBase
{
public:
	CTeamMember(UINT64 roleID);
	~CTeamMember();

	inline UINT64 RoleID() { return m_qwRoleID; }

	bool IsRobot(){ return (NULL != m_pRobotSummary);}
	void GenerateRoleSummry(const KKSG::RoleSummaryStored& data);
	CRoleSummary* GetRobotSummry() { return m_pRobotSummary; }

	inline bool IsHelper() { return (KKSG::TMT_HELPER == m_type); }
	void SetIsHelper(bool helper);

	inline int UseTicket() { return (KKSG::TMT_USETICKET == m_type)?1:0; }
	void SetUsetTicket(int useTicket);

	void SetMemType(KKSG::TeamMemberType type) { m_type = type; }
	KKSG::TeamMemberType GetMemType() { return m_type; }

	UINT32 LeftCount() { return m_extraData.leftcount(); }

	void SaveToTeamMember(KKSG::TeamMember *pMemberData);
	void InfoPrint();

	UINT64 m_qwRoleID;
	int    MemberState;

	CRoleSummary *m_pRobotSummary;

private:
};

#endif // __TEAMMEMBER_H__