#include "pch.h"
#include "ibshopmgr.h"
#include "ibshop/ptcg2c_ibshopicon.h"
#include "ibshop/ptcg2c_ibshophasbuyntf.h"
#include "../unit/role.h"
#include "ibshoprecord.h"
#include "../util/XCommon.h"
#include "payv2Record.h"
#include "define/itemdef.h"
#include "bagtransition.h"
#include "globalconfig.h"
#include <time.h>
#include "unit/rolemanager.h"
#include "util/timespecificmgr.h"
#include "define/systemiddef.h"
#include "utility/tlogger.h"
#include "payconfig.h"
#include "gamelogic/midas.h"
#include "util/gametime.h"
#include "event/eventmgr.h"
#include "define/eventdef.h"
#include "reportdatamgr.h"
#include "reportdata/reportdata_def.h"

INSTANCE_SINGLETON(IBShopMgr);

IBShopMgr::IBShopMgr()
{
	m_Timer = CTimerMgr::Instance()->SetTimer(this, 0, 5000, -1, __FILE__, __LINE__);
}

IBShopMgr::~IBShopMgr()
{
	CTimerMgr::Instance()->KillTimer(m_Timer);
}

bool IBShopMgr::Init()
{
	bool nRet = LoadFile();;
	if (nRet)
	{
		m_nRWeek			=  GetGlobalConfig().RefreshIBShopTime[0];
		m_nRHour			=  GetGlobalConfig().RefreshIBShopTime[1];
		m_nRMin				=  GetGlobalConfig().RefreshIBShopTime[2];
		m_nTime				= (UINT32)XCommon::GetWeekBeginTime(false) + (m_nRWeek-1)*24*3600 + m_nRHour*3600 + m_nRMin*60;//周刷新时间
	}
	return nRet;
}

void IBShopMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	time_t  nTime		= GameTime::GetTime();
	if (nTime > m_nTime)
	{
		m_nTime = (UINT32)m_nTime + 7*24*3600;
		for (auto iter = RoleManager::Instance()->Begin(); iter != RoleManager::Instance()->End(); ++iter)
		{
			Role* pRole = *iter;
			if (pRole)
			{
				SendIcon2Client(pRole);
			}
		}
	}
}

UINT32 IBShopMgr::GetMaxPlayerDegree()
{
	return m_nMaxPlayerDegree;
}

UINT32 IBShopMgr::_GetCanBuyCount(Role* pRole, UINT32 goodsID, int& count)
{
	if (!pRole)
	{
		return KKSG::ERR_FAILED;
	}

	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	auto iter = pRecord->m_mapIBShopItem.find(goodsID);
	if (iter == pRecord->m_mapIBShopItem.end())
	{
		return KKSG::ERR_IBSHOP_LACKGOODS;
	}

	IBShop::RowData *pCfg = m_IBShopConfig.GetByid(goodsID);
	if (!pCfg)
		return KKSG::ERR_IBSHOP_LACKGOODS;
	if(pRole->GetLevel() < (int)pCfg->levelbuy)
		return KKSG::ERR_IBSHOP_BUYLV;


	CPayV2Record *pPayV2Record	= pRole->Get<CPayV2Record>();
	if (pPayV2Record->GetVipLevel() <  (int)pCfg->viplevel)
		return KKSG::ERR_IBSHOP_VIPLEVEL;

	if (pCfg->refreshtype == eIBRefreshOpenGroupAfter && GSConfig::Instance()->GetGameServerOpenDays() < pCfg->opensystemtime)
		return KKSG::ERR_IBSHOP_OPENGROUP;

	if (pCfg->refreshtype == eIBRefreshOpenGroupBefore && GSConfig::Instance()->GetGameServerOpenDays() > pCfg->opensystemtime)
		return KKSG::ERR_IBSHOP_OPENGROUP;

	UINT32 nCost = 0;
	auto& ibItemInfo = iter->second;
	if (ibItemInfo.nLimitCount)
	{
		count = pPayV2Record->GetRemainShopCount(goodsID) + ibItemInfo.nLimitCount - ibItemInfo.nHasBuyCount + ibItemInfo.nActivity;
		if (count<=0)
		{
			return KKSG::ERR_IBSHOP_LACKGOODS;
		}
	}

	return KKSG::ERR_SUCCESS;
}

