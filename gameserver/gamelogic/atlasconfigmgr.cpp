#include "pch.h"
#include "atlasconfigmgr.h"
#include "unit/role.h"
#include "gamelogic/bag.h"
#include "gamelogic/bagtransition.h"
#include "gamelogic/itemconfig.h"
#include "tablemgr/XEntityInfoLibrary.h"
#include "foreach.h"
#include "atlas.h"

INSTANCE_SINGLETON(CAtlasConfigMagr);
CAtlasConfigMagr::CAtlasConfigMagr()
{

}
CAtlasConfigMagr::~CAtlasConfigMagr()
{

}

bool CAtlasConfigMagr::Init()
{
	if (!m_cardsFire.LoadFile("table/CardsFireProperty.txt"))
	{
		SSWarn<<"load file table/CardsFireProperty.txt failed"<<END;
		return false;
	}
	if (!m_cardsGroup.LoadFile("table/CardsGroup.txt"))
	{
		SSWarn<<"load file table/CardsGroup.txt failed"<<END;
		return false;
	}
	if (m_cardsGroup.Table.size() != m_cardsGroup.TeamId_Index.size())
	{
		LogWarn("CardsGroup TeamId key has duplicate key");
		return false;
	}
	for (auto i = m_cardsGroup.Table.begin(); i != m_cardsGroup.Table.end(); i++)
	{
		std::vector<std::vector<std::pair<UINT32, UINT32>>>& condition = m_teamid2Condition[(*i)->TeamId];
		if ((*i)->StarFireCondition.empty())
		{
			LogWarn("CardsGroup, TeamId [%u], StarFireCondition is empty", (*i)->TeamId);
			return false;
		}
		for (UINT32 j = 0; j < (*i)->StarFireCondition.size();)
		{
			if ((*i)->StarFireCondition[j][0] == 0 || j + (*i)->StarFireCondition[j][0] >= (*i)->StarFireCondition.size())
			{
				LogWarn("CardsGroup, TeamId [%u], StarFireCondition format wrong", (*i)->TeamId);
				return false;
			}
			std::vector<std::pair<UINT32, UINT32>> vec;
			UINT32 end = j + (*i)->StarFireCondition[j][0];
			j++;
			while (j <= end)
			{
				vec.push_back(std::make_pair((*i)->StarFireCondition[j][0], (*i)->StarFireCondition[j][1]));
				j++;
			}
			condition.push_back(vec);
		}
	}
	if (!m_cardGroupList.LoadFile("table/CardsGroupList.txt"))
	{
		SSWarn<<"load file table/CardsGroupList.txt failed"<<END;
		return false;
	}
	if (!m_cardList.LoadFile("table/CardsList.txt"))
	{
		SSWarn<<"load file table/CardsList.txt failed"<<END;
		return false;
	}
	return true;
}

void CAtlasConfigMagr::Uninit()
{

}

bool CAtlasConfigMagr::IsGroupOpen(UINT32 groupId, UINT32 roleLvl)
{
	foreach (i in m_cardGroupList.Table)
	{
		if ((*i)->GroupId == groupId)
		{
			if ((*i)->OpenLevel <= roleLvl)
			{
				return true;
			}
			else
			{
				return false;
			}

		}
	}

	return false;
}

