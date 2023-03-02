#include "pch.h"
#include <time.h>
#include "teammgr.h"
#include "team.h"
#include "team/ptcg2c_leaveteam.h"
#include "foreach.h"
#include "unit/role.h"
#include "teamrequest.h"
#include "scene/scene.h"
#include "rolesummarymgr.h"
#include "teammember.h"
#include "teaminvitemgr.h"
#include "unit/rolemanager.h"
#include "stagemgr.h"
#include "gamelogic/teamrequest.h"
#include "gamelogic/teamrecord.h"
#include "table/expeditionconfigmgr.h"
#include "team/teamcommon.h"
#include "cmdline.h"
#include "teamsynextradata.h"

INSTANCE_SINGLETON(TeamCopyMgr)

static bool TeamCmdHandler(const CmdLineArgs &args, std::string &outputMessage)
{
	TeamCopyMgr::Instance()->InfoPrint();

	return true;
}

bool TeamCopyMgr::Init()
{
	CmdLine::RegistCmd("team", &TeamCmdHandler);

	return true;
}

void TeamCopyMgr::Uninit()
{
	ClearData();
}

TeamCopyMgr::TeamCopyMgr()
{

}

TeamCopyMgr::~TeamCopyMgr()
{

}

void TeamCopyMgr::SynFromMS(const KKSG::TeamSynPara& data, KKSG::TeamSynRes& roRes)
{
	switch(data.type())
	{
	case KKSG::STM2C_ALL_DATA:
		{
			SynAddOrUpdateTeam(data.dataall());
		}
		break;
	case KKSG::STM2C_DESTROY:
		{
			int teamid = data.dataall().teamid();
			SynDestroy(teamid);
		}
		break;
	case KKSG::STM2C_GETEXTRADATA:
		{
			SynGetExtraData(data, roRes);	
		}
		break;
	case KKSG::STM2C_RESETCOST:
		{
			SynResetCost(data.leaderid());
		}
		break;
	case KKSG::STM2C_TEAM_LIST:
		{
			for(int i = 0; i < data.teamlist_size(); ++i)
			{
				SynAddOrUpdateTeam(data.teamlist(i));
			}
		}
		break;
	case KKSG::STM2C_TS_DISCONNECTED:
		{
			SynTsDisconnected();
		}
		break;
	default:
		break;
	}

	//TestPrint();
}

void TeamCopyMgr::SynAddOrUpdateTeam(const KKSG::TeamSynAll& data)
{
	int teamid = data.teamid();
	if(m_teams.find(teamid) == m_teams.end())
	{
		SynAddTeam(data);			
	}
	else
	{
		SynUpdateTeam(data);
	}

}

void TeamCopyMgr::SynAddTeam(const KKSG::TeamSynAll& data)
{
	int teamID = data.teamid();
	if(m_teams.find(teamID) != m_teams.end())
	{
		return ;
	}
	TeamCopy* pTeam = new TeamCopy();
	pTeam->LoadT(data);
	m_teams[teamID] = pTeam;

	UpdateRole2Team(pTeam, true);
}

void TeamCopyMgr::SynUpdateTeam(const KKSG::TeamSynAll& data)
{
	int teamID = data.teamid();
	auto it = m_teams.find(teamID);
	if(it == m_teams.end())
	{
		return;
	}
	
	auto pTeam = (it->second);

	UpdateRole2Team(pTeam, false);
	pTeam->LoadT(data);
	UpdateRole2Team(pTeam, true);
}

void TeamCopyMgr::SynDestroy(int teamID)
{
	auto it = m_teams.find(teamID);
	if(it == m_teams.end())
	{
		return;
	}
	UpdateRole2Team(it->second, false);
	delete it->second;
	it->second = NULL;
	m_teams.erase(teamID);
}

void TeamCopyMgr::InfoPrint()
{
	for(auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		(it->second)->InfoPrint();
	}

	for(auto it = m_role2team.begin(); it != m_role2team.end(); ++it)
	{
		SSInfo << " role = " << it->first << " team = " << it->second << END;
	}
}

TeamCopy* TeamCopyMgr::GetTeamByRoleID(UINT64 roleID)
{
	auto it = m_teams.find(GetTeamIDByRoleID(roleID));
	if(it != m_teams.end())
	{
		return (it->second);
	}
	return NULL;
}

int TeamCopyMgr::GetTeamIDByRoleID(UINT64 roleID)
{
	auto it = m_role2team.find(roleID);
	if(it != m_role2team.end())
	{
		return (it->second);
	}
	return 0;
}

void TeamCopyMgr::SynGetExtraData(const KKSG::TeamSynPara& roArg, KKSG::TeamSynRes& roRes)
{
	int expid = roArg.expid();
	ExpeditionTable::RowData* pConf = ExpeditionConfigMgr::Instance()->GetExpeditionConf(expid);
	if(NULL == pConf)
	{
		return;
	}
	for(int i = 0; i < roArg.roleids_size(); ++i)
	{
		UINT64 roleid = roArg.roleids(i);
		Role* pRole = RoleManager::Instance()->FindByRoleID(roleid);
		if(NULL == pRole)
		{
			continue;
		}
		//KKSG::TeamSynExtraData* extradata = roRes.add_memberextradata();
		//extradata->set_roleid(roleid);
		//if(pConf->CostCountType != TEAM_CCT_NONE)
		//{
		//	TeamRequest req(pRole);
		//	int leftcount = req.GetDayCountLeft(pRole, pConf->Type);
		//	extradata->set_leftcount(leftcount);
		//}
		HTeamSynExtraData syn;
		syn.FillExtraDataExpID(pRole, expid, *roRes.add_memberextradata());	
	}
}

void TeamCopyMgr::SynResetCost(UINT64 leaderid)
{
	Role* role = RoleManager::Instance()->FindByRoleID(leaderid);
	if (NULL == role)
	{
		SSWarn<<"reset team cost, role is NULL:"<<leaderid<<END;
		return;
	}
	role->Get<CTeamRecord>()->RecoverTeamCostInfo();
}

void TeamCopyMgr::UpdateRole2Team(TeamCopy* pTeam, bool add)
{
	if(NULL == pTeam)
	{
		return;
	}

	int teamID = pTeam->GetTeamID();
	const auto& members = pTeam->GetMember();
	for(size_t i = 0; i < members.size(); ++i)
	{
		UINT64 roleID = members[i].RoleID();
		if(add)
		{
			m_role2team[roleID] = teamID;	
		}
		else//remove
		{
			m_role2team.erase(roleID);
		}
	}
}

void TeamCopyMgr::OnMsClose()
{
	SSInfo << __FUNCTION__ << END;
	ClearData();	
}

void TeamCopyMgr::ClearData()
{
	for(auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		delete it->second;
	}
	m_teams.clear();
	m_role2team.clear();
}

void TeamCopyMgr::SynTsDisconnected()
{
	std::vector<int> WaitDel;
	for(auto it = m_teams.begin(); it != m_teams.end(); ++it)
	{
		int teamID = it->first;
		if(HTeam::IsTsTeam(teamID))
		{
			WaitDel.push_back(teamID);
		}
	}

	for(size_t i = 0; i < WaitDel.size(); ++i)
	{
		SynDestroy(WaitDel[i]);
	}
}

