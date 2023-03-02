#include "pch.h"
#include "battlewatch/rpcc2g_getmywatchrecord.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "live/liverecord.h"

// generate by ProtoGen at date: 2016/7/11 16:12:16

RPC_SERVER_IMPLEMETION(RpcC2G_GetMyWatchRecord, GetMyWatchRecordArg, GetMyWatchRecordRes)

void RpcC2G_GetMyWatchRecord::OnCall(const GetMyWatchRecordArg &roArg, GetMyWatchRecordRes &roRes)
{
	roRes.set_error(ERR_FAILED);

	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role || !role->GetCurrScene())
	{
		roRes.set_error(ERR_INVALID_REQUEST);
		return;
	}

	CLiveRecord* record = role->Get<CLiveRecord>();
	if (!record)
	{
		return;
	}

	UINT32 error = record->GetMyRecord2Client(roRes);
	roRes.set_error((ErrorCode)error);
}

void RpcC2G_GetMyWatchRecord::OnDelayReplyRpc(const GetMyWatchRecordArg &roArg, GetMyWatchRecordRes &roRes, const CUserData &roUserData)
{
}
