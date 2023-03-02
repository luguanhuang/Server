#include "pch.h"
#include "qqvip/rpcc2g_queryqqfriendsvipinfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "httptask/httptaskmgr.h"
#include "httptask/queryqqviprichtask.h"
#include "httptask/queryqqfriendsviptask.h"
#include "gamelogic/qqviprecord.h"
#include "unit/systeminfo.h"
#include "define/systemiddef.h"


// generate by ProtoGen at date: 2016/12/4 20:26:56

RPC_SERVER_IMPLEMETION(RpcC2G_QueryQQFriendsVipInfo, QueryQQFriendsVipInfoArg, QueryQQFriendsVipInfoRes)

void RpcC2G_QueryQQFriendsVipInfo::OnCall(const QueryQQFriendsVipInfoArg &roArg, QueryQQFriendsVipInfoRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	// 系统开放
	if (!role->Get<CRoleSystem>()->IsSystemOpened(SYS_QQVIP))
	{
		roRes.set_result(KKSG::ERR_SYS_NOTOPEN);
		return;
	}

	QueryQQFriendsVipTask* task = new QueryQQFriendsVipTask();
	task->SetRpcID(DelayRpc());
	task->SetOpenId(role->GetAccount());
	task->SetToken(roArg.token());
	task->SetPF(role->GetClientConf().pf());
	SSWarn << role->GetClientConf().ip() << END;
	for (int i = 0; i < roArg.friendopenids_size(); ++i)
	{
		task->AddFriendOpenId(roArg.friendopenids(i));
	}

	task->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(task);
}

void RpcC2G_QueryQQFriendsVipInfo::OnDelayReplyRpc(const QueryQQFriendsVipInfoArg &roArg, QueryQQFriendsVipInfoRes &roRes, const CUserData &roUserData)
{
	roRes.set_result(KKSG::ERR_SUCCESS);
	if (roUserData.m_pUserPtr == NULL)
	{
		return;
	}
	// vip信息
	std::vector<KKSG::FriendVipInfo>& infos = *((std::vector<KKSG::FriendVipInfo>*)roUserData.m_pUserPtr);
	for (auto it = infos.begin(); it != infos.end(); ++it)
	{
		KKSG::FriendVipInfo* info = roRes.add_info();
		info->CopyFrom(*it);
	}
}
