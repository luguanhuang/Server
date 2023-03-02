#include "pch.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "timeutil.h"
#include "ibgiftmgr.h"
#include "role/rolemanager.h"
#include "ibshop/rpcm2g_giftibitemreq.h"
#include "pay/rpcm2d_savegiftibitem.h"
#include "pay/rpcm2d_savegiftibhist.h"
#include "pay/ptcm2c_ibgifticonntf.h"
#include "pay/ptcm2c_giftibsucessntf.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"


#include "network/dblink.h"
#include "util/gametime.h"
#include "pay/ptcm2d_savepayibitem.h"
#include "network/dblink.h"
#include "config.h"
#include "paymgr.h"
#include "SnsSigCheck.h"
#include "table/globalconfig.h"
#include "role/rolemanager.h"
#include "role/rolesummary.h"
#include "role/rolesummarymgr.h"
#include "event/eventmgr.h"
#include "platforminfo/platformfriendmgr.h"

#include "loghelper/tlogr.h"

INSTANCE_SINGLETON(IBGiftMgr);

IBGiftMgr::IBGiftMgr(){}

IBGiftMgr::~IBGiftMgr(){}

bool IBGiftMgr::Init()
{
	bool bRet = ReadAllGiftFromDB();
	if(bRet == false)
	{
		return false;
	}
	bRet = ReadHistGiftDB();
	if (bRet == false)
	{
		return false;
	}
	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	m_nTime = 0;
	return bRet;
}

void IBGiftMgr::Uninit()
{
	SaveHistGift(true);
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
}

bool IBGiftMgr::ReadHistGiftDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	std::stringstream ss;
	ss << "select _id,giftself,gifttarget from giftrecord";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table giftrecord failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT64 dwLen = 0;

		UINT32 nLen	= 0;
		UINT64 nRoleID = 0;
		if(poRes->GetFieldValue(0, &pVal, nLen))
		{
			nRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read '_id' column failed");
			return false;
		}
		std::string strData;
		if(poRes->GetFieldValue(1, &pVal, nLen))
		{
			strData.assign(pVal, nLen);
		}
		else
		{
			LogError("Read 'giftself' column failed");
			return false;
		}

		KKSG::IBGiftHistAllItem allItem;
		if(!allItem.ParseFromString(strData))
		{
			LogError("Parse giftself failed, roleID %llu", nRoleID);
			return false;
		}

		for (int i = 0; i < allItem.allitem_size(); i++)
		{
			IBGiftHistItem  *pHistInfo = allItem.mutable_allitem(i);
			stIBGiftHistItem histitem;
			histitem.name = pHistInfo->name();
			histitem.nTime = pHistInfo->time();
			histitem.item.CopyFrom(pHistInfo->item());
			m_mapSelfHistItems[nRoleID].push_back(histitem);
		}

		if(poRes->GetFieldValue(2, &pVal, nLen))
		{
			strData.assign(pVal, nLen);
		}
		else
		{
			LogError("Read 'gifttarget' column failed");
			return false;
		}
		allItem.Clear();
		if(!allItem.ParseFromString(strData))
		{
			LogError("Parse gifttarget failed, roleID %llu", nRoleID);
			return false;
		}
		for (int i = 0; i < allItem.allitem_size(); i++)
		{
			IBGiftHistItem  *pHistInfo = allItem.mutable_allitem(i);
			stIBGiftHistItem histitem;
			histitem.name = pHistInfo->name();
			histitem.nTime = pHistInfo->time();
			histitem.item.CopyFrom(pHistInfo->item());
			m_mapTargetHistItems[nRoleID].push_back(histitem);
		}
	}
	poRes->Release();
	return true;
}

