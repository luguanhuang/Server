#include "pch.h"
#include "fashionmgr.h"
#include "bag.h"
#include "rolefashion.h"
#include "unit/role.h"
#include "foreach.h"
#include "itemsuffix.h"
#include "unit/combatattribute.h"
#include "gamelogic/levelsealMgr.h"
#include "time.h"

INSTANCE_SINGLETON(FashionMgr)

	FashionMgr::FashionMgr()
{

}

FashionMgr::~FashionMgr()
{

}

bool FashionMgr::Init()
{
	return LoadFile();
}

void FashionMgr::Uninit()
{

}

bool FashionMgr::CheckFile()
{
	FashionSuitTable SuitTable;
	if (!SuitTable.LoadFile("table/FashionSuit.txt"))
	{
		SSWarn<<"load table/FashionSuit.txt failed"<<END;
		return false;
	}
	FashionComposeTable ComposeTable;
	if (!ComposeTable.LoadFile("table/FashionCompose.txt"))
	{
		SSWarn<<"load table/FashionCompose.txt failed"<<END;
		return false;
	}
	FashionComposeSet composeset;
	if (!composeset.LoadFile("table/FashionComposeSet.txt"))
	{
		SSWarn<<"load table/FashionComposeSet.txt failed"<<END;
		return false;
	}
	FashionEffectTable effecttable;
	if (!effecttable.LoadFile("table/FashionEffect.txt"))
	{
		SSWarn<<"load table/FashionEffect.txt failed"<<END;
		return false;
	}

	FashionCharm  fashion_charm;
	if (!fashion_charm.LoadFile("table/FashionCharm.txt"))
	{
		SSWarn<<"load table/FashionCharm.txt failed"<<END;
		return false;
	}

	FashionHair  fashion_hair;
	if (!fashion_hair.LoadFile("table/FashionHair.txt"))
	{
		SSWarn<<"load table/FashionHair.txt failed"<<END;
		return false;
	}

	FashionSynthesis  fashion_synthesis;
	if(!fashion_synthesis.LoadFile("table/FashionSynthesis.txt"))
	{
		SSWarn << " load FashionSynthesis.txt failed " << END;
		return false;
	}

	// 特效
	FashionSuitSpecialEffects  fashionsuit_specialeffects;
	if(!fashionsuit_specialeffects.LoadFile("table/FashionSuitSpecialEffects.txt"))
	{
		SSWarn << " load FashionSuitSpecialEffects.txt failed " << END;
		return false;
	}
	return true;
}

