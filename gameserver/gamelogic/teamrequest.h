#ifndef __TEAMREQUEST_H__
#define __TEAMREQUEST_H__

#include "table/ExpeditionTable.h"
#include "gamelogic/team.h"

class TeamCopy;
class Role;

namespace KKSG
{
	class TeamOPRes;
	class TeamOPM2GRes;
	class TeamHelper;
	class BuyTeamSceneCountRet;
}

//ֻ���ж�����
class TeamRequest
{
public:
	TeamRequest(Role *pRole, TeamCopy* pTeam = NULL);
	~TeamRequest();

	int  CreateExpenditionTeam(UINT32 expID);
	int  JoinTeam();
	int  Invite(UINT64 roleID);
	//ƥ��
	int  StartMatch(int expid);
	//��ս
	int	 SwitchHelper(int expID);
	int SwitchUseTicket();

	int  StartBattleBeginVote(KKSG::TeamOPRes &roRes);
	int  BattleContinue(KKSG::TeamOPM2GRes &m2gRes);
	int  ChangeExpID(int expID, KKSG::TeamOPRes& roRes, KKSG::TeamHelper& nothelpers);

    int GetTeamTypeCount(KKSG::TeamOPRes& roRes);

	void BuyTeamCount(int type, KKSG::BuyTeamSceneCountRet& roRes);

	int GetDayCountLeft(Role *pRole, int Type);

	//����ĳ���ؿ�
	int CheckOpenCondtion(Role *pRole, int expid);
	//�������
	int CheckJoinCondtion(Role *pRole);
	//����ս��
	int CheckEnterCondtion(Role *pRole, int expid, TeamEnterData* pEnterData = &TeamEnterData::GlobalTeamEnterData);

	int CheckStartBattleCondition(UINT64& problemRoleID);

	///> �ͽ���
	int HandleTeamCost(UINT32 index);

private:
	int CheckChangeExpCondition(int expID, UINT64& problemRoleID);

private:
	TeamCopy m_teaminfo;
	Role *m_pRole;
};

#endif // __TEAMREQUEST_H__
