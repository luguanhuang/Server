#include "pch.h"
#include "partner/rpcm2g_getpartnerrecordm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolepartner.h"

// generate by ProtoGen at date: 2016/12/11 13:27:38

RPC_SERVER_IMPLEMETION(RpcM2G_GetPartnerRecordM2G, GetPartnerRecordM2GArg, GetPartnerRecordM2GRes)

void RpcM2G_GetPartnerRecordM2G::OnCall(const GetPartnerRecordM2GArg &roArg, GetPartnerRecordM2GRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("no role by session[%llu]", m_sessionID);
		return;
	}
	RolePartner* partner = pRole->Get<RolePartner>();
	partner->SetPartnerID(roArg.partnerid());
	if (roArg.partnerid() != 0)
	{
		partner->SetApplyLeaveTime(roArg.apply_leave_time());
	}
	partner->UpdateLeaveState();

	roRes.set_last_leave_time(partner->GetLastLeavePartnerTime());
	roRes.set_liveness_redpoint(partner->HasCheskCanTake(roArg.level(), roArg.liveness()));
	roRes.set_takedchest(partner->GetChestTakeInfo());
}

void RpcM2G_GetPartnerRecordM2G::OnDelayReplyRpc(const GetPartnerRecordM2GArg &roArg, GetPartnerRecordM2GRes &roRes, const CUserData &roUserData)
{
}