bool IBGiftMgr::ReadAllGiftFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	std::stringstream ss;
	ss << "select billno,fromroleid,toroleid,goodsid,itemid,count,paydegree,text,ts,status from payibitem";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table payibitem failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}
	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;

		std::string strBillno;
		if(poRes->GetFieldValue(0, &pVal, dwLen))
		{
			strBillno.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'billno' column failed");
			return false;
		}

		UINT64 roleID = 0;
		if (poRes->GetFieldValue(1, &pVal, dwLen))
		{
			roleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'fromroleid' column failed");
			return false;
		}

		UINT64 toRoleID = 0;
		if (poRes->GetFieldValue(2, &pVal, dwLen))
		{
			toRoleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'toroleid' column failed");
			return false;
		}

		std::string strGoodsid;
		if(poRes->GetFieldValue(3, &pVal, dwLen))
		{
			strGoodsid.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'goodsid' column failed");
			return false;
		}

		UINT32 itemid = 0;
		if (poRes->GetFieldValue(4, &pVal, dwLen))
		{
			itemid = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'itemid' column failed");
			return false;
		}


		UINT32 Count = 0;
		if (poRes->GetFieldValue(5, &pVal, dwLen))
		{
			Count = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'count' column failed");
			return false;
		}

		UINT32 PayDegree = 0;
		if (poRes->GetFieldValue(6, &pVal, dwLen))
		{
			PayDegree = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'paydegree' column failed");
			return false;
		}


		std::string strText;
		if (poRes->GetFieldValue(7, &pVal, dwLen))
		{
			strText.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'text' column failed");
			return false;
		}

		UINT32 ts = 0;
		if (poRes->GetFieldValue(8, &pVal, dwLen))
		{
			ts = convert<UINT32>(pVal);
		}
		else
		{
			LogError("Read 'ts' column failed");
			return false;
		}

		UINT32 status = 0;
		if (poRes->GetFieldValue(9, &pVal, dwLen))
		{
			status = convert<INT32>(pVal);
		}
		else
		{
			LogError("Read 'status' column failed");
			return false;
		}
		KKSG::PayGiftIbItem *pData = new KKSG::PayGiftIbItem();
		pData->set_billno(strBillno);
		pData->set_fromroleid(roleID);
		pData->set_toroleid(toRoleID);
		pData->set_goodsid(strGoodsid);
		pData->set_count(Count);
		pData->set_ts(ts);
		pData->set_status(status);
		pData->set_text(strText);
		pData->set_itemid(itemid);
		auto it = m_mapGiftIb.find(roleID);
		if (it != m_mapGiftIb.end())
		{
			it->second.push_back(pData);
		}
		else
		{
			std::vector<KKSG::PayGiftIbItem*> vItem;
			vItem.push_back(pData);
			m_mapGiftIb.insert(std::make_pair(roleID, vItem));
		}
		if (pData->status() == PayGiftIbStatus_SuccessNotAddItem)
		{
			AddToIbItem(pData);
		}
		m_mapBill[strBillno] = roleID;
	}
	poRes->Release();
	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all punish data from db use time: %llu ms", qwTimeUsed);
	return true;
}

UINT32 IBGiftMgr::GetUnRecvGift(UINT64 nRole)
{
	UINT32 nTotal = 0;
	auto iter = m_mapToGiftIb.find(nRole);
	if (iter!=m_mapToGiftIb.end())
	{
		for (auto siter = iter->second.begin(); siter!=iter->second.end();siter++)
		{
			KKSG::PayGiftIbItem  *pGiftItem = *siter;
			if(pGiftItem->status() == PayGiftIbStatus_SuccessNotAddItem)
			{
				nTotal++;
			}
		}
	}
	return nTotal;
}


