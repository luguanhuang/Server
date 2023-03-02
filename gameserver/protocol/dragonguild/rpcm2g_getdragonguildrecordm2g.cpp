#include "pch.h"
#include "dragonguild/rpcm2g_getdragonguildrecordm2g.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "dragonguild/dragonguildrecord.h"

// generate by ProtoGen at date: 2017/9/6 10:33:09

RPC_SERVER_IMPLEMETION(RpcM2G_GetDragonGuildRecordM2G, GetPartnerRecordM2GArg, GetPartnerRecordM2GRes)

void RpcM2G_GetDragonGuildRecordM2G::OnCall(const GetPartnerRecordM2GArg &roArg, GetPartnerRecordM2GRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		LogWarn("no role by session[%llu]", m_sessionID);
		return;
	}
	DragonGuildRecord* record = pRole->Get<DragonGuildRecord>();
	if(record == NULL)
	{
		LogWarn("no dragonguildrecord by session[%llu]", m_sessionID);
		return;
	}

	/*
	if (roArg.partnerid() != 0)
	{
		record->SetApplyLeaveTime(roArg.apply_leave_time());
	}
	record->UpdateLeaveState();
	*/

	//roRes.set_last_leave_time(record->GetLastLeaveDragonGuildTime());
	roRes.set_liveness_redpoint(record->HasCheskCanTake(roArg.level(), roArg.liveness()));
	roRes.set_takedchest(record->GetChestTakeInfo());
	
}

void RpcM2G_GetDragonGuildRecordM2G::OnDelayReplyRpc(const GetPartnerRecordM2GArg &roArg, GetPartnerRecordM2GRes &roRes, const CUserData &roUserData)
{
}
