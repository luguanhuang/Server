#ifndef __ROLESUMMARY_H__
#define __ROLESUMMARY_H__

#include "rolesummary/rolesummarybase.h"

class CRoleSummary : public RoleSummaryBase
{
public:
	CRoleSummary();
	~CRoleSummary();

	virtual void Build(KKSG::UnitAppearance* appearance, int mask);

	void SetOutLook(KKSG::OutLook* outlook);
	void SaveToTeamMember(KKSG::TeamMember *pMemberData);

	bool IsPushTypeForbid(UINT32 type);

	void FillSmallInfo(KKSG::RoleSmallInfo& data);
	void FillRoleOutLookBrief(KKSG::RoleOutLookBrief& data);

	void SetLastLogout(UINT32 _time);
	UINT32 GetLastLogout();

	inline UINT32 LogoutTime() { return logouttime; }

	inline UINT32 GetLastUpdateTime() { return lastupdatetime; }
	inline void SetLastUpdateTime(UINT32 time) { lastupdatetime = time; }

	inline void SetBrief(bool isbrief) { m_isbrief = isbrief; }
	inline bool IsBrief() { return m_isbrief; }

	void FillRolePvpInfo(KKSG::PvpRoleInfo& info);
	void FillWeddingRoleInfo(KKSG::WeddingRoleBrief& info);
	void FillDailyTaskRefreshRoleInfo(KKSG::DailyTaskRefreshRoleInfo& info);


private:
	bool m_isbrief;
	UINT32 logouttime;
	UINT32 lastupdatetime;
};

#endif