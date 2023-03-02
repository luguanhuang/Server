#include "pch.h"
#include "db/mysqlmgr.h"
#include "util.h"
#include "timeutil.h"
#include <time.h>
#include "paymgr.h"
#include "SnsSigCheck.h"
#include "config.h"
#include "appid.h"
#include "loghelper/tlogr.h"
#include "httptask/paysend.h"
#include "httptask/httptaskmgr.h"
#include "pay/rpcm2d_savepayaccessdata.h"
#include "network/dblink.h"
#include "role/rolemanager.h"
#include "httptask/payconsume.h"
#include "idip/idip.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"
#include "network/gslink.h"
#include "httptask/paynotify.h"
#include "define/tlogenum.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"
#include "util/timespecificmgr.h"
#include "notice/notice.h"
#include "util/XCommon.h"
#include "event/eventmgr.h"
#include "pay/ptcm2g_paytotalntf.h"

void stPayAccessInfo::ToKKSG(KKSG::PayAccessInfo* pAccessinfo) const
{
	pAccessinfo->Clear();
	pAccessinfo->set_roleid(m_uRoleID);
	pAccessinfo->set_saveamt(m_iSaveamt);
	pAccessinfo->set_nosaveamt(m_iNoSaveAmt);
	for (auto i = m_oConsumeList.begin(); i != m_oConsumeList.end(); i ++)
	{
		KKSG::Payconsumelist* pConlist =  pAccessinfo->mutable_consumeinfo()->add_consume();
		pConlist->CopyFrom(i->second);
	}

	for (auto i = m_oSendList.begin(); i != m_oSendList.end(); i ++)
	{
		KKSG::Paysendlist* pSendList = pAccessinfo->mutable_sendinfo()->add_send();
		pSendList->CopyFrom(i->second);
	}

	for (UINT32 i = 0; i < m_oOtherList.size(); i ++)
	{
		KKSG::Payotherlist* pOtherList = pAccessinfo->mutable_otherinfo()->add_other();
		pOtherList->CopyFrom(m_oOtherList[i]);
	}
}

void stPayAccessInfo::ToThis(const KKSG::PayAccessInfo& oInfo)
{
	m_uRoleID = oInfo.roleid();
	m_iSaveamt = oInfo.saveamt();
	m_iNoSaveAmt = oInfo.nosaveamt();
	for (INT32 i = 0; i < oInfo.otherinfo().other_size(); i ++)
	{
		m_oOtherList.push_back(oInfo.otherinfo().other(i));
	}

	for (INT32 i = 0; i < oInfo.consumeinfo().consume_size(); i ++)
	{
		m_oConsumeList.insert(std::make_pair(oInfo.consumeinfo().consume(i).billno(),oInfo.consumeinfo().consume(i)));
	}

	for (INT32 i = 0; i < oInfo.sendinfo().send_size(); i ++)
	{
		m_oSendList.insert(std::make_pair(oInfo.sendinfo().send(i).billno(), oInfo.sendinfo().send(i)));
	}
}


INSTANCE_SINGLETON(CPayMgr);

CPayMgr::CPayMgr()
{
	m_ErrCount = 0;
	m_bIsMaster = true;
	m_RequestCount = 0;
	m_lastUpdateTime = 0;
	m_LastCheckRetryOverTime = 0;
	m_IsGMPayCheckRole = false;
}

CPayMgr::~CPayMgr()
{

}

bool CPayMgr::Init()
{
	if (!ReadAllPayFromDB())return false;
	m_TimeHandler = CTimerMgr::Instance()->SetTimer(this, 0, 1000, -1, __FILE__, __LINE__);
	LoadTimer();
	return true;
}

void CPayMgr::Uninit()
{
	CTimerMgr::Instance()->KillTimer(m_TimeHandler);
	ClearTimers();
}

void CPayMgr::OnTimer(UINT32 dwID, UINT32 dwCount)
{
	// by huangds, 2018/5/16
	CheckRecharge();
	
	for (auto it = m_oPayAccessInfo.begin(); it != m_oPayAccessInfo.end(); it ++)
	{
		CRole* role = CRoleManager::Instance()->GetByRoleID(it->second.m_uRoleID);
		if (role && !role->GetPayParameterInfo(true).pf().empty())
		{
			UpdatePay(&it->second, role);
			UpdateConsume(&it->second, role);
			UpdateSend(&it->second, role);
		}
	}

	if (!XCommon::IsDaySame(GameTime::GetTime(), m_lastUpdateTime))
	{
		LoadTimer();
		m_lastUpdateTime = GameTime::GetTime();
	}
	if ((m_LastCheckRetryOverTime + CHECK_RETRY_INTERVAL_TIME) < (UINT32)GameTime::GetTime())
	{
		CheckPayRetryOver();
		m_LastCheckRetryOverTime = (UINT32)GameTime::GetTime();
	}
}

