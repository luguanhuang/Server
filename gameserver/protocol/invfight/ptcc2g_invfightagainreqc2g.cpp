#include "pch.h"
#include "invfight/ptcc2g_invfightagainreqc2g.h"
#include "unit/unit.h"
#include "unit/rolemanager.h"
#include "scene/scene.h"
#include "scene/sceneinvfight.h"

// generate by ProtoGen at date: 2017/8/30 15:39:46

void PtcC2G_InvfightAgainReqC2G::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if(pRole && pRole->GetCurrScene() && pRole->GetCurrScene()->GetInvFightHandler())
	{
		pRole->GetCurrScene()->GetInvFightHandler()->RoleReqFightAgain(pRole);
	}
}
