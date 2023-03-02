#include "pch.h"
#include "paybuygoods.h"
#include "config.h"
#include "util.h"
#include "pay/paymgr.h"
#include "util/jsonutil.h"
#include "pay/SnsSigCheck.h"
#include "role/rolemanager.h"
#include "crpc.h"
#include "pay/ibgiftmgr.h"

PayBuyGoodsTask::PayBuyGoodsTask()
{
	m_RoleId = 0;
	m_rpcId = 0;
	m_Ret = -1;
	m_strUrl = MSConfig::Instance()->GetBuyGoodsUrl();
	//CPayMgr::Instance()->UpdateRequestCount();
	SetTimeOut(4000);
}

PayBuyGoodsTask::~PayBuyGoodsTask()
{

}

const std::string PayBuyGoodsTask::GetUrl()
{
	std::ostringstream ss;
	ss << m_strUrl << BuyGoodsUrlPath << "?";
	std::string sPara =  CSnsSigCheck::Instance()->join_params(m_oparams);
	ss << sPara;
	return ss.str();
}

void PayBuyGoodsTask::OnResult(const std::string &response)
{
	LogInfo("roleid=%llu,response=%s", m_RoleId, response.c_str());
	//CPayMgr::Instance()->ClearMidasErrCount();
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser pay buy_goods res failed, roleid=" << m_RoleId << ",res=" << response << END;
		OnReply();
		return;
	}

	if (!doc.HasMember("ret"))
	{
		SSWarn << "has no ret,roleid=" << m_RoleId << ",res=" << response << END;
		OnReply();
		return;
	}
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	m_Ret = doc["ret"].GetInt();
	if (m_Ret != 0)
	{
		if (m_Ret == 1018)
		{
			CPayMgr::Instance()->PayTokenInvalidNtf(role);
		}
		OnReply();
		CPayMgr::Instance()->PrintLog(m_oparams);
		SSWarn << "ret error,roleid=" << m_RoleId << ",res=" << response << END;
		return;
	}
	if (!doc.HasMember("token"))
	{
		SSWarn << "has no token,roleid=" << m_RoleId << ",res=" << response << END;
		OnReply();
		return;
	}

	if (!doc.HasMember("url_params"))
	{
		SSWarn << "has no url_params,roleid=" << m_RoleId << ",res=" << response << END;
		OnReply();
		return;
	}
	std::string token = JsonUtil::GetJsonString(doc, "token");
	std::string urlparams = JsonUtil::GetJsonString(doc, "url_params");
	IBGiftMgr::Instance()->AddNewIbItem(m_Data);

	CUserData data;
	data.m_dwUserData = KKSG::ERR_SUCCESS;
	KKSG::PayFriendItemRes* pData = new KKSG::PayFriendItemRes();
	pData->set_ret(KKSG::ERR_SUCCESS);
	pData->set_token(token);
	pData->set_url_param(urlparams);
	pData->set_billno(m_Data.billno());
	data.m_pUserPtr = (void*)pData;
	CRpc::ReplyDelayRpc(m_rpcId, data);
}

void PayBuyGoodsTask::OnFailed()
{
	OnReply();
	//CPayMgr::Instance()->UpdateMidasErrCount();
}

void PayBuyGoodsTask::SetParamData(KKSG::PayParameterInfo oData)
{
	std::string strCookie = CPayMgr::Instance()->BuildCookie(oData, BuyGoodsUrlPath);
	SetHttpCookie(strCookie);
}

void PayBuyGoodsTask::SetPayItem(KKSG::PayGiftIbItem oItem, UINT32 price)
{
	std::ostringstream ss;
	ss << oItem.goodsid() << "*" << price << "*" << oItem.count();
	std::string strPayItem = ss.str();
	UINT32 amt = price * oItem.count();
	m_oparams["payitem"] = strPayItem;
	m_oparams["amt"] = ToString(amt);
	m_oparams["goodsurl"] = "";
	m_oparams["appmode"] = "1";
	m_Data = oItem;

}
void PayBuyGoodsTask::SetGoodsMeta(UINT64 roleid, const std::string& name, const std::string& desc)
{
	std::ostringstream ss;
	ss << name << "*" << desc;
	std::string strGoodsMeta;
	strGoodsMeta = ss.str();
	m_oparams["goodsmeta"] = strGoodsMeta;
	m_RoleId = roleid;
}

void PayBuyGoodsTask::OnReply()
{
	CUserData data;
	data.m_dwUserData = m_Ret==0?KKSG::ERR_SUCCESS:KKSG::ERR_MIDAS_FAILED;
	data.m_pUserPtr = NULL;
	CRpc::ReplyDelayRpc(m_rpcId, data);
}
