#include "pch.h"
#include "paysend.h"
#include "config.h"
#include "crpc.h"
#include <time.h>
#include "idip/idip.h"
#include "pay/SnsSigCheck.h"
#include "appid.h"
#include "util/gametime.h"
#include "util.h"
#include "pay/paymgr.h"
#include "util/jsonutil.h"
#include "pay/ptcg2m_payadddiamondntf.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "pay/ptcm2g_balanceinfontf.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"

PaySendTask::PaySendTask()
{
	m_strUrl = CPayMgr::Instance()->GetPayUrl();
	m_RoleId = 0;
	CPayMgr::Instance()->UpdateRequestCount();
}

PaySendTask::~PaySendTask()
{

}

const std::string PaySendTask::GetUrl()
{
	std::ostringstream ss;
	ss << m_strUrl << SendUrlPath << "?";
	std::string sPara =  CSnsSigCheck::Instance()->join_params(m_oparams);
	ss << sPara;
	return ss.str();
}

void PaySendTask::BuildData(INT32 iAmt, std::string billno)
{
	m_oparams["presenttimes"] = ToString(iAmt);
	m_oparams["billno"] = billno;
	strBillNo = billno;
}

void PaySendTask::OnResult(const std::string &response)
{
	CPayMgr::Instance()->ClearMidasErrCount();
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser get_balance_m res failed, roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}

	if (!doc.HasMember("ret"))
	{
		SSWarn << "has no ret,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}
	stPayAccessInfo* pData = CPayMgr::Instance()->GetPayAccessInfo(m_RoleId);
	if (NULL == pData)return;
	INT32 ret = doc["ret"].GetInt();
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	if (ret != 0 && ret != 1002215)
	{
		SSWarn <<  "return failed,roleid=" << m_RoleId << ",res=" << response << END;
		CPayMgr::Instance()->PrintLog(m_oparams);
		if (ret == 1018 && role)
		{
			CPayMgr::Instance()->PayTokenInvalidNtf(role);
		}
		return;
	}
	auto it = pData->m_oSendList.find(strBillNo);
	if (it != pData->m_oSendList.end())
	{
		if (ret == 0)
		{
			if (!doc.HasMember("balance"))
			{
				SSWarn << "has no balance,roleid=" << m_RoleId << ",res=" << response << END;
				return;
			}
			INT32 iBalance = JsonUtil::GetJsonInt(doc, "balance");;
			PtcM2G_BalanceInfoNtf ptc;
			ptc.m_Data.set_roleid(m_RoleId);
			ptc.m_Data.set_balance(iBalance);
			ptc.m_Data.mutable_list()->CopyFrom(it->second);
			if (role)
			{
				role->SendMsgToGS(ptc);
			}
			LogInfo("roleid=%llu,balance=%d,amt=%d,billno=%s", m_RoleId, iBalance, it->second.amt(), strBillNo.c_str());
		}
		else if (ret == 1002215 && role)//已经操作的订单返回中没有余额字段，需要重新拉取下余额;
		{
			CPayMgr::Instance()->PayNotify(role, KKSG::PAY_PARAM_LIST, "");
		}
		pData->m_oSendList.erase(it);
		CPayMgr::Instance()->SaveData(m_RoleId);
	}

}

void PaySendTask::OnFailed()
{
	//与米大师通信失败;
	CPayMgr::Instance()->UpdateMidasErrCount();
}

void PaySendTask::SetParamData(KKSG::PayParameterInfo oData)
{
	std::string strCookie = CPayMgr::Instance()->BuildCookie(oData, SendUrlPath);
	SetHttpCookie(strCookie);
}
