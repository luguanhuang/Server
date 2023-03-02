#include "pch.h"
#include "payconsume.h"
#include "config.h"
#include "appid.h"
#include "util/gametime.h"
#include "util.h"
#include "pay/SnsSigCheck.h"
#include "pay/paymgr.h"
#include "util/jsonutil.h"
#include "pay/rpcm2g_payconsumeaddgoods.h"
#include "role/rolemanager.h"
#include "network/gslink.h"
#include "pay/ptcm2c_payparameterinfoinvalidntf.h"
#include "pay/ptcm2g_balanceinfontf.h"
#include "team/teamrequest.h"
#include "pay/ptcm2c_midasexceptionntf.h"
#include "httptask/httptaskmgr.h"
#include "timeutil.h"
#include "idip/idiphandler.h"
#include "idip/idipproc.h"
#include "team/teamtranshandler.h"


PayConsumeTask::PayConsumeTask()
{
	m_RoleId = 0;
	m_rpcid = 0;
	m_isdirectly = false;
	m_isFirstHint = false;
	m_iRet = 0;
	m_strUrl = CPayMgr::Instance()->GetPayUrl();
	CPayMgr::Instance()->UpdateRequestCount();
	m_MidasErrCount = 0;
}

PayConsumeTask::~PayConsumeTask()
{

}

const std::string PayConsumeTask::GetUrl()
{
	std::ostringstream ss;
	ss << m_strUrl << ConsumeUrlPath << "?";
	std::string sPara =  CSnsSigCheck::Instance()->join_params(m_oparams);
	ss << sPara;
	return ss.str();
}