// by huangds, 2018/5/16
void CPayMgr::CheckRecharge()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	std::stringstream ss;
	ss << "select id, roleid from pay_charge_notify";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table pay_charge_notify failed, errorMsg: %s", poMySqlConn->GetError());
		return;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;
		
		UINT64 id = 0;
		UINT64 roleID = 0;
		
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			id = convert<UINT64>(pVal);
		}
		if (poRes->GetFieldValue(1, &pVal, dwLen))
		{
			roleID = convert<UINT64>(pVal);
		}
		
		bool fDelete = false;
		// 只有角色在线才能获取CRole
		CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleID);
		if (pRole)
		{
			fDelete = true;
			PayNotify(pRole, KKSG::PAY_PARAM_LIST, ""); //查询余额
		}
		else
		{
			std::stringstream ssRole;
			ssRole << "select count(*) from role where _id=" << roleID;
			IMysqlRecordSet* poRes2 = NULL;
			if(poMySqlConn->QueryWithRecord(ssRole.str().c_str(), ssRole.str().size(), &poRes2))
			{
				if (poRes2->FetchRow() && poRes2->GetFieldValue(0, &pVal, dwLen)) 
				{
					fDelete = (convert<INT32>(pVal) == 0);
				} 
				poRes2->Release();
			}
			else
			{
				LogError("Query table role failed, errorMsg: %s", poMySqlConn->GetError());
			}
		}
		
		if (fDelete)
		{
			std::stringstream ssDelete;
			ssDelete << "delete from pay_charge_notify where id=" << id;
			if(!poMySqlConn->Query(ssDelete.str().c_str(), ssDelete.str().size(), NULL))
			{
				LogError("Delete pay_charge_notify row failed, errorMsg: %s", poMySqlConn->GetError());
			}
		}
	}
	
	poRes->Release();
}

void CPayMgr::PayNotify(CRole* role, KKSG::PayParamType paytype, std::string paramid)
{
	PayNotifyTask* pTask = new PayNotifyTask();
	pTask->SetParamData(role->GetPayParameterInfo());
	pTask->SetRoleId(role->GetID());
	pTask->SetParamType(paytype);
	pTask->SetParamId(paramid);
	BuildHttpParam(role, pTask->m_oparams);
	BuildSign(role, pTask->m_oparams, GetBalanceUrlPath);
	HttpTaskMgr::Instance()->AddTask(pTask);
}
void CPayMgr::PayConsume(stPayAccessInfo* pData, CRole* pRole)
{
	for (auto it = pData->m_oConsumeList.begin(); it != pData->m_oConsumeList.end(); it ++)
	{
		if (it->second.status() == ConsumeBillStatus_Midas)
		{
			PayConsumeTask* poTask = new PayConsumeTask();
			poTask->SetRoleId(pData->m_uRoleID);
			poTask->BuildData(it->second.amt(), it->second.billno());
			poTask->SetParamData(pRole->GetPayParameterInfo());
			poTask->SetBillNo(it->second.billno());
			poTask->SetListInfo(it->second);
			BuildHttpParam(pRole, poTask->m_oparams);
			BuildSign(pRole, poTask->m_oparams, ConsumeUrlPath);
			HttpTaskMgr::Instance()->AddTask(poTask);
		}
		else if (it->second.status() == ConsumeBillStatus_AddItem)
		{
			PayNotify(pRole, KKSG::PAY_PARAM_LIST, "");//拉取下余额
			RpcM2G_PayConsumeAddGoods* rpc = RpcM2G_PayConsumeAddGoods::CreateRpc();
			rpc->m_oArg.set_roleid(pRole->GetID());
			rpc->m_oArg.set_balance(-1);
			rpc->m_oArg.mutable_list()->CopyFrom(it->second);
			GSLink::Instance()->SendToLine(pRole->GetGsLine(), *rpc);
		}
	}
	pData->m_iConsumeRetryCount ++;
	pData->m_uLastCheckConsumeTime = INT32(GameTime::GetTime());
}
void CPayMgr::PaySend(stPayAccessInfo* pData, CRole* pRole)
{
	for (auto it = pData->m_oSendList.begin(); it != pData->m_oSendList.end(); it++)
	{
		PayPresentM(pRole, it->second);
	}
	pData->m_iSendRetryCount ++;
	pData->m_uLastCheckSendTime = INT32(GameTime::GetTime());
}
void CPayMgr::UpdatePay(stPayAccessInfo* pData, CRole* pRole)
{
	if (NULL == pData || NULL == pRole || pRole->GetLoginType() == KKSG::LOGIN_PASSWORD)
	{
		return;
	}
	if (pRole->GetIsPayFirstGetBalance())//正在拉取的话，不要走定时器拉取;
	{
		return;
	}
	INT32 iCheck = 0;
	if (pData->m_iNoSaveAmt == 0 && pData->m_oOtherList.size() == 0)
	{
		if ((pData->m_uLastCheckPayTime + NormalGetBalanceTime) <= INT32(GameTime::GetTime()))
		{
			PayNotify(pRole, KKSG::PAY_PARAM_LIST, "");//常规检查查询余额，10分钟检查一次;
			pData->m_uLastCheckPayTime = INT32(GameTime::GetTime());
		}
	}
	else
	{

		if ((pData->m_uLastCheckPayTime + GetPayCheckIntervalTime(pData->m_iRetryCount)) <= INT32(GameTime::GetTime()))
		{
			//订阅服务未到账查询;
			for (UINT32 i = 0; i < pData->m_oOtherList.size(); i ++)
			{
				PayNotify(pRole, KKSG::PAY_PARAM_NONE, pData->m_oOtherList[i].paramid());
			}
			//充值未到帐;
			if (pData->m_iNoSaveAmt)
			{
				PayNotify(pRole, KKSG::PAY_PARAM_LIST, "");
			}
			pData->m_iRetryCount ++;
			pData->m_uLastCheckPayTime = INT32(GameTime::GetTime());
		}
	}
}

