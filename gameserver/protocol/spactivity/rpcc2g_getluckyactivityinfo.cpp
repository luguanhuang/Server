#include "pch.h"
#include "unit/role.h"
#include "unit/rolemanager.h"
#include "gamelogic/rolemisc.h"
#include "spactivity/spactivitydef.h"
#include "spactivity/spactivitymgr.h"
#include "spactivity/spactivitysmall.h"
#include "spactivity/rpcc2g_getluckyactivityinfo.h"

// generate by ProtoGen at date: 2017/9/12 11:13:18

RPC_SERVER_IMPLEMETION(RpcC2G_GetLuckyActivityInfo, GetLuckyActivityInfoArg, GetLuckyActivityInfoRes)

void RpcC2G_GetLuckyActivityInfo::OnCall(const GetLuckyActivityInfoArg &roArg, GetLuckyActivityInfoRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		SSError << "Cannot Find Role, SessionID:" << m_sessionID << END;
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	SpActivityBase* pBase = SpActivityMgr::Instance()->GetSpActivity(SpActivity_LuckyTurntable);
	if (pBase == NULL)
	{
		roRes.set_errorcode(KKSG::ERR_ACT_NOT_OPEN);
		return;
	}
	KKSG::LuckyActivity turntable = pRole->Get<RoleMisc>()->GetLuckyActicityInfo();
	static_cast<SpActivityLuckyTurntable*>(pBase)->FillConsume(roRes, turntable.state());
	roRes.set_ispay(turntable.ispay());
	roRes.mutable_itemrecord()->CopyFrom(turntable.itemrecord());
}

void RpcC2G_GetLuckyActivityInfo::OnDelayReplyRpc(const GetLuckyActivityInfoArg &roArg, GetLuckyActivityInfoRes &roRes, const CUserData &roUserData)
{
}
