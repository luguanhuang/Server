#include "pch.h"
#include "mobabattle/rpcc2m_getmobabattleinfo.h"
#include "role/rolemanager.h"
#include "mobabattle/mobarecordmgr.h"

// generate by ProtoGen at date: 2017/6/26 14:31:51

RPC_SERVER_IMPLEMETION(RpcC2M_GetMobaBattleInfo, GetMobaBattleInfoArg, GetMobaBattleInfoRes)

void RpcC2M_GetMobaBattleInfo::OnCall(const GetMobaBattleInfoArg &roArg, GetMobaBattleInfoRes &roRes)
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
	record->FillMobaBattleInfo(m_oRes);
}

void RpcC2M_GetMobaBattleInfo::OnDelayReplyRpc(const GetMobaBattleInfoArg &roArg, GetMobaBattleInfoRes &roRes, const CUserData &roUserData)
{
}
