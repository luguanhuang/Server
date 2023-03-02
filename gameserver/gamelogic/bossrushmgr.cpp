#include "pch.h"
#include "bossrushmgr.h"
#include "util/XRandom.h"
#include "unit/role.h"
#include "bagtransition.h"
#include "stagemgr.h"
#include "payv2Record.h"
#include "payv2Mgr.h"
#include "unit/enemymanager.h"
#include "utility/tlogger.h"
#include "unit/unit.h"
#include "scene/scene.h"
#include "XLevelWave.h"
#include "unit/rolemanager.h"
#include "stage/ptcg2c_bossrushonefinishntf.h"
#include "scene/scenefinder.h"
#include "entity/XRole.h"
#include "unit/attributewatcher.h"
#include "entity/XObject.h"
#include "component/XBuffComponent.h"
#include "component/XSkillComponent.h"
#include "scene/scenebossrush.h"
#include "event/eventmgr.h"
#include "gamelogic/activityhandler.h"
INSTANCE_SINGLETON(BossRushMgr);

BossRushMgr::BossRushMgr()
{

}

BossRushMgr::~BossRushMgr()
{

}

bool BossRushMgr::Init()
{
	if(!LoadFile())
	{
		return false;
	}
	m_maxRank =  GetGlobalConfig().GetInt("BossRushMaxWave", 5); 

	return true;
}

void BossRushMgr::Uninit()
{
}

bool BossRushMgr::LoadFile()
{
	BossRushTable brTableTmp;
	BossRushBuffTable bbuffTableTmp;
	if (!brTableTmp.LoadFile("table/BossRush.txt"))
	{
		SSWarn<<"load table/BossRush.txt failed"<<END;
		return false;
	}

	if (!bbuffTableTmp.LoadFile("table/BossRushBuff.txt"))
	{
		SSWarn<<"load table/BossRushBuff.txt failed"<<END;
		return false;
	}


	m_brTable.Clear();
	if (!m_brTable.LoadFile("table/BossRush.txt"))
	{
		SSWarn<<"load table/BossRush.txt failed"<<END;
		return false;
	}

	m_bbuffTable.Clear();
	if (!m_bbuffTable.LoadFile("table/BossRushBuff.txt"))
	{
		SSWarn<<"load table/BossRushBuff.txt failed"<<END;
		return false;
	}

	SSDebug << " load bossrush table succeed " << END;

	return true;
}

int BossRushMgr::RefreshData(Role* pRole)
{
	BRRoleState* data = GetRoleState(pRole);	
	if(NULL == data)
	{
		return KKSG::ERR_FAILED;
	}
	int uniqID = RandomConf(pRole, data->confid);
	if(0 == uniqID)
	{
		return KKSG::ERR_FAILED;
	}
	data->confid = uniqID;
	RandomBuff(data->brbid1, data->brbid2);

	pRole->Get<StageMgr>()->SetModified();

	SSDebug << __FUNCTION__ << " " << data->confid << " " << data->brbid1 << " " << data->brbid2 << END;
	return 0;
}

bool BossRushMgr::MakeRefreshCost(Role* pRole)
{
	if(NULL == pRole)
	{
		return false;
	}
	if(pRole->Get<CPayV2Record>()->GetRemainBossRushCount() > 0)
	{
		return true;
	}
	UINT32 count = UINT32(pRole->Get<StageMgr>()->GetBRRefreshCount(true));	
	const vector<UINT32>& cost =  GetGlobalConfig().BossRushCost;
	//cost.size = 8

	if(count < cost[0])
	{
		return true;
	}
	else
	{
		count = count - cost[0]; 
	}

	UINT32 itemID = 0;
	UINT32 itemCount = 0;
	if(count < cost[1])
	{
		itemID = cost[2];
		itemCount = cost[4] + count * cost[5];
	}
	else
	{
		count = count - cost[1];
		itemID = cost[3];
		itemCount = cost[6] + count * cost[7];
	}

	if(0 == itemID || 0 == itemCount)
	{
		SSError << " itemid = " << itemID << " itemcount = " << itemCount << END;
		return false;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Stage, ItemFlow_Stage_BossRush);  
	if(!take.TakeItem(itemID, itemCount))
	{
		take.RollBack();
		return false;
	}
	take.NotifyClient();

	return true;
}

