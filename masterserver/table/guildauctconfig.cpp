#include "pch.h"
#include "guildauctconfig.h"
#include "config/itemdropconfig.h"
#include "global/GlobalConfigDBBuild.h"
INSTANCE_SINGLETON(GuildAuctConfig)

	GuildAuctConfig::GuildAuctConfig()
{

}

GuildAuctConfig::~GuildAuctConfig()
{

}

bool GuildAuctConfig::Init()
{
	return LoadFile();
}

bool GuildAuctConfig::Reload()
{
	Uninit();	
	return Init();
}

void GuildAuctConfig::Uninit()
{
	ClearFile();
}

bool GuildAuctConfig::CheckFile()
{
	GuildAuctReward tmpRewrad;
	if(!tmpRewrad.LoadFile("table/GuildAuctReward.txt"))
	{
		SSWarn << " load GuildAuctReward.txt failed " << END;
		return false;
	}
	GuildAuctItem tmpItem;
	if(!tmpItem.LoadFile("table/GuildAuctItem.txt"))
	{
		SSWarn << " load GuildAuctItem.txt failed " << END;
		return false;
	}
	return true;
}

bool GuildAuctConfig::LoadFile()
{
	if(!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_GAReward.LoadFile("table/GuildAuctReward.txt");
	m_GAItem.LoadFile("table/GuildAuctItem.txt");

	for(size_t i = 0; i < m_GAReward.Table.size(); ++i)
	{
		auto pConf = m_GAReward.Table[i];
		auto& tmp = m_actConf[pConf->ActType];
		tmp.actName = pConf->ActName;
		tmp.profitType = pConf->BonusType;
		tmp.flow = pConf->Flow;
		if(tmp.rankCount < pConf->Rank[1])
		{
			tmp.rankCount = pConf->Rank[1];
		}
	}

	GetSItemsByActType();

	return true;
}

void GuildAuctConfig::ClearFile()
{
	m_actConf.clear();
	m_GAReward.Clear();
	m_GAItem.Clear();
	certainly_items_.clear();
}

GuildAuctReward::RowData* GuildAuctConfig::GetItems(int actType, int rank, std::vector<ItemDesc>& items, UINT32 roleCount)
{	
	int world_level = convert<UINT32>(CGlobalConfigDBBuild::Instance()->GetConfigValue(eWorldLevel));
	for(size_t i = 0; i < m_GAReward.Table.size(); ++i)
	{
		auto conf = m_GAReward.Table[i];
		if(conf->ActType != actType)
		{
			continue;
		}
		if(conf->Rank[0] != 0 && !(conf->Rank[0] <= rank && rank <= conf->Rank[1]))
		{
			continue;
		}
		if(!(conf->WorldLevel[0] <= world_level && world_level <= conf->WorldLevel[1]))
		{
			continue;
		}

	
		if (roleCount > conf->DropNeedNum )
		{
			for(size_t j = 0; j < conf->DropID.size(); ++j)
			{
				int dropid = conf->DropID[j];
				auto desc = ItemDropConfig::Instance()->GetRandomDrop(dropid);
				if(desc)
				{
					items.push_back(*desc);
				}
			}
		}

		for(size_t j = 0; j < conf->SItem.size(); ++j)
		{
			ItemDesc desc(conf->SItem[j][0], conf->SItem[j][1], false);
			float sItemPro = GetSItemPo(roleCount,conf->SItem[j][0],conf->SItemPro);
			desc.itemCount = (UINT32)(desc.itemCount * sItemPro);
			if (desc.itemCount > 0 )
			{
				items.push_back(desc);
			}
		}
		return conf;
	}
	return NULL;
}

float GuildAuctConfig::GetSItemPo(const UINT32 role_count,const UINT32 item_id,vector<Sequence<float, 3>>& sitem_pro)
{
	float sItemPro = 0.0;
	auto it = sitem_pro.begin();
	auto it_end = sitem_pro.end();
	for (; it != it_end; ++it)
	{
		if (role_count >= (UINT32)it->seq[0])
		{
			if (item_id == (UINT32)it->seq[1])
			{
				sItemPro = it->seq[2];
			}
		}
	}

	return sItemPro;
}

GuildAuctItem::RowData* GuildAuctConfig::GetItemConf(UINT32 itemID)
{
	return m_GAItem.GetByItemID(itemID);
}

int GuildAuctConfig::GetItemType(UINT32 itemID)
{
	auto pConf = GetItemConf(itemID);
	if(pConf)
	{
		return pConf->ItemType;
	}
	return 0;
}

int GuildAuctConfig::GetProfitType(int actType)
{
	auto it = m_actConf.find(actType);
	if(it != m_actConf.end())
	{
		return (it->second).profitType;
	}
	return 0;
}

std::string GuildAuctConfig::GetActName(int actType)
{
	auto it = m_actConf.find(actType);
	if(it != m_actConf.end())
	{
		return (it->second).actName;
	}
	return "";
}

bool GuildAuctConfig::IsActFlow(int actType)
{
	auto it = m_actConf.find(actType);
	if(it != m_actConf.end())
	{
		return (bool)(it->second).flow;
	}
	return false;
}

int GuildAuctConfig::GetRankCount(int actType)
{
	auto it = m_actConf.find(actType);
	if(it != m_actConf.end())
	{
		return (it->second).rankCount;
	}
	return 0;
}

void GuildAuctConfig::GetSItemsByActType()
{
	for(size_t i = 0; i < m_GAReward.Table.size(); ++i)
	{
		auto conf = m_GAReward.Table[i];		
		for(size_t j = 0; j < conf->SItem.size(); ++j)
		{
			auto it = certainly_items_.find(conf->ActType);
			if (it != certainly_items_.end())
			{
				it->second.insert(conf->SItem[j][0]);
			}
			else
			{
				std::unordered_set<UINT32> item_set;
				item_set.insert(conf->SItem[j][0]);
				certainly_items_.insert(std::make_pair(conf->ActType,item_set));
			}			
		}
	}
}
