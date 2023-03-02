#include "pch.h"
#include "holidaystage/rpcc2g_getholidaystageinfo.h"
#include "gamelogic/holidaystage.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/1/12 20:19:20

RPC_SERVER_IMPLEMETION(RpcC2G_GetHolidayStageInfo, GetHolidayStageInfoArg, GetHolidayStageInfoRes)

void RpcC2G_GetHolidayStageInfo::OnCall(const GetHolidayStageInfoArg &roArg, GetHolidayStageInfoRes &roRes)
{
	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		return ;
	}

	UINT32 id = CHolidayStage::Instance()->GetHolidayId(); 
	UINT32 sceneId = CHolidayStage::Instance()->GetHolidaySceneId(); 
	if (id == 0)
	{
		roRes.set_holidayid(0);
		roRes.set_sceneid(0);
	}
	else
	{
		roRes.set_holidayid(id);
		roRes.set_sceneid(sceneId);
		roRes.set_havetimes(CHolidayStage::Instance()->GetLostTimes(pRole->GetID(), sceneId));
		roRes.set_lasttime(CHolidayStage::Instance()->GetHaveTime());
	}
	
}

void RpcC2G_GetHolidayStageInfo::OnDelayReplyRpc(const GetHolidayStageInfoArg &roArg, GetHolidayStageInfoRes &roRes, const CUserData &roUserData)
{
}