bool FashionMgr::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	m_SuitTable.LoadFile("table/FashionSuit.txt");
	m_ComposeTable.LoadFile("table/FashionCompose.txt");
	m_composeset.LoadFile("table/FashionComposeSet.txt");
	m_effecttable.LoadFile("table/FashionEffect.txt");
	fashion_charm_.LoadFile("table/FashionCharm.txt");
	fashion_hair_.LoadFile("table/FashionHair.txt");

	fashion_synthesis_.LoadFile("table/FashionSynthesis.txt");
	fashionsuit_specialeffects_.LoadFile("table/FashionSuitSpecialEffects.txt");

	for (unsigned i = 0; i < m_SuitTable.Table.size(); ++i)
	{
		FashionSuitTable::RowData *Row = m_SuitTable.Table[i];
		for (auto j = Row->FashionID.begin(); j != Row->FashionID.end(); ++j)
		{
			m_SuitSetID[*j] = Row->SuitID;
		}
	}
	// quality effect
	for (auto i = m_effecttable.Table.begin(); i != m_effecttable.Table.end(); ++i)
	{
		FashionEffectTable::RowData* data = *i;
		FashionSuitEffect effects;
		// 2
		if (!data->Effect2.empty() && 0 != data->Effect2[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[2], data->Effect2);
		}
		// 3
		if (!data->Effect3.empty() && 0 != data->Effect3[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[3], data->Effect3);
		}
		// 4
		if (!data->Effect4.empty() && 0 != data->Effect4[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[4], data->Effect4);
		}
		// 5
		if (!data->Effect5.empty() && 0 != data->Effect5[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[5], data->Effect5);
		}
		// 6
		if (!data->Effect6.empty() && 0 != data->Effect6[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[6], data->Effect6);
		}
		// 7
		if (!data->Effect7.empty() && 0 != data->Effect7[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[7], data->Effect7);
		}

		if (!data->IsThreeSuit)
		{
			m_qualityeffects[data->Quality] = effects;
		}
		else
		{
			m_threequalityeffects[data->Quality] = effects;
		}
	}
	// suit effect
	for (auto i = m_SuitTable.Table.begin(); i != m_SuitTable.Table.end(); ++i)
	{
		FashionSuitTable::RowData* data = *i;
		FashionSuitEffect effects;
		// 2
		if (!data->Effect2.empty() && 0 != data->Effect2[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[2], data->Effect2);
		}
		// 3
		if (!data->Effect3.empty() && 0 != data->Effect3[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[3], data->Effect3);
		}
		// 4
		if (!data->Effect4.empty() && 0 != data->Effect4[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[4], data->Effect4);
		}
		// 5
		if (!data->Effect5.empty() && 0 != data->Effect5[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[5], data->Effect5);
		}
		// 6
		if (!data->Effect6.empty() && 0 != data->Effect6[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[6], data->Effect6);
		}
		// 7
		if (!data->Effect7.empty() && 0 != data->Effect7[0].seq[0])
		{
			InitSuitEffect(effects.m_effect[7], data->Effect7);
		}

		m_suiteffects[data->SuitID] = effects;
	}

	time_t nowtime = time(NULL);
	tm* stTime = localtime(&nowtime);
	stTime->tm_hour = 0;
	stTime->tm_min = 0;
	stTime->tm_sec = 0;
	for (auto j = m_composeset.Table.begin(); j != m_composeset.Table.end(); ++j)
	{
		FashionComposeSet::RowData* data = *j;
		FashionSet set;	
		set.type = data->Type;
		set.levelseal = data->LevelSeal;
		set.fashion = data->FashionSet;

		stTime->tm_year = (convert<int>(data->Time[0])) - 1900;
		stTime->tm_mon = (UINT32)(convert<int>(data->Time[1])/100) - 1;
		stTime->tm_mday = (convert<int>(data->Time[1])%100);
		set.timebegin = (UINT32)mktime(stTime);

		stTime->tm_year = (convert<int>(data->Time[2])) - 1900;
		stTime->tm_mon = (UINT32)(convert<int>(data->Time[3])/100) - 1;
		stTime->tm_mday = (convert<int>(data->Time[3])%100);
		set.timeend = (UINT32)mktime(stTime);

		m_type2composeset.push_back(set);
	}


	suilt_items_.clear();
	charm_count_.clear();
	auto it_conf = fashion_charm_.Table.begin();
	for ( ; it_conf != fashion_charm_.Table.end(); ++it_conf)
	{		
		std::vector<UINT32> count;
		if ((*it_conf)->Effect1.size() > 0)
		{
			count.push_back(1);
		}
		if ((*it_conf)->Effect2.size() > 0)
		{
			count.push_back(2);
		}
		if ((*it_conf)->Effect3.size() > 0)
		{
			count.push_back(3);
		}
		if ((*it_conf)->Effect4.size() > 0)
		{
			count.push_back(4);
		}
		if ((*it_conf)->Effect5.size() > 0)
		{
			count.push_back(5);
		}
		if ((*it_conf)->Effect6.size() > 0)
		{
			count.push_back(6);
		}
		if ((*it_conf)->Effect7.size() > 0)
		{
			count.push_back(7);
		}
		charm_count_.insert(std::make_pair((*it_conf)->SuitID,count));

		std::unordered_set<UINT32> items;
		for (auto it = (*it_conf)->SuitParam.begin(); it != (*it_conf)->SuitParam.end(); ++it)
		{			
			suilt_items_[*it]=(*it_conf)->SuitID;
		}
	}
	BuildDesignation();

	return true;
}

void FashionMgr::BuildDesignation()
{
	m_mapEquipFashion.clear();
	for( auto i = m_SuitTable.Table.begin(); i != m_SuitTable.Table.end(); i++ )
	{
		UINT32 suitId = (*i)->SuitID;
		for( UINT32 inner = 0; inner < (*i)->FashionID.size(); inner++ )
		{
			UINT32 fashionId = (*i)->FashionID[inner];
			m_mapEquipFashion.insert( std::make_pair(fashionId, suitId) );
		}
	}
}