void IBGiftMgr::GiftItem(KKSG::PayGiftIbItem &info)
{
	//1：查看玩家在在线否
	//2：发送rpc到gamesever，gameserver回馈决定是否存档。
	IBGiftMgr::stIBGiftHistItem histItem;
	const std::string szToName =  CRoleSummaryMgr::Instance()->GetRoleName(info.toroleid());
	
	histItem.name = szToName;
	histItem.item.set_itemid(info.itemid());
	histItem.item.set_itemcount(info.count());
	histItem.nTime = GameTime::GetTime();
	IBGiftMgr::Instance()->AddHistItem(info.fromroleid(),IBGiftMgr::eIBGIFTSELF,histItem);
	AddToIbItem(&info);

	CRole *pFromRole = CRoleManager::Instance()->GetByRoleID(info.fromroleid());
	if (pFromRole!=NULL)
	{
		SendIcon(pFromRole);
		CRoleSummary* pToSum   = CRoleSummaryMgr::Instance()->GetRoleSummary(info.toroleid());
		CRoleSummary* pFromSum = CRoleSummaryMgr::Instance()->GetRoleSummary(info.fromroleid());
		PtcM2C_GiftIBSucessNtf ntf;	
		ntf.m_Data.set_openid("");
		if (pToSum&&pFromSum)
		{
			if(CPlatformFriendMgr::Instance()->IsPlatFriend(pFromSum->GetAccount(),pToSum->GetAccount()) == true)
			{
				ntf.m_Data.set_openid(pToSum->GetAccount());
				ntf.m_Data.set_name(pFromSum->GetName());
			}
		}
		pFromRole->Send(ntf);
		LogInfo("----AddIbItem fromroleid=%llu name=%s", info.fromroleid(),pToSum->GetAccount().c_str());
	}

	CRole *pRole = CRoleManager::Instance()->GetByRoleID(info.toroleid());
	if(pRole)
	{
		RpcM2G_GiftIbItemReq* pRpc = RpcM2G_GiftIbItemReq::CreateRpc();
		pRpc->m_oArg.set_roleid(info.toroleid());
		auto iter = m_mapToGiftIb.find(info.toroleid());
		if (iter!=m_mapToGiftIb.end())
		{
			for (auto siter = iter->second.begin(); siter!=iter->second.end();siter++)
			{
				KKSG::PayGiftIbItem  *pGiftItem = *siter;
				LogInfo("GiftItem billno=%s fromroleid=%llu toroleid=%llu status=%u",pGiftItem->billno().c_str(),pGiftItem->fromroleid(),pGiftItem->toroleid(),pGiftItem->status());
				if (pGiftItem->status() == PayGiftIbStatus_SuccessNotAddItem)
				{
					PayGiftIbItem *pItem =pRpc->m_oArg.add_allitem();
					pItem->CopyFrom(*pGiftItem);
					CRoleSummary* pFromSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGiftItem->fromroleid());
					if (pFromSum)
					{
						pItem->set_name(pFromSum->GetName());
						pItem->set_fromopenid(pFromSum->GetAccount());
					}
					CRoleSummary* pToSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pGiftItem->toroleid());
					if (pToSum)
						pItem->set_toopenid(pToSum->GetAccount());	
				}
			}
		}
		SendIcon(pRole);
		pRole->SendMsgToGS(*pRpc);
	}
}



UINT32 IBGiftMgr::GetSize()
{
	return 0;
}
bool IBGiftMgr::CheckStatusExpire(UINT32 sta, UINT32 ts)
{
	UINT32 now = (UINT32)GameTime::GetTime();
	if (sta == KKSG::PayGiftIbStatus_CreateBill && (ts + MAX_PAYGIFT_CREATEBILL) < now)
	{
		return true;
	}

	if (sta == KKSG::PayGiftIbStatus_Fail && (ts + MAX_PAYGIFT_FAIL) < now)
	{
		return true;
	}

	if (sta == KKSG::PayGiftIbStatus_Success && (ts + MAX_PAYGIFT_SUCCESS) < now)
	{
		return true;
	}

	return false;
}

void IBGiftMgr::DelToGiftIBItem(const KKSG::PayGiftIbItem* pItem)
{
	auto toit = m_mapToGiftIb.find(pItem->toroleid());
	if (toit!=m_mapToGiftIb.end())
	{
		auto iter = toit->second.begin();
		while(iter!=toit->second.end())
		{
			if (pItem == *iter)
			{
				LogInfo("DelToGiftIBItem oreder=fromroleid=%llu toroleid=%llu billno=%s status=%d itemid=%d count=%d ts=%d text=%s",
					(*iter)->fromroleid(),(*iter)->toroleid(),(*iter)->billno().c_str(),(*iter)->status(),(*iter)->itemid(),(*iter)->count(),(*iter)->ts(),(*iter)->text().c_str());
				iter = toit->second.erase(iter);
				break;
			}
			iter++;
		}
		if (toit->second.empty())
			m_mapToGiftIb.erase(toit);
	}
}


void IBGiftMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	UINT32 now = (UINT32)GameTime::GetTime();
	if (now> m_nTime)
	{
		m_nTime = now + 60;
		for (auto it = m_mapGiftIb.begin(); it != m_mapGiftIb.end(); )
		{
			for (auto i = it->second.begin(); i != it->second.end(); )
			{
				KKSG::PayGiftIbItem* pItem = *i;
				if (CheckStatusExpire(pItem->status(), pItem->ts()))
				{
					Save(KKSG::MYSQL_OP_DELETE, *pItem);
					LogInfo("timer del oreder=fromroleid=%llu toroleid=%llu billno=%s status=%d itemid=%d count=%d ts=%d text=%s",
						(*i)->fromroleid(),(*i)->toroleid(),(*i)->billno().c_str(),(*i)->status(),(*i)->itemid(),(*i)->count(),(*i)->ts(),(*i)->text().c_str());
					m_mapBill.erase(pItem->billno());
					i = it->second.erase(i);
					DelToGiftIBItem(pItem);
					delete pItem;
				}
				else
				{
					i++;
				}
			}

			if (it->second.empty())
			{
				m_mapGiftIb.erase(it ++);			
			}
			else
			{
				it ++;
			}
		}
	}

	SaveHistGift();
}

void IBGiftMgr::SaveHistGift(bool bAll)
{
	UINT32 nSaveMax = 10;
	UINT32 i = 0;
	auto iter = m_mapSaveHistInfo.begin();
	while (iter!=m_mapSaveHistInfo.end())
	{
		if (bAll == false)
			if (i>=nSaveMax)
				break;
		RpcM2D_SaveGiftIbHist* pRpc = RpcM2D_SaveGiftIbHist::CreateRpc();
		pRpc->m_oArg.CopyFrom(iter->second);
		DBLink::Instance()->SendTo(*pRpc);
		m_mapSaveHistInfo.erase(iter++);
		i++;
	}
}

