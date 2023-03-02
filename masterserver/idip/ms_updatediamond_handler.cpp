#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "table/globalconfig.h"
#include "mail/mailconfig.h"
#include "httptask/payconsume.h"
#include "pay/paymgr.h"
#include "httptask/httptaskmgr.h"

IDIP_HANDLER_MODIFY_IMPLEMETION_EXCEPT_GSREPLAY(UpdateDiamond)
IDIP_REPLY_FROMGS_IMPLEMETION(UpdateDiamond)
INT32 CMSIdipUpdateDiamondHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	if (GetCmd() != IDIP_AQ_DO_UPDATE_DIAMOND_REQ)
	{
		m_nNum	= JsonUtil::GetJsonInt64(roBodyJson, "DiamondNum");
	}
	else
	{
		m_nNum	= JsonUtil::GetJsonInt(roBodyJson, "Num");
	}
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if(m_nNum == 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_IDIP_COMPENSATION_AWARD);
	std::string strMailTitle;
	if(roBodyJson.HasMember("MailTitle"))
	{
		strMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	}
	else
	{
		strMailTitle = pConf?pConf->m_title:"";
	}

	std::string strMailContent;
	if (roBodyJson.HasMember("MailContent"))
	{
		strMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");
	}
	else
	{
		strMailContent = pConf?pConf->m_content:"";
	}
	std::vector<ItemDesc> items;
	ItemDesc pdesc(DIAMOND, INT32(m_nNum));
	items.push_back(pdesc);
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	oData.SetReason(ItemFlow_IDIP_Oper);
	if (GetCmd() == IDIP_DO_GM_UPDATE_DIAMOND_REQ) //管理端;
	{
		if(m_nNum < 0)
		{
			roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}

		if (!CMailMgr::Instance()->SendSystemMail(oData))
		{
			roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
		}
		else
		{
			IdipLog();
		}
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);

	if (m_nNum > 0)//添加的话，直接邮件;
	{
		if (GetCmd() == IDIP_AQ_DO_UPDATE_DIAMOND_REQ && m_nNum > 5000)//安全侧的修改上限为5000;
		{
			roReply.m_nErrCode = IDIP_ERR_MAX_UPDATE_NUM;
			roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
			return IDIP_SERVER_NONE;
		}
		if (!CMailMgr::Instance()->SendMail(roleid, oData))
		{
			roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
		}
		else
		{
			IdipLog();
		}
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(roleid);
	if (NULL == pRole)//不在线,米大师扣除不了，直接返回错误
	{
		roReply.m_nErrCode = IDIP_ERR_ROLE_OFFLINE;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	return IDIP_SERVER_OTHER;
}
//修改数据
bool CMSIdipUpdateDiamondHandler::ModifyMsData(CUserData& roUserData)
{
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag data(pInfo);
	data.ReduceVirtualItem(DIAMOND, (m_nNum));
	data.Save();
	return true;
}
void CMSIdipUpdateDiamondHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	if (GetCmd() == IDIP_DO_GM_UPDATE_DIAMOND_REQ)
	{
		TIdipDoGmMoneyLog oLog("IdipDoGmUpdateDiamondFlow");
		oLog.m_nNum = m_nNum;
		oLog.Do(doc["head"], doc["body"]);
	}
	else
	{
		std::string strTableName = GetCmd()==IDIP_AQ_DO_UPDATE_DIAMOND_REQ?"IdipUpdateDiamondFlow":"IdipUpdateDiamondFlow";
		TIdipMoneyLog oLog(strTableName);
		oLog.m_uRoleId = GetRoleID();
		oLog.m_nNum = m_nNum;
		oLog.szOpenId = szOpenId;
		oLog.Do(doc["head"], doc["body"]);
	}
}

bool CMSIdipUpdateDiamondHandler::DoOther(CIdipReply& roReply)
{
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	CRole* pRole = CRoleManager::Instance()->GetByRoleID(GetRoleID());
	if (NULL == pRole)
	{
		roReply.m_nErrCode = IDIP_ERR_ROLE_OFFLINE;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return false;
	}
	INT64 num = m_nNum*(-1);
	std::string billno = TLogMgr::Instance()->GetTagString();
	KKSG::Payconsumelist info;
	info.set_amt((INT32)num);
	info.set_billno(billno);
	info.set_reason(ItemFlow_IDIP);
	info.set_subreason(ItemFlow_IDIP_Oper);
	PayConsumeIdipTask* poTask = new PayConsumeIdipTask();
	poTask->SetRoleId(pRole->GetID());
	poTask->BuildData(INT32(num), billno);
	poTask->SetParamData(pRole->GetPayParameterInfo());
	poTask->SetBillNo(billno);
	poTask->SetRpcID(GetRpcID());
	poTask->SetIsDirectly(true);
	poTask->SetListInfo(info);
	poTask->SetID(GetID());
	CPayMgr::Instance()->BuildHttpParam(pRole, poTask->m_oparams);
	CPayMgr::Instance()->BuildSign(pRole, poTask->m_oparams, ConsumeUrlPath);
	HttpTaskMgr::Instance()->AddTask(poTask);
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return true;
}

bool CMSIdipUpdateDiamondHandler::OnAsyncReplyFromOther(const CUserData& roUserData)
{
	CRpc::ReplyDelayRpc(GetRpcID(), roUserData);
	if (roUserData.m_pUserPtr != NULL && (KKSG::ErrorCode)roUserData.m_dwUserData == ERR_SUCCESS)\
	{
		IdipLog();
	}
	return true;
}
