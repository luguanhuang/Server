#ifndef __BAGLISTENER_H__
#define __BAGLISTENER_H__

enum ItemOpType
{
	ADD_ITEM,
	REMOVE_ITEM,
	CHANGE_ITEM,
	SWAP_ITEM,
	CHANGEATTR_ITEM,
	RECYLEADD_ITEM,
	RECYLECHANGE_TIEM,
};

///> 记录下操作过程中物品的改动，在Transition结束时发包通知客户端
struct XItem;
struct ItemChangeRecord
{
	ItemOpType op;
	XItem  *pItem;
	UINT64 uid1;
	UINT64 uid2;
};

struct VirtualItemRecord
{
	UINT32   itemID;
	UINT64   count;
};

struct FashionInfo;
class BagListener
{
public:
	virtual ~BagListener() {}
	virtual void OnAddItem(XItem *pItem) = 0;
	virtual void OnRemoveItem(XItem *pItem) = 0;
	virtual void OnSwapItem(XItem *pItem1, XItem *pItem2, int pos) = 0;
	virtual void OnChangeItemCount(XItem *pItem) = 0;
	virtual void OnChangeVirtualItem(UINT32 itemID, UINT64 Count) = 0;
	virtual void OnChangeItemAttr(XItem* pItem) = 0;

	virtual void OnAddFashion(FashionInfo* fashion) = 0;

	virtual void OnBagFullAndSendMail(const ItemDesc& desc) = 0;
	virtual void OnBagFullAndSendMail(const XItem* pItem) = 0;
};

class MailSender;
///> 用来组包通知客户端bag的改动
class BagItemOperationRecord : public BagListener
{
public:
	BagItemOperationRecord();
	~BagItemOperationRecord();	
 
	virtual void OnAddItem(XItem *pItem);
	virtual void OnRemoveItem(XItem *pItem);
	virtual void OnSwapItem(XItem *pItem1, XItem *pItem2, int pos);
	virtual void OnChangeItemCount(XItem *pItem);
	virtual void OnChangeVirtualItem(UINT32 itemID, UINT64 Count);
	virtual void OnChangeItemAttr(XItem* pItem);
	
	virtual void OnAddFashion(FashionInfo* fashion);

	virtual void OnBagFullAndSendMail(const ItemDesc& desc);
	virtual void OnBagFullAndSendMail(const XItem* pItem);

	void SetRecyle() { m_isrecyle = true; };

	void SetSendMailSubReason(int subReason) { m_subReason = subReason; };

	inline int GetSendMailSubReason() const { return m_subReason; }

	void NotifyClient(Role *pRole);	

private:
	void NotifyChangedRecord(Role *pRole);

	bool m_isrecyle;
	std::list<std::pair<int, int>> m_WearFashion;
	std::list<ItemChangeRecord> m_ChangeRecord;
	std::list<VirtualItemRecord> m_VirtualItemRecord;

	std::vector<FashionInfo*> m_fashions;

	MailSender*	m_mailSender;
	bool	m_needMail;
	int		m_subReason;
};

#endif