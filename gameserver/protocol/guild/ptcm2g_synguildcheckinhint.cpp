#include "pch.h"
#include "guild/ptcm2g_synguildcheckinhint.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/9/20 19:30:31

void PtcM2G_SynGuildCheckinHint::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (pRole != NULL)
	{
		if (!pRole->Get<CGuildRecord>()->IsCheckin())
		{
			pRole->HintNotify(SYS_GUILD_CHECKIN, false);
		}
	}
}
