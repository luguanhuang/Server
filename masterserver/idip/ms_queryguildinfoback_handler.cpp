#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"

INT32 CMSIdipQueryGuildInfoBackHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	std::string strName = JsonUtil::GetJsonString(roBodyJson, "GuildName");
	Guild* pGuild =  CGuildMgr::Instance()->GetGuildByName(strName);
	if (NULL == pGuild)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	UINT64 GuildId = 0;
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);

	oBuilder.AddMember_String("GuildName", UrlEncode(strName));
	oBuilder.AddMember_String("GuildId", ToString(pGuild->GetID()));
	oBuilder.AddMember_Int("GuildId32", 0);
	roReply.m_strResult = oBuilder.GetString();

	return IDIP_SERVER_NONE;
}