void IBShopMgr::PackIBItems(Role *pRole,UINT32 nType,UINT32 nSubType,KKSG::IBQueryItemRes &roRes)
{
	IBShopRecord* pRecord = pRole->Get<IBShopRecord>();
	UINT32 nPlayDegree    = pRecord->GetPayDegree();
	std::list<stIBShopItem> listitem;
	IBShopMgr::Instance()->GetIBShopItems(pRole,nType,nSubType,listitem);
	for (auto iter = listitem.begin(); iter != listitem.end(); iter++)
	{
		IBShop::RowData* pCfg = IBShopMgr::Instance()->GetByGID((*iter).nGoodsID);
		if (pCfg)
		{
			KKSG::IBShopItemInfo* pItemInfo = roRes.add_iteminfo();
			pItemInfo->set_itemid(pCfg->itemid);
			pItemInfo->set_nlimitcount((*iter).nLimitCount + (*iter).nActivity);
			pItemInfo->set_goodsid((*iter).nGoodsID);
			pItemInfo->set_nbuycount((*iter).nHasBuyCount);
			pItemInfo->set_nlimittime((*iter).nLimitEndTime);
			CRoleSystem* pRoleSys = pRole->Get<CRoleSystem>();
			if (pRoleSys->IsSystemOpened(SYS_IBSHOP_GIFT) == true)
			{
				if (pCfg->paydegree > 0 && nPlayDegree >= pCfg->paydegree)
					pItemInfo->set_gift(true);
			}
			else
			{
				pItemInfo->set_gift(false);
			}
		}
	}
	//新品页面
	std::set<UINT32> setTag;
	bool bShowVip  = false;
	IBShopMgr::Instance()->BuildShowTag(pRole, nType,setTag ,bShowVip);
	for (auto iter = setTag.begin(); iter != setTag.end(); iter++)
	{
		roRes.add_newproducts(*iter);
	}
	roRes.set_viptab(bShowVip);
	roRes.set_errorcode(KKSG::ERR_SUCCESS);
}

void IBShopMgr::AddBuyCount(Role* pRole, UINT32 goodsID, UINT32 count,bool bSync)
{
	if (!pRole || !count)
	{
		return;
	}
	pRole->ClearLockTime();
	IBShop::RowData * pRowData = m_IBShopConfig.GetByid(goodsID);
	if (!pRowData)
	{
		return;
	}
	if (pRowData->fashion==true)
	{
		//todo记录购买时装上报数据
		ReportDataMgr::Instance()->Common(pRole, Tx_Reportdata_BuyFashion, ToString(1));
	}

	CPayV2Record *pPayV2Record	= pRole->Get<CPayV2Record>();
	IBShopRecord* pRecord = pRole->Get<IBShopRecord>();
	auto iter = pRecord->m_mapIBShopItem.find(goodsID);
	if (iter == pRecord->m_mapIBShopItem.end())
		return;
	if (iter->second.nActivity>0)
	{
		if (iter->second.nActivity>=count)
		{
			iter->second.nActivity-=count;
			return;
		}else
		{
			count = count- iter->second.nActivity;
			iter->second.nActivity = 0;
		}
	}
	int temp = pPayV2Record->GetRemainShopCount(goodsID);
	if (temp >= count)
	{
		pPayV2Record->UpdateShopCount(goodsID, count);
	}
	else
	{
		if (temp)
			pPayV2Record->UpdateShopCount(goodsID, temp);	
		iter->second.nHasBuyCount += (count - temp);
		pRecord->SetSaveFlag();
	}
	PtcG2C_IBShopHasBuyNtf ntf;
	ntf.m_Data.set_count(iter->second.nHasBuyCount);
	ntf.m_Data.set_goodsid(goodsID);
	pRole->Send(ntf);

	for (int i = 0; i < count; ++i)
	{
		EventMgr::Instance()->AddGuildBonusEvent(pRole->GetID(), BONUS_EVENT_OFFSET + GUILDBONUS_COM_TYPE_BUYIBSHOP, goodsID);
	}
}

