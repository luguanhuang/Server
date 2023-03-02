#include "pch.h"
#include "title/rpcc2g_titlelevelup.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "gamelogic/titlerecord.h"
#include "unit/roleoutlook.h"
#include "gamelogic/rolesummary.h"
#include "utility/tlogger.h"

// generate by ProtoGen at date: 2016/8/23 18:02:35

RPC_SERVER_IMPLEMETION(RpcC2G_TitleLevelUp, TitleLevelUpArg, TitleLevelUpRes)

void RpcC2G_TitleLevelUp::OnCall(const TitleLevelUpArg &roArg, TitleLevelUpRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_errorcode(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	TitleRecord* pTitleRecord = pRole->Get<TitleRecord>();
	KKSG::ErrorCode errorCode = pTitleRecord->TitleLevelUp();
	if (errorCode == ERR_SUCCESS)
	{
		TTitleFlow oLog(pRole);
		oLog.Do();
	}
	roRes.set_errorcode(errorCode);
	roRes.set_titleid(pTitleRecord->GetTitle());
}

void RpcC2G_TitleLevelUp::OnDelayReplyRpc(const TitleLevelUpArg &roArg, TitleLevelUpRes &roRes, const CUserData &roUserData)
{
}
