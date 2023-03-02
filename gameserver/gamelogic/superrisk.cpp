#include "pch.h"
#include "pb/project.pb.h"
#include "superrisk.h"
#include "superriskconfig.h"
#include "unit/role.h"
#include "util/XRandom.h"
#include "gamelogic/superriskrecord.h"
#include "gamelogic/bagtransition.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/scenerisk.h"
#include "superrisk/ptcg2c_riskbuyntf.h"
#include "config/itemdropconfig.h"
#include "gamelogic/taskrecord.h"

SuperRisk::SuperRisk(Role* role)
{
	getvalue = 0;
	m_role = role;
	mapID = 0;
	sceneID = 0;
	inDice = false;
	receiveOver = false;
	gridType = 0;
	canBuy = false;
	debugDiceNum = 0;
}

SuperRisk::~SuperRisk()
{

}

bool SuperRisk::PlayDice(int mapID, int randValue, int& finalValue)
{
	if (inDice)
	{
		LogError("superrisk,id:%llu, in dice, mapid:%d, randvalue:%d.", m_role->GetID(), mapID, randValue);
		return false;
	}
	if (!m_role->Get<CSuperRiskRecord>()->IsMapOpen(mapID))
	{
		LogError("superrisk,id:%llu, map not open, mapid:%d, randvalue:%d.", m_role->GetID(), mapID, randValue);
		return false;
	}
	if (!m_role->GetCurrScene() || m_role->GetCurrScene()->GetSceneType() == KKSG::SCENE_RISK)
	{
		LogError("superrisk,id:%llu, not in correct scene, mapid:%d, randvalue:%d.", m_role->GetID(), mapID, randValue);
		return false;
	}
	if (randValue < 1 || randValue > 6)
	{
		LogError("superrisk,id:%llu, value invalid, mapid:%d, randvalue:%d.", m_role->GetID(), mapID, randValue);
		return false;
	}

	ClearDiceInfo();

	const std::vector<RiskDiceRandomTable::RowData*>& data = SuperRiskConfig::Instance()->GetDiceRandomTable();

	auto iter = data.begin();
	for (; iter != data.end(); ++iter)
	{
		if (randValue <= (*iter)->RandomValue)
		{
			break;
		}
	}

	if (iter == data.end())
	{
		LogError("superrisk,id:%llu, xml error, mapid:%d, randvalue:%d.", m_role->GetID(), mapID, randValue);
		return false;
	}

	if (!m_role->Get<CSuperRiskRecord>()->ReduceDiceNum(1))
	{
		LogError("superrisk,id:%llu, dice not enough, mapid:%d, randvalue:%d.", m_role->GetID(), mapID, randValue);
		return false;
	}
	int total = 0;
	total = (*iter)->One + (*iter)->Two + (*iter)->Three + (*iter)->Four + (*iter)->Five + (*iter)->Six;
	int randResult = XRandom::randInt(1, total + 1);
	int value = 0;
	int finalDice = 0;
	for (int i = 1; i <=6; ++i)
	{
		//int value = 0;
		switch (i)
		{
		case 1:
			value += (*iter)->One;
			break;
		case 2:
			value += (*iter)->Two;
			break;
		case 3:
			value += (*iter)->Three;
			break;
		case 4:
			value += (*iter)->Four;
			break;
		case 5:
			value += (*iter)->Five;
			break;
		case 6:
			value += (*iter)->Six;
			break;
		default:
			break;
		}
		if (randResult <= value)
		{
			finalDice = i;
			break;
		}
	}
	if (debugDiceNum)
	{
		finalDice = debugDiceNum;
		debugDiceNum = 0;
	}
	getvalue = finalDice;
	finalValue = finalDice;
	this->mapID = mapID;

	m_role->Get<TaskRecord>()->UpdateTask(KKSG::TaskOper_Add, KKSG::TaskConn_Activity, KKSG::TaskActType_SuperRisk);
	SetInDice(true);

	LogInfo("superrisk,id:%llu, dicevalue:%d.", m_role->GetID(), getvalue);

	return true;
}