void IBGiftMgr::Save(KKSG::MysqlOpType op, const KKSG::PayGiftIbItem& oItem)
{
	LogInfo("save=fromroleid=%llu toroleid=%llu billno=%s status=%d itemid=%d count=%d ts=%d text=%s",\
		oItem.fromroleid(),oItem.toroleid(),oItem.billno().c_str(),oItem.status(),oItem.itemid(),oItem.count(),oItem.ts(),oItem.text().c_str());
	PtcM2D_SavePayIbItem ptc;
	ptc.m_Data.set_op(op);
	ptc.m_Data.mutable_data()->CopyFrom(oItem);
	DBLink::Instance()->SendTo(ptc);
}
std::string IBGiftMgr::PayBuyGoodsRes(INT32 ret, std::string msg)
{
	std::ostringstream ss;
	ss << "{\"ret\":" << ret << ",\"msg\":\"" << msg << "\"}";
	return ss.str();
}
INT32 IBGiftMgr::PayBuyGoods(std::map<std::string, std::string>& params, std::string& sig, std::ostringstream& msg)
{
	//充值发货根据订单号来处理;
	INT32 ret = 1;
	std::ostringstream ss;
	std::vector<std::string> vec;
	if (params.find("channel_id") == params.end())
	{
		msg << "error,no channel_id param";
		return ret;
	}
	if (params.find("openid") == params.end())
	{
		msg << "error,no openid param";
		return ret;
	}
	vec = Split(params["channel_id"], "-");
	if (vec.size() < 3)
	{
		msg << "error,invalid channel_id=" << params["channel_id"];
		return ret;
	}
	std::string strBillNo = vec[vec.size()-2];
	KKSG::PayGiftIbItem* pIbItem = GetIbItem(strBillNo);
	if (NULL == pIbItem)
	{
		msg << "err,invalid billno=" << strBillNo;
		return ret;
	}

	CRole* role = CRoleManager::Instance()->GetByRoleID(pIbItem->fromroleid());
	INT32 platid = role==NULL?INT32(MSConfig::Instance()->GetPlatType()):role->GetLoginPlatID();
	
	std::string strMethod = "GET";
	std::string urlpath = BuyGoodsCallBack;
	std::string appkey = CPayMgr::Instance()->GetMidasPayKey(platid, PayTypeBuyGoods);
	if(!CSnsSigCheck::Instance()->verifySig(strMethod, urlpath, params, sig, appkey))
	{
		msg << "sig err,roleid=" << pIbItem->fromroleid() << ",sig=" << sig;
		return ret;
	}
	//订单已发过货;
	if (pIbItem->status() == PayGiftIbStatus_Success || pIbItem->status() == PayGiftIbStatus_SuccessNotAddItem)
	{
		SSWarn << "already add item,roleid=" << pIbItem->fromroleid() << END;
		msg << "OK";
		return 0;
	}
	pIbItem->set_status(PayGiftIbStatus_SuccessNotAddItem);
	Save(MYSQL_OP_UPDATE,*pIbItem);
	
	EventMgr::Instance()->AddEvent(pIbItem->fromroleid(),MIX_EVENT_OFFSET+EVENT_MIDAS_PAY_DEGREE,pIbItem->paydegree());
	EventMgr::Instance()->AddEvent(pIbItem->fromroleid(),MIX_EVENT_OFFSET+EVENT_MIDAS_PAY_COUNT);

	GiftItem(*pIbItem);
	msg << "OK";

	CRoleSummary* summary = CRoleSummaryMgr::Instance()->GetRoleSummary(pIbItem->fromroleid());
	if (summary)
	{
		DoTxLog(summary->GetAccount(), pIbItem);
	}

	return 0;
}


void IBGiftMgr::AddSimOrder(CRole *pFrom,CRole *pTo)
{
	KKSG::PayGiftIbItem *pData = new KKSG::PayGiftIbItem();
	std::string strTime = TimeUtil::GetDateTimeStr(GameTime::GetTime());
	pData->set_billno(strTime);
	pData->set_text("diff debug");
	CRoleSummary* pFromSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pFrom->GetID());
	if (pFromSum)
	{
		pData->set_fromopenid(pFromSum->GetAccount());
	}

	CRoleSummary* pToSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pTo->GetID());
	if (pToSum)
	{
		pData->set_toopenid(pToSum->GetAccount());
	}
	pData->set_goodsid("com.tencent.dragonnest.box_980");
	pData->set_toroleid(pTo->GetID());
	pData->set_fromroleid(pFrom->GetID());
	pData->set_paydegree(10);
	pData->set_ts(GameTime::GetTime());
	pData->set_itemid(1);
	pData->set_count(1);
	pData->set_status(PayGiftIbStatus_SuccessNotAddItem);
	GiftItem(*pData);
}

KKSG::PayGiftIbItem* IBGiftMgr::GetIbItem(const std::string& strBillNo)
{
	auto it = m_mapBill.find(strBillNo);
	return it!=m_mapBill.end()?GetIbItem(it->second, strBillNo):NULL;
}

KKSG::PayGiftIbItem* IBGiftMgr::GetIbItem(UINT64 qwRoleId, const std::string& strBillNo)
{
	auto it = m_mapGiftIb.find(qwRoleId);
	if (it != m_mapGiftIb.end())
	{
		for (UINT32 i = 0; i < it->second.size(); i ++)
		{
			if (it->second[i]->billno() == strBillNo)
			{
				return it->second[i];
			}
		}
	}
	return NULL;
}


