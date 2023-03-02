#include "pch.h"
#include "task/rpcc2m_getdailytaskrefreshrecord.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "task/roletaskmgr.h"
#include "guild/guild.h"
#include "guild/guildmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "foreach.h"
#include "util/gametime.h"
#include "friend/friend.h"
#include "friend/friendmgr.h"

// generate by ProtoGen at date: 2017/9/18 1:41:28

RPC_SERVER_IMPLEMETION(RpcC2M_GetDailyTaskRefreshRecord, GetDailyTaskRefreshRecordArg, GetDailyTaskRefreshRecordRes)

void RpcC2M_GetDailyTaskRefreshRecord::OnCall(const GetDailyTaskRefreshRecordArg &roArg, GetDailyTaskRefreshRecordRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_INVALID_REQUEST);
		return;
	}
	CRoleTask* pRTask = RoleTaskMgr::Instance()->GetRoleTask(pRole->GetID());
	if (pRTask == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	for (auto it = pRTask->m_refreshRecord.begin(); it != pRTask->m_refreshRecord.end(); ++it)
	{
		CRoleSummary* pSu = CRoleSummaryMgr::Instance()->GetRoleSummary(it->m_roleId);
		if (pSu == NULL)
		{
			continue;
		}
		KKSG::DailyTaskRefreshInfo* pInfo = roRes.add_records();
		pInfo->set_roleid(it->m_roleId);
		pInfo->set_name(pSu->GetName());
		pInfo->set_isnew(it->m_isNew);
		pInfo->set_score(it->m_score);
		pInfo->set_time(it->m_time);
		pInfo->set_old_score(it->m_oldScore);
		it->m_isNew = false;
	}
}

void RpcC2M_GetDailyTaskRefreshRecord::OnDelayReplyRpc(const GetDailyTaskRefreshRecordArg &roArg, GetDailyTaskRefreshRecordRes &roRes, const CUserData &roUserData)
{
}