bool SuperRisk::DiceOver(KKSG::PlayDiceOverRes& roRes, bool forceOver)
{
	if (!inDice)
	{
		LogError("superrisk,diceover, not in dice,id:%llu, mapID:%d.", m_role->GetID(), mapID);
		return false;
	}
	auto record = m_role->Get<CSuperRiskRecord>();
	const RiskMapRoleInfo* info = record->GetRoleRiskMapInfo(mapID);
	if (!info)
	{
		LogError("superrisk,diceover, map error,id:%llu, mapID:%d.", m_role->GetID(), mapID);
		return false;
	}
	const SuperRiskMapGeneratedInfo* geneInfo = record->GetGeneratedMapInfo(mapID);
	if (!geneInfo)
	{
		LogError("superrisk,diceover, map generate error,id:%llu, mapID:%d.", m_role->GetID(), mapID);
		return false;
	}
	XSuperRiskMap riskMap(mapID, info->pos.x, info->pos.y, (RiskDirection)info->moveDirection);
	Coordinate dest(-1, -1);
	RiskDirection direction = Right;
	bool reachEnd = false;
	for (int i = 1; i <= getvalue; ++i)
	{
		direction = riskMap.StartMoveNext(dest);
		if (SuperRiskConfig::Instance()->IsTerminalPoint(mapID, dest))
		{
			reachEnd = true;
			break;
		}
	}


	const GridInfo* gridInfo = geneInfo->GetGridInfo(dest);

	record->EndMove(mapID, dest, direction);

	const_cast<SuperRiskMapGeneratedInfo*>(geneInfo)->UpdateGridBoxState(dest, KKSG::RISK_BOX_UNLOCKED);

	gridType = 0;
	if (gridInfo)
	{
		gridType = gridInfo->gridType;

		if (gridInfo->gridType == KKSG::RISK_GRID_NORMALREWARD)
		{
			BagGiveItemTransition transition(m_role);
			transition.SetReason(ItemFlow_SuperRisk, ItemFlow_SuperRiskDice);
			transition.GiveItem(gridInfo->itemID, gridInfo->itemNum);	
			transition.NotifyClient();
		}
		else if (gridInfo->gridType == KKSG::RISK_GRID_DICE)
		{
			record->AddDiceNum(1, true);
		}
		else if (gridInfo->gridType == KKSG::RISK_GRID_ADVENTURE)
		{
			if (!forceOver)
			{
				int sceneID = SuperRiskConfig::Instance()->GetRandomBattleSceneID(mapID);
				SceneManager::Instance()->ChangeScene(m_role, sceneID);
				this->sceneID = sceneID;
			}
		}
		else if (gridInfo->gridType == KKSG::RISK_GRID_REWARDBOX)
		{
			int slot = record->AddBox(mapID, gridInfo->itemID, gridInfo->itemNum);
			if (slot >= 0)
			{
				roRes.set_mapid(mapID);
				auto boxInfos = m_role->Get<CSuperRiskRecord>()->GetBoxInfos();
				auto find = boxInfos.GetSlotInfo(slot);
				if (find)
				{
					auto boxInfo = roRes.mutable_addboxinfo();
					boxInfo->CopyFrom(*find);
				}
			}

		}

	}

	if(!gridInfo || gridInfo->gridType != KKSG::RISK_GRID_ADVENTURE)
		OnNormalDiceOver();

	receiveOver = true;

	canBuy = CheckTriggerBuy(gridType);

	roRes.set_hastriggerbuy(canBuy);

	if (reachEnd)
	{
		record->RefreshMapInfo(mapID);
	}

	LogInfo("superrisk,diceover,id:%llu, mapID:%d.", m_role->GetID(), mapID);
	return true;
}

void SuperRisk::OnNormalDiceOver()
{
	SetInDice(false);

	//canBuy = CheckTriggerBuy(gridType);
	LogInfo("superrisk,normaldiceover,id:%llu.", m_role->GetID());
}

void SuperRisk::ClearDiceInfo()
{
	mapID = 0;
	getvalue = 0;
	sceneID = 0;
	gridType = 0;
	canBuy = false;
	receiveOver = false;
	SetInDice(false);
}

bool SuperRisk::CheckTriggerBuy(int gridtype)
{
	auto data = SuperRiskConfig::Instance()->GetTriggerBuyData(mapID, gridtype);
	if (data)
	{
		int rand = XRandom::randInt(1, 101);
		if (rand <= data->Probability)
		{
			PtcG2C_RiskBuyNtf ntf;
			for (auto iter = data->RewardItem.begin(); iter != data->RewardItem.end(); ++iter)
			{
				KKSG::ItemBrief* brief = ntf.m_Data.add_rewarditems();
				brief->set_itemid(iter->seq[0]);
				brief->set_itemcount(iter->seq[1]);
			}
			if (!data->CostItem.empty())
			{
				KKSG::ItemBrief* brief = ntf.m_Data.mutable_cost();
				brief->set_itemid(data->CostItem[0].seq[0]);
				brief->set_itemcount(data->CostItem[0].seq[1]);
			}

			m_role->Send(ntf);

			return true;
		}
	}

	return false;
}

