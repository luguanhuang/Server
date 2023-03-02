#include "pch.h"
#include "spactivity/rpcc2g_themeactivityhint.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "define/spactivityiddef.h"
#include "spactivity/spactivitydatasmall.h"

// generate by ProtoGen at date: 2017/8/1 19:22:46

RPC_SERVER_IMPLEMETION(RpcC2G_ThemeActivityHint, ThemeActivityHintArg, ThemeActivityHintRes)

void RpcC2G_ThemeActivityHint::OnCall(const ThemeActivityHintArg &roArg, ThemeActivityHintRes &roRes)
{
	Role *pRole = RoleManager::Instance()->FindBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_UNKNOWN);
	if (roArg.actid() != SpActivity_BioHell && roArg.actid() != SpActivity_Duck)
	{
		SSWarn << "invalid actid=" << roArg.actid() << END;
		return;
	}
	SpActThemeData* themeData = static_cast<SpActThemeData*>(pRole->Get<SpActivityRecord>()->GetSpActData(roArg.actid()));
	if (NULL == themeData)
	{
		return;
	}
	themeData->SetHint(false);
	roRes.set_result(KKSG::ERR_SUCCESS);

}

void RpcC2G_ThemeActivityHint::OnDelayReplyRpc(const ThemeActivityHintArg &roArg, ThemeActivityHintRes &roRes, const CUserData &roUserData)
{
}