const std::vector<IBShop::RowData*>* IBShopMgr::GetConfsByItemID(UINT32 itemID)
{
	auto iter = m_itemID2Confs.find(itemID);
	if (iter == m_itemID2Confs.end())
	{
		return NULL;
	}
	return &iter->second;
}

void IBShopMgr::Uninit()
{
	ClearFile();
}

bool IBShopMgr::CheckFile()
{
	IBShop tmpShopTable;
	if (!tmpShopTable.LoadFile("table/IBShop.txt"))
	{
		SSWarn<<"load file table/IBShop.txt failed"<<END;
		return false;

	}
	for(auto i = tmpShopTable.Table.begin(); i != tmpShopTable.Table.end(); i++)
	{
		IBShop::RowData* pData = *i;
		if(!pData->currencycount || !pData->currencytype)
		{
			return false;
		}
	}
	return true;
}

bool IBShopMgr::LoadFile()
{
	if (!CheckFile())
		return false;
	ClearFile();
	if (!m_IBShopConfig.LoadFile("table/IBShop.txt"))
	{
		SSWarn<<"load file table/IBShop.txt failed"<<END;
		return false;
	}
	LoadActivityTime();
	LoadMaxPlayerDegree();
	return true;
}

void IBShopMgr::LoadMaxPlayerDegree()
{
	for(auto iter = m_IBShopConfig.Table.begin(); iter!= m_IBShopConfig.Table.end(); iter++)
	{
		IBShop::RowData* pCfg = *iter;
		if(m_nMaxPlayerDegree < pCfg->paydegree)
		{
			m_nMaxPlayerDegree = pCfg->paydegree;
		}
		m_itemID2Confs[pCfg->itemid].push_back(pCfg);
	}
}

void IBShopMgr::LoadActivityTime()
{
	m_mapActivity.clear();
	for(auto iter = m_IBShopConfig.Table.begin(); iter!= m_IBShopConfig.Table.end(); iter++)
	{
		IBShop::RowData* pCfg = *iter;
		if (!pCfg||pCfg->activitytime.size()==0)
			continue;
		UINT32 nCount = 0;
		std::vector<std::string>  vec= Split(pCfg->activitytime,"|");
		for (auto siter= vec.begin(); siter!=vec.end();siter++)
		{
			std::vector<std::string> tvec = Split(*siter,"=");
			if(tvec.size()==2)
			{
				ActivityConfig time;
				time.nCount		= pCfg->activitycount[nCount];
				time.nStartTime = XCommon::ParseTime(tvec[0]);
				time.nEndTime	= XCommon::ParseTime(tvec[1]);
				m_mapActivity[(*iter)->id].push_back(time);
				++nCount;
			}
		}
	}
}


void IBShopMgr::GetActivityCount(UINT64 nRole,UINT32 nGoodsID,stIBShopItem &item)
{
	bool bFlag = true;
	UINT32 nTime = GameTime::GetTime();
	for(auto iter = m_mapActivity[nGoodsID].begin(); iter !=  m_mapActivity[nGoodsID].end(); iter++)
	{
		if (nTime>(*iter).nEndTime&&item.nActivtiyTime==(*iter).nStartTime)
		{
			LogInfo("clear role=%llu goodid=%d start=%d end=%d",nRole,nGoodsID,(*iter).nStartTime,(*iter).nEndTime);
			item.nActivity     = 0;
		}
		if (nTime>= (*iter).nStartTime &&nTime<(*iter).nEndTime&&item.nActivtiyTime != (*iter).nStartTime)
		{
			item.nActivity     = (*iter).nCount;
			item.nActivtiyTime = (*iter).nStartTime;
			LogDebug("set goodid=%d start=%d end=%d",nGoodsID,(*iter).nStartTime,(*iter).nEndTime);
			break;
		}
	}
}




