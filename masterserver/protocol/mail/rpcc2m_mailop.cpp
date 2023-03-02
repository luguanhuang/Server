#include "pch.h"
#include "mail/rpcc2m_mailop.h"
#include "role/rolemanager.h"
#include "mail/mailmgr.h"
#include "define/systemiddef.h"
#include "table/OpenSystemMgr.h"

// generate by ProtoGen at date: 2016/7/22 2:06:22

RPC_SERVER_IMPLEMETION(RpcC2M_MailOp, MailOpArg, MailOpRes)


static bool HandleMail(INT32 op, CRole* poRole, UINT64 qwMailID)
{
	switch (op)
	{
	case Mail_Read:
		{
			return CMailMgr::Instance()->Read(poRole->GetID(), qwMailID);
		}
	case Mail_Delete:
		{
			return CMailMgr::Instance()->Delete(poRole->GetID(), qwMailID);
		}
	default:
		LogError("Role %llu send invalid mail op type %d", poRole->GetID(), op);
		break;
	}

	return false;
}


void RpcC2M_MailOp::OnCall(const MailOpArg &roArg, MailOpRes &roRes)
{
	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		roRes.set_errorcode(ERR_INVALID_REQUEST);
		return;
	}
	/////> system open
	//if (OpenSystemMgr::Instance()->IsIdipCloseSystem(SysMailId))
	//{
	//	SSInfo<<"mail system close"<<END;
	//	return;
	//}

	if (!poRole->IsSystemOpened(SysMailId))
	{
		SSInfo<<"mail system close"<<END;
		roRes.set_errorcode(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	switch (roArg.optype())
	{
	case Mail_Reward:
		{
			if(roArg.uid_size())
			{
				UINT32 delayid = DelayRpc();
				if (!CMailMgr::Instance()->Reward(poRole->GetID(), roArg.uid(0), poRole, delayid))
				{
					CUserData data;
					data.m_dwUserData = (UINT32)KKSG::ERR_UNKNOWN;
					CRpc::ReplyDelayRpc(delayid, data);
				}
			}
			return;
		}
	case Mail_RewardAll:
		{
			UINT32 delayid = DelayRpc();
			if (!CMailMgr::Instance()->RewardAll(poRole->GetID(), poRole, delayid))
			{
				CUserData data;
				data.m_dwUserData = (UINT32)KKSG::ERR_NOMAIL_GETREWARD;
				CRpc::ReplyDelayRpc(delayid, data);
			}
			return;
		}
	default:
		{
			break;
		}
	}

	for(int i = 0; i < roArg.uid_size(); ++i)
	{
		UINT64 uid = roArg.uid(i);
		if(!HandleMail(roArg.optype(), poRole, uid))
		{
			continue;
		}
	}

	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void RpcC2M_MailOp::OnDelayReplyRpc(const MailOpArg &roArg, MailOpRes &roRes, const CUserData &roUserData)
{
	roRes.set_errorcode((KKSG::ErrorCode)roUserData.m_dwUserData);
}
