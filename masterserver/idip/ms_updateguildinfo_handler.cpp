#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

INT32 CMSIdipUpdateGuildInfoHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 GuildId = JsonUtil::GetJsonUint64(roBodyJson, "GuildId");
	std::string strMailTitle = JsonUtil::GetJsonString(roBodyJson, "MailTitle");
	std::string strMailContent = JsonUtil::GetJsonString(roBodyJson, "MailContent");
	INT32 Num = JsonUtil::GetJsonInt(roBodyJson, "Num");
	if(Num == 0)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	Guild* pGuild = CGuildMgr::Instance()->GetGuild(GuildId);
	if (NULL == pGuild)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_REQ_NUM;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}

	std::string name;
	if (GetCmd() == IDIP_DO_UPDATE_GUILD_EXP_REQ)
	{
		Num?pGuild->AddExp(Num):pGuild->DelExp(abs(Num));
		name = "UpdateGuildExp";
	}
	else
	{
		Num?pGuild->AddPrestige(Num):pGuild->DelPrestige(abs(Num));
		name = "UpdateGuildPrestige";
	}
	std::vector<ItemDesc> items;
	MailData oData(strMailTitle, strMailContent, items, MAIL_EXPIRE_TIME);
	CMailMgr::Instance()->SendGuildMail(GuildId, oData);

	//√Ù∏–»’÷æ;
	TIdipUpdateGuildInfo oLog(name);
	oLog.m_Num = Num;
	oLog.strMailTitle = strMailTitle;
	oLog.strMailContent = strMailContent;
	oLog.m_GuildID = GuildId;
	oLog.Do(roHeadJson, roBodyJson);

	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
	return IDIP_SERVER_NONE;
}
