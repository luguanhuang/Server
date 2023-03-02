#include "pch.h"
#include "levelseal/rpcc2g_getlevelsealinfo.h"
#include "gamelogic/levelsealMgr.h"
#include "gamelogic/levelsealRecord.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2016/6/1 17:13:42

RPC_SERVER_IMPLEMETION(RpcC2G_GetLevelSealInfo, GetLevelSealInfoArg, GetLevelSealInfoRes)

void RpcC2G_GetLevelSealInfo::OnCall(const GetLevelSealInfoArg &roArg, GetLevelSealInfoRes &roRes)
{
	roRes.set_errorcode(ERR_UNKNOWN);
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	CLevelSealRecord* record = pRole->Get<CLevelSealRecord>();
	if( NULL == record )
	{
		roRes.set_errorcode(ERR_SUCCESS);
		return; 
	}
	LevelSealInfo* pstSealInfo = roRes.mutable_levelsealdata();
	stServerLevelSealInfo stSealInfo = CLevelSealMgr::Instance()->GetLevelSealInfo();
	record->CheckType(stSealInfo.m_uType);
	pstSealInfo->set_type(stSealInfo.m_uType);
	pstSealInfo->set_endtime(stSealInfo.m_uEndTime);
	pstSealInfo->set_totalcollectcount(stSealInfo.m_uTotalCollectCount);
	pstSealInfo->set_selfcollectcount(record->GetSelfCollectCount());
	pstSealInfo->set_selfawardcountindex(record->GetSelfAwardCountIndex());
	//拉取的时候需要判断下
	pRole->CheckLevelSealStatus();
	pstSealInfo->set_status(record->GetButtonStatus());
	roRes.set_errorcode(ERR_SUCCESS);

	LogInfo("levelseal openid=%s,roleid=%llu,type=%u,end_time=%u,status=%u", pRole->GetAccount().c_str(), 
		pRole->GetID(), stSealInfo.m_uType, stSealInfo.m_uEndTime, record->GetButtonStatus());
}

void RpcC2G_GetLevelSealInfo::OnDelayReplyRpc(const GetLevelSealInfoArg &roArg, GetLevelSealInfoRes &roRes, const CUserData &roUserData)
{
}
