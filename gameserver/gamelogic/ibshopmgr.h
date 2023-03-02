#ifndef __IBSHOPMGR_H__
#define __IBSHOPMGR_H__
#include "table/IBShop.h"
#include <map>
#include <vector>

#include "ibshopdef.h"

class Role;


class IBShopMgr:public ITimer
{
public:

	IBShopMgr();
	~IBShopMgr();

	DECLARE_SINGLETON(IBShopMgr);
public:
	bool Init();
	void Uninit();
	bool CheckFile();
	bool LoadFile();
	void LoadMaxPlayerDegree();
	void LoadActivityTime();
	void GetActivityCount(UINT64 nRoldID,UINT32 nGoodsID,stIBShopItem &item);
	int CheckIBItemLimit(vector<Sequence<string, 2>> &veccfg,UINT32 &nStartTime,UINT32 &nEndTime);
	void ClearRecord(Role *pRole);
	void GetIBShopItems(Role *pRole,UINT32 nType,UINT32 nSubType,std::list<stIBShopItem > &list);
	std::set<UINT32> GetHotPoint(Role *pRole,bool &bLimitHot);
	void SendIcon2Client(Role *pRole);
	void ClearIconFlag(Role *pRole);
	KKSG::ErrorCode BuyItem(Role *pRole,UINT32 nGoodsID, UINT32 nCount);
	void BuildShowTag(Role *pRole,UINT32 nType, std::set<UINT32 > &setIBShopTag, bool &bShowVip);
	void GetActivityTime(IBShop::RowData *pCfg,stIBShopItem &item);
	void BuildIBShop(Role *pRole);
	void ClearFile();
	void GetPayMemberPrivilegeShop(Role* pRole, INT32 rate, std::vector<KKSG::PayPrivilegeShop> &list);
	void OnTimer(UINT32 dwID, UINT32 dwCount);
	UINT32 GetMaxPlayerDegree();

	void AddBuyCount(Role* role, UINT32 goodsID, UINT32 count,bool bSync=false);
	IBShop::RowData* GetByGID(UINT32 nGID) { return m_IBShopConfig.GetByid(nGID);}
	IBShop::RowData* GetByGID(const string &goodsid) {return m_IBShopConfig.GetBygoodsid(goodsid); }
	const std::vector<IBShop::RowData*>* GetConfsByItemID(UINT32 itemID);
public:
	UINT32 _GetCanBuyCount(Role* role, UINT32 goodsID, int& count);

	void PackIBItems(Role *pRole,UINT32 nType,UINT32 nSubType,KKSG::IBQueryItemRes &roRes);
	IBShop m_IBShopConfig;

	struct ActivityConfig
	{
		ActivityConfig():nStartTime(0),nEndTime(0){}
		UINT32 nStartTime;
		UINT32 nEndTime;
		UINT32 nCount;
	};
	std::map<UINT32, std::vector<ActivityConfig>> m_mapActivity;
private:
	UINT32 m_nTime;
	HTIMER m_Timer;
	UINT32 m_nRWeek;
	UINT32 m_nRHour;
	UINT32 m_nRMin;
	UINT32 m_nMaxPlayerDegree;
	std::map<UINT32, std::vector<IBShop::RowData*>> m_itemID2Confs;
};



#endif