void BossRushMgr::OnBattleFinish(Role* pRole, std::vector<Sequence<UINT32,2>>& reward, int& succeedcount)
{
	if(NULL == pRole)
	{
		return ;
	}

	BRRoleState* curState = GetRoleState(pRole);
	if(NULL == curState)
	{
		return ;
	}

	if(curState->rank < m_maxRank)
	{
		return;
	}

	if(LeftBossRushCount(pRole) > 0)
	{
		AddBossRushCount(pRole);
	}

	ResetRefreshCount(pRole);

	for(size_t i = 0; i < curState->reward.size(); ++i)
	{
		SSDebug << __FUNCTION__ << curState->reward[i][0] << " " << curState->reward[i][1]<< END;
		reward.push_back(curState->reward[i]);
	}

	succeedcount = curState->rank;

	ClearRoleState(pRole);

	pRole->Get<StageMgr>()->SetModified();

	EventMgr::Instance()->AddGuildBonusEvent(pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_BOSSRUSH, m_maxRank);
}

void BossRushMgr::FillBossAppearance(Role* pRole, KKSG::UnitAppearance* app)
{
	if(NULL == pRole || NULL == app)
	{
		return ;
	}
	
	BRRoleState* curState = GetRoleState(pRole);
	if(NULL == curState)
	{
		return ;
	}

	BossRushTable::RowData* conf = m_brTable.GetByqniqueid(curState->confid);
	if(NULL == conf)
	{
		return ;
	}

	app->set_waveid(curState->rank + 1);
	EnemyManager::LoadEnemyAppearance(conf->bossid, app);

	KKSG::Attribute* pAttr = app->mutable_attributes();
	for(int i = 0; i < pAttr->attrid_size() && i < pAttr->basicattribute_size() ; ++i)
	{
		int attrid = pAttr->attrid(i); 
		if(BASIC_CurrentHP == attrid || BASIC_MaxHP == attrid)
		{
			double value = pAttr->basicattribute(i);	
			value = value * conf->lifepercent;
			pAttr->set_basicattribute(i, value);
		}

		if(BASIC_PhysicalAtk == attrid || BASIC_PhysicalAtkMod == attrid || BASIC_MagicAtk == attrid)
		{
			double value = pAttr->basicattribute(i);
			value = value * conf->atkpercent;
			pAttr->set_basicattribute(i, value);
		}

		if(BASIC_PhysicalDef == attrid || BASIC_PhysicalDefMod == attrid || BASIC_MagicDef == attrid)
		{
			double value = pAttr->basicattribute(i);
			value = value * conf->defpercent;
			pAttr->set_basicattribute(i, value);
		}
	}
	
}

void BossRushMgr::FinishOneCount(Role* pRole, bool win)
{
	SSDebug << __FUNCTION__ << " " << (int)(win) << END;
	if(NULL == pRole)
	{
		return ;
	}
	BRRoleState* curState= GetRoleState(pRole);
	if(NULL == curState || 0 == curState->confid)
	{
		return ;
	}
	
	TRoundFlowLog oLog(pRole);
	oLog.m_nBattleID = curState->rank + 1;
	oLog.m_nBattleType = KKSG::SCENE_BOSSRUSH;
	oLog.m_nResult = ((win== true)?TX_BATTLE_SUCCESS:TX_BATTLE_FAIL);
	oLog.m_BrID = curState->confid;
	oLog.m_Brbid1 = curState->brbid1;
	oLog.m_Brbid2 = curState->brbid2;
	oLog.m_BrrefreshCount = GetCurRefreshCount(pRole);
	oLog.Do();

	ResetRefreshCount(pRole);
	if(!win)
	{
		if(LeftBossRushCount(pRole) > 0)
		{
			AddBossRushCount(pRole);
		}
		ClearRoleState(pRole);
	}
	else
	{
		if (NULL != pRole->GetCurrScene())
		{
			CActivityHandler::GlobalHandler.OnCompleteScene(pRole, pRole->GetCurrScene());
		}
		AddStateRank(pRole);
		GiveReward(pRole);				
		if(curState && curState->rank >= m_maxRank)
		{
			;//空
		}
		else
		{
			PtcG2C_BossRushOneFinishNtf ptc;
			ptc.m_Data.set_rank(curState->rank);
			ptc.m_Data.set_win(win);
			pRole->Send(ptc);

			RefreshData(pRole);
		}

		if (NULL != pRole->GetCurrScene())
		{
			CActivityHandler::GlobalHandler.OnCompleteScene(pRole, pRole->GetCurrScene());
		}
	}

	pRole->Get<StageMgr>()->SetModified();
}