void CPayMgr::UpdateConsume(stPayAccessInfo* pData, CRole* pRole)
{
	if (NULL == pData || NULL == pRole || pRole->GetLoginType() == KKSG::LOGIN_PASSWORD)
	{
		return;
	}
	INT32 iTime = GetPayCheckIntervalTime(pData->m_iConsumeRetryCount);
	if (iTime < MIN_CHECK_CONSUME_TIME)
	{
		iTime = MIN_CHECK_CONSUME_TIME;
	}
	if ((pData->m_uLastCheckConsumeTime + iTime) > INT32(GameTime::GetTime()))
	{
		return;
	}
	PayConsume(pData, pRole);
}

void CPayMgr::UpdateSend(stPayAccessInfo* pData, CRole* pRole)
{
	if (NULL == pData || NULL == pRole || pRole->GetLoginType() == KKSG::LOGIN_PASSWORD)
	{
		return;
	}
	INT32 iTime = GetPayCheckIntervalTime(pData->m_iSendRetryCount);
	if (iTime < MIN_CHECK_SEND_TIME)
	{
		iTime = MIN_CHECK_SEND_TIME;
	}
	if ((pData->m_uLastCheckSendTime + iTime) > INT32(GameTime::GetTime()))
	{
		return;
	}
	PaySend(pData, pRole);
}

