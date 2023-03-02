#include "pch.h"
#include "team/rpcc2m_teamrequestc2m.h"
#include "team/rpcm2g_teamrequestm2g.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "team/teammgr.h"
#include "team/team.h"
#include "network/gslink.h"
#include "team/teamrequest.h"
#include "team/teammember.h"
#include "partner/partnermgr.h"
#include "pay/paymgr.h"
#include "team/teammatchmgr.h"
#include "team/teamtranshandler.h"
#include "network/teamlink.h"

// generate by ProtoGen at date: 2016/8/18 17:18:40

RPC_SERVER_IMPLEMETION(RpcC2M_TeamRequestC2M, TeamOPArg, TeamOPRes)

void RpcC2M_TeamRequestC2M::OnCall(const TeamOPArg &roArg, TeamOPRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	TeamTransHandler trans;
    KKSG::ErrorCode err = trans.HandleRoleOpCond(roArg, pRole);
    if(KKSG::ERR_SUCCESS != err)
    {
        roRes.set_result(err);
        return;
    }
	if(trans.NeedHandleToTs(roArg, pRole))//去组队服务器
	{
		if(TeamLink::Instance()->IsConnected())
		{
			trans.HandleRoleOpReqFromC(DelayRpc(), pRole, roArg);
		}
		else
		{
			roRes.set_result(ERR_SERVER_UNNORMAL);
		}
		return;
	}

	// ms handle 
	int ret = ERR_SUCCESS;
	bool gotogs = true;
	UINT32 DiamondCostCount = 0;
	TeamRequest tq(pRole);
	switch(roArg.request())
	{
	case KKSG::TEAM_GET_FULL_DATA:
		{
			tq.QueryFullData();
			ret = KKSG::ERR_SUCCESS;
			gotogs = false;
		}
		break;
	case KKSG::TEAM_LEAVE:
		{
			ret = tq.LeaveTeam(KKSG::LTT_BY_SELF);
			gotogs = false;
		}
		break;
	case KKSG::TEAM_KICK:
		{
			ret = tq.KickTeam(roArg.roleid());	
			gotogs = false;
		}
		break;
	case KKSG::TEAM_INVITE:
		{
			ret = tq.InvitePre(roArg.roleid(), roArg.account());			
		}
		break;
	case KKSG::TEAM_STOP_MATCH:
		{
			ret = tq.StopMatch();
			gotogs = false;
		}
		break;
	case KKSG::TEAM_START_BATTLE_DISAGREE:
		{
			ret = tq.StartBattleVote(pRole->GetID(), false);
			gotogs = false;
		}
		break;
	case KKSG::TEAM_PPTLIMIT:
		{
			ret = tq.SetTeamPPTLimit(roArg.has_extrainfo() ? roArg.extrainfo().pptlimit() : 0);
			gotogs = false;
		}
		break;
	case KKSG::TEAM_COSTTYPE:
		{
			if(roArg.has_extrainfo())
			{
				ret = tq.SetTeamCostPre(roArg.extrainfo().costindex(), DiamondCostCount);
			}
			else
			{
				ret = ERR_FAILED;
			}
		}
		break;
	case KKSG::TEAM_START_BATTLE_AGREE:
		{
			UINT32 teamId = pRole->GetTeamID();
			Team* pTeam = TeamMgr::Instance()->FindTeam(teamId);
			if (pTeam && pTeam->GetConf() && pTeam->GetConf()->Type == PARTNER_TEAM_SCENE)
			{
				const std::list<CTeamMember*>& members = pTeam->GetMember();
				std::vector<UINT64> roleIds;
				for (auto it = members.begin(); it != members.end(); ++it)
				{
					CTeamMember* m = *it;
					if (m) roleIds.push_back(m->m_qwRoleID);
				}
				ret = PartnerMgr::Instance()->CheckMakePartner(pRole->GetID(), roleIds);
			}
		}
		break;
	case KKSG::TEAM_CHANGE_PASSWORD:
		{
			ret = tq.ChangePassword(roArg.password());
			gotogs = false;
		}
		break;
	case KKSG::TEAM_TRAHS_LEADER:
		{
			ret = tq.TransLeader(roArg.roleid());
			gotogs = false;
		}
		break;
	case KKSG::TEAM_CHANGE_EPXTEAMID:
		{
			ret = tq.ChangeExpIDPre(roArg.expid());
		}
		break;
	default:
		break;
	}

	if(ERR_SUCCESS != ret || false == gotogs)
	{
		roRes.set_result(ErrorCode(ret));
		return;
	}

	if(DiamondCostCount > 0)
	{
		CPayMgr::Instance()->TeamCost(pRole, DelayRpc(), DiamondCostCount, roArg);
	}
	else
	{
		tq.TransSendToGs(DelayRpc(), roArg);
	}

	//// gs handle 
	//RpcM2G_TeamRequestM2G *rpc = RpcM2G_TeamRequestM2G::CreateRpc();

	//rpc->m_oArg.set_roleid(pRole->GetID());
	//UINT32 delayid = DelayRpc();
	//rpc->m_oArg.set_delayid(delayid);
	//rpc->m_oArg.mutable_c2marg()->CopyFrom(roArg);

	//// fill ms team info for gs 
	//Team* pTeam = NULL;
	//if(pRole->GetTeamID())
	//{
	//	pTeam = TeamMgr::Instance()->FindTeam(pRole->GetTeamID());
	//}
	//else 
	//{
	//	if (0 != m_oArg.teamid())
	//	{
	//		pTeam = TeamMgr::Instance()->FindTeam(m_oArg.teamid());
	//	}
	//}
	//if(pTeam)
	//{
	//	pTeam->SaveSynData(*rpc->m_oArg.mutable_dataall());
	//}

	//if(TEAM_QUERYCOUNT == roArg.request())//剩余次数在任何gamesever都可以请求
	//{
	//	pRole->SendMsgToGS(*rpc);
	//}
	//else
	//{
	//	GSLink::Instance()->SendToLine(MAIN_HALL_GS_LINE, *rpc);
	//}

}

