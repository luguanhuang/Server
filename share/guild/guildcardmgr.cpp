#include "pch.h"
#include "guildcardmgr.h"
#include "util/XRandom.h"
#include "util/XCommon.h"
#include "define/itemdef.h"


INSTANCE_SINGLETON(GuildCardMgr);

GuildCardMgr::GuildCardMgr()
{

}

GuildCardMgr::~GuildCardMgr()
{

}

bool GuildCardMgr::Init()
{
	if (!m_storetable.LoadFile("table/CardStore.txt"))
	{
		SSWarn<<"load file table/CardStore.txt failed"<<END;
		return false;
	}
	if (!m_rewardtable.LoadFile("table/CardReward.txt"))
	{
		SSWarn<<"load file table/CardReward.txt failed"<<END;
		return false;
	}
	if (!m_rankrewardtable.LoadFile("table/CardRankReward.txt"))
	{
		SSWarn<<"load file table/CardRankReward.txt failed"<<END;
		return false;
	}
	for (auto i = m_rankrewardtable.Table.begin(); i != m_rankrewardtable.Table.end(); ++i)
	{
		CardRankRewardTable::RowData* rowdata = *i;
		std::vector<ItemDesc> items;
		bool exist = false;

		for (auto it = levelseal_rankd2items_.begin(); it != levelseal_rankd2items_.end(); ++it)
		{
			if (it->levelseal[0] == rowdata->levelseal[0] && it->levelseal[1] == rowdata->levelseal[1])
			{
				items.clear();				
				for (auto j = rowdata->reward.begin(); j != rowdata->reward.end(); ++j)
				{
					items.push_back(ItemDesc(j->seq[0], j->seq[1]));
				}
				it->m_rankd2items[rowdata->type][rowdata->rank] = items;
				exist = true;
			}
		}
		if (!exist)
		{
			GuildCardRankRewardItem  rank_reward;
			rank_reward.levelseal[0] = rowdata->levelseal[0];
			rank_reward.levelseal[1] = rowdata->levelseal[1];
			for (auto j = rowdata->reward.begin(); j != rowdata->reward.end(); ++j)
			{
				items.push_back(ItemDesc(j->seq[0], j->seq[1]));
			}
			rank_reward.m_rankd2items[rowdata->type][rowdata->rank] = items;
			levelseal_rankd2items_.push_back(rank_reward);
		}
	}
	return true;
}

void GuildCardMgr::Uninit()
{

}

UINT32 GuildCardMgr::RandomStoreType()
{
	UINT32 random = XRandom::randInt(1, 101);
	UINT32 count = 1;
	for (auto i = m_storetable.Table.begin(); i != m_storetable.Table.end(); ++i)
	{
		CardStoreTable::RowData* rowdata = *i;
		count += rowdata->probability;
		if (random <= count)
		{
			return rowdata->store;
		}
	}
	return 0;
}

void GuildCardMgr::GetReward(UINT32 type, std::vector<ItemDesc>& items)
{
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CardRewardTable::RowData* rowdata = *i;
		if (rowdata->type == type)
		{
			for (auto j = rowdata->reward.begin(); j != rowdata->reward.end(); ++j)
			{
				items.push_back(ItemDesc((int)(j->seq[0]), (int)(j->seq[1])));
			}
			break;
		}
	}
}

void GuildCardMgr::GetMatchReward(UINT32 type, std::vector<ItemDesc>& items)
{
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CardRewardTable::RowData* rowdata = *i;
		if (rowdata->type == type)
		{
			for (auto j = rowdata->matchreward.begin(); j != rowdata->matchreward.end(); ++j)
			{
				items.push_back(ItemDesc((int)(j->seq[0]), (int)(j->seq[1])));
			}
			break;
		}
	}
}
void GuildCardMgr::GetPokerTournamentMatchReward(UINT32 type, std::vector<ItemDesc>& items)
{
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CardRewardTable::RowData* rowdata = *i;
		if (rowdata->type == type)
		{
			for (auto j = rowdata->jokerreward.begin(); j != rowdata->jokerreward.end(); ++j)
			{
				items.push_back(ItemDesc((int)(j->seq[0]), (int)(j->seq[1])));
			}
			break;
		}
	}
}


