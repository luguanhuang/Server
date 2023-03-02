//本文件用来idip处理离线roleallinfo数据修改的;
#ifndef _H_BAG_H__
#define _H_BAG_H__
#include "pch.h"
#include "define/itemdef.h"
#include "table/ItemConfigMgr.h"
#include "pb/project.pb.h"

struct EqualDesignationID
{
	UINT32 ID;
	EqualDesignationID(UINT32 iID) { ID = iID;}
	bool operator()(KKSG::StcDesignationInfo& roInfo)
	{
		return roInfo.designationid() == ID;
	}
};

struct EqualID
{
	UINT64 ID;
	EqualID(UINT64 iID) { ID = iID;}
	bool operator()(KKSG::Item& roInfo)
	{
		return roInfo.uid() == ID;
	}
	bool operator()(KKSG::FashionData& roInfo)
	{
		return roInfo.uid() == ID;
	}
};
class CBag
{
public:
	CBag(KKSG::RoleAllInfo* pInfo);
	~CBag();
	void Init();
	void Save();

	void ReduceVirtualItem(UINT32 itemid, INT64 Count);
	bool ReduceItem(UINT32 itemid, INT64 Count);
	INT32 CountItem(UINT32 itemid);
	bool SetItemCount(UINT32 itemid, INT32 Count);
	bool SetScore(UINT32 uType, INT32 Count);
	static bool CompareCount(KKSG::Item* fir, KKSG::Item* sec);
	static void SortCount(std::vector<KKSG::Item*>& items);
	void FindItem(UINT32 itemID, std::vector<KKSG::Item*>& items, ItemBind filter);
	bool AddItem(UINT32 itemid, INT64 Count, bool isbind=true);
	void AddNewItem(UINT32 itemid, INT32 iType, INT64 lCount, bool isbind);
	bool IsVirtualItem(UINT32 itemid);
	void DelDesignationID(UINT32 ID);
	bool DelItem(UINT32 itemid, UINT64 uid);
	bool DelFashion(UINT32 itemid, UINT64 uid);
	bool DelItemByUid(UINT32 itemid, UINT64 uid);
	bool DelEmblemByUid(UINT32 itemid, UINT64 uid);
	bool DelItemJade(KKSG::ItemJade* pData, INT32 jadeid);
	bool DelItemJade(UINT32 itemid, UINT64 uid, INT32 jadeid);

private:
	KKSG::RoleAllInfo* pRoleInfo;
	//虚拟物品：钻石，龙币，etc;
	INT64    m_VirtualItem[VIRTUAL_ITEM_MAX];
	//背包里的物品;
	std::vector<KKSG::Item*> m_Items;

	//身上称号
	std::vector<KKSG::StcDesignationInfo> m_Designations;

	//身上的时装
	std::vector<KKSG::FashionData> m_BodyFashion;
	//背包的时装
	std::map<UINT64, KKSG::FashionData> m_BagFashion;

	//身上装备
	std::vector<KKSG::Item> m_BodyItems;

	//身上纹章
	std::vector<KKSG::Item> m_BodyEmblems;
};
#endif
