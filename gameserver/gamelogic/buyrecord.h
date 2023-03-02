#ifndef __BUYRECORD_H__
#define __BUYRECORD_H__

#include "unit/roleserilizeinterface.h"
#include "define/itemdef.h"

class Role;

namespace KKSG
{
	class BuyGoldFatInfo;
}

struct BuyFatigueSt
{
	int fagitueID;
	int buyCount;
};

struct BackFlowBuyInfo
{
	bool islock;
	Switch modify;

	UINT32 buyindex;    ///> 当前买的index
	UINT32 buyday;      ///> 当前买index的时间
	UINT32 buycount;	///> 当前买index的次数

	void Load(const KKSG::BackFlowBuy& info);
	void Save(KKSG::BackFlowBuy* info);

	void Reset();
	void DoBuy(UINT32 index, UINT32 day);
	UINT32 GetTodayIndex(UINT32 day);

	void Lock() { islock = true; }
	void UnLock() { islock = false; }
	bool IsLock() { return islock; }

	BackFlowBuyInfo()
	{
		Reset();
	}
};

class BuyRecord : public RoleSerializeInterface
{
public:

	BuyRecord(Role *pRole);
	~BuyRecord();

	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);

	int  GetBuyGoldCount();
	void IncBuyGoldCount();
	int  GetBuyFatigueCount(int fatigueID);
	void IncBuyFagitueCount(int fatigueID);
	int  GetBuyDragonCoinCount();
	void IncBuyDragonCoinCount(UINT32 count);
	void SendToClient();
	void LoginCheck();
	bool CanBuyGold();
	bool CanBuyFatigue(int fatigueID);
	bool CanBuyDragon(UINT32 count);

	void HintBackFlow();
	BackFlowBuyInfo& GetBackFlow() { return m_backflow; }

private:
	void SaveTo(KKSG::BuyGoldFatInfo *pBuyInfo);
	void CheckDay();

	Role *m_pRole;
	Switch m_isModified;
	int m_BuyDay;
	int m_BuyGoldCount;
	int m_BuyDragonCoinCount;
	BackFlowBuyInfo m_backflow;
	std::vector<BuyFatigueSt> m_BuyFatgiueCount;
};

#endif // __BUYRECORD_H__

