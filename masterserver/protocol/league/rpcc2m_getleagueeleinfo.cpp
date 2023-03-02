#include "pch.h"
#include "league/rpcc2m_getleagueeleinfo.h"
#include "leaguebattle/leagueeliminationmgr.h"
#include "leaguebattle/leaguecrosselimgr.h"
#include "leaguebattle/leagueseasonmgr.h"
#include "role/role.h"
#include "role/rolemanager.h"

// generate by ProtoGen at date: 2017/1/14 15:14:50

RPC_SERVER_IMPLEMETION(RpcC2M_GetLeagueEleInfo, GetLeagueEleInfoArg, GetLeagueEleInfoRes)

void RpcC2M_GetLeagueEleInfo::OnCall(const GetLeagueEleInfoArg &roArg, GetLeagueEleInfoRes &roRes)
{
	CRole* pRole = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (pRole == NULL)
	{
		roRes.set_result(KKSG::ERR_FAILED);
		return;
	}
	roRes.set_result(KKSG::ERR_SUCCESS);

	KKSG::LeagueEliType type = LeagueSeasonMgr::Instance()->GetEliType();
	// 本服淘汰赛
	if (type == LeagueEliType_Self)
	{
		auto& rounds = LeagueEliminationMgr::Instance()->GetRounds();
		for (auto it = rounds.begin(); it != rounds.end(); ++it)
		{
			LeagueEliminationMgr::Instance()->FillRoundInfo(&it->second, *roRes.add_rounds());
		}
		LeagueEliminationMgr::Instance()->FillChamptionInfo(*roRes.mutable_chamption());
	}
	// 跨服淘汰赛
	else if (type == LeagueEliType_Cross)
	{
		LeagueCrossEliMgr::Instance()->FillEliInfo(roRes);
	}
	else
	{
		LogError("Can't GetEliInfo");
	}

	LeagueEliminationMgr::Instance()->AddWatcher(pRole->GetID());
}

void RpcC2M_GetLeagueEleInfo::OnDelayReplyRpc(const GetLeagueEleInfoArg &roArg, GetLeagueEleInfoRes &roRes, const CUserData &roUserData)
{
}
