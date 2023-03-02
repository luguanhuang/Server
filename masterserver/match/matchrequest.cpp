#include "pch.h"
#include "matchrequest.h"
#include "other/matchhandler.h"
#include "team/teammgr.h"
#include "role/role.h"
#include "match/rpcm2g_kmatchcommonreqm2c.h"
#include "role/rolemanager.h"
#include "network/teamlink.h"
#include "team/teamtranshandler.h"
#include "team/teamrequest.h"
#include "team/team.h"
#include "report/reportmgr.h"
#include "util/gametime.h"

MatchRequest::MatchRequest(CRole* pRole)
	:m_pRole(pRole)
{

}

int MatchRequest::HandleReq(const KKSG::KMatchCommonArg& roArg, const KKSG::KMatchCommonResM2G& roRes)
{
	KKSG::KMatchCommonResM2G tmpRes = roRes;
	
	if(roArg.op() == KKSG::KMATCH_OP_START)
	{
		int ret = CheckMatchCond(roArg, tmpRes);
		TJUDGERET(ret);
	}
	HMatchHandler handler;
	handler.Data().matchArg = roArg;
	handler.Data().matchData = tmpRes;
	if(roArg.op() == KKSG::KMATCH_OP_START)
	{
		if(roArg.isteam())
		{
			return handler.Start(TeamMgr::Instance()->FindTeam(m_pRole->GetTeamID()), roArg.type());
		}
		else
		{
			return handler.Start(m_pRole, roArg.type());
		}
	}
	else if(roArg.op() == KKSG::KMATCH_OP_STOP)
	{
		if(roArg.isteam())
		{
			handler.Stop(TeamMgr::Instance()->FindTeam(m_pRole->GetTeamID()), roArg.type());
		}
		else
		{
			handler.Stop(m_pRole, roArg.type());
		}
	}
	return KKSG::ERR_SUCCESS;
}

void MatchRequest::HandleC2MArg(UINT32 delayID, const KKSG::KMatchCommonArg& roArg)
{
	//统一发到GS
	RpcM2G_KMatchCommonReqM2C* rpc = RpcM2G_KMatchCommonReqM2C::CreateRpc();
	auto& data = rpc->m_oArg;
	data.set_delayid(delayID);
	data.set_roleid(m_pRole->GetID());
	*data.mutable_c2marg() = roArg;
	m_pRole->SendMsgToGS(*rpc);
}

void MatchRequest::HandleM2GRes(const KKSG::KMatchCommonArgM2G& roArg, const KKSG::KMatchCommonResM2G& roRes)
{
	const KKSG::KMatchCommonArg& c2mArg = roArg.c2marg();
	KKSG::KMatchCommonRes tmpRes = roRes.c2mres();
	
	CUserData userData;
	userData.m_pUserPtr = (void*)(&tmpRes);

	if(tmpRes.errorcode() != KKSG::ERR_SUCCESS)
	{
		CRpc::ReplyDelayRpc(roArg.delayid(), userData);		
		return;
	}

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		SSWarn << " find role failed roleid = " << roArg.roleid() << END;
		tmpRes.set_errorcode(KKSG::ERR_FAILED);		
		CRpc::ReplyDelayRpc(roArg.delayid(), userData);		

		return;
	}

	MatchRequest req(pRole);

	if(req.CheckReportLimit(c2mArg, tmpRes))
	{
		CRpc::ReplyDelayRpc(roArg.delayid(), userData);		
		return;
	}
	
	int ret = req.HandleReq(c2mArg, roRes);
	if(ErrorCode_IsValid(ret))
	{
		tmpRes.set_errorcode((ErrorCode)(ret));
	}
	CRpc::ReplyDelayRpc(roArg.delayid(), userData);		
}

bool MatchRequest::CheckReportLimit(const KKSG::KMatchCommonArg& roArg, KKSG::KMatchCommonRes& roRes)
{
	if(roArg.op() == KKSG::KMATCH_OP_STOP)//停止不判
	{
		return false;
	}

	UINT32 sceneType = 0;
	switch(roArg.type())
	{
	case KKSG::KMT_HERO:
		{
			sceneType = KKSG::SCENE_HEROBATTLE;
		}
		break;
	case KKSG::KMT_MOBA:
		{
			sceneType = KKSG::SCENE_MOBA;
		}
		break;
	default:
		break;
	}

	if(0 == sceneType)
	{
		SSInfo << " have not handle type = " << roArg.type() << END;
		return false;
	}

	std::vector<UINT64> roleIDs;
	if(roArg.isteam())
	{
		Team* pTeam = TeamMgr::Instance()->FindTeam(m_pRole->GetTeamID());
		if(pTeam)
		{
			pTeam->GetRoleIDs(roleIDs);
		}
		else
		{
			SSError << " find team failed role = " << m_pRole->GetID() << " team = " << m_pRole->GetTeamID() << END;
		}
	}
	else
	{
		roleIDs.push_back(m_pRole->GetID());
	}

	for(size_t i = 0; i < roleIDs.size(); ++i)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleIDs[i]);
		if(pRole)
		{
			UINT32 leftTime = ReportMgr::Instance()->GetRoleBanTime(roleIDs[i], sceneType);
			if(leftTime > 0)
			{
				roRes.set_errorcode(KKSG::ERR_REPORT_FORBID);
				roRes.set_problem_name(pRole->GetName());
				UINT32 endTime = UINT32(GameTime::GetTime()) + leftTime; 
				roRes.set_endtime(endTime);
				return true;
			}
		}
	}

	return false;
}

int MatchRequest::CheckMatchCond(const KKSG::KMatchCommonArg& roArg, KKSG::KMatchCommonResM2G& tmpRes)
{
	std::vector<UINT64> roleIDs;
	if(roArg.isteam())
	{
		TeamRequest req(m_pRole);
		int ret = req.TeamMatch(roArg);
		TJUDGERET(ret);

		Team* pTeam = TeamMgr::Instance()->FindTeam(m_pRole->GetTeamID());
		if(NULL == pTeam)
		{
			SSError << " find team failed role = " << m_pRole->GetID() << " team = " << m_pRole->GetTeamID() << END;
			return KKSG::ERR_FAILED;
		}
		pTeam->GetRoleIDs(roleIDs);
	}
	else
	{
		roleIDs.push_back(m_pRole->GetID());
	}
	for(size_t i = 0; i < roleIDs.size(); ++i)
	{
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleIDs[i]);
		if(NULL == pRole)
		{
			SSError << " find role failed roleid = " << roleIDs[i] << END;
			return KKSG::ERR_FAILED;
		}
		KKSG::KMatchRole* krole = NULL;
		for(int j = 0; j < tmpRes.roledata_size(); ++j)
		{
			if(tmpRes.roledata(j).roleid() == pRole->GetID())
			{
				krole = tmpRes.mutable_roledata(j);
				break;
			}
		}
		if(NULL == krole)//必须找到
		{
			SSInfo << " find gs role matchdata failed roleid = " << pRole->GetID() << END;
			return KKSG::ERR_PVP_ROLE_INBATTLE;//有可能玩家没有离开战斗场景
		}
		HMatchHandler handler;
		KKSG::ErrorCode ret = handler.RoleCondition(pRole, roArg.type(), krole);
		TJUDGERET(ret);
	}
	return KKSG::ERR_SUCCESS;
}

