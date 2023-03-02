#include "pch.h"
#include "teammember.h"
#include "pb/project.pb.h"
#include "unit/unit.h"
#include "team/teamcommon.h"

void TeamCopyMember::LoadM(const KKSG::TeamSynMember& data)
{
	m_data = data;
}

bool TeamCopyMember::IsRobot() const
{
	return (Unit::GetUnitType(RoleID()) == KKSG::Category_DummyRole);
}

bool TeamCopyMember::IsHelper() const
{
	return HTeam::IsHelper(m_data.memtype());
}

void TeamCopyMember::InfoPrint()
{
	SSInfo << " role = " << RoleID() << " server = " << ServerID() << " type = " << GetMemberType() << END;
}

TeamEnterData::TeamEnterData(KKSG::TeamMemberType memtype)
{
	Reset();
	m_memtype = memtype;
}

TeamEnterData::TeamEnterData(bool gotoNext, KKSG::TeamMemberType memtype)
{
	Reset();
	m_gotoNext = gotoNext;
	m_memtype = memtype;
}

TeamEnterData::TeamEnterData()
{
	Reset();
}

TeamEnterData::~TeamEnterData()
{
	
}

void TeamEnterData::Reset()
{
	m_gotoNext = false;
	m_memtype = KKSG::TMT_NORMAL;
}

bool TeamEnterData::IsGotoNext()
{
	return m_gotoNext;
}

bool TeamEnterData::IsHelper()
{
	return HTeam::IsHelper(m_memtype);
}

bool TeamEnterData::IsUseTicket()
{
	return HTeam::IsUseTicket(m_memtype);
}

TeamEnterData TeamEnterData::GlobalTeamEnterData;