void BossRushMgr::FillRoleStatus(Role* pRole, KKSG::BossRushData* data)
{
	if(NULL == pRole || NULL == data)
	{
		return ;
	}
	BRRoleState* curStatus = GetRoleState(pRole);
	if(curStatus == NULL)
	{
		return ;
	}

	if(0 == curStatus->confid && LeftBossRushCount(pRole) > 0)
	{
		//需要初始化
		RefreshData(pRole);	
	}

	curStatus = GetRoleState(pRole);
	if(curStatus == NULL)
	{
		return ;
	}
	
	data->set_confid(curStatus->confid);
	data->set_buffid1(curStatus->brbid1);
	data->set_buffid2(curStatus->brbid2);
	data->set_currank(curStatus->rank + 1);
	data->set_maxrank(m_maxRank);
	
	data->set_currefreshcount(GetCurRefreshCount(pRole));
	data->set_maxrefreshcount(GetMaxRefreshCount(pRole));

	data->set_joincounttoday(GetCurBossRushCount(pRole));
	data->set_joincountmax(GetMaxBossRushCount());

}

int BossRushMgr::LeftRefreshCount(Role* pRole)
{
	int cur = GetCurRefreshCount(pRole);
	int max = GetMaxRefreshCount(pRole);
	if(cur >= max)
	{
		return 0;
	}
	return (max - cur);
}

void BossRushMgr::AddRefreshCount(Role* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}
	pRole->Get<StageMgr>()->AddBRRefreshCount();
}

void BossRushMgr::ResetRefreshCount(Role* pRole)
{
	if(NULL == pRole)
	{
		return ;
	}

	SSDebug << __FUNCTION__ << " " << pRole->GetName() << END;
	pRole->Get<StageMgr>()->ResetBRRefreshCount();
}

int BossRushMgr::LeftBossRushCount(Role* pRole)
{
	int cur = GetCurBossRushCount(pRole);
	int max = GetMaxBossRushCount();

	if(cur >= max)
	{
		return 0;
	}
	return (max - cur);
}

void BossRushMgr::AddBossRushCount(Role* pRole)
{
	SSDebug << __FUNCTION__ << END;
	if(NULL == pRole)
	{
		return ;
	}
	
	pRole->Get<StageMgr>()->AddBRJoinCount();

	EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_BossRush, 0, 1);
}

void BossRushMgr::TestClear(Role* pRole)
{
	ClearRoleState(pRole);	
	pRole->Get<StageMgr>()->TestClear();
	pRole->Get<CPayV2Record>()->ClearBossRushCount();
}

void BossRushMgr::TestPrint(Role* pRole)
{   
	KKSG::BossRushData data;
	FillRoleStatus(pRole, &data);
	SSWarn << data.confid()<< " " << data.buffid1() << " " << data.buffid2() << " " << data.currank()
		<< " " << data.maxrank() << " " << data.currefreshcount() << " " << data.maxrefreshcount() << END;
}

