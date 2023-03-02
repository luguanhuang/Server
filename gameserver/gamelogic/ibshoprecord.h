#ifndef __IBSHOPRECORD_H__
#define __IBSHOPRECORD_H__
#include "unit/roleserilizeinterface.h"
#include "ibshopdef.h"
#include "pch.h"


class Role;

class IBShopRecord : public RoleSerializeInterface
{
public:
	IBShopRecord(Role* role);
	virtual ~IBShopRecord();
	virtual bool Load(const KKSG::RoleAllInfo *poRoleAllInfo);
	virtual void Save(KKSG::RoleAllInfo *poRoleAllInfo, CDataFieldSet& roChanged);
	void SetSaveFlag();
	void AddOrder(const std::string &order,UINT32 nTime,bool bSave=false);
	void DelOrder(const std::string &order,bool bSave=false);
	void ClearOrder();
	bool HasOrder(const std::string &order);
	void SetPayDegree(UINT32 nDegree);
	void AddPayDegree(const UINT32 nDegree);
	const UINT32 GetPayDegree();
private:
	Switch m_IsModify;

public:
	typedef std::map<UINT32, stIBShopItem > mapIBShopItem;
	mapIBShopItem	m_mapIBShopItem;
	UINT32			m_nLastTime;
	UINT32			m_nVipLv;
	Role*			m_pRole;
	bool			m_bLimitHot;				//显示红点标志
	std::set<UINT32 >	 m_setVipGoodsID;		//vip googdsid
	bool			m_bShowVipTag;				//显示VIP 页面
	std::unordered_map<std::string,UINT32> m_mapOrder;
	UINT32			m_nPullTime;
	UINT32			m_nDayTime;
private:
	UINT32			m_nPayDegree;




};


#endif