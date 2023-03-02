#include "pch.h"
#include "dragonguild/rpcc2m_leavefromdragonguild.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "dragonguild/dragonguild.h"
#include "dragonguild/dragonguilddef.h"
#include "dragonguild/dragonguildmgr.h"
#include "mail/mailmgr.h"
#include "mail/mailconfig.h"
#include "util/XCommon.h"
#include "loghelper/tlogr.h"

// generate by ProtoGen at date: 2017/9/7 17:24:34

RPC_SERVER_IMPLEMETION(RpcC2M_LeaveFromDragonGuild, LeaveDragonGuildArg, LeaveDragonGuildRes)

void RpcC2M_LeaveFromDragonGuild::OnCall(const LeaveDragonGuildArg &roArg, LeaveDragonGuildRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		SSWarn<<"pRole is NULL, session id:"<<m_sessionID<<END;
		return;
	}

	// 踢人 
	if (roArg.has_roleid() && roArg.roleid() != pRole->GetID())
	{
		UINT64 dgid = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID());
		if (dgid == INVAILD_DRAGONGUILD)
		{
			roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
			return ;
		}

		DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuild(dgid);
		if (NULL == pDGuild)
		{
			roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
			return;
		}

		if (!DragonGuildMgr::Instance()->IsInsameDragonGuild(roArg.roleid(), pRole->GetID()))
		{
			roRes.set_result(KKSG::ERR_DG_NOT_IN_SAME);
			return ;
		}

		DragonGuildMember* pMember = pDGuild->GetMember(pRole->GetID());
		DragonGuildMember* pOther = pDGuild->GetMember(roArg.roleid());

		if (pMember->position > pOther->position)
		{
			roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
			return;
		}

		if (!DragonGuildMgr::Instance()->HasPermission(pMember->position, DGEM_FIREMEMBER))
		{
			roRes.set_result(KKSG::ERR_DG_NO_PERMISSION);
			return;
		}
		
		pDGuild->DoLeaveMember(roArg.roleid());

		const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_DragonGuileBeFired);
		if (pConf!=NULL)
		{
			char szBuf[64] = {0};
			string szContent = pConf->m_content;
			string szTitle   = pConf->m_title;
			string szMailContent = XCommon::ReplaceStr(szContent, "%s", pDGuild->GetName());
			std::vector<ItemDesc> item;
			MailData mail( pConf->m_title, szMailContent, item, MAIL_EXPIRE_TIME);	
			CMailMgr::Instance()->SendMail(roArg.roleid(), mail);
		}

		roRes.set_result(KKSG::ERR_SUCCESS);

		TDragonGuildFlowLog oLog(TLogMgr::Instance()->GetTagString());
		pDGuild->DoTxLog(oLog);
		oLog.m_iActType = TX_Dragon_GUILD_ACT_TYPE_8;
		oLog.m_uRoleID = roArg.roleid();	// 被踢
		oLog.m_RoleIDT = pRole->GetID();
		oLog.Do();
	}
	else	// 离开
	{
		UINT64 dgid = DragonGuildMgr::Instance()->GetDragonGuildIdByRole(pRole->GetID());
		if (dgid == INVAILD_DRAGONGUILD)
		{
			roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
			return ;
		}

		DragonGuild* pDGuild = DragonGuildMgr::Instance()->GetDragonGuild(dgid);
		if (NULL == pDGuild)
		{
			roRes.set_result(KKSG::ERR_DG_DGUILD_NOT_EXIST);
			return;
		}

		DragonGuildMember* m = pDGuild->GetMember(pRole->GetID());
		if (m)
		{
			if (m->position == DGPOS_LEADER)	// 解散
			{
				DragonGuildMgr::Instance()->Dismiss(pDGuild->GetID());
			}
			else								// 离开
			{
				pDGuild->DoLeaveMember(pRole->GetID());
			}			
		}
		else
		{
			roRes.set_result(KKSG::ERR_DG_NOT_IN_GUILD);
		}
	}
}

void RpcC2M_LeaveFromDragonGuild::OnDelayReplyRpc(const LeaveDragonGuildArg &roArg, LeaveDragonGuildRes &roRes, const CUserData &roUserData)
{
}
