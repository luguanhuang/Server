#include "pch.h"
#include "guild/ptcm2g_notifyguildcheckinprocess.h"
#include "guild/ptcg2c_guildcheckinboxntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"

// generate by ProtoGen at date: 2016/9/18 9:50:31

void PtcM2G_NotifyGuildCheckinProcess::Process(UINT32 dwConnID)
{
	PtcG2C_GuildCheckinBoxNtf msg;
	msg.m_Data.set_processbar(m_Data.process());

	for (UINT32 cnt = 0; cnt < m_Data.roles_size(); ++cnt)
	{
		UINT64 roleId = m_Data.roles(cnt);
		Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roles(cnt));
		if (pRole == NULL)
		{
			continue;
		}

		CGuildRecord* record = pRole->Get<CGuildRecord>();
		record->Update();
		msg.m_Data.set_boxmask(record->GetBoxMask());
		pRole->Send(msg);
	}

}
