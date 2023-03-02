#include "pch.h"
#include "teamforceop.h"
#include "team.h"
#include "role/role.h"
#include "teamrequest.h"
#include "teammgr.h"
#include "teamtranshandler.h"

Team* TeamForceOp::CreateTeam(const std::vector<CRole*>& roles, int expid, TeamOpType optype)
{
	for(size_t i = 0; i < roles.size(); ++i)
	{
		if(NULL == roles[i] || roles[i]->GetTeamID())
		{
			return NULL;
		}
	}

	if(roles.empty())
	{
		return NULL;
	}

	//利用第一个玩家创建队伍
	TeamRequest Req(roles[0]);
	int ret = Req.CreateExpenditionTeam(expid, optype, "");
	if(KKSG::ERR_SUCCESS != ret)
	{
		SSError << " create team failed roleid = " << roles[0]->GetID() << " expid = " << expid << " ret = " << ret << END;
		return NULL;
	}

	Team* pTeam = TeamMgr::Instance()->FindTeam(roles[0]->GetTeamID());
	if(NULL == pTeam)
	{
		return NULL;
	}

	std::vector<CRole*> tmp = roles;
	tmp.erase(tmp.begin());
	JoinTeam(pTeam->GetID(), tmp, optype);

	return pTeam;
}

void TeamForceOp::JoinTeam(int teamid, const std::vector<CRole*>& roles, TeamOpType optype)
{
	Team* pTeam = TeamMgr::Instance()->FindTeam(teamid);
	if(NULL == pTeam)
	{
		SSError << " not find teamid = " << teamid << " op = " << optype << END;
		return ;
	}

	for(size_t i = 0; i < roles.size(); ++i)
	{
		if(NULL == roles[i] || roles[i]->GetTeamID())
		{
			continue;
		}
		TeamRequest Tq(roles[i]);
		int flag = Tq.JoinTeam(teamid, optype, "", false);
		if(flag != KKSG::ERR_SUCCESS)
		{
			SSError << " join team fail teamid = " << teamid << " roleid = " << roles[i]->GetID() << " op = " << optype << " ret = " << flag << END;
		}
	}
}

void TeamForceOp::LeaveTeam(const std::vector<CRole*>& roles, int reason/* = 0*/)
{
	for(size_t i = 0; i < roles.size(); ++i)
	{
		if(NULL == roles[i])
		{
			continue;
		}
		LeaveTeam(roles[i], reason);
	}
}

void TeamForceOp::LeaveTeam(CRole* pRole, int reason/* = 0*/)
{
	if(NULL == pRole)
	{
		return;
	}
	if(pRole->HasTeamInTs())
	{
		LeaveTsTeam(pRole);
	}
	else if(pRole->HasTeamInMs())
	{
		TeamRequest Tq(pRole);
		Tq.LeaveTeam(reason);
	}
}

void TeamForceOp::SetInBattleState(int teamid)
{
	Team* pTeam = TeamMgr::Instance()->FindTeam(teamid);
	if(pTeam)
	{
		pTeam->SetInBattleState();
	}
}

void TeamForceOp::LeaveTsTeam(CRole* pRole)
{
	if(NULL == pRole)
	{
		return;
	}
	//模拟请求离队	
	KKSG::TeamOPArg roArg;
	roArg.set_request(KKSG::TEAM_LEAVE);
	TeamTransHandler trans;
	trans.HandleRoleOpReqFromC(0, pRole, roArg);
}
