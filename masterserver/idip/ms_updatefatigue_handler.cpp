#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/bag.h"

IDIP_HANDLER_MODIFY_IMPLEMETION(UpdateFatigue)
INT32 CMSIdipUpdateFatigueHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	m_nNum	= JsonUtil::GetJsonInt64(roBodyJson, "Num");
	szOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if(m_nNum == 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	std::string strMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string strMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");
	std::vector<ItemDesc> items;
	ItemDesc pdesc(FATIGUE, (INT32)m_nNum);
	items.push_back(pdesc);
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	oData.SetReason(ItemFlow_IDIP_Oper);
	if (GetCmd() == IDIP_DO_GM_UPDATE_PHYSICAL_REQ)
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
	if (m_nNum > 0)//添加的话，直接邮件
	{
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
//修改数据
bool CMSIdipUpdateFatigueHandler::ModifyMsData(CUserData& roUserData)
{
	//修改离线数据
	RoleAllInfo* pInfo = (RoleAllInfo*)roUserData.m_pUserPtr;
	CBag data(pInfo);
	data.ReduceVirtualItem(FATIGUE, (m_nNum));
	data.Save();
	return true;
}
void CMSIdipUpdateFatigueHandler::IdipLog()
{
	rapidjson::Document doc;
	std::string s = GetStrJson();
	if (doc.Parse<0>(GetStrJson().c_str()).HasParseError())
	{
		return ;
	}
	if (GetCmd() == IDIP_DO_GM_UPDATE_PHYSICAL_REQ)
	{
		TIdipDoGmMoneyLog oLog("IdipDoGmUpdateFatigueFlow");
		oLog.m_nNum = m_nNum;
		oLog.Do(doc["head"], doc["body"]);

	}
	else
	{
		TIdipMoneyLog oLog("IdipUpdateFatigueFlow");
		oLog.m_uRoleId = GetRoleID();
		oLog.m_nNum = m_nNum;
		oLog.szOpenId = szOpenId;
		oLog.Do(doc["head"], doc["body"]);
	}
}

