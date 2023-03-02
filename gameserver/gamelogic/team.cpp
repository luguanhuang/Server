#include "pch.h"
#include "team.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "protocol.h"
#include "teammgr.h"
#include "globalconfig.h"
#include "teammember.h"
#include "stagemgr.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "team/ptcg2c_leaveteam.h"
#include "common/ptcg2c_gserrornotify.h"
#include "table/expeditionconfigmgr.h"
#include "rolesummarymgr.h"
#include "scene/sceneconfig.h"
#include "scene/enterscenecondition.h"
#include <time.h>
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "foreach.h"
#include "teamhandler.h"
#include "unit/dummyrolemanager.h"
#include "unit/dummyrole.h"
#include "robotgenerator.h"
#include "robotconfig.h"
#include "live/liveconfig.h"
#include "live/livemanager.h"
#include "table/DragonNestTable.h"
#include "gamelogic/dragonconfig.h"
#include "gamelogic/dragonrecord.h"
#include "bagtransition.h"
#include "teamrecord.h"

struct BroadCastParam
{
	CProtocol *proto;
	UINT64 exludeRoleID;
};

TeamCopy::TeamCopy()
{
	m_teamID = 0;
	m_guildID = 0;
	m_leaderID = 0;
	m_expID = 0;
	m_pptlimit = 0;
	m_teamcost = (UINT32)(-1);
	m_haspassward = false;
}

TeamCopy::~TeamCopy()
{

}

void TeamCopy::LoadT(const KKSG::TeamSynAll& data)
{
	m_teamID = data.teamid();
	m_expID = data.expid();
	m_leaderID = data.leaderid();
	m_guildID = data.guildid();
	m_pptlimit = data.pptlimit();
	m_teamcost = data.teamcost();
	m_haspassward = data.haspassword();
	
	m_members.clear();
	for(int i = 0; i < data.members_size(); ++i)
	{
		TeamCopyMember tmp;
		tmp.LoadM(data.members(i));	
		m_members.push_back(tmp);
	}
}

void TeamCopy::InfoPrint()
{
	SSInfo << " team copy id = " << m_teamID << END;
	SSInfo << " exp = " << m_expID << END;
	SSInfo << " leader = " << m_leaderID << " haspassword = " << m_haspassward << END;
	SSInfo << " guild = " << m_guildID << END;
	SSInfo << " pptlimit = " << m_pptlimit << " temaconst = " << m_teamcost << END;
	for(auto it = m_members.begin(); it != m_members.end(); ++it)
	{
		(*it).InfoPrint();
	}
}

ExpeditionTable::RowData* TeamCopy::GetConf()
{
	return ExpeditionConfigMgr::Instance()->GetExpeditionConf(m_expID);
}

void TeamCopy::GetOnlineRoles(std::vector<Role *> &roles)
{
	foreach(i in m_members)
	{
		if((*i).IsRobot())
		{
			continue;	
		}
		Role* pRole = RoleManager::Instance()->FindByRoleID((*i).RoleID());
		if(pRole)
		{
			roles.push_back(pRole);
		}
	}
}

bool TeamCopy::HaveRole(UINT64 roleID)
{
	for(size_t i = 0; i < m_members.size(); ++i)
	{
		if(m_members[i].RoleID() == roleID)
		{
			return true;
		}
	}
	return false;
}

void TeamCopy::BroadCast(CProtocol &proto, UINT64 qwExludeRole /*= 0*/)
{
	for(size_t i = 0; i < m_members.size(); ++i)
	{
		if(m_members[i].IsRobot())
		{
			continue;
		}
		UINT64 roleID = m_members[i].RoleID();
		if(roleID == qwExludeRole)
		{
			continue;
		}

		Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
		if(pRole)
		{
			pRole->Send(proto);
		}
	}
}

void TeamCopy::EnterTakeCost(UINT64 roleID)
{
	ExpeditionTable::RowData* pConf = GetConf();
	if(NULL == pConf)
	{
		return ;
	}
	if(!HaveRole(roleID))
	{
		return;
	}
	if(GetMemberType(roleID) != KKSG::TMT_NORMAL)
	{
		return;
	}
	Role* pRole = RoleManager::Instance()->FindByRoleID(roleID);
	if(NULL == pRole)
	{
		return;
	}

	TeamCostCtrl pTeamCostCtrl;

	pTeamCostCtrl.EnterTakeItem(pRole, pConf->CostItem);
	pTeamCostCtrl.CheckAddTeamDayCount(pRole, pConf, TEAM_CCT_ENTER);
}

void TeamCopy::EnterTakeCost()
{
	std::vector<UINT64> roleIDs;
	FilleRoleIDs(roleIDs, true);
	for(size_t i = 0; i < roleIDs.size(); ++i)
	{
		EnterTakeCost(roleIDs[i]);
	}
}

KKSG::TeamMemberType TeamCopy::GetMemberType(UINT64 roleID)
{
	for(size_t i = 0; i < m_members.size(); ++i)
	{
		if(m_members[i].RoleID() == roleID)
		{
			return m_members[i].GetMemberType();	
		}
	}
	return KKSG::TMT_NORMAL;
}

void TeamCopy::FilleRoleIDs(std::vector<UINT64>& roleIDs, bool excludeRobot)
{
	for(size_t i = 0; i < m_members.size(); ++i)
	{
		if(m_members[i].IsRobot() && excludeRobot)	
		{
			continue;
		}
		roleIDs.push_back(m_members[i].RoleID());
	}
}

void TeamCostCtrl::CheckAddTeamDayCount(Role *pRole, ExpeditionTable::RowData* pConf, TeamCostCountType cctype)
{
	if(NULL == pRole || NULL == pConf)
	{
		return;
	}

	if(TEAM_CCT_NONE == pConf->CostCountType)
	{
		return ;
	}

	if(pConf->CostCountType != cctype)
	{
		return ;
	}

	pRole->Get<CTeamRecord>()->AddTeamFinishCountToday(pConf->Type);

	pRole->OnTeamStage(pConf->Type);
}

void TeamCostCtrl::EnterTakeItem(Role *pRole, std::vector<Sequence<int, 2>>& pVec)
{
	if(pVec.empty())
	{
		return;
	}

	BagTakeItemTransition transtion(pRole);
	transtion.SetReason(ItemFlow_DRAGON, ItemFlow_DRAGON_Enter);
	foreach(i in pVec)
	{
		transtion.TakeItem(i->seq[0], i->seq[1]);
	}
	transtion.NotifyClient();
}

void TeamCostCtrl::TakeUseTicket(Role *pRole, ExpeditionTable::RowData* pConf)
{
	if(NULL == pRole || NULL == pConf)
	{
		return;
	}
	
	UINT32 itemID = pConf->UseTicket[0];
	if(0 == itemID)
	{
		SSError << " role = " << pRole->GetID() << " expid = " << pConf->DNExpeditionID << " item = " << itemID << END;
		return ;
	}

	pRole->Get<CTeamRecord>()->AddUseTicketCount();

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Stage, ItemFlow_Team_UseTicket);
	if(!take.TakeItem(itemID, 1))
	{
		take.RollBack();
		SSError << " role = " << pRole->GetID() << " expid = " << pConf->DNExpeditionID << " item = " << itemID << END;
		return ;
	}
	take.NotifyClient();
}

