#ifndef __TEAMMGR_H__
#define __TEAMMGR_H__

namespace KKSG
{
	class TeamSynPara;
	class TeamSynAll;
	class TeamSynRes;
}

class TeamCopy;

class TeamCopyMgr 
{
	TeamCopyMgr();
	~TeamCopyMgr();
	DECLARE_SINGLETON(TeamCopyMgr);

public:

	bool Init();
	void Uninit();

	TeamCopy* GetTeamByRoleID(UINT64 roleID);
	int GetTeamIDByRoleID(UINT64 roleID);

	void SynFromMS(const KKSG::TeamSynPara& data, KKSG::TeamSynRes& roRes);

	void OnMsClose();

	void InfoPrint();

private:
	void ClearData();

	void SynAddOrUpdateTeam(const KKSG::TeamSynAll& data);
	void SynAddTeam(const KKSG::TeamSynAll& data);
	void SynUpdateTeam(const KKSG::TeamSynAll& data);
	void SynDestroy(int teamID);
	void SynGetExtraData(const KKSG::TeamSynPara& roArg, KKSG::TeamSynRes& roRes);
	//int SynStartBattle(int teamID);
	void SynResetCost(UINT64 leaderid);
	void SynTsDisconnected();

	void UpdateRole2Team(TeamCopy* pTeam, bool add);

	std::unordered_map<int,	TeamCopy*> m_teams;
	std::unordered_map<UINT64, int> m_role2team;//包括机器人 其他服玩家
};

#endif // __TEAMMGR_H__
