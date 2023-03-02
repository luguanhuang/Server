#ifndef __TEAMFORCEOP_H__
#define __TEAMFORCEOP_H__

#include "define/teamdef.h"

class CRole;
class Team;
class TeamForceOp
{
public:
	//////////////////////////////////////////////////
	// 创建一个新的队伍  要求roles没有在队伍中
	// 成功返回队伍指针 失败返回空指针
	/////////////////////////////////////////////////
	Team* CreateTeam(const std::vector<CRole*>& roles, int expid, TeamOpType optype = TEAM_OPT_FORCE);

	//////////////////////////////////////////////////
	// 加入已有队伍  要求roles没有在队伍中
	/////////////////////////////////////////////////
	void JoinTeam(int teamid, const std::vector<CRole*>& roles, TeamOpType optype = TEAM_OPT_FORCE);

	//////////////////////////////////////////////////
	// 离开已有队伍 
	// 支持离开在teamserver的队伍 但是是异步操作
	// reason = 0 -> KKSG::LTT_BY_SELF
	/////////////////////////////////////////////////
	void LeaveTeam(const std::vector<CRole*>& roles, int reason = 0);
	void LeaveTeam(CRole* pRole, int reason = 0);

	//////////////////////////////////////////////////
	// 利用组队过程和信息 但未走组队去战斗的流程 走了自己去战斗的流程  
	// 需要在去战斗场景的时候 把队伍状态置成战斗状态
	/////////////////////////////////////////////////
	void SetInBattleState(int teamid);

private:
	void LeaveTsTeam(CRole* pRole);
};

#endif