#ifndef __BUYEXCHANGE_H__
#define __BUYEXCHANGE_H__

#include "table/BuyGoldTable.h"
#include "table/BuyFatigueTable.h"
#include "table/BuyDragonCoin.h"

class Role;

class BuyExchangeMgr
{
	BuyExchangeMgr();
	~BuyExchangeMgr();
	DECLARE_SINGLETON(BuyExchangeMgr)

public:

	bool Init();
	void Uninit();

	int BuyGold(Role *pRole, int useitemid);
	int BuyFatigue(Role *pRole, int useitemid, int fatigueID);
	int BuyDragonCoinFunPre(Role *pRole, int useitemid, UINT32 oncecount);
	void BuyDragonCoinFunAft(Role *pRole, int useitemid, UINT32 oncecount);
	int BuyBlueBird(Role* pRole, int useitemid);
	int ExchangeDiamond(Role* pRole, UINT32 count);

private:

	BuyGoldTable m_GoldTable;
	BuyFatigueTable m_FatigueTable;
	BuyDragonCoin m_DragonCoinTable;
};


#endif // __BUYGOLDFATIGUEMGR_H__