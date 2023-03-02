#include "pch.h"
#include "common/ptcc2g_closehintntf.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"
#include "define/systemiddef.h"
#include "idip/idiprecord.h"

// generate by ProtoGen at date: 2017/2/9 20:55:50

void PtcC2G_CloseHintNtf::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return;
	}
	LogInfo("systemid: %u", m_Data.systemid());
	switch (m_Data.systemid())
	{
	case SYS_QQVIP:
		pRole->Get<RoleMisc>()->ReadQQVipHint();
		break;
	case SYS_EGame:
		pRole->Get<RoleMisc>()->ReadEGameHint();
		break;
	case SYS_XINYUEVIP:
		pRole->Get<RoleMisc>()->ReadXinYueHint();
		break;
	default:
		pRole->Get<CIdipRecord>()->SetHint(m_Data.systemid(), false);
		break;
	}
}
