#ifndef __TEAMMGR_H__
#define __TEAMMGR_H__

#include "cmdline.h"

namespace KKSG
{
	class TeamSynRes;
	class CrashRecoverM2GRes;
}

class Team;
class CRole;

class TeamMgr : public ITimer
{
	TeamMgr();
	~TeamMgr();
	DECLARE_SINGLETON(TeamMgr)

public:

	bool Init();
	void Uninit();

	Team *CreateTeam(UINT32 expeditionID);
	Team *FindTeam(int teamID);
	void BeforeDestory(Team *pTeam);
	void DestoryTeam(Team *pTeam);

	std::unordered_map<int, Team *>& AllTeams() { return m_Teams; }

	std::list<int> GetTeams(const std::vector<UINT32> &Category);
	std::list<int> GetTeams(UINT32 expeditionID);

	void OnRoleLeaveBattle(CRole* pRole);
	void OnRoleEnterBattle(CRole* pRole);
	void OnRoleEnterScene(CRole *pRole);
	void OnRoleLogout(CRole* pRole);

	virtual void OnTimer(UINT32 dwID, UINT32 dwCount);

	void SynAllData2GS(Team* pTeam);
	void SynDel2GS(int teamID);
	void SynGetExtraData(Team* pTeam, UINT64 roleid = 0);
	void SynGetExtraDataBack(int teamid, const KKSG::TeamSynRes& roRes);
	void SynToGSCommon(const KKSG::TeamSynPara& para, UINT32 gsLine = 0);

	void OnGsRegister(UINT32 gsLine);
	void OnMsCrashRecover(UINT32 gsLine, const KKSG::CrashRecoverM2GRes &roRes);
	void OnTsDisconnected();

	void TestPrint();

private:

	void UpdateAllTeam();

	std::unordered_map<int, Team *> m_Teams;
	HTIMER m_Handler;
};


#endif