bool CPayMgr::ReadAllPayFromDB()
{
	IMysqlConn* poMySqlConn = CMysqlMgr::Instance()->GetMysqConn();
	UINT64 qwBegin = TimeUtil::GetMilliSecond();

	std::stringstream ss;
	ss << "select roleid,saveamt,nosaveamt,payotherlist,consumelist,sendlist from pay";
	IMysqlRecordSet* poRes = NULL;
	if(!poMySqlConn->QueryWithRecord(ss.str().c_str(), ss.str().size(), &poRes))
	{
		LogError("Read table pay failed, errorMsg: %s", poMySqlConn->GetError());
		return false;
	}

	while(poRes->FetchRow())
	{
		char* pVal = NULL;
		UINT32 dwLen = 0;
		UINT64 roleID = 0;
		if (poRes->GetFieldValue(0, &pVal, dwLen))
		{
			roleID = convert<UINT64>(pVal);
		}
		else
		{
			LogError("Read 'roleid' column failed");
			return false;
		}

		INT32 iSaveamt = 0;
		if (poRes->GetFieldValue(1, &pVal, dwLen))
		{
			iSaveamt = convert<INT32>(pVal);
		}
		else
		{
			LogError("Read 'saveamt' column failed");
			return false;
		}

		INT32 iNosaveamt = 0;
		if (poRes->GetFieldValue(2, &pVal, dwLen))
		{
			iNosaveamt = convert<INT32>(pVal);
		}
		else
		{
			LogError("Read 'nosaveamt' column failed");
			return false;
		}

		std::string strPayOther;
		if (poRes->GetFieldValue(3, &pVal, dwLen))
		{
			strPayOther.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'payotherlist' column failed");
			return false;
		}

		std::string strPayConsume;
		if (poRes->GetFieldValue(4, &pVal, dwLen))
		{
			strPayConsume.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'consumelist' column failed");
			return false;
		}

		std::string strPaySend;
		if (poRes->GetFieldValue(5, &pVal, dwLen))
		{
			strPaySend.assign(pVal, dwLen);
		}
		else
		{
			LogError("Read 'sendlist' column failed");
			return false;
		}


		KKSG::PayAccessInfo oData;
		oData.set_roleid(roleID);
		oData.set_saveamt(iSaveamt);
		oData.set_nosaveamt(iNosaveamt);
		
		KKSG::PayotherInfo other;
		if (!strPayOther.empty() && other.ParseFromString(strPayOther))
		{
			oData.mutable_otherinfo()->CopyFrom(other);
		}

		KKSG::PayConsumeInfo consume;
		if (!strPayConsume.empty() && consume.ParseFromString(strPayConsume))
		{
			oData.mutable_consumeinfo()->CopyFrom(consume);
		}

		KKSG::PaySendInfo send;
		if (!strPaySend.empty() && send.ParseFromString(strPaySend))
		{
			oData.mutable_sendinfo()->CopyFrom(send);
		}
		stPayAccessInfo oInfo;
		oInfo.ToThis(oData);
		m_oPayAccessInfo.insert(std::make_pair(roleID, oInfo));
	}

	poRes->Release();
	UINT64 qwEnd = TimeUtil::GetMilliSecond();
	UINT64 qwTimeUsed = (qwEnd >= qwBegin ? qwEnd - qwBegin : 0);
	LogInfo("Read all punish data from db use time: %llu ms", qwTimeUsed);
	return true;
}
void CPayMgr::BuildHttpParam(CRole* role, std::map<std::string, std::string> &params, INT32 paytype)
{
	KKSG::PayParameterInfo roInfo = role->GetPayParameterInfo(true);
	params["openid"] = role->GetAccount();
	params["openkey"] = roInfo.openkey();
	params["appid"] = GetMidasPayAppid(role->GetLoginPlatID(), paytype);//MSConfig::Instance()->GetPlatType()==KKSG::PLAT_ANDROID?MidasAndroidAppid:MidasIosAppid;
	params["ts"] = ToString((UINT32)time(NULL));
	params["pf"] = roInfo.pf();
	params["pfkey"] = roInfo.pfkey();
	std::ostringstream ss;
	ss << role->GetServerId();
	if (paytype != PayTypeBuyGoods)
	{
		ss  << "_" << role->GetID();
	}
	params["zoneid"] = ss.str();
	params["userip"] = role->GetIP();
	params["format"] = "json";

}

void CPayMgr::BuildSign(CRole* pRole, std::map<std::string, std::string> &params, std::string url, INT32 paytype)
{
	std::string sMethod = "GET";
	std::string appkey = GetMidasPayKey(pRole->GetLoginPlatID(), paytype);
	//SSWarn << "method=" << sMethod << " url_path=" << url << " secret=" << appkey << END; 
	
	// by huangds. 2018/5/16
	//std::string strSig = CSnsSigCheck::Instance()->makeSig(sMethod, url, params, appkey);
	std::string strSig = CSnsSigCheck::Instance()->makeSig2(params);
	
	params["sig"] = strSig;
	/*
	for (auto i = params.begin(); i != params.end(); i ++)
	{
		SSDebug << "key=" << i->first << "  value=" << i->second << END;
	}

	std::string sPara =  CSnsSigCheck::Instance()->join_params(params);
	SSDebug << "?" << sPara << END;
	*/
}

std::string CPayMgr::BuildCookie(KKSG::PayParameterInfo oData, std::string org_loc)
{
	std::map<std::string, std::string> param;
	param["session_id"] = oData.sessionid();
	param["session_type"] = oData.sessiontype();
#if defined(WIN32) || defined(_WIN64)
	param["org_loc"] = UrlEncode(org_loc);
#else
	param["org_loc"] = CSnsSigCheck::Instance()->url_encode(org_loc);
#endif
	std::string strCookie = CSnsSigCheck::Instance()->makeCookie(param);
	SSDebug << strCookie << END;
	return strCookie;
}

bool CPayMgr::AddDiamond(UINT64 roleid, INT32 iCount, INT32 reason, INT32 subreason)
{
	 KKSG::Paysendlist list;
	 list.set_amt(iCount);
	 list.set_reason(reason);
	 list.set_subreason(subreason);
	 list.set_billno(TLogMgr::Instance()->GetTagString());
	 list.set_ts(INT32(GameTime::GetTime()));
	 return AddDiamond(roleid, list);
}

