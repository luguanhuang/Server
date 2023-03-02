#ifndef __GUILDCARDGMR_H__
#define __GUILDCARDGMR_H__

#include "table/CardStoreTable.h"
#include "table/CardRewardTable.h"
#include "table/CardRankRewardTable.h"

#define CARDCOUNT 5

enum CardType
{
	CardR1 = 0,
	CardR2 = 1,
	CardB1 = 2,
	CardB2 = 3,
};

enum CardNum
{
	Card2 = 2,
	Card3 = 3,
	Card4 = 4,
	Card5 = 5,
	Card6 = 6,
	Card7 = 7,
	Card8 = 8,
	Card9 = 9,
	Card10 = 10,
	CardJ = 11,
	CardQ = 12,
	CardK = 13,
	CardA = 14,

	CardMin = Card2,
	CardMax = CardA,
};

enum CardResult
{
	CardABCDE = 0,
	CardAAAAB = 1,
	CardAAABB = 2,
	Cardaaaaa = 3,
	Cardabcde = 4,
	CardAAABC = 5,
	CardAABBC = 6,
	CardAABCD = 7,

	CardDummy = 8,
};

struct ItemDesc;
class Role;
struct GuildCardRankRewardItem
{
	Sequence<int, 2>	levelseal;
	std::map<UINT32, std::map<UINT32, std::vector<ItemDesc>>> m_rankd2items;
};

class GuildCardMgr
{
	GuildCardMgr();
	~GuildCardMgr();
	DECLARE_SINGLETON(GuildCardMgr);
public:	
	bool Init();
	void Uninit();

	UINT32 RandomStore(std::vector<UINT32>& cards, std::vector<UINT32>& cardstore);
	void FullStore(UINT32 type, std::vector<UINT32>& cardstore);
	UINT32 RandomCards(std::vector<UINT32>& cardstore, UINT32 card = 0);
	UINT32 ChangeCard(UINT32 card, std::vector<UINT32>& cards, std::vector<UINT32>& cardstore);

	void GetReward(UINT32 type, std::vector<ItemDesc>& items);
	void GetMatchReward(UINT32 type, std::vector<ItemDesc>& items);
	void GetPokerTournamentMatchReward(UINT32 type, std::vector<ItemDesc>& items);
	UINT32 GetExp(UINT32 result);

	UINT32 GetResult(std::vector<UINT32>& cards);
	UINT32 GetPoint(UINT32 result);

	//bool HintNotify(Role* role, Guild* guild);

	const std::vector<ItemDesc>* GetRankReward(UINT32 type, UINT32 rank,int level_seal);

	bool Compare(UINT32 result, std::vector<UINT32>& firstcards, std::vector<UINT32>& secondcards);

private:
	UINT32 RandomStoreType();
	std::vector<UINT32> Shrink(UINT32 result, std::vector<UINT32>& cards);
	bool CompareSingle(std::vector<UINT32>& firstcards, std::vector<UINT32>& secondcards);

private:
	CardStoreTable m_storetable;
	CardRewardTable m_rewardtable;
	CardRankRewardTable m_rankrewardtable;
	//std::map<UINT32, std::map<UINT32, std::vector<ItemDesc>>> m_rankd2items;
	std::vector<GuildCardRankRewardItem>  levelseal_rankd2items_;
};

#endif