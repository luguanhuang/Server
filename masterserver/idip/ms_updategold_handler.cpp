#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"
#include "table/globalconfig.h"
#include "mail/mailconfig.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(UpdateGold)
INT32 CMSIdipUpdateGoldHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	m_nNum	= JsonUtil::GetJsonInt64(roBodyJson, "Num");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if(m_nNum == 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	const MailConf* pConf = MailConfig::Instance()->GetMailConf(MailConf_IDIP_COMPENSATION_AWARD);
	if(roBodyJson.HasMember("MailTitle"))
	{
		szMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	}
	else
	{
		szMailTitle = pConf?pConf->m_title:"";
	}

	std::string strMailContent;
	if (roBodyJson.HasMember("MailContent"))
	{
		szMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");
	}
	else
	{
		szMailContent = pConf?pConf->m_content:"";
	}
	std::vector<ItemDesc> items;
	ItemDesc pdesc(GOLD, (INT32)m_nNum);
	items.push_back(pdesc);
	MailData oData(szMailTitle, szMailContent, items, MAIL_EXPIRE_TIME);
	oData.SetReason(ItemFlow_IDIP_Oper);

	if (GetCmd() == IDIP_DO_GM_UPDATE_GOLD_REQ)
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

	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(szOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	if (m_nNum > 0)//添加的话，直接邮件
	{
		if (GetCmd() == IDIP_AQ_DO_UPDATE_GOLD_COIN_REQ && m_nNum > 100000)////安全侧的修改上限为10W;
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
	if (NULL == pRole)//不在线ms读取数据扣除;
	{
		return IDIP_SERVER_MS;
	}
	return IDIP_SERVER_GS;
}
//修改数据;
bool CMSIdipUpdateGoldHandler::ModifyMsData(CUserData& roUserData)
{
	//修改离线数据;
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag data(pInfo);
	data.ReduceVirtualItem(GOLD, (m_nNum));
	data.Save();

	return true;
}
void CMSIdipUpdateGoldHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	if (GetCmd() == IDIP_DO_GM_UPDATE_GOLD_REQ)
	{
		TIdipDoGmMoneyLog oLog("IdipDoGmUpdateGoldFlow");
		oLog.m_nNum = m_nNum;
		oLog.Do(doc["head"], doc["body"]);
	}
	else
	{
		std::string strTableName = GetCmd()==IDIP_AQ_DO_UPDATE_GOLD_COIN_REQ?"IdipAqDoUpdateGoldFlow":"IdipUpdateGoldFlow";
		TIdipMoneyLog oLog(strTableName);
		oLog.m_uRoleId = GetRoleID();
		oLog.m_nNum = m_nNum;
		oLog.szOpenId = szOpenId;
		oLog.Do(doc["head"], doc["body"]);
	}
}

