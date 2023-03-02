#include "pch.h"
#include "guild/rpcc2m_reqguildarenahistory.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "guild/guildarena.h"
#include "foreach.h"

// generate by ProtoGen at date: 2016/11/10 10:24:23

RPC_SERVER_IMPLEMETION(RpcC2M_ReqGuildArenaHistory, ReqGuildArenaHistoryRes, ReqGuildArenaHistoryRse)

void RpcC2M_ReqGuildArenaHistory::OnCall(const ReqGuildArenaHistoryRes &roArg, ReqGuildArenaHistoryRse &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == pRole)
	{
		return;
	}

	const std::map<UINT32, HistoryRecordData>& historyMap = CGuildArenaIntegralMgr::Instance()->GetHistory();
	foreach (i in historyMap)
	{
		KKSG::GuildArenaHistory* pData = roRes.add_history();
		pData->set_first(i->second.first);
		pData->set_second(i->second.second);
	}
}

void RpcC2M_ReqGuildArenaHistory::OnDelayReplyRpc(const ReqGuildArenaHistoryRes &roArg, ReqGuildArenaHistoryRse &roRes, const CUserData &roUserData)
{
}
