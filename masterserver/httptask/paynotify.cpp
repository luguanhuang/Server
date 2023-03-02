#include "pch.h"
#include "paynotify.h"
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
#include "pay/rpcm2g_paygetbalance.h"
#include "role/rolemanager.h"
#include "network/gslink.h"

PayNotifyTask::PayNotifyTask()
{
	m_rpcId = 0;
	m_paytype = KKSG::PAY_PARAM_NONE;
	m_RoleId = 0;
	m_CostSum = 0;
	SetTimeOut(4000);
	m_strUrl = CPayMgr::Instance()->GetPayUrl();
	CPayMgr::Instance()->UpdateRequestCount();
}

PayNotifyTask::~PayNotifyTask()
{

}

const std::string PayNotifyTask::GetUrl()
{
	std::ostringstream ss;
	ss << m_strUrl << GetBalanceUrlPath << "?";
	std::string sPara =  CSnsSigCheck::Instance()->join_params(m_oparams);
	ss << sPara;
	return ss.str();
}

void PayNotifyTask::OnResult(const std::string &response)
{
	CPayMgr::Instance()->ClearMidasErrCount();
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		OnReply();;
		SSWarn << "parser get_balance_m res failed,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}
	if (!doc.HasMember("ret"))
	{
		SSWarn << "has no ret,roleid=" << m_RoleId << ",res=" << response << END;
		OnReply();
		return;
	}
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	INT32 ret = doc["ret"].GetInt();
	if (ret != 0)
	{
		OnReply();
		SSWarn <<  "return failed,roleid=" << m_RoleId << ",res=" << response << END;
		CPayMgr::Instance()->PrintLog(m_oparams);
		if (ret == 1018 && role)
		{
			CPayMgr::Instance()->PayTokenInvalidNtf(role);
		}
		return;
	}

	if (!doc.HasMember("balance"))
	{
		OnReply();
		SSWarn << "has no balance,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}

	if (!doc.HasMember("save_amt"))
	{
		OnReply();
		SSWarn << "has no save_amt,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}

	if (!doc.HasMember("cost_sum"))
	{
		OnReply();
		SSWarn << "has no cost_sum,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}

	if (!doc.HasMember("tss_list"))
	{
		OnReply();
		SSWarn << "has no save_amt,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}
	const rapidjson::Value& roMember = JsonUtil::GetJsonValue(doc, "tss_list");
	if (!roMember.IsArray())
	{
		OnReply();
		SSWarn << "tss_list not a list,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}
	INT32 iBalance =  JsonUtil::GetJsonInt(doc, "balance");//doc["balance"].GetInt();
	INT32 iSaveAmt = JsonUtil::GetJsonInt(doc, "save_amt"); //doc["save_amt"].GetInt();
	m_CostSum = JsonUtil::GetJsonUint(doc, "cost_sum");
	bool isOther = false;
	RpcM2G_PayGetBalance* rpc = RpcM2G_PayGetBalance::CreateRpc();
	rpc->m_oArg.set_balance(iBalance);
	rpc->m_oArg.set_roleid(m_RoleId);
	rpc->m_oArg.set_saveamt(iSaveAmt);
	rpc->m_oArg.set_paramid(m_paramId);
	rpc->m_oArg.set_costsum(m_CostSum);
	for (UINT32 i = 0; i < roMember.Size(); i ++)
	{
		KKSG::PayTsslist* poList = rpc->m_oArg.add_tsslist();
		std::string productid = JsonUtil::GetJsonString(roMember[i], "inner_productid");
		if (m_paramId == productid)
		{
			isOther = true;
		}
		poList->set_innerproductid(productid);
		poList->set_begintim(JsonUtil::GetJsonString(roMember[i], "begintime"));
		poList->set_endtime(JsonUtil::GetJsonString(roMember[i], "endtime"));
		poList->set_paychan(JsonUtil::GetJsonString(roMember[i], "paychan"));
		poList->set_paysubchan(JsonUtil::GetJsonInt(roMember[i], "paysubchan"));
		poList->set_autopaychan(JsonUtil::GetJsonString(roMember[i], "autopaychan"));
		poList->set_autopaysubchan(JsonUtil::GetJsonInt(roMember[i], "autopaysubchan"));
		poList->set_grandtotalopendays(JsonUtil::GetJsonInt(roMember[i], "grandtotal_opendays"));
		poList->set_grandtotalpresentdays(JsonUtil::GetJsonInt(roMember[i], "grandtotal_presentdays"));
		poList->set_firstbuytime(JsonUtil::GetJsonString(roMember[i], "first_buy_time"));
		poList->set_extend(JsonUtil::GetJsonString(roMember[i], "extend"));
	}
	if (role)
	{
		role->SetCostDiamond(m_CostSum);
		role->SendMsgToGS(*rpc);
		LogInfo("account=%s,roleid=%llu,response=%s", role->GetAccount().c_str(), m_RoleId, response.c_str());
	}
	CPayMgr::Instance()->PayFinish(isOther, m_paramId, m_RoleId, iSaveAmt);
	OnReply();
}
void PayNotifyTask::OnReply()
{
	//虽然失败，也需提示成功;
	if (m_rpcId)
	{
		CUserData data;
		data.m_dwUserData = KKSG::ERR_SUCCESS;
		KKSG::PayDegreeRes oData;
		oData.set_degree(m_CostSum);
		oData.set_error(KKSG::ERR_SUCCESS);
		data.m_pUserPtr = (void*)&oData;
		CRpc::ReplyDelayRpc(m_rpcId, data);
	}
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	if (role)
	{
		role->SetIsPayFirstGetBalance(false);
	}
}

void PayNotifyTask::OnFailed()
{
	SSWarn << "midas error" << END;
	OnReply();
	//与米大师通信失败;
	CPayMgr::Instance()->UpdateMidasErrCount();
}

void PayNotifyTask::SetParamData(KKSG::PayParameterInfo oData)
{
	std::string org_url = "/mpay/get_balance_m";
	std::string strCookie = CPayMgr::Instance()->BuildCookie(oData, org_url);
	SSDebug << "cookie" << END;
	SetHttpCookie(strCookie);
}
