#ifndef __IBGIFTMGR_H__
#define __IBGIFTMGR_H__


//缓存没有处理完成礼物订单，数据改变立刻存入数据库，没有做定时Update，没有数据role直接发协议到db，让db删除
//跨服不给物品
//PayGiftIbStatus_CreateBill = 1		//玩家发起购买生成订单，超过48小时的订单删除;
//PayGiftIbStatus_Fail = 2				//扣款失败客户端通知服务器订单标识为失败(暂时不删除，防止客户端错误通知，导致后续发货找不到订单)，超过12小时删除;
//PayGiftIbStatus_Success = 3			//扣款成功且发货成功,7天后删除(防止米大师多次调用重复发货)
//PayGiftIbStatus_SuccessNotAddItem = 4 //付款成功未真正发货(例如不在线);
enum PayGiftIbStatusExpireTime
{
	MAX_PAYGIFT_CREATEBILL = 5 * 24 * 3600,
	MAX_PAYGIFT_FAIL = 12 * 3600,
	MAX_PAYGIFT_SUCCESS = 7 * 24 * 3600,
};

enum GiftIbShipStatus
{
	GIFTIB_NOT_SHIPPED		= 0,
	GIFTIB_BEING_SHIPPED	= 1,
	GIFTIB_FINISH_SHIPPED   = 2,
};

class CRole;

class IBGiftMgr : public ITimer
{
	IBGiftMgr();
	~IBGiftMgr();
	DECLARE_SINGLETON(IBGiftMgr);

public:
	bool Init();
	void Uninit();
	bool ReadHistGiftDB();
	bool ReadAllGiftFromDB();
	UINT32 GetUnRecvGift(UINT64 nRole);
	void GiftItem(KKSG::PayGiftIbItem &info);
	UINT32 GetSize();
	void OnTimer(UINT32 dwID, UINT32 dwCount);
	void SaveHistGift(bool bAll = false);
	void Save(KKSG::MysqlOpType op, const KKSG::PayGiftIbItem& oItem);
	INT32 PayBuyGoods(std::map<std::string, std::string>& params, std::string& sig, std::ostringstream& msg);
	void AddSimOrder(CRole *pFrom,CRole *pTo);
	std::string PayBuyGoodsRes(INT32 ret, std::string msg);
	KKSG::PayGiftIbItem* GetIbItem(UINT64 qwRoleId, const std::string& strBillNo); 
	KKSG::PayGiftIbItem* GetIbItem(const std::string& strBillNo);
	void TestGiftIBSucessNtf(CRole *pRole);
	void AddIbItem(UINT64 qwRoleId, KKSG::PayGiftIbItem *oItem);
	void AddNewIbItem(KKSG::PayGiftIbItem &oItem);
	void DelToGiftIBItem(const KKSG::PayGiftIbItem* pItem);
	void AddToIbItem(KKSG::PayGiftIbItem* pItem);
	void DoTxLog(const std::string& Account, KKSG::PayGiftIbItem* pItem);
	KKSG::PayGiftIbItem* GetGiftIbByOrder(UINT64 nRoleID,const std::string &orderid);
private:
	bool CheckStatusExpire(UINT32 sta, UINT32 ts);
	std::unordered_map<UINT64, std::vector<KKSG::PayGiftIbItem*> > m_mapGiftIb;		//from roleid, payiteminfo
	std::unordered_map<UINT64, std::vector<KKSG::PayGiftIbItem*> > m_mapToGiftIb;	//to roleid,  payiteminfo
	std::map<UINT64, KKSG::SaveIbGiftRecordArg> m_mapSaveHistInfo;
	std::unordered_map<std::string,UINT64> m_mapBill; //billno roleid
	HTIMER m_TimeHandler;


public:
	enum eIBGiftType
	{
		eIBGIFTSELF  = 0,		//收礼品
		eIBGIFTTARGET = 1,		//送礼品
	};
	struct stIBGiftHistItem
	{
		std::string name;
		KKSG::ItemBrief item;
		UINT32 nTime;
	};
	struct stIBGiftHistItemCmp
	{
		bool operator()(const stIBGiftHistItem &p1, const stIBGiftHistItem &p2)
		{
			return p1.nTime > p2.nTime;
		}
	};
	void AddHistItem(UINT64 nRole,UINT32 type, const stIBGiftHistItem &item);
	void PrepSave(UINT64 nRole);
	void GetHistItem(UINT64 nRole,UINT32 type,KKSG::IBGiftHistAllItem *allitem);

	KKSG::PayGiftIbItem *GetGoods(CRole *pRole);
	void OnLogin(CRole *pRole);


	void SendIcon(CRole *pRole);
private:
	std::map<UINT64,std::vector<stIBGiftHistItem>> m_mapSelfHistItems;
	std::map<UINT64,std::vector<stIBGiftHistItem>> m_mapTargetHistItems;
	UINT32 m_nTime;
};


#endif