bool CPayMgr::AddDiamond(UINT64 roleid, KKSG::Paysendlist list)
{
	LogInfo("roleid=%llu begin add money=%d,billno=%s,reason=%d,subreason=%d,extparam=%s request", roleid, list.amt(), list.billno().c_str(), list.reason(), list.subreason(), list.extparam().c_str());
	std::string strBillNo = TLogMgr::Instance()->GetTagString();
	auto it = m_oPayAccessInfo.find(roleid);
	if (it == m_oPayAccessInfo.end())
	{
		stPayAccessInfo oData;
		oData.m_uRoleID = roleid;
		oData.m_oSendList.insert(std::make_pair(list.billno(), list));
		m_oPayAccessInfo.insert(std::make_pair(roleid, oData));
	}
	else
	{
		it->second.m_uLastCheckSendTime = GameTime::GetTime();
		it->second.m_iSendRetryCount = 0;
		it->second.m_oSendList.insert(std::make_pair(list.billno(), list));
	}
	SaveData(roleid);
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (role)
	{
		PayPresentM(role, list);
	}

	if (list.reason() == ItemFlow_IDIP && list.subreason() == ItemFlow_IdipXinYueRPay)
	{
		EventMgr::Instance()->AddEvent(roleid, MIX_EVENT_OFFSET+EVENT_MIDAS_PAY_DEGREE, list.amt());
	}
	return true;
}

stPayAccessInfo* CPayMgr::GetPayAccessInfo(UINT64 roleid)
{
	auto it = m_oPayAccessInfo.find(roleid);
	if (it == m_oPayAccessInfo.end())
	{
		stPayAccessInfo oData;
		oData.m_uRoleID = roleid;
		m_oPayAccessInfo.insert(std::make_pair(roleid, oData));
	}
	return &m_oPayAccessInfo[roleid];
}

void CPayMgr::SaveData(UINT64 roleid, INT32 op)
{
	stPayAccessInfo* poInfo = GetPayAccessInfo(roleid);
	if (poInfo)
	{
		SaveData2DB(roleid, *poInfo, op);
	}
}
void CPayMgr::SaveData2DB(UINT64 roleid, stPayAccessInfo oInfo, INT32 op)
{
	KKSG::PayAccessInfo oData;
	oInfo.ToKKSG(&oData);

	std::string strOther;
	if (!oData.otherinfo().SerializeToString(&strOther))
	{
		LogError("other info SerializeToString fail");
		return;
	}
	std::string strConsume;
	if (!oData.consumeinfo().SerializeToString(&strConsume))
	{
		LogError("consume info SerializeToString fail");
		return;
	}
	std::string strSend;
	if (!oData.sendinfo().SerializeToString(&strSend))
	{
		LogError("send info SerializeToString fail");
		return;
	}
	RpcM2D_SavePayAccessData* rpc = RpcM2D_SavePayAccessData::CreateRpc();
	rpc->m_oArg.set_roleid(roleid);
	rpc->m_oArg.set_saveamt(oInfo.m_iSaveamt);
	rpc->m_oArg.set_nosaveamt(oInfo.m_iNoSaveAmt);
	rpc->m_oArg.set_other(strOther);
	rpc->m_oArg.set_consume(strConsume);
	rpc->m_oArg.set_send(strSend);
	rpc->m_oArg.set_op(op);
	DBLink::Instance()->SendTo(*rpc);
}

void CPayMgr::SaveConsumeDataOnReply(UINT64 roleid, KKSG::Payconsumelist list, bool isSuccess)
{
	stPayAccessInfo* pData = GetPayAccessInfo(roleid);
	if (NULL == pData)
	{
		return ;
	}
	pData->m_uLastCheckConsumeTime = GameTime::GetTime();
	auto it = pData->m_oConsumeList.find(list.billno());
	if (it == pData->m_oConsumeList.end())
	{
		return ;
	}
	if (!isSuccess)
	{
		pData->m_oConsumeList.erase(it);
		SaveData(roleid);
		return;
	}
	pData->m_iConsumeRetryCount = 0;
	//开始请求midas扣款;
	CRole* role = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == role)
	{
		return ;
	}
	PayConsumeTask* poTask = new PayConsumeTask();
	poTask->SetRoleId(role->GetID());
	poTask->BuildData(list.amt(), list.billno());
	poTask->SetParamData(role->GetPayParameterInfo());
	poTask->SetBillNo(list.billno());
	poTask->SetListInfo(list);
	poTask->SetFirstHint(true);
	BuildHttpParam(role, poTask->m_oparams);
	BuildSign(role, poTask->m_oparams, ConsumeUrlPath);
	HttpTaskMgr::Instance()->AddTask(poTask);
}
void CPayMgr::PayBegin(KKSG::PayParamType paytype, std::string paramid, INT32 amount, UINT64 roleid)
{
	stPayAccessInfo* pData = GetPayAccessInfo(roleid);
	if (NULL == pData)
	{
		return ;
	}
	if (paytype == KKSG::PAY_PARAM_LIST)
	{
		pData->m_iNoSaveAmt += INT32(amount/10);
	}
	else
	{
		KKSG::Payotherlist olist;
		olist.set_ts(UINT32(GameTime::GetTime()));
		olist.set_amount(INT32(amount));
		olist.set_paramid(paramid);
		pData->m_oOtherList.push_back(olist);
	}
	pData->m_iRetryCount = 0;
	pData->m_uLastCheckPayTime = INT32(GameTime::GetTime());
	SaveData(roleid);
}