int BossRushMgr::GetCurRefreshCount(Role* pRole)
{
	if(NULL == pRole)
	{
		return 0;
	}
	return pRole->Get<StageMgr>()->GetBRRefreshCount();
}
int BossRushMgr::GetCurRank(Role* pRole)
{
	BRRoleState* pData = GetRoleState(pRole);
	if (pData)
	{
		return pData->rank + 1;
	}
	return 0;
}
int BossRushMgr::GetMaxRefreshCount(Role* pRole)
{
	return 10000;
	if(NULL == pRole)
	{
		return 0;
	}
	int vip = pRole->Get<CPayV2Record>()->GetVipLevel();
	VIPTable::RowData* conf = PayV2Mgr::Instance()->GetVipConf(vip);
	if(NULL == conf)
	{
		return 0;
	}
	return conf->BossRushCount + pRole->Get<CPayV2Record>()->GetBossRushCount();
}

int BossRushMgr::GetCurBossRushCount(Role* pRole)
{
	if(NULL == pRole)
	{
		return 0;
	}
	return pRole->Get<StageMgr>()->GetBRJoinCount();
}

int BossRushMgr::GetMaxBossRushCount()
{
	return  GetGlobalConfig().GetInt("BossRushDayCount", 1);
}

void BossRushMgr::GiveReward(Role* pRole)
{
	SSDebug << __FUNCTION__ << END;
	if(NULL == pRole)
	{
		return ;
	}
	BRRoleState* data = GetRoleState(pRole);
	if(NULL == data)
	{
		return ;
	}
	if(data->rank > m_maxRank)
	{
		return ;
	}
	BossRushTable::RowData* conf = m_brTable.GetByqniqueid(data->confid);
	if(NULL == conf)
	{
		return ;
	}

	for(size_t i = 0; i < conf->reward.size(); ++i)
	{
		UINT32 itemID = conf->reward[i][0];
		INT64 count = conf->reward[i][1]; 

		BossRushBuffTable::RowData* brbc = m_bbuffTable.GetByBossRushBuffID(data->brbid1);
		if(brbc && brbc->RewardBuff > 0.0)
		{
			count = (INT64)(count * brbc->RewardBuff); 
		}
		brbc = m_bbuffTable.GetByBossRushBuffID(data->brbid2);
		if(brbc && brbc->RewardBuff > 0.0)
		{
			count = (INT64)(count * brbc->RewardBuff); 
		}

		Sequence<UINT32,2> seq;
		seq[0] = itemID;
		seq[1] = (UINT32)count;
		data->reward.push_back(seq);
	}

	if(data->rank < m_maxRank)//前几层直接给
	{
		BagGiveItemTransition give(pRole);
		give.SetReason(ItemFlow_Stage, ItemFlow_Stage_BossRush);
		for(size_t i = 0; i < data->reward.size(); ++i)
		{
			give.GiveItem(data->reward[i][0], data->reward[i][1], false);	
		}
		give.NotifyClient();
		data->reward.clear();
	}
}

void BossRushMgr::AddStateRank(Role* pRole)
{
	BRRoleState* data = GetRoleState(pRole);
	if(data)
	{
		data->rank++;
	}
}

int BossRushMgr::RandomConf(Role* pRole, int oldconfid)
{
	if(NULL == pRole)
	{
		return 0;
	}

	BRRoleState* curState = GetRoleState(pRole);	
	if(NULL == curState)
	{
		return 0;
	}
	int roleLevel = pRole->GetLevel();
	int rank = (curState->rank + 1);
	int allpro = 0;
	std::vector<Sequence<int,2>> selvec;
	for(size_t i = 0; i < m_brTable.Table.size(); ++i)
	{
		BossRushTable::RowData* data = m_brTable.Table[i];
		if(NULL == data)
		{
			continue;
		}
		if(oldconfid == data->qniqueid)
		{
			continue;
		}
		if(rank == data->rank && data->level[0] <= roleLevel && roleLevel <= data->level[1])
		{
			Sequence<int,2> seq;
			seq[0] = data->qniqueid;
			seq[1] = data->percent;
			allpro += data->percent;
			selvec.push_back(seq);
		}
	}

	if(selvec.empty())
	{
		return oldconfid;
	}
	
	int rk = XRandom::randInt(1, allpro+1);
	for(size_t i = 0; i < selvec.size(); ++i)
	{
		rk -= selvec[i][1];
		if(rk <= 0)
		{
			return selvec[i][0];
		}
	}
	
	return 0;
}

