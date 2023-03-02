#include "pch.h"
#include "mobabattle/rpcc2m_getmobabattleweekreward.h"
#include "role/rolemanager.h"
#include "mobabattle/mobarecordmgr.h"

// generate by ProtoGen at date: 2017/6/26 14:36:04

RPC_SERVER_IMPLEMETION(RpcC2M_GetMobaBattleWeekReward, GetMobaBattleWeekRewardArg, GetMobaBattleWeekRewardRes)

void RpcC2M_GetMobaBattleWeekReward::OnCall(const GetMobaBattleWeekRewardArg &roArg, GetMobaBattleWeekRewardRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	MobaRecord* record = MobaRecordMgr::Instance()->GetMobaRoleRecord(pRole->GetID());
	if (record == NULL)
	{
		LogWarn("role [%llu], login but mobarecord is NULL", pRole->GetID());
		roRes.set_errorcode(KKSG::ERR_FAILED);
		return;
	}
	KKSG::ErrorCode errorcode = record->GetWeekNextReward(pRole);
	roRes.set_errorcode(errorcode);
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		roRes.set_weekprize(record->GetWeekPrize());
		roRes.set_getnextweekprize(record->CanGetNextWeekPrize());
	}
}

void RpcC2M_GetMobaBattleWeekReward::OnDelayReplyRpc(const GetMobaBattleWeekRewardArg &roArg, GetMobaBattleWeekRewardRes &roRes, const CUserData &roUserData)
{
}
