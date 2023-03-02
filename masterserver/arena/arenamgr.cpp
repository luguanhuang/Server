#include "pch.h"
#include "arenamgr.h"
#include "ranklist/ranklist.h"
#include "ranklist/arenaranklist.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "table/globalconfig.h"
#include "arena/ptcm2g_syncarenags.h"
#include "network/gslink.h"
#include "role/role.h"
#include "mail/mailmgr.h"

INSTANCE_SINGLETON(ArenaMgr);

ArenaMgr::ArenaMgr()
{

}

ArenaMgr::~ArenaMgr()
{

}

bool ArenaMgr::Init()
{
	if (!CheckFile())
	{
		SSWarn<<"Arena Init Failed"<<END;
		return false;
	}
	ClearFile();
	return LoadFile();
}

void ArenaMgr::Uninit()
{
	ClearFile();
}

bool ArenaMgr::CheckFile()
{
	if (!m_table.LoadFile("table/Arena.txt"))
	{
		SSWarn<<"Load table table/Arena.txt failed!"<<END;
		return false;
	}
	if (!m_npctable.LoadFile("serveronly/ArenaNpc.txt"))
	{
		SSWarn<<"Load table serveronly/ArenaNpc.txt failed!"<<END;
		return false;
	}
	return true;
}

bool ArenaMgr::LoadFile()
{
	m_table.LoadFile("table/Arena.txt");
	m_npctable.LoadFile("serveronly/ArenaNpc.txt");

	const MailConf* conf = MailConfig::Instance()->GetMailConf(MailConf_ArenaRankReward);
	if (NULL != conf)
	{
		m_mailconf = *conf;
	}
	return true;
}

void ArenaMgr::ClearFile()
{
	m_table.Clear();
	m_npctable.Clear();
}

bool ArenaMgr::InvalidRank(UINT32 rank)
{
	if (INVALID_RANK == rank || 0 == rank || m_npctable.Count() < rank)
	{
		return true;
	}
	return false;
}

UINT32 ArenaMgr::GetRankFirstTime(CRole* role)
{
	if (NULL == m_ranklist)
	{
		return INVALID_RANK;
	}
	UINT64 roleid = role->GetID();
	UINT32 rank = m_ranklist->GetRank(roleid);
	if (INVALID_RANK == rank)
	{
		rank = m_ranklist->GetNextEmptyPos(roleid);
		PtcM2G_SyncArenaGs sync;
		sync.m_Data.set_roleid(roleid);
		KKSG::SyncAreanOptimalRank* optimalrank = sync.m_Data.mutable_optimalrank();
		optimalrank->set_rank(rank);
		GSLink::Instance()->SendToLine(role->GetGsLine(), sync);	
	}
	return rank;
}

void ArenaMgr::GetCanBeChallenged(UINT32 count, UINT32 rank, std::vector<UINT32>& ranks)
{
	if (InvalidRank(rank))
	{
		rank = ARENANPC_COUNT;
	}
	const ArenaTable::RowData* pRowData = GetRowData(rank);
	if (NULL == pRowData)
	{
		return;
	}
	if (rank <= count)
	{
		for (UINT32 i = 1; i <= count + 1; ++i)
		{
			if (rank != i)
			{
				ranks.push_back(i);	
			}
		}
		return;
	}
	UINT32 confstart = pRowData->ArenaRank[0];
	UINT32 confend = pRowData->ArenaRank[1];
	UINT32 confcanstart = pRowData->CanAttack[0];
	UINT32 confcanend = pRowData->CanAttack[1];
	float param = 0.0f;
	if (confend != confstart)
	{
		param = (float)(rank - confstart)/(float)(confend - confstart);
	}
	UINT32 dwStart = (UINT32)(param*(confcanend - confcanstart)) + confcanstart;
	UINT32 dwLength = rank - dwStart;
	if (dwLength < count)
	{
		dwLength = count;
	}
	bool canattack = pRowData->canattackrole;
	for (UINT32 i = 0; i < count + 1; ++i)
	{
		UINT32 rankbegin = dwStart + (UINT32)(((float)i/(float)count)*dwLength); 
		UINT32 rankend = dwStart + (UINT32)(((float)(i+1)/(float)count)*dwLength); 
		if (!canattack)
		{
			rankbegin = GetExceptRole(rankbegin, rankend);
		}
		if(rank == rankbegin)
		{
			///> 最多执行一次
			continue;
		}
		ranks.push_back(rankbegin);

		if(ranks.size() >= count)
		{
			break;
		}
	}
}