int IBShopMgr::CheckIBItemLimit(vector<Sequence<string, 2>> &veccfg,UINT32 &nStartTime,UINT32 &nEndTime)
{
	nStartTime      = 0;
	nEndTime        = 0;
	UINT32 nCurr	= GameTime::GetTime();
	for (int i = 0; i < veccfg.size(); i++)
	{
		struct tm tm;
		sscanf(veccfg[i][0].c_str(), "%d-%d-%d %d:%d:%d",   
			&tm.tm_year, &tm.tm_mon, &tm.tm_mday,  
			&tm.tm_hour, &tm.tm_min, &tm.tm_sec); 
		 tm.tm_year -= 1900;
		 tm.tm_mon--;
		time_t nTStart = mktime(&tm);

		sscanf(veccfg[i][1].c_str(), "%d-%d-%d %d:%d:%d",   
			&tm.tm_year, &tm.tm_mon, &tm.tm_mday,  
			&tm.tm_hour, &tm.tm_min, &tm.tm_sec); 
		tm.tm_year -= 1900;
		tm.tm_mon--;
		time_t nTEndTime = mktime(&tm);
		if (nCurr >= nTStart && nCurr <= nTEndTime)
		{
			nStartTime = (UINT32)nTStart;
			nEndTime   = (UINT32)nTEndTime;
			return i;
		}
	}
	return -1;
}

void IBShopMgr::ClearRecord(Role *pRole)
{
	IBShopRecord* pRecord	= pRole->Get<IBShopRecord>();
	pRecord->m_mapIBShopItem.clear();
	pRecord->m_bLimitHot   = false;
	pRecord->m_nLastTime   = 0;
	pRecord->m_nVipLv      = 0;
	pRecord->ClearOrder();
	pRecord->SetSaveFlag();
}


void IBShopMgr::GetIBShopItems(Role *pRole,UINT32 nType,UINT32 nSubType,std::list<stIBShopItem > &list)
{
	BuildIBShop(pRole);
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	for (auto iter = pRecord->m_mapIBShopItem.begin(); iter != pRecord->m_mapIBShopItem.end(); iter++)
	{
		stIBShopItem item = iter->second;
		IBShop::RowData *pRowData = m_IBShopConfig.GetByid(item.nGoodsID);
		if (pRowData&&pRowData->currencytype == nType && pRowData->type == nSubType)
		{
			list.push_back(item);
		}
	}
	return;
}

std::set<UINT32> IBShopMgr::GetHotPoint(Role *pRole,bool &bLimitHot)
{
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	if (pRecord->m_bLimitHot)
		bLimitHot = true;
	return pRecord->m_setVipGoodsID;
}



void IBShopMgr::SendIcon2Client(Role *pRole)
{
	bool bLimitHot =  false;
	PtcG2C_IBShopIcon icon;
	BuildIBShop(pRole);
	std::set<UINT32> setGoods = GetHotPoint(pRole,bLimitHot);
	if (bLimitHot)
	{
		if (pRole->GetLevel()>= GetGlobalConfig().OpenIBShopRedPointLv)
			icon.m_Data.set_limittag(true);
		else
			icon.m_Data.set_limittag(false);
	}
	for (auto iter = setGoods.begin(); iter!=setGoods.end(); iter++)
	{
		icon.m_Data.add_viptag(*iter);
	}
	pRole->Send(icon);
}

void IBShopMgr::ClearIconFlag(Role *pRole)
{
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	pRecord->m_bLimitHot		= false;
	pRecord->m_setVipGoodsID.clear();
}

