#ifndef __PAYCONFIG_H__
#define __PAYCONFIG_H__

#include "table/PayListTable.h"
#include "table/PayCardTable.h"
#include "table/PayAileenTable.h"
#include "table/RechargeTable.h"
#include "table/PayFirst.h"
#include "table/PayMemberTable.h"
#include "pb/project.pb.h"
#include "gamelogic/payv2Record.h"

class EqualBillNo
{
public:
	EqualBillNo(std::string bill)
		:billno(bill)
	{
	}
	bool operator ()(const std::multimap<UINT32, std::string>::value_type &pair)
	{
		return pair.second == billno;
	}
private:
	std::string billno;                    
};

struct EqualPayMemberID
{
	INT32 ID;
	EqualPayMemberID(INT32 iID) { ID = iID;}
	bool operator()(const KKSG::PayMemberRecord& mem)
	{
		return mem.id() == ID;
	}
	bool operator()(const KKSG::PayPrivilegeShop& mem)
	{
		return mem.goodsid() == ID;
	}
};

class CPayConfig
{
	CPayConfig();
	~CPayConfig();
	DECLARE_SINGLETON(CPayConfig)

public:
	bool Init();
	void Uninit();
	bool CheckConfig();//检查配置是否合法

	bool LoadFile();
	bool CheckFile();
	void ClearFile();

	PayFirst::RowData *GetPayFirstConf(){return m_pPayFirstData;}
	RechargeTable::RowData *GetGrowthFundConf(){return m_pGrowthFundData;}
	INT32 GetGrowthFundSystemID(){return m_nGrowthFundSystemID;}
	INT32 GetPayFirstSystemID(){return m_nPayFirstSystemID;}
	INT32 GetCardAward(INT32 nType);
	INT32 GetParamType(std::string szParamID);
	std::string GetParamByServiceCode(std::string servicecode);
	INT32 GetAileenItemID(std::string szParamID);
	PayAileenTable& GetPayAileenTable(){ return m_oPayAileenTable; }
	PayListTable& GetPayListTable() { return m_oPayListTable; }
	PayCardTable& GetPayCardTable() { return m_oPayCardTable; }
	RechargeTable& GetPayGrowthFundTable() { return m_oPayGrowthFundTable; }
	PayMemberTable& GetPayMemberTable() { return m_oPayMemberTable; }
	void CheckAileenConfig(CPayV2Record* pRecord, KKSG::PayAllInfo* pPayInfo);
	void CheckMemberConfig(CPayV2Record* pRecord, KKSG::PayAllInfo* pPayInfo);
	bool IsPaySystemID(INT32 iSystemID);
	PayMemberTable::RowData* GetPayMemberBySystem(INT32 iSystemID);
	PayMemberTable::RowData* GetPayMemberByID(INT32 iID);
	bool IsTodayAlieen();//检查今日是否有艾琳礼包;
	PayAileenTable::RowData* GetTodayPayAileenTable(std::string szParamID);

private:
	PayListTable m_oPayListTable;
	PayCardTable m_oPayCardTable;
	PayAileenTable m_oPayAileenTable;
	RechargeTable m_oPayGrowthFundTable;//成长基金
	PayFirst m_oPayFirstTable;
	PayMemberTable m_oPayMemberTable;
	INT32 m_nGrowthFundSystemID;
	INT32 m_nPayFirstSystemID;
	PayFirst::RowData* m_pPayFirstData;
	RechargeTable::RowData* m_pGrowthFundData;

	std::map<std::string, UINT32> m_vParaTypeInfo;
	std::map<std::string, std::string> m_vServiceCodeInfo;//一开始是用商品Id（paramid）处理逻辑的，后来midas的是servicecode，所以中间加一个转换过程;
	std::vector<INT32> m_vPaySystemID;
};

#endif