UINT32 GuildCardMgr::GetExp(UINT32 result)
{
	UINT32 defaultexp = 0;
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CardRewardTable::RowData* rowdata = *i;
		defaultexp = rowdata->exp;
		if (rowdata->type == result)
		{
			return rowdata->exp;
		}
	}
	return defaultexp;
}

UINT32 GuildCardMgr::GetResult(std::vector<UINT32>& cards)
{
	if (cards.size() != CARDCOUNT)
	{
		return CardDummy;
	}
	std::sort(cards.begin(), cards.end());
	std::map<UINT32, std::vector<UINT32>> tempcards;
	UINT32 type = UINT32(-1);
	bool issame = true;
	for (auto i = cards.begin(); i != cards.end(); ++i)
	{
		UINT32 card = *i;
		UINT32 num = (card >> 4);
		tempcards[num].push_back(card);

		UINT32 temptype = (card & 0x0f);
		if (type == (UINT32)(-1))
		{
			type = temptype;
		}
		else
		{
			if (temptype != type)
			{
				issame = false;
			}
		}
	}

	UINT32 tempsame1 = 0;
	UINT32 tempsame2 = 0;
	for (auto i = tempcards.begin(); i != tempcards.end(); ++i)
	{
		UINT32 size = i->second.size();
		if (size > 1)
		{
			if (0 == tempsame1)
			{
				tempsame1 = size;
			}
			else
			{
				tempsame2 = size;
			}
		}
	}

	if (0 == tempsame1 && 0 == tempsame2)
	{
		UINT32 firstnum = cards[0] >> 4;
		UINT32 lastnum = cards[CARDCOUNT - 1] >> 4;
		UINT32 secondlast = cards[CARDCOUNT - 2] >> 4;
		bool isstraight = ((4 == (lastnum - firstnum)) 
			|| (((lastnum == CardA)) && (firstnum == Card2) && (3 == (secondlast - firstnum))));
		if (issame)
		{
			return isstraight ? CardABCDE : Cardaaaaa;
		}
		else
		{
			return isstraight ? Cardabcde : CardDummy;
		}
	}
	else
	{
		if (4 == tempsame1 || 4 == tempsame2)
		{
			return CardAAAAB;
		}
		else if (3 == tempsame1 || 3 == tempsame2)
		{
			if (2 == tempsame1 || 2 == tempsame2)
			{
				return CardAAABB;
			}
			else
			{
				return CardAAABC;
			}
		}
		else if (2 == tempsame1 && 2 == tempsame2)
		{
			return CardAABBC;
		}
		else if (2 == tempsame1 || 2 == tempsame2)
		{
			return CardAABCD;
		}
	}
	return CardDummy;
}

bool GuildCardMgr::Compare(UINT32 result, std::vector<UINT32>& firstcards, std::vector<UINT32>& secondcards)
{
	if (firstcards.size() != CARDCOUNT || secondcards.size() != CARDCOUNT)
	{
		return true;
	}
	std::vector<UINT32> first = Shrink(result, firstcards);
	std::vector<UINT32> second = Shrink(result, secondcards);

	return CompareSingle(first, second);
}