void CPayMgr::PayFinish(bool isOther,  std::string paramid, UINT64 roleid, INT32 saveamt)
{
	stPayAccessInfo* pData = GetPayAccessInfo(roleid);
	if (NULL == pData)
	{
		return ;
	}
	INT32 iNoSaveAmt = 0;
	if (saveamt < pData->m_iSaveamt)
	{
		LogWarn("midas data error,roleid=%llu ,midas_saveamt=%d,self_saveamt=%d", roleid, saveamt, pData->m_iSaveamt);
	}
	else
	{
		iNoSaveAmt = saveamt-pData->m_iSaveamt;
	}

	if (pData->m_iSaveamt != saveamt)
	{
		if (!iNoSaveAmt)
		{
			pData->m_iNoSaveAmt = 0;
		}
		else
		{
			pData->m_iNoSaveAmt = pData->m_iNoSaveAmt>iNoSaveAmt?pData->m_iNoSaveAmt-iNoSaveAmt:0;
		}
		pData->m_iSaveamt = saveamt;
		SaveData(roleid);
	}
}

void CPayMgr::ConsumeFinish(UINT64 roleid, INT32 iBalance, std::string billno)
{
	stPayAccessInfo* pData = GetPayAccessInfo(roleid);
	if (NULL == pData)
	{
		return;
	}
	auto it = pData->m_oConsumeList.find(billno);
	if (it == pData->m_oConsumeList.end())
	{
		return;
	}
	pData->m_oConsumeList.erase(it);
	SaveData(roleid);
}

void CPayMgr::TeamCost(CRole* role, INT32 rpcid, INT32 iCount, const KKSG::TeamOPArg& roArg)
{
	PayConsumeTeamTask* poTask = TeamCost(role, rpcid, iCount);
	if(poTask)
	{
		poTask->SetArg(roArg);
	}
}

void CPayMgr::TeamCost(CRole* role, INT32 rpcid, INT32 iCount, const KKSG::TeamTransData& roArg)
{
	PayConsumeTeamTask* poTask = TeamCost(role, rpcid, iCount);
	if(poTask)
	{
		poTask->SetArg(roArg);
	}
}

PayConsumeTeamTask* CPayMgr::TeamCost(CRole* role, INT32 rpcid, INT32 iCount)
{
	if (NULL == role)return NULL;

	SSInfo << " role = " << role->GetID() << " rpcid = " << rpcid << " count = " << iCount << END;

	std::string billno = TLogMgr::Instance()->GetTagString();
	KKSG::Payconsumelist info;
	info.set_amt(iCount);
	info.set_billno(billno);
	info.set_reason(ItemFlow_TeamCost);
	info.set_subreason(ItemFlow_TeamCostTake);
	PayConsumeTeamTask* poTask = new PayConsumeTeamTask();
	//poTask->SetArg(roArg);
	poTask->SetRoleId(role->GetID());
	poTask->BuildData(iCount, billno);
	poTask->SetParamData(role->GetPayParameterInfo());
	poTask->SetBillNo(billno);
	poTask->SetRpcID(rpcid);
	poTask->SetIsDirectly(true);
	poTask->SetListInfo(info);
	BuildHttpParam(role, poTask->m_oparams);
	BuildSign(role, poTask->m_oparams, ConsumeUrlPath);
	HttpTaskMgr::Instance()->AddTask(poTask);
	return poTask;
}
bool CPayMgr::isFinishConsume(stPayAccessInfo* pData, INT32 type, INT32 index)
{
	if (NULL == pData)
	{
		return false;
	}
	for (auto i = pData->m_oConsumeList.begin(); i != pData->m_oConsumeList.end(); i ++)
	{
		if (i->second.type() == type && i->second.index() == index)
		{
			return false;
		}
	}
	return true;
}
//根据次数来判断重试时间间隔;
INT32 CPayMgr::GetPayCheckIntervalTime(INT32 iCount)
{
	if (iCount >= 0 && iCount <= 2)
	{
		return 2;
	}
	if (iCount == 3)
	{
		return 4;
	}

	if (iCount >=4 && iCount <= 10)
	{
		return GetBalanceTime;
	}

	if (iCount >= 11 && iCount <= 30)
	{
		return GetBalanceTime * 3;
	}

	return GetBalanceTime * 20;
}

