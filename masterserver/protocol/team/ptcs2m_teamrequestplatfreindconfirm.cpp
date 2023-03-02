#include "pch.h"
#include "team/ptcs2m_teamrequestplatfreindconfirm.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/teaminvitemgr.h"

// generate by ProtoGen at date: 2017/5/3 15:42:44

void PtcS2M_TeamRequestPlatFreindConfirm::Process(UINT32 dwConnID)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_Data.destinfo().roleid());
	if (!role)
	{
		return;
	}
	//todo

	KKSG::TeamInvite data = m_Data.teaminviteinfo();
	data.set_invtoroleid(m_Data.destinfo().roleid());
	TeamInviteMgr::Instance()->MakeInvite(data);
}