bool ArenaMgr::GetBriefInfo(UINT64 roleid, KKSG::UnitAppearance* unitAppearance)
{
	if (0 == roleid)
	{
		SSWarn<<"challenged role id:0"<<END;
		return false;
	}
	if (IsArenaNpc(roleid))
	{
		const ArenaNpcInfo* pNpcInfo = GetArenaNpcInfo(roleid);
		if (NULL == pNpcInfo)
		{
			SSWarn<<"pNpc Info is NULL, npc id:"<<roleid<<END;
			return false;
		}
		UINT32 dwRank = GetRank(roleid);
		UINT32 dwPowerPoint = GetNpcBattlePoint(dwRank);
		UINT32 dwLevel = GetNpcBattleLevel(dwRank);
		UINT32 dwEnemyId = GetNpcIdByProfession(pNpcInfo->dwNpcProfession);

		unitAppearance->set_uid(roleid);
		unitAppearance->set_unittype(dwEnemyId);
		unitAppearance->set_unitname(pNpcInfo->szNpcName);
		unitAppearance->set_level(dwLevel);
		unitAppearance->set_powerpoint(dwPowerPoint);
		unitAppearance->set_category(KKSG::Category_Enemy);

		return true;
	}
	else
	{
		CRoleSummary* pRoleSummary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
		if (NULL == pRoleSummary)
		{
			SSWarn<<"pRoleSummary is NULL, role id:"<<roleid<<END;
			return false;
		}
		pRoleSummary->Build(unitAppearance, SUMMARY_OUTLOOK);
		return true;
	}

	return true;
}

void ArenaMgr::GetBattleInfo(UINT64 roleid, UINT64 challengeid, KKSG::ArenaBattleInfo* info)
{
	if (0 == challengeid)
	{
		SSWarn<<"challenged role id:0"<<END;
		return;
	}
	if (IsArenaNpc(challengeid))
	{
		const ArenaNpcInfo* pNpcInfo = GetArenaNpcInfo(challengeid);
		if (NULL == pNpcInfo)
		{
			SSWarn<<"pNpc Info is NULL, npc id:"<<challengeid<<END;
			return;
		}
		info->set_isnpc(true);

		UINT32 dwEnemyId = GetNpcIdByProfession(pNpcInfo->dwNpcProfession);
		info->set_challengid(challengeid);
		info->set_enemyid(dwEnemyId);
		info->set_name(pNpcInfo->szNpcName);
		info->set_profession(pNpcInfo->dwNpcProfession);

		UINT32 dwRank = GetRank(challengeid);
		UINT32 dwLevel = GetNpcBattleLevel(dwRank);
		info->set_level(dwLevel);

		UINT32 powerpoint = GetNpcBattlePoint(dwRank);
		info->set_ppt(powerpoint);
	}
	else
	{
		info->set_isnpc(false);
		info->set_challengid(challengeid);
	}
	info->set_recommendppt(GetRecommendPower(challengeid));
}

bool ArenaMgr::IsArenaNpc(UINT64 uid)
{
	return m_npctable.InTable(uid);
}

const ArenaNpcInfo* ArenaMgr::GetArenaNpcInfo(UINT64 uid)
{
	return m_npctable.GetInfo(uid);
}

UINT32 ArenaMgr::GetNpcBattleLevel(UINT32 rank)
{
	if (InvalidRank(rank))
	{
		return 0;
	}
	const ArenaTable::RowData* data = GetRowData(rank);
	if (NULL == data)
	{
		return 0;
	}
	float coef = GetCoef(rank, data);
	return GetCoefResult(coef, data->ArenaNpcLevel[0], data->ArenaNpcLevel[1]); 
}