KKSG::ErrorCode IBShopMgr::BuyItem(Role *pRole,UINT32 nGoodsID, UINT32 nCount)
{
	BuildIBShop(pRole);
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	CPayV2Record *pPayV2Record	= pRole->Get<CPayV2Record>();
	auto iter = pRecord->m_mapIBShopItem.find(nGoodsID);
	if (iter == pRecord->m_mapIBShopItem.end())
	{
		return KKSG::ERR_IBSHOP_LACKGOODS;
	}else
	{
		stIBShopItem ibItemInfo = iter->second;
		IBShop::RowData *pCfg = m_IBShopConfig.GetByid(ibItemInfo.nGoodsID);
		if (!pCfg)
			return KKSG::ERR_IBSHOP_LACKGOODS;

		int leftBuyCount = 0;
		UINT32 error = _GetCanBuyCount(pRole, nGoodsID, leftBuyCount);
		if (error != KKSG::ERR_SUCCESS)
		{
			return (KKSG::ErrorCode)error;
		}
		if ((leftBuyCount && leftBuyCount < nCount) || !nCount)
		{
			return KKSG::ERR_IBSHOP_LIMITCOUNT;
		}

		UINT32 nCost = 0;
		UINT32 uMoneyValue = 0;
		UINT32 uMoneyExt = 0;
		if (pCfg->currencytype == DIAMOND)
		{
			UINT32 nTime = TimeUtil::GetTime();
			UINT32 nLockTime = pRole->GetLockTime();
			if (nTime < nLockTime)
			{
				LogError("ibbuy exception，midas ---roleid=%llu goodsid=%d count=%d",pRole->GetID(),nGoodsID,nCount);
				return KKSG::ERR_COMMENDWATCH_COUNTLIMIT;
			}
			if (pCfg->discount)
			{
				nCost = pCfg->currencycount*nCount*pCfg->discount/100;
			}else
			{
				nCost = pCfg->currencycount*nCount;
			}

			std::vector<ItemDesc> vecItem;
			ItemDesc item;
			item.itemID = pCfg->itemid;
			item.itemCount = nCount;
			item.isbind = pCfg->bind;
			vecItem.push_back(item);
			KKSG::PayConsumeBase info;
			info.set_type((int)MidasConsumeType_IBShop);
			info.set_amt(nCost);
			info.set_subreason(ItemFlow_BuyIBItemByDiamond);
			info.set_reason(ItemFlow_BuyIBShop);
			info.set_count(nCount);
			info.set_index(nGoodsID);
			if(!pRole->ConsumeDiamond(info,vecItem))
				return KKSG::ERR_IBSHOP_LACKDIAMOND;
			pRole->SetLockTime();

		}else if (pCfg->currencytype == DRAGON_COIN)
		{
			if (pCfg->discount)
			{
				nCost = pCfg->currencycount*nCount*pCfg->discount/100;
			}else
			{
				nCost = pCfg->currencycount*nCount;
			}

			UINT64 nDragon  = pRole->Get<Bag>()->CountVirtualItem(DRAGON_COIN);
			UINT64 nDiamond = 0;
			if (nDragon < nCost)
			{
				return KKSG::ERR_IBSHOP_LACKDRAGON;
			}else
			{
				nDragon = nCost;
			}
			BagTakeItemTransition stTakeTransition(pRole);
			stTakeTransition.SetReason(ItemFlow_BuyIBShop, ItemFlow_BuyIBItemByDragonCoin);
			if(false == stTakeTransition.TakeItem(DRAGON_COIN, (int)nDragon))
			{
				stTakeTransition.RollBack();
				return KKSG::ERR_IBSHOP_LACKDRAGON;
			}
			stTakeTransition.NotifyClient();
			uMoneyValue = nDragon;
			uMoneyExt = nDiamond;
			BagGiveItemTransition stTransition(pRole);
			stTransition.SetReason(ItemFlow_BuyIBShop, ItemFlow_BuyIBItemByDragonCoin, false);
			stTransition.GiveItem(pCfg->itemid, nCount, pCfg->bind);
			stTransition.NotifyClient();
			if (pCfg->buycount)
				AddBuyCount(pRole, nGoodsID, nCount);
			pRole->OnBuyIBShopItem();

			//tlog日志记录;
			TShopFlow oLog(pRole);
			oLog.m_iShopType = TX_NORMAL_SHOP;
			oLog.m_iMoneyType = uMoneyExt?0:pCfg->currencytype;
			oLog.m_iLimitFlag = ibItemInfo.nLimitCount?1:0;
			oLog.m_iMoneyValue = uMoneyValue;
			oLog.m_iMoneyValueExt = uMoneyExt;
			oLog.m_iItemID = pCfg->itemid;
			oLog.m_iItemCount = nCount;
			oLog.m_iGoodsType = pCfg->type;
			oLog.Do();

		}
		EventMgr::Instance()->AddEvent(pRole->GetID(), MENTOR_EVENT_OFFSET + KKSG::MentorTask_IBShopBuy, pCfg->itemid, nCount);
	}
	return  KKSG::ERR_SUCCESS;
}

