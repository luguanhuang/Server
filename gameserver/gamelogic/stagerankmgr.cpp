#include "pch.h"
#include "stagerankmgr.h"
#include "globalconfig.h"
#include "item.h"
#include "util/XCommon.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "scene/scene.h"
#include "scene/scenemortuary.h"
#include "unit/role.h"
#include "payv2Record.h"
#include "payv2Mgr.h"
#include "pb/enum.pb.h"

StageRankMgr::StageRankMgr()
{

}

StageRankMgr::~StageRankMgr()
{

}

INSTANCE_SINGLETON(StageRankMgr);

bool StageRankMgr::Init()
{
	return LoadFile();
}

void StageRankMgr::Uninit()
{

}

void StageRankMgr::ClearFile()
{
	m_oTable.Clear();
	//m_nesttable.Clear();
	m_nestScoreTable.Clear();
}

bool StageRankMgr::CheckFile()
{
	StageRankTable tempTable;
	if (!tempTable.LoadFile("table/StageRank.txt"))
	{
		SSWarn<<"load table/StageRank.txt failed"<<END;
		return false;
	}
	//NestRewardTable tempnesttable;
	//if (!tempnesttable.LoadFile("table/NestReward.txt"))
	//{
	//	SSWarn<<"load table/NestReward.txt"<<END;
	//	return false;
	//}
	NestScoresTable tempnestScoreTable;
	if (!tempnestScoreTable.LoadFile("table/ScoresStandard.txt"))
	{
		SSWarn<<"load table/ScoresStandard.txt"<<END;
		return false;
	}
	return true;
}

bool StageRankMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	if (!m_oTable.LoadFile("table/StageRank.txt"))
	{
		SSWarn<<"load table/StageRank.txt failed"<<END;
		return false;
	}
	//if (!m_nesttable.LoadFile("table/NestReward.txt"))
	//{
	//	SSWarn<<"load table/NestReward.txt"<<END;
	//	return false;
	//}
	if (!m_nestScoreTable.LoadFile("table/ScoresStandard.txt"))
	{
		SSWarn<<"load table/ScoresStandard.txt"<<END;
		return false;
	}

	return true;
}

void StageRankMgr::CalculateRankInfo(UINT32 sceneid, StageRankInfo& info)
{
	StageRankTable::RowData* row = m_oTable.GetByscendid(sceneid);
	if (row == NULL)
	{
		return;
	}
	bool cond1 = CheckOneCondition(row->star2, info);
	bool cond2 = CheckOneCondition(row->star3, info);
	info.rank = CalculateRank(cond1, cond2);	
}

bool StageRankMgr::CheckOneCondition(Sequence<uint, 3>& cond, StageRankInfo& info)
{
	UINT32 condId = cond[0];
	UINT32 subId = cond[1];
	UINT32 condNum = cond[2];
	switch (condId)
	{
	// 通关时间
	case KKSG::StageRankCond_Time:
		if (condNum == 0 || info.timespan <= condNum)
		{
			return true;
		}
		break;
	// 剩余血量
	case KKSG::StageRankCond_Hppercent:
		if (info.hppercent >= condNum)
		{
			return true;
		}
		break;
	// 进战次数
	case KKSG::StageRankCond_Found:
		if (info.found <= condNum)
		{
			return true;
		}
		break;
	// 被击打次数
	case KKSG::StageRankCond_Behit:
		if (info.behit <= condNum)
		{
			return true;
		}
		break;
	// npc血量
	case KKSG::StageRankCond_NpcHp:
		if (info.npchp >= condNum)
		{
			return true;
		}
		break;
	// 连击次数
	case KKSG::StageRankCond_Combo:
		if (info.combo >= condNum)
		{
			return true;
		}
		break;
	//杀怪积分
	case KKSG::StageRankCond_KillEnemyScore:
		if (info.killenemyscore >= condNum)
		{
			return true;
		}
		break;
	//存活时间
	case KKSG::StageRankCond_AliveTime:
		if (info.timespan >= condNum)
		{
			return true;
		}
		break;
	//总杀怪积分
	case KKSG::StageRankCond_TotalKillEnemyScore:
		if (info.totalkillenemyscore >= condNum)
		{
			return true;
		}
		break;
	default:
		SSWarn << "stage rank invalid condition: " << condId << END;
	}

	return false;
}

int StageRankMgr::CalculateRank(bool star2, bool star3)
{
	if (star2 && star3)
	{
		return RANK_SSS;
	}
	else if (star2)
	{
		return RANK_SS_1;
	}
	else if (star3)
	{
		return RANK_SS_2;
	}
	else
	{
		return RANK_S;
	}
}

//ItemDesc StageRankMgr::GetNestChestReward(UINT32 sceneid, UINT32 pos)
//{
//	ItemDesc item;
//	NestRewardTable::RowData* rowdata = m_nesttable.GetBysceneid(sceneid);
//	if (NULL == rowdata || pos >= rowdata->chestreward.size())
//	{
//		return item;
//	}
//	item.itemID = rowdata->chestreward[pos].seq[0];
//	item.itemCount = rowdata->chestreward[pos].seq[1];
//	return item;
//}

