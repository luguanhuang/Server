#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/rolesummarymgr.h"

INT32 CMSIdipSendPersonMailHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	std::string	strMailTitle	= JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string	strMailContent	= JsonUtil::GetJsonString(roBodyJson, "MailContent");
	std::vector<ItemDesc> items;
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	if (!CMailMgr::Instance()->SendMail(roleid, oData))
	{
		roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
	}
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