void IBShopMgr::BuildShowTag(Role *pRole,UINT32 nType, std::set<UINT32 > &setIBShopTag, bool &bShowVip)
{	
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	for(auto iter = pRecord->m_mapIBShopItem.begin(); iter != pRecord->m_mapIBShopItem.end();iter++)
	{
		stIBShopItem item = iter->second;
		IBShop::RowData *pCfg = m_IBShopConfig.GetByid(item.nGoodsID);
		if (!pCfg)
		{
			continue;
		}
		if (pCfg->currencytype == nType)
		{
			if (pCfg->newproduct)
			{
				setIBShopTag.insert(pCfg->type);
			}
			if (pCfg->type == eIBVip && pCfg->buycount > item.nHasBuyCount)
			{
				bShowVip =  true;
			}
		}
	}
}


//数据build的接口限制，1min只限制build一次，正常概率发生变化不大
void IBShopMgr::BuildIBShop(Role *pRole)
{
	IBShopRecord* pRecord		= pRole->Get<IBShopRecord>();
	CPayV2Record *pPayV2Record	= pRole->Get<CPayV2Record>();
	UINT32 nVipLv				= pPayV2Record->GetVipLevel();
	UINT32 nRecoredVipLv		= pRecord->m_nVipLv;
	UINT32 nDayTime             = GameTime::GetDayBeginTime(GameTime::GetTime());
	pRecord->m_setVipGoodsID.clear();
	for(auto iter = m_IBShopConfig.Table.begin(); iter != m_IBShopConfig.Table.end(); iter++)
	{
		IBShop::RowData* pCfg = *iter;
		UINT32 nStartTime = 0;
		UINT32 nEndTime   = 0; 
		if (pCfg->level[1]!=0&&(pRole->GetLevel() < pCfg->level[0]||pRole->GetLevel()>pCfg->level[1]))
		{
			auto miter = pRecord->m_mapIBShopItem.find(pCfg->id);
			if (miter != pRecord->m_mapIBShopItem.end())
				pRecord->m_mapIBShopItem.erase(miter);
			continue;
		}
		if (pCfg->refreshtype == eIBRefreshOpenGroupAfter)
		{
			if (GSConfig::Instance()->GetGameServerOpenDays() < pCfg->opensystemtime)
			{
				auto miter = pRecord->m_mapIBShopItem.find(pCfg->id);
				if (miter != pRecord->m_mapIBShopItem.end())
					pRecord->m_mapIBShopItem.erase(miter);
				continue;
			}
		}

		if (pCfg->refreshtype == eIBRefreshOpenGroupBefore)
		{
			if (GSConfig::Instance()->GetGameServerOpenDays() > pCfg->opensystemtime)
			{
				auto miter = pRecord->m_mapIBShopItem.find(pCfg->id);
				if (miter != pRecord->m_mapIBShopItem.end())
					pRecord->m_mapIBShopItem.erase(miter);
				continue;
			}
		}

		if (pCfg->refreshtype == eIBRefreshTimer)
		{
			if (pRecord->m_nLastTime < m_nTime)
			{
				pRecord->m_bLimitHot								= true;
				pRecord->m_mapIBShopItem[pCfg->id].nHasBuyCount		= 0;
				pRole->Get<CPayV2Record>()->RefreshShopCount();
			}
		}

		if (pCfg->refreshtype == eIBRefreshTimePeriod||(pCfg->refreshtype == eIBRefreshDayPeriod&&(pCfg->refreshtype&(1<<eIBRefreshTimePeriod))))
		{
			int nFlag = CheckIBItemLimit(pCfg->limittime,nStartTime,nEndTime);
			if (nFlag!=-1)
			{
				if (nEndTime > pRecord->m_mapIBShopItem[pCfg->id].nLimitEndTime)
				{
					pRecord->m_mapIBShopItem[pCfg->id].nLimitEndTime = nEndTime;
					pRecord->m_mapIBShopItem[pCfg->id].nHasBuyCount	 = 0;
				}
			}else
			{
				auto miter = pRecord->m_mapIBShopItem.find(pCfg->id);
				if (miter != pRecord->m_mapIBShopItem.end())
				{
					pRecord->m_mapIBShopItem.erase(miter);
				}
				continue;
			}

			if (pCfg->refreshtype == eIBRefreshDayPeriod)
			{
				if (pRecord->m_nDayTime < nDayTime)
				{
					pRecord->m_bLimitHot								= true;
					pRecord->m_mapIBShopItem[pCfg->id].nHasBuyCount		= 0;
					pRole->Get<CPayV2Record>()->RefreshShopCount();
				}
			}
		}

		if (pCfg->type == eIBVip && nRecoredVipLv < nVipLv)//显示的goodesid
		{
			pRecord->m_setVipGoodsID.insert(pCfg->id);
		}

		GetActivityCount(pRole->GetID(),pCfg->id, pRecord->m_mapIBShopItem[pCfg->id]);
		pRecord->m_mapIBShopItem[pCfg->id].nGoodsID		     = pCfg->id;
		pRecord->m_mapIBShopItem[pCfg->id].nLimitStartTime   = nStartTime;
		pRecord->m_mapIBShopItem[pCfg->id].nLimitEndTime	 = nEndTime;
		pRecord->m_mapIBShopItem[pCfg->id].nCurrencyCount	 = pCfg->currencycount;
		pRecord->m_mapIBShopItem[pCfg->id].nLimitCount       = pCfg->buycount;
		pRecord->m_mapIBShopItem[pCfg->id].nDiscount		 = pCfg->discount;
		if (pRecord->m_mapIBShopItem[pCfg->id].nHasBuyCount > pCfg->buycount)
		{
			pRecord->m_mapIBShopItem[pCfg->id].nHasBuyCount = pCfg->buycount;
		}
	}

	for (auto iter = pRecord->m_mapIBShopItem.begin(); iter != pRecord->m_mapIBShopItem.end();)
	{
		stIBShopItem item = iter->second;
		IBShop::RowData * pConfig = m_IBShopConfig.GetByid(item.nGoodsID);
		if (!pConfig)
		{
			pRecord->m_mapIBShopItem.erase(iter++);
		}else
		{
			iter++;
		}
	}

	pRecord->m_nVipLv = nVipLv;
	if(pRecord->m_nLastTime < m_nTime)
	{
		LogInfo("ibshop refresh time nRoleID=%llu nTime=%u",pRole->GetID(),m_nTime);
		pRecord->m_nLastTime = m_nTime;
	}
	if (pRecord->m_nDayTime < nDayTime)
	{
		pRecord->m_nDayTime = nDayTime;
	}
	pRecord->SetSaveFlag();
}

void IBShopMgr::ClearFile()
{
	m_IBShopConfig.Clear();
	m_nMaxPlayerDegree = 0;
	m_itemID2Confs.clear();
}

void IBShopMgr::GetPayMemberPrivilegeShop(Role* pRole, INT32 rate, std::vector<KKSG::PayPrivilegeShop> &list)
{
	for (auto i = m_IBShopConfig.Table.begin(); i != m_IBShopConfig.Table.end(); i ++)
	{
		if ((*i)->ischeckpaymember)
		{
			auto it = std::find_if(list.begin(), list.end(), EqualPayMemberID((*i)->id));
			if (it == list.end())
			{
				KKSG::PayPrivilegeShop roInfo;
				roInfo.set_goodsid((*i)->id);
				roInfo.set_totalcount(INT32((*i)->buycount * rate / 100.0));
				roInfo.set_usedcount(0);
				list.push_back(roInfo);
			}
			else
			{
				it->set_totalcount(INT32((*i)->buycount * rate / 100.0));
			}
		}
	}
}

