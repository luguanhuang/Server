#include "pch.h"
#include "pandoraconfig.h"
#include "config/itemdropconfig.h"

INSTANCE_SINGLETON(PandoraConfig)

PandoraConfig::PandoraConfig()
{

}

PandoraConfig::~PandoraConfig()
{

}

bool PandoraConfig::Init()
{
	return LoadFile();
}

void PandoraConfig::Uninit()
{
	
}

static bool HandlerTableData(PandoraHeart& heartTable, std::map<UINT32, std::vector<PandoraHeartConf*>>& mapPandora,
					PandoraHeartReward& rewardTable, std::map<UINT32, std::map<UINT64, PandoraHeartRewardConf*>>& mapReward,
					PandoraDropList& dropTable, std::map<UINT32, std::vector<PandoraDropData>>& mapDrop)
{
	if (!dropTable.LoadFile("table/PandoraDropList.txt"))
	{
		LogWarn("Load PandoraDropList.txt failed");
		return false;
	}
	for (auto i = dropTable.Table.begin(); i != dropTable.Table.end(); ++i)
	{
		mapDrop[(*i)->PandoraDropID].push_back(PandoraDropData());

		PandoraDropData& drop = mapDrop[(*i)->PandoraDropID].back();
		drop.conf = *i;

		if ((*i)->CommonDropID == (*i)->BetterDropID && std::find((*i)->BestDropID.begin(), (*i)->BestDropID.end(), (*i)->BetterDropID) != (*i)->BestDropID.end())
		{
			LogWarn("PandoraDropID:%u, three dropID equal, please use DropList, not PandoraDropList", (*i)->PandoraDropID);
			return false;
		}

		if ((*i)->BestDropID.empty())
		{
			LogWarn("PandoraDrop:%u, BestDropID is empty", (*i)->PandoraDropID);
			return false;
		}

		if ((*i)->BestDropID.size() != (*i)->EnterBestDropTimes.size())
		{
			LogWarn("PandoraDrop:%u, BestDropID size:%u != EnterBestDropTimes size:%u", (*i)->PandoraDropID, (*i)->BestDropID.size(), (*i)->EnterBestDropTimes.size());
			return false;
		}

		int hasNullItem;
		std::vector<ItemDesc> vecItems;

		for (auto j = (*i)->BestDropID.begin(); j != (*i)->BestDropID.end(); ++j)
		{
			drop.bestDrop.push_back(std::set<std::pair<UINT32, UINT32>>());
			std::set<std::pair<UINT32, UINT32>>& bestDropSet = drop.bestDrop.back();
			
			vecItems.clear();
			ItemDropConfig::Instance()->GetAllDropItem(*j, vecItems, &hasNullItem);
			if (vecItems.empty())
			{
				LogWarn("PandoraDropID:%u, can't find dropid:%u in droplist", (*i)->PandoraDropID, *j);
				return false;
			}
			if (hasNullItem == 1)
			{
				LogWarn("PandoraDropID:%u, BestDropID:%u shouldn't has itemid 0 drop", (*i)->PandoraDropID, *j);
				return false;
			}
			for (UINT32 k = 0; k < vecItems.size(); ++k)
			{
				bestDropSet.insert(std::make_pair(vecItems[k].itemID, vecItems[k].itemCount));
			}
		}

		vecItems.clear();
		ItemDropConfig::Instance()->GetAllDropItem((*i)->BetterDropID, vecItems, &hasNullItem);
		if (vecItems.empty())
		{
			LogWarn("PandoraDropID:%u, can't find dropid:%u in droplist", (*i)->PandoraDropID, (*i)->BetterDropID);
			return false;
		}
		if (hasNullItem == 1)
		{
			LogWarn("PandoraDropID:%u, BetterDropID:%u shouldn't has itemid 0 drop", (*i)->PandoraDropID, (*i)->BetterDropID);
			return false;
		}
		for (UINT32 j = 0; j < vecItems.size(); ++j)
		{
			std::pair<UINT32, UINT32> itemPair = std::make_pair(vecItems[j].itemID, vecItems[j].itemCount);
			bool canFind = false;
			for (auto k = drop.bestDrop.begin(); k != drop.bestDrop.end(); ++k)
			{
				if ((*k).find(itemPair) != (*k).end())
				{
					canFind = true;
					break;
				}
			}
			if (!canFind)
			{
				drop.betterDrop.insert(itemPair);
			}
		}
	}

	if (!heartTable.LoadFile("table/PandoraHeart.txt"))
	{
		LogWarn("Load PandoraHeart.txt failed!");
		return false;
	}
	std::unordered_set<UINT32> heart;
	for (auto i = heartTable.Table.begin(); i != heartTable.Table.end(); i++)
	{
		heart.insert((*i)->PandoraID);
		mapPandora[(*i)->PandoraID].push_back(*i);
	}
	for (auto i = heartTable.Table.begin(); i != heartTable.Table.end(); i++)
	{
		if (heart.find((*i)->FireID) != heart.end())
		{
			LogWarn("PandoraID:%u 's FireID:%u error, it's also PandoraID", (*i)->PandoraID, (*i)->FireID);
			return false;
		}
	}

	if (!rewardTable.LoadFile("table/PandoraHeartReward.txt"))
	{
		LogWarn("Load PandoraHeartReward.txt failed!");
		return false;
	}
	for (auto i = rewardTable.Table.begin(); i != rewardTable.Table.end(); i++)
	{
		mapReward[(*i)->pandoraid][PandoraConfig::Instance()->CombinKey((*i)->itemid, (*i)->itemcount)] = *i;
	}

	return true;
}

