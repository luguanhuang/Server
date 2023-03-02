#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

INT32 CMSIdipSendMailHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string	strMailTitle	= JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string	strMailContent	= JsonUtil::GetJsonString(roBodyJson, "MailContent");
	std::vector<ItemDesc> items;
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	if (!CMailMgr::Instance()->SendSystemMail(oData))
	{
		roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
	}
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
