#include "pch.h"
#include "timeutil.h"
#include "roleidgenerator.h"
#include "config.h"


CRoleIDGenerator::CRoleIDGenerator()
:m_dwTime(0)
,m_wIndex(0)
,m_wMaxIndex((1 << 12) -1)
{
}

CRoleIDGenerator::~CRoleIDGenerator()
{
}

bool CRoleIDGenerator::Init()
{
	m_wIndex = 1;
	m_dwTime = TimeUtil::GetTime();
	return true;
}

void CRoleIDGenerator::UnInit()
{
}

UINT64 CRoleIDGenerator::NewRoleID(UINT32 serverID)
{
	if(m_wIndex > m_wMaxIndex)
	{
		m_wIndex = 1;
		m_dwTime = TimeUtil::GetTime();
	}

	UINT16 wServerID = (UINT16)serverID;
	UINT32 dwHigh = MakeUINT32(m_wIndex, wServerID);

	++m_wIndex;
	return MakeUINT64(dwHigh, m_dwTime);
}