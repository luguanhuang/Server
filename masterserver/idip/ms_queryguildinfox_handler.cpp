#include "pch.h"
#include "ms_commondef_handler.h"
#include "util/jsonutil.h"
#include "config.h"
#include "role/rolemanager.h"
#include "guild/guildmgr.h"
#include "guild/guild.h"
#include "idip/idip.h"

INT32 CMSIdipQueryGuildInfoXHandler::HandlerRequest(const rapidjson::Value& roHeadJson, const rapidjson::Value& roBodyJson, CIdipReply& roReply)
{
	UINT64 uGuildId = JsonUtil::GetJsonUint64(roBodyJson, "GuildId");
	Guild *pGuild = CGuildMgr::Instance()->GetGuild(uGuildId);
	if (pGuild == NULL)
	{
		roReply.m_nErrCode = IDIP_ERR_INVALID_GUILDID;
		roReply.m_strResult = PackErrResponse(roReply.m_nErrCode);
		return IDIP_SERVER_NONE;
	}
	roReply.m_nErrCode = IDIP_ERR_SUCCESS;
	JsonBuilder oBuilder;
	oBuilder.AddMember_Int("Result", roReply.m_nErrCode);
	oBuilder.AddMember_String("GuildId", ToString(uGuildId));
	oBuilder.AddMember_Int("GuildId32", 0);
	oBuilder.AddMember_String("GuildName", UrlEncode(pGuild->GetName()).c_str());
	oBuilder.AddMember_String("LeaderName", UrlEncode(pGuild->GetLeaderName()).c_str());
	oBuilder.AddMember_String("LeaderRoleId", ToString(pGuild->GetLeaderID()).c_str());
	oBuilder.AddMember_Int("Rank", CGuildMgr::Instance()->GetRankNum(GuildSortByPrestige, uGuildId));
	oBuilder.AddMember_Int("Level", pGuild->GetLevel());
	oBuilder.AddMember_Int("EnableCnt", pGuild->GetCapcityCount());
	oBuilder.AddMember_Int("CurMemberCnt", pGuild->GetMemberCount());
	oBuilder.AddMember_String("GuildContent", UrlEncode(pGuild->GetAnnoucement()));
	oBuilder.AddMember_String("GuildDecla", "0");
	oBuilder.AddMember_Int("Money", 0);
	roReply.m_strResult = oBuilder.GetString();

	return IDIP_SERVER_NONE;
}
