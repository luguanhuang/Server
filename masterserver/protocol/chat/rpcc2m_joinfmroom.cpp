#include "pch.h"
#include "chat/rpcc2m_joinfmroom.h"
#include "chat/rpcm2f_upjoinfmroom.h"
#include "network/fmlink.h"
#include "role/rolemanager.h"
#include "chat/fmmgr.h"
#include "loghelper/tlogr.h"
// generate by ProtoGen at date: 2016/11/23 17:44:59

RPC_SERVER_IMPLEMETION(RpcC2M_JoinFmRoom, JoinLargeRoomArg, JoinLargeRoomRes)

void RpcC2M_JoinFmRoom::OnCall(const JoinLargeRoomArg &roArg, JoinLargeRoomRes &roRes)
{
	CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if(NULL == poRole)
	{
		roRes.set_error(ERR_UNKNOWN);
		return;
	}
	if(FMMgr::Instance()->GetAudioSize()||FMMgr::Instance()->IsAnchor(poRole->GetAccount()))
	{
		RpcM2F_UPJoinFmRoom* pRpc = RpcM2F_UPJoinFmRoom::CreateRpc();
		pRpc->m_oArg.set_param(DelayRpc());
		pRpc->m_oArg.set_roleid(poRole->GetID());
		pRpc->m_oArg.set_plat(1);
		pRpc->m_oArg.set_ip(poRole->GetIP());
		pRpc->m_oArg.set_acc(poRole->GetAccount());
		FMLink::Instance()->SendTo(*pRpc);
	}else
	{
		roRes.set_error(ERR_FM_NOANCHOR);
	}
}

void RpcC2M_JoinFmRoom::OnDelayReplyRpc(const JoinLargeRoomArg &roArg, JoinLargeRoomRes &roRes, const CUserData &roUserData)
{
	if (roUserData.m_dwUserData == 0 && roUserData.m_pUserPtr)
	{
		KKSG::JoinLargeRoomRes* pInfo = (KKSG::JoinLargeRoomRes*)roUserData.m_pUserPtr;
		roRes.CopyFrom(*pInfo);
		CRole* poRole = CRoleManager::Instance()->GetBySession(m_sessionID);
		if(poRole)
		{
			FMMgr::Instance()->AddRole(poRole->GetID());
			FMMgr::Instance()->SendWhiteList(poRole);
			FMMgr::Instance()->DoLog(poRole->GetAccount(),poRole->GetID(), 0);
		}
	}else
	{
		roRes.set_error(KKSG::ERR_FM_NOANCHOR);
	}
}