void FashionMgr::ClearFile()
{
	m_SuitTable.Clear();
	m_ComposeTable.Clear();
	m_composeset.Clear();
	m_effecttable.Clear();

	m_type2composeset.clear();
	m_SuitSetID.clear();
	m_suiteffects.clear();
	m_qualityeffects.clear();

	fashion_charm_.Clear();
	fashion_hair_.Clear();	

	fashion_synthesis_.Clear();
	fashionsuit_specialeffects_.Clear();
}

UINT32 FashionMgr::GetFashionSuitID(UINT32 dwFashionID)
{
	auto it = m_SuitSetID.find(dwFashionID);
	if (it == m_SuitSetID.end())
	{
		return 0;
	}
	else
	{
		return it->second;
	}
}

FashionSuitTable::RowData * FashionMgr::GetFashionSuitConf(UINT32 dwSuitID)
{
	for (unsigned i = 0; i < m_SuitTable.Table.size(); ++i)
	{
		FashionSuitTable::RowData *Row = m_SuitTable.Table[i];
		if ((UINT32)Row->SuitID == dwSuitID)
		{
			return Row;
		}
	}

	return NULL;
}


bool FashionMgr::GetFashionSuit(UINT32 nFashionID,UINT32 nSuitID)
{
	FashionSuitTable::RowData* pConf = m_SuitTable.GetBySuitID(nSuitID);
	if( NULL == pConf )
	{
		return false;
	}
	//当前物品是否属于配置时装
	auto itFindFashion = std::find(pConf->FashionID.begin(), pConf->FashionID.end(), nFashionID);
	if( itFindFashion == pConf->FashionID.end())
	{
		return false;
	}
	return true;
}


///>判断是否有完整时装
bool FashionMgr::HaveEntireFashion(Role* pRole, UINT32 suitId)
{
	FashionSuitTable::RowData* pFashionConfig = m_SuitTable.GetBySuitID(suitId);
	if( NULL == pFashionConfig )
		return false; 
	Bag* pBag = pRole->Get<Bag>();
	for( UINT32 i = 0; i < pFashionConfig->FashionID.size(); i++ )
	{
		UINT32 count = pBag->CountItemTotal(pFashionConfig->FashionID[i]);
		if( count == 0 )
		{
			return false; 
		}
	}
	return true; 
}


FashionComposeTable::RowData * FashionMgr::GetFashionComposeConf(UINT32 dwFashionID, int Level)
{
	for (unsigned i = 0; i < m_ComposeTable.Table.size(); ++i)
	{
		FashionComposeTable::RowData *Row = m_ComposeTable.Table[i];
		if ((UINT32)Row->FashionID == dwFashionID &&
			Row->FashionLevel == Level)
		{
			return Row;
		}
	}

	return NULL;
}

void FashionMgr::SuitEffect(FashionInfo* fashions[], UINT32 count, CombatAttribute* attrs)
{
	std::map<UINT32, UINT32> quality2count;
	std::map<UINT32, UINT32> threequality2count;
	//std::map<UINT32, UINT32> id2count;
	for (UINT32 i = 0; i < count; ++i)
	{
		if (NULL != fashions[i])
		{
			ItemConf* conf = ItemConfig::Instance()->GetItemConf(fashions[i]->itemID);
			if (NULL != conf)
			{
				if (i < FashionWings)
				{
					quality2count[conf->ItemQuality] += 1;
				}
				else
				{
					//UINT32 suitid = GetFashionSuitID(fashions[i]->itemID);
					//if (0 != suitid)
					//{
					//	id2count[suitid] += 1;
					//}

					UINT32 suitid = GetFashionSuitID(fashions[i]->itemID);
					if (0 != suitid)
					{
						threequality2count[conf->ItemQuality] += 1;
					}
				}
			}
		}
	}
	// quality
	for (auto k = quality2count.begin(); k != quality2count.end(); ++k)
	{
		QualityEffect(k->first, k->second, attrs, false);
	}
	// three quality
	for (auto k = threequality2count.begin(); k != threequality2count.end(); ++k)
	{
		QualityEffect(k->first, k->second, attrs, true);
	}

	// suit
	//for (auto k = id2count.begin(); k != id2count.end(); ++k)
	//{
	//	SuitEffect(k->first, k->second, attrs);
	//}

}

