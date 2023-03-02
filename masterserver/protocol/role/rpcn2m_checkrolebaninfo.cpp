#include "pch.h"
#include "role/rpcn2m_checkrolebaninfo.h"
#include "account/punishmgr.h"
#include "idip/accountbanmgr.h"
#include "mail/mailconfig.h"
#include "table/globalconfig.h"
#include "timeutil.h"

// generate by ProtoGen at date: 2016/11/9 10:17:54

RPC_SERVER_IMPLEMETION(RpcN2M_CheckRoleBanInfo, CheckRoleBanInfoArg, CheckRoleBanInfoRes)

void RpcN2M_CheckRoleBanInfo::OnCall(const CheckRoleBanInfoArg &roArg, CheckRoleBanInfoRes &roRes)
{
	if (CPunishMgr::Instance()->IsPunish(roArg.roleid(), KKSG::PUNISH_USER_TEMP))
	{
		roRes.set_result(KKSG::ERR_LOCKED_ROLE);
		return;
	}

	UINT32 nEndTime    = 0;
	std::string reason = "";
	if (AccountBanMsMgr::Instance()->IsOpenIDInBanList(roArg.openid(),nEndTime,reason))
	{
		roRes.set_result(KKSG::ERR_PLAT_BANACC);
		roRes.set_endtime(nEndTime);
		roRes.set_reason(reason);
		return;
	}

	KKSG::PunishData oData;
	if (CPunishMgr::Instance()->GetPunishInfo(roArg.roleid(), KKSG::PUNISH_USER_LOGIN, oData))
	{
		roRes.set_result(KKSG::ERR_LOGIN_FORBID);
		roRes.set_endtime(INT32(oData.punishdate()+oData.bantime()));
		roRes.set_bantime(oData.bantime());
		return;
	}

	if (CPunishMgr::Instance()->GetPunishInfo(roArg.roleid(), KKSG::PUNISH_USER_HG, oData))
	{
		roRes.set_result(KKSG::ERR_HG_FORBID);
		roRes.set_endtime(INT32(oData.punishdate()+oData.bantime()));
		roRes.set_reason(MailConfig::Instance()->ReplaceString("Z", ToString(TimeUtil::GetDateTimeStr(roRes.endtime())), GetGlobalConfig().LoginRemindMsg));
		return;
	}

	roRes.set_result(KKSG::ERR_SUCCESS);
}

void RpcN2M_CheckRoleBanInfo::OnDelayReplyRpc(const CheckRoleBanInfoArg &roArg, CheckRoleBanInfoRes &roRes, const CUserData &roUserData)
{
}