void PayConsumeTask::OnResult(const std::string &response)
{
	CPayMgr::Instance()->ClearMidasErrCount();
	SSDebug << response << END;
	rapidjson::Document doc;
	doc.Parse(response.c_str());
	if (doc.HasParseError())
	{
		SSWarn << "parser get_balance_m res failed,roleid=" << m_RoleId << ",res=" << response << END;
		OnRetFailed();
		return;
	}

	if (!doc.HasMember("ret"))
	{
		SSWarn << "has no ret,roleid=" << m_RoleId << ",res=" << response << END;
		OnRetFailed();
		return;
	}

	m_iRet = doc["ret"].GetInt();
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	INT32 iBalance = -1;
	if (m_iRet == 0)
	{
		if (!doc.HasMember("balance"))
		{
			SSWarn << "has no balance,roleid=" << m_RoleId << ",res=" << response << END;
			return;
		}
		iBalance = JsonUtil::GetJsonInt(doc, "balance");
	}
	if (m_isdirectly && m_rpcid)
	{
		OnReply(m_iRet==0?KKSG::ERR_SUCCESS:KKSG::ERR_FAILED);
		if (m_iRet == 0 && role)
		{	
			RpcM2G_PayConsumeAddGoods* rpc = RpcM2G_PayConsumeAddGoods::CreateRpc();
			rpc->m_oArg.set_roleid(m_RoleId);
			rpc->m_oArg.set_balance(iBalance);
			rpc->m_oArg.mutable_list()->CopyFrom(m_Listinfo);
			role->SendMsgToGS(*rpc);
		}
		return;
	}
	if (m_iRet == 3000111 && role)//需要重试一次;
	{
		SSWarn <<  "return 3000111,already request count= " << m_MidasErrCount << ",roleid=" << m_RoleId << ",res=" << response << END;
		stPayAccessInfo* pData = CPayMgr::Instance()->GetPayAccessInfo(m_RoleId);
		if (NULL != pData)
		{
			if (m_MidasErrCount >= MAX_CHECK3000111_COUNT)
			{
				SSWarn << "already request count=" << m_MidasErrCount << "roleid=" << m_RoleId << END;
				pData->m_oConsumeList.erase(m_strBillNo);
				CPayMgr::Instance()->SaveData(m_RoleId);
				NotifyClient();
				return;
			}
			LogInfo("start request again,roleid=%llu,amt=%d,billno=%s", m_RoleId, m_Listinfo.amt(), m_strBillNo.c_str());
			PayConsumeTask* poTask = new PayConsumeTask();
			poTask->SetRoleId(role->GetID());
			poTask->BuildData(m_Listinfo.amt(), m_strBillNo);
			poTask->SetParamData(role->GetPayParameterInfo());
			poTask->SetBillNo(m_strBillNo);
			poTask->SetListInfo(m_Listinfo);
			poTask->m_MidasErrCount = m_MidasErrCount + 1;
			CPayMgr::Instance()->BuildHttpParam(role, poTask->m_oparams);
			CPayMgr::Instance()->BuildSign(role, poTask->m_oparams, ConsumeUrlPath);
			HttpTaskMgr::Instance()->AddTask(poTask);
			pData->m_uLastCheckConsumeTime = GameTime::GetTime();
		}
		return;
	}
	if (m_iRet != 0 && m_iRet != 1002215)
	{
		//std::string err = doc["err_code"].GetString();
		SSWarn <<  "return failed,roleid=" << m_RoleId << ",res=" << response << END;
		CPayMgr::Instance()->PrintLog(m_oparams);
		OnRetFailed();
		return;
	}
	if (role)//需要重新拉取下余额,修改消费金额的值;
	{
		CPayMgr::Instance()->PayNotify(role, KKSG::PAY_PARAM_LIST, "");
	}
	AddGoods(role, iBalance);
}
void PayConsumeTask::AddGoods(CRole*role, INT32 iBalance)
{
	stPayAccessInfo* pData = CPayMgr::Instance()->GetPayAccessInfo(m_RoleId);
	if (NULL == pData)return;
	auto it = pData->m_oConsumeList.find(m_strBillNo);
	if (it != pData->m_oConsumeList.end())
	{
		it->second.set_status(ConsumeBillStatus_AddItem);
		CPayMgr::Instance()->SaveData(m_RoleId);
		if (role)
		{		
			RpcM2G_PayConsumeAddGoods* rpc = RpcM2G_PayConsumeAddGoods::CreateRpc();
			rpc->m_oArg.set_roleid(m_RoleId);
			rpc->m_oArg.set_balance(iBalance);
			rpc->m_oArg.mutable_list()->CopyFrom(it->second);
			role->SendMsgToGS(*rpc);
		}
		LogInfo("roleid=%llu,balance=%d,amt=%d,billno=%s", m_RoleId, iBalance, it->second.amt(),m_strBillNo.c_str());
	}

}

void PayConsumeTask::OnRetFailed()
{
	//非超时错误直接删除;
	stPayAccessInfo* pData = CPayMgr::Instance()->GetPayAccessInfo(m_RoleId);
	if (pData)
	{
		pData->m_oConsumeList.erase(m_strBillNo);
		CPayMgr::Instance()->SaveData(m_RoleId);
	}
	NotifyClient();
	if (m_isdirectly && m_rpcid)
	{
		OnReply(KKSG::ERR_FAILED);
	}
}
void PayConsumeTask::OnFailed()
{
	//超时错误需要重试,非超时错误直接删除并提示客户端;
	if (!GetTimeOutStatus())
	{
		OnRetFailed();
	}
	else 
	{
		if (m_isdirectly && m_rpcid)
		{
			OnReply(KKSG::ERR_FAILED);
		}
	}
	CPayMgr::Instance()->UpdateMidasErrCount();
}

void PayConsumeTask::NotifyClient()
{
	KKSG::ErrorCode err = KKSG::ERR_MIDAS_FAILED;
	CRole* role = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	if (role)
	{
		if (m_iRet == 1018)
		{
			CPayMgr::Instance()->PayTokenInvalidNtf(role);
		}
		else if (m_iRet == 1004)
		{
			err = KKSG::ERR_MIDAS_BALANCE_NOTENOUTH;
			CPayMgr::Instance()->PayNotify(role, KKSG::PAY_PARAM_LIST, "");
		}
		if (m_isFirstHint)//后面轮询重试的不要一直发;
		{
			PtcM2C_MidasExceptionNtf ptc;
			ptc.m_Data.set_result(err);
			role->Send(ptc);
		}
	}
}

