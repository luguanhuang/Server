#include "pch.h"
#include "team/ptch2m_teamtrants2ms.h"
#include "define/teamdef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "team/ptcm2g_teamtranms2gs.h"
#include "other/matchhandler.h"
#include "team/ptcm2c_fastmbconfirmm2cntf.h"
#include "team/ptcm2c_fastmbdismissm2cntf.h"
#include "team/ptcm2c_startbattlefailedm2cntf.h"

// generate by ProtoGen at date: 2017/7/10 21:22:43

void PtcH2M_TeamTranTs2Ms::Process(UINT32 dwConnID)
{
	switch(m_Data.type())
	{
	case TEAM_TRANS_COST_CANCEL:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
			if(pRole)
			{
				PtcM2G_TeamTranMs2Gs ptc;
				ptc.m_Data = m_Data;
				pRole->SendMsgToGS(ptc);
			}
			else
			{
				SSWarn << " find role failed roleid = " << m_Data.roleid() << " type = " << m_Data.type() << END;
			}
		}
		break;
	case TEAM_TRANS_ROLE_MATCH:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
			if(pRole)
			{
				HMatchHandler handler;
				handler.Done(pRole, KKSG::KMT_EXP);
			}
			else
			{
				SSWarn << " find role failed roleid = " << m_Data.roleid() << " type = " << m_Data.type() << END;
			}
		}
		break;
	case TEAM_TRANS_RM_CONFIRM_NOTICE:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
			if(pRole)
			{
				PtcM2C_FastMBConfirmM2CNtf ptc;
				pRole->Send(ptc);
			}
			else
			{
				SSWarn << " find role failed roleid = " << m_Data.roleid() << " type = " << m_Data.type() << END;
			}
		}
		break;
	case TEAM_TRANS_RM_REFUSE_NOTICE:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
			if(pRole)
			{
				PtcM2C_fastMBDismissM2CNtf ptc;
				ptc.m_Data.set_quitrolename(m_Data.account());
				pRole->Send(ptc);
			}
			else
			{
				SSWarn << " find role failed roleid = " << m_Data.roleid() << " type = " << m_Data.type() << END;
			}
		}
		break;
	case TEAM_TRANS_START_BATTLE_FAILED:
		{
			CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_Data.roleid());
			if(pRole)
			{
				PtcM2C_StartBattleFailedM2CNtf ptc;
				ptc.m_Data = m_Data.startbattlefaileddata();
				pRole->Send(ptc);
			}
			else
			{
				SSWarn << " find role failed roleid = " << m_Data.roleid() << " type = " << m_Data.type() << END;
			}
		}
		break;
	default:
		break;
	}
}
