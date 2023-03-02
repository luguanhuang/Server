#include "pch.h"
#include "team/rpcm2g_teamtranms2gs.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/teamrequest.h"
#include "gamelogic/teamsynextradata.h"
#include "gamelogic/matchhandler.h"
#include "scene/sceneswitch.h"
#include "gamelogic/teamrecord.h"
#include "gamelogic/dragonrecord.h"
#include "gamelogic/dragonconfig.h"

// generate by ProtoGen at date: 2017/4/25 21:36:58

RPC_SERVER_IMPLEMETION(RpcM2G_TeamTranMs2Gs, TeamTransData, TeamTransRes)

void TeamTransOnCallTeam(const TeamTransData &roArg, TeamTransRes &roRes)
{
	const KKSG::TeamOPArg& c2mArg = roArg.c2moparg();
	KKSG::TeamOPRes& c2mRes = *roRes.mutable_c2mopres();
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2mRes.set_result(ERR_FAILED);
		return;	
	}

	TeamCopy pTeam;
	if(roArg.has_teamalldata())
	{
		pTeam.LoadT(roArg.teamalldata());
	}	

	HTeamSynExtraData synExtraData;
	TeamRequest req(pRole, &pTeam);
	
	int ret = ERR_SUCCESS;	
	switch(c2mArg.request())
	{
	case TEAM_CREATE:
		{
			ret = req.CreateExpenditionTeam(c2mArg.expid());
			synExtraData.FillExtraDataExpID(pRole, c2mArg.expid(), *roRes.mutable_teamsynextradata());
		}
		break;
	case TEAM_JOIN:
		{
			ret = req.JoinTeam();
			synExtraData.FillExtraDataExpID(pRole, pTeam.GetExpID(), *roRes.mutable_teamsynextradata());
		}
		break;
	case TEAM_INVITE://目前只是邀请本服玩家
		{
			ret = req.Invite(c2mArg.roleid());	
		}
		break;
	case TEAM_START_BATTLE:
		{
			KKSG::TeamMemberType type = pTeam.GetMemberType(roArg.roleid());	
			TeamEnterData enterData(type);
			ret = req.CheckEnterCondtion(pRole, pTeam.GetExpID(), &enterData);
		}
		break;
	case TEAM_CHANGE_EPXTEAMID:
		{
			ret = req.CheckOpenCondtion(pRole, c2mArg.expid());
			if(pTeam.GetMemberType(pRole->GetID()) == KKSG::TMT_HELPER)
			{
				if(KKSG::ERR_SUCCESS == req.SwitchHelper(c2mArg.expid()))
				{
					roRes.add_roles(pRole->GetID());//继续助战
				}
			}
			synExtraData.FillExtraDataExpID(pRole, c2mArg.expid(), *roRes.mutable_teamsynextradata());
		}
		break;
	case KKSG::TEAM_MEMBER_TYPE:
		{
			KKSG::TeamMemberType memType = c2mArg.membertype();
			switch(memType)
			{
			case TMT_HELPER:
				{
					ret = req.SwitchHelper(pTeam.GetExpID());
				}
				break;
			case TMT_USETICKET:
				{
					ret = req.SwitchUseTicket();
				}
				break;
			case TMT_NORMAL:
				{
					;//need no handle
				}
				break;
			default:
				break;
			}
		}
		break;
	case KKSG::TEAM_COSTTYPE:
		{
			ret = req.HandleTeamCost(c2mArg.extrainfo().costindex());
		}
		break;
	case KKSG::TEAM_START_MATCH:
		{
			ret = req.StartMatch(c2mArg.expid());
		}
		break;
	default:
		break;
	}
	if(ErrorCode_IsValid(ret))
	{
		c2mRes.set_result((ErrorCode)(ret));
	}
}

void TeamTransOnCallTeamMatch(const TeamTransData &roArg, TeamTransRes &roRes)
{
	const KKSG::KMatchCommonArg& c2mArg = roArg.matcharg();
	KKSG::KMatchCommonRes& c2mRes = *roRes.mutable_matchres();
	KKSG::KMatchRole& roleData = *roRes.mutable_matchroledata();

	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2mRes.set_errorcode(ERR_FAILED);
		return;
	}

	MatchHandler handler;
	ErrorCode ret = handler.RoleCondition(pRole, c2mArg.type(), &roleData);
	c2mRes.set_errorcode(ret);
}

