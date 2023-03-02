#include "pch.h"
#include "guild/rpcc2m_modifymsguildname.h"
#include "guild/rpcm2g_modifygsguildname.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "role/rolemanager.h"
#include "util/XCommon.h"
#include "network/gslink.h"
#include "mail/mailconfig.h"
#include "mail/mailmgr.h"
#include "tsssdk/MsTssHandler.h"


// generate by ProtoGen at date: 2017/4/11 22:30:35

RPC_SERVER_IMPLEMETION(RpcC2M_ModifyMsGuildName, ModifyArg, ModifyRes)

void RpcC2M_ModifyMsGuildName::OnCall(const ModifyArg &roArg, ModifyRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_error(KKSG::ERR_UNKNOWN);
		return;
	}
	int ret = XCommon::ValidNameCheck(roArg.name());
	if (ret != KKSG::ERR_SUCCESS)
	{
		roRes.set_error((KKSG::ErrorCode)ret);
		return;
	}
	KKSG::ErrorCode nErrCode = CTssHandler::Instance()->ValidNameCheck(roArg.name());
	if(nErrCode != KKSG::ERR_SUCCESS)
	{
		roRes.set_error((KKSG::ErrorCode)nErrCode);
		return;
	}

	if (!CGuildMgr::Instance()->IsNameNotUsed(roArg.name()))
	{
		roRes.set_error(KKSG::ERR_GUILD_NAME_EXIST);
		return;
	}
	Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(role->GetID());
	if (pGuild == NULL)
	{
		roRes.set_error(KKSG::ERR_GUILD_NOT_EXIST);
		return;
	}

	if (CGuildMgr::Instance()->IsLockName(roArg.name()))
	{
		roRes.set_error(KKSG::ERR_GUILD_LOCK_NAME);
		return;
	}
	CGuildMgr::Instance()->AddLockName(roArg.name());
	RpcM2G_ModifyGsGuildName *pRpc = RpcM2G_ModifyGsGuildName::CreateRpc();
	pRpc->m_oArg.set_rpcid(DelayRpc());
	pRpc->m_oArg.set_name(roArg.name());
	pRpc->m_oArg.set_roleid(role->GetID());
	GSLink::Instance()->SendToLine(role->GetGsLine(), *pRpc);

}

void RpcC2M_ModifyMsGuildName::OnDelayReplyRpc(const ModifyArg &roArg, ModifyRes &roRes, const CUserData &roUserData)
{
	UINT32 errorcode = roUserData.m_dwUserData;	
	if (errorcode == KKSG::ERR_SUCCESS)
	{
		UINT64 roleid = *(UINT64*)roUserData.m_pUserPtr;		
		Guild *pGuild = CGuildMgr::Instance()->GetGuildByRoleId(roleid);
		if (pGuild)
		{
			CGuildMgr::Instance()->ModifyName(pGuild,pGuild->GetName(),roArg.name());
			pGuild->SetName(roArg.name());
			const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_GuildModifyName);
			if (pConf!=NULL)
			{
				char szBuf[64] = {0};
				string szContent = pConf->m_content;
				string szTitle   = pConf->m_title;
				string szMailContent = XCommon::ReplaceStr(szContent, "%s", roArg.name());
				std::vector<ItemDesc> item;
				MailData mail( pConf->m_title, szMailContent, item, MAIL_EXPIRE_TIME);	
				pGuild->SendModifyNameMail(mail);
			}

		}else
		{
			LogError("ModifyGuildName %llu",roleid);
		}
	}
	roRes.set_error(KKSG::ErrorCode(errorcode));
	CGuildMgr::Instance()->UnlockName(roArg.name());

}
