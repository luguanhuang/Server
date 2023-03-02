#include "pch.h"
#include "payconfig.h"
#include "pb/enum.pb.h"
#include "gamelogic/levelsealMgr.h"
#include "define/systemiddef.h"
#include "util/XCommon.h"
#include "util/gametime.h"


INSTANCE_SINGLETON(CPayConfig)

CPayConfig::CPayConfig()
{
	m_pPayFirstData = NULL;
	m_pGrowthFundData = NULL;
	m_nPayFirstSystemID = 0;
	m_nGrowthFundSystemID = 0;
}

CPayConfig::~CPayConfig()
{

}

bool CPayConfig::Init()
{
	if (!m_oPayListTable.LoadFile("table/PayList.txt"))
	{
		LogWarn("Load table %s failed!", "table/PayList.txt");
		return false;
	}
	if (!m_oPayCardTable.LoadFile("table/PayCard.txt"))
	{
		LogWarn("Load table %s failed!", "table/PayCard.txt");
		return false;
	}

	if (!m_oPayAileenTable.LoadFile("table/PayAileen.txt"))
	{
		LogWarn("Load table %s failed!", "table/PayAileen.txt");
		return false;
	}

	if (!m_oPayGrowthFundTable.LoadFile("table/Recharge/RechargeTable.txt"))
	{
		LogWarn("Load table %s failed!", "table/Recharge/RechargeTable.txt");
		return false;
	}

	if (!m_oPayFirstTable.LoadFile("table/Recharge/PayFirst.txt"))
	{
		LogWarn("Load table %s failed!", "table/Recharge/PayFirst.txt");
		return false;
	}

	if (!m_oPayMemberTable.LoadFile("table/PayMember.txt"))
	{
		LogWarn("Load table %s failed!", "table/PayMember.txt");
		return false;
	}

	if (m_oPayFirstTable.Table.size())
	{
		m_pPayFirstData = m_oPayFirstTable.Table[0];
		m_nPayFirstSystemID = m_oPayFirstTable.Table[0]->SystemID;
	}
	if (m_oPayGrowthFundTable.Table.size())
	{
		m_pGrowthFundData = m_oPayGrowthFundTable.Table[0];
		m_nGrowthFundSystemID = m_oPayGrowthFundTable.Table[0]->SystemID;
	}
	return CheckConfig();
}
bool CPayConfig::CheckConfig()
{
	for (auto i = m_oPayListTable.Table.begin(); i != m_oPayListTable.Table.end(); i ++)
	{
		if (m_vParaTypeInfo.find((*i)->ParamID) != m_vParaTypeInfo.end())
		{
			LogWarn("table/PayList.txt has a same ParamID=%s,failed", (*i)->ParamID.c_str());
			return false;
		}
		m_vParaTypeInfo.insert(std::make_pair((*i)->ParamID, KKSG::PAY_PARAM_LIST));
	}

	for (auto i = m_oPayCardTable.Table.begin(); i != m_oPayCardTable.Table.end(); i ++)
	{
		if (m_vParaTypeInfo.find((*i)->ParamID) != m_vParaTypeInfo.end())
		{
			LogWarn("table/PayCard.txt has a same ParamID=%s,failed", (*i)->ParamID.c_str());
			return false;
		}
		m_vParaTypeInfo.insert(std::make_pair((*i)->ParamID, KKSG::PAY_PARAM_CARD));

		/*
		if (m_vServiceCodeInfo.find((*i)->ServiceCode) != m_vServiceCodeInfo.end())
		{
			LogWarn("table/PayCard.txt has a same ServiceCode=%s,failed", (*i)->ServiceCode.c_str());
			return false;
		}
		m_vServiceCodeInfo.insert(std::make_pair((*i)->ServiceCode, (*i)->ParamID));
		*/
	}

	for (auto i = m_oPayAileenTable.Table.begin(); i != m_oPayAileenTable.Table.end(); i ++)
	{
		if (m_vParaTypeInfo.find((*i)->ParamID) != m_vParaTypeInfo.end())
		{
			LogWarn("table/PayAileen.txt has a same ParamID=%s,failed", (*i)->ParamID.c_str());
			return false;
		}
		if (m_vServiceCodeInfo.find((*i)->ServiceCode) != m_vServiceCodeInfo.end())
		{
			LogWarn("table/PayAileen.txt has a same ServiceCode=%s,failed", (*i)->ServiceCode.c_str());
			return false;
		}
	}

	for (auto i = m_oPayAileenTable.Table.begin(); i != m_oPayAileenTable.Table.end(); i ++)
	{
		if (m_vParaTypeInfo.find((*i)->ParamID) == m_vParaTypeInfo.end())
		{
			m_vParaTypeInfo.insert(std::make_pair((*i)->ParamID, KKSG::PAY_PARAM_AILEEN));
		}
		if (m_vServiceCodeInfo.find((*i)->ServiceCode) == m_vServiceCodeInfo.end())
		{
			m_vServiceCodeInfo.insert(std::make_pair((*i)->ServiceCode, (*i)->ParamID));
		}
	}

	for (auto i = m_oPayGrowthFundTable.Table.begin(); i != m_oPayGrowthFundTable.Table.end(); i ++)
	{
		if (m_vParaTypeInfo.find((*i)->ParamID) != m_vParaTypeInfo.end())
		{
			LogWarn("table/Recharge/RechargeTable.txt has a same ParamID=%s,failed", (*i)->ParamID.c_str());
			return false;
		}
		m_vParaTypeInfo.insert(std::make_pair((*i)->ParamID, KKSG::PAY_PARAM_GROWTH_FUND));

		if (m_vServiceCodeInfo.find((*i)->ServiceCode) != m_vServiceCodeInfo.end())
		{
			LogWarn("table/Recharge/RechargeTable.txt has a same ServiceCode=%s,failed", (*i)->ServiceCode.c_str());
			return false;
		}
		m_vServiceCodeInfo.insert(std::make_pair((*i)->ServiceCode, (*i)->ParamID));
	}

	for (auto i = m_oPayMemberTable.Table.begin(); i != m_oPayMemberTable.Table.end(); i ++)
	{
		if (m_vParaTypeInfo.find((*i)->ParamID) != m_vParaTypeInfo.end())
		{
			LogWarn("table/PayMember.txt has a same ParamID=%s,failed", (*i)->ParamID.c_str());
			return false;
		}
		m_vParaTypeInfo.insert(std::make_pair((*i)->ParamID, KKSG::PAY_PARAM_MEMBER));

		if (m_vServiceCodeInfo.find((*i)->ServiceCode) != m_vServiceCodeInfo.end())
		{
			LogWarn("table/PayMember.txt has a same ServiceCode=%s,failed", (*i)->ServiceCode.c_str());
			return false;
		}
		m_vServiceCodeInfo.insert(std::make_pair((*i)->ServiceCode, (*i)->ParamID));
		m_vPaySystemID.push_back((*i)->SystemID);
	}
	m_vPaySystemID.push_back(m_nPayFirstSystemID);
	m_vPaySystemID.push_back(m_nGrowthFundSystemID);
	m_vPaySystemID.push_back(Welfare_DiamondCard);
	m_vPaySystemID.push_back(Welfare_GiftBag);

	return true;
}

