#include "pch.h"
#include "team/rpch2m_teamtrants2ms.h"
#include "team/teamcommon.h"
#include "team/teamtranshandler.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "define/teamdef.h"
#include "team/teammgr.h"
#include "team/teamrequest.h"
#include "team/teamforceop.h"
#include "pay/paymgr.h"

// generate by ProtoGen at date: 2017/4/25 21:36:22

RPC_SERVER_IMPLEMETION(RpcH2M_TeamTranTs2Ms, TeamTransData, TeamTransRes)

void RpcH2M_TeamTranTs2Ms::OnCall(const TeamTransData &roArg, TeamTransRes &roRes)
{
	TeamTransHandler trans;

	switch(roArg.type())
	{
	case TEAM_TRANS_OP_REQ:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
			if(pRole)
			{
				if(roArg.c2moparg().request() == KKSG::TEAM_COSTTYPE && roArg.diamondcount() > 0)//钻石赏金团
				{
                    TeamRequest req(pRole);
                    auto ret = req.DiamondCostCond();
                    if(KKSG::ERR_SUCCESS == ret)
                    {
					    CPayMgr::Instance()->TeamCost(pRole, DelayRpc(), (int)roArg.diamondcount(), roArg);
                    }
                    else
                    {
				        roRes.mutable_c2mopres()->set_result(ret);
                    }
				}
				else
				{
					trans.HandleRoleOpReqFromTs(DelayRpc(), pRole, roArg);
				}
			}
			else
			{
				SSWarn << " find role failed role = " << roArg.roleid() << END;
				roRes.mutable_c2mopres()->set_result(ERR_FAILED);
			}
		}
		break;
	case TEAM_TRANS_TEAM_MATCH:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
			if(pRole)
			{
				trans.HandleMatchReqFromTs(DelayRpc(), pRole, roArg);
			}
			else
			{
				SSWarn << " find role failed role = " << roArg.roleid() << END;
				roRes.mutable_matchres()->set_errorcode(ERR_FAILED);
			}
		}
		break;
	case TEAM_TRANS_ROLE_MATCH:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
			if(pRole)
			{
				trans.HandleRoleMatchFromTs(DelayRpc(), pRole, roArg);
			}
			else
			{
				SSWarn << " find role failed role = " << roArg.roleid() << END;
				roRes.mutable_c2mopres()->set_result(ERR_FAILED);
			}
		}
		break;
	case TEAM_TRANS_TEAM_CHANGEDATA:
		{
			trans.HandleTeamChangeFromTs(roArg.roleid(), roArg.changedata());			
		}
		break;
	case TEAM_TRANS_ROLELEAVE:
		{
			trans.HandleLeaveTeamFromTs(roArg.roleid(), roArg.leavereason());
		}
		break;
	case TEAM_TRANS_ROLE2TEAM:
		{
			//if(roArg.teamid())//for test
			//{
			//	std::vector<CRole*> vec;
			//	vec.push_back(CRoleManager::Instance()->GetByRoleID(roArg.roleid()));
			//	TeamForceOp op;
			//	op.CreateTeam(vec, 1015);
			//}
			trans.HandleRole2TeamFromTs(roArg.roleid(), roArg.teamid());
		}
		break;
	case TEAM_TRANS_INVITEDATA:
		{
			trans.HandleInviteDataFromTs(roArg);
		}
		break;
	case TEAM_TRANS_TEAMFULLDATA:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
			if(pRole)
			{
				trans.HandleTeamFullData(pRole, roArg.teamfulldata());
			}
		}
		break;
	case TEAM_TRANS_TEAMSYN2GS:
		{
			TeamMgr::Instance()->SynToGSCommon(roArg.synpara(), roArg.gsline());
		}
		break;
	case TEAM_TRANS_INV_REFUSE:
		{
			trans.HandleInvRefuseFromTs(roArg.account(), roArg.roleid());
		}
		break;
	case TEAM_TRANS_CHAT:
		{
			trans.HandleChatFromTs(roArg.roleid(), roArg.chatinfo());
		}
		break;
	case TEAM_TRANS_CHECK_EXP_CHANGE:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(roArg.roleid());
			if(pRole)
			{
				trans.HandleTransArgFromTsToGs(DelayRpc(), pRole, roArg);
			}
			else
			{
				roRes.set_expid(0);
			}
		}
		break;
	default:
		break;
	}
}

void RpcH2M_TeamTranTs2Ms::OnDelayReplyRpc(const TeamTransData &roArg, TeamTransRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		return;
	}
	switch(roArg.type())
	{
	case TEAM_TRANS_OP_REQ:
	case TEAM_TRANS_TEAM_MATCH:
	case TEAM_TRANS_ROLE_MATCH:
	case TEAM_TRANS_CHECK_EXP_CHANGE:
		{
			KKSG::TeamTransRes* tmpRes = (KKSG::TeamTransRes*)(roUserData.m_pUserPtr);
			roRes = *tmpRes;
		}
		break;
	default:
		break;
	}
}
