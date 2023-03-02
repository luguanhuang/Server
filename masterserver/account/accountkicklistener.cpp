#include "pch.h"
#include "accountkicklistener.h"
#include "scene/scenemanager.h"
#include "scene/gsmanager.h"
#include "friend/friendop.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummarymgr.h"
#include "role/ptcm2d_delrolentf.h"
#include "util/dbutil.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "network/dblink.h"

KickAccountDelRoleListener KickAccountDelRoleListener::Listener;

void KickAccountDelRoleListener::OnKickReply(KKSG::ErrorCode nErrCode, const std::string& strAccount, UINT64 qwRoleID)
{
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(qwRoleID);
	if (!summary)
	{
		return;
	}
	PtcM2D_DelRoleNtf ntf;
	ntf.m_Data.set_role_id(qwRoleID);
	ntf.m_Data.set_account(strAccount);
	ntf.m_Data.set_slot(MapProfessionToSlot(summary->GetProfession()));
	DBLink::Instance()->SendTo(ntf);
}