float ArenaMgr::GetNpcBattleCoefByPPT(UINT32 powerpoint)
{
	float scale = 1.0f;
	for (auto i = m_table.Table.begin(); i != m_table.Table.end(); ++i)
	{
		ArenaTable::RowData* rowdata = *i;
		if (powerpoint >= rowdata->OccupyNpcPowerPoint[1] && powerpoint <= rowdata->OccupyNpcPowerPoint[0])
		{
			float coef = 1.0f;
			if (rowdata->OccupyNpcPowerPoint[1] != rowdata->OccupyNpcPowerPoint[0])
			{
				coef = (float)(rowdata->OccupyNpcPowerPoint[0] - powerpoint)/(float)(rowdata->OccupyNpcPowerPoint[0] - rowdata->OccupyNpcPowerPoint[1]);
			}
			scale = (float)rowdata->OccupyNpcRatio[1] + (float)((rowdata->OccupyNpcRatio[0] - rowdata->OccupyNpcRatio[1])*coef);
			break;
		}
	}
	return scale;
}

UINT32 ArenaMgr::GetNpcBattlePoint(UINT32 dwRank)
{
	if (InvalidRank(dwRank))
	{
		return 0;
	}
	const ArenaTable::RowData* pRowData = GetRowData(dwRank);
	if (NULL == pRowData)
	{
		return 0;
	}
	float fCoef = GetCoef(dwRank, pRowData);
	return pRowData->OccupyNpcPowerPoint[1] + (UINT32)((pRowData->OccupyNpcPowerPoint[0] - pRowData->OccupyNpcPowerPoint[1])*fCoef); 
}

UINT32 ArenaMgr::GetNpcIdByProfession(UINT32 dwProfession)
{
	for (auto i = GetGlobalConfig().ArenaFighters.begin(); i != GetGlobalConfig().ArenaFighters.end(); ++i)
	{
		if (dwProfession == (UINT32)(*i)[0])
		{
			return (UINT32)(*i)[1];
		}
	}
	return 0;
}

const ArenaTable::RowData* ArenaMgr::GetRowData(UINT32 rank)
{
	for (auto i = m_table.Table.rbegin(); i != m_table.Table.rend(); ++i)
	{
		ArenaTable::RowData* data = *i;
		if (rank >= data->ArenaRank[0] && rank <= data->ArenaRank[1])
		{
			return data;
		}
	}
	return NULL;
}

float ArenaMgr::GetCoef(UINT32 rank, const ArenaTable::RowData* data)
{
	float coef = 1.0f;
	if (data->ArenaRank[1] != data->ArenaRank[0])
	{
		coef = (float)(data->ArenaRank[1] - rank)/(float)(data->ArenaRank[1] - data->ArenaRank[0]);
	}
	return coef;
}

UINT32 ArenaMgr::GetCoefResult(float coef, UINT32 begin, UINT32 end)
{
	return begin + (UINT32)((begin - end)*coef);
}

UINT32 ArenaMgr::GetRank(UINT64 roleid)
{
	if (NULL == m_ranklist)	
	{
		return INVALID_RANK;
	}
	return m_ranklist->GetRank(roleid);
}

UINT64 ArenaMgr::GetRoleId(UINT32 rank)
{
	return m_ranklist->GetRoleId(rank);
}

UINT32 ArenaMgr::GetRecommendPower(UINT64 challengid)
{
	if (!IsArenaNpc(challengid))
	{
		CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(challengid);
		return (NULL == summary) ? 0 : summary->GetPowerPoint();
	}
	UINT32 rank = GetRank(challengid);
	return GetNpcBattlePoint(rank);
}

void ArenaMgr::PushChallengedRoleId(UINT64 qwRoleId, UINT64 qwChallengedRoleId)
{
	auto i = m_roleid2challenge.find(qwRoleId);
	if (i != m_roleid2challenge.end())
	{
		SSWarn<<"role id:"<<qwRoleId<<"challenged role id:"<<qwChallengedRoleId<<END;
	}
	UINT32 rank = GetRank(qwChallengedRoleId);
	m_roleid2challenge[qwRoleId] = ChallengeRole(qwChallengedRoleId, rank);
	return;
}

UINT64 ArenaMgr::PopChallengedRoleId(UINT64 qwRoleId)
{
	auto i = m_roleid2challenge.find(qwRoleId);
	if (i == m_roleid2challenge.end())
	{
		return 0;
	}
	UINT64 qwChallengedRoleId = i->second.roleid;
	m_roleid2challenge.erase(i);
	return qwChallengedRoleId;
}