void IBGiftMgr::TestGiftIBSucessNtf(CRole *pRole)
{

	CRoleSummary* pToSum = CRoleSummaryMgr::Instance()->GetRoleSummary(pRole->GetID());
	if (pToSum)
	{
		PtcM2C_GiftIBSucessNtf ntf;
		ntf.m_Data.set_openid(pToSum->GetAccount());
		pRole->Send(ntf);
	}

}


void IBGiftMgr::AddIbItem(UINT64 qwRoleId,KKSG::PayGiftIbItem* pItem)
{
	if (!pItem)
	{
		return;
	}

	//通知客户端
	AddToIbItem(pItem);
	auto it = m_mapGiftIb.find(qwRoleId);
	if (it != m_mapGiftIb.end())
	{
		it->second.push_back(pItem);
	}
	else
	{
		std::vector<KKSG::PayGiftIbItem*> vec;
		vec.push_back(pItem);
		m_mapGiftIb.insert(std::make_pair(qwRoleId, vec));
	}
	m_mapBill[pItem->billno()] = qwRoleId;
	Save(MYSQL_OP_ADD, *pItem);
}


void IBGiftMgr::AddNewIbItem(KKSG::PayGiftIbItem &oItem)
{
	KKSG::PayGiftIbItem *pData = new KKSG::PayGiftIbItem();
	pData->CopyFrom(oItem);
	AddIbItem(oItem.fromroleid(),pData);
}

void IBGiftMgr::AddToIbItem(KKSG::PayGiftIbItem* pItem)
{
	if (!pItem)
	{
		return;
	}
	LogInfo("_AddToIbItem =fromroleid=%llu toroleid=%llu billno=%s status=%d itemid=%d count=%d ts=%d text=%s",
		pItem->fromroleid(),pItem->toroleid(),pItem->billno().c_str(),pItem->status(),pItem->itemid(),pItem->count(),pItem->ts(),pItem->text().c_str());

	auto it = m_mapToGiftIb.find(pItem->toroleid());
	if (it != m_mapToGiftIb.end())
	{
		for (auto si = m_mapToGiftIb[pItem->toroleid()].begin(); si !=m_mapToGiftIb[pItem->toroleid()].end();si++)
		{
			if ((*si)->billno()==pItem->billno())
			{
			    si = m_mapToGiftIb[pItem->toroleid()].erase(si);
				break;
			}
		}
		m_mapToGiftIb[pItem->toroleid()].push_back(pItem);
	}
	else
	{
		std::vector<KKSG::PayGiftIbItem*> vec;
		vec.push_back(pItem);
		m_mapToGiftIb.insert(std::make_pair(pItem->toroleid(), vec));
	}
}




void IBGiftMgr::AddHistItem(UINT64 nRole,UINT32 type,const stIBGiftHistItem &item)
{
	if (type == eIBGIFTSELF)
	{
		if (m_mapSelfHistItems.size())
		{
			auto iter = m_mapSelfHistItems.find(nRole);
			if (iter != m_mapSelfHistItems.end())
			{
				std::vector<stIBGiftHistItem>& vecItems = iter->second;
				vecItems.push_back(item);
				if (vecItems.size()>=GetGlobalConfig().IBShopHistMaxSize)
				{
					std::sort(vecItems.begin(),vecItems.end(),stIBGiftHistItemCmp());
					vecItems.pop_back();
				}else
				{
					std::sort(vecItems.begin(),vecItems.end(),stIBGiftHistItemCmp());
				}
			}else
			{
				std::vector<stIBGiftHistItem> vecItems;
				vecItems.push_back(item);
				m_mapSelfHistItems.insert(std::pair<UINT64,std::vector<stIBGiftHistItem>>(nRole,vecItems));
			}
			return;
		}else
		{
			std::vector<stIBGiftHistItem> vecItems;
			vecItems.push_back(item);
			m_mapSelfHistItems.insert(std::pair<UINT64,std::vector<stIBGiftHistItem>>(nRole,vecItems));
		}
	}else
	{
		if (m_mapTargetHistItems.size())
		{
			auto iter = m_mapTargetHistItems.find(nRole);
			if (iter != m_mapTargetHistItems.end())
			{
				std::vector<stIBGiftHistItem>& vecItems = iter->second;
				vecItems.push_back(item);
				if (vecItems.size()>=GetGlobalConfig().IBShopHistMaxSize)
				{
					std::sort(vecItems.begin(),vecItems.end(),stIBGiftHistItemCmp());
					vecItems.pop_back();
				}else
				{
					std::sort(vecItems.begin(),vecItems.end(),stIBGiftHistItemCmp());
				}
			}else
			{
				std::vector<stIBGiftHistItem> vecItems;
				vecItems.push_back(item);
				m_mapTargetHistItems.insert(std::pair<UINT64,std::vector<stIBGiftHistItem>>(nRole,vecItems));
			}
			return;
		}else
		{
			std::vector<stIBGiftHistItem> vecItems;
			vecItems.push_back(item);
			m_mapTargetHistItems.insert(std::pair<UINT64,std::vector<stIBGiftHistItem>>(nRole,vecItems));
		}
	}

	PrepSave(nRole);
}

