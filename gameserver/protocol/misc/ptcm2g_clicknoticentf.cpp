#include "pch.h"
#include "misc/ptcm2g_clicknoticentf.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "idip/idiprecord.h"

// generate by ProtoGen at date: 2017/2/4 11:06:37

void PtcM2G_ClickNoticeNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
	if (pRole == NULL)
	{
		LogError("Can't find role by roleid: %llu", m_Data.roleid());
		return;
	}
	pRole->Get<CIdipRecord>()->ClickNotice(m_Data.notice());
}