void FashionMgr::SuitEffect(UINT32 suitid, UINT32 count, CombatAttribute* attrs)
{
	auto i = m_suiteffects.find(suitid);
	if (i != m_suiteffects.end())
	{
		for (auto j = i->second.m_effect.begin(); j != i->second.m_effect.end(); ++j)
		{
			if (j->first > count)
			{
				break;
			}
			ApplyEffectAux(j->second, attrs);
		}
	}
}

void FashionMgr::QualityEffect(UINT32 quality, UINT32 count, CombatAttribute* attrs, bool isthree)
{
	if (!isthree)
	{
		auto i = m_qualityeffects.find(quality);
		if (i != m_qualityeffects.end())
		{
			for (auto j = i->second.m_effect.begin(); j != i->second.m_effect.end(); ++j)
			{
				if (j->first > count)
				{
					break;
				}
				ApplyEffectAux(j->second, attrs);
			}
		}
	}
	else
	{
		auto i = m_threequalityeffects.find(quality);
		if (i != m_threequalityeffects.end())
		{
			for (auto j = i->second.m_effect.begin(); j != i->second.m_effect.end(); ++j)
			{
				if (j->first > count)
				{
					break;
				}
				ApplyEffectAux(j->second, attrs);
			}
		}
	}
}

bool FashionMgr::InitSuitEffect(std::vector<ItemChangeAttr>& attrs, std::vector<Sequence<UINT32, 2>>& conf)
{
	for (auto i = conf.begin(); i != conf.end(); ++i)
	{
		attrs.push_back(ItemChangeAttr(i->seq[0], i->seq[1]));
	}
	return !conf.empty();
}

UINT32 FashionMgr::RandomComposeFashion()
{
	UINT32 now = (UINT32)time(NULL);
	UINT32 levelseal = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;

	UINT32 value = 0;
	std::vector<Sequence<UINT32, 2>> fashions;
	for (auto i = m_type2composeset.begin(); i != m_type2composeset.end(); ++i)
	{
		if (i->Valid(levelseal, now))
		{
			value += i->Fill(fashions);
		}
	}
	UINT32 random = XRandom::randInt(1, value + 1);
	UINT32 temp = 0;
	UINT32 fashionid = 0;
	for (auto j = fashions.begin(); j != fashions.end(); ++j)
	{
		temp += j->seq[1];
		if (random <= temp)
		{
			fashionid = j->seq[0];	
			break;
		}
	}
	SSInfo<<"random compose fashion:"<<fashionid<<" random:"<<random<<END;

	return fashionid;
}

void FashionMgr::AddFashionCharmAttrs(vector<Sequence<uint, 2>>& add_attribute, CombatAttribute* attrs)
{
	for (auto it = add_attribute.begin(); it != add_attribute.end(); ++it)
	{
		LogDebug(" attrDef = %u  value= %u",it->seq[0], it->seq[1]);
		attrs->AddAttr((CombatAttrDef)it->seq[0], it->seq[1]);
	}
}
void FashionMgr::FashionActivateEffect(std::vector<SuitDisplayStore>& active_charm,CombatAttribute* attrs)
{
	vector<Sequence<uint, 2>> add_all_attribute;
	for (auto it = active_charm.begin(); it != active_charm.end(); ++it)
	{
		if (it->count <= 0)
		{
			continue;
		}
		bool exist = false;
		auto it_conf = fashion_charm_.Table.begin();
		for ( ; it_conf != fashion_charm_.Table.end(); ++it_conf)
		{
			if ((*it_conf)->SuitID == it->suit_id)
			{
				exist = true;
				break;
			}
		}
		if (!exist)
		{
			continue;
		}
		if (it->count >= 1)
		{
			for (auto exist_it = (*it_conf)->Effect1.begin(); exist_it != (*it_conf)->Effect1.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}
		}
		if (it->count >= 2)
		{
			for (auto exist_it = (*it_conf)->Effect2.begin(); exist_it != (*it_conf)->Effect2.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}
		}

		if (it->count >= 3)
		{
			for (auto exist_it = (*it_conf)->Effect3.begin(); exist_it != (*it_conf)->Effect3.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}
		}
		if (it->count >= 4)
		{
			for (auto exist_it = (*it_conf)->Effect4.begin(); exist_it != (*it_conf)->Effect4.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}
		}
		if (it->count >= 5)
		{
			for (auto exist_it = (*it_conf)->Effect5.begin(); exist_it != (*it_conf)->Effect5.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}
		}
		if (it->count >= 6)
		{
			for (auto exist_it = (*it_conf)->Effect6.begin(); exist_it != (*it_conf)->Effect6.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}
		}
		if (it->count >= 7)
		{
			for (auto exist_it = (*it_conf)->Effect7.begin(); exist_it != (*it_conf)->Effect7.end(); ++exist_it)
			{
				Sequence<uint, 2> temp_seq;
				temp_seq.seq[0] = exist_it->seq[0];
				temp_seq.seq[1] = exist_it->seq[1];
				add_all_attribute.push_back(temp_seq);
			}	
		}
	}

	AddFashionCharmAttrs(add_all_attribute,attrs);
}