void CPayConfig::Uninit()
{

}

bool CPayConfig::LoadFile()
{
	if (!CheckFile())
	{
		return false;
	}

	ClearFile();

	// load file PayAileen
	// to do...
	m_oPayAileenTable.LoadFile("table/PayAileen.txt");
	return true;
}

bool CPayConfig::CheckFile()
{
	// check file PayAileen
	// to do...
	PayAileenTable tmpAileenTable;
	if (!tmpAileenTable.LoadFile("table/PayAileen.txt"))
	{
		LogWarn("Load table %s failed!", "table/PayAileen.txt");
		return false;
	}

	for (auto i = tmpAileenTable.Table.begin(); i != tmpAileenTable.Table.end(); i ++)
	{
		auto it = m_vParaTypeInfo.find((*i)->ParamID);
		if (it == m_vParaTypeInfo.end() )
		{
			return false;
		}
		auto j = m_vServiceCodeInfo.find((*i)->ServiceCode);
		if (j == m_vServiceCodeInfo.end() || j->second != (*i)->ParamID)
		{
			return false;
		}
	}
	return true;
}

void CPayConfig::ClearFile()
{
	// clear file PayAileen
	// to do...
	m_oPayAileenTable.Clear();
}

INT32 CPayConfig::GetAileenItemID(std::string szParamID)
{
	UINT32 uType = CLevelSealMgr::Instance()->GetLevelSealInfo().m_uType;
	PayAileenTable::RowData* rowdata = GetTodayPayAileenTable(szParamID);
	if (!uType || NULL == rowdata || rowdata->LevelSealGiftID.size() == 0)
	{
		return 0;
	}
	if (rowdata->LevelSealGiftID.size() > uType - 1)
	{
		return rowdata->LevelSealGiftID[uType-1];
	}
	return rowdata->LevelSealGiftID[rowdata->LevelSealGiftID.size()-1];
}

INT32 CPayConfig::GetParamType(std::string szParamID)
{
	auto it = m_vParaTypeInfo.find(szParamID);
	if (it != m_vParaTypeInfo.end())
	{
		return it->second;
	}
	return KKSG::PAY_PARAM_NONE;
}
std::string CPayConfig::GetParamByServiceCode(std::string servicecode)
{
	auto it = m_vServiceCodeInfo.find(servicecode);
	if (it != m_vServiceCodeInfo.end())
	{
		return it->second;
	}
	return "";
}

INT32 CPayConfig::GetCardAward(INT32 nType)
{
	for (auto i = m_oPayCardTable.Table.begin(); i != m_oPayCardTable.Table.end(); i ++)
	{
		if ((*i)->Type == nType)
		{
			return (*i)->DayAward;
		}
	}
	return 0;
}