bool PandoraConfig::CheckFile()
{
	PandoraHeart heartTable; std::map<UINT32, std::vector<PandoraHeartConf*>> mapPandora;
	PandoraHeartReward rewardTable; std::map<UINT32, std::map<UINT64, PandoraHeartRewardConf*>> mapReward;
	PandoraDropList dropTable; std::map<UINT32, std::vector<PandoraDropData>> mapDrop;

	return HandlerTableData(heartTable, mapPandora, rewardTable, mapReward, dropTable, mapDrop);
}

bool PandoraConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	HandlerTableData(m_heartTable, m_mapHeart, m_oRewardTable, m_mapReward, m_dropTable, m_mapDrop);

	return true;
}

void PandoraConfig::ClearFile()
{
	m_dropTable.Clear();
	m_mapDrop.clear();

	m_heartTable.Clear();
	m_mapHeart.clear();	

	m_oRewardTable.Clear();
	m_mapReward.clear();
}

const PandoraDropData* PandoraConfig::GetPandoraDropData(UINT32 pandoraDropID, UINT32 profID)
{
	auto iter = m_mapDrop.find(pandoraDropID);
	if (iter == m_mapDrop.end())
	{
		LogError("pandoraDropID:%u not in PandoraDrop.txt", pandoraDropID);
		return NULL;
	}
	if (iter->second.size() == 1)
	{
		return &iter->second[0];
	}
	for (UINT32 i = 0; i < iter->second.size(); i++)
	{
		if (iter->second[i].conf->ProfID == profID)
		{
			return &iter->second[i];
		}
	}
	LogError("pandoraDropID:%u, profID:%u can't find in PandoraDrop.txt", pandoraDropID, profID);
	return NULL;
}

const PandoraHeartConf* PandoraConfig::GetPandoraHeartConf(UINT32 pandoraID, UINT32 profID)
{	
	auto iter = m_mapHeart.find(pandoraID);
	if (iter == m_mapHeart.end())
	{
		LogError("pandoraID:%u not in PandoraHeart.txt", pandoraID);
		return NULL;
	}
	if (iter->second.size() == 1)
	{
		return iter->second[0];
	}
	for (UINT32 i = 0; i < iter->second.size(); i++)
	{
		if (iter->second[i]->ProfID == profID)
		{
			return iter->second[i];
		}
	}
	LogError("pandoraID:%u, profID:%u can't find in PandoraHeart.txt", pandoraID, profID);
	return NULL;
}

const PandoraHeartRewardConf* PandoraConfig::GetRewardConf(UINT32 pandoraID, const ItemDesc& item)
{
	auto iter = m_mapReward.find(pandoraID);
	if (iter == m_mapReward.end())
	{
		return NULL;
	}
	auto iter2 = iter->second.find(CombinKey(item.itemID, item.itemCount));
	if (iter2 == iter->second.end())
	{
		return NULL;
	}
	return iter2->second;
}

UINT32 PandoraConfig::GetItemQuality(UINT32 pandoraDropID, UINT32 profID, const ItemDesc& item)
{
	const PandoraDropData* dropData = GetPandoraDropData(pandoraDropID, profID);
	if (dropData == NULL)
	{
		return 1;
	}
	std::pair<UINT32, UINT32> itemPair = std::make_pair(item.itemID, item.itemCount);
	for (UINT32 i = 0; i < dropData->bestDrop.size(); ++i)
	{
		if (dropData->bestDrop[i].find(itemPair) != dropData->bestDrop[i].end())
		{
			return 3 + i;
		}
	}
	if (dropData->betterDrop.find(itemPair) != dropData->betterDrop.end())
	{
		return 2;
	}
	return 1;
}