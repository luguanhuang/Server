#include "pch.h"
#include "photo/rpcg2m_getcharm.h"
#include "social/social.h"
#include "social/socialmgr.h"
#include "role/rolemanager.h"
#include "role/role.h"
// generate by ProtoGen at date: 2016/12/15 9:00:10

RPC_SERVER_IMPLEMETION(RpcG2M_GetCharm, GetCharmArg, GetCharmRes)

	void RpcG2M_GetCharm::OnCall(const GetCharmArg &roArg, GetCharmRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetByRoleID(roArg.role_id());
	if (NULL == role)
	{
		SSWarn<<"role is NULL, sessionid:"<<roArg.role_id()<<END;
		return;
	}
	SocialInfo*  info = SocialMgr::Instance()->GetSocialInfo(role->GetID());
	if (NULL != info )
	{
		// 历史的+今天的
		UINT32 charm = info->flower.GetTotalReceiveCount();
		charm += info->flower.GetTodayReceiveCount();
		roRes.set_charm(charm);
	}
}

void RpcG2M_GetCharm::OnDelayReplyRpc(const GetCharmArg &roArg, GetCharmRes &roRes, const CUserData &roUserData)
{
}