void CPayConfig::CheckAileenConfig(CPayV2Record* pRecord, KKSG::PayAllInfo* pPayInfo)
{
	time_t tNowTime = GameTime::GetTime();
	bool bIsNew = true;
	KKSG::PayAileen* pAileen = pPayInfo->mutable_aileen();
	KKSG::PayAileenInfo* pAileenInfo = NULL;
	UINT32 nDays = XCommon::GetWeekNum(GameTime::GetTime());
	pAileen->set_remainedtime((UINT32)(XCommon::GetDayBeginTime(tNowTime) + XCommon::GetOneDayTime() - tNowTime));
	pAileen->set_weekdays(nDays);
	for (auto i = pRecord->aileenInfo.begin(); i != pRecord->aileenInfo.end(); i ++)
	{
		pAileenInfo = pAileen->add_aileeninfo();
		pAileenInfo->set_paramid(i->second.paramid());
		pAileenInfo->set_itemid(i->second.itemid());
		pAileenInfo->set_isbuy(i->second.lastbuytime()>0?true:false);
	}
	for (auto i = m_oPayAileenTable.Table.begin(); i != m_oPayAileenTable.Table.end(); ++ i)
	{
		PayAileenTable::RowData* rowdata = *i;
		if (NULL == rowdata)
		{
			continue;
		}
		if (rowdata->Days != INT32(nDays))
		{
			continue;
		}
		auto j = pRecord->aileenInfo.find(rowdata->ParamID);
		if (j == pRecord->aileenInfo.end())
		{
			INT32 itemid = CPayConfig::Instance()->GetAileenItemID(rowdata->ParamID);
			if (itemid)
			{
				pAileenInfo = pAileen->add_aileeninfo();
				pAileenInfo->set_paramid(rowdata->ParamID);
				pAileenInfo->set_itemid(itemid);
				pAileenInfo->set_isbuy(false);
			}
		}
	}
	/*
	for (INT32 i = 0; i < pAileen->aileeninfo_size(); i ++)
	{
		LogDebug("paraid=%s,itemid=%d,isbuy=%d", pAileen->aileeninfo(i).paramid().c_str(), pAileen->aileeninfo(i).itemid(), pAileen->aileeninfo(i).isbuy()?1:0);
	}
	*/
}
void CPayConfig::CheckMemberConfig(CPayV2Record* pRecord, KKSG::PayAllInfo* pPayInfo)
{
	INT32 nowTime = INT32(GameTime::GetTime());
	for (auto i = pRecord->memberInfo.begin(); i != pRecord->memberInfo.end(); i ++)
	{
		KKSG::PayMember* pMemInfo = pPayInfo->add_paymemberinfo();
		pMemInfo->set_id(i->id());
		pMemInfo->set_isclick(i->buttonstatus()==KKSG::PAY_BUTTON_CLICK?true:false);
		pMemInfo->set_expiretime(nowTime>i->expiretime()?0:i->expiretime()-nowTime);
	}
	for (auto i = m_oPayMemberTable.Table.begin(); i != m_oPayMemberTable.Table.end(); i ++)
	{
		auto it = std::find_if(pRecord->memberInfo.begin(), pRecord->memberInfo.end(), EqualPayMemberID((*i)->ID));
		if (it == pRecord->memberInfo.end())
		{
			KKSG::PayMember* pMemInfo = pPayInfo->add_paymemberinfo();
			pMemInfo->set_id((*i)->ID);
			pMemInfo->set_isclick(false);
			pMemInfo->set_expiretime(0);
		}
	}
}

bool CPayConfig::IsPaySystemID(INT32 iSystemID)
{
	auto it = std::find(m_vPaySystemID.begin(), m_vPaySystemID.end(), iSystemID);
	if (it != m_vPaySystemID.end())
	{
		return true;
	}
	return false;
}
PayMemberTable::RowData* CPayConfig::GetPayMemberBySystem(INT32 iSystemID)
{
	for (auto i = m_oPayMemberTable.Table.begin(); i != m_oPayMemberTable.Table.end(); i ++)
	{
		if ((*i)->SystemID == iSystemID)
		{
			return *i;
		}
	}
	return NULL;
}

PayMemberTable::RowData* CPayConfig::GetPayMemberByID(INT32 iID)
{
	for (auto i = m_oPayMemberTable.Table.begin(); i != m_oPayMemberTable.Table.end(); i ++)
	{
		if ((*i)->ID == iID)
		{
			return *i;
		}
	}
	return NULL;
}

bool CPayConfig::IsTodayAlieen()
{
	UINT32 nDays = XCommon::GetWeekNum(GameTime::GetTime());
	for (auto i = m_oPayAileenTable.Table.begin(); i != m_oPayAileenTable.Table.end(); i ++)
	{
		if ((*i)->Days == nDays)
		{
			return true;
		}
	}
	return false;
}

PayAileenTable::RowData* CPayConfig::GetTodayPayAileenTable(std::string szParamID)
{
	UINT32 nDays = XCommon::GetWeekNum(GameTime::GetTime());
	for (auto i = m_oPayAileenTable.Table.begin(); i != m_oPayAileenTable.Table.end(); i ++)
	{
		if ((*i)->Days == (INT32)nDays && szParamID == (*i)->ParamID)
		{
			return (*i);
		}
	}
	return NULL;
}
