#include "pch.h"
#include "checkin/rpcc2g_checkin.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/checkinmgr.h"
#include "gamelogic/checkinrecord.h"
#include "time.h"
#include "util/gametime.h"
#include "table/OpenSystemMgr.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2015/6/22 21:19:55

RPC_SERVER_IMPLEMETION(RpcC2G_Checkin, CheckinArg, CheckinRes)

void RpcC2G_Checkin::OnCall(const CheckinArg &roArg, CheckinRes &roRes)
{
	roRes.set_daycancheck(0);
	roRes.set_daycheckinfo(0);
	roRes.set_bonus(0);
	roRes.set_daymakeup(0);

	///> system open
	//if (OpenSystemMgr::Instance()->IsIdipCloseSystem(SysCheckinId))
	//{
	//	SSInfo<<"system close"<<END;
	//	return;
	//}

	Role* pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	if (!pRole->Get<CRoleSystem>()->IsSystemOpened(SysCheckinId))
	{
		SSInfo<<"system close"<<END;
		roRes.set_errorcode(ERR_FAILED);
		return;
	}

	CCheckinRecord* pRecord = pRole->Get<CCheckinRecord>();
	if (NULL == pRecord)
	{
		SSWarn<<"check in record is NULL, role unique id:"<<pRole->GetID()<<END;
		roRes.set_errorcode(ERR_UNKNOWN);
		return;
	}
	pRecord->Update();

	///> 和正在天数偏移5小时
	time_t dwNowTime = GameTime::GetTime() - SECONDS_OFFSET;
	tm* stNowTime = localtime(&dwNowTime);
	UINT32 dwMonth = stNowTime->tm_mon;
	UINT32 dwMonthDay = stNowTime->tm_mday;

	CanCheckinInfo stInfo = CCheckinMgr::Instance()->GetCanCheckinInfo(pRole, dwNowTime);
	///> 判断是否还可以签到
	if (!CCheckinMgr::Instance()->IsCanCheckin(pRecord, dwMonthDay, stInfo.dwStartDay))
	{
		roRes.set_errorcode(ERR_CHECKIN_FULL);
		return;
	}
	///> 签到
	UINT32 dwBonus = 1;
	if (!CCheckinMgr::Instance()->OnCheckin(pRole, pRecord, dwMonth, dwMonthDay, dwBonus, stInfo.dwStartDay))
	{
		roRes.set_errorcode(ERR_CHECKIN_LACKDRAGONCOIN);
		return;
	}
	roRes.set_daycancheck(stInfo.dwCanCheckDay);
	roRes.set_startday(stInfo.dwStartDay);
	roRes.set_daycheckinfo(pRecord->GetCheckInfo());
	roRes.set_daymakeup(pRecord->GetCheckinCount());
	roRes.set_bonus(dwBonus);
	roRes.set_errorcode(ERR_SUCCESS);
}

void RpcC2G_Checkin::OnDelayReplyRpc(const CheckinArg &roArg, CheckinRes &roRes, const CUserData &roUserData)
{
}
