#include "pch.h"
#include "team/rpcm2g_teamstartbattlem2g.h"
#include "gamelogic/team.h"
#include "gamelogic/towerrecord.h"
#include "gamelogic/towerconfig.h"
#include "gamelogic/msusesummarymgr.h"
#include "gamelogic/rolesummary.h"
#include "gamelogic/rolesummarymgr.h"
#include "scene/sceneswitch.h"
#include "unit/rolemanager.h"
#include "gamelogic/teamrequest.h"

// generate by ProtoGen at date: 2016/8/18 17:31:11

RPC_SERVER_IMPLEMETION(RpcM2G_TeamStartBattleM2G, TeamStartBattleM2GArg, TeamStartBattleM2GRes)

void RpcM2G_TeamStartBattleM2G::OnCall(const TeamStartBattleM2GArg &roArg, TeamStartBattleM2GRes &roRes)
{
	TeamCopy pTeam;
	pTeam.LoadT(roArg.ssdata().teamdata().teamsyndata());
	auto pConf = pTeam.GetConf();
	if(NULL == pConf)
	{
		SSError << " conf = null teamid = " << pTeam.GetTeamID() << END;
		roRes.set_errcode(ERR_FAILED);
		return;
	}

	TeamRequest Tq(NULL, &pTeam);
	UINT64 tmpID = 0;//tmp, no use
	int ret = Tq.CheckStartBattleCondition(tmpID);
	if(ERR_SUCCESS != ret)
	{
		roRes.set_errcode(ErrorCode(ret));
		return;
	}

	KKSG::SceneSwitchData oData;
	oData.CopyFrom(roArg.ssdata());

	KKSG::SceneSwitchTeamData* data = oData.mutable_teamdata();
	//if(data->sceneids_size() <= 0)
	//{
	//	SSError << " team scene list = 0 " << END;
	//	roRes.set_errcode(ERR_FAILED);
	//	return ;
	//}
	//UINT32 sceneTemplateID = data->sceneids(0);

	////塔 no use for now
	//if (pConf->RandomSceneIDs.size() <= TOTAL_FLOOR_SIZE)
	//{
	//	data->set_towerreachfloor(TOTAL_FLOOR_SIZE - pConf->RandomSceneIDs.size());
	//	int hardlevel = TowerConfig::Instance()->GetHardLevel(sceneTemplateID);
	//	int time = TowerConfig::Instance()->GetSweepEachLevelTime(hardlevel);
	//	data->set_towerusetime(data->towerreachfloor() * time);
	//}

	UINT32 sceneTemplateID = data->airsceneid();
	if(0 == sceneTemplateID)//没有飞艇
	{
		sceneTemplateID = ExpeditionConfigMgr::Instance()->GetExpRandomScene(pTeam.GetExpID());
	}

	if(0 == sceneTemplateID)
	{
		SSError << " find mapid = 0 expid = " << pTeam.GetExpID() << END;
		roRes.set_errcode(ERR_FAILED);
		return;
	}

	CSceneSwitch::RoleList rolelist;


	const auto& members = pTeam.GetMember();
	for(size_t i = 0; i < members.size(); ++i)
	{
		if(members[i].IsRobot())
		{
			continue;
		}
		Role* pRole = RoleManager::Instance()->FindByRoleID(members[i].RoleID());
		if(NULL == pRole || NULL == pRole->GetCurrScene())
		{
			SSWarn << " unnormal role id = " << members[i].RoleID() << END;
			roRes.set_errcode(ERR_FAILED);
			return ;
		}
		rolelist.push_back(pRole);
	}


	if (sceneTemplateID == 50026)
	{
		for (size_t i = 0; i < members.size(); ++i)
		{
			oData.mutable_horsedata()->add_roleid(members[i].RoleID());
		}
	}
	bool flag = CSceneSwitch::ChangeScene(rolelist, sceneTemplateID, oData);
	if(!flag)
	{
		SSWarn << " team change scene err teamid = " << pTeam.GetTeamID() << END;
		roRes.set_errcode(ERR_FAILED);
		return;
	}
	pTeam.EnterTakeCost();
}

void RpcM2G_TeamStartBattleM2G::OnDelayReplyRpc(const TeamStartBattleM2GArg &roArg, TeamStartBattleM2GRes &roRes, const CUserData &roUserData)
{
}