void PayConsumeTask::OnReply(INT32 err)
{
	CUserData data;
	data.m_dwUserData = err;
	data.m_pUserPtr = NULL;
	if (m_rpcid)
	{
		CRpc::ReplyDelayRpc(m_rpcid, data);
	}
}
void PayConsumeTask::BuildData(INT32 iAmt, std::string billno)
{
	m_oparams["amt"] =  ToString(iAmt);
	m_oparams["billno"] = billno;
	m_oparams["accounttype"] = "common";
}

void PayConsumeTask::SetParamData(KKSG::PayParameterInfo oData)
{
	std::string strCookie = CPayMgr::Instance()->BuildCookie(oData, ConsumeUrlPath);
	SetHttpCookie(strCookie);
}

void PayConsumeTeamTask::SetArg(const KKSG::TeamOPArg& roArg)
{
	m_roArg = roArg;
	m_type = 1;
}

void PayConsumeTeamTask::SetArg(const KKSG::TeamTransData& roArg)
{
	m_roData = roArg;
	m_type = 2;
}

void PayConsumeTeamTask::OnReply(INT32 err)
{
	SSInfo << " roleid = " << m_RoleId << " delayid = " << m_rpcid << END;

	CRole* pRole = CRoleManager::Instance()->GetByRoleID(m_RoleId);
	if(NULL == pRole)
	{
		SSWarn << " find role failed roleid = " << m_RoleId << " delayid = " << m_rpcid << END;
		return;
	}
	if(1 == m_type)
	{
		if(KKSG::ERR_SUCCESS != err)	
		{
			TeamOPM2GRes tmpRes;
			tmpRes.mutable_c2mres()->set_result(KKSG::ERR_TEAMCOST_DIAMOND);

			CUserData userData;
			userData.m_pUserPtr = (void*)(&tmpRes);

			CRpc::ReplyDelayRpc(m_rpcid, userData);		 
		}
		else
		{
			TeamRequest tq(pRole);
			tq.TransSendToGs(m_rpcid, m_roArg);
		}
	}
	else if(2 == m_type)
	{
		TeamTransHandler handler;

		if(KKSG::ERR_SUCCESS != err)	
		{
			KKSG::TeamTransRes roRes;
			roRes.mutable_c2mopres()->set_result(KKSG::ERR_TEAMCOST_DIAMOND);
			handler.HandleRoleOpResFromGs(m_rpcid, m_RoleId, m_roData, roRes);
		}
		else
		{
			handler.HandleRoleOpReqFromTs(m_rpcid, pRole, m_roData);
		}
	}
	else
	{
		SSWarn << " payconsumeteamtask type = 0 role = " << m_RoleId << END;
	}
}

PayConsumeIdipTask::PayConsumeIdipTask()
{
	m_dwID = 0;
}

PayConsumeIdipTask::~PayConsumeIdipTask()
{

}

void PayConsumeIdipTask::SetID(UINT32 id)
{
	m_dwID = id;
}

void PayConsumeIdipTask::OnReply(INT32 err)
{
	std::string str;
	CIdipReply oReply(str);
	oReply.m_nErrCode = KKSG::ERR_SUCCESS != err?IDIP_ERR_UNKNOWN:IDIP_ERR_SUCCESS;
	oReply.m_strResult = PackErrResponse(oReply.m_nErrCode);

	CUserData data;
	data.m_dwUserData = oReply.m_nErrCode;
	data.m_pUserPtr = &oReply;
	CIdipProcess::Instance()->OnAsyncReply(GetID(), data);
}