void CPayMgr::OnLogin(CRole* role)
{
	PayNotify(role, KKSG::PAY_PARAM_LIST, "");
	stPayAccessInfo* pData = GetPayAccessInfo(role->GetID());
	if (pData)
	{
		PayConsume(pData, role);
		PaySend(pData, role);
	}
}

void CPayMgr::UpdateMidasErrCount()
{
	m_ErrCount++;
	if (m_ErrCount >= MSConfig::Instance()->GetPayErrNum())
	{
		m_ErrCount = 0;
		bool isStatus = m_bIsMaster?false:true;
		if (!MSConfig::Instance()->GetMidasPayUrl(isStatus).empty())
		{
			m_bIsMaster = isStatus;
			SSWarn << "request midas failed,start change url to link=" << GetPayUrl() << END;
		}
	}
}

std::string CPayMgr::GetPayUrl()
{
	if (MSConfig::Instance()->GetPayDelayTest())
	{
		if (GameTime::GetTime()%3)
		{
			return "";
		}
	}
	std::string url = MSConfig::Instance()->GetMidasPayUrl(m_bIsMaster);
	//SSWarn << "errcount=" << m_ErrCount << "url=" << url << END;
	return MSConfig::Instance()->GetMidasPayUrl(m_bIsMaster);
}

void CPayMgr::PrintLog(std::map<std::string, std::string> &params)
{
	for (auto i = params.begin(); i != params.end(); i ++)
	{
		SSWarn << "key=" << i->first << "  value=" << i->second << END;
	}
}

void CPayMgr::PayTokenInvalidNtf(CRole* role)
{
	if (role == NULL)
	{
		return;
	}
	if ((role->GetLastPayTokenTime() + CHECK_PAY_TOKEN_TIME) < GameTime::GetTime())
	{
		PtcM2C_PayParameterInfoInvalidNtf ptc;
		role->Send(ptc);
		role->SetLastPayTokenTime(GameTime::GetTime());
		SSWarn << "roleid=" << role->GetID() << ",token invalid,token=" << role->GetPayParameterInfo().openkey() << END;
	}
}

time_t CPayMgr::GetGrowthFundCloseTime()
{
	return (MSConfig::Instance()->GetGameServerOpenTimeStamp() + GetGlobalConfig().GrowthFundBuyDays * XCommon::GetOneDayTime());
}

void CPayMgr::ClearTimers()
{
	for (auto it = m_timers.begin(); it != m_timers.end(); ++it)
	{
		CTimeSpecificMgr::Instance()->UnRegisterEvent(*it);
	}
	m_timers.clear();
}

void CPayMgr::LoadTimer()
{
	ClearTimers();
	INT32 now = GameTime::GetTime();
	INT32 closetime = INT32(GetGrowthFundCloseTime());
	if (closetime > now && (now + GetGlobalConfig().GrowthFundExpireNoticeDays * XCommon::GetOneDayTime()) >= closetime)
	{
		for (UINT32 i = 0; i < GetGlobalConfig().GrowthFundExpireNoticeTime.size(); ++ i)
		{
			UINT32 time = GetGlobalConfig().GrowthFundExpireNoticeTime[i];
			UINT32 min = time % 100;
			UINT32 hour = time / 100;
			if (min > 59 || hour > 23)
			{
				SSError << "error config,invalid time " << time << END;
				continue;
			}
			TimeSpecific timespec(hour, min, 0);
			UINT32 timerId = CTimeSpecificMgr::Instance()->RegisterEvent(timespec, std::bind(&CPayMgr::GrowthFundNotice, this));
			m_timers.push_back(timerId);
			SSInfo << "growthfund close notice time=" << time << END;
		}
	}
}

bool CPayMgr::GrowthFundNotice()
{
	LogInfo("NoticeGrowthFundNotice");
	Notice notice(NoticeGrowthFundNotice);
	INT32 closetime = INT32(GetGrowthFundCloseTime());
	INT32 now = INT32(GameTime::GetTime());
	INT32 num = closetime - (now - now%3600);
	notice.Replace("$N", ToString(num/3600));
	notice.Send();
	return true;
}

UINT32 CPayMgr::GetRequestCount(bool isClear)
{
	UINT32 uCount = m_RequestCount;
	if (isClear)
	{
		m_RequestCount = 0;
	}
	return uCount;
}

void CPayMgr::UpdateRequestCount()
{
	m_RequestCount ++;
}

