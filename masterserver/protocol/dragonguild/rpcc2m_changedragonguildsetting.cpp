#include "pch.h"
#include "dragonguild/rpcc2m_changedragonguildsetting.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguilddef.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/ptcm2c_dragonguildsettingchanged.h"

// generate by ProtoGen at date: 2017/9/7 16:27:08

RPC_SERVER_IMPLEMETION(RpcC2M_ChangeDragonGuildSetting, ChangeDragonGuildSettingArg, ChangeDragonGuildSettingRes)

void RpcC2M_ChangeDragonGuildSetting::OnCall(const ChangeDragonGuildSettingArg &roArg, ChangeDragonGuildSettingRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	UINT64 gid = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID());
	if (gid == INVAILD_DRAGONGUILD)
	{
		roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		return;
	}

	DragonGuild *pGuild = DragonGuildMgr::Instance()->GetDragonGuild(gid);
	if (pGuild == NULL)
	{
		roRes.set_result(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	DragonGuildMember* m = pGuild->GetMember(pRole->GetID());
	if (roArg.has_annoucement())
	{
		if(!DragonGuildMgr::Instance()->HasPermission(m->position, DGEM_ANNOUNCEMENT))
		{
			roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
			return ;
		}

		pGuild->SetAnnoucement(roArg.annoucement());
		pGuild->SetDirty();
	}
	else
	{
		if(!DragonGuildMgr::Instance()->HasPermission(m->position, DGEM_SETTINGS))
		{
			roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
			return ;
		}

		if (roArg.has_powerpoint())
		{
			pGuild->SetRecuitPPT(roArg.powerpoint());
		}

		if (roArg.has_needapproval())
		{
			pGuild->SetNeedApproval(roArg.needapproval());
		}
		pGuild->SetDirty();
	}
	roRes.set_result(KKSG::ERR_SUCCESS);

	pGuild->SendDragonGuildBaseInfo(pRole);
	pGuild->SendDragonGuildSettinfInfoToAll();
}

void RpcC2M_ChangeDragonGuildSetting::OnDelayReplyRpc(const ChangeDragonGuildSettingArg &roArg, ChangeDragonGuildSettingRes &roRes, const CUserData &roUserData)
{
}
