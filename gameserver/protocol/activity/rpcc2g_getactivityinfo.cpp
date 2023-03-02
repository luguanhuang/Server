#include "pch.h"
#include "activity/rpcc2g_getactivityinfo.h"
#include "unit/rolemanager.h"
#include "gamelogic/activityrecord.h"
#include "gamelogic/activitymgr.h"

// generate by ProtoGen at date: 2016/8/13 17:01:43

RPC_SERVER_IMPLEMETION(RpcC2G_GetActivityInfo, GetActivityInfoArg, GetActivityInfoRes)

void RpcC2G_GetActivityInfo::OnCall(const GetActivityInfoArg &roArg, GetActivityInfoRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	CActivityRecord* pRecord = pRole->Get<CActivityRecord>();
	if (NULL == pRecord)
	{
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	CActivityMgr::Instance()->UpdateRecord(pRecord, pRole);

	KKSG::ActivityRecord* pRecoreSend = roRes.mutable_record();
	pRecord->RecordToKKSG(pRecoreSend);
	for (int i = 0; i < pRecoreSend->activityid_size(); ++i)
	{
		UINT32 dwConfigCount = CActivityMgr::Instance()->GetConfigFinishCount(pRecoreSend->activityid(i));
		if (UINT32(-1) == dwConfigCount)
		{
			dwConfigCount = 0;
		}
		pRecoreSend->add_needfinishcount(dwConfigCount);
	}
	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_GetActivityInfo::OnDelayReplyRpc(const GetActivityInfoArg &roArg, GetActivityInfoRes &roRes, const CUserData &roUserData)
{
}