bool CAtlasConfigMagr::CanUpStar(UINT32 groupId, UINT32 lvl, UINT32 finishId)
{
	foreach (i in m_cardGroupList.Table)
	{
		if ((*i)->GroupId == groupId)
		{
			if (finishId >= (*i)->BreakLevel.size())
			{
				return false;
			}
			if ((*i)->BreakLevel[finishId] <= lvl)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

UINT32 CAtlasConfigMagr::GetUpStarTimes(UINT32 groupId)
{
	foreach (i in m_cardGroupList.Table)
	{
		if ((*i)->GroupId == groupId)
		{
			return (*i)->BreakLevel.size();
		}
	}
	return 0;
}

void CAtlasConfigMagr::GetTeamAttr(std::map<UINT32, UINT32>& allAttrMap, UINT32 teamId)
{
	/*
	foreach(i in m_cardsGroup.Table)
	{
		if ((*i)->TeamId == teamId)
		{
			foreach(j in (*i)->FireProperty)
			{
				allAttrMap[j->seq[0]] += j->seq[1];
			}
			return;
		}
	}*/
}

void CAtlasConfigMagr::GetNumAttr(std::map<UINT32, UINT32>& allAttrMap, UINT32 num, UINT32 groupId)
{
	foreach (i in m_cardsFire.Table)
	{
		if ((*i)->GroupId == groupId && (*i)->FireCounts <= num)
		{
			foreach (j in (*i)->Promote)
			{
				allAttrMap[j->seq[0]] = j->seq[1];
			}
		}
	}
}

const std::vector<std::pair<UINT32, UINT32>>* CAtlasConfigMagr::_FindTeamNeedCards(UINT32 teamId, UINT32 finishid)
{
	auto iter = m_teamid2Condition.find(teamId);
	if (iter == m_teamid2Condition.end())
	{
		return NULL;
	}
	if (finishid >= iter->second.size())
	{
		return NULL;
	}
	return &iter->second[finishid];
}

UINT32 CAtlasConfigMagr::TryCreateTeam(Role* pRole, UINT32 teamId, UINT32 finishid)
{
	if (pRole == NULL)
	{
		return KKSG::ERR_UNKNOWN;
	}

	const std::vector<std::pair<UINT32, UINT32>>* cardSet = _FindTeamNeedCards(teamId, finishid);
	if (cardSet == NULL || cardSet->size() == 0)
	{
		return KKSG::ERR_UNKNOWN;
	}

	BagTakeItemTransition take(pRole);
	take.SetReason(ItemFlow_Atlas, ItemFlow_CreateAtlasTeam);
	for (auto i = cardSet->begin(); i != cardSet->end(); i++)
	{
		if (!take.TakeItem(i->first, i->second))
		{
			take.RollBack();
			return KKSG::ATLAS_CARD_NOT_ENOUGH;
		}
	}
	take.NotifyClient();

	return KKSG::ERR_SUCCESS;
}

UINT32 CAtlasConfigMagr::GetGroupId(UINT32 teamId)
{
	CardsGroup::RowData* data = m_cardsGroup.GetByTeamId(teamId);
	return data == NULL ? 0 : data->GroupId;
}

void CAtlasConfigMagr::GetGroupItem(UINT32 groupId, std::set<UINT32>& itemSet)
{
	foreach (i in m_cardsGroup.Table)
	{
		if ((*i)->GroupId == groupId)
		{
			itemSet.insert((*i)->TeamId);
		}
	}
}

void CAtlasConfigMagr::GetAllAttr(UINT32 profession, std::map<UINT32, UINT32>& allAttrMap, const std::map<UINT32, std::set<UINT32> >& m_groupMap, const std::map<UINT32, UINT32>& m_finishRcd)
{
	std::map<UINT32, UINT32> tempAttr;
	foreach (i in m_groupMap)
	{
		tempAttr.clear();
		std::map<UINT32, UINT32> addPerMap; // 加成百分比
		UINT32 finishId = m_finishRcd.find(i->first) == m_finishRcd.end() ? 0 : m_finishRcd.find(i->first)->second; 
		_GetPerAdd(i->first, i->second.size(), finishId, addPerMap);

		foreach (j in i->second)
		{
			CardsGroup::RowData* k = m_cardsGroup.GetByTeamId(*j);
			if (k == NULL) continue;

			vector<Sequence<uint, 2>> attrVec;
			UINT32 attackType = XEntityInfoLibrary::Instance()->GetAttackType(profession);
			if (attackType == 1)
			{
				attrVec = k->FireProperty_1;
			}
			else if (attackType == 2)
			{
				attrVec = k->FireProperty_2;
			}
			if (attrVec.empty())
			{
				continue;
			}

			UINT32 attr = 0;
			UINT32 num = 0;
			if (attrVec.size() > finishId)
			{
				attr = attrVec[finishId].seq[0];
				num = attrVec[finishId].seq[1];
			}

			if (num == 0)
			{
				continue;
			}

		//	auto iter = addPerMap.find(attr);
		//	if (iter != addPerMap.end())
			//{
				//allAttrMap[attr] += num * (100 + iter->second) / 100; 
			//}
			//else
			//{
				tempAttr[attr] += num;
			//}
		}

		foreach (x in tempAttr)
		{
			auto iter = addPerMap.find(x->first);
			if (iter != addPerMap.end())
			{
				allAttrMap[x->first] += x->second * (100 + iter->second) / 100; 
			}
			else
			{
				allAttrMap[x->first] += x->second;
			}
		}
	}

	foreach (k in m_finishRcd)
	{
		for (UINT32 cnt = 0; cnt < k->second; ++cnt)
		{
			_GetGroupAttr(profession, k->first, cnt, allAttrMap);
		}
	}
}

void CAtlasConfigMagr::_GetGroupAttr(UINT32 profession, UINT32 groupId, UINT32 finishId, std::map<UINT32, UINT32>& allAttrMap)
{
	std::map<UINT32, UINT32> addPerMap; // 加成百分比
	_GetAllPer(groupId, finishId, addPerMap);

	std::map<UINT32, UINT32> tempAttr;

	foreach (i in m_cardsGroup.Table)
	{
		if ((*i)->GroupId != groupId)
		{
			continue ;
		}

		vector<Sequence<uint, 2>> attrVec;
		UINT32 attackType = XEntityInfoLibrary::Instance()->GetAttackType(profession);
		if (attackType == 1)
		{
			attrVec = (*i)->FireProperty_1;
		}
		else if (attackType == 2)
		{
			attrVec = (*i)->FireProperty_2;
		}
		if (attrVec.empty())
		{
			continue;
		}

		UINT32 attr = 0;
		UINT32 num = 0;
		if (attrVec.size() > finishId)
		{
			attr = attrVec[finishId].seq[0];
			num = attrVec[finishId].seq[1];
		}

		if (num == 0)
		{
			continue;
		}

		tempAttr[attr] += num;
		//allAttrMap[attr] += num;

		/*
		auto iter = addPerMap.find(attr);
		if (iter != addPerMap.end())
		{
			allAttrMap[attr] += num * (100 + iter->second) / 100; 
		}
		else
		{
			allAttrMap[attr] += num;
		}
		*/
	}

	foreach (j in tempAttr)
	{
		auto iter = addPerMap.find(j->first);
		if (iter != addPerMap.end())
		{
			allAttrMap[j->first] += j->second * (100 + iter->second) / 100; 
		}
		else
		{
			allAttrMap[j->first] += j->second;
		}
	}
}

void CAtlasConfigMagr::_GetPerAdd(UINT32 groupId, UINT32 num, UINT32 finishId, std::map<UINT32, UINT32>& addPerMap)
{
	foreach (i in m_cardsFire.Table)
	{
		if ((*i)->GroupId == groupId && (*i)->BreakLevel == finishId && (*i)->FireCounts <= num)
		{
			foreach (j in (*i)->Promote)
			{
				addPerMap[(j->seq[0] - 1000)] += j->seq[1];
			}
		}
	}
}

void CAtlasConfigMagr::_GetAllPer(UINT32 groupId, UINT32 finishId, std::map<UINT32, UINT32>& addPerMap)
{
	foreach (i in m_cardsFire.Table)
	{
		if ((*i)->GroupId == groupId && (*i)->BreakLevel == finishId)
		{
			foreach (j in (*i)->Promote)
			{
				addPerMap[(j->seq[0] - 1000)] += j->seq[1];
			}
		}
	}
}

void CAtlasConfigMagr::GetAddAttr(UINT32 profession, vector<Sequence<uint, 2>>& attrsVec, const std::set<UINT32>& m_groupSet)
{
	foreach (i in m_groupSet)
	{
		CardsGroup::RowData* k = m_cardsGroup.GetByTeamId(*i);
		if (k == NULL) continue;

		vector<Sequence<uint, 2>> attrVec;
		UINT32 attackType = XEntityInfoLibrary::Instance()->GetAttackType(profession);
		if (attackType == 1)
		{
			attrVec = k->FireProperty_1;
		}
		else if (attackType == 2)
		{
			attrVec = k->FireProperty_2;
		}

		foreach(n in attrVec)
		{
			attrsVec.push_back(*n);
			//allAttrMap[n->seq[0]] += n->seq[1];
		}
	}
}

void CAtlasConfigMagr::GetAddPer(std::map<UINT32, UINT32>& allPerMap, UINT32 groupId, UINT32 num, UINT32 finishId)
{
	foreach (i in m_cardsFire.Table)
	{
		if ((*i)->GroupId == groupId && (*i)->FireCounts <= num && (*i)->BreakLevel == finishId)
		{
			foreach (j in (*i)->Promote)
			{
				//allPerVec.push_back(*j);
				allPerMap[j->seq[0] - 1000] += j->seq[1];
			}
		}
	}
}

void CAtlasConfigMagr::GetQualitCards(UINT32 quailt, std::map<UINT32, UINT32>& cardsMap, UINT32 groupId)
{
	foreach(i in m_cardList.Table)
	{
		if ((*i)->GroupId == groupId && ItemConfig::Instance()->GetItemQuailty((*i)->CardId) == quailt)
		{
			cardsMap[(*i)->CardId] += 1;
		}
	}
}

UINT32 CAtlasConfigMagr::NeedCardTimes(Role* pRole, UINT32 cardId, const std::set<UINT32>& hasTeamid)
{
	if (pRole == NULL) return 0;

	UINT32 needNum = 0;
	for (auto i = m_teamid2Condition.begin(); i != m_teamid2Condition.end(); i++)
	{
		if (hasTeamid.find(i->first) != hasTeamid.end())
		{
			continue;
		}
		UINT32 finishid = pRole->Get<CAtlasSys>()->GetFinishId(GetGroupId(i->first));
		if (finishid >= i->second.size())
		{
			continue;
		}
		for (auto j = i->second[finishid].begin(); j != i->second[finishid].end(); j++)
		{
			if (cardId == j->first)
			{
				needNum += j->second;
			}
		}
	}
	return needNum;
}