void RpcC2M_TeamRequestC2M::OnDelayReplyRpc(const TeamOPArg &roArg, TeamOPRes &roRes, const CUserData &roUserData)
{
	if(NULL == roUserData.m_pUserPtr)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return ;
	}

	TeamOPM2GRes* m2gRes = (TeamOPM2GRes*)(roUserData.m_pUserPtr);
	roRes.CopyFrom(m2gRes->c2mres());

	if(1 == roUserData.m_dwUserData)//跨服组队
	{
		return;
	}

	if(KKSG::ERR_SUCCESS != roRes.result())
	{
		return ;
	}

	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return ;
	}

	int ret = KKSG::ERR_SUCCESS;
	TeamRequest tq(pRole);
	//里面不要出现 return
	switch(roArg.request())
	{
	case KKSG::TEAM_CREATE:
		{
			ret = tq.CreateExpenditionTeam(roArg.expid(), TEAM_OPT_NORMAL, roArg.password());
		}
		break;
	case KKSG::TEAM_JOIN:
		{
			ret = tq.JoinTeam(roArg.teamid(), TEAM_OPT_NORMAL, roArg.password(), true);	
		}
		break;
	case KKSG::TEAM_START_BATTLE:
		{
			ret = tq.StartBattleBeginVote(roArg, roRes);
		}
		break;
	case KKSG::TEAM_START_BATTLE_AGREE:
		{
			ret = tq.StartBattleVote(pRole->GetID(), true);
		}
		break;
	case KKSG::TEAM_CHANGE_EPXTEAMID:
		{
			ret = tq.ChangeExpID(roArg.expid(), roRes, m2gRes->teamnothelper());
		}
		break;
	case KKSG::TEAM_INVITE:
		{
			ret = tq.InviteAft(roArg.roleid());
		}
		break;
	case KKSG::TEAM_START_MATCH:
		{
			ret = tq.StartMatch(roArg.expid(), false);
		}
		break;
	case KKSG::TEAM_DOWN_MATCH:
		{
			ret = tq.StartMatch(roArg.expid(), true);
		}
		break;
	case KKSG::TEAM_MEMBER_TYPE:
		{
			ret = tq.SwitchMemType(roArg.membertype());
		}
		break;
	case KKSG::TEAM_COSTTYPE:
		{
			if (roArg.has_extrainfo())
			{
				ret = tq.SetTeamCost(roArg.extrainfo().costindex());
			}
		}
		break;
	case KKSG::TEAM_QUERYCOUNT:
		{
			roRes.set_weeknestexpid(tq.GetWeekNestCurExp());	
		}
		break;
	case KKSG::TEAM_BATTLE_CONTINUE:
		{
			ret = tq.BattleContinue(m2gRes->teamhelper());
		}
		break;
	default:
		break;
	}

	if(ERR_TEAM_SERVER_OPEN_TIME == ret)
	{
		UINT32 opentime = tq.GetExpOpenTime(roArg.expid());
		roRes.set_opentime(opentime);
	}

	roRes.set_result((KKSG::ErrorCode)ret);
}