void IBGiftMgr::PrepSave(UINT64 nRole)
{
	KKSG::SaveIbGiftRecordArg  arg;
	arg.set_roleid(nRole);
	arg.set_opertype(KKSG::SaveIbGift_All);
	auto iter = m_mapSelfHistItems.find(nRole);
	if (iter!=m_mapSelfHistItems.end())
	{
		for (auto siter = iter->second.begin(); siter!=iter->second.end();siter++)
		{	
			KKSG::IBGiftHistItem  *pGift =  arg.mutable_giftself()->add_allitem();
			pGift->set_name(siter->name);
			pGift->set_time(siter->nTime);
			pGift->mutable_item()->CopyFrom(siter->item);
		}
	}
	auto titer = m_mapTargetHistItems.find(nRole);
	if (titer!=m_mapTargetHistItems.end())
	{
		for (auto siter = titer->second.begin(); siter!=titer->second.end();siter++)
		{	
			KKSG::IBGiftHistItem  *pGift =  arg.mutable_gifttarget()->add_allitem();
			pGift->set_name(siter->name);
			pGift->set_time(siter->nTime);
			pGift->mutable_item()->CopyFrom(siter->item);
		}
	}
	auto miter = m_mapSaveHistInfo.find(nRole);
	if (miter!=m_mapSaveHistInfo.end())
	{
		miter->second = arg;
	}else
	{
		m_mapSaveHistInfo.insert(std::pair<UINT64, SaveIbGiftRecordArg>(nRole,arg));
	}
}

void IBGiftMgr::GetHistItem(UINT64 nRole,UINT32 type, KKSG::IBGiftHistAllItem *allitem)
{
	if(type == eIBGIFTSELF)
	{
		if (m_mapSelfHistItems.size())
		{
			auto iter = m_mapSelfHistItems.find(nRole);
			if (iter != m_mapSelfHistItems.end())
			{
				std::vector<stIBGiftHistItem> &vecHist = iter->second;
				for (auto siter = vecHist.begin();siter!=vecHist.end();siter++)
				{
					KKSG::IBGiftHistItem *pHistItem = allitem->add_allitem();
					KKSG::ItemBrief *pItemBrief     = pHistItem->mutable_item(); 
					pHistItem->set_name((*siter).name);
					pHistItem->set_time((*siter).nTime);
					pItemBrief->CopyFrom((*siter).item);
				}
			}
		}
	}else
	{
		if (m_mapTargetHistItems.size())
		{
			auto iter = m_mapTargetHistItems.find(nRole);
			if (iter != m_mapTargetHistItems.end())
			{
				std::vector<stIBGiftHistItem> &vecHist = iter->second;
				for (auto siter = vecHist.begin();siter!=vecHist.end();siter++)
				{
					KKSG::IBGiftHistItem *pHistItem = allitem->add_allitem();
					KKSG::ItemBrief *pItemBrief     = pHistItem->mutable_item(); 
					pHistItem->set_name((*siter).name);
					pHistItem->set_time((*siter).nTime);
					pItemBrief->CopyFrom((*siter).item);
				}
			}
		}
	}

}

