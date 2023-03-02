#include "pch.h"
#include "team/ptcm2g_teamtranms2gs.h"
#include "define/teamdef.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/teamrecord.h"

// generate by ProtoGen at date: 2017/7/10 21:23:22

void PtcM2G_TeamTranMs2Gs::Process(UINT32 dwConnID)
{
	switch(m_Data.type())
	{
	case TEAM_TRANS_COST_CANCEL:
		{
			Role* pRole = RoleManager::Instance()->FindByRoleID(m_Data.roleid());
			if(pRole)
			{
				pRole->Get<CTeamRecord>()->RecoverTeamCostInfo();
			}
			else
			{
				SSWarn << " find role failed roleid = " << m_Data.roleid() << END;
			}
		}
		break;
	default:
		break;
	}
}
