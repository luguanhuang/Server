#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "role/rolesummarymgr.h"
#include "loghelper/tlogr.h"

INT32 CMSIdipXinYueRPayHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string	strMailTitle	= JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string	strMailContent	= JsonUtil::GetJsonString(roBodyJson, "MailContent");
	std::string strOpenId = JsonUtil::GetJsonString(roBodyJson, "OpenId");
	UINT64 roleid = JsonUtil::GetJsonUint64(roBodyJson, "RoleId");
	UINT32 Num = JsonUtil::GetJsonUint(roBodyJson, "Diamond");
	std::string strExtParam = JsonUtil::GetJsonString(roBodyJson, "ExtendParameter");
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	if (MSConfig::Instance()->GetPlatType() != KKSG::PLAT_IOS)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	if (!CRoleSummaryMgr::Instance()->IsValidAcountRole(strOpenId, roleid))
	{
		roReply.m_nErrCode = IDIP_ERR_PLAYER_NOT_EXIST;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	SetRoleID(roleid);
	if (!Num || Num == UINT32(-1))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	std::vector<ItemDesc> items;
	ItemDesc pdesc(DIAMOND, Num);
	items.push_back(pdesc);
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	oData.SetReason(ItemFlow_IDIP, ItemFlow_IdipXinYueRPay);
	oData.SetExtParam(strExtParam);
	if (!CMailMgr::Instance()->SendMail(roleid, oData))
	{
		roReply.m_nErrCode = IDIP_ERR_SEND_MAIL;
	}
	else
	{
		TIdipXinYueRPay oLog("IdipXinYueRRecharge");
		oLog.m_RoleId = roleid;
		oLog.m_strOpenId = strOpenId;
		oLog.m_strMailTitle = strMailTitle;
		oLog.m_strMailContent = strMailContent;
		oLog.m_strExtParam = strExtParam;
		oLog.m_Num = Num;
		oLog.Do(roHeadJson, roBodyJson);
	}
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