std::vector<UINT32> GuildCardMgr::Shrink(UINT32 result, std::vector<UINT32>& cards)
{
	std::map<UINT32, std::vector<UINT32>> temp;
	for (auto i = cards.begin(); i != cards.end(); ++i)
	{
		UINT32 card = *i;
		UINT32 num = (card >> 4);
		temp[num].push_back(card);
	}
	UINT32 four = 0;
	UINT32 three = 0;
	std::vector<UINT32> two;
	std::vector<UINT32> single;
	for (auto i = temp.rbegin(); i != temp.rend(); ++i)
	{
		switch (i->second.size())
		{
		case 1:
			{
				single.push_back(i->first);
				break;
			}
		case 2:
			{
				two.push_back(i->first);
				break;
			}
		case 3:
			{
				three = i->first;
				break;
			}
		case 4:
			{
				four = i->first;
				break;
			}
		default:
			{
				break;
			}
		}
	}
	std::vector<UINT32> tempcards;
	switch(result)
	{
	case CardABCDE:
	case Cardabcde:
		{
			if (single.size() == 5)
			{
				if (single[0] == CardA && single[1] == Card5)
				{
					tempcards.push_back(single[1]);					
				}
				else
				{
					tempcards.push_back(single[0]);
				}
			}
			break;
		}
	case CardAAAAB:
		{
			tempcards.push_back(four);
			if (single.size() == 1)
			{
				tempcards.push_back(single[0]);
			}
			break;
		}
	case CardAAABB:
		{
			tempcards.push_back(three);
			if (two.size() == 1)
			{
				tempcards.push_back(two[0]);
			}
			break;
		}
	case Cardaaaaa:
		{
			return single;
			break;
		}
	case CardAAABC:
		{
			tempcards.push_back(three);
			if (single.size() == 2)
			{
				tempcards.push_back(single[0]);
				tempcards.push_back(single[1]);
			}
			break;
		}
	case CardAABBC:
		{
			if (two.size() == 2)
			{
				tempcards.push_back(two[0]);
				tempcards.push_back(two[1]);
			}
			if (single.size() == 1)
			{
				tempcards.push_back(single[0]);
			}
			break;
		}
	case CardAABCD:
		{
			if (two.size() == 1)
			{
				tempcards.push_back(two[0]);
			}
			if (single.size() == 3)
			{
				tempcards.push_back(single[0]);
				tempcards.push_back(single[1]);
				tempcards.push_back(single[2]);
			}
			break;
		}
	case CardDummy:
		{
			return single;
			break;
		}
	default:
		break;
	}
	return tempcards;
}

bool GuildCardMgr::CompareSingle(std::vector<UINT32>& firstcards, std::vector<UINT32>& secondcards)
{
	if (firstcards.size() != secondcards.size())
	{
		return true;
	}
	for (UINT32 i = 0; i < firstcards.size(); ++i)
	{
		if (firstcards[i] == secondcards[i])
		{
			continue;
		}
		else
		{
			return firstcards[i] > secondcards[i];
		}
	}
	return true;
}

UINT32 GuildCardMgr::RandomStore(std::vector<UINT32>& cards, std::vector<UINT32>& cardstore)
{
	UINT32 store = RandomStoreType();
	switch(store)
	{
	case 0:
		{
			FullStore(1, cardstore);
			break;
		}
	case 1:
		{
			FullStore(3, cardstore);
			break;
		}
	case 2:
		{
			FullStore(1, cardstore);
			UINT32 card = RandomCards(cardstore);
			UINT32 num = card >> 4;
			UINT32 type = card & 0xf;
			cards.push_back(card);

			UINT32 card1 = (num << 4) + (++type)%CardB2;
			RandomCards(cardstore, card1);
			cards.push_back(card1);
			break;
		}
	case 3:
		{
			FullStore(1, cardstore);
			UINT32 card = RandomCards(cardstore);
			UINT32 num = card >> 4;
			UINT32 type = card & 0x0f;
			cards.push_back(card);

			UINT32 card1 = (num << 4) + (++type)%CardB2;
			RandomCards(cardstore, card1);
			cards.push_back(card1);

			UINT32 card2 = (num << 4) + (++type)%CardB2;
			RandomCards(cardstore, card2);
			cards.push_back(card2);

			store = 3;
			break;
		}
	case 4:
		{
			FullStore(2, cardstore);
			break;
		}
	default:
		{
			FullStore(1, cardstore);
			break;
		}
	}

	while (CARDCOUNT != cards.size())
	{
		UINT32 card = RandomCards(cardstore);
		cards.push_back(card);
	}
	return store;
}