KKSG::PayGiftIbItem *IBGiftMgr::GetGoods(CRole *pRole)
{
	if (pRole == NULL)
		return NULL;
	auto iter  = m_mapToGiftIb.find(pRole->GetID());
	if (iter!=m_mapToGiftIb.end())
	{
		for(auto siter = iter->second.begin(); siter!=iter->second.end();siter++)
		{
			if ((*siter)->status()==PayGiftIbStatus_SuccessNotAddItem)
			{
				return *siter;
			}
		}
	}
	return NULL;
}


void IBGiftMgr::OnLogin(CRole *pRole)
{

	if (pRole == NULL)
		return;

	auto iter  = m_mapToGiftIb.find(pRole->GetID());
	if (iter!=m_mapToGiftIb.end())
	{
		RpcM2G_GiftIbItemReq* pRpc = NULL;	

		for(auto siter = iter->second.begin(); siter!=iter->second.end();siter++)
		{
			if ((*siter)->status()==PayGiftIbStatus_SuccessNotAddItem)
			{
				if(pRpc == NULL)
				{
					pRpc = RpcM2G_GiftIbItemReq::CreateRpc();
					pRpc->m_oArg.set_roleid(pRole->GetID());
				}
				PayGiftIbItem *pItem =pRpc->m_oArg.add_allitem();
				pItem->CopyFrom(**siter);
				CRoleSummary* pFromSum = CRoleSummaryMgr::Instance()->GetRoleSummary((*siter)->fromroleid());
				if (pFromSum)
				{
					pItem->set_name(pFromSum->GetName());
					pItem->set_fromopenid(pFromSum->GetAccount());
				}

				CRoleSummary* pToSum = CRoleSummaryMgr::Instance()->GetRoleSummary((*siter)->toroleid());
				if (pToSum)
					pItem->set_toopenid(pToSum->GetAccount());	

			}
		}
		if (pRpc)
		{
			pRole->SendMsgToGS(*pRpc);
		}
	}
	SendIcon(pRole);
}

void IBGiftMgr::SendIcon(CRole *pRole)
{
	if (pRole == NULL)
	{
		return;
	}
	bool bIcon = false;
	auto mapTo = m_mapToGiftIb.find(pRole->GetID());
	if (mapTo!=m_mapToGiftIb.end())
	{
		if(mapTo->second.empty() ==false)
		{
			bIcon = true;
		}
	}

	mapTo = m_mapGiftIb.find(pRole->GetID());
	if (mapTo!=m_mapGiftIb.end())
	{
		if(mapTo->second.empty() ==false)
		{
			bIcon = true;
		}
	}

	PtcM2C_IBGiftIconNtf ntf;
	ntf.m_Data.set_status(bIcon);
	pRole->Send(ntf);
}

void IBGiftMgr::DoTxLog(const std::string& Account, KKSG::PayGiftIbItem* pItem)
{
	if (NULL == pItem)
	{
		return ;
	}
	TPayItem oLog(pItem->billno());
	oLog.m_FromOpenId = Account;
	oLog.m_FromRoleId = pItem->fromroleid();
	oLog.m_Count = pItem->count();
	oLog.m_PayDegree = pItem->paydegree()*10;
	oLog.m_ToRoleId = pItem->toroleid();
	oLog.m_GoodsId = pItem->goodsid();
	oLog.Do();
}

KKSG::PayGiftIbItem* IBGiftMgr::GetGiftIbByOrder(UINT64 nRoleID,const std::string &orderid)
{
	auto iter =m_mapToGiftIb.find(nRoleID);
	if (iter!=m_mapToGiftIb.end())
	{
		for (auto siter = iter->second.begin(); siter!=iter->second.end();siter++)
		{
			if ((*siter)->billno()==orderid)
			{
				return *siter;
			}
		}
	}
	return NULL;
}
