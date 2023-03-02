#include "pch.h"
#include "team.h"
#include "teammember.h"
#include "role/role.h"
#include "pb/project.pb.h"
#include "role/rolemanager.h"
#include "role/rolesummarymgr.h"
#include "guild/guildmgr.h"
#include "table/expeditionconfigmgr.h"
#include "scene/scenemanager.h"
#include "robotsummarymgr.h"
#include "team/tarjateammgr.h"
#include "util/gametime.h"
#include "dragonguild/dragonguildmgr.h"
#include "teamsynextradata.h"

void CTeamMember::SaveToTeamMember(KKSG::TeamMember *pMemberData)
{
	if(NULL == pMemberData)
	{
		return;
	}

	pMemberData->set_memberid(m_qwRoleID);
	pMemberData->set_state(MemberState);
	pMemberData->set_robot(IsRobot());
	pMemberData->set_membertype(m_type);

	if(NULL != m_pRobotSummary)//机器人
	{
		m_pRobotSummary->SaveToTeamMember(pMemberData);
		pMemberData->set_leftcount(1);
		pMemberData->set_sceneid(HALL_MAP_ID);
		return;
	}

	UINT32 nTime = TarjaTeamMgr::Instance()->GetTime(m_qwRoleID);
	if (nTime > GameTime::GetTime())
	{
		LogDebug("--member=%llu--name=%s ntime=%u", m_qwRoleID,pMemberData->name().c_str(),nTime);
		pMemberData->set_tarjatime(nTime);
	}

	CRole *pRole = CRoleManager::Instance()->GetByRoleID(m_qwRoleID);

	if (pRole != NULL)//统一用RoleSummary
	{
		//int leftCount = extraData.leftCount;
		//pMemberData->set_leftcount(leftCount);
		//pMemberData->set_guildid(CGuildMgr::Instance()->GetGuildIdByRoleId(pRole->GetID()));
		//pMemberData->set_dragonguildid(DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID()));
		//pMemberData->set_sceneid(pRole->GetMapID());
		//pMemberData->set_dragonguildid(DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID()));
		KKSG::TeamSynExtraData tmpData = m_extraData;
		HTeamSynExtraData syn;
		syn.FillExtraData(pRole, tmpData);
		FillTeamMemberFromExtraData(*pMemberData, tmpData);
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
		if(pRoleSummary)
		{
			pRoleSummary->SaveToTeamMember(pMemberData);
		}
	}
	else
	{
		//不应该出现不在线的情况
		SSError << " team member can not find role id = " << m_qwRoleID << END;
	}
}

void CTeamMember::InfoPrint()
{
	SSInfo << " role = " << RoleID() << " type = " << GetMemType() << " leftcount = " << LeftCount() << " kingback = " << m_extraData.kingback() << END;
}

void CTeamMember::GenerateRoleSummry(const KKSG::RoleSummaryStored& data)
{
	if(m_pRobotSummary)
	{
		return;
	}
	m_pRobotSummary = new CRoleSummary();
	m_pRobotSummary->Init(data);
}

CTeamMember::CTeamMember(UINT64 roleID)
{
	m_qwRoleID = roleID; 
	m_pRobotSummary = NULL;
	MemberState = KKSG::EXPTEAM_IDLE;
}

CTeamMember::~CTeamMember()
{
	delete m_pRobotSummary;
}

void CTeamMember::SetUsetTicket(int useTicket)
{
	if(!useTicket && !UseTicket())
	{
		return;
	}
	SetMemType(useTicket?KKSG::TMT_USETICKET:KKSG::TMT_NORMAL);
}

void CTeamMember::SetIsHelper(bool helper)
{
	if(!helper && !IsHelper())
	{
		return;
	}
	SetMemType(helper?KKSG::TMT_HELPER:KKSG::TMT_NORMAL);
}
