#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "util/XCommon.h"
#include "tsssdk/MsTssHandler.h"

INT32 CMSIdipUpdateGuildNameHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 GuildId = JsonUtil::GetJsonUint64(roBodyJson, "GuildId");
	std::string strGuildName = JsonUtil::GetJsonString(roBodyJson, "GuildName");
	std::string strMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string strMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(GuildId);
	if (NULL == pGuild || strGuildName.empty())
	{
		roReply.m_nErrCode = IDIP_ERR_UNKNOWN;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (XCommon::ValidNameCheck(strGuildName) != KKSG::ERR_SUCCESS)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (!CGuildMgr::Instance()->IsNameNotUsed(strGuildName))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	if (CTssHandler::Instance()->ValidNameCheck(strGuildName) != KKSG::ERR_SUCCESS)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	
	if (!CGuildMgr::Instance()->ModifyName(pGuild, pGuild->GetName(), strGuildName))
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_PARA;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	std::vector<ItemDesc> items;
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendGuildMail(pGuild->GetID(), oData);

	SSInfo << "guildid=" << pGuild->GetID() << ",oldname=" << pGuild->GetName() << ",newname=" << strGuildName << END;
	pGuild->SetName(strGuildName);
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
