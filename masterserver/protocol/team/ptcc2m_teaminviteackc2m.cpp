#include "pch.h"
#include "team/ptcc2m_teaminviteackc2m.h"
#include "team/teaminvitemgr.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teamrequest.h"

// generate by ProtoGen at date: 2016/8/18 14:23:53

void PtcC2M_TeamInviteAckC2M::Process(UINT32 dwConnID)
{
	CRole *pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}

	if(m_Data.accept() && pRole->HasTeam())
	{
		pRole->ErrorCodeNtf(KKSG::ERR_TEAM_ALREADY_INTEAM, true);
		return;
	}

	TeamInviteMgr::Instance()->OnAck(pRole, m_Data.inviteid(), m_Data.accept());
}
