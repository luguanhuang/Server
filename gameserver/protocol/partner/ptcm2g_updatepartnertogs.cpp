#include "pch.h"
#include "partner/ptcm2g_updatepartnertogs.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolepartner.h"
#include "util/gametime.h"

// generate by ProtoGen at date: 2016/12/12 10:54:39

void PtcM2G_UpdatePartnerToGS::Process(UINT32 dwConnID)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("no role by session[%llu]", m_sessionID);
		return;
	}
	RolePartner* partner = pRole->Get<RolePartner>();
	switch (m_Data.type())
	{
		case KKSG::POT_Normal:
		{
			// 加入战友时
			partner->SetPartnerID(m_Data.partnerid());
			break;
		}
		case KKSG::POT_Liveness:
		{
			// 更新活跃度进度
			partner->CheckChest(m_Data.level(), m_Data.liveness());
			break;
		}
		case KKSG::POT_ApplyLeave:
		{
			// 申请了离开
			partner->SetApplyLeaveTime(m_Data.apply_leave_time());
			break;
		}
		case  KKSG::POT_CancelLeave:
		{
			// 取消了离开
			partner->SetApplyLeaveTime(0);
			break;
		}
		case KKSG::POT_Leave:
		{
			// 主动离开
			partner->SetPartnerID(0);
			partner->SetLastLeavePartnerTime(GameTime::GetTime());
			break;
		}
		case KKSG::POT_Dissolve:
		{
			// 被动解散
			partner->SetPartnerID(0);
			break;
		}
		default:
			LogWarn("invalid update type[%u]", m_Data.type());
	}

}