UINT32 SuperRisk::RiskBuy()
{
	if (!canBuy)
	{
		LogError("%llu,%s, can not buy", m_role->GetRoleID(), m_role->GetName().c_str());
		return KKSG::ERR_FAILED;
	}
	auto data = SuperRiskConfig::Instance()->GetTriggerBuyData(mapID, gridType);
	if (!data)
	{
		LogError("%llu,%s, mapid %u gridtype %u not exist", m_role->GetRoleID(), m_role->GetName().c_str(), mapID, gridType);
		return KKSG::ERR_FAILED;
	}
	if (data->CostItem.empty())
	{
		LogError("%llu,%s, mapid %u gridtype %u cost empty", m_role->GetRoleID(), m_role->GetName().c_str(), mapID, gridType);
		return KKSG::ERR_FAILED;
	}
	if (!XItem::IsVirtualItem(data->CostItem[0].seq[0]))
	{
		LogError("%llu,%s, mapid %u gridtype %u cost error", m_role->GetRoleID(), m_role->GetName().c_str(), mapID, gridType);
		return KKSG::ERR_FAILED;
	}

	BagTakeItemTransition transiton(m_role);
	transiton.SetReason(ItemFlow_SuperRisk,ItemFlow_SuperRiskBuy);
	if (!transiton.TakeItem(data->CostItem[0].seq[0], data->CostItem[0].seq[1]))
	{
		transiton.RollBack();
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}	
	transiton.NotifyClient();

	BagGiveItemTransition giveTransiton(m_role);
	giveTransiton.SetReason(ItemFlow_SuperRisk, ItemFlow_SuperRiskBuy);
	for (auto iter = data->RewardItem.begin(); iter != data->RewardItem.end(); ++iter)
	{
		giveTransiton.GiveItem(iter->seq[0], iter->seq[1]);
	}	
	giveTransiton.NotifyClient();

	canBuy = false;
	Change();

	return KKSG::ERR_SUCCESS;
}

void SuperRisk::GiveReward(std::map<int, int>& reward)
{
	auto data = SuperRiskConfig::Instance()->GetRiskMapBattleData(mapID, sceneID);
	if (data)
	{
		for (auto iter = data->DropID.begin(); iter != data->DropID.end(); ++iter)
		{
			const ItemDesc* pConf = ItemDropConfig::Instance()->GetRandomDrop(*iter, m_role->GetLevel());
			if (pConf)
			{
				reward[pConf->itemID] += pConf->itemCount;
			}
		}
	}
}

void SuperRisk::OnLeave(Scene* scene)
{
	if(scene->GetSceneRisk())
	{
		bool reEnter = scene->GetSceneRisk()->IsReEnter();
		if (!reEnter)
		{
			OnNormalDiceOver();
		}
	}
	if (scene->GetSceneType() == KKSG::SCENE_HALL)
	{
		if (inDice && !receiveOver)
		{
			//m_role->Get<CSuperRiskRecord>()->AddDiceNum(1);
			KKSG::PlayDiceOverRes temp;
			DiceOver(temp, true);
			LogInfo("%llu,%s,indice ,receive no over, changescene", m_role->GetID(), m_role->GetName().c_str());
		}
	}
}

bool SuperRisk::CheckReBattle()
{
	if(/*!inDice || */!sceneID)
		return false;
	if (!m_role->GetCurrScene() || m_role->GetCurrScene()->GetSceneType() != KKSG::SCENE_RISK)
	{
		return false;
	}

	return true;
}

UINT32 SuperRisk::ReEnterBattle()
{
	if (!CheckReBattle())
	{
		return KKSG::ERR_FAILED;
	}

	if (!m_role->GetCurrScene() || m_role->GetCurrScene()->GetSceneType() != KKSG::SCENE_RISK || m_role->GetCurrScene()->GetSceneState() != SCENE_LOSE)
	{
		return KKSG::ERR_FAILED;
	}

	BagTakeItemTransition transition(m_role);
	transition.SetReason(ItemFlow_SuperRisk, ItemFlow_SuperRiskRebattle);
	if (!transition.TakeItem( GetGlobalConfig().RiskReBattleCost.seq[0],  GetGlobalConfig().RiskReBattleCost[1]))
	{
		transition.RollBack();
		return KKSG::ERR_ITEM_NOT_ENOUGH;
	}	
	transition.NotifyClient();

	m_role->GetCurrScene()->GetSceneRisk()->SetReEnter(true);
	SceneManager::Instance()->ChangeScene(m_role, this->sceneID);

	return KKSG::ERR_SUCCESS;
}

bool SuperRisk::DebugSetDiceNum(int num)
{
	/*if (num > 6 || num < 1)
	{
		return false;
	}*/

	debugDiceNum = num;

	return true;
}

void SuperRisk::Load(const KKSG::RoleRiskInfo* info)
{
	if (info)
	{
		mapID = info->mapid();
		gridType = info->gridtype();
		sceneID = info->sceneid();
		canBuy = info->canbuy();
	}

}

void SuperRisk::Save(KKSG::RoleRiskInfo* info)
{
	if (info)
	{
		info->set_mapid(mapID);
		info->set_gridtype(gridType);
		info->set_sceneid(sceneID);
		info->set_canbuy(canBuy);
	}
}

void SuperRisk::Change()
{
	m_role->Get<CSuperRiskRecord>()->Change();
}
