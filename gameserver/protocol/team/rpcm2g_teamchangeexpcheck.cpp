#include "pch.h"
#include "team/rpcm2g_teamchangeexpcheck.h"
#include "gamelogic/team.h"
#include "unit/rolemanager.h"
#include "gamelogic/teamrequest.h"
#include "unit/role.h"
#include "gamelogic/dragonconfig.h"
#include "gamelogic/dragonrecord.h"
#include "gamelogic/teamrecord.h"

// generate by ProtoGen at date: 2016/8/24 19:58:05

RPC_SERVER_IMPLEMETION(RpcM2G_TeamChangeExpCheck, TeamChangeExpArg, TeamChangeExpRes)

void RpcM2G_TeamChangeExpCheck::OnCall(const TeamChangeExpArg &roArg, TeamChangeExpRes &roRes)
{
	roRes.set_ret(ERR_FAILED);//默认失败

	TeamCopy pTeam;
	pTeam.LoadT(roArg.teamalldata());
	UINT64 leader = pTeam.GetLeaderID();
	Role* pRole = RoleManager::Instance()->FindByRoleID(leader);
	if(NULL == pRole)
	{
		return ;
	}
	auto pConf = pTeam.GetConf();
	if(NULL == pConf)
	{
		return;
	}
	int expID = 0;
	switch(pConf->Type)
	{
	case DRAGON_TEAM_SCENE://龙本
		{
			expID = pRole->Get<CDragonRecord>()->GetNextExpID(pTeam.GetExpID());
		}
		break;
	case NEST_TEAM_SCENE://巢穴
		{
			auto pmgr = ExpeditionConfigMgr::Instance();
			if(!pmgr->IsStarExp(pConf))
			{
				return;
			}
			int tmpid = pRole->Get<CTeamRecord>()->GetStarProgressByType(pmgr->GetStarExpType(pConf));
			if(tmpid != pConf->DNExpeditionID)
			{
				expID = tmpid;
			}
		}
		break;
	default:
		break;
	}

	if(0 == expID)
	{
		return ;
	}

	TeamRequest req(pRole, &pTeam);
	TeamOPRes tmp;//no use
	int ret = req.ChangeExpID(expID, tmp, *roRes.mutable_teamnothelper());
	roRes.set_ret((ErrorCode)ret);
	roRes.set_expid(expID);
}

void RpcM2G_TeamChangeExpCheck::OnDelayReplyRpc(const TeamChangeExpArg &roArg, TeamChangeExpRes &roRes, const CUserData &roUserData)
{
}