//std::vector<Sequence<UINT32, 2>>* StageRankMgr::GetNestReward(UINT32 sceneid)
//{
//	NestRewardTable::RowData* rowdata = m_nesttable.GetBysceneid(sceneid);
//	if (NULL == rowdata)
//	{
//		return NULL;
//	}
//	return &rowdata->reward;
//}

bool StageRankMgr::Compare(int rank1, int rank2)
{
	if (-1 == rank2)
	{
		return true;
	}
	int i = XCommon::GetOneCount(rank1);
	int j = XCommon::GetOneCount(rank2);
	return i >= j;
}

bool StageRankMgr::HasRank(UINT32 sceneid)
{
	StageRankTable::RowData* rowdata = m_oTable.GetByscendid(sceneid);
	return NULL != rowdata;
}

UINT32 StageRankMgr::GetStarByRank( int rank )
{
	switch(rank)
	{
	case RANK_S:
		return 1;
	case RANK_SS_1:
	case RANK_SS_2:
		return 2;
	case RANK_SSS:
		return 3;
	default:
		return 0;
	}
	return 0;
}

const char *RankStr(int rank)
{
	switch(rank)
	{
	case RANK_S:
		return "<S>";
	case RANK_SS_1:
	case RANK_SS_2:
		return "<SS>";
	case RANK_SSS:
		return "<SSS>";
	case RANK_A:
		return "<A>";
	case RANK_B:
		return "<B>";
	default:
		return "<Error Rank>";
	}

	return "<Error Rank>";
}

StageRankInfo StageRankMgr::CalculateNestRankInfo(Scene *pScene, Role *pRole, int dmgRank, int pptRank, int treatrank)
{
	StageRankInfo info;
	auto *pRow = m_nestScoreTable.GetBySceneID(pScene->GetSceneTemplateID());
	if (pRow == NULL)
	{
		LogWarn("Not found nest score for scene: %d", pScene->GetSceneTemplateID());
		return info;
	}

	double timeScore = 0;
	double liveScore = 0;
	double actionScore = 0;

	int sceneTimeSpan = pScene->GetTimeSpan();
	if(sceneTimeSpan <= 0)
	{
		sceneTimeSpan = 1;
	}
	info.timespan = sceneTimeSpan;
	timeScore = pRow->MaxTimeScores * pRow->Time / sceneTimeSpan;
	if (timeScore > pRow->MaxTimeScores)
	{
		timeScore = pRow->MaxTimeScores;
	}

	liveScore = pRow->MaxSurviveScores - pRow->DeadScores * pScene->GetMortuary()->GetDeathCount(pRole->GetID());
	if (liveScore < 0)
	{
		liveScore = 0;
	}

	actionScore = pRow->ProfessionScores[pRole->GetProfession()%10-1];
	if (dmgRank < pRow->DamageScores.size())
	{
		actionScore += pRow->DamageScores[dmgRank];
	}
	if (treatrank < pRow->TreatScores.size())
	{
		actionScore += pRow->TreatScores[treatrank];
	}
	if (pptRank < pRow->FightScores.size())
	{
		actionScore += pRow->FightScores[pptRank];
	}

	double totalScore = timeScore + liveScore + actionScore;

	int vip = pRole->Get<CPayV2Record>()->GetVipLevel();
	VIPTable::RowData *pPayRow = PayV2Mgr::Instance()->GetVipConf(vip);
	if (pPayRow != NULL)
	{
		info.vipscore = (int)(totalScore * pPayRow->NestScoreBonus / 100);
	}

	info.score = (int)totalScore + info.vipscore;

	if (totalScore <= pRow->Standard[0])
	{
		info.rank = RANK_B;
	}
	else if (totalScore <= pRow->Standard[1])
	{
		info.rank = RANK_A;
	}
	else if (totalScore <= pRow->Standard[2])
	{
		info.rank = RANK_S;
	}
	else if (totalScore <= pRow->Standard[3])
	{
		info.rank = RANK_SS_1;
	}
	else
	{
		info.rank = RANK_SSS;
	}

	bool debug_print = false;
	if (debug_print)
	{
		std::stringstream ss;
		ss << "NestScore for " << pRole << "\n"
			<< "DamageRank: " << dmgRank << ", PowerPointRank: " << pptRank << "\n"
			<< "timeScore: " << timeScore << "\n"
			<< "liveScore: " << liveScore << "\n"
			<< "DamageScore: " << pRow->DamageScores[dmgRank] << "\n"
			<< "TreatScore: " << pRow->TreatScores[dmgRank] << "\n"
			<< "FigthScore: " << pRow->FightScores[pptRank] << "\n"
			<< "ProfessionScore: " << pRow->ProfessionScores[pRole->GetProfession()%10-1] << "\n"
			<< "VIPScore: " << info.vipscore << "\n"
			<< "TotalScore: " << info.score << "\n"
			<< "Rank: " << RankStr(info.rank) << "\n";
		LogInfo("%s", ss.str().c_str());
	}
	
	return info;
}

UINT32 StageRankMgr::GetConcernNpcId(UINT32 sceneid)
{
	StageRankTable::RowData* row = m_oTable.GetByscendid(sceneid);
	if (!row)
	{
		return 0;
	}
	if (row->star2[0] == KKSG::StageRankCond_NpcHp)
	{
		return row->star2[1];
	}
	if (row->star3[0] == KKSG::StageRankCond_NpcHp)
	{
		return row->star3[1];
	}
	return 0;
}
