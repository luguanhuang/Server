#include "pch.h"
#include "mobabattle/rpcc2m_getmobabattlebriefrecord.h"
#include "role/rolemanager.h"
#include "role/role.h"
#include "mobabattle/mobarecordmgr.h"
#include "mobabattle/mobarecord.h"

// generate by ProtoGen at date: 2017/6/16 11:00:33

RPC_SERVER_IMPLEMETION(RpcC2M_GetMobaBattleBriefRecord, GetMobaBattleBriefRecordArg, GetMobaBattleBriefRecordRes)

void RpcC2M_GetMobaBattleBriefRecord::OnCall(const GetMobaBattleBriefRecordArg &roArg, GetMobaBattleBriefRecordRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		return;
	}
	MobaRecord* record = MobaRecordMgr::Instance()->GetMobaRoleRecord(pRole->GetID());
	if (record == NULL)
	{
		LogWarn("role [%llu], login but mobarecord is NULL", pRole->GetID());
		return;
	}
	record->FillGameBriefRecord(roRes);
}

void RpcC2M_GetMobaBattleBriefRecord::OnDelayReplyRpc(const GetMobaBattleBriefRecordArg &roArg, GetMobaBattleBriefRecordRes &roRes, const CUserData &roUserData)
{
}