UINT32 ArenaMgr::GetChallengedRank(UINT64 qwRoleId)
{
	auto i = m_roleid2challenge.find(qwRoleId);
	if (i == m_roleid2challenge.end())
	{
		return 0;
	}
	return i->second.rank;
}

bool ArenaMgr::IsInChallenge(UINT64 roleid)
{
	for (auto i = m_roleid2challenge.begin(); i != m_roleid2challenge.end(); ++i)
	{
		if (i->second.roleid == roleid)
		{
			return true;
		}
	}
	return false;
}

bool ArenaMgr::OnEnd(UINT64 roleid, bool iswin, KKSG::ArenaEndInfo& info)
{
	UINT32 rank = ArenaMgr::Instance()->GetChallengedRank(roleid);
	UINT64 challengedid = PopChallengedRoleId(roleid);
	if (0 == challengedid)
	{
		return false;
	}
	UINT32 challenrank = GetRank(challengedid);
	UINT32 lastrank = GetRank(roleid);
	info.set_lastrank(lastrank);	
	info.set_currentrank(lastrank);	

	info.set_npccount(ARENANPC_COUNT);
	info.set_challengeid(challengedid);
	info.set_isnpc(IsArenaNpc(challengedid));

	if (iswin)
	{
		if (!UpdateRankList(challengedid, challenrank, roleid, lastrank))
		{
			SSWarn<<"swap failed, role1 id:"<<challengedid<<"role2 id:"<<roleid<<END;
			return true;
		}
		if (challenrank != rank)
		{
			info.set_ismissed(true);
		}
		info.set_currentrank(challenrank);	
	}
	return true;
}

bool ArenaMgr::UpdateRankList(UINT64 qwRoleId1, UINT32 dwRank1, UINT64 qwRoleId2, UINT32 dwRank2)
{
	///> 被挑战者不在排行榜，或被挑战者排行低于挑战者
	if (InvalidRank(dwRank1) || dwRank1 >= dwRank2)
	{
		return false;
	}
	if (InvalidRank(dwRank2))
	{
		dwRank2 = INVALID_RANK;
	}
	return m_ranklist->SwapPos(qwRoleId1, dwRank1, qwRoleId2, dwRank2);
}

UINT32 ArenaMgr::GetExceptRole(UINT32 rankbegin, UINT32 rankend)
{
	for (UINT32 i = rankbegin; i < rankend; ++i)
	{
		UINT64 id = GetRoleId(i);
		if (IsArenaNpc(id))
		{
			return i;
		}
	}
	return rankbegin;
}

UINT32 ArenaMgr::GetDragonCoinCountByRank(UINT32 dwRank)
{
	const ArenaTable::RowData* pRowData = GetRowData(dwRank);
	if (NULL == pRowData)
	{
		return 0;
	}
	float fCoef = GetCoef(dwRank, pRowData);
	return pRowData->DailyDragonCoin[1] + (UINT32)((pRowData->DailyDragonCoin[0] - pRowData->DailyDragonCoin[1])*fCoef);
}

UINT32 ArenaMgr::GetGoldCoinCountByRank(UINT32 dwRank)
{
	const ArenaTable::RowData* pRowData = GetRowData(dwRank);
	if (NULL == pRowData)
	{
		return 0;
	}
	float fCoef = GetCoef(dwRank, pRowData);
	return pRowData->DailyGoldCoin[1] + (UINT32)((pRowData->DailyGoldCoin[0] - pRowData->DailyGoldCoin[1])*fCoef);
}

void ArenaMgr::GiveRankReward(UINT64 roleid, UINT32 dwRank)
{
	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(roleid);
	if (NULL == summary)
	{
		return;
	}
	std::vector<ItemDesc> vecItem;
	UINT32 dwGoldCount = GetGoldCoinCountByRank(dwRank);
	if (0 != dwGoldCount)
	{
		vecItem.push_back(ItemDesc(GOLD, dwGoldCount));
	}
	UINT32 dwDragonCount = GetDragonCoinCountByRank(dwRank);
	if (0 != dwDragonCount)
	{
		vecItem.push_back(ItemDesc(DRAGON_COIN, dwDragonCount));
	}

	MailData mail(m_mailconf.m_title, m_mailconf.m_content, vecItem, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendMail(roleid, mail);

	return; 
}