std::string CPayMgr::GetMidasPayKey(INT32 plattype, INT32 paytype)
{
	std::string appkey;
	if (MSConfig::Instance()->GetPayDebug())
	{
		if (paytype == PayTypeBuyGoods)
		{
			appkey = plattype==KKSG::PLAT_ANDROID?PayBuyGoodsAppKeyTestAnd:PayBuyGoodsAppKeyTestIos;
		}
		else if (paytype == PayTypeNormal)
		{
			appkey = plattype==KKSG::PLAT_ANDROID?PayAppKeyTestAnd:PayAppKeyTestIos;
		}
		else
		{
			appkey =  XinYueRPayAppkeyTestIos;
		}
	}
	else
	{
		if (paytype == PayTypeBuyGoods)
		{
			appkey = plattype==KKSG::PLAT_ANDROID?PayBuyGoodsAppKeyAnd:PayBuyGoodsAppKeyIos;
		}
		else if (paytype == PayTypeNormal)
		{
			appkey = plattype==KKSG::PLAT_ANDROID?PayAppKeyAnd:PayAppKeyIos;
		}
		else
		{
			appkey =  XinYueRPayAppkeyIos;
		}
	}
	appkey = appkey + "&";
	return appkey;
}


std::string CPayMgr::GetMidasPayAppid(INT32 plattype, INT32 paytype)
{
	std::string appid;
	if (paytype == PayTypeBuyGoods)
	{
		appid = plattype==KKSG::PLAT_ANDROID?PayBuyGoodsAndroidAppid:PayBuyGoodsIosAppid;
	}
	else if (paytype == PayTypeNormal)
	{
		appid = plattype==KKSG::PLAT_ANDROID?MidasAndroidAppid:MidasIosAppid;
	}
	else
	{
		appid = XinYueRPayIosAppid;
	}
	return appid;
}

void CPayMgr::PayOtherFinsih(std::string paramid, UINT64 roleid)
{
	stPayAccessInfo* pData = GetPayAccessInfo(roleid);
	if (NULL == pData)
	{
		return ;
	}
	bool isSave = false;
	KKSG::Payotherlist olist;
	auto i = std::find_if(pData->m_oOtherList.begin(), pData->m_oOtherList.end(), EqualPayParamID(paramid));
	if (i != pData->m_oOtherList.end())
	{
		pData->m_oOtherList.erase(i);
		SaveData(roleid);
	}
}
void CPayMgr::CheckPayRetryOver()
{
	for (auto it = m_oPayAccessInfo.begin(); it != m_oPayAccessInfo.end(); it ++)
	{
		bool isSave = false;
		for (auto i = it->second.m_oOtherList.begin(); i != it->second.m_oOtherList.end();)
		{
			LogDebug("roleid=%llu,ts=%d,paraid=%s", it->first, i->ts(), i->paramid().c_str());
			if ((i->ts() + MAX_RETRY_EXPIRE_TIME) <  (UINT32)GameTime::GetTime())
			{
				i = it->second.m_oOtherList.erase(i);
				isSave = true;
			}
			else
			{
				i ++;
			}
		}
		if (it->second.m_iRetryCount > MAX_PAY_RETRYCOUNT && it->second.m_iNoSaveAmt)
		{
			it->second.m_iNoSaveAmt = 0;
			isSave = true;
		}
		if (isSave)
		{
			SaveData(it->first);
		}
	}
}

bool CPayMgr::CheckIsXinYuePay(PaySendTask* poTask, const KKSG::Paysendlist& data)
{
	if (NULL == poTask)
	{
		return false;
	}
	if (MSConfig::Instance()->GetPlatType() != KKSG::PLAT_IOS)
	{
		return false;
	}
	if (data.reason() == ItemFlow_IDIP && data.subreason() == ItemFlow_IdipXinYueRPay)
	{
		poTask->m_oparams["appid"] = XinYueRPayIosAppid;
		poTask->m_oparams["extendparameter"] = data.extparam();
		return true;
	}
	return false;
}

void CPayMgr::PayPresentM(CRole* pRole, const KKSG::Paysendlist& list)
{
	if (NULL == pRole)
	{
		return ;
	}
	PaySendTask* poTask = new PaySendTask();
	poTask->SetRoleId(pRole->GetID());
	poTask->SetParamData(pRole->GetPayParameterInfo());
	poTask->BuildData(list.amt(), list.billno());
	INT32 type = CheckIsXinYuePay(poTask, list)?PayTypeXinYueR:PayTypeNormal;
	BuildHttpParam(pRole, poTask->m_oparams, type);
	BuildSign(pRole, poTask->m_oparams, SendUrlPath, type);
	HttpTaskMgr::Instance()->AddTask(poTask);
}

UINT32 CPayMgr::GetSaveAmt(UINT64 roleid)
{
	auto it = m_oPayAccessInfo.find(roleid);
	if (it == m_oPayAccessInfo.end())
	{
		return 0;
	}
	return it->second.m_iSaveamt;
}