void TeamTransOnCallStartBattle(const TeamTransData &roArg, TeamTransRes &roRes)
{
	const KKSG::TeamStartBattleArg& sb = roArg.startbattlearg();
	UINT32 sceneID = sb.sceneid();
	UINT32 mapID = sb.mapid();
	UINT32 gsLine = sb.gsline();

	const KKSG::TeamSynAll& synAll = sb.teamdata().teamsyndata();

	TeamCopy pTeam;
	pTeam.LoadT(synAll);

	SSInfo << " cross team start battle teamid = " << pTeam.GetTeamID() << " sceneid = " << sceneID << " mapid = " << mapID << " gsline = " << gsLine << END;

	for(int i = 0; i < synAll.members_size(); ++i)
	{
		UINT64 roleID = synAll.members(i).roleid();
        UINT32 serverID = synAll.members(i).serverid();
        if(GSConfig::Instance()->GetServerID() != serverID)//其他服玩家
        {
            continue;
        }
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
		if(NULL == pRole)
		{
			SSWarn << " find role failed roleid = " << roleID << END;
			continue;
		}
		if(NULL == pRole->GetCurrScene())
		{
			SSWarn << " role getcurrscene = null roleid = " << roleID << END;
			continue;
		}
		TeamEnterData enterData(synAll.members(i).memtype());
		TeamRequest req(pRole);
		int ret = req.CheckEnterCondtion(pRole, synAll.expid(), &enterData);
		if(KKSG::ERR_SUCCESS != ret)
		{
			SSWarn << " role check enter condition failed roleid = " << roleID << END;
			continue;
		}

        KKSG::SceneSwitchData tmpData;//每次数据都是新的
        *tmpData.mutable_teamdata() = sb.teamdata();
		if(!CSceneSwitch::EnterScene(pRole, mapID, sceneID, tmpData, true, gsLine))
		{
			SSWarn << " role enter scene failed roleid = " << roleID << END;
			continue;
		}

		pTeam.EnterTakeCost(roleID);
		SSInfo << "cross team role enter scene success roleid = " << roleID << END;
	}
}

void TeamTransOnCallRoleMatch(const TeamTransData &roArg, TeamTransRes &roRes)
{
	int expID = roArg.c2moparg().expid();
	KKSG::TeamOPRes& c2mRes = *roRes.mutable_c2mopres();
	Role* pRole = RoleManager::Instance()->FindByRoleID(roArg.roleid());
	if(NULL == pRole)
	{
		c2mRes.set_result(ERR_FAILED);
		return;	
	}

	TeamCopy pTeam;
	if(roArg.has_teamalldata())
	{
		pTeam.LoadT(roArg.teamalldata());
	}	

	HTeamSynExtraData synExtraData;
	TeamRequest req(pRole, &pTeam);
	
	KKSG::ErrorCode ret = (KKSG::ErrorCode)req.StartMatch(expID);	
	synExtraData.FillExtraDataExpID(pRole, expID, *roRes.mutable_teamsynextradata());

	c2mRes.set_result(ret);
}

void TeamTransCheckExpChange(const TeamTransData &roArg, TeamTransRes &roRes)
{
	TeamCopy team;
	team.LoadT(roArg.teamalldata());
	UINT64 leaderID = team.GetLeaderID();
	Role* pRole = RoleManager::Instance()->FindByRoleID(leaderID);
	if(NULL == pRole)
	{
		return;
	}
	auto pConf = team.GetConf();
	if(NULL == pConf)
	{
		return;
	}
	switch(pConf->Type)
	{
	case NEST_TEAM_SCENE:
		{
			auto pmgr = ExpeditionConfigMgr::Instance();
			if(pmgr->IsStarExp(pConf))
			{
				int expID = pRole->Get<CTeamRecord>()->GetStarProgressByType(pmgr->GetStarExpType(pConf));
				if(expID != pConf->DNExpeditionID)
				{
					roRes.set_expid(expID);
				}	
			}
		}
		break;
	case DRAGON_TEAM_SCENE:
		{
			int expID = pRole->Get<CDragonRecord>()->GetNextExpID(team.GetExpID());
			roRes.set_expid(expID);
		}
		break;
	default:
		break;
	}

}

void RpcM2G_TeamTranMs2Gs::OnCall(const TeamTransData &roArg, TeamTransRes &roRes)
{
	switch(roArg.type())
	{
	case TEAM_TRANS_OP_REQ:
		{
			TeamTransOnCallTeam(roArg, roRes);
		}
		break;
	case TEAM_TRANS_TEAM_MATCH:
		{
			TeamTransOnCallTeamMatch(roArg, roRes);
		}
		break;
	case TEAM_TRANS_START_BATTLE:
		{
			TeamTransOnCallStartBattle(roArg, roRes);
		}
		break;
	case TEAM_TRANS_ROLE_MATCH:
		{
			TeamTransOnCallRoleMatch(roArg, roRes);
		}
		break;
	case TEAM_TRANS_CHECK_EXP_CHANGE:
		{
			TeamTransCheckExpChange(roArg, roRes);
		}
		break;
	default:
		break;
	}
}

void RpcM2G_TeamTranMs2Gs::OnDelayReplyRpc(const TeamTransData &roArg, TeamTransRes &roRes, const CUserData &roUserData)
{
}
