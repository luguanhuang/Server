#ifndef __TEAMFORCEOP_H__
#define __TEAMFORCEOP_H__

#include "define/teamdef.h"

class CRole;
class Team;
class TeamForceOp
{
public:
	//////////////////////////////////////////////////
	// ����һ���µĶ���  Ҫ��rolesû���ڶ�����
	// �ɹ����ض���ָ�� ʧ�ܷ��ؿ�ָ��
	/////////////////////////////////////////////////
	Team* CreateTeam(const std::vector<CRole*>& roles, int expid, TeamOpType optype = TEAM_OPT_FORCE);

	//////////////////////////////////////////////////
	// �������ж���  Ҫ��rolesû���ڶ�����
	/////////////////////////////////////////////////
	void JoinTeam(int teamid, const std::vector<CRole*>& roles, TeamOpType optype = TEAM_OPT_FORCE);

	//////////////////////////////////////////////////
	// �뿪���ж��� 
	// ֧���뿪��teamserver�Ķ��� �������첽����
	// reason = 0 -> KKSG::LTT_BY_SELF
	/////////////////////////////////////////////////
	void LeaveTeam(const std::vector<CRole*>& roles, int reason = 0);
	void LeaveTeam(CRole* pRole, int reason = 0);

	//////////////////////////////////////////////////
	// ������ӹ��̺���Ϣ ��δ�����ȥս�������� �����Լ�ȥս��������  
	// ��Ҫ��ȥս��������ʱ�� �Ѷ���״̬�ó�ս��״̬
	/////////////////////////////////////////////////
	void SetInBattleState(int teamid);

private:
	void LeaveTsTeam(CRole* pRole);
};

#endif