void GuildCardMgr::FullStore(UINT32 type, std::vector<UINT32>& cardstore)
{
	switch(type)
	{	
	case 2:
		{
			UINT32 random1 = XRandom::randInt(CardR1, CardB2 + 1);
			UINT32 random2 = XRandom::randInt(CardR1, CardB2 + 1);
			if (random1 == random2)
			{
				random2 = (random2 + 1)%CardB2;
			}
			for (UINT32 card = CardMin; card <= CardMax; ++card)
			{
				cardstore.push_back((card << 4) + random1);
				cardstore.push_back((card << 4) + random2);
			}
			break;
		}
	case 3:
		{
			UINT32 card = (UINT32)XRandom::randInt(CardMin, Card10 + 1);
			UINT32 cardmax = card + CARDCOUNT;
			for (; card < cardmax; ++card)
			{
				cardstore.push_back((card << 4) + CardR1);
				cardstore.push_back((card << 4) + CardR2);
				cardstore.push_back((card << 4) + CardB1);
				cardstore.push_back((card << 4) + CardB2);
			}
			break;
		}
	case 1:
	default:
		{
			for (UINT32 card = CardMin; card <= CardMax; ++card)
			{
				cardstore.push_back((card << 4) + CardR1);
				cardstore.push_back((card << 4) + CardR2);
				cardstore.push_back((card << 4) + CardB1);
				cardstore.push_back((card << 4) + CardB2);
			}
			break;
		}
		break;
	}
}

UINT32 GuildCardMgr::RandomCards(std::vector<UINT32>& cardstore, UINT32 card)
{
	if (cardstore.empty())
	{
		return 0;
	}
	if (0 != card)
	{
		for (auto i = cardstore.begin(); i != cardstore.end(); ++i)
		{
			if (card == *i)
			{
				cardstore.erase(i);
				break;
			}
		}
		return card;
	}
	else
	{
		UINT32 size = cardstore.size();
		UINT32 random = XRandom::randInt(0, size);
		auto i = cardstore.begin() + random;
		UINT32 card = *i;
		cardstore.erase(i);
		return card;
	}
}

UINT32 GuildCardMgr::ChangeCard(UINT32 card, std::vector<UINT32>& cards, std::vector<UINT32>& cardstore)
{
	bool istrue = false;
	for (auto i = cards.begin(); i != cards.end(); ++i)
	{
		if (card == *i)
		{
			cards.erase(i);
			istrue = true;
			break;
		}
	}
	if (istrue)
	{
		UINT32 tempcard = RandomCards(cardstore);
		cardstore.push_back(card);
		cards.push_back(tempcard);
		return tempcard;
	}
	else
	{
		return (UINT32)(-1);
	}
}

UINT32 GuildCardMgr::GetPoint(UINT32 result)
{
	for (auto i = m_rewardtable.Table.begin(); i != m_rewardtable.Table.end(); ++i)
	{
		CardRewardTable::RowData* rowdata = *i;
		if (rowdata->type == result)
		{
			return rowdata->point;	
		}
	}
	return 0;
}

const std::vector<ItemDesc>* GuildCardMgr::GetRankReward(UINT32 type, UINT32 rank,int level_seal)
{
	auto it = levelseal_rankd2items_.begin();
	auto it_end = levelseal_rankd2items_.end();
	for (; it != it_end; ++it)
	{
		if (level_seal >= it->levelseal[0] && level_seal <= it->levelseal[1] )
		{
			auto i = it->m_rankd2items.find(type);
			if (i == it->m_rankd2items.end())
			{
				return NULL;
			}
			auto j = i->second.find(rank);
			if (j == i->second.end())
			{
				return NULL;
			}
			return &j->second;
		}
	}

	// 防止世界等级太高没有找到奖励
	if (levelseal_rankd2items_.size() > 0)
	{
		auto& rank_items  = levelseal_rankd2items_[levelseal_rankd2items_.size() -1].m_rankd2items;	
		auto max_it = rank_items.find(type);
		if (max_it == rank_items.end())
		{
			return NULL;
		}
		auto j = max_it->second.find(rank);
		if (j == max_it->second.end())
		{
			return NULL;
		}
		return &j->second;
	}
	return NULL;
}

//bool GuildCardMgr::HintNotify(Role* role, Guild* guild)
//{
//	CGuildRecord* record = role->Get<CGuildRecord>();
//	if (record->IsHintCard() || !guild->CanPlayCard(role))
//	{
//		return false;	
//	}
//	record->SetHintCard(true);
//	role->HintNotify(GuildCardSystem, false);		
//	return true;
//}
