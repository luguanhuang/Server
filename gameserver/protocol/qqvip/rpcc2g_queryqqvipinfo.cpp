#include "pch.h"
#include "qqvip/rpcc2g_queryqqvipinfo.h"
#include "unit/rolemanager.h"
#include "unit/role.h"
#include "httptask/httptaskmgr.h"
#include "httptask/queryqqviprichtask.h"
#include "httptask/queryqqviptask.h"
#include "gamelogic/qqviprecord.h"
#include "unit/systeminfo.h"
#include "define/systemiddef.h"

// generate by ProtoGen at date: 2016/11/20 22:23:12

RPC_SERVER_IMPLEMETION(RpcC2G_QueryQQVipInfo, QueryQQVipInfoArg, QueryQQVipInfoRes)

void RpcC2G_QueryQQVipInfo::OnCall(const QueryQQVipInfoArg &roArg, QueryQQVipInfoRes &roRes)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	if (role->GetLoginType() != KKSG::LOGIN_QQ_PF)
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

	QueryQQVipRichTask* task = new QueryQQVipRichTask();
	task->SetRpcID(DelayRpc());
	task->SetOpenId(role->GetAccount());
	task->SetToken(roArg.token());
	task->SetRoleId(role->GetID());
	task->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(task);

	// 顺便查询心悦等级
	QueryQQVipTask* pVipTask = new QueryQQVipTask();
	pVipTask->SetOpenId(role->GetAccount());
	pVipTask->SetToken(roArg.token());
	pVipTask->SetRoleId(role->GetID());
	pVipTask->BuildPostData();
	HttpTaskMgr::Instance()->AddTask(pVipTask);
}

void RpcC2G_QueryQQVipInfo::OnDelayReplyRpc(const QueryQQVipInfoArg &roArg, QueryQQVipInfoRes &roRes, const CUserData &roUserData)
{
	Role* role = RoleManager::Instance()->FindBySession(m_sessionID);
	if (NULL == role)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	QQVipRecord* vipRecord = role->Get<QQVipRecord>();
	vipRecord->FillClientInfo(*roRes.mutable_info());
}