UINT32 FashionMgr::GetSuitActiveLevel(UINT32 suit_id)
{
	auto it_conf = fashion_charm_.Table.begin();
	for ( ; it_conf != fashion_charm_.Table.end(); ++it_conf)
	{
		if ((*it_conf)->SuitID == suit_id)
		{
			return (*it_conf)->Level;
		}
	}

	return 0;
}

bool FashionMgr::NeedActiveFashionSuit(UINT32 suit_id)
{
	auto it_conf = fashion_charm_.Table.begin();
	for ( ; it_conf != fashion_charm_.Table.end(); ++it_conf)
	{
		if ((*it_conf)->SuitID == suit_id)
		{
			return true;
		}
	}

	return false;
}


UINT32 FashionMgr::GetActiveCount(UINT32 suit_id,UINT32 have_items)
{
	UINT32 allow_count = 0;
	auto it = charm_count_.find(suit_id);
	if (it != charm_count_.end())
	{
		auto count_it = it->second.begin();
		auto count_it_end = it->second.end();
		for (; count_it != count_it_end; ++count_it)
		{
			if (have_items >= *count_it && allow_count <= *count_it)
			{
				allow_count = *count_it;
			}
		}
	}

	return allow_count;
}


void FashionMgr::GetSuitItems(UINT32 suit_id, std::unordered_set<UINT32>& items)
{
	/*auto it_conf = fashion_charm_.Table.begin();
	for ( ; it_conf != fashion_charm_.Table.end(); ++it_conf)
	{
	if ((*it_conf)->SuitID == suit_id)
	{
	for (auto it = (*it_conf)->SuitParam.begin(); it != (*it_conf)->SuitParam.end(); ++it)
	{
	items.insert(*it);
	}
	}
	}*/
}


UINT32 FashionMgr::GetSuitIdForCharm(UINT32 item_id)
{
	auto it =suilt_items_.find(item_id);
	if (it != suilt_items_.end())
	{
		return it->second;
	}
	return 0;
}

FashionCharm::RowData* FashionMgr::GetFashionCharmConf(UINT32 dwSuitID)
{
	return fashion_charm_.GetBySuitID(dwSuitID);
}


void FashionMgr::GetColorByHair(UINT32 hair_id,std::unordered_set<UINT32>& colors)
{
	for (auto it = fashion_hair_.Table.begin(); it != fashion_hair_.Table.end(); ++it)
	{
		if ((*it)->HairID == hair_id)
		{
			auto color_it = (*it)->UnLookColorID.begin();
			auto color_it_end = (*it)->UnLookColorID.end();
			for (; color_it != color_it_end; ++color_it)
			{
				colors.insert(*color_it);
			}
			colors.insert((*it)->DefaultColorID);
			break;
		}
	}
}

