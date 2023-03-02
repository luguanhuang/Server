#include "pch.h"
#include "dragonguild/rpcc2m_modifydragonguildname.h"
#include "dragonguild/rpcm2g_modifydragonguildnamecost.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguildmgr.h"
#include "dragonguild/dragonguilddef.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "mail/mailmgr.h"
#include "util/XCommon.h"
#include "tsssdk/MsTssHandler.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"

// generate by ProtoGen at date: 2017/9/8 10:48:17

RPC_SERVER_IMPLEMETION(RpcC2M_ModifyDragonGuildName, ModifyDragonGuildNameArg, ModifyDragonGuildNameRes)

void RpcC2M_ModifyDragonGuildName::OnCall(const ModifyDragonGuildNameArg &roArg, ModifyDragonGuildNameRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_UNKNOWN);
		return;
	}

	int ret = XCommon::ValidNameCheck(roArg.name());
	if (ret != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)ret);
		return;
	}

	KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(roArg.name());
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_result((KKSG::ErrorCode)nErrCode);
		return;
	}

	if (!DragonGuildMgr::Instance()->IsNameNotUsed(roArg.name()))
	{
		roRes.set_result(KKSG::ERR_GUILD_NAME_EXIST);
		return;
	}

	DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(role->GetID());
	if (NULL == pDGuild)
	{
		roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
		return ;
	}

	if (DragonGuildMgr::Instance()->IsLockName(roArg.name()))
	{
		roRes.set_result(KKSG::ERR_DG_LOCK_NAME);
		return ;
	}
	DragonGuildMgr::Instance()->AddLockName(roArg.name());

	RpcM2G_ModifyDragonGuildNameCost *rpc = RpcM2G_ModifyDragonGuildNameCost::CreateRpc();
	rpc->m_oArg.set_roleid(role->GetID());
	rpc->m_oArg.set_name(roArg.name());
	rpc->m_oArg.set_rpcid(DelayRpc());
	GSLink::Instance()->SendToLine(role->GetGsLine(), *rpc);
}

void RpcC2M_ModifyDragonGuildName::OnDelayReplyRpc(const ModifyDragonGuildNameArg &roArg, ModifyDragonGuildNameRes &roRes, const CUserData &roUserData)
{
	UINT32 errorcode = roUserData.m_dwUserData;
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		UINT64 roleid = *(UINT64 *)roUserData.m_pUserPtr;
		DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuildByRole(roleid);
		if (pDGuild)
		{
			DragonGuildMgr::Instance()->ModifyName(pDGuild, pDGuild->GetName(), roArg.name());
			pDGuild->SetName(roArg.name());

			const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_DragonGuildModifyName);
			if (pConf!=NULL)
			{
				string szContent = pConf->m_content;
				string szTitle   = pConf->m_title;
				string szMailContent = XCommon::ReplaceStr(szContent, "%s", roArg.name());
				std::vector<ItemDesc> item;
				MailData mail( pConf->m_title, szMailContent, item, MAIL_EXPIRE_TIME);	
				pDGuild->BroadCastMail(mail);
			}
		}
		else
		{
			LogError("ModifyName %llu", roleid);
		}
	}

	roRes.set_result(KKSG::ErrorCode(errorcode));
	DragonGuildMgr::Instance()->UnlockName(roArg.name());
}
