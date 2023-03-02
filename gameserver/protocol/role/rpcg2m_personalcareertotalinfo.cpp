#include "pch.h"
#include "role/rpcg2m_personalcareertotalinfo.h"
#include "httptask/personalcareerinfo.h"
#include "httptask/personalcareerrank.h"
#include "httptask/httptaskmgr.h"
#include "unit/role.h"
#include "unit/rolemanager.h"

// generate by ProtoGen at date: 2017/2/3 10:19:18

RPC_CLIENT_IMPLEMETION(RpcG2M_PersonalCareerTotalInfo, PersonalCareerTotalInfoArg, PersonalCareerTotalInfoRes)

void RpcG2M_PersonalCareerTotalInfo::OnReply(const PersonalCareerTotalInfoArg &roArg, const PersonalCareerTotalInfoRes &roRes, const CUserData &roUserData)
{
	CUserData data;
	/*data.m_pUserPtr = (void*)(&roRes);
	CRpc::ReplyDelayRpc(roArg.rpc_id(), data);*/

	Role* role = RoleManager::Instance()->FindByRoleID(roArg.role_id());
	if (NULL == role)
	{
		CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
		return;
	}

	switch (roArg.quest_type())
	{
	case KKSG::PCRT_HOME_PAGE:
		{
			PersonalCarrerInfoTask* task = new PersonalCarrerInfoTask();			
			task->SetRPCID(roArg.rpc_id());
			task->SetServerID(role->GetServerId());
			task->SetRoleID(roArg.role_id());
			task->SetOpenID(role->GetAccount());
			task->SetServerName(roRes.server_name());
			HttpTaskMgr::Instance()->AddTask(task);
		}
		break;
	case KKSG::PCRT_PVP_PKINFO:
		{
			PersonalCarrerRankTask* task = new PersonalCarrerRankTask();			
			task->SetRPCID(roArg.rpc_id());
			task->SetServerID(role->GetServerId());
			task->SetRoleID(roArg.role_id());
			task->SetOpenID(role->GetAccount());
			task->SetPkRank(roRes.pk_rank());
			task->SetProfession(roRes.profession_rank());
			HttpTaskMgr::Instance()->AddTask(task);
		}
		break;
	default:
		{
			data.m_pUserPtr = (void*)(&roRes);
			CRpc::ReplyDelayRpc(roArg.rpc_id(), data);
		}
		break;
	}		
}

void RpcG2M_PersonalCareerTotalInfo::OnTimeout(const PersonalCareerTotalInfoArg &roArg, const CUserData &roUserData)
{
}
