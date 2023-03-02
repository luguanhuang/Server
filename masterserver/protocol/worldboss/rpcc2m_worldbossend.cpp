#include "pch.h"
#include "worldboss/rpcc2m_worldbossend.h"
#include "ranklist/worldbossranklist.h"
#include "role/role.h"
#include "role/rolemanager.h"
#include "ranklist/ranklistmgr.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"

// generate by ProtoGen at date: 2016/9/3 21:08:42

RPC_SERVER_IMPLEMETION(RpcC2M_WorldBossEnd, WorldBossEndArg, WorldBossEndRes)

#define RANKCOUNT 20

void RpcC2M_WorldBossEnd::OnCall(const WorldBossEndArg &roArg, WorldBossEndRes &roRes)
{
	CRole* role = CRoleManager::Instance()->GetBySession(m_sessionID);
	if (NULL == role)
	{
		return;
	}
	BossDamageRankList* ranklist = (BossDamageRankList*)CRankListMgr::Instance()->GetRankList(KKSG::WorldBossDamageRank);
	if (NULL == ranklist)
	{
		return;
	}
	UINT32 rank = ranklist->GetRank(role->GetID());
	BossDamageRankInfo* rankinfo = (BossDamageRankInfo*)ranklist->GetInfoByRank(rank);
	if (NULL != rankinfo)
	{
		KKSG::WorldBossDamageInfo* info = roRes.mutable_selfdamage();
		info->set_rank((INVALID_RANK == rank) ? INVALID_RANK : (rank - 1));
		info->set_damage(rankinfo->m_damage);
		info->set_rolename(role->GetName());
	}

	std::vector<BossDamageRankInfo*> rankinfos;
	ranklist->GetSomeRankInfo(RANKCOUNT, rankinfos);
	UINT32 count = 0;
	for (auto i = rankinfos.begin(); i != rankinfos.end(); ++i)
	{
		KKSG::WorldBossDamageInfo* info = roRes.add_damages();
		info->set_rank(count);
		info->set_damage((*i)->m_damage);
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary((*i)->GetUniqueId());
		if (NULL != summary)
		{
			info->set_rolename(summary->GetName());
		}
		++count;
	}
}

void RpcC2M_WorldBossEnd::OnDelayReplyRpc(const WorldBossEndArg &roArg, WorldBossEndRes &roRes, const CUserData &roUserData)
{
}