void BossRushMgr::RandomBuff(int& brbid1, int& brbid2)
{
	std::vector<Sequence<int,2>> buffvec;
	for(size_t i = 0; i < m_bbuffTable.Table.size(); ++i)
	{
		BossRushBuffTable::RowData* conf = m_bbuffTable.Table[i];
		/*
		if(conf->BossRushBuffID == brbid1 || conf->BossRushBuffID == brbid2)
		{
			continue;
		}
		*/
		Sequence<int,2> seq;
		seq[0] = conf->BossRushBuffID;
		seq[1] = conf->type;
		buffvec.push_back(seq);
	}

	if(buffvec.empty())
	{
		return ;
	}

	int index = XRandom::randInt(0, buffvec.size());
	brbid1 = buffvec[index][0];
	int type = buffvec[index][1];
	for(auto it = buffvec.begin(); it != buffvec.end();)
	{
		if((*it)[1] == type)
		{
			it = buffvec.erase(it);
		}
		else
		{
			it++;
		}
	}

	if(buffvec.empty())
	{
		return ;
	}

	index = XRandom::randInt(0, buffvec.size());
	brbid2 = buffvec[index][0];
}

void BossRushMgr::ClearRoleState(Role* pRole)
{
	if(pRole)
	{
		pRole->Get<StageMgr>()->ClearBRRoleState();
	}
}

BRRoleState* BossRushMgr::GetRoleState(Role* pRole)
{
	if(NULL == pRole)
	{
		return NULL;
	}
	return pRole->Get<StageMgr>()->GetBRRoleState();
}

void BossRushMgr::OnRoleEnterScene(Role* pRole)
{
	//刷第一boss
	if(pRole && pRole->GetCurrScene() && pRole->GetCurrScene()->GetSceneType() == KKSG::SCENE_BOSSRUSH)
	{
		OnRoleCountinue(pRole);	
	}
}

void BossRushMgr::OnRoleCountinue(Role* pRole)
{
	SSDebug << __FUNCTION__ << END;
	if(pRole && pRole->GetCurrScene() && pRole->GetCurrScene()->GetBRHandler())
	{
		XLevelSpawner*	pSpawner = pRole->GetCurrScene()->GetSpawner();
		BRRoleState* data = GetRoleState(pRole);
		BossRushTable::RowData* pConf = m_brTable.GetByqniqueid(data->confid);	
		if(pSpawner && data && pConf)
		{
			pSpawner->ReplaceRandmizedEnemyID(data->rank, pConf->bossid);
		}
		pRole->GetCurrScene()->GetBRHandler()->MakeCountinue();
		//SSDebug << " maxhp = " << fvalue << " curhp " << pRole->GetAttr(BASIC_CurrentHP) << END;
	}
}

void BossRushMgr::FillBossData(Enemy* pBoss, Scene* pScene)
{
	SSDebug << __FUNCTION__ << END;
	if(pBoss && pScene && pScene->GetSceneType() == KKSG::SCENE_BOSSRUSH)
	{
		Role* pRole = RoleManager::Instance()->FindByRoleID(pScene->GetOwnerID());
		if(NULL == pRole)
		{
			SSError << " find not owner roleid = " << pScene->GetOwnerID() << END;
			return ;
		}
		BRRoleState* pState = GetRoleState(pRole);
		if(NULL == pState)
		{
			return ;
		}
		BossRushTable::RowData* pConf = m_brTable.GetByqniqueid(pState->confid);
		if(NULL == pConf)
		{
			return ;
		}
		//改变属性
		pBoss->ResetBaseAttr(pConf->lifepercent, pConf->atkpercent, pConf->defpercent);
	}
}

BossRushTable::RowData* BossRushMgr::GetConf(int confid)
{
	return m_brTable.GetByqniqueid(confid);
}

BossRushBuffTable::RowData* BossRushMgr::GetBuffConf(int confid)
{
	return m_bbuffTable.GetByBossRushBuffID(confid);
}