void FashionMgr::GetHairColorCost(UINT32 hair_id,UINT32 hair_color,std::vector<ItemDesc>& items)
{
	for (auto it = fashion_hair_.Table.begin(); it != fashion_hair_.Table.end(); ++it)
	{
		if ((*it)->HairID == hair_id)
		{
			auto cost_it = (*it)->Cost.begin();
			auto cost_it_end = (*it)->Cost.end();
			for (; cost_it != cost_it_end; ++cost_it)
			{
				if ( hair_color == cost_it->seq[0])
				{
					ItemDesc desc;
					desc.itemID = cost_it->seq[1];
					desc.itemCount = cost_it->seq[2];
					items.push_back(desc);
				}					
			}			
		}
	}
}


UINT32 FashionMgr::GetDefaultHairColor(UINT32 hair_id)
{
	for (auto it = fashion_hair_.Table.begin(); it != fashion_hair_.Table.end(); ++it)
	{
		if ((*it)->HairID == hair_id)
		{
			return (*it)->DefaultColorID;
		}
	}

	return 0;
}



///>指定时装ID, 获取该时装套数
UINT32 FashionMgr::GetFashionCountBySuitID(Role* pRole, UINT32 suitId)
{
	FashionSuitTable::RowData* pSuitData = m_SuitTable.GetBySuitID(suitId);
	if( NULL == pSuitData )
		return 0; 
	Bag* pBag = pRole->Get<Bag>();
	UINT32 fashionNum = (UINT32)pSuitData->FashionID.size();
	std::map<UINT32, UINT32> mapNum;
	for( UINT32 i=0; i<fashionNum; i++ )
	{
		UINT32 itemID = (UINT32)pSuitData->FashionID[i];
		UINT32 itemCount = pBag->CountItemTotal(itemID);
		mapNum.insert( std::make_pair(itemID, itemCount) );
	}

	if( mapNum.empty())
		return 0; 

	///>如果套装中有某件物品的个数为0，则suitCount最后为0
	UINT32 suitCount = mapNum.begin()->second;
	for( auto iter=mapNum.begin(); iter!= mapNum.end(); iter++)
	{
		if( suitCount < iter->second )
			continue; 
		suitCount = iter->second;
	}
	return suitCount;
}

///>获取时装数量：整套时装数量
UINT32 FashionMgr::GetFashionCount(Role* pRole)
{
	UINT32 total = 0; 
	for( auto i= m_SuitTable.Table.begin(); i!= m_SuitTable.Table.end(); i++ )
	{
		int suitId = (*i)->SuitID;
		UINT32 curNum = this->GetFashionCountBySuitID(pRole, (UINT32)suitId);
		total += curNum;
	}
	return total;
}


/////>获取某件时装所在套装的品质：如果没有完整的套装，则返回false
bool FashionMgr::GetFashionQuality(Role* pRole, UINT32 fashionId, UINT32& quality)
{
	quality = 0; 
	auto itFind = m_mapEquipFashion.find(fashionId);
	if( itFind == m_mapEquipFashion.end() )
		return false; 
	FashionSuitTable::RowData* pData = m_SuitTable.GetBySuitID(itFind->second);
	if( NULL == pData )
		return false; 
	UINT32 count = GetFashionCountBySuitID(pRole, itFind->second);
	if( 0 == count )
		return false; 
	quality = pData->SuitQuality;
	return true; 
}

FashionSynthesis::RowData* FashionMgr::GetFashinSynthesisinfo(const UINT32 fashion_id)
{
	for (auto it = fashion_synthesis_.Table.begin(); it != fashion_synthesis_.Table.end(); ++it)
	{
		if ((*it)->FashionID == fashion_id)
		{
			return *it;
		}
	}

	return NULL;
}


UINT32 FashionMgr::GetSpecialEffectsIDBySuitID(UINT32 suit_id)
{
	auto it = fashionsuit_specialeffects_.Table.begin();
	auto it_end = fashionsuit_specialeffects_.Table.end();
	for (; it != it_end; ++it)
	{
		if ((*it)->suitid == suit_id)
		{
			return (*it)->specialeffectsid;
		}
	}

	return 0;
}

UINT32 FashionMgr::GetSuitIDBySpecialEffectsID(UINT32 special_effects_id)
{
	auto it = fashionsuit_specialeffects_.Table.begin();
	auto it_end = fashionsuit_specialeffects_.Table.end();
	for (; it != it_end; ++it)
	{
		if ((*it)->specialeffectsid == special_effects_id)
		{
			return (*it)->suitid;
		}
	}

	return 0;
}