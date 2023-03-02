#include "pch.h"
#include "skycraft/rpcc2m_getskycraftinfo.h"

#include "role/rolemanager.h"
#include "util/gametime.h"
#include "ranklist/ranklistmgr.h"
#include "ranklist/skycraftranklist.h"
#include "skyteam/skyteam.h"
#include "skyteam/skyteammgr.h"

// generate by ProtoGen at date: 2017/4/11 14:28:48

RPC_SERVER_IMPLEMETION(RpcC2M_GetSkyCraftInfo, GetSkyCraftInfoArg, GetSkyCraftInfoRes)

void RpcC2M_GetSkyCraftInfo::OnCall(const GetSkyCraftInfoArg &roArg, GetSkyCraftInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);
	roRes.set_rank(INVALID_RANK);
	roRes.set_stid(0);

	SkyTeam* pSTeam = SkyTeamMgr::Instance()->GetSkyTeamByRoleId(pRole->GetID());
	if (pSTeam)
	{
		roRes.set_stid(pSTeam->GetId());
		roRes.set_name(pSTeam->GetName());
		SkyTeamRecord& record = pSTeam->GetTeamRecord();
		roRes.set_total_num(record.GetTotalNum());
		roRes.set_winrate(record.GetTotalWinRate());
		roRes.set_today_num(record.GetTodayNum());

		UINT32 rank = CRankListMgr::Instance()->GetSkyCraftRankList()->GetRank(pSTeam->GetId());
		if (rank != INVALID_RANK) rank--;
		roRes.set_rank(rank);

		std::vector<SkyTeamMember>& members = pSTeam->GetAllMember();
		for (auto it = members.begin(); it != members.end(); ++it)
		{
			it->FillMemberInfo(*roRes.add_members());
		}
	}
}

void RpcC2M_GetSkyCraftInfo::OnDelayReplyRpc(const GetSkyCraftInfoArg &roArg, GetSkyCraftInfoRes &roRes, const CUserData &roUserData)
{
}
