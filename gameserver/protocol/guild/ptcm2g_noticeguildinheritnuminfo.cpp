#include "pch.h"
#include "guild/ptcm2g_noticeguildinheritnuminfo.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "guild/guildrecord.h"
#include "guild/ptcg2c_synguildinheritnuminfo.h"
#include <time.h>

// generate by ProtoGen at date: 2016/11/19 14:48:19

void PtcM2G_NoticeGuildInheritNumInfo::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (pRole == NULL)
	{
		return ;
	}

	PtcG2C_SynGuildInheritNumInfo msg;
	msg.m_Data.set_reqnum(m_Data.reqnum());
	if (GetGlobalConfig().GuildInheritStuLimit > pRole->Get<CGuildRecord>()->GetInheritStuTime())
	{
		msg.m_Data.set_studentnum(GetGlobalConfig().GuildInheritStuLimit - pRole->Get<CGuildRecord>()->GetInheritStuTime());
	}
	else
	{
		msg.m_Data.set_studentnum(0);
	}

	if (GetGlobalConfig().GuildInheritTeaLimit > pRole->Get<CGuildRecord>()->GetInheritTeaTime())
	{
		msg.m_Data.set_teachernum(GetGlobalConfig().GuildInheritTeaLimit - pRole->Get<CGuildRecord>()->GetInheritTeaTime());
	}
	else
	{
		msg.m_Data.set_teachernum(0);
	}

	UINT32 cdTime = pRole->Get<CGuildRecord>()->GetGuildInheritCDTime(); 
	UINT32 nowTime = time(NULL);
	if (nowTime >= cdTime && (nowTime - cdTime) < (GetGlobalConfig().GuildInheritCDTime * 60))
	{
		msg.m_Data.set_lasttime((GetGlobalConfig().GuildInheritCDTime * 60) - (nowTime - cdTime));
	}
	else
	{
		msg.m_Data.set_lasttime(0);
	}
	pRole->Send(msg);